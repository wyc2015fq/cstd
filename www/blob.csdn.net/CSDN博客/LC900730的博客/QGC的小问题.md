# QGC的小问题 - LC900730的博客 - CSDN博客
2017年09月20日 21:04:51[lc900730](https://me.csdn.net/LC900730)阅读数：365
#### 问题1
文件位置 
resource/qgroundcontrol.qrc/qml/QGroundControl/Controls/QGCView.qml 
问题：46与55行的else if有重复
qgcApp()->toolbox()->settingsManager()->appSettings->offlineEditingFirmwareType()->setRawValue(firmwareType)
在MissionSettingsEditor.qml中
```
FactCombox{
    fact:QGroundControl.settings.appSettings.offlineEditingFirmwareType
    indexModel:false
}
```
FactSystem
### FactPanelController.h
//检查Internal error
```
//FactPanelController.h中
Vehicle * _vehicle;
UASInterface *_uas;
AutoPilotPlugin* _autopilot;
```
