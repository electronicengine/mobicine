#ifndef QMLLISTOBJECT_H
#define QMLLISTOBJECT_H

#include <QObject>
#include <QQuickWidget>
#include <qmllistitem.h>

class QmlListObject : public QObject
{

    Q_OBJECT

    QQuickWidget *Widget_;

    QList<QObject *> Item_List;
    int Current_Index;

public:

    QmlListObject(QObject *parent = nullptr)
        : QObject(parent)
    {
    }


    QmlListObject(QQuickWidget *Widget);

    Q_INVOKABLE void currentItemIdexChanged(int Index);

    void addItem(const QString &ItemStr);
    void clearList();
    QString getCurrentItemText();
    QString getItem(int Index);
    void eraseItem(int Index);
    int getSize();
};

#endif // QMLLISTOBJECT_H
