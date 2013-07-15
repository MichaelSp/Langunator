#include <QtConcurrent/QtConcurrent>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "db.h"

#ifdef Q_OS_WIN
#include "Windows.h"

void MainWindow::setActiveKeyboardLayout(USHORT primaryLanguage, USHORT subLanguage) {
    DWORD dwLang = MAKELANGID(primaryLanguage, subLanguage);
    WCHAR szBuf[32];
    wsprintf(szBuf, L"%.8x", dwLang);
    ActivateKeyboardLayout(LoadKeyboardLayout(szBuf, KLF_ACTIVATE | KLF_REPLACELANG), KLF_REORDER);
}
#else
void MainWindow::setActiveKeyboardLayout(QString lang) {
    qWarning("NOT IMPLEMENTED: setActiveKeyboardLayout");
}
#endif


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lblWarning->hide();

    connect(ui->edtLanguage1, &QLineEdit::textEdited, this, &MainWindow::currentLanguageChanged);
    connect(ui->edtLanguage2, &QLineEdit::textEdited, this, &MainWindow::currentLanguageChanged);
    connect(&backend, &Backend::categoriesUpdated, this, &MainWindow::updateCategories);
    connect(&backend, &Backend::currentCategoryChanged, this, &MainWindow::currentCategoryChanged);


    setActiveKeyboardLayout(LANG_GREEK, SUBLANG_DEFAULT);
    QList<QString> lang = languages();
    ui->cmbKeyboardLayout1->addItems(lang);
    ui->cmbKeyboardLayout2->addItems(lang);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateCategories(CategoriesPtr cats)
{
    ui->cmbQuestionCategory->clear();
    ui->cmbEnterCategory->clear();
    foreach(CategoryPtr cat, cats){
        ui->cmbQuestionCategory->addItem(cat->categoryName(), QVariant::fromValue(cat));
        ui->cmbEnterCategory->addItem(cat->categoryName(), QVariant::fromValue(cat));
    }
    int currentIndex = ui->cmbEnterCategory->count()-1;
    ui->cmbEnterCategory->addItem("<Neue Kategorie>", false);
    ui->cmbEnterCategory->setCurrentIndex(currentIndex);

    backend.setCurrentCategory( ui->cmbEnterCategory->itemData( currentIndex ).value<CategoryPtr>() );
}

void MainWindow::currentCategoryChanged(CategoryPtr cat)
{
    bool valid = !cat.isNull() && cat->isValid();
    ui->btnCategoryAdd->setDisabled(valid);
    ui->btnShowAnswer->setEnabled(false);
    if (valid) {
        ui->edtLanguage1->setText(cat->languageFrom());
        ui->edtLanguage2->setText(cat->languageTo());
        Vocabel *voc = backend.currentVocable();
        ui->grpAnswers->setEnabled(voc != NULL);
        if (voc) {
            ui->txtQuestion->setText( voc->language1 );
        }
    }
    else {
        ui->edtLanguage1->clear();
        ui->edtLanguage2->clear();
    }
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
        qDebug() << cat->categoryName() ;
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
    QString layout1 = ui->cmbKeyboardLayout1->currentText();
    QString layout2 = ui->cmbKeyboardLayout2->currentText();
    if (ui->cmbKeyboardLayout1->currentIndex() <=1)
        layout1 = "";
    if (ui->cmbKeyboardLayout2->currentIndex() <=1)
        layout2 = "";

    backend.addCategory(languageFrom, languageTo, layout1, layout2);
}

void MainWindow::on_cmbEnterCategory_currentIndexChanged(int index)
{
    bool validItem = (index>=0);
    bool newItem = ui->cmbEnterCategory->itemData(index).type() == QVariant::Bool;
    ui->btnCategoryRemove->setEnabled(validItem && !newItem);

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
    backend.currentCategory()->addVocable(ui->txtLanguage1->toHtml(), ui->txtLanguage2->toHtml());
    ui->txtLanguage1->clear();
    ui->txtLanguage2->clear();
}

void MainWindow::on_btnShowAnswer_clicked()
{
    if (backend.currentVocable() == NULL)
        return;
    ui->txtLanguage2->setText( backend.currentVocable()->language2 );
}

BOOL CALLBACK Locale_EnumLocalesProcEx(_In_ LPWSTR lpLocaleString,
        _In_  DWORD dwFlags, _In_ LPARAM lParam)
{
    Q_UNUSED(dwFlags);
    ((QList<QString>*)lParam)->append(QString::fromWCharArray(lpLocaleString));
    return TRUE;
}

QList<QString> MainWindow::languages()
{
    QList<QString> lang;
    EnumSystemLocalesEx(&Locale_EnumLocalesProcEx, LOCALE_WINDOWS, (LPARAM)&lang, NULL);
    lang.removeAll("");
    qSort(lang);
    return lang;
}
