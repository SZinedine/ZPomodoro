#ifndef TIMELABEL_H
#define TIMELABEL_H

#include <QLabel>
#include <QString>
#include "ztime.h"

/*
 * a label that represents time and has the ability to control it (like add seconds, hours...) via another class which is Time
*/

class TimeLabel : public QLabel
{
    Q_OBJECT


public:
    TimeLabel(QWidget *parent = nullptr);
    TimeLabel(QString text, QWidget *parent = nullptr);
    TimeLabel(Time t, QWidget *parent = nullptr);

    inline Time* getTimeObj() const 			{ return time; }
    inline void setTime(int h, int m, int s) 	{ time->setTime(h,m,s); }
    inline void setTime(const QTime t)			{ time->setTime(t); }
    inline void setTime(const QString t) 		{ time->setTime(t); }
    inline QTime getTime() const 				{ return QTime(time->getHoursInt(), time->getMinutesInt(), time->getSecondsInt()); }
    inline bool isZero() const					{ return time->isZero(); }

private:
    void setupSignalsAndSlots();

public slots:
    void displayTime();
    inline void addSecond()		{ time->addSecond(); }
    inline void addMinute()		{ time->addMinute(); }
    inline void addHour()		{ time->addHour();	 }
    inline void subSecond()		{ time->subSecond(); }
    inline void subMinute()		{ time->subMinute(); }
    inline void subHour()		{ time->subHour();   }
    inline void setZero()		{ time->setTime(0,0,0); }

signals:
    void displayChanged();
    void timeout();

private:
    Time *time;
};

#endif // TIMELABEL_H
