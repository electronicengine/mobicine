import QtQuick 2.0
import QtQuick.Window 2.0
import QtQml 2.0
import QtQuick.Controls 2.2



Flickable
{


    id: fl

    contentWidth: 2000
    clip: true
    ScrollBar.horizontal: ScrollBar { active: true; visible: true }
    ScrollBar.vertical: ScrollBar { active: true; visible: true }
    Rectangle
    {
        id: background
        anchors.fill: parent
        color: "#f68c85"
        gradient: Gradient
        {
           GradientStop { position: 0.0; color: "#f9bdb9" }
           GradientStop { position: 1.0; color: "#f68c85" }
        }
    }


    ListView
    {
        id: myListView
        width: parent.width
        height: parent.height
        highlight: Rectangle { color: "#41d6cf"; radius: 10 }
        highlightMoveDuration: 1
        highlightMoveVelocity: 1000
        focus: true

        model: myModel


        delegate: Component
        {


            Item
            {
                property variant itemData: model.modelData
                width: column.width
                height: column.height


                Column
                {
                    id:column

                    SequentialAnimation on x
                    {

                        NumberAnimation { target: column; property: "x"; to: 10 }
                        PropertyAnimation { target: column; property: "opacity"; to: 0  }
                        PropertyAnimation { target: column; property: "opacity"; to: 100  }
                        NumberAnimation { target: column; property: "x"; to: 0 }

                    }

                    Text
                    {
                        text: name
                        wrapMode: Text.WordWrap
                        color: "black"
                    }

                }
                MouseArea
                {
                    id: itemMouseArea
                    anchors.fill: parent
                    onClicked:
                    {
                        myListView.currentIndex = index

                    }
                }
            }
        }


        Keys.onSpacePressed: model.insert(0, { "name": "Item " + model.count })
        onCurrentItemChanged: mainwindow.currentItemIdexChanged(myListView.currentIndex)
    }
}
