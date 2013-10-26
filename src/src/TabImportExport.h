#ifndef TABIMPORTEXPORT_H
#define TABIMPORTEXPORT_H

#include "GithubAPI.h"
#include <QWidget>

namespace Ui {
class TabImportExport;
}

class TabImportExport : public QWidget
{
    Q_OBJECT
    
public:
    explicit TabImportExport(QWidget *parent = 0);
    ~TabImportExport();

    void share(CategoryPtr cat);

    void initTableWidget(QTableWidget *widget);
protected:
    void showEvent(QShowEvent *);
    
private slots:
    void on_btnRefresh_clicked();
    void on_btnDownload_clicked();
    void on_tblImport_itemClicked(QTableWidgetItem *);
    void on_tblImport_itemDoubleClicked(QTableWidgetItem *item);
    void on_tblExport_itemClicked(QTableWidgetItem *);
    void on_tblExport_itemDoubleClicked(QTableWidgetItem *item);
    void on_edtName_textChanged(const QString &author);
    void on_btnUpload_clicked();

private:
    bool saveDuplicate(DQList<Vocable> &dupl, const Vocable &voc);
    void removePackFrom(QTableWidget *wid, const CategoryPtr &pack);
    void insertRow(QTableWidget *tableWidget, const CategoryPtr &pack, bool isSelected=false);
    QTableWidgetItem *createItem(const CategoryPtr &pack, QString text, bool checkable=false, bool isSelected=false);
    CategoriesPtr getPackages(QTableWidget *tableWidget);
    Ui::TabImportExport *ui;
    GithubAPI github;
};

#endif // TABIMPORTEXPORT_H
