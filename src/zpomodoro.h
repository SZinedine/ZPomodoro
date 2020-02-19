#ifndef ZTIMER_H
#define ZTIMER_H

#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <QTimer>
#include <QMediaPlayer>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QCloseEvent>
#include "timelabel.h"
#include "ztreewidget.h"


class ZPomodoro : public QMainWindow
{
    Q_OBJECT
    enum State{			// it isn't used currently
        Running = 0,
        Paused,
        Stopped
    };

public:
    ZPomodoro(QWidget *parent = nullptr);

private:
    void setupCentral();
    void setupSignalsAndSlots();
    void setupStyle();
    void setupMenu();
    void setBusy(const bool b);
    inline bool isBusy() const		{ return busy; }
    inline void setEnable(QWidget* w, const bool b) { w->setEnabled(b); }

private slots:
    void makeOperation();		// find the current Item, set time then call counting() to start the timer
    void start();				// trigger the gear
    void pause();
    void reset();				// put the TimeLabel to zero
    void jump();				// put an end to the current countdown and jump into the next Item
    void disableSomeWidgets();	// disable the +/- buttons when it is busy
    void endOfPeriod();			// alarm + set Item::Done
    void counting();			// count for a second then substract a second. it is called everytime qtimer::timeout() is emitted
    void hideLowerPart();		// make (in)visible the lower part when the button hideButton is clicked
    void closeEvent(QCloseEvent *event) override;
    void saveSettings();
    void loadSettings();
    void oneItem(QTreeWidgetItem *item);	// triggered when an item is clicked.
    void about();				// display a Dialog that contains information about the app

private:
    TimeLabel *timeLabel;
    ZTreeWidget *treeWidget;
    QPushButton *jumpButton;
    QPushButton *startButton;
    QPushButton *pauseButton;
    QPushButton *resetButton;
    QPushButton *addItemButton;
    QPushButton *delItemButton;
    QPushButton *clearItemsButton;
    QSpinBox *iterationsSpinBox;
    QVBoxLayout *layout;
    QTimer *qtimer;
    bool busy;
    State currentState;		// it has no use for now. I will probably remove it
    QMediaPlayer *player;

    QAction *quitAction;
    QAction *startAction;
    QAction *pauseAction;
    QAction *resetAction;
    QAction *jumpAction;
    QAction *aboutAction;

    QWidget *lowerPartWidget;
    QPushButton *hideButton;

signals:
    void stateChanged();
    void endOfCycle();
};

#endif // ZTIMER_H
