#pragma once

#include <QSet>

#ifdef Q_OS_WIN
#include "Windows.h"
#endif

class KeyboardLayout
{
public:
    KeyboardLayout();

    struct LanguageInfo {
        QString name;
        int code;
    };

    static void restore();
    static QList<LanguageInfo> languages();
    static void setActiveKeyboardLayout(int langCode);
private:
    static BOOL CALLBACK KeyboardLayout::enumLocalesCallback(_In_  LPTSTR lpLocaleString);
    static QList<LanguageInfo> langList;
};

inline bool operator ==(const KeyboardLayout::LanguageInfo &a, const KeyboardLayout::LanguageInfo &b){
    return a.name == b.name;
}
inline bool operator <(const KeyboardLayout::LanguageInfo &a, const KeyboardLayout::LanguageInfo &b){
    return a.name < b.name;
}
