# HTML5晃动DeviceMotionEvent事件 - z69183787的专栏 - CSDN博客
2016年01月31日 22:41:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1809
### 关于devicemotion
html5提供了几个新的DOM事件来获得设备物理方向及运动的信息，包括：陀螺仪、罗盘及加速计。
第一个DOM事件是**deviceorientation**，其提供设备的物理方向信息，表示为一系列本地坐标系的旋角。
第二个DOM事件是**devicemotion**，其提供设备的加速信息，表示为定义在设备上的坐标系中的卡尔迪坐标。其还提供了设备在坐标系中的自转速率。
第三个DOM事件是**compassneedscalibration**，其用于通知Web站点使用罗盘信息校准上述事件。
### 原理
开发者从各个内置传感器那里获取未经修改的传感数据，并观测或响应设备各种运动和角度变化。这些传感器包括陀螺仪、加速器和磁力仪(罗盘)。
加速器和陀螺仪的数据都是描述沿着iOS设备三个方向轴上的位置，对于一个竖屏摆放的iPhone来说，X方向从设备的左边(负)到右边(正)，Y方向则是由设备的底部(-)到顶部(+)，而Z方向为垂直于屏幕由设备的背面(-)到正面(+)。
![](http://segmentfault.com/img/bVkeJo)
DeviceMotionEvent 会在设备发生有意义的摆动(或运动)时产生.事件对象封装有产生的间距值,旋转率,和设备加速度.
加速度的计算方式是重力和用户产生的两个加速度矢量之和.设备是通过 陀螺仪和加速计来区别这两者的.
通过DeviceMotion对设备运动状态的判断，则可以帮助我们在网页上就实现“摇一摇”的交互效果。
### 事件监听
```
if ((window.DeviceMotionEvent) {
  window.addEventListener('devicemotion', deviceMotionHandler, false);
} else {
  document.getElementById("dmEvent").innerHTML = "Not supported on your device."
}
```
### 捕捉重力加速度
```
var acceleration = eventData.accelerationIncludingGravity;
```
### 摇一摇demo
```java
```
var SHAKE_THRESHOLD = 800;
var last_update = 0;
var x, y, z, last_x, last_y, last_z;       
function deviceMotionHandler(eventData) {        
  var acceleration =eventData.accelerationIncludingGravity;
  var curTime = new Date().getTime();       
  if ((curTime - last_update)> 300) {                
      var diffTime = curTime -last_update;
      last_update = curTime;       
      x = acceleration.x;
      y = acceleration.y;
      z = acceleration.z;       
      var speed = Math.abs(x +y + z - last_x - last_y - last_z) / diffTime * 10000;          
           if (speed > SHAKE_THRESHOLD) {
                alert("shaked!");
      }
      last_x = x;
      last_y = y;
      last_z = z;
    }
}
```
```
### 另外一个栗子~
```java
```
function deviceMotionHandler(eventData) {
  // 捕捉重力加速度
  var acceleration = eventData.accelerationIncludingGravity;     
  // 打印加速数据
  var rawAcceleration = "[" +  Math.round(acceleration.x) + ", " +Math.round(acceleration.y) + ", " + Math.round(acceleration.z) + "]";
// Z轴,可知设备朝上或者朝下
  var facingUp = -1;
  if (acceleration.z > 0) {
    facingUp = +1;
  }     
  // 根据重力通过 acceleration.x|y 转换得到加速值,
  // 运用重力加速度9.81来计算得到一个百分比然后乘以转换角度90
  var tiltLR = Math.round(((acceleration.x) / 9.81) * -90);
  var tiltFB = Math.round(((acceleration.y + 9.81) / 9.81) * 90 * facingUp);     
  // 打印加速度的计算结果
  document.getElementById("moAccel").innerHTML = rawAcceleration;
  document.getElementById("moCalcTiltLR").innerHTML = tiltLR;
  document.getElementById("moCalcTiltFB").innerHTML = tiltFB;     
  // 将2D和3D的转换应用到图片上
  var rotation = "rotate(" + tiltLR + "deg) (1,0,0, " + (tiltFB) + "deg)";
  document.getElementById("imgLogo").style.webkitTransform = rotation;
}
```
```
