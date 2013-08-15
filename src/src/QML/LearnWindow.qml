import QtQuick 2.0
import QtQuick.Controls 1.0

Rectangle {
    id: rectangle1
    width: 300
    height: 400

    Text {
        id: lblQuestion
        text: qsTr("Frage:")
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        font.pointSize: 12
    }

    TextArea {
        id: question
        x: 10
        readOnly: true
        anchors.bottom: lblAnswer.top
        anchors.bottomMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: lblQuestion.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        font.pointSize: 12
    }
    TextArea {
        id: answer
        x: 10
        readOnly: true
        anchors.top: lblAnswer.bottom
        anchors.topMargin: 10
        anchors.bottom: buttonBox.top
        anchors.bottomMargin: 6
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        font.pointSize: 12



        Rectangle {
            id: cover
            visible: false
            color: "#cacaca"
            anchors.fill: parent
            Text {
                anchors.fill: parent
                text: qsTr("Antwort zeigen")
                font.pointSize: 12
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("inasfinaisnfini")
                        cover.state = "";
                    }
                }
            }

            states: [
                State {
                    name: "hidden"
                    PropertyChanges {
                        target: cover
                        visible: true
                    }
                }
            ]
            state: "hidden"
        }
    }

    Rectangle {
        id: buttonBox
        width: 280
        height: 50
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10

        Button {
            id: btnRight
            x: 194
            y: 0
            text: "Richtig"
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            iconSource: "qrc:/img/img/dialog-ok-apply.png"
        }

        Button {
            id: btnWrong
            x: 103
            y: 0
            text: "Falsch"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            iconSource: "qrc:/img/img/dialog-ok.png"
        }

        Button {
            id: btnBox1
            y: 0
            text: "Box 0"
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            iconSource: "qrc:/img/img/edit-delete.png"
        }
    }

    Text {
        id: lblAnswer
        y: 86
        text: qsTr("Antwort:")
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        font.pointSize: 12
    }
}
