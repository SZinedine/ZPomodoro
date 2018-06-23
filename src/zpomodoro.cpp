#include "zpomodoro.h"
#include "common.h"
#include <QVariant>
#include <QFontDatabase>
#include <QFont>
#include <QVector>
#include <QMessageBox>
#include <QSettings>

ZPomodoro::ZPomodoro(QWidget *parent)
    : QMainWindow(parent)
{
    setupCentral();
    setupSignalsAndSlots();
    setBusy(false);
    currentState = State::Stopped;

    setMinimumWidth(500);
    loadSettings();
}


void ZPomodoro::setupCentral()
{
    QWidget *mainWidget = new QWidget;
    setCentralWidget(mainWidget);
    layout = new QVBoxLayout;
    mainWidget->setLayout(layout);
    QHBoxLayout *controlButtonsLayout;

    lowerPartWidget = new QWidget;
    QVBoxLayout *lowerPartLayout = new QVBoxLayout;
    lowerPartWidget->setLayout(lowerPartLayout);

    hideButton = new QPushButton;
    hideButton->setFixedHeight(10);

    {	// construct the upper part of the UI
        timeLabel = new TimeLabel("00:00:00", this);
        treeWidget = new ZTreeWidget(this);

        startButton = new QPushButton("Start", this);
        pauseButton = new QPushButton("Pause", this);
        resetButton = new QPushButton("Reset", this);
        jumpButton  = new QPushButton("Jump", this);

        controlButtonsLayout = new QHBoxLayout;
        controlButtonsLayout->addWidget(startButton);
        controlButtonsLayout->addWidget(pauseButton);
        controlButtonsLayout->addWidget(resetButton);
    }

    {	// construct the lower part
        addItemButton = new QPushButton("+", this);
        delItemButton = new QPushButton("-", this);
        clearItemsButton = new QPushButton("*-*", this);
        QHBoxLayout *treeWidgetButtonsLayout = new QHBoxLayout;
        treeWidgetButtonsLayout->addWidget(addItemButton);
        treeWidgetButtonsLayout->addWidget(delItemButton);
        treeWidgetButtonsLayout->addWidget(clearItemsButton);
        treeWidgetButtonsLayout->setAlignment(Qt::AlignRight);

        iterationsSpinBox = new QSpinBox(this);
        iterationsSpinBox->setRange(0, 100);
        iterationsSpinBox->setValue(1);
        QLabel *iterLabel = new QLabel("Iterations: ");
        QHBoxLayout *iterLayout = new QHBoxLayout;
        iterLayout->addWidget(iterLabel);
        iterLayout->addWidget(iterationsSpinBox);
        iterLayout->setAlignment(Qt::AlignLeft);

        QHBoxLayout *treeWidgetControlsLayout = new QHBoxLayout;
        treeWidgetControlsLayout->addLayout(iterLayout);
        treeWidgetControlsLayout->addLayout(treeWidgetButtonsLayout);

        lowerPartLayout->addLayout(treeWidgetControlsLayout);
        lowerPartLayout->addWidget(treeWidget);
    }

    layout->addWidget(timeLabel);
    layout->addLayout(controlButtonsLayout);
    layout->addWidget(jumpButton);
    layout->addWidget(hideButton);
    layout->addWidget(lowerPartWidget);

    qtimer = new QTimer(this);
    player = new QMediaPlayer(this);
    player->setMedia(QUrl("qrc:/audio/alarm.mp3"));
    connect(hideButton,		SIGNAL(clicked()),		this,		SLOT(hideLowerPart())	);

    setupMenu();
    setupStyle();
}

void ZPomodoro::setupMenu()
{
    QMenu *menuFile = new QMenu;
    menuFile = menuBar()->addMenu("File");
    menuFile->addSeparator();

    quitAction = new QAction("Quit");
    menuFile->addAction(quitAction);

    QMenu *menuEdit = new QMenu;
    menuEdit = menuBar()->addMenu("Edit");

    startAction = new QAction("Start");
    pauseAction = new QAction("Pause");
    resetAction = new QAction("Reset");
    jumpAction = new QAction("Jump to the next");
    QList<QAction*> editList = { startAction, pauseAction, resetAction, jumpAction};
    menuEdit->addActions(editList);

    QMenu *menuHelp = new QMenu;
    menuHelp = menuBar()->addMenu("?");
    aboutAction = new QAction("About");
    menuHelp->addAction(aboutAction);
}

void ZPomodoro::setupStyle()
{
//    QFontDatabase::addApplicationFont(":font/Segment7Standard.otf");
//    QFont f("Segment7, Demi Bold Italic", 40);
//    QFontDatabase::addApplicationFont(":font/DS-DIGI.TTF");
//    QFont f("DS-Digital, Regular", 60);
    QFontDatabase::addApplicationFont(":font/LCDAT&TPhoneTimeDate.ttf");
    QFont f(time_label_font, time_label_size_1);

    timeLabel->setFont(f);

    layout->setAlignment(Qt::AlignCenter);
    timeLabel->setAlignment(Qt::AlignCenter);

    // set the properties of widgets in to customise them with CSS
    startButton->setProperty("managerButton", true);
    pauseButton->setProperty("managerButton", true);
    resetButton->setProperty("managerButton", true);
    addItemButton->setProperty("treeWidgetButton", true);
    delItemButton->setProperty("treeWidgetButton", true);
    clearItemsButton->setProperty("treeWidgetButton", true);

    const int s = 30;
    addItemButton->setFixedWidth(s);
    delItemButton->setFixedWidth(s);
    clearItemsButton->setFixedWidth(s);

    timeLabel->setContentsMargins(0, 0, 0, 0);

    treeWidget->setAlternatingRowColors(false);
}

void ZPomodoro::setupSignalsAndSlots()
{
    connect(qtimer,		SIGNAL(timeout()),			this,			SLOT(counting())	);
    // disable some buttons when the TimeLabel is busy.
    connect(this,		SIGNAL(stateChanged()),		this,			SLOT(disableSomeWidgets()));
    // play alarm when zero
    connect(timeLabel->getTimeObj(),	SIGNAL(zeroed()),	this,	SLOT(endOfPeriod()));
    // menu actions
    connect(quitAction, 	SIGNAL(triggered()),		this, 		SLOT(close())	);
    // these are the control buttons connected to local functions to start/pause/stop the timer
    connect(startButton,	SIGNAL(clicked()),			this, 		SLOT(start())	);
    connect(startAction,	SIGNAL(triggered()),		this, 		SLOT(start())	);
    connect(pauseButton,	SIGNAL(clicked()),			this, 		SLOT(pause())	);
    connect(pauseAction,	SIGNAL(triggered()),		this, 		SLOT(pause())	);
    connect(resetButton,	SIGNAL(clicked()),			this, 		SLOT(reset())	);
    connect(resetAction,	SIGNAL(triggered()),		this, 		SLOT(reset())	);
    // add & del items from the TreeWidget
    connect(addItemButton,	SIGNAL(clicked()),			treeWidget,	SLOT(addItem())			);
    connect(delItemButton,	SIGNAL(clicked()),			treeWidget,	SLOT(deleteSelected())	);
    connect(clearItemsButton, SIGNAL(clicked()),		treeWidget, SLOT(clear())			);
    connect(aboutAction, SIGNAL(triggered()),			this,		SLOT(about())			);

    connect(jumpButton,		SIGNAL(clicked()),			this, 		SLOT(jump()) 			);
    connect(jumpAction,		SIGNAL(triggered()),		this, 		SLOT(jump()) 			);
    connect(treeWidget,	SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),			this,		SLOT(oneItem(QTreeWidgetItem*)));
}


void ZPomodoro::start()
{
    if(isBusy()) return;
    player->stop();
    setBusy(true);
    currentState = State::Running;
    if(iterationsSpinBox->value() > 0)
        makeOperation();
}


void ZPomodoro::pause()
{
    setBusy(false);
    currentState = State::Paused;
}

void ZPomodoro::reset()
{
    setBusy(false);
    currentState = State::Stopped;
    timeLabel->setZero();

    QTreeWidgetItemIterator it(treeWidget);
    while (*it){
        Item *i = static_cast<Item*>(*it);
        i->setStatus(Item::Waiting);
        it++;
    }
    player->stop();
}


void ZPomodoro::jump()
{
    if (!isBusy()) return;

    QTreeWidgetItemIterator it(treeWidget);
    while (*it) {
        Item *current = static_cast<Item*>(*it);
        if (current->getStatus() == Item::Current) {

            // jump into the next item
            Item *next = nullptr;
            it++;
            if (*it) next = static_cast<Item*>(*it);
            else { 	// if it was the last item, finish the cycle
                timeLabel->setTime(0,0,0);
                return;
            }

            // change status
            current->setStatus(Item::Done);
            next->setStatus(Item::Current);

            // set time and launch the countdown
            timeLabel->setTime(next->getTime());
            counting();
        }
        it++;
    }
}

void ZPomodoro::oneItem(QTreeWidgetItem *item)
{
    if (isBusy()) return;

    // make the current one waiting. start the clicked one and make it current
    QTreeWidgetItemIterator it(treeWidget);
    while (*it) {
        Item *current = static_cast<Item*>(*it);
        if (current->getStatus() == Item::Current) current->setStatus(Item::Waiting);
        it++;
    }

    // send the clicked one and start the countdown
    Item *i = static_cast<Item*>(item);
    timeLabel->setTime(i->getTime());
    i->setStatus(Item::Current);
    setBusy(true);
    counting();
}

void ZPomodoro::makeOperation()
{
    if (!isBusy()) return;
    Item *p_item = nullptr;						// hosts the good selected item

    QTreeWidgetItemIterator it(treeWidget);

    while (*it){				// search for an Item in a state of State::Waiting
        Item *i = static_cast<Item*>(*it);
        if(i->getStatus() == Item::Waiting) {
            p_item = i;
            i->setStatus(Item::Current);
            break;
        }
        else if (i->getStatus() == Item::Current){	// it have been paused or something
            counting();
            return;
        }
        it++;
    }
    if (p_item){			// if an item is found, get its time, put into the timeLabel and start counting
        timeLabel->setTime(p_item->getTime());
        counting();
    }
    else {				// if p_item isn't set, that means that all Items are Done. now we will start another cycle
        QMessageBox::information(this, "cycle ended", "cycle ended");
        const int val = iterationsSpinBox->value();
        if(val != 0) {
            reset();
            iterationsSpinBox->setValue(val-1);
        }
        start();
    }
}


void ZPomodoro::counting()
{
    if (isBusy()){
        qtimer->start(999);
        if	(!timeLabel->isZero())
            timeLabel->subSecond();
        else		// if everything timeLabel->isZero() to prevent disableSomeWidgets(true)
            setBusy(false);
    }
}

void ZPomodoro::endOfPeriod()
{
    if(!isBusy()) return;
    player->play();

    QTreeWidgetItemIterator it(treeWidget);
    while(*it){
        Item *i = static_cast<Item*>(*it);
        if(i->getStatus() == Item::Current) {
            i->setStatus(Item::Done);
            break;
        }
        it++;
    }

//    if(isBusy())
        QMessageBox::information(this, "End of a period",
                                       "The Period has ended. hit the button to start the next one.");
    setBusy(false);
    currentState = State::Stopped;
    timeLabel->setZero();
    start();
}


void ZPomodoro::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
}

void ZPomodoro::hideLowerPart()
{
    if (lowerPartWidget->isVisible()) {
        lowerPartWidget->setVisible(false);
        QFont f(time_label_font, time_label_size_2);
        timeLabel->setFont(f);
    }
    else {
        lowerPartWidget->setVisible(true);
        QFont f(time_label_font, time_label_size_1);
        timeLabel->setFont(f);
    }
}

void ZPomodoro::saveSettings()
{
    QSettings s;
    s.beginGroup("items");
    QList<QVariant> lst;

    QTreeWidgetItemIterator it(treeWidget);
    while (*it){
        Item *i = static_cast<Item*>(*it);

        QMap<QString, QVariant> map {
            {"type",		QVariant(i->getTypeInt())	},
            {"time",		QVariant(i->getTime())		},
            {"comment",		QVariant(i->getComment())	}
        };

        lst.append(QVariant(map));
        it++;
    }

    s.setValue("last_items", QVariant(lst));
    s.endGroup();
}

void ZPomodoro::loadSettings()
{
    QSettings s;
    s.beginGroup("items");
    QList<QVariant> v = s.value("last_items").toList();
    s.endGroup();

    for (const QVariant &i : v){
        QMap<QString, QVariant> map = i.toMap();
        treeWidget->addItem(
                    map.value("type").toInt(),
                    map.value("time").toString(),
                    map.value("comment").toString()	);
    }
}

void ZPomodoro::setBusy(const bool b)
{
    busy = b;
    emit stateChanged();
}


void ZPomodoro::disableSomeWidgets()
{
    QVector<QWidget*> list {		// a list of widgets to be disabled when the app is busy
                addItemButton,			delItemButton,		clearItemsButton,
                iterationsSpinBox
    };

    for (QWidget *w : list)
        if (isBusy()) 	setEnable(w, false);
        else 			setEnable(w, true);
}


void ZPomodoro::about()
{
    QString msg;
    msg.append("<b>" + QString(APP_NAME) + "<br>");
    msg.append("Version </b>" + QString(APP_VERSION));
    msg.append("<br>Copyright " + QString(APP_AUTHOR));
    msg.append("<br><b>E-mail: </b>" + QString(APP_AUTHOR_EMAIL) + "<br>");
    msg.append("licenced under the GPL2<br>");
    QMessageBox::about(this, "About ZPomodoro", msg);
}
