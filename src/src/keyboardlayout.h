#pragma once

#include <QSet>
#include <QVariant>

#ifdef Q_OS_WIN
#include "Windows.h"
#endif

class KeyboardLayout
{
public:
    KeyboardLayout();
    virtual ~KeyboardLayout();

    struct LanguageInfo {
        QString shortName;
        QString name;
        int code;
    };

    static void restore();
    static QList<LanguageInfo> languages();
    static void setActiveKeyboardLayout(int langCode, int subCode = SUBLANG_DEFAULT);
private:
#ifdef Q_OS_WIN
    static BOOL CALLBACK enumLocalesCallback(LPTSTR lpLocaleString);
    static BOOL CALLBACK enumLanguageGroupLocalesProc(
             LGRPID LanguageGroup, LCID Locale, LPTSTR lpLocaleString, LONG_PTR lParam);
#endif
    static QList<LanguageInfo> langList;
};
Q_DECLARE_METATYPE(KeyboardLayout::LanguageInfo)

inline bool operator ==(const KeyboardLayout::LanguageInfo &a, const KeyboardLayout::LanguageInfo &b){
    return a.name == b.name;
}
inline bool operator <(const KeyboardLayout::LanguageInfo &a, const KeyboardLayout::LanguageInfo &b){
    return a.name < b.name;
}
