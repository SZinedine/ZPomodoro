#include "ztime.h"
#include <exception>
#include <iostream>
#include <QDebug>

Time::Time(QObject *parent)
    : QObject(parent)
{
    init();
    setupSignalsAndSlots();
}

Time::Time(QString hours, QString minutes, QString seconds, QObject *parent)
    : QObject(parent)
{
    init();
    setupSignalsAndSlots();
    setTime(hours, minutes, seconds);
}

Time::Time(int hours, int minutes, int seconds, QObject *parent)
    : QObject(parent)
{
    init();
    setupSignalsAndSlots();
    setTime(hours, minutes, seconds);
}

Time::Time(QString time, QObject *parent)
    : QObject (parent)
{
    setupSignalsAndSlots();
    setTime(time);
}

Time::Time(const Time &t)
    : QObject(nullptr)
{
    setupSignalsAndSlots();
    s_seconds = t.s_seconds;
    s_minutes = t.s_minutes;
    s_hours	  = t.s_hours;
    d_seconds = t.d_seconds;
    d_minutes = t.d_minutes;
    d_hours   = t.d_hours;
}

void Time::init()
{
    d_seconds = d_minutes = d_hours = 0;
    s_seconds = s_minutes = s_hours = "00";
}

void Time::setupSignalsAndSlots()
{
    connect(this, SIGNAL(secondsChanged()),		this, SLOT(adjustSeconds() ));
    connect(this, SIGNAL(minutesChanged()),		this, SLOT(adjustMinutes() ));
    connect(this, SIGNAL(hoursChanged()),		this, SLOT(adjustHours()   ));
}


void Time::setTime(QString hours, QString minutes, QString seconds)
{
    setHours(hours);
    setMinutes(minutes);
    setSeconds(seconds);
}

void Time::setTime(int hours, int minutes, int seconds)
{
    setHours(hours);
    setMinutes(minutes);
    setSeconds(seconds);
}

void Time::setTime(QString time)
{
    QStringList lst = time.split(":");
    try {
        setTime(lst[0], lst[1], lst[2]);
    } catch (const std::out_of_range &err) {
        std::cerr << err.what() << "\nERROR. out of range error in the list of QStrings.Time object has not a valid value.\n";
    }
    catch(...){
        std::cerr << "and undefined error has occured in the Time object while strying to initialize the members with a QString.\n";
    }
}

void Time::setTime(QTime time)
{
    setTime(time.hour(), time.minute(), time.second());
}

void Time::setHours(QString h)
{
    int tmp = d_hours;
    d_hours = h.toInt();
    s_hours = h;
    emit hoursChanged();
    if(tmp != d_hours) emit timeChanged();
}

void Time::setHours(int h)
{
    int tmp = d_hours;
    s_hours = QString::number(h);
    d_hours = h;
    emit hoursChanged();
    if(tmp != d_hours) emit timeChanged();
}

void Time::setMinutes(QString m)
{
    int tmp = d_minutes;
    d_minutes = m.toInt();
    s_minutes = m;
    emit minutesChanged();
    if (tmp != d_minutes) emit timeChanged();
}

void Time::setMinutes(int m)
{
    int tmp = d_minutes;
    d_minutes = m;
    s_minutes = QString::number(m);
    emit minutesChanged();
    if (tmp != d_minutes) emit timeChanged();
}

void Time::setSeconds(QString s)
{
    int tmp = d_seconds;
    s_seconds = s;
    d_seconds = s.toInt();
    if (tmp != d_seconds) {
        emit secondsChanged();
        emit timeChanged();
        if( getSecondsInt() == 0 && getMinutesInt() == 0 && getHoursInt() == 0 )
            emit zeroed();
    }
}

void Time::setSeconds(int s)
{
    int tmp = d_seconds;
    d_seconds = s;
    s_seconds = QString::number(s);
    if (tmp != d_seconds) {
        emit secondsChanged();
        emit timeChanged();
        if( getSecondsInt() == 0 && getMinutesInt() == 0 && getHoursInt() == 0 )
            emit zeroed();
    }
}

int Time::getTotalSec() const
{
    int tmp = getSecondsInt();
    tmp += getMinutesInt() * 60;
    tmp += (getHoursInt() * 60) * 60;

    return tmp;
}

void Time::setTotalSec(int total)
{
    setHours(total/3600);
    total -= getHoursInt() * 3600;
    setSeconds(total % 60);
    setMinutes((total - getSecondsInt()) / 60);
}

QString Time::getFormatedTime() const
{
    QString tmp = s_hours;
    tmp.append(":" + s_minutes + ":" + s_seconds);
    return tmp;
}


void Time::addSecond()
{
    if(getSecondsInt() == 59) {
        setSeconds(0);
        addMinute();
    }
    else setSeconds(getSecondsInt()+1);
}


void Time::addMinute()
{
    if(getMinutesInt() == 59){
        setMinutes(0);
        addHour();
    } else setMinutes(getMinutesInt()+1);
}


void Time::addHour()
{
    setHours( getHoursInt() + 1 );
}

void Time::subSecond()
{
    if (getSecondsInt() == 0){
        if (getMinutesInt() == 0 && getHoursInt() == 0) return;
        setSeconds(59);
        subMinute();
    } else setSeconds(getSecondsInt() - 1 );
}

void Time::subMinute()
{
    if (getMinutesInt() == 0){
        if (getHoursInt() == 0) return;
        setMinutes(59);
        subHour();
    } else setMinutes( getMinutesInt() - 1 );
}

void Time::subHour()
{
    if (getHoursInt() == 0) return;
    setHours( getHoursInt() - 1 );
}

void Time::adjustSeconds()
{
    if(d_seconds > 59) d_seconds = 0;
    s_seconds = QString::number(d_seconds);
    if(d_seconds < 10 && getSecondsStr().length() == 1) s_seconds.prepend("0");
}

void Time::adjustMinutes()
{
    if(d_minutes > 59) d_minutes = 0;
    s_minutes = QString::number(d_minutes);
    if(d_minutes < 10 && getMinutesStr().length() == 1) s_minutes.prepend("0");
}

void Time::adjustHours()
{
    if(getHoursInt() < 10 && getHoursStr().length() == 1) s_hours.prepend("0");
}


QString Time::addSecondAndReturnTimeStr()
{
    addSecond();
    return getFormatedTime();
}


QString Time::subSecondAndReturnTimeStr()
{
    subSecond();
    return getFormatedTime();
}



Time Time::operator+(const Time &t)
{
    Time tmp;
    tmp.setHours	(	getHoursInt()	+	t.getHoursInt() 	);
    tmp.setMinutes	( 	getMinutesInt()	+	t.getMinutesInt() 	);
    tmp.setSeconds	(	getSecondsInt()	+	t.getSecondsInt()	);

    return tmp;
}
