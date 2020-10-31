import QtQuick 2.0
import QtQuick.Window 2.0
import QtQml 2.0
import QtQuick.Controls 2.2

Flickable {
  id: fl

  contentWidth: 2000

  clip: true
  ScrollBar.horizontal: ScrollBar { active: true; visible: true }
  ScrollBar.vertical: ScrollBar { active: true; visible: true }

    ListView{
        id: myListView
        width: parent.width
        height: parent.height
        highlight: Rectangle { color: "#41d6cf"; radius: 10 }
        focus: true

        model: myModel


        delegate: Component{



            Item{
                id:item
                property variant itemData: model.modelData
                width: parent.width
                height: 20

                Column{
                    id:column
                    Text {text: name
                            color: model.modelData.color}

                }
                MouseArea{
                    id: itemMouseArea
                    anchors.fill: parent
                    onClicked: {
                        myListView.currentIndex = index

                    }
                }
            }
        }

        onCurrentItemChanged: mainwindow.currentItemIdexChanged(myListView.currentIndex)


    }
}






//    ListView{
//        id: myListView
//        width: parent.width
//        height: parent.height
//        highlight: Rectangle { color: "lightsteelblue"; radius: 10 }
//        focus: true
//        orientation: myListView.Horizontal

//        model: myModel

//        ScrollBar
//        {
//            id: verticalScrollBar
//            width: view.width-12; height: 12
//            anchors.bottom: view.bottom
//            opacity: 10
//            orientation: Qt.Horizontal
//            position: view.visibleArea.xPosition

//        }


//        delegate: Component{


//            Item{
//                property variant itemData: model.modelData
//                width: parent.width * 5
//                height: 20

//                Column{

//                    Text {text: name
//                            color: model.modelData.color}

//                }
//                MouseArea{
//                    id: itemMouseArea
//                    anchors.fill: parent
//                    onClicked: {
//                        myListView.currentIndex = index

//                    }
//                }
//            }
//        }

//        onCurrentItemChanged: mainwindow.currentItemIdexChanged(myListView.currentIndex)

//        ScrollBar.horizontal: ScrollBar { active: true; visible: true }
//        ScrollBar.vertical: ScrollBar { active: true; visible: true }

//    }

