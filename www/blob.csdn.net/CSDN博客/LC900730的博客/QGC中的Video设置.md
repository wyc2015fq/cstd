# QGC中的Video设置 - LC900730的博客 - CSDN博客
2017年09月19日 23:39:25[lc900730](https://me.csdn.net/LC900730)阅读数：440
## VideoSettings
Settings/VideoSettings 
以组为单位进行配置 
在SettingsGroup.h中
```
QMap<QString,FactMetaData*>_nameToMetaDataMap
在VideoSettings.cc中
_nameToMetaDataMap[videoSourceName]->setEnumInfo(videoSourceList,videoSourceVarList)
```
