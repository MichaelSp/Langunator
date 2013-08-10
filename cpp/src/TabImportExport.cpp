#include "TabImportExport.h"
#include "ui_TabImportExport.h"
#include "vocabel.h"
#include <QtNetwork>

TabImportExport::TabImportExport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabImportExport),
    github(this)
{
    ui->setupUi(this);
    initTableWidget( ui->tblExport);
    initTableWidget( ui->tblImport);
    QSettings set;
    ui->edtName->setText( set.value("Author", "").toString() );
}

void TabImportExport::initTableWidget(QTableWidget *widget)
{
    widget->horizontalHeader()->setDefaultSectionSize(170);
    widget->horizontalHeader()->resizeSection(0, 28);
    widget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
}


TabImportExport::~TabImportExport()
{
    QSettings set;
    set.setValue("Author", ui->edtName->text());
    delete ui;
}

void TabImportExport::share(CategoryPtr cat)
{
    if (ui->tblExport->findItems(cat->categoryName(),Qt::MatchExactly).isEmpty()) {
        cat->refreshNumbers();
        if (cat->author.get().toString().isEmpty())
            cat->author = ui->edtName->text().isEmpty()?ui->edtName->placeholderText():ui->edtName->text();

        insertRow(ui->tblExport, cat, true);
    }
}

void TabImportExport::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
    on_btnRefresh_clicked();
}

QTableWidgetItem* TabImportExport::createItem(const CategoryPtr &pack, QString text, bool checkable, bool isSelected) {
    QTableWidgetItem *item = new QTableWidgetItem(text);
    item->setData(Qt::UserRole, QVariant::fromValue(pack));
    if (checkable)
        item->setCheckState(Qt::Unchecked);
    if (isSelected)
        item->setCheckState(Qt::Checked);
    return item;
}

CategoriesPtr TabImportExport::getPackages(QTableWidget *tableWidget)
{
    CategoriesPtr packages;
    for(int i=0;i< tableWidget->rowCount();i++)
    {
        QTableWidgetItem *item = tableWidget->item(i,0);
        if (item->checkState() == Qt::Checked) {
            CategoryPtr package = item->data(Qt::UserRole).value<CategoryPtr>();
            packages.append(package);
        }
    }
    return packages;
}

void TabImportExport::insertRow(QTableWidget *tableWidget, const CategoryPtr &pack, bool isSelected) {
    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);
    tableWidget->setItem(row, 0, createItem(pack, "", true, isSelected) );
    tableWidget->setItem(row, 1, createItem(pack, pack->categoryName() ));
    tableWidget->setItem(row, 2, createItem(pack, QString("%1").arg(pack->lesson) ));
    tableWidget->setItem(row, 3, createItem(pack, tr("%1 Einträge").arg(pack->count) ));
    tableWidget->setItem(row, 4, createItem(pack, QString("%1").arg(pack->author) ));
}

void TabImportExport::on_btnRefresh_clicked()
{
    qDebug() << "refresh";
    while(ui->tblImport->rowCount()>0)
        ui->tblImport->removeRow(0);
    ui->tblImport->setEnabled(false);
    github.loadIndex( [this](CategoriesPtr &packages) {
        foreach(const CategoryPtr &pack, packages){
            insertRow(ui->tblImport, pack);
        }
        ui->tblImport->setEnabled(true);
    }, [this](QNetworkReply* reply){
        QMessageBox::warning(this, tr("Error loading index"),
                             reply->errorString());
    });
}

void TabImportExport::on_btnDownload_clicked()
{
    github.downloadPackages(getPackages(ui->tblImport), [this](QList<Vocable> &vocList){
        qDebug()<< "Package successfull: " << vocList;
    }, [=](const CategoryPtr &, const QString &error){
        QMessageBox::warning(this, tr("Package download error"), error);
    });
}

void TabImportExport::on_tblImport_itemClicked(QTableWidgetItem *)
{
    ui->btnDownload->setDisabled( getPackages(ui->tblImport).isEmpty() );
}

void TabImportExport::on_tblExport_itemClicked(QTableWidgetItem *)
{
    ui->btnUpload->setDisabled( getPackages(ui->tblExport).isEmpty() );
}

void TabImportExport::on_tblImport_itemDoubleClicked(QTableWidgetItem *item)
{
    QTableWidgetItem *col0 = ui->tblImport->item(item->row(), 0);
    if (col0->checkState() == Qt::Checked)
        col0->setCheckState( Qt::Unchecked);
    else
        col0->setCheckState( Qt::Checked);

}

void TabImportExport::on_tblExport_itemDoubleClicked(QTableWidgetItem *item)
{
    QTableWidgetItem *col0 = ui->tblExport->item(item->row(), 0);
    if (col0->checkState() == Qt::Checked)
        col0->setCheckState( Qt::Unchecked);
    else
        col0->setCheckState( Qt::Checked);
}

void TabImportExport::on_edtName_textChanged(const QString &author)
{
    for(int i=0;i<ui->tblExport->rowCount();i++) {
        QTableWidgetItem *item = ui->tblExport->item(i,4);
        CategoryPtr package = item->data(Qt::UserRole).value<CategoryPtr>();
        package->author = author;
        item->setText(QString("%1").arg(package->author));
    }
}

void TabImportExport::removePackFrom(QTableWidget *wid, const CategoryPtr &pack) {
    for(int i=0;i<wid->rowCount();i++) {
        QTableWidgetItem * item = wid->item(i,0);
        if (pack == item->data(Qt::UserRole).value<CategoryPtr>())
        {
            wid->removeRow(i);
        }
    }
}

void TabImportExport::on_btnUpload_clicked()
{
    CategoriesPtr uploads = getPackages(ui->tblExport);
    github.uploadPackages(uploads, [uploads, this](const CategoryPtr &done){
        qDebug()<< "Package successfull: " << done->categoryName();
        removePackFrom(ui->tblExport, done);
    }, [uploads, this](const CategoryPtr &failed, const QString &error){
        QMessageBox::warning(this, tr("Package upload error"), error);
    }, [this](){
        on_btnRefresh_clicked();
    });
}
