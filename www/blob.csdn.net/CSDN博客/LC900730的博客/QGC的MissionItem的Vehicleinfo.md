# QGC的MissionItem的Vehicleinfo - LC900730的博客 - CSDN博客
2017年09月20日 14:33:38[lc900730](https://me.csdn.net/LC900730)阅读数：292
![这里写图片描述](https://img-blog.csdn.net/20170920134607030?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```
//MissionSettingsEditor.qml
Rectangle{
    id:valueRect
    width:availwidth
    height:
    color:
    visibie:missionItem.isCurrent
    readonly property string _firmwareLabel:    
        qsTr("Firmware");
    readonly property string _vehicle:  
        qsTr("Vehicle");
}
```
```
//MissionItem.qml
Component{
    id:missionSettings
    Column{
        id:valueColumn
        anchors.left: parent ? parent.left:undefined
        ...
        ...
        Column{
            anchors.left:parent.left
            ...
            ...
            GridLayout{
                anchors.left:parent.left
                ...
                QGCLabel{
                    text:qsTr("Waypoint alt");
                }
                FactTextField{  
                                                                                 fact:                                                                                                  QGroundControl.settingsManager.appSettings.
                    defaultMissionItemAltitude
                }
            }
    QGCCheckBox{
        id:flightSpeedCheckBox
        qsTr:("Flight speed")
        visible:!_missionVehicle.vtol
    }
        }
    }
}
```
### Vehicle Info
```
SectionHeader{
    id:vehicleInfoSectionHeader
    text:qsTr("Vehicle Info")
    visible:_offlineEditing&&!_waypointsOnlyMode
}
GridLayout{
    QGCLabel{
        text:_firmwareLabel
        Layout.fillWidth:true
        visible:_showOfflineVehicleCombos
    }
    FactComboBox{
        fact:QGroundControl...
        indexModel:false
        visible:_showOfflineVehicleCombos
        enabled:_
    }
}
```
![这里写图片描述](https://img-blog.csdn.net/20170920150541882?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
json文件中描述如下： 
![这里写图片描述](https://img-blog.csdn.net/20170920150731561?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
