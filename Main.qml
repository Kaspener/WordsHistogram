import QtQuick
import QtQuick.Dialogs
import QtQuick.Layouts
import QtQuick.Controls
import WordsHistogram
Window {
    id: root
    minimumWidth: 400
    minimumHeight: 680
    visible: true
    title: qsTr("Words Histogram")
    Controller {
        id: controller
    }
    FileDialog {
        id: openFileDialog

        title: "Choose file"
        options: FileDialog.ReadOnly
        fileMode: FileDialog.OpenFile
        nameFilters: ["Text files (*.txt)"]

        onAccepted: {
            controller.filePath = selectedFile
        }
    }
    ColumnLayout{
        id: mainColumnLayout
        anchors.fill: parent
        spacing: 5
        anchors.margins: 10
        Rectangle{
            Layout.fillWidth: true
            Layout.fillHeight: true

            border.width: 1
            border.color: "gray"

            ListView {
                id: histogram

                anchors {
                    fill: parent
                    margins: 1
                }

                clip: true
                model: controller.model

                spacing: 4
                readonly property int padding: 4

                delegate: Item {
                    width: histogram.width * percent
                    height: wordRow.height + histogram.spacing

                    Rectangle {
                        id: wordRow

                        anchors {
                            bottom: parent.bottom
                            horizontalCenter: parent.horizontalCenter
                        }

                        width: parent.width - 2 * histogram.padding
                        height: label.height

                        border {
                            width: 1
                            color: "gray"
                        }

                        color: Qt.rgba(Math.random(), Math.random(), Math.random(), 1);

                        Text {
                            id: label

                            anchors.verticalCenter: parent.verticalCenter

                            padding: 6
                            width: histogram.width - 2 * histogram.padding

                            elide: Text.ElideMiddle
                            text: `${word} (${count})`
                        }
                    }
                }
                move: Transition {
                    NumberAnimation { properties: "y" }
                }

                add: Transition {
                    NumberAnimation { properties: "x"; from: -histogram.width/2 }
                }

                remove: Transition {
                    NumberAnimation { properties: "opacity"; to: 0.15 }
                }

                displaced: Transition {
                    NumberAnimation { properties: "y" }
                }
            }
        }
        ProgressBar {
            id: progressBar
            Layout.fillWidth: true
            value: controller.progress

            visible: stopButton.visible
        }

        RowLayout{
            id: fileLayout
            RoundButton{
                id: openFileButton
                width: 50
                height: 30
                radius: 5
                text: "Open file"
                onClicked: openFileDialog.open()
            }

            Text {
                id: filePath
                text: controller.filePath
            }
        }
        RowLayout{
            id: buttonsLayout
            RoundButton{
                id: startPauseButton
                Layout.fillWidth: true
                width: 50
                height: 30
                radius: 5
                text: "Start"
                enabled: filePath.text != ""
                onClicked: {
                    openFileButton.enabled = false;
                    controller.startPause();
                    if (text == "Start"){
                        text = "Pause";
                        stopButton.visible = true;
                    }
                    else{
                        text = "Start";
                    }

                }
            }
            RoundButton{
                id: stopButton
                Layout.fillWidth: true
                visible: false
                width: 50
                height: 30
                radius: 5
                text: "Stop"
                onClicked: {
                    controller.stop();
                    visible = false;
                    startPauseButton.text = "Start";
                    openFileButton.enabled = true;
                }
            }
        }

    }
}
