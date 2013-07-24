#pragma once

#include <QSet>

#ifdef Q_OS_WIN
#include "Windows.h"
#endif

class KeyboardLayout
{
public:
    KeyboardLayout();
    virtual ~KeyboardLayout();

    struct LanguageInfo {
        QString name;
        int code;
    };

    static void restore();
    static QList<LanguageInfo> languages();
    static void setActiveKeyboardLayout(int langCode);
private:
#ifdef Q_OS_WIN
    static BOOL CALLBACK enumLocalesCallback(_In_  LPTSTR lpLocaleString);
#endif
    static QList<LanguageInfo> langList;
};

inline bool operator ==(const KeyboardLayout::LanguageInfo &a, const KeyboardLayout::LanguageInfo &b){
    return a.name == b.name;
}
inline bool operator <(const KeyboardLayout::LanguageInfo &a, const KeyboardLayout::LanguageInfo &b){
    return a.name < b.name;
}