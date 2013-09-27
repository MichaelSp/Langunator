#pragma once

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

    void startLearning();

    void initLatexWebView();
    void setEditButtonsStateEnabled(bool enabled);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *evt);
    virtual void dragEnterEvent(QDragEnterEvent *evt);

private slots:
    void currentCategoryChanged(CategoryPtr cat);
    void updateCategories(CategoriesPtr cats);
    void on_btnCategoreRename_clicked();
    void vocableSelectionChanged(const QModelIndex &current, const QModelIndex &previous);
    void setVocable(Vocable *voc);

    void on_btnCategoryAdd_clicked();
    void on_cmbEnterCategory_currentIndexChanged(int index);
    void on_btnCategoryRemove_clicked();
    void on_btnQuestionSave_clicked();
    void on_btnQuestionDelete_clicked();
    void showAnswer();
    void on_tabWidget_currentChanged(int index);
    void on_btnStay_clicked();
    void on_btnBackTo0_clicked();
    void on_btnNext_clicked();
    void on_btnBack_clicked();
    void on_chkShowLatex_clicked(bool showLatex);
    void on_txtLanguage2_textChanged();
    void on_btnQuestionNew_clicked();
    void on_btnShare_clicked();

private:
    Ui::MainWindow *ui;
    Backend backend;
    bool modeRename;
};

