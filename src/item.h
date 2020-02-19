#ifndef ITEM_H
#define ITEM_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QLineEdit>
#include <QTimeEdit>
#include <QComboBox>
#include <QLabel>

#define STATUS_COLUMN 0
#define TYPE_COLUMN 1
#define TIME_COLUMN 2
#define COMMENT_COLUMN 3

/*	this class represents the item that will be displayed in the treewidget.
 *  it is a custom QTreeWidgetItem which contains:
 * 		- a ComboBox that displays its type/utility
 * 		- a time to be copied into the TimeDisplayer
 * 		- a LineEdit which is the comment
 */

class Item : public QTreeWidgetItem
{

public:
    enum Status{
        Waiting	= 0,   		// not yet passed on
        Current,			// this is the item under treatment
        Done				// the item is already passed on by the loop
    };

    Item(QTreeWidget *parent);
    Item(QTime tm, int tp, QTreeWidget *parent);
    Item(Item& other);
    void setup();

    void setStatus(const Status& st);
    inline Status getStatus() 				const 	{ return m_status;			}
    inline QString getTime() 				const 	{ return time->text();		}
    inline QTime getTimeObj() 				const 	{ return time->time();		}
    inline void setTime(const QTime t)				{ time->setTime(t);			}
    inline void setComment(const QString& com) 		{ comment->setText(com);	}
    inline QString getComment() 					{ return comment->text();	}
    inline int getTypeInt() const 					{ return type->currentIndex(); }
    inline QString getTypeStr() const				{ return type->currentText(); }
    inline void setType(const int t)				{ type->setCurrentIndex( (t<=2) ? t : 0 ); }
//    inline QTimeEdit* getTimeEdit() { return time; }
    void setProperties();


private:
    QTimeEdit *time;
    QComboBox *type;
    QLineEdit *comment;
    QLabel *statusLabel;
    QTreeWidget *parent;
    Status m_status;
};

#endif // ITEM_H
