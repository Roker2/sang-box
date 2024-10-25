import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import MMaterial

Rectangle {
    id: root
    color: "#FABA94"
    radius: 12

    Layout.preferredWidth: 300
    Layout.preferredHeight: 300

    property color lineColor: "#552D11"
    property int lineHeight: 30
    property int lineThickness: 1

    property alias label: label
    property alias labelText: label.labelText
    property alias labelLeftMargin: label.anchors.leftMargin
    property alias labelTopMargin: label.anchors.topMargin
    property alias verticalLineLeftMargin: verticalLine.anchors.leftMargin
    property alias horizontalLine: horizontalLine
    property alias verticalLine: verticalLine

    Rectangle {
        id: horizontalLine
        color: root.lineColor
        height: root.lineThickness
        width: parent.width

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: root.lineHeight
    }

    Rectangle {
        id: verticalLine
        color: root.lineColor
        width: root.lineThickness
        height: root.lineHeight

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 99.5
    }

    Label {
        id: label
        property string labelText: qsTr("Profiles")
        text: labelText
        font.pixelSize: 16;

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 5
    }
}
