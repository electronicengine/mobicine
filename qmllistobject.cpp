#include "qmllistobject.h"
#include <QQuickWidget>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickView>
#include <QObject>
#include <QVariant>
#include "ui_mainwindow.h"




QmlListObject::QmlListObject(QQuickWidget *Widget)
{

    Widget->rootContext()->setContextProperty("mainwindow", this);
    Widget->rootContext()->setContextProperty("myModel", QVariant::fromValue(Item_List));
    Widget->setSource(QUrl("qrc:widget.qml"));

    Widget_ = Widget;
}



void QmlListObject::currentItemIdexChanged(int Index)
{
    Current_Index = Index;
    qDebug() << Index;
}



void QmlListObject::addItem(const QString &ItemStr)
{

    Item_List.append(new QmlListItem(ItemStr, "black"));
    Widget_->rootContext()->setContextProperty("myModel", QVariant::fromValue(Item_List));

}



void QmlListObject::clearList()
{
    Item_List.clear();
    Widget_->rootContext()->setContextProperty("myModel", QVariant::fromValue(Item_List));

}



QString QmlListObject::getCurrentItemText()
{
    QmlListItem *item = dynamic_cast<QmlListItem*>(Item_List[Current_Index]);

    return item->name();
}



QString QmlListObject::getItem(int Index)
{
    QmlListItem *item = dynamic_cast<QmlListItem*>(Item_List[Index]);
    return item->name();

}



int QmlListObject::getSize()
{
    return Item_List.size();
}


