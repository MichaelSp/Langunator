#include "tabstatistics.h"
#include "ui_tabstatistics.h"
#include "Vocable.h"

TabStatistics::TabStatistics(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabStatistics)
{
    ui->setupUi(this);
}

TabStatistics::~TabStatistics()
{
    delete ui;
}

void TabStatistics::showEvent(QShowEvent *)
{
    int right = Vocable::objects().call("count", "nextClicks").toInt();
    int left = Vocable::objects().call("count", "backClicks").toInt();
    int up = Vocable::objects().call("count", "stayClicks").toInt();
    int down = Vocable::objects().call("count", "backTo0Clicks").toInt();
    ui->lblLeft->setText(QString::number(left));
    ui->lblRight->setText(QString::number(right));
    ui->lblUp->setText(QString::number(up));
    ui->lblDown->setText(QString::number(down));
}
