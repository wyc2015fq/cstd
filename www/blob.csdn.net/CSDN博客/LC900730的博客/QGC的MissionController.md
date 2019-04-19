# QGC的MissionController - LC900730的博客 - CSDN博客
2017年09月18日 16:15:33[lc900730](https://me.csdn.net/LC900730)阅读数：413
## MissionController.cc
```
_addMissionSettings(_visualItems,false);
_initAllVisualItems();
MissionController::_addMissionSettings(){
    MissionSettingsItems*settingsItem=new MissionSettingsItem(_controllerVehicle,visualItems);
    visualItems->insert(0,settingsItem);
//看MissionSettingsItem构造函数
}
```
//看MissionSettingsItem构造函数
```
MissionSettingsItem::MissionSettingsItem(Vehicle* vehicle,QObject *parent):ComplexMissionItem(x,x)
    ,_plannedHomePositionAltitudeFact(0,x,x)
    ...
    ...
{   
    _editorQml="qrc:/qml/MissionSettingsEditor.qml"
    if(_metaDataMap.isEmpty()){
        _metaDataMap=FactMetaData::createMapFromJsonFile
(
    QStringLiteral(":/json/MissionSettings.FactMetaData.json")
)
    _plannedHomePositionAltitudeFact.setMetaData(_meta...)
    _plannedHomePositionAltitudeFact.setRawData(_meta...)
    }
    ...
    ...
    _cameraSection.setAvailable(true);
    _speedSection.setAvailable(true);
    ...
}
```
在MissionSettingsEditor.qml中
``
```
//QmlObjectListModel.h中
private:
    QList<QObject*>_objectList;
```
