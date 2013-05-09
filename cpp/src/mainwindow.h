#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "pch.h"
#include "factory.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void updateCategories(Categories &cats);
    void currentLanguageChanged(QString);
    void on_btnCategoryAdd_clicked();
    void on_cmbEnterCategory_currentIndexChanged(int index);
    void on_btnCategoryRemove_clicked();
    void on_btnQuestionSave_clicked();
    void on_btnQuestionDelete_clicked();
    void on_btnNext_clicked();
    void on_btnPrev_clicked();
    void on_btnMoveToBox_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
