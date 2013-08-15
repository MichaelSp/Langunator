import QtQuick 2.0
import QtQuick.Controls 1.0

ListView {
    id: list_view1
    height: 160
    interactive: true
    keyNavigationWraps: true
    snapMode: ListView.SnapToItem
    highlightFollowsCurrentItem: true
    spacing: 5
    clip: true
    anchors.right: parent.right
    anchors.rightMargin: 10
    anchors.left: parent.left
    anchors.leftMargin: 10
    anchors.top: parent.top
    anchors.topMargin: 10
    highlight: highlight
    focus: true
    populate:  Transition {
        NumberAnimation { properties: "x,y"; from: 100; duration: 1000 }
    }
    delegate: Rectangle {
        id: itemEntry
        height: lang1.height + 10
        width: parent.width
        radius: 5
        border.width: 1
        border.color: "#000000"
        
        Text {
            id: lang1
            text: language1
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize : 12
        }
        
        Text {
            id: lang2
            text: language2
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize : 12
        }
        
        Image {
            id: selectedImage
            visible: false
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            source: "qrc:/img/img/dialog-ok-apply.png"
        }
        states: [
            State {
                name: "selected"
                PropertyChanges {
                    target: selectedImage
                    visible: true
                }
                PropertyChanges {
                    target: itemEntry
                    border.color: "#0000FF"
                }
            }
        ]
    }
    model: ListModel {
        ListElement {
            language1: "sprache1"
            language2: "sprache2"
        }
        ListElement {
            language1: "ab"
            language2: "bc"
        }
        ListElement {
            language1: "sprache1"
            language2: "sprache2"
        }
        ListElement {
            language1: "ab"
            language2: "bc"
        }
        ListElement {
            language1: "ab"
            language2: "bc"
        }
        ListElement {
            language1: "ab"
            language2: "bc"
        }
        ListElement {
            language1: "ab"
            language2: "bc"
        }
    }
}
