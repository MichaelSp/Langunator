#ifndef TABSTATISTICS_H
#define TABSTATISTICS_H

#include <QWidget>

namespace Ui {
class TabStatistics;
}

class TabStatistics : public QWidget
{
    Q_OBJECT
    
public:
    explicit TabStatistics(QWidget *parent = 0);
    ~TabStatistics();

protected:
    virtual void showEvent(QShowEvent *);
    
private:
    Ui::TabStatistics *ui;
};

#endif // TABSTATISTICS_H
