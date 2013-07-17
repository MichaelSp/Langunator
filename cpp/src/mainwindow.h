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

    void setActiveKeyboardLayout(USHORT primaryLanguage, USHORT subLanguage);
    void startLearning();

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void currentCategoryChanged(CategoryPtr cat);
    void updateCategories(CategoriesPtr cats);
    void currentLanguageChanged(QString);
    void vocableSelectionChanged(const QModelIndex &current, const QModelIndex &previous);
    void setVocable(Vocable *voc);

    void on_btnCategoryAdd_clicked();
    void on_cmbEnterCategory_currentIndexChanged(int index);
    void on_btnCategoryRemove_clicked();
    void on_btnQuestionSave_clicked();
    void on_btnShowAnswer_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_btnStay_clicked();
    void on_btnBackTo0_clicked();
    void on_btnNext_clicked();
    void on_btnBack_clicked();
    void on_cmbKeyboardLayout1_activated(const QString &arg1);

    void on_cmbKeyboardLayout2_activated(const QString &arg1);

private:
    Ui::MainWindow *ui;
    Backend backend;
};

#endif // MAINWINDOW_H
