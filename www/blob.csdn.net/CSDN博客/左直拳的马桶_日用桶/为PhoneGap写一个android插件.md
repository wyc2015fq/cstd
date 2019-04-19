# 为PhoneGap写一个android插件 - 左直拳的马桶_日用桶 - CSDN博客
2014年08月14日 22:53:07[左直拳](https://me.csdn.net/leftfist)阅读数：4270标签：[PhoneGap																[Cordova																[Android																[插件](https://so.csdn.net/so/search/s.do?q=插件&t=blog)](https://so.csdn.net/so/search/s.do?q=Android&t=blog)](https://so.csdn.net/so/search/s.do?q=Cordova&t=blog)](https://so.csdn.net/so/search/s.do?q=PhoneGap&t=blog)
个人分类：[Java																[Android																[PhoneGap																[JavaScript](https://blog.csdn.net/leftfist/article/category/94500)](https://blog.csdn.net/leftfist/article/category/2455151)](https://blog.csdn.net/leftfist/article/category/1302161)](https://blog.csdn.net/leftfist/article/category/2412493)
为PhoneGap写一个android插件，要怎么做？
其实这句话应该反过来说，为android写一个PhoneGap插件，要怎么做？
这里以最简单的Hello World!为例，做个说明：
**1、第一步，要先建立一个支持PhoneGap（Cordova）的android工程**
因为这个插件本质上是安卓插件，用于PhoneGap，因此，要二者支持才行，所以我们要建立一个支持PhoneGap（Cordova）的android工程，插件在这个工程里面编写。
```
扫盲：PhoneGap现在已经出售给了Apache基金会，现在改名为Cordova。之所以用这个名字，是因为创建PhoneGap的那个公司，当时位于一条名叫Cordova的街道，大概是为了纪念吧。
现在要下载最新的cordova的话，应该去Cordova官网，而PhoneGap官网停留在了2.9.1。
```
我们采用cordova CLI来进行工程创建。这样会比较方便。否则要先创建安卓工程，然后手动将cordova的东西拷到工程文件夹内，还要修改代码，麻烦得很。
**1）先创建一个cordova项目**
在命令行方式下：
`cordova create hello com.example.test HelloWorld`
这样就在当前路径，创建了一个名为hello的文件夹，里面是cordova的各种文件
**2）设置cordova支持android平台**
在命令行方式下：
```
cd hello
cordova platform add android
```
这样在hello\platforms文件夹下，多了一个android子目录，这个就是我们要的支持phoegap的安卓工程了，插件可以在这个工程里面编写。
**2、编写插件**
**1）Hello.java**
用eclipse打开该工程
在src下新建包及class
Hello.java：
```java
package com.example.test.plugin;
import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaPlugin;
import org.json.JSONArray;
import org.json.JSONException;
public class Hello extends CordovaPlugin{
	 @Override 
	 public boolean execute(String action
			 , JSONArray args
			 , CallbackContext callbackContext) throws JSONException {
		 try{
			 if (action.equals("sayHello")) {
				 callbackContext.success("Hello World!你好，科尔多瓦！");
				 return true;
			 }
		 } catch (Exception e) {
			 callbackContext.error("Oh shit!");
			 return false;
		 }
	    return super.execute(action, args, callbackContext);
	 }
}
```
**2）config.xml**
修改res/xml/config.xml
在节点<widget>里加入：
```
<feature name="Hello">
		<param name="android-package" value="com.example.test.plugin.Hello" />
	</feature>
```
**3、调用插件**
调用是javascript唱独角戏。
**1）添加 js/plugin.js**
```java
var helloPlugin = {
    say: function(successCallback, errorCallback) {
        cordova.exec(
            successCallback, 	// success callback function
            errorCallback, 		// error callback function
            'Hello', 			// mapped to our native Java class called "CalendarPlugin"
            'sayHello', 		// with this action name
            []                 	// and this array of custom arguments to create our entry
        ); 
    }
}
```
**2）修改js/index.js**
修改部分主要在结尾部分
```java
var app = {
    // Application Constructor
    initialize: function() {
        this.bindEvents();
    },
    // Bind Event Listeners
    //
    // Bind any events that are required on startup. Common events are:
    // 'load', 'deviceready', 'offline', and 'online'.
    bindEvents: function() {
        document.addEventListener('deviceready', this.onDeviceReady, false);
    },
    // deviceready Event Handler
    //
    // The scope of 'this' is the event. In order to call the 'receivedEvent'
    // function, we must explicitly call 'app.receivedEvent(...);'
    onDeviceReady: function() {
        app.receivedEvent('deviceready');
    },
    // Update DOM on a Received Event
    receivedEvent: function(id) {
        var parentElement = document.getElementById(id);
        var listeningElement = parentElement.querySelector('.listening');
        var receivedElement = parentElement.querySelector('.received');
        listeningElement.setAttribute('style', 'display:none;');
        receivedElement.setAttribute('style', 'display:block;');
        console.log('Received Event: ' + id);
		app.sayHello();
    },
	sayHello: function(){
		var success = function(message) { alert(message); };
		var error = function(message) { alert("Oopsie! " + message); };		
	    helloPlugin.say(success, error);
	}
};
```
3）修改首页index.html
```
<script type="text/javascript" src="cordova.js"></script>
        <script type="text/javascript" src="js/index.js"></script>
        <script type="text/javascript" src="js/plugin.js"></script>
        <script type="text/javascript">
            app.initialize();
        </script>
```
至此，我们就完成了一个for phonegap的安卓插件的编写和调用过程。
按道理，插件应该独立于具体工程，可以供多个工程调用，但本示例是插件直接写于工程内。独立的方式，我还不知道怎么写。
