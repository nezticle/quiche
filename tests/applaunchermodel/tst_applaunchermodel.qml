import QtQuick 2.0
import Quiche 1.0
import QtTest 1.0

Item {

    width: 1366
    height: 768
    id: desktop

    Component {
        id: testDelegate
        Item {
            width: desktop.width/2; height: 50
            Rectangle {
                id: activeIndicator
                anchors.fill:parent;
                color: "lightsteelblue";
                border.color: "darkblue";
                radius: 8
                visible: false
            }
            Column {
                Text { text: '<b>Name:</b> ' + name }
                Text { text: '<b>Target:</b> ' + target }
            }

        }
    }
    ListView {
        id: listview
        anchors.fill: parent

        model: model

        delegate: testDelegate
    }

    AppLauncherModel {
        id: model
        applicationDirectory: "./data"
    }

    TestCase {
        name: "AppLauncherModel"

        function test_validMetaData()
        {

        }
    }

}
