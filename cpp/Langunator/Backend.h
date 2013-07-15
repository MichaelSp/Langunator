#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = 0);

signals:

public slots:
    void addCategory(QString cat);

};

#endif // BACKEND_H
