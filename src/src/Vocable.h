#pragma once

#include "pch.h"
#include "category.h"

class Vocable: public DQModel {
    DQ_MODEL
public:
    Vocable(){}
    Vocable(const QJsonObject &obj, CategoryPtr cat);
    virtual ~Vocable(){}
    void next();
    void back();
    void backTo0();
    void stay();

    DQForeignKey<Category> category;
    DQField<QString> language1;
    DQField<QString> language2;
    DQField<int> lesson;
    DQField<int> nextClicks;
    DQField<int> backClicks;
    DQField<int> stayClicks;
    DQField<int> backTo0Clicks;
    DQField<int> rightInRow;
    DQField<QDateTime> lastAsked;

private:
    QDateTime getTimestamp();
};

inline void operator <<(QJsonObject &obj, const Vocable &voc) {
    QVariantMap map;
    map.insert("lang1", voc.language1.get().toString().toUtf8().toBase64());
    map.insert("lang2", voc.language2.get().toString().toUtf8().toBase64());
    map.insert("lesson", voc.lesson.get().toInt());
    obj = QJsonObject::fromVariantMap(map);
}
inline void operator <<(Vocable &voc, const QJsonObject&obj) {
    voc.language1 = QString::fromUtf8(QByteArray::fromBase64(obj.value("lang1").toString("<undefined>").toUtf8()));
    voc.language2 = QString::fromUtf8(QByteArray::fromBase64(obj.value("lang2").toString("<undefined>").toUtf8()));
    voc.lesson = obj.value("lesson").toDouble(0);
}

DQ_DECLARE_MODEL(Vocable,
                 "vocable", // the table name.
                 DQ_FIELD(category, DQNotNull),
                 DQ_FIELD(language1, DQNotNull),
                 DQ_FIELD(language2, DQNotNull),
                 DQ_FIELD(lesson, DQDefault(0)),
                 DQ_FIELD(nextClicks, DQDefault(0)),
                 DQ_FIELD(backClicks, DQDefault(0)),
                 DQ_FIELD(stayClicks, DQDefault(0)),
                 DQ_FIELD(backTo0Clicks, DQDefault(0)),
                 DQ_FIELD(rightInRow, DQDefault(0)),
                 DQ_FIELD(lastAsked, DQDefault("CURRENT_TIMESTAMP"))
                 )

Q_DECLARE_METATYPE(Vocable)
Q_DECLARE_METATYPE(Vocable*)
