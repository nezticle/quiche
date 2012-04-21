import QtQuick 2.0
import Quiche 1.0



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
		 listview.model.sendRequest("QUEUE "+target)
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
  	  }

   delegate: testDelegate
   }
}
