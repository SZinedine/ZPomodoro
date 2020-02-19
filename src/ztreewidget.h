#ifndef ZTREEWIDGET_H
#define ZTREEWIDGET_H

#include <QTreeWidget>
#include <QWidget>
#include "item.h"

class ZTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ZTreeWidget(QWidget *parent=Q_NULLPTR);
    Item* getCurrent();
    void addItem(const int type, const QTime time, QString comment);
    void addItem(const int type, const QString time, QString comment);
    void zero();

public slots:
    void deleteSelected();
    void addItem();
};

#endif // ZTREEWIDGET_H
