#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "pch.h"
#include "Backend.h"

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
    void currentCategoryChanged(CategoryPtr cat);
    void updateCategories(CategoriesPtr cats);
    void currentLanguageChanged(QString);

    void on_btnCategoryAdd_clicked();
    void on_cmbEnterCategory_currentIndexChanged(int index);
    void on_btnCategoryRemove_clicked();
    void on_btnQuestionSave_clicked();

    void on_btnShowAnswer_clicked();

private:
    Ui::MainWindow *ui;
    Backend backend;
};

#endif // MAINWINDOW_H
