# Android基站定位 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年02月11日 15:21:51[boonya](https://me.csdn.net/boonya)阅读数：923标签：[基站定位																[移动																[数据																[移动应用](https://so.csdn.net/so/search/s.do?q=移动应用&t=blog)
个人分类：[Android](https://blog.csdn.net/boonya/article/category/2496125)






原文地址：[http://www.cnblogs.com/rayee/archive/2012/02/02/2336101.html](http://www.cnblogs.com/rayee/archive/2012/02/02/2336101.html)

### 前言


经过前面几节的学习，我们已经对Android程序的开发流程有了个大体的了解，为了提高我们的学习兴趣，在这一节我们将编写一个简单的基站定位程序。现在LBS（Location Based Service，基于位置的服务）移动应用相当流行（如：微信，切客，嘀咕，街旁等），基站定位是这类程序用到的关键性技术之一，我们来揭开它的神秘面纱吧。


在这一节里，我们会接触到事件、TelephonyManager、HTTP通信、JSON的使用等知识点。

### 目录


一、[设置界面](http://www.cnblogs.com/rayee/archive/2012/02/02/2336101.html#section-1)


二、[为按钮绑定事件](http://www.cnblogs.com/rayee/archive/2012/02/02/2336101.html#section-2)


三、[获取基站信息](http://www.cnblogs.com/rayee/archive/2012/02/02/2336101.html#section-3)


四、[获取经纬度](http://www.cnblogs.com/rayee/archive/2012/02/02/2336101.html#section-4)


五、[获取物理位置](http://www.cnblogs.com/rayee/archive/2012/02/02/2336101.html#section-5)


六、[显示结果](http://www.cnblogs.com/rayee/archive/2012/02/02/2336101.html#section-6)


七、[运行程序](http://www.cnblogs.com/rayee/archive/2012/02/02/2336101.html#section-7)


八、[总结](http://www.cnblogs.com/rayee/archive/2012/02/02/2336101.html#section-8)


九、[程序代码](http://www.cnblogs.com/rayee/archive/2012/02/02/2336101.html#section-9)

### 正文


在Android操作系统下，基站定位其实很简单，先说一下实现流程：


调用SDK中的API（TelephonyManager）获得MCC、MNC、LAC、CID等信息，然后通过google的API获得所在位置的经纬度，最后再通过google map的API获得实际的地理位置。（google真牛！）


有同学会问：MNC、MCC、LAC、CID都是些什么东西？google又怎么通过这些东西就获得经纬度了呢？


我们一起来学习一下：

> 
MCC，Mobile Country Code，移动国家代码（中国的为460）；

MNC，Mobile Network Code，移动网络号码（中国移动为00，中国联通为01）；

LAC，Location Area Code，位置区域码；

CID，Cell Identity，基站编号，是个16位的数据（范围是0到65535）。



了解了这几个名词的意思，相信有些朋友已经知道后面的事了：google存储了这些信息，直接查询就能得到经纬度了。（至于google怎么得到移动、联通的基站信息，这就不得而知了，反正google免费提供接口，直接调用就是）


下面开始动手。

#### 一、设置界面


我们在上一节的程序的基础上进行开发，在DemoActivity的界面上实现这个功能。（没有代码的同学可[点击这里下载](http://files.cnblogs.com/rayee/Android_Demo_3_2012_02_01.7z)，感谢[yuejianjun](http://www.cnblogs.com/jianjunyue/)同学的建议，以后我会在每一节的最后把例子打包提供下载）


首先我们将DemoActivity使用的布局修改一下：

![image](http://images.cnblogs.com/cnblogs_com/rayee/201202/201202030936211918.png)


第1行为TextView，显示提示文字；第2行为一个Button，触发事件；第3行、第4行分别显示基站信息和地理位置（现在为空，看不到）。


layout/main.xml文件内容如下：



```
```xml
<?
```

```xml
xml
```

```xml
version="1.0"
 encoding="utf-8"?>
```



```xml
<
```

```xml
LinearLayout
```

```xml
xmlns:android="[http://schemas.android.com/apk/res/android](http://schemas.android.com/apk/res/android)"
```



```xml

```

```xml
android:layout_width="fill_parent"
```



```xml

```

```xml
android:layout_height="fill_parent"
```



```xml

```

```xml
android:orientation="vertical"
 >
```





```xml

```

```xml
<
```

```xml
TextView
```



```xml

```

```xml
android:layout_width="fill_parent"
```



```xml

```

```xml
android:layout_height="wrap_content"
```



```xml

```

```xml
android:text="Please
 click the button below to get your location" />
```





```xml

```

```xml
<
```

```xml
Button
```



```xml

```

```xml
android:id="@+id/button1"
```



```xml

```

```xml
android:layout_width="wrap_content"
```



```xml

```

```xml
android:layout_height="wrap_content"
```



```xml

```

```xml
android:text="Click
 Me" />
```





```xml

```

```xml
<
```

```xml
TextView
```



```xml

```

```xml
android:id="@+id/cellText"
```



```xml

```

```xml
android:layout_width="wrap_content"
```



```xml

```

```xml
android:layout_height="wrap_content"
```



```xml

```

```xml
android:text=""
 />
```





```xml

```

```xml
<
```

```xml
TextView
```



```xml

```

```xml
android:id="@+id/lacationText"
```



```xml

```

```xml
android:layout_width="wrap_content"
```



```xml

```

```xml
android:layout_height="wrap_content"
```



```xml

```

```xml
android:text=""
 />
```





```xml
</
```

```xml
LinearLayout
```

```xml
>
```
```




接下来我们打开DemoActivity.java编写代码。

#### 二、为按钮绑定事件


我们在Activity创建时绑定事件，将以下代码添加到setContentView(R.layout.main);后：



```
```java
/**
 为按钮绑定事件 */
```



```java
Button
 btnGetLocation = (Button)findViewById(R.id.button1);
```



```java
btnGetLocation.setOnClickListener(
```

```java
new
```

```java
OnClickListener()
 {
```



```java

```

```java
@Override
```



```java

```

```java
public
```

```java
void
```

```java
onClick(View
 arg0) {
```



```java

```

```java
//
 TODO Auto-generated method stub
```



```java

```

```java
onBtnClick();
```



```java

```

```java
}
```



```java
});
```
```




同时还需要在头部import相关组件：



```
```java
import
```

```java
android.view.View;
```



```java
import
```

```java
android.widget.Button;
```



```java
import
```

```java
android.view.View.OnClickListener;
```
```




我们来分析一下这段代码：


首先我们通过findViewById(R.id.button1)找到按钮这个对象，前面加(Button)表示显示的转换为Button对象；


然后设置按钮点击事件的监听器，参数为OnClickListener对象，再重载这个类的onClick方法，调用onBtnClick方法（这个方法得由我们自己去写，他在点击按钮时被调用）。


好了，调用方法写好了，我们来写实现（调用后需要做什么事）。动手编码之前先在脑中整理好思路，养成好习惯。


我们需要在DemoActivty类中添加如下私有方法：
- 
我们需要刚刚提到的onBtnClick回调方法，被调用时实现取得基站信息、获取经纬度、获取地理位置、显示的功能。但是很显然，全部揉到一个方法里面并不是个好主意，我们将它分割为几个方法；
- 
添加获取基站信息的方法getCellInfo，返回基站信息；
- 
添加获取经纬度的方法getItude，传入基站信息，返回经纬度；
- 
添加获取地理位置的方法getLocation，传入经纬度，返回地理位置；
- 
添加显示结果的方法showResult，传入得到的信息在界面上显示出来。


好了，先将方法添上，完整代码如下：



```
```java
package
```

```java
com.android.demo;
```





```java
import
```

```java
android.R.bool;
```



```java
import
```

```java
android.R.integer;
```



```java
import
```

```java
android.app.Activity;
```



```java
import
```

```java
android.os.Bundle;
```



```java
import
```

```java
android.view.View;
```



```java
import
```

```java
android.widget.Button;
```



```java
import
```

```java
android.view.View.OnClickListener;
```





```java
public
```

```java
class
```

```java
DemoActivity
```

```java
extends
```

```java
Activity
 {
```



```java

```

```java
/**
 Called when the activity is first created. */
```



```java

```

```java
@Override
```



```java

```

```java
public
```

```java
void
```

```java
onCreate(Bundle
 savedInstanceState) {
```



```java

```

```java
super
```

```java
.onCreate(savedInstanceState);
```



```java

```

```java
setContentView(R.layout.main);
```



```java

```



```java

```

```java
/**
 为按钮绑定事件 */
```



```java

```

```java
Button
 btnGetLocation = (Button)findViewById(R.id.button1);
```



```java

```

```java
btnGetLocation.setOnClickListener(
```

```java
new
```

```java
OnClickListener()
 {
```



```java

```

```java
@Override
```



```java

```

```java
public
```

```java
void
```

```java
onClick(View
 arg0) {
```



```java

```

```java
//
 TODO Auto-generated method stub
```



```java

```

```java
onBtnClick();
```



```java

```

```java
}
```



```java

```

```java
});
```



```java

```

```java
}
```



```java

```



```java

```

```java
/**
 基站信息结构体 */
```



```java

```

```java
public
```

```java
class
```

```java
SCell{
```



```java

```

```java
public
```

```java
int
```

```java
MCC;
```



```java

```

```java
public
```

```java
int
```

```java
MNC;
```



```java

```

```java
public
```

```java
int
```

```java
LAC;
```



```java

```

```java
public
```

```java
int
```

```java
CID;
```



```java

```

```java
}
```



```java

```



```java

```

```java
/**
 经纬度信息结构体 */
```



```java

```

```java
public
```

```java
class
```

```java
SItude{
```



```java

```

```java
public
```

```java
String
 latitude;
```



```java

```

```java
public
```

```java
String
 longitude;
```



```java

```

```java
}
```



```java

```



```java

```

```java
/**
 按钮点击回调函数 */
```



```java

```

```java
private
```

```java
void
```

```java
onBtnClick(){
```



```java

```



```java

```

```java
}
```



```java

```



```java

```

```java
/**
 获取基站信息 */
```



```java

```

```java
private
```

```java
SCell
 getCellInfo(){
```





```java

```

```java
}
```



```java

```



```java

```

```java
/**
 获取经纬度 */
```



```java

```

```java
private
```

```java
SItude
 getItude(SCell cell){
```



```java

```



```java

```

```java
}
```



```java

```



```java

```

```java
/**
 获取地理位置 */
```



```java

```

```java
private
```

```java
String
 getLocation(SItude itude){
```



```java

```



```java

```

```java
}
```



```java

```



```java

```

```java
/**
 显示结果 */
```



```java

```

```java
private
```

```java
void
```

```java
showResult(SCell
 cell, String location){
```



```java

```



```java

```

```java
}
```



```java
}
```
```




现在在onBtnClick方法中编码，依次调用后面几个方法，代码如下：



```
```java
/**
 按钮点击回调函数 */
```



```java
private
```

```java
void
```

```java
onBtnClick(){
```



```java

```

```java
/**
 弹出一个等待状态的框 */
```



```java

```

```java
ProgressDialog
 mProgressDialog =
```

```java
new
```

```java
ProgressDialog(
```

```java
this
```

```java
);
```



```java

```

```java
mProgressDialog.setMessage(
```

```java
"正在获取中..."
```

```java
);
```



```java

```

```java
mProgressDialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
```



```java

```

```java
mProgressDialog.show();
```



```java

```



```java

```

```java
try
```

```java
{
```



```java

```

```java
/**
 获取基站数据 */
```



```java

```

```java
SCell
 cell = getCellInfo();
```



```java

```



```java

```

```java
/**
 根据基站数据获取经纬度 */
```



```java

```

```java
SItude
 itude = getItude(cell);
```



```java

```



```java

```

```java
/**
 获取地理位置 */
```



```java

```

```java
String
 location = getLocation(itude);
```



```java

```



```java

```

```java
/**
 显示结果 */
```



```java

```

```java
showResult(cell,
 location);
```



```java

```



```java

```

```java
/**
 关闭对话框 */
```



```java

```

```java
mProgressDialog.dismiss();
```



```java

```

```java
}
```

```java
catch
```

```java
(Exception
 e) {
```



```java

```

```java
/**
 关闭对话框 */
```



```java

```

```java
mProgressDialog.dismiss();
```



```java

```

```java
/**
 显示错误 */
```



```java

```

```java
TextView
 cellText = (TextView)findViewById(R.id.cellText);
```



```java

```

```java
cellText.setText(e.getMessage());
```



```java

```

```java
}
```



```java
}
```
```




按钮相关的工作就完成了，接下来编写获取基站信息的方法。

#### 三、获取基站信息


获取基站信息我们需要调用SDK提供的API中的TelephonyManager，需要在文件头部引入：



```
```java
import
```

```java
android.telephony.TelephonyManager;
```



```java
import
```

```java
android.telephony.gsm.GsmCellLocation;
```
```




完整代码为：



```
```java
/**
```



```java

```

```java
*
 获取基站信息
```



```java

```

```java
*
```



```java

```

```java
*
 @throws Exception
```



```java

```

```java
*/
```



```java
private
```

```java
SCell
 getCellInfo()
```

```java
throws
```

```java
Exception
 {
```



```java

```

```java
SCell
 cell =
```

```java
new
```

```java
SCell();
```





```java

```

```java
/**
 调用API获取基站信息 */
```



```java

```

```java
TelephonyManager
 mTelNet = (TelephonyManager) getSystemService(Context.TELEPHONY_SERVICE);
```



```java

```

```java
GsmCellLocation
 location = (GsmCellLocation) mTelNet.getCellLocation();
```



```java

```

```java
if
```

```java
(location
 ==
```

```java
null
```

```java
)
```



```java

```

```java
throw
```

```java
new
```

```java
Exception(
```

```java
"获取基站信息失败"
```

```java
);
```





```java

```

```java
String
 operator = mTelNet.getNetworkOperator();
```



```java

```

```java
int
```

```java
mcc
 = Integer.parseInt(operator.substring(
```

```java
0
```

```java
,
```

```java
3
```

```java
));
```



```java

```

```java
int
```

```java
mnc
 = Integer.parseInt(operator.substring(
```

```java
3
```

```java
));
```



```java

```

```java
int
```

```java
cid
 = location.getCid();
```



```java

```

```java
int
```

```java
lac
 = location.getLac();
```





```java

```

```java
/**
 将获得的数据放到结构体中 */
```



```java

```

```java
cell.MCC
 = mcc;
```



```java

```

```java
cell.MNC
 = mnc;
```



```java

```

```java
cell.LAC
 = lac;
```



```java

```

```java
cell.CID
 = cid;
```





```java

```

```java
return
```

```java
cell;
```



```java
}
```
```




如果获得的位置信息为null将抛出错误，不再继续执行。最后将获取的基站信息封装为结构体返回。

#### 四、获取经纬度


在这一步，我们需要采用HTTP调用google的API以获取基站所在的经纬度。


Android作为一款互联网手机，联网的功能必不可少。Android提供了多个接口供我们使用，这里我们使用DefaultHttpClient。


完整的方法代码如下：



```
```java
/**
```



```java

```

```java
*
 获取经纬度
```



```java

```

```java
*
```



```java

```

```java
*
 @throws Exception
```



```java

```

```java
*/
```



```java
private
```

```java
SItude
 getItude(SCell cell)
```

```java
throws
```

```java
Exception
 {
```



```java

```

```java
SItude
 itude =
```

```java
new
```

```java
SItude();
```





```java

```

```java
/**
 采用Android默认的HttpClient */
```



```java

```

```java
HttpClient
 client =
```

```java
new
```

```java
DefaultHttpClient();
```



```java

```

```java
/**
 采用POST方法 */
```



```java

```

```java
HttpPost
 post =
```

```java
new
```

```java
HttpPost(
```

```java
"[http://www.google.com/loc/json](http://www.google.com/loc/json)"
```

```java
);
```



```java

```

```java
try
```

```java
{
```



```java

```

```java
/**
 构造POST的JSON数据 */
```



```java

```

```java
JSONObject
 holder =
```

```java
new
```

```java
JSONObject();
```



```java

```

```java
holder.put(
```

```java
"version"
```

```java
,
```

```java
"1.1.0"
```

```java
);
```



```java

```

```java
holder.put(
```

```java
"host"
```

```java
,
```

```java
"maps.google.com"
```

```java
);
```



```java

```

```java
holder.put(
```

```java
"address_language"
```

```java
,
```

```java
"zh_CN"
```

```java
);
```



```java

```

```java
holder.put(
```

```java
"request_address"
```

```java
,
```

```java
true
```

```java
);
```



```java

```

```java
holder.put(
```

```java
"radio_type"
```

```java
,
```

```java
"gsm"
```

```java
);
```



```java

```

```java
holder.put(
```

```java
"carrier"
```

```java
,
```

```java
"HTC"
```

```java
);
```





```java

```

```java
JSONObject
 tower =
```

```java
new
```

```java
JSONObject();
```



```java

```

```java
tower.put(
```

```java
"mobile_country_code"
```

```java
,
 cell.MCC);
```



```java

```

```java
tower.put(
```

```java
"mobile_network_code"
```

```java
,
 cell.MNC);
```



```java

```

```java
tower.put(
```

```java
"cell_id"
```

```java
,
 cell.CID);
```



```java

```

```java
tower.put(
```

```java
"location_area_code"
```

```java
,
 cell.LAC);
```





```java

```

```java
JSONArray
 towerarray =
```

```java
new
```

```java
JSONArray();
```



```java

```

```java
towerarray.put(tower);
```



```java

```

```java
holder.put(
```

```java
"cell_towers"
```

```java
,
 towerarray);
```





```java

```

```java
StringEntity
 query =
```

```java
new
```

```java
StringEntity(holder.toString());
```



```java

```

```java
post.setEntity(query);
```





```java

```

```java
/**
 发出POST数据并获取返回数据 */
```



```java

```

```java
HttpResponse
 response = client.execute(post);
```



```java

```

```java
HttpEntity
 entity = response.getEntity();
```



```java

```

```java
BufferedReader
 buffReader =
```

```java
new
```

```java
BufferedReader(
```

```java
new
```

```java
InputStreamReader(entity.getContent()));
```



```java

```

```java
StringBuffer
 strBuff =
```

```java
new
```

```java
StringBuffer();
```



```java

```

```java
String
 result =
```

```java
null
```

```java
;
```



```java

```

```java
while
```

```java
((result
 = buffReader.readLine()) !=
```

```java
null
```

```java
)
 {
```



```java

```

```java
strBuff.append(result);
```



```java

```

```java
}
```





```java

```

```java
/**
 解析返回的JSON数据获得经纬度 */
```



```java

```

```java
JSONObject
 json =
```

```java
new
```

```java
JSONObject(strBuff.toString());
```



```java

```

```java
JSONObject
 subjosn =
```

```java
new
```

```java
JSONObject(json.getString(
```

```java
"location"
```

```java
));
```





```java

```

```java
itude.latitude
 = subjosn.getString(
```

```java
"latitude"
```

```java
);
```



```java

```

```java
itude.longitude
 = subjosn.getString(
```

```java
"longitude"
```

```java
);
```



```java

```



```java

```

```java
Log.i(
```

```java
"Itude"
```

```java
,
 itude.latitude + itude.longitude);
```



```java

```



```java

```

```java
}
```

```java
catch
```

```java
(Exception
 e) {
```



```java

```

```java
Log.e(e.getMessage(),
 e.toString());
```



```java

```

```java
throw
```

```java
new
```

```java
Exception(
```

```java
"获取经纬度出现错误:"
```

```java
+e.getMessage());
```



```java

```

```java
}
```

```java
finally
```

```java
{
```



```java

```

```java
post.abort();
```



```java

```

```java
client
 =
```

```java
null
```

```java
;
```



```java

```

```java
}
```



```java

```



```java

```

```java
return
```

```java
itude;
```



```java
}
```
```




代笔中关键的地方都作了注释，同学们还有不理解的举手哈。


在这里采用POST方法将JSON数据发送到googleAPI，google返回JSON数据，我们得到数据后解析，得到经纬度信息。

> 
关于google 基站信息API的官方说明[>>请到这里查看](http://code.google.com/intl/zh-CN/apis/gears/geolocation_network_protocol.html)。


#### 五、获取物理位置


得到经纬度后，我们将之转换为物理地址。


我们仍然使用DefaultHttpClient来调用google地图的API，获得物理信息，不过在这里我们使用GET方法。


完整的方法代码如下：



```
```java
/**
```



```java

```

```java
*
 获取地理位置
```



```java

```

```java
*
```



```java

```

```java
*
 @throws Exception
```



```java

```

```java
*/
```



```java
private
```

```java
String
 getLocation(SItude itude)
```

```java
throws
```

```java
Exception
 {
```



```java

```

```java
String
 resultString =
```

```java
""
```

```java
;
```





```java

```

```java
/**
 这里采用get方法，直接将参数加到URL上 */
```



```java

```

```java
String
 urlString = String.format(
```

```java
"[http://maps.google.cn/maps/geo?key=abcdefg&q=%s](http://maps.google.cn/maps/geo?key=abcdefg&q=%s),%s"
```

```java
,
 itude.latitude, itude.longitude);
```



```java

```

```java
Log.i(
```

```java
"URL"
```

```java
,
 urlString);
```





```java

```

```java
/**
 新建HttpClient */
```



```java

```

```java
HttpClient
 client =
```

```java
new
```

```java
DefaultHttpClient();
```



```java

```

```java
/**
 采用GET方法 */
```



```java

```

```java
HttpGet
 get =
```

```java
new
```

```java
HttpGet(urlString);
```



```java

```

```java
try
```

```java
{
```



```java

```

```java
/**
 发起GET请求并获得返回数据 */
```



```java

```

```java
HttpResponse
 response = client.execute(get);
```



```java

```

```java
HttpEntity
 entity = response.getEntity();
```



```java

```

```java
BufferedReader
 buffReader =
```

```java
new
```

```java
BufferedReader(
```

```java
new
```

```java
InputStreamReader(entity.getContent()));
```



```java

```

```java
StringBuffer
 strBuff =
```

```java
new
```

```java
StringBuffer();
```



```java

```

```java
String
 result =
```

```java
null
```

```java
;
```



```java

```

```java
while
```

```java
((result
 = buffReader.readLine()) !=
```

```java
null
```

```java
)
 {
```



```java

```

```java
strBuff.append(result);
```



```java

```

```java
}
```



```java

```

```java
resultString
 = strBuff.toString();
```





```java

```

```java
/**
 解析JSON数据，获得物理地址 */
```



```java

```

```java
if
```

```java
(resultString
 !=
```

```java
null
```

```java
&&
 resultString.length() >
```

```java
0
```

```java
)
 {
```



```java

```

```java
JSONObject
 jsonobject =
```

```java
new
```

```java
JSONObject(resultString);
```



```java

```

```java
JSONArray
 jsonArray =
```

```java
new
```

```java
JSONArray(jsonobject.get(
```

```java
"Placemark"
```

```java
).toString());
```



```java

```

```java
resultString
 =
```

```java
""
```

```java
;
```



```java

```

```java
for
```

```java
(
```

```java
int
```

```java
i
 =
```

```java
0
```

```java
;
 i < jsonArray.length(); i++) {
```



```java

```

```java
resultString
 = jsonArray.getJSONObject(i).getString(
```

```java
"address"
```

```java
);
```



```java

```

```java
}
```



```java

```

```java
}
```



```java

```

```java
}
```

```java
catch
```

```java
(Exception
 e) {
```



```java

```

```java
throw
```

```java
new
```

```java
Exception(
```

```java
"获取物理位置出现错误:"
```

```java
+
 e.getMessage());
```



```java

```

```java
}
```

```java
finally
```

```java
{
```



```java

```

```java
get.abort();
```



```java

```

```java
client
 =
```

```java
null
```

```java
;
```



```java

```

```java
}
```





```java

```

```java
return
```

```java
resultString;
```



```java
}
```
```




GET方法就比POST方法简单多了，得到的数据同样为JSON格式，解析一下得到物理地址。

#### 六、显示结果


好了，我们已经得到我们想要的信息了，我们把它显示出来，方法代码如下：



```
```java
/**
 显示结果 */
```



```java
private
```

```java
void
```

```java
showResult(SCell
 cell, String location) {
```



```java

```

```java
TextView
 cellText = (TextView) findViewById(R.id.cellText);
```



```java

```

```java
cellText.setText(String.format(
```

```java
"基站信息：mcc:%d,
 mnc:%d, lac:%d, cid:%d"
```

```java
,
```



```java

```

```java
cell.MCC,
 cell.MNC, cell.LAC, cell.CID));
```





```java

```

```java
TextView
 locationText = (TextView) findViewById(R.id.lacationText);
```



```java

```

```java
locationText.setText(
```

```java
"物理位置："
```

```java
+
 location);
```



```java
}
```
```



#### 七、运行程序


我们的编码工作已经完成了。在上面的代码中有些地方需要的引入代码没有提到，下面把完整的代码贴出来：



```
```java
package
```

```java
com.android.demo;
```





```java
import
```

```java
java.io.BufferedReader;
```



```java
import
```

```java
java.io.InputStreamReader;
```





```java
import
```

```java
org.apache.http.HttpEntity;
```



```java
import
```

```java
org.apache.http.HttpResponse;
```



```java
import
```

```java
org.apache.http.client.HttpClient;
```



```java
import
```

```java
org.apache.http.client.methods.HttpGet;
```



```java
import
```

```java
org.apache.http.client.methods.HttpPost;
```



```java
import
```

```java
org.apache.http.entity.StringEntity;
```



```java
import
```

```java
org.apache.http.impl.client.DefaultHttpClient;
```





```java
import
```

```java
org.json.JSONArray;
```



```java
import
```

```java
org.json.JSONObject;
```





```java
import
```

```java
android.app.Activity;
```



```java
import
```

```java
android.app.ProgressDialog;
```



```java
import
```

```java
android.content.Context;
```



```java
import
```

```java
android.os.Bundle;
```



```java
import
```

```java
android.telephony.TelephonyManager;
```



```java
import
```

```java
android.telephony.gsm.GsmCellLocation;
```



```java
import
```

```java
android.util.Log;
```



```java
import
```

```java
android.view.View;
```



```java
import
```

```java
android.widget.Button;
```



```java
import
```

```java
android.widget.TextView;
```



```java
import
```

```java
android.view.View.OnClickListener;
```





```java
public
```

```java
class
```

```java
DemoActivity
```

```java
extends
```

```java
Activity
 {
```



```java

```

```java
/**
 Called when the activity is first created. */
```



```java

```

```java
@Override
```



```java

```

```java
public
```

```java
void
```

```java
onCreate(Bundle
 savedInstanceState) {
```



```java

```

```java
super
```

```java
.onCreate(savedInstanceState);
```



```java

```

```java
setContentView(R.layout.main);
```





```java

```

```java
/**
 为按钮绑定事件 */
```



```java

```

```java
Button
 btnGetLocation = (Button) findViewById(R.id.button1);
```



```java

```

```java
btnGetLocation.setOnClickListener(
```

```java
new
```

```java
OnClickListener()
 {
```



```java

```

```java
@Override
```



```java

```

```java
public
```

```java
void
```

```java
onClick(View
 arg0) {
```



```java

```

```java
//
 TODO Auto-generated method stub
```



```java

```

```java
onBtnClick();
```



```java

```

```java
}
```



```java

```

```java
});
```



```java

```

```java
}
```



```java

```



```java

```

```java
/**
 基站信息结构体 */
```



```java

```

```java
public
```

```java
class
```

```java
SCell{
```



```java

```

```java
public
```

```java
int
```

```java
MCC;
```



```java

```

```java
public
```

```java
int
```

```java
MNC;
```



```java

```

```java
public
```

```java
int
```

```java
LAC;
```



```java

```

```java
public
```

```java
int
```

```java
CID;
```



```java

```

```java
}
```



```java

```



```java

```

```java
/**
 经纬度信息结构体 */
```



```java

```

```java
public
```

```java
class
```

```java
SItude{
```



```java

```

```java
public
```

```java
String
 latitude;
```



```java

```

```java
public
```

```java
String
 longitude;
```



```java

```

```java
}
```



```java

```



```java

```

```java
/**
 按钮点击回调函数 */
```



```java

```

```java
private
```

```java
void
```

```java
onBtnClick()
 {
```



```java

```

```java
/**
 弹出一个等待状态的框 */
```



```java

```

```java
ProgressDialog
 mProgressDialog =
```

```java
new
```

```java
ProgressDialog(
```

```java
this
```

```java
);
```



```java

```

```java
mProgressDialog.setMessage(
```

```java
"正在获取中..."
```

```java
);
```



```java

```

```java
mProgressDialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
```



```java

```

```java
mProgressDialog.show();
```





```java

```

```java
try
```

```java
{
```



```java

```

```java
/**
 获取基站数据 */
```



```java

```

```java
SCell
 cell = getCellInfo();
```





```java

```

```java
/**
 根据基站数据获取经纬度 */
```



```java

```

```java
SItude
 itude = getItude(cell);
```





```java

```

```java
/**
 获取地理位置 */
```



```java

```

```java
String
 location = getLocation(itude);
```





```java

```

```java
/**
 显示结果 */
```



```java

```

```java
showResult(cell,
 location);
```





```java

```

```java
/**
 关闭对话框 */
```



```java

```

```java
mProgressDialog.dismiss();
```



```java

```

```java
}
```

```java
catch
```

```java
(Exception
 e) {
```



```java

```

```java
/**
 关闭对话框 */
```



```java

```

```java
mProgressDialog.dismiss();
```



```java

```

```java
/**
 显示错误 */
```



```java

```

```java
TextView
 cellText = (TextView) findViewById(R.id.cellText);
```



```java

```

```java
cellText.setText(e.getMessage());
```



```java

```

```java
Log.e(
```

```java
"Error"
```

```java
,
 e.getMessage());
```



```java

```

```java
}
```



```java

```

```java
}
```



```java

```



```java

```

```java
/**
```



```java

```

```java
*
 获取基站信息
```



```java

```

```java
*
```



```java

```

```java
*
 @throws Exception
```



```java

```

```java
*/
```



```java

```

```java
private
```

```java
SCell
 getCellInfo()
```

```java
throws
```

```java
Exception
 {
```



```java

```

```java
SCell
 cell =
```

```java
new
```

```java
SCell();
```





```java

```

```java
/**
 调用API获取基站信息 */
```



```java

```

```java
TelephonyManager
 mTelNet = (TelephonyManager) getSystemService(Context.TELEPHONY_SERVICE);
```



```java

```

```java
GsmCellLocation
 location = (GsmCellLocation) mTelNet.getCellLocation();
```



```java

```

```java
if
```

```java
(location
 ==
```

```java
null
```

```java
)
```



```java

```

```java
throw
```

```java
new
```

```java
Exception(
```

```java
"获取基站信息失败"
```

```java
);
```





```java

```

```java
String
 operator = mTelNet.getNetworkOperator();
```



```java

```

```java
int
```

```java
mcc
 = Integer.parseInt(operator.substring(
```

```java
0
```

```java
,
```

```java
3
```

```java
));
```



```java

```

```java
int
```

```java
mnc
 = Integer.parseInt(operator.substring(
```

```java
3
```

```java
));
```



```java

```

```java
int
```

```java
cid
 = location.getCid();
```



```java

```

```java
int
```

```java
lac
 = location.getLac();
```





```java

```

```java
/**
 将获得的数据放到结构体中 */
```



```java

```

```java
cell.MCC
 = mcc;
```



```java

```

```java
cell.MNC
 = mnc;
```



```java

```

```java
cell.LAC
 = lac;
```



```java

```

```java
cell.CID
 = cid;
```





```java

```

```java
return
```

```java
cell;
```



```java

```

```java
}
```



```java

```



```java

```

```java
/**
```



```java

```

```java
*
 获取经纬度
```



```java

```

```java
*
```



```java

```

```java
*
 @throws Exception
```



```java

```

```java
*/
```



```java

```

```java
private
```

```java
SItude
 getItude(SCell cell)
```

```java
throws
```

```java
Exception
 {
```



```java

```

```java
SItude
 itude =
```

```java
new
```

```java
SItude();
```





```java

```

```java
/**
 采用Android默认的HttpClient */
```



```java

```

```java
HttpClient
 client =
```

```java
new
```

```java
DefaultHttpClient();
```



```java

```

```java
/**
 采用POST方法 */
```



```java

```

```java
HttpPost
 post =
```

```java
new
```

```java
HttpPost(
```

```java
"[http://www.google.com/loc/json](http://www.google.com/loc/json)"
```

```java
);
```



```java

```

```java
try
```

```java
{
```



```java

```

```java
/**
 构造POST的JSON数据 */
```



```java

```

```java
JSONObject
 holder =
```

```java
new
```

```java
JSONObject();
```



```java

```

```java
holder.put(
```

```java
"version"
```

```java
,
```

```java
"1.1.0"
```

```java
);
```



```java

```

```java
holder.put(
```

```java
"host"
```

```java
,
```

```java
"maps.google.com"
```

```java
);
```



```java

```

```java
holder.put(
```

```java
"address_language"
```

```java
,
```

```java
"zh_CN"
```

```java
);
```



```java

```

```java
holder.put(
```

```java
"request_address"
```

```java
,
```

```java
true
```

```java
);
```



```java

```

```java
holder.put(
```

```java
"radio_type"
```

```java
,
```

```java
"gsm"
```

```java
);
```



```java

```

```java
holder.put(
```

```java
"carrier"
```

```java
,
```

```java
"HTC"
```

```java
);
```





```java

```

```java
JSONObject
 tower =
```

```java
new
```

```java
JSONObject();
```



```java

```

```java
tower.put(
```

```java
"mobile_country_code"
```

```java
,
 cell.MCC);
```



```java

```

```java
tower.put(
```

```java
"mobile_network_code"
```

```java
,
 cell.MNC);
```



```java

```

```java
tower.put(
```

```java
"cell_id"
```

```java
,
 cell.CID);
```



```java

```

```java
tower.put(
```

```java
"location_area_code"
```

```java
,
 cell.LAC);
```





```java

```

```java
JSONArray
 towerarray =
```

```java
new
```

```java
JSONArray();
```



```java

```

```java
towerarray.put(tower);
```



```java

```

```java
holder.put(
```

```java
"cell_towers"
```

```java
,
 towerarray);
```





```java

```

```java
StringEntity
 query =
```

```java
new
```

```java
StringEntity(holder.toString());
```



```java

```

```java
post.setEntity(query);
```





```java

```

```java
/**
 发出POST数据并获取返回数据 */
```



```java

```

```java
HttpResponse
 response = client.execute(post);
```



```java

```

```java
HttpEntity
 entity = response.getEntity();
```



```java

```

```java
BufferedReader
 buffReader =
```

```java
new
```

```java
BufferedReader(
```

```java
new
```

```java
InputStreamReader(entity.getContent()));
```



```java

```

```java
StringBuffer
 strBuff =
```

```java
new
```

```java
StringBuffer();
```



```java

```

```java
String
 result =
```

```java
null
```

```java
;
```



```java

```

```java
while
```

```java
((result
 = buffReader.readLine()) !=
```

```java
null
```

```java
)
 {
```



```java

```

```java
strBuff.append(result);
```



```java

```

```java
}
```





```java

```

```java
/**
 解析返回的JSON数据获得经纬度 */
```



```java

```

```java
JSONObject
 json =
```

```java
new
```

```java
JSONObject(strBuff.toString());
```



```java

```

```java
JSONObject
 subjosn =
```

```java
new
```

```java
JSONObject(json.getString(
```

```java
"location"
```

```java
));
```





```java

```

```java
itude.latitude
 = subjosn.getString(
```

```java
"latitude"
```

```java
);
```



```java

```

```java
itude.longitude
 = subjosn.getString(
```

```java
"longitude"
```

```java
);
```



```java

```



```java

```

```java
Log.i(
```

```java
"Itude"
```

```java
,
 itude.latitude + itude.longitude);
```



```java

```



```java

```

```java
}
```

```java
catch
```

```java
(Exception
 e) {
```



```java

```

```java
Log.e(e.getMessage(),
 e.toString());
```



```java

```

```java
throw
```

```java
new
```

```java
Exception(
```

```java
"获取经纬度出现错误:"
```

```java
+e.getMessage());
```



```java

```

```java
}
```

```java
finally
```

```java
{
```



```java

```

```java
post.abort();
```



```java

```

```java
client
 =
```

```java
null
```

```java
;
```



```java

```

```java
}
```



```java

```



```java

```

```java
return
```

```java
itude;
```



```java

```

```java
}
```



```java

```



```java

```

```java
/**
```



```java

```

```java
*
 获取地理位置
```



```java

```

```java
*
```



```java

```

```java
*
 @throws Exception
```



```java

```

```java
*/
```



```java

```

```java
private
```

```java
String
 getLocation(SItude itude)
```

```java
throws
```

```java
Exception
 {
```



```java

```

```java
String
 resultString =
```

```java
""
```

```java
;
```





```java

```

```java
/**
 这里采用get方法，直接将参数加到URL上 */
```



```java

```

```java
String
 urlString = String.format(
```

```java
"[http://maps.google.cn/maps/geo?key=abcdefg&q=%s](http://maps.google.cn/maps/geo?key=abcdefg&q=%s),%s"
```

```java
,
 itude.latitude, itude.longitude);
```



```java

```

```java
Log.i(
```

```java
"URL"
```

```java
,
 urlString);
```





```java

```

```java
/**
 新建HttpClient */
```



```java

```

```java
HttpClient
 client =
```

```java
new
```

```java
DefaultHttpClient();
```



```java

```

```java
/**
 采用GET方法 */
```



```java

```

```java
HttpGet
 get =
```

```java
new
```

```java
HttpGet(urlString);
```



```java

```

```java
try
```

```java
{
```



```java

```

```java
/**
 发起GET请求并获得返回数据 */
```



```java

```

```java
HttpResponse
 response = client.execute(get);
```



```java

```

```java
HttpEntity
 entity = response.getEntity();
```



```java

```

```java
BufferedReader
 buffReader =
```

```java
new
```

```java
BufferedReader(
```

```java
new
```

```java
InputStreamReader(entity.getContent()));
```



```java

```

```java
StringBuffer
 strBuff =
```

```java
new
```

```java
StringBuffer();
```



```java

```

```java
String
 result =
```

```java
null
```

```java
;
```



```java

```

```java
while
```

```java
((result
 = buffReader.readLine()) !=
```

```java
null
```

```java
)
 {
```



```java

```

```java
strBuff.append(result);
```



```java

```

```java
}
```



```java

```

```java
resultString
 = strBuff.toString();
```





```java

```

```java
/**
 解析JSON数据，获得物理地址 */
```



```java

```

```java
if
```

```java
(resultString
 !=
```

```java
null
```

```java
&&
 resultString.length() >
```

```java
0
```

```java
)
 {
```



```java

```

```java
JSONObject
 jsonobject =
```

```java
new
```

```java
JSONObject(resultString);
```



```java

```

```java
JSONArray
 jsonArray =
```

```java
new
```

```java
JSONArray(jsonobject.get(
```

```java
"Placemark"
```

```java
).toString());
```



```java

```

```java
resultString
 =
```

```java
""
```

```java
;
```



```java

```

```java
for
```

```java
(
```

```java
int
```

```java
i
 =
```

```java
0
```

```java
;
 i < jsonArray.length(); i++) {
```



```java

```

```java
resultString
 = jsonArray.getJSONObject(i).getString(
```

```java
"address"
```

```java
);
```



```java

```

```java
}
```



```java

```

```java
}
```



```java

```

```java
}
```

```java
catch
```

```java
(Exception
 e) {
```



```java

```

```java
throw
```

```java
new
```

```java
Exception(
```

```java
"获取物理位置出现错误:"
```

```java
+
 e.getMessage());
```



```java

```

```java
}
```

```java
finally
```

```java
{
```



```java

```

```java
get.abort();
```



```java

```

```java
client
 =
```

```java
null
```

```java
;
```



```java

```

```java
}
```





```java

```

```java
return
```

```java
resultString;
```



```java

```

```java
}
```



```java

```



```java

```

```java
/**
 显示结果 */
```



```java

```

```java
private
```

```java
void
```

```java
showResult(SCell
 cell, String location) {
```



```java

```

```java
TextView
 cellText = (TextView) findViewById(R.id.cellText);
```



```java

```

```java
cellText.setText(String.format(
```

```java
"基站信息：mcc:%d,
 mnc:%d, lac:%d, cid:%d"
```

```java
,
```



```java

```

```java
cell.MCC,
 cell.MNC, cell.LAC, cell.CID));
```





```java

```

```java
TextView
 locationText = (TextView) findViewById(R.id.lacationText);
```



```java

```

```java
locationText.setText(
```

```java
"物理位置："
```

```java
+
 location);
```



```java

```

```java
}
```



```java
}
```
```




我们连上手机在手机上运行程序看看。


不出意外的话程序运行起来了，自动跳转到了主界面。点击“Click Me”，出错了！

![image](http://images.cnblogs.com/cnblogs_com/rayee/201202/201202030936234657.png)


详细的错误信息为：Neither user 10078 nor current process has android.permission.ACCESS_COARSE_LOCATION.


原来是没有权限，经过前面的学习，我们知道Android在应用的安全上下了一番功夫，要用一些特殊功能必须先报告，安装应用的时候列给用户看，必须要得到用户的允许。这里我们用了获取基站信息的功能，涉及到用户的隐私了，所以我们必须申明一下。


打开AndroidManifest.xml配置文件，在里面添加相应的配置信息：



```
```xml
<
```

```xml
uses-permission
```

```xml
android:name="android.permission.ACCESS_FINE_LOCATION"></
```

```xml
uses-permission
```

```xml
>
```
```




我们继续把网络连接的权限申明也加上：



```
```xml
<
```

```xml
uses-permission
```

```xml
android:name="android.permission.INTERNET"></
```

```xml
uses-permission
```

```xml
>
```
```




再编译运行看看（点击“Click Me”后程序会卡住，等待一段时间才有反应，取决于网络情况）：

![豌豆荚截图201202021637](http://images.cnblogs.com/cnblogs_com/rayee/201202/201202030936258443.png)


成功啦！


可能有的同学还是出现错误，没有成功：


█ 提示“www.google.com…”什么的错误


请确认你的手机能访问互联网，调用google的API是必须联网的。


█ 提示获取不到基站信息


你确定你是在手机上测试的吗？模拟器可不行哦。或者你的手机使用的CMDA网络？这个例子只支持GSM网络…


█ 获取不到经纬度


很有可能你中奖了，你所在的基站还没纳入google的数据库…（话说我之前也遇到过，怎么查就是查不出经纬度来，返回数据为空）


█ 获取到的地理地址不正确


这个可能程序出错了，可能google出错了？


其实google map API返回的数据中还包含了很多其他信息，我们可以用来开发一些更有趣的功能，如制作我们专属的地图软件、足迹记录软件等，充分发挥你的创造力：）

#### 八、总结


这个程序基本实现了基站定位功能，但还有很多问题，如：点击了按钮后界面会卡住（访问网络时阻塞了进程）、未对异常进一步处理、不兼容CMDA网络等。


另外这个程序的精度也不够，获得的位置实际上是基站的物理位置，与人所在的位置还有一定差距。在城市里面，一般采用密集型的小功率基站，精度一般在几百米范围内，而在郊区常为大功率基站，密度很小，精度一般在几千米以上。


想要取得更高的精度需要通过一些其他的算法来实现，如果大家有兴趣的话我们可以一起来研究一下，再专门写篇笔记。


可见写一段程序和做一个实际的产品是有很大差别的。

#### 九、程序代码


这一节完整程序的[请点击这里下载](http://files.cnblogs.com/rayee/Android_Demo_4_2012_02_02.7z)。

### 结尾


这一节基本实现了最简单的基站定位，只是作为学习的例子，远远达不到产品的要求，请大家见谅。


我们进一步熟悉了JAVA编码，之前没怎么接触JAVA看起来有点吃力的同学建议找点JAVA基础的书来看看。


话说我这段代码也是写得乱七八糟，JAVA没系统学习过，不知道JAVA编程的习惯，命名规则等，大家见笑了。


相对前面几节来说这一节没那么详细了，我也很困惑：详细点呢进度很慢，进度提上去了又怕不够详细，看看大家的意见，请留言哈。


下一节我们回归理论知识，一起深入学习Android的Activity组件。


如果你喜欢看到这些文字，请点击右下角的“推荐”支持我一下，谢谢！](https://so.csdn.net/so/search/s.do?q=数据&t=blog)](https://so.csdn.net/so/search/s.do?q=移动&t=blog)](https://so.csdn.net/so/search/s.do?q=基站定位&t=blog)




