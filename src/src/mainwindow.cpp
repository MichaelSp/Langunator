
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
    ui->edtLanguage1->installEventFilter(this);
    ui->edtLanguage2->installEventFilter(this);

    ui->txtLanguage1->setBackend(backend,true);
    ui->txtLanguage2->setBackend(backend,false);

    ui->lstVocables->horizontalHeader()->resizeSection(1,50);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    connect(&backend, &Backend::categoriesUpdated, this, &MainWindow::updateCategories);
    connect(&backend, &Backend::currentCategoryChanged, this, &MainWindow::currentCategoryChanged);
    connect(&backend, &Backend::newVocable, this, &MainWindow::setVocable);
#else
    connect(&backend, SIGNAL(categoriesUpdated(CategoriesPtr)), this, SLOT(updateCategories(CategoriesPtr)));
    connect(&backend, SIGNAL(currentCategoryChanged(CategoryPtr)), this, SLOT(currentCategoryChanged(CategoryPtr)));
    connect(&backend, SIGNAL(newVocable(Vocable*)), this, SLOT(setVocable(Vocable*)));
#endif

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
    bool hasEntries = ui->cmbEnterCategory->count();
    ui->btnCategoryRemove->setEnabled( hasEntries);
    //ui->btnShare->setEnabled( hasEntries );
    ui->btnCategoreRename->setEnabled(hasEntries);

    if (hasEntries)
        backend.setCurrentCategory( ui->cmbEnterCategory->itemData( ui->cmbEnterCategory->currentIndex() ).value<CategoryPtr>() );
}

void MainWindow::setVocable(Vocable *voc)
{
    ui->grpAnswers->setEnabled(voc != NULL);
    ui->chkInvers->setEnabled(voc != NULL);
    ui->txtQuestion->clear();
    ui->txtAnswer->clear();
    if (voc) {
        bool invers = ui->chkInvers->isChecked();
        ui->lblQuestionLangFrom->setText(invers?voc->category->languageTo():voc->category->languageFrom());
        ui->lblQuestionLangTo->setText(invers?voc->category->languageFrom():voc->category->languageTo());
        ui->txtQuestion->setFont( invers?voc->category->fontTo():voc->category->fontFrom());
        ui->txtQuestion->setText( invers?voc->language2:voc->language1 );
    }
    else
        ui->lblMessages->show();
}

void MainWindow::startLearning()
{
    ui->lblQuestionLangTo->clear();
    ui->lblQuestionLangFrom->clear();
    backend.prepareTrainingSet();
}

void MainWindow::currentCategoryChanged(CategoryPtr cat)
{
    bool valid = !cat.isNull() && cat->isValid();
    ui->btnCategoreRename->setEnabled( valid );
    ui->splitterEnterVocabs->setEnabled(valid);
    ui->lblLanguage1->setText(tr("Sprache 1"));
    ui->lblLanguage2->setText(tr("Sprache 2"));
    ui->edtLanguage1->clear();
    ui->edtLanguage2->clear();
    if (!valid) {
        qDebug() << "changed to invalid category";
        ui->edtLanguage1->setFocus();
        ui->lstVocables->setModel(NULL);
        return;
    }
    ui->edtLanguage1->setText(cat->languageFrom());
    ui->edtLanguage2->setText(cat->languageTo());
    ui->txtLanguage1->setFont(cat->fontFrom() );
    ui->txtLanguage2->setFont(cat->fontTo() );
    ui->lblLanguage1->setText(cat->languageFrom());
    ui->lblLanguage2->setText(cat->languageTo());
    ui->lstVocables->setModel( backend.currentVocabularyModel());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    connect(ui->lstVocables->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::vocableSelectionChanged);
#else
    connect(ui->lstVocables->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(vocableSelectionChanged(QModelIndex,QModelIndex)));
#endif
}

void MainWindow::on_btnCategoreRename_clicked()
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

    if (languageFrom.isEmpty() || languageTo.isEmpty()) {
        ui->lblWarning->setText(tr("Die Eingabefelder für Sprache 1 und Sprache 2 dürfen nicht leer sein."));
        ui->lblWarning->show();
        QTimer::singleShot(5000, ui->lblWarning, SLOT(hide()));
        return;
    }

    backend.addCategory(languageFrom, languageTo);
}

void MainWindow::on_cmbEnterCategory_currentIndexChanged(int index)
{
    bool validItem = ui->cmbEnterCategory->count()>0;
    ui->btnCategoryRemove->setEnabled( validItem );
    ui->btnCategoreRename->setEnabled( validItem );
    ui->btnShare->setEnabled(validItem );
    ui->txtLanguage1->clear();
    ui->txtLanguage2->clear();

    if(validItem)
        backend.setCurrentCategory( ui->cmbEnterCategory->itemData(index).value<CategoryPtr>() );
    else
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
        qDebug() << "update ";
        backend.updateVocable( ui->lstVocables->selectionModel()->currentIndex(),
                               ui->txtLanguage1->toPlainText(),
                               ui->txtLanguage2->toPlainText(),
                               ui->spnLektion->value());
    }
    else {
        qDebug() << "insert ";
        backend.addVocable(ui->txtLanguage1->toPlainText(),
                           ui->txtLanguage2->toPlainText(),
                           ui->spnLektion->value());
    }
    ui->txtLanguage1->clear();
    ui->txtLanguage2->clear();
    ui->txtLanguage1->setFocus();
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

void MainWindow::showAnswer()
{
    if (backend.currentVocable() == NULL)
        return;

    bool invers = ui->chkInvers->isChecked();
    Vocable *voc = backend.currentVocable();
    ui->txtAnswer->setFont( invers?voc->category->fontFrom():voc->category->fontTo());
    ui->txtAnswer->setText( invers?voc->language1:voc->language2 );
}

void MainWindow::setEditButtonsStateEnabled(bool enabled)
{
    ui->btnQuestionDelete->setEnabled(enabled);
    ui->btnQuestionNew->setEnabled(enabled);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *evt)
{
    if (evt->type() == QEvent::FocusIn || evt->type() == QEvent::FocusOut) {
        if (obj != ui->txtLanguage1 && obj != ui->txtLanguage2)
            KeyboardLayout::restore();
    }
    return QMainWindow::eventFilter(obj,evt);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *evt)
{
    foreach(QString format, evt->mimeData()->formats())
        qDebug() << format << "\t= " << evt->mimeData()->data(format).toHex();

    if (evt->mimeData()->hasFormat("application/x-qt-windows-mime;value=\"FileName\"")){
        QFileInfo nfo(evt->mimeData()->data("application/x-qt-windows-mime;value=\"FileName\""));
        qDebug() << nfo.absoluteFilePath();
    }
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
    Q_UNUSED(index);
    KeyboardLayout::restore();
    if (ui->tabWidget->currentWidget() == ui->tabLearn) {
        startLearning();
    }
}

void MainWindow::on_btnStay_clicked()
{
    if (ui->txtAnswer->toPlainText().isEmpty())
        showAnswer();
    else {
        backend.currentVocable()->stay();
        backend.showNextVocable();
    }
}

void MainWindow::on_btnBackTo0_clicked()
{
    if (ui->txtAnswer->toPlainText().isEmpty())
        showAnswer();
    else {
        backend.currentVocable()->backTo0();
        backend.showNextVocable();
    }
}

void MainWindow::on_btnNext_clicked()
{
    if (ui->txtAnswer->toPlainText().isEmpty())
        showAnswer();
    else {
        backend.currentVocable()->next();
        backend.showNextVocable();
    }
}

void MainWindow::on_btnBack_clicked()
{
    if (ui->txtAnswer->toPlainText().isEmpty())
        showAnswer();
    else {
        backend.currentVocable()->back();
        backend.showNextVocable();
    }
}

void MainWindow::on_btnShare_clicked()
{
    qApp->setOverrideCursor( Qt::WaitCursor );
    ui->tabImportExport->share( backend.currentCategory() );
    ui->tabWidget->setCurrentWidget( ui->tabImportExport );
    qApp->restoreOverrideCursor();
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
