#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "db.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Factory::instance(this);

    connect(Factory::instance(), SIGNAL(categoriesUpdated(Categories&)), SLOT(updateCategories(Categories&)));
//    connect(DB::instance(), SIGNAL(currentCategoryChanged(Category&)), SLOT(currentCategoryChanged(Category&)));
    connect(ui->edtLanguage1, SIGNAL(textEdited(QString)), SLOT(currentLanguageChanged(QString)));
    connect(ui->edtLanguage2, SIGNAL(textEdited(QString)), SLOT(currentLanguageChanged(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateCategories(Categories &cats)
{
    ui->cmbQuestionCategory->clear();
    ui->cmbEnterCategory->clear();
    foreach(Category cat,cats){
        ui->cmbQuestionCategory->addItem(cat.categoryName(), QVariant::fromValue(cat));
        ui->cmbEnterCategory->addItem(cat.categoryName(), QVariant::fromValue(cat));
    }
    ui->cmbEnterCategory->addItem("<Neue Kategorie>", false);
    ui->cmbEnterCategory->setCurrentIndex(ui->cmbEnterCategory->count()-2);
}

/*
void MainWindow::currentCategoryChanged(Category &cat)
{
    ui->btnCategoryAdd->setDisabled(cat.isValid());
    ui->tabEditBoxes->setEnabled(cat.isValid());
    if (cat.isValid()) {
        ui->edtLanguage1->setText(cat.languageFrom());
        ui->edtLanguage2->setText(cat.languageTo());
    }
    else {
        ui->edtLanguage1->clear();
        ui->edtLanguage2->clear();
    }
}
*/

void MainWindow::currentLanguageChanged(QString)
{
    QString languageFrom = ui->edtLanguage1->text();
    QString languageTo = ui->edtLanguage2->text();

    if (Category::currentCategory().isValid()){
        if (!languageFrom.isEmpty())
            Category::currentCategory().setLanguageFrom(languageFrom);
        if (!languageTo.isEmpty())
            Category::currentCategory().setLanguageTo(languageTo);
        ui->cmbEnterCategory->setItemText(ui->cmbEnterCategory->currentIndex(), Category::currentCategory().categoryName() );
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
    Category cat(languageFrom,languageTo);
    //cat.save();
}

void MainWindow::on_cmbEnterCategory_currentIndexChanged(int index)
{
    bool validItem = (index>=0);
    bool newItem = ui->cmbEnterCategory->itemData(index).type() == QVariant::Bool;
    ui->btnCategoryRemove->setEnabled(validItem && !newItem);

    /*
     if(validItem && !newItem)
        DB::instance()->setCurrentCategoryIndex(index);
    if(newItem)
    {
        DB::instance()->setCurrentCategoryIndex(-1);
    }
    */
}

void MainWindow::on_btnCategoryRemove_clicked()
{
   // DB::instance()->removeCategory( ui->cmbEnterCategory->itemData(ui->cmbEnterCategory->currentIndex()).value<Category>() );
}

void MainWindow::on_btnQuestionSave_clicked()
{
    if (ui->txtLanguage1->toPlainText().isEmpty() || ui->txtLanguage2->toPlainText().isEmpty())
    {
        return; // TODO: Warning
    }
   // DB::instance()->currentCategory().add(ui->txtLanguage1->toHtml(), ui->txtLanguage2->toHtml());
    ui->txtLanguage1->clear();
    ui->txtLanguage2->clear();
}

void MainWindow::on_btnQuestionDelete_clicked()
{

}

void MainWindow::on_btnNext_clicked()
{

}

void MainWindow::on_btnPrev_clicked()
{

}

void MainWindow::on_btnMoveToBox_clicked()
{

}
