
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "db.h"
#include "keyboardlayout.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lblWarning->hide();
    ui->lblMessages->hide();
    ui->txtLanguage1->installEventFilter(this);
    ui->txtLanguage2->installEventFilter(this);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    connect(ui->edtLanguage1, &QLineEdit::textEdited, this, &MainWindow::currentLanguageChanged);
    connect(ui->edtLanguage2, &QLineEdit::textEdited, this, &MainWindow::currentLanguageChanged);
    connect(&backend, &Backend::categoriesUpdated, this, &MainWindow::updateCategories);
    connect(&backend, &Backend::currentCategoryChanged, this, &MainWindow::currentCategoryChanged);
    connect(&backend, &Backend::newVocable, this, &MainWindow::setVocable);
#else
    connect(ui->edtLanguage1, SIGNAL(textEdited(QString)), this, SLOT(currentLanguageChanged(QString)));
    connect(ui->edtLanguage2, SIGNAL(textEdited(QString)), this, SLOT(currentLanguageChanged(QString)));
    connect(&backend, SIGNAL(categoriesUpdated(CategoriesPtr)), this, SLOT(updateCategories(CategoriesPtr)));
    connect(&backend, SIGNAL(currentCategoryChanged(CategoryPtr)), this, SLOT(currentCategoryChanged(CategoryPtr)));
    connect(&backend, SIGNAL(newVocable(Vocable*)), this, SLOT(setVocable(Vocable*)));
#endif

    QList<KeyboardLayout::LanguageInfo> lang = KeyboardLayout::languages();
    foreach(KeyboardLayout::LanguageInfo nfo, lang) {
        ui->cmbKeyboardLayout1->addItem(nfo.name, nfo.code);
        ui->cmbKeyboardLayout2->addItem(nfo.name, nfo.code);
    }
    initLatexWebView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateCategories(CategoriesPtr cats)
{
    ui->cmbEnterCategory->clear();
    foreach(CategoryPtr cat, cats){
        ui->cmbEnterCategory->addItem(cat->categoryName(), QVariant::fromValue(cat));
    }
    int currentIndex = ui->cmbEnterCategory->count()-1;
    ui->cmbEnterCategory->addItem("<Neue Kategorie>", false);
    ui->cmbEnterCategory->setCurrentIndex(currentIndex);

    backend.setCurrentCategory( ui->cmbEnterCategory->itemData( currentIndex ).value<CategoryPtr>() );
}

void MainWindow::setVocable(Vocable *voc)
{
    ui->grpAnswers->setEnabled(voc != NULL);
    ui->chkInvers->setEnabled(voc != NULL);
    ui->txtQuestion->clear();
    ui->txtAnswer->clear();
    if (voc) {
        bool invers = ui->chkInvers->isChecked();
        ui->txtQuestion->setText( invers?voc->language2:voc->language1 );
    }
    else
        ui->lblMessages->show();
}

void MainWindow::startLearning()
{
    backend.prepareTrainingSet();
}

void MainWindow::currentCategoryChanged(CategoryPtr cat)
{
    bool valid = !cat.isNull() && cat->isValid();
    ui->splitterEnterVocabs->setEnabled(valid);
    ui->btnCategoryAdd->setDisabled(valid);
    ui->edtLanguage1->clear();
    ui->edtLanguage2->clear();
    ui->cmbKeyboardLayout1->setCurrentIndex(0);
    ui->cmbKeyboardLayout2->setCurrentIndex(0);
    if (!valid) {
        ui->edtLanguage1->setFocus();
        ui->lstVocables->setModel(NULL);
        return;
    }
    ui->edtLanguage1->setText(cat->languageFrom());
    ui->edtLanguage2->setText(cat->languageTo());
    for(int i=0;i<ui->cmbKeyboardLayout1->count();i++)
        if (ui->cmbKeyboardLayout1->itemData(i).toInt() == cat->keyboardLayoutFrom())
            ui->cmbKeyboardLayout1->setCurrentIndex(i);
    for(int i=0;i<ui->cmbKeyboardLayout2->count();i++)
        if (ui->cmbKeyboardLayout2->itemData(i).toInt() == cat->keyboardLayoutTo())
            ui->cmbKeyboardLayout2->setCurrentIndex(i);
    ui->lstVocables->setModel( backend.currentVocabularyModel());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    connect(ui->lstVocables->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::vocableSelectionChanged);
#else
    connect(ui->lstVocables->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(vocableSelectionChanged(QModelIndex,QModelIndex)));
#endif
}

void MainWindow::currentLanguageChanged(QString)
{
    QString languageFrom = ui->edtLanguage1->text();
    QString languageTo = ui->edtLanguage2->text();

    CategoryPtr cat = backend.currentCategory();
    if (!cat.isNull() && cat->isValid()){
        if (!languageFrom.isEmpty())
            cat->setLanguageFrom(languageFrom);
        if (!languageTo.isEmpty())
            cat->setLanguageTo(languageTo);
        ui->cmbEnterCategory->setItemText(ui->cmbEnterCategory->currentIndex(), cat->categoryName() );
    }
    else{
        bool nameValid=(!languageFrom.isEmpty() && !languageTo.isEmpty());
        ui->btnCategoryAdd->setEnabled(nameValid);
    }

}

void MainWindow::on_btnCategoryAdd_clicked()
{
    QString languageFrom = ui->edtLanguage1->text();
    QString languageTo = ui->edtLanguage2->text();
    int layout1 = ui->cmbKeyboardLayout1->itemData(ui->cmbKeyboardLayout1->currentIndex()).toInt();
    int layout2 = ui->cmbKeyboardLayout2->itemData(ui->cmbKeyboardLayout2->currentIndex()).toInt();

    backend.addCategory(languageFrom, languageTo, layout1, layout2);
}

void MainWindow::on_cmbEnterCategory_currentIndexChanged(int index)
{
    bool validItem = (index>=0);
    bool newItem = ui->cmbEnterCategory->itemData(index).type() == QVariant::Bool;
    ui->btnCategoryRemove->setEnabled(validItem && !newItem);
    ui->txtLanguage1->clear();
    ui->txtLanguage2->clear();

    if(validItem && !newItem)
        backend.setCurrentCategory( ui->cmbEnterCategory->itemData(index).value<CategoryPtr>() );
    if(newItem)
        backend.setCurrentCategory(CategoryPtr());
}

void MainWindow::on_btnCategoryRemove_clicked()
{
    CategoryPtr cat = ui->cmbEnterCategory->itemData(ui->cmbEnterCategory->currentIndex()).value<CategoryPtr>();
    backend.removeCategory( cat );
}

void MainWindow::on_btnQuestionSave_clicked()
{
    if (ui->txtLanguage1->toPlainText().isEmpty() || ui->txtLanguage2->toPlainText().isEmpty())
    {
        ui->lblWarning->setText(tr("Die Eingabefelder für %1 und %2 dürfen nicht leer sein.")
                                .arg(backend.currentCategory()->languageFrom()).arg(backend.currentCategory()->languageTo()));
        ui->lblWarning->show();
        QTimer::singleShot(5000, ui->lblWarning, SLOT(hide()));
        return;
    }
    if (ui->lstVocables->selectionModel()->currentIndex().isValid()) {
        backend.updateVocable( ui->lstVocables->selectionModel()->currentIndex(),
                               ui->txtLanguage1->toPlainText(),
                               ui->txtLanguage2->toPlainText(),
                               ui->spnLektion->value());
    }
    else
        backend.addVocable(ui->txtLanguage1->toPlainText(),
                           ui->txtLanguage2->toPlainText(),
                           ui->spnLektion->value());
    ui->txtLanguage1->clear();
    ui->txtLanguage2->clear();
}

void MainWindow::on_btnQuestionNew_clicked()
{
    ui->lstVocables->selectionModel()->blockSignals(true);
    ui->lstVocables->selectionModel()->clear();
    ui->lstVocables->selectionModel()->blockSignals(false);
    on_btnQuestionSave_clicked();
}

void MainWindow::on_btnQuestionDelete_clicked()
{
    backend.removeVocable( ui->lstVocables->currentIndex() );
    ui->txtLanguage1->clear();
    ui->txtLanguage2->clear();
}

void MainWindow::on_btnShowAnswer_clicked()
{
    if (backend.currentVocable() == NULL)
        return;
    ui->txtAnswer->setText( backend.currentVocable()->language2 );
}

void MainWindow::setEditButtonsStateEnabled(bool enabled)
{
    ui->btnQuestionDelete->setEnabled(enabled);
    ui->btnQuestionNew->setEnabled(enabled);
}

void MainWindow::vocableSelectionChanged(const QModelIndex &current, const QModelIndex &previous) {
    Q_UNUSED(previous);
    ui->txtLanguage1->clear();
    ui->txtLanguage2->clear();
    ui->spnLektion->clear();
    setEditButtonsStateEnabled(current.isValid());
    if (!current.isValid())
        return;
    int row = current.row();
    ui->txtLanguage1->setText( current.model()->index(row,0).data().toString() );
    ui->txtLanguage2->setText( current.model()->index(row,1).data().toString()  );
    ui->spnLektion->setValue( current.model()->index(row,2).data().toInt()  );
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    Q_UNUSED(index)
    if (ui->tabWidget->currentWidget() == ui->tabLearn) {
        startLearning();
    }
}

void MainWindow::on_btnStay_clicked()
{
    backend.currentVocable()->stay();
    backend.showNextVocable();
}

void MainWindow::on_btnBackTo0_clicked()
{
    backend.currentVocable()->backTo0();
    backend.showNextVocable();
}

void MainWindow::on_btnNext_clicked()
{
    backend.currentVocable()->next();
    backend.showNextVocable();
}

void MainWindow::on_btnBack_clicked()
{
    backend.currentVocable()->back();
    backend.showNextVocable();
}

void MainWindow::on_cmbKeyboardLayout1_activated(const QString &)
{
    if (backend.currentCategory().isNull())
        return;
    int code = ui->cmbKeyboardLayout1->itemData(ui->cmbKeyboardLayout1->currentIndex()).toInt();
    backend.currentCategory()->setKeyboardLayoutFrom(code);
}

void MainWindow::on_cmbKeyboardLayout2_activated(const QString &)
{
    if (backend.currentCategory().isNull())
        return;
    int code = ui->cmbKeyboardLayout2->itemData(ui->cmbKeyboardLayout2->currentIndex()).toInt();
    backend.currentCategory()->setKeyboardLayoutTo(code);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if ((obj == ui->txtLanguage1 || obj == ui->txtLanguage2) &&
            (event->type() == QEvent::FocusIn ||
             event->type() == QEvent::FocusOut)) {
        QFocusEvent *evt = static_cast<QFocusEvent*>(event);
        if (evt->lostFocus())
            KeyboardLayout::restore();
        else if (obj == ui->txtLanguage1)
            KeyboardLayout::setActiveKeyboardLayout(backend.currentCategory()->keyboardLayoutFrom());
        else if (obj == ui->txtLanguage2)
            KeyboardLayout::setActiveKeyboardLayout(backend.currentCategory()->keyboardLayoutTo());
    }
    return QObject::eventFilter(obj, event);
}


void MainWindow::initLatexWebView()
{
    QFile f(":/html/MathJax/MathJax.html");
    if (!f.open(QIODevice::ReadOnly))
        qWarning() << "unable to read: " << f.fileName();
    QByteArray html=f.readAll();
    //ui->webView->setHtml(html, QUrl("qrc:/html/MathJax/"));
}

void MainWindow::on_chkShowLatex_clicked(bool showLatex)
{
    ui->stackedWidget->setCurrentIndex(showLatex?1:0);
    if (showLatex) {
        on_txtLanguage2_textChanged();
    }
}

void MainWindow::on_txtLanguage2_textChanged()
{
    if (!ui->chkShowLatex->isChecked())
        return;
    // \\[ g\frac{d^2u}{dx^2} + L\\sin u = 0 \\]
    //  \[ g\frac{d^2u}{dx^2} + L\sin u = 0 \]
    // \left( \sum_{k=1}^n a_k b_k \right)^2 \leq \left( \sum_{k=1}^n a_k^2 \right) \left( \sum_{k=1}^n b_k^2 \right)
    QString js = "try {document.getElementById('MathInput').value='"+ ui->txtLanguage2->toPlainText() +"'; Preview.Update();} catch (e) {console.log(e);}";
    qDebug() << js;
    ui->webView->page()->currentFrame()->evaluateJavaScript(js);
}

