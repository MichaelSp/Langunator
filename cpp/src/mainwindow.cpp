#include <QtConcurrent/QtConcurrent>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "db.h"



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
    ui->tabEditBoxes->setEnabled(valid);
    if (valid) {
        ui->edtLanguage1->setText(cat->languageFrom());
        ui->edtLanguage2->setText(cat->languageTo());
        ui->lstVocables->setModel( backend.currentVocabularyModel() );
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

    backend.addCategory(languageFrom, languageTo);
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
        QLabel *lbl = ui->lblWarning;
        lbl->show();
        QTimer::singleShot(5000, lbl, SLOT(hide()));
        return;
    }
    backend.addVocable(ui->txtLanguage1->toPlainText(), ui->txtLanguage2->toPlainText());
    ui->txtLanguage1->clear();
    ui->txtLanguage2->clear();
}

