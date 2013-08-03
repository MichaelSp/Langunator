#include "InputWidget.h"
#include "keyboardlayout.h"
#include "Backend.h"

InputWidget::InputWidget(QWidget *parent) :
    QTextEdit(parent),
    fontButton(this),
    keyboardLayout(this),
    languages(this)
{
    fontButton.setIcon( QIcon(":/img/img/apps_preferences_desktop_filetype_association.png") );
    keyboardLayout.setIcon( QIcon(":/img/img/Taste.png") );
    fontButton.setAttribute(Qt::WA_SetFont, false);
    keyboardLayout.setAttribute(Qt::WA_SetFont, false);
    languages.setAttribute(Qt::WA_SetFont, false);

    QList<KeyboardLayout::LanguageInfo> lang = KeyboardLayout::languages();
    languages.addItem("<Nicht ändern>");
    foreach(KeyboardLayout::LanguageInfo nfo, lang)
        languages.addItem(nfo.name, QVariant::fromValue(nfo));
    languages.hide();

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    connect(&fontButton, &QToolButton::clicked, this, &InputWidget::showFontDialog);
    connect(&keyboardLayout, &QToolButton::clicked, &languages, &QComboBox::show);
    connect(&keyboardLayout, &QToolButton::clicked, &languages, &QComboBox::showPopup);
#else
    connect(&keyboardLayout, SIGNAL(clicked()), &languages, SLOT(show()));
    connect(&fontButton, SIGNAL(clicked()), this, SLOT(showFontDialog()));
#endif
    connect(&languages, SIGNAL(activated(int)), this, SLOT(keyboardLayoutSelected(int)));
    setButtonsVisible(false);
}

void InputWidget::setButtonsVisible(bool visible)
{
    fontButton.setVisible(visible);
    keyboardLayout.setVisible(visible);
}
void InputWidget::focusInEvent(QFocusEvent *evt)
{
    QTextEdit::focusInEvent(evt);
    setButtonsVisible(true);
    languages.hide();
    if(isFirstLanguage){
        KeyboardLayout::setActiveKeyboardLayout(backend->currentCategory()->keyboardLayoutFrom());
        qDebug() << "restore " <<  backend->currentCategory()->fontFrom();
        setFont( backend->currentCategory()->fontFrom() );
    }
    else {
        KeyboardLayout::setActiveKeyboardLayout(backend->currentCategory()->keyboardLayoutTo());
        setFont( backend->currentCategory()->fontTo() );
    }
}

void InputWidget::focusOutEvent(QFocusEvent *e)
{
    QTextEdit::focusOutEvent(e);
    setButtonsVisible(false);
    KeyboardLayout::restore();
}

void InputWidget::showEvent(QShowEvent *e)
{
    QTextEdit::showEvent(e);
    moveButtons();
}

void InputWidget::moveButtons()
{
    int spacing = fontButton.iconSize().width()*2;
    fontButton.move(width() - spacing, 10);
    keyboardLayout.move(width() - spacing*2, 10);
    languages.move(width() - languages.sizeHint().width() - 10, 10 + keyboardLayout.height());
}

void InputWidget::setTextFont(QFont fnt)
{
    qDebug() << "store " <<  fnt;
    if (isFirstLanguage)
        backend->currentCategory()->setFontFrom( fnt );
    else
        backend->currentCategory()->setFontTo( fnt );
    setFont(fnt);
}

void InputWidget::showFontDialog()
{
    bool ok;
    QFont fnt = QFontDialog::getFont( &ok, font(), this);
    if (!ok) {
        return;
    } else {
        setTextFont(fnt);
    }
}

void InputWidget::showKeyboardLayoutSelection()
{
    int currentLayout = backend->currentCategory()->keyboardLayoutTo();
    if (isFirstLanguage)
        currentLayout = backend->currentCategory()->keyboardLayoutFrom();

    qDebug() << currentLayout;
    languages.blockSignals(true);
    for(int i=0;i<languages.count();i++)
        if (languages.itemData(i).isValid() &&
                languages.itemData(i).value<KeyboardLayout::LanguageInfo>().code == currentLayout) {
            languages.setCurrentIndex(i);
            break;
        }
    languages.blockSignals(false);
}

void InputWidget::setFontSizeDelta(int size) {
    QFont fnt = font();
    fnt.setPixelSize( qMin(200, qMax(5, fnt.pixelSize() + size ) ));
    setTextFont(fnt);
}

void InputWidget::setBackend(Backend &backend, bool isFirstLanguage)
{
    this->backend = &backend;
    this->isFirstLanguage=isFirstLanguage;
}

void InputWidget::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() & Qt::ControlModifier) {
        e->accept();
        int delta = qMin(1,qMax(-1,e->delta()));
        setFontSizeDelta(delta);
    }
}

void InputWidget::keyboardLayoutSelected(int index)
{
    if (backend->currentCategory().isNull())
        return;
    if (!languages.itemData(index).isValid()) {
        backend->currentCategory()->setKeyboardLayoutFrom(0);
        keyboardLayout.setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
    KeyboardLayout::LanguageInfo nfo = languages.itemData(index).value<KeyboardLayout::LanguageInfo>();
    if (isFirstLanguage)
        backend->currentCategory()->setKeyboardLayoutFrom(nfo.code);
    else
        backend->currentCategory()->setKeyboardLayoutTo(nfo.code);
    KeyboardLayout::setActiveKeyboardLayout(nfo.code);
    keyboardLayout.setText(nfo.shortName);
    keyboardLayout.setToolButtonStyle(Qt::ToolButtonTextOnly);
    setButtonsVisible(true);
    languages.hide();
}
