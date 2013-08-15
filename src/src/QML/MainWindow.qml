import QtQuick 2.0

Rectangle {
    width: 400
    height: 500
    Flipable {
        id: flipable
        anchors.fill: parent
        property bool flipped: false
        front: LearnWindow {
            id: learn
            anchors.fill: parent
        }
        back: EditWindow {
            id: edit
            anchors.fill: parent
        }

        transform: Rotation {
            id: rotation;
            origin.x: flipable.width/2
            origin.y: flipable.height/2
            axis.x: 0; axis.y: 1; axis.z: 0     // set axis.y to 1 to rotate around y-axis
            angle: 0    // the default angle
        }

        states: State {
            name: "back"
            PropertyChanges { target: rotation; angle: 180 }
            when: flipable.flipped
        }

        transitions: Transition {
            NumberAnimation { target: rotation; property: "angle"; duration: 200 }
        }

        GestureArea {
            anchors.fill: parent
            onSwipeRight: flipable.flipped = !flipable.flipped
            onSwipeLeft: flipable.flipped = !flipable.flipped
        }
    }
}
