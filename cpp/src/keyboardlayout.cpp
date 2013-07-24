#include "keyboardlayout.h"
#include <QApplication>

#ifdef Q_OS_WIN

void KeyboardLayout::setActiveKeyboardLayout(int langCode) {
    if (langCode == 0 || langCode ==  LOWORD(GetKeyboardLayout(0)))
        return;
    qApp->setOverrideCursor(Qt::WaitCursor);
    LCID dwLang = MAKELANGID(langCode, SUBLANG_DEFAULT);
    WCHAR szBuf[32];
    wsprintf(szBuf, L"%.8x", dwLang);
    HKL result = ActivateKeyboardLayout(LoadKeyboardLayout(szBuf, KLF_ACTIVATE | KLF_REPLACELANG), KLF_REORDER);
    if (result<=0)
        qWarning() << "unable to set keyboard layout " << GetLastError();
    else
        qDebug() << "Keyboard layout set to "<< langCode;
    qApp->restoreOverrideCursor();
}
#else
void KeyboardLayout::setActiveKeyboardLayout(int langCode) {
    qWarning("NOT IMPLEMENTED: setActiveKeyboardLayout");
}
#endif

QList<KeyboardLayout::LanguageInfo> KeyboardLayout::langList;

KeyboardLayout::KeyboardLayout()
{
}

KeyboardLayout::~KeyboardLayout()
{
    restore();
}

void KeyboardLayout::restore()
{
    setActiveKeyboardLayout(GetSystemDefaultLCID ());
}

#ifdef Q_OS_WIN
BOOL CALLBACK KeyboardLayout::enumLocalesCallback(_In_  LPTSTR lpLocaleString)
{
    LCID anLCID = QString::fromWCharArray(lpLocaleString).toInt(0,16);
    WCHAR szBuffer[MAX_PATH];

    GetLocaleInfo(anLCID, LOCALE_SENGLANGUAGE, szBuffer, MAX_PATH);
    LanguageInfo nfo;
    nfo.name= QString::fromWCharArray(szBuffer);
    nfo.code = anLCID;
    if (!KeyboardLayout::langList.contains(nfo))
        KeyboardLayout::langList.append(nfo);
    return true;
}

QList<KeyboardLayout::LanguageInfo> KeyboardLayout::languages()
{
    if (langList.isEmpty()) {
        EnumSystemLocales(&KeyboardLayout::enumLocalesCallback, LCID_INSTALLED);
        qSort(langList);
    }
    return langList;
}
#else
QList<KeyboardLayout::LanguageInfo> KeyboardLayout::languages()
{
    return QList<KeyboardLayout::LanguageInfo>();
}

#endif
