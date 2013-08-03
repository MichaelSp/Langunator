#pragma once

#include "pch.h"
class Backend;

class InputWidget : public QTextEdit
{
    Q_OBJECT
public:
    explicit InputWidget(QWidget *parent = 0);
    
    void setFontSizeDelta(int size);
    void setBackend(Backend &backend, bool isFirstLanguage);

    void setButtonsVisible(bool visible);
    void setTextFont(QFont fnt);
protected:
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void showEvent(QShowEvent *);
    void wheelEvent(QWheelEvent *e);

private slots:
    void keyboardLayoutSelected(int index);
    void moveButtons();
    void showFontDialog();
    void showKeyboardLayoutSelection();

private:
    QToolButton fontButton;
    QToolButton keyboardLayout;
    QComboBox languages;
    Backend * backend;
    bool isFirstLanguage;
};
