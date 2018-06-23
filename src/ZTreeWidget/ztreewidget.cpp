#include "ztreewidget.h"
#include <QHeaderView>
#include <QWidget>
#include "item.h"
#include <QDebug>

ZTreeWidget::ZTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    setAlternatingRowColors(true);
    setToolTip("time sequences");

    QStringList columns;
    columns << "Status"
            << " Type "
            << " Time "
            << " Comment ";

    setHeaderLabels(columns);
}

void ZTreeWidget::addItem()
{
    new Item(this);
}

void ZTreeWidget::addItem(const int type, const QTime time, QString comment)
{
    Item *i = new Item(time, type, this);
    i->setComment(comment);
}

void ZTreeWidget::addItem(const int type, const QString time, QString comment)
{
    QStringList l;
    QTime t;
    try{
        l = time.split(":");
        t = QTime(l[0].toInt(), l[1].toInt(), l[2].toInt());
    }
    catch(...){
        qDebug() << "ERROR in ZTreeWidget::addItem(int, QString, QString);";
    }
    addItem(type, t, comment);
}

void ZTreeWidget::deleteSelected()
{
    QTreeWidgetItem *i = currentItem();
    if(i) delete i;
}

Item* ZTreeWidget::getCurrent()
{
    // cast the current item
    Item *i = dynamic_cast<Item*>(currentItem());
    return i;
}

void ZTreeWidget::zero()
{
    QTreeWidgetItemIterator it(this);
    while (*it) {
        Item *i = static_cast<Item*>(*it);
        i->setStatus(Item::Waiting);
    }
}
