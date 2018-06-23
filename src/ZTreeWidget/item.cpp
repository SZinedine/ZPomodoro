#include "item.h"
#include <QTime>
#include <QStringList>

Item::Item(QTreeWidget *parent)
    : QTreeWidgetItem{parent},
      parent{parent}
{
    setup();
}


Item::Item(QTime tm, int tp, QTreeWidget *parent)
    : QTreeWidgetItem{parent},
      parent{parent}
{
    setup();
    setTime(tm);
    setType(tp);
}

Item::Item(Item& other)
    : QTreeWidgetItem (other.parent),
      parent(other.parent)
{
    setup();
    time->setTime(other.time->time());
    type->setCurrentIndex(other.type->currentIndex());
    comment->setText(other.comment->text());
}

void Item::setup()
{
    time = new QTimeEdit(QTime(0, 0, 0));
    type = new QComboBox;
    comment = new QLineEdit;
    statusLabel = new QLabel();

    setStatus(Status::Waiting);

    time->setDisplayFormat("hh:mm:ss");
    time->setMaximumTime(QTime(99, 59, 59));
    time->setMinimumTime(QTime(0, 0, 0, 0));

    QStringList types;
    types << "Work time"
          << "Brief pause"
          << "Long break";
    type->addItems(types);

    int cm = 2;
    time->setContentsMargins(cm, cm, cm, cm);
    type->setContentsMargins(cm, cm, cm, cm);
    comment->setContentsMargins(cm, cm, cm, cm);

    treeWidget()->setItemWidget(this, STATUS_COLUMN, statusLabel);
    treeWidget()->setItemWidget(this, TYPE_COLUMN, type);
    treeWidget()->setItemWidget(this, TIME_COLUMN, time);
    treeWidget()->setItemWidget(this, COMMENT_COLUMN, comment);

    setProperties();
}


void Item::setStatus(const Status& st)
{
    m_status = st;
    if (st == Status::Current)		statusLabel->setText("Current");
    else if (st == Status::Done)	statusLabel->setText("Done");
    else							statusLabel->setText("Waiting");
//    statusLabel->setText(QString::number(m_status));
}


void Item::setProperties()
{
    time->setProperty("ItemComponent", true);
    type->setProperty("ItemComponent", true);
    comment->setProperty("ItemComponent", true);
    statusLabel->setProperty("ItemComponent", true);
}
