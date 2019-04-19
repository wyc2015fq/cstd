# QGC之MissionController - LC900730的博客 - CSDN博客
2017年09月20日 11:22:13[lc900730](https://me.csdn.net/LC900730)阅读数：358
## MissionController
```
//主要是任务项的管理控制器
//MissionController.h
class MissionController :public PlanElementController{
    Q_OBJECT
public:
    //指定父对象
    MissionController
    (planMasterController *masterController)
    ~MissionController()
typedef struct {
    double maxTelemetrydistance;
    double totalDistance;
    double totalTime;
    ...
    ...
    double vehicleSpeed;
    double vehicleYaw
    doubel gimbalYaw
    ...
}MissionFlightStatus_t;
Q_PROPERTY(QmlObjectListModel* visualItems READ visualItems NOTIFY visualItemsChanged);
Q_PROPERTY(int currentMissionIndex READ currentMissionIndex NOTIFY currentMissionIndexChanged)
    ...
    ...
Q_INVOKABLE void removeMissionITem(int index)
}
}
```
在地图上点击添加waypoint的时候，出现 
QJsonDocument({ 
“data”:”No coveage for 72.9634,24.8653”,”status”:”fail” 
}) 
预计是添加waypoint的位置写入json文件失败。
//点击showallvalues出现 
![这里写图片描述](https://img-blog.csdn.net/20170920121042426?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```
//SimpleMissionItem.cc
static const struct EnumInfo_s _rgMavFrameInfo[]={
    {"MAV_FRAME_GLOBAL",MAV_FRAME_GLOBAL},
    {"MAV_FRAME_LOCAL_NED",MAV_FRAME_LOCAL_NED},
    ...
}
...上面一个MQV_CMD指令太多，是读取json文件来的
```
勾选show all values后 
有个QGCLabel：Provides advanced access to all commands/parameters.Be very careful.
SimpleItemEditor.qml中
```
Rectangle{
    ...
    ...
    Column{
        id:...
        QGCLable{
            width:
            text:missionItem.rawEdit?qsTr("Provides advanced access to all commands/parameters.Be very careful"):missionItem.commandDescription
        }
    }
}
```
