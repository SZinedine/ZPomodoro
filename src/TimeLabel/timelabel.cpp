#include "timelabel.h"
#include <QDebug>
#include <QRegularExpression>

TimeLabel::TimeLabel(QWidget *parent)
    : QLabel(QString("00:00:00"), parent)
{
    time = new Time("00:00:00", this);
    setupSignalsAndSlots();
    setContentsMargins(0, 0, 0, 0);
}
TimeLabel::TimeLabel(QString text, QWidget *parent)
    : QLabel(text, parent)
{
    time = new Time(text, this);
    setupSignalsAndSlots();
    setContentsMargins(0, 0, 0, 0);
}

TimeLabel::TimeLabel(Time t, QWidget *parent)
    : QLabel(t.getFormatedTime(), parent)
{
    time = new Time(t);
    time->setParent(this);
    setupSignalsAndSlots();
    setContentsMargins(0, 0, 0, 0);
}


void TimeLabel::displayTime()
{
    QString previousDisplay = text();
    QString tmp = time->getFormatedTime();
    setText(tmp);

    if(previousDisplay != tmp) emit displayChanged();
}

void TimeLabel::setupSignalsAndSlots()
{
    connect(time,	SIGNAL(timeChanged()),			this, SLOT(displayTime()));
}
