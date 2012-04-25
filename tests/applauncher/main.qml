import QtQuick 2.0
import Quiche 1.0

Item {

    width: 1366
    height: 768
    id: desktop

    Component {
        id: testDelegate
        Item {
            width: desktop.width/2; height: 150
            Rectangle {
                id: activeIndicator
                anchors.fill:parent;
                color: "lightsteelblue";
                border.color: "darkblue";
                radius: 8
                visible: false
            }
	    Row {
		spacing:10
		anchors.verticalCenter: parent.center
		Item {
		width:100; height:100;
		//Rectangle {width:100; height:100; color: "black"}
		Image {anchors.fill:parent; source: icon }
		}
            	Column {
                    Text { text: '<b>Name:</b> ' + name }
                    Text { text: '<b>Target:</b> ' + target }
                    Text { text: '<b>Type:</b> ' + type }
                    Text { text: '<b>Icon:</b> ' + icon }
                    Text { text: '<b>Path:</b> ' + path }
                    Text { text: '<b>Args:</b> ' + args }
            	}
	    }
            MouseArea {
                id: mouse_area1
                z: 1
                hoverEnabled: false
                anchors.fill: parent

                onPressed:{
                    activeIndicator.visible = true
                }
                onReleased:{
                    activeIndicator.visible = false;
                    quicheConsole.sendRequest("QUEUE " + target);
                    Qt.quit()
                }
                onCanceled:{
                    activeIndicator.visible = false
                }

            }

        }
    }
    ListView {
        id: listview
        anchors.fill: parent

        model: AppLauncherModel {
            applicationDirectory: "./apps"
	    iconPath: "./icons"
        }

        delegate: testDelegate
    }

    QuicheConsole {
        id: quicheConsole;
    }
}
