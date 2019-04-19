# skyline三维地图与arcgis二维地图联动 - 左直拳的马桶_日用桶 - CSDN博客
2017年10月21日 17:37:43[左直拳](https://me.csdn.net/leftfist)阅读数：1712
无论是二维控制三维，还是三维控制二维，都是通过事件来触发，只不过各自的事件有所区别就是了。
其中二维控制三维，可以通过
> 
“onMouseUp” “onMouseWheel” “onExtentChange”
这三个事件；
而三维控制二维，可以通过
> 
“OnLButtonUp” “OnMouseWheel”
这两个事件。
此外，还需要设置一个全局变量，记录当前控制权是在二维还是三维，不然会乱套。
上代码：
```
(function () {
    var map = parent.map;//二维地图。二维、三维各自放在一个iframe里。
    var sgworld = parent.sgworld;//三维对象
    var token = parent.token;//当前控制权在二维还是三维？
    //二维控制三维
    dojo.connect(map, "onMouseUp", function () {
        token.set2D();//当前控制权在二维
    });
    dojo.connect(map, "onMouseWheel", function () {
        token.set2D();
    });
    dojo.connect(map, "onExtentChange", function(eve) {
        if (token.is3D()) return;
        var pointLB = utils.xyToLngLat(eve.xmin, eve.ymin);//左下
        var pointRT = utils.xyToLngLat(eve.xmax, eve.ymax);//右上
        var xMax = pointRT[0];
        var yMax = pointRT[1];
        var xMin = pointLB[0];
        var yMin = pointLB[1];
        _move3DMapTo(xMax, yMax, xMin, yMin);
    });
    //三维控制二维
    sgworld.AttachEvent("OnLButtonUp", function () {
        token.set3D();//当前控制权在三维
        _move2DMapTo();
    });
    sgworld.AttachEvent("OnMouseWheel", function () {
        token.set3D();
        _move2DMapTo();
    });
    function _move2DMapTo() {//二维跟随移动
        var rect = sgworld.Window.Rect;
        var positionMax = sgworld.Window.PixelToWorld(0, rect.Height - 1, -1).Position;
        var positionMin = sgworld.Window.PixelToWorld(rect.Width - 1, 0, -1).Position;
        var extent = new esri.geometry.Extent({ "xmax": positionMin.X, "ymax": positionMin.Y, "xmin": positionMax.X, "ymin": positionMax.Y });
        map.setExtent(extent);
    }
    function _move3DMapTo(xMax, yMax, xMin, yMin) {//三维跟随移动
        var lgtd = (xMax + xMin) / 2;
        var lttd = (yMax + yMin) / 2;
        var altitude = sgworld.CoordServices.GetDistance(xMax, yMax, xMin, yMin);
        var altitudeTypeCode = 0;
        var yaw = 0.0;
        var pitch = -90;
        var roll = 0.0;
        var distance = sgworld.CoordServices.GetDistance(xMax, yMax, xMin, yMin);
        var pPosition = sgworld.Creator.CreatePosition(lgtd, lttd, altitude, altitudeTypeCode, yaw, pitch, roll, distance);
        sgworld.Navigate.SetPosition(pPosition);
    }
})();
```
