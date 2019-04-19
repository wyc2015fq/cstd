# QGC之MissionManager - LC900730的博客 - CSDN博客
2017年09月18日 14:31:13[lc900730](https://me.csdn.net/LC900730)阅读数：531
不仅仅是MissionController、还有GeoFenceController、还有 
RallyPointController 
都是qmlRegisterUncreatable类型
```
//不需要创建对象，直接在qml中使用？
qmlRegisterUncreatable<MissionController>
(
"QGroundControl.Controllers",1,0,"MissionControlle"
)
```
masterController是PlanMasterController.cc的
```java
PlanMasterController {
        id: masterController
        Component.onCompleted: {
            start(true /* editMode */)
            setCurrentItem(0, true)
        }
        function upload() {
            if (_activeVehicle && _activeVehicle.armed && _activeVehicle.flightMode === _activeVehicle.missionFlightMode) {
                _qgcView.showDialog(activeMissionUploadDialogComponent, qsTr("Plan Upload"), _qgcView.showDialogDefaultWidth, StandardButton.Cancel)
            } else {
                sendToVehicle()
            }
        }
        function loadFromSelectedFile() {
            fileDialog.title =          qsTr("Select Plan File")
            fileDialog.selectExisting = true
            fileDialog.nameFilters =    masterController.loadNameFilters
            fileDialog.openForLoad()
        }
        function saveToSelectedFile() {
            fileDialog.title =          qsTr("Save Plan")
            fileDialog.selectExisting = false
            fileDialog.nameFilters =    masterController.saveNameFilters
            fileDialog.openForSave()
        }
        function fitViewportToItems() {
            mapFitFunctions.fitMapViewportToMissionItems()
        }
    }
```
### start()
在PlanMasterController.cc中
```
void PlanMasterController::start(bool editMode)
{
    _editMode = editMode;
    _missionController.start(editMode);
    _geoFenceController.start(editMode);
    _rallyPointController.start(editMode);
    connect(_multiVehicleMgr, &MultiVehicleManager::activeVehicleChanged, this, &PlanMasterController::_activeVehicleChanged);
    _activeVehicleChanged(_multiVehicleMgr->activeVehicle());
}
```
