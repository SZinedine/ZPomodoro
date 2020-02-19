#ifndef TIME_H
#define TIME_H

#include <QObject>
#include <QString>
#include <QTime>

/*
 * the class Time is made to save the time value easily in order to be passed from function to function
 */


class Time : public QObject
{
    Q_OBJECT

    void init();
    void setupSignalsAndSlots();
public:
    Time(QObject *parent=nullptr);
    Time(QString time, QObject *parent=nullptr);
    Time(QString hours, QString minutes, QString seconds, QObject *parent=nullptr);
    Time(int hours, int minutes, int seconds, QObject *parent=nullptr);
    Time(const Time &t);

    void setTime(QString hours, QString minutes, QString seconds);
    void setTime(int hours, int minutes, int seconds);
    void setTime(QString time);
    void setTime(QTime time);
    void setHours(QString h);
    void setHours(int h);
    void setMinutes(QString m);
    void setMinutes(int m);
    void setSeconds(QString s);
    void setSeconds(int s);
    int getTotalSec() const;
    void setTotalSec(int total);
    QString getFormatedTime() const;
    QString addSecondAndReturnTimeStr();	// abstraction
    QString subSecondAndReturnTimeStr();	// abstraction
    inline QString getHoursStr() const 		{ return s_hours; 	}
    inline QString getMinutesStr() const 	{ return s_minutes; }
    inline QString getSecondsStr() const	{ return s_seconds; }
    inline int getHoursInt() const			{ return d_hours; 	}
    inline int getMinutesInt() const		{ return d_minutes; }
    inline int getSecondsInt() const		{ return d_seconds; }
    inline bool isZero() const				{ return (d_hours+d_seconds+d_minutes == 0) ? true : false; }
    Time operator+(const Time &t);		// untested. it probably doesn't work well

public slots:
    void addSecond();
    void addMinute();
    void addHour();
    void subSecond();
    void subMinute();
    void subHour();
    inline void setZero()	{ setTime(0, 0, 0); }

private slots:
    void adjustSeconds();
    void adjustMinutes();
    void adjustHours();

signals:
    void secondsChanged();
    void minutesChanged();
    void hoursChanged();
    void timeChanged();
    void zeroed();			// when all the clock is at 0. works only if the seconds were the ones that brought it to zero
                            // it helps implement timeout()

private:
    QString s_hours, s_minutes, s_seconds;
    int d_hours, d_minutes, d_seconds;

};

#endif // TIME_H
