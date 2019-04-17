# 混合开发经验之谈-4TH - DEVELOPER - CSDN博客





2018年01月10日 17:18:00[学术袁](https://me.csdn.net/u012827205)阅读数：177
所属专栏：[Android-开发](https://blog.csdn.net/column/details/20398.html)









![这里写图片描述](https://img-blog.csdn.net/20180110171149806?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### CORDOVA 建议
- 建议在都博客之前可以到这里做一下功课
[可以先到 CORDOVA 官方网站，对立面的知识都通读一下](https://cordova.apache.org/docs/en/latest/guide/overview/index.html)
[然后到这里学习一下 JavaScript 知识](http://www.itxueyuan.org/javascript/jiaocheng_1/)

接下来进入在本人了解-学习使用 CORDOVA 过程中，使用-遇到的问题，进行全面笔记记录。
### 摘要
- 自定义插件 —— 命令
- 自定义插件 —— 为什么？
- 自定义插件 —— url页面加载
- 自定义插件 —— 详述
- 自定义插件 —— 注意

#### 自定义插件 （命令）

**生成 CORDOVA 项目的命令：**`cordova create [项目名] [包名]`

_ 其中 [项目名] 表示 app的名字，而 [包名] 表示 app的包名；如命令>> `cordova create CordovaDemo com.ztman.test`

**增加 CORDOVA 项目的 平台命令：**`cordova platform add [系统平台名][@version]`

_ 其中 [平台名] 表示 移动应用的系统平台；**[@version] (可有可无)**表示在cordova项目平台上的系统平台版本，如果没有指定[@version]，则所添加的系统平台的版本号会是最新，当然最新的系统平台下的gradle也会是最新的，可能会与你目前使用的IDE不匹配。所以在添加平台时，可以优先考虑一下需要的哪种平台版本更合适；如命令>> `cordova platform add android@6.2.1 --save`   or    `cordova platform add ios --save`
**删除CORDOVA 项目中 平台命令 **`ordova platform rm [平台名] --save`

_ 其中 [平台名] 表示 移动应用的系统平台；如命令>> `cordova platform rm android --save`   or    `cordova platform rm ios --save`
**编译 CORDOVA 移动项目的命令：**`cordova build android`
**运行 CORDOVA 移动项目的命令：**`cordova run android`

自定义Cordova中的Android插件（需要安装plugman,Cordova需要用这个来创建插件） 
**安装plugman的命令：**`npm install -g plugman`
**创建自定义插件的命令： **`plugman create --name [插件名] --plugin_id [插件ID] --plugin_version [插件版本号]`

_ 其中 [插件名] 表示 自定义插件在config.xml 中的标签plugin下的name属性，[插件ID] 表示 自定义插件在plugin.xml 中的标签plugin下的 id 属性；如命令>> `plugman create --name CordovaPluginToast --plugin_id com.ztman.test.plugin --plugin_version 1.0.0`
**向项目中添加自定义插件命令：**`cordova plugin add [插件的绝对地址] --save`
**项目移除自定义插件命令：**`cordova plugin rm [插件的id] --save`
#### 自定义插件 （为什么？）

什么是自定义**插件**，它的**作用**是什么？A plugin is a package of injected code that allows the Cordova webview within which your app renders to communicate with the native platform on which it runs. （自定义插件是一个注入代码包，允许您的应用程序呈现的Cordova webview与其运行的本地平台进行通信。） 

作为一个自定义的插件，类似一个独立的依赖库，通过引入这个依赖库，我们可以轻松的调用自定义插件的方法，来完成与原生代码的交互通信。而对自定义插件进行调用的位置，是在 js 中进行的。 

所以，使用 CORDOVA 这种方式的混合开发模式，实现 app 的一些功能需求，关键就在自定义插件！而我在学习过程一直存在一个疑问，就是自定义插件怎么使用？不是其方法上的使用，而是都在什么地方使用，怎么在这些地方使用？？正如上面所说，自定义的插件类似一个独立的依赖库，引入之后，在 js（比如js：[项目名]/www/js/index.js） 中就能轻松的调用自定义插件的方法完成与原生代码的交互通信，从而达到功能上的目的。 
[官网上提供的插件，通过输入你想要使用的插件名字，插件在使用上的介绍很清楚；](https://cordova.apache.org/plugins/?q=dialog&platforms=cordova-android)
#### 自定义插件（url页面加载）

修改默认加载页面 

CORDOVA 应用的 MainActivity.java 有个 loadUrl(launchUrl) 的方法，将launchUrl 修改为远程网站的地址，即可加载远程网站，例如：loadUrl(“[http://www.aikaoen.com](http://www.aikaoen.com)“)；

或者修改项目的根文件夹里的 config.xml 文件，将 content src 指定为远程网站的地址 `<content src="http://www.aikaoen.com" />` 这样运行后，直接在应用内部加载该网页。此时网站中点击链接跳转会调用系统自带的浏览器打开。

再或者重写shouldOverrideUrlLoading方法，[这里可以帮助你](http://blog.csdn.net/zfy865628361/article/details/50151557)，

```java
@Override    
public boolean shouldOverrideUrlLoading(WebView view, String url) 
{
      //return parentEngine.client.onNavigationAttempt(url);       
      view.loadUrl(url);        
      return true;   
 }
```

#### 自定义插件 （详述）

接下来就详细走一下过程，深刻的理解一下：[点击进入，查看源码](https://gitee.com/yuan1530702811/CordovaHybrid2)或者[这个源码](https://gitee.com/yuan1530702811/CordovaHybrid3)
![这里写图片描述](https://img-blog.csdn.net/20180112101415793?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

生成的项目文件结构如下： 
![这里写图片描述](https://img-blog.csdn.net/20180112101534282?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
此时，因为并没有添加任何的系统平台，所以这时 platforms 文件下是空的。然后我们添加一个Android系统平台：

![这里写图片描述](https://img-blog.csdn.net/20180112101822558?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后，可以看到 platforms/android 文件夹下面的目录结构： 
![这里写图片描述](https://img-blog.csdn.net/20180112102203479?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

当然，到这里我们如果执行命令：`cordova build android 或者直接 cordova run android` 就可以运行这个项目显示在手机上。cordova 下 android 运行效果展示如下： 
![cordova 下 android 运行展示 ](https://img-blog.csdn.net/20180112102725252?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
添加自定义的插件，比如是一个吐司功能。在安装过 **plugman** 之后，执行命令 `plugman create --name CordovaPluginToast --plugin_id com.ztman.test.xiao.plugin --plugin_version 1.0.0`

![这里写图片描述](https://img-blog.csdn.net/20180112104305863?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

看一下结构： 
![这里写图片描述](https://img-blog.csdn.net/20180112104548213?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
一起观察一下自定义插件的文件结构： 
![这里写图片描述](https://img-blog.csdn.net/20180112104729600?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

自动生成的plugin.xml 文件的目录结构：
```java
<?xml version='1.0' encoding='utf-8'?>
<plugin id="com.ztman.test.xiao.plugin" version="1.0.0" xmlns="http://apache.org/cordova/ns/plugins/1.0" xmlns:android="http://schemas.android.com/apk/res/android">
    <name>CordovaPluginToast</name>
    <js-module name="CordovaPluginToast" src="www/CordovaPluginToast.js">
        <clobbers target="cordova.plugins.CordovaPluginToast" />
    </js-module>
</plugin>
```

第一步：自定义插件类 

在目录 `xiaoming\CordovaPluginToast\src` 下创建文件目录和java文件，创建的java文件是这样的目录 ：`xiaoming/CordovaPluginToast/src/android/CordovaPluginToast.java`

```java
package com.ztman.test.xiao;
import android.os.Bundle;
import android.widget.Toast;
import android.app.Activity;
import org.apache.cordova.*;
import org.json.JSONArray;
import org.json.JSONException;

public class CordovaPluginToast extends CordovaPlugin {

    @Override
    public boolean execute(String action, CordovaArgs args, CallbackContext callbackContext) throws JSONException {
        //test 是你在JS中调用的方法名；

        if (action.equals("test")) {
            //这里可以实现一些你的原生逻辑功能
            Toast.makeText(cordova.getActivity(), "青青子衿，悠悠我心。", Toast.LENGTH_SHORT).show();
            return true;
        }

        return false;
    }
}
```

第二步：完善 plugin.xml 文件内容

![这里写图片描述](https://img-blog.csdn.net/20180116162534117?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

添加android平台之后的 plugin.xml 的文件目录结构：
```java
<?xml version='1.0' encoding='utf-8'?>
<plugin id="com.ztman.test.xiao.plugin" version="1.0.0" xmlns="http://apache.org/cordova/ns/plugins/1.0" xmlns:android="http://schemas.android.com/apk/res/android">
    <name>CordovaPluginToast</name>
    <js-module name="CordovaPluginToast" src="www/CordovaPluginToast.js">
        <clobbers target="CordovaPluginToast" />
    </js-module>


<!-- 添加 android 平台-平台类型-->
    <platform name="android"> 
        <config-file target="res/xml/config.xml" parent="/*">  
            <!--JS调用时的前缀名字 -->
            <feature name="CordovaPluginToast">  
                  <!--.java类名全路径-->
                <param name="android-package" value="com.ztman.test.xiao.CordovaPluginToast"/> 
            </feature>  
        </config-file>  
        <!--src:java源文件的路径， target-dir:插件安装好后，源文件的位置，要和上面的包名对应 -->
        <source-file src="src/android/CordovaPluginToast.java" target-dir="src/com/ztman/test/xiao" />  

        <config-file target="AndroidManifest.xml" parent="/*">  
            <!--添加权限，这个添加的是弹窗的权限-->
              <uses-permission android:name="android.permission.SYSTEM_ALERT_WINDOW" />  
        </config-file>
    </platform>
</plugin>
```

第三步：按照自己的需求修改 CordovaPluginToast.js 文件逻辑

```java
var exec = require('cordova/exec');

exports.test = function (arg0) {
    //alert("---function---");
    exec(null, null, "CordovaPluginToast", "test", [arg0]);
};
```

第四步：给项目添加自定义的插件 

在添加之前需要，使用切换到目录 CordovaPluginToast 下，并使用命令 `npm init` ，然后一路 enter 就ok。

![这里写图片描述](https://img-blog.csdn.net/20180112113136039?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后会在你的CordovaPluginToast 目录下生出一个新的 .json 文件： 
![这里写图片描述](https://img-blog.csdn.net/20180112113648630?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

修改并填充一些信息 package.json 文件的内容，可依据 plugin.xml 内容进行修改：
```java
{
  "name": "cordovaplugintoast",
  "version": "1.0.0",
  "description": "this demo just a test .",
  "cordova": {
    "id": "com.ztman.test.xiao",
    "platforms": [
      "android"
    ]
  },

  "main": "index.js",
  "scripts": {
    "test": "echo \"Error: no test specified\" && exit 1"
  },
  "author": "xiaoming",
  "license": "ISC"
}
```

为项目添加刚自定义的插件，切换到 ./platforms/android，使用命令：`cordova plugin add [插件目录绝对地址]`

![这里写图片描述](https://img-blog.csdn.net/20180112125806716?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

看到这里说明就执行成功了！当然如果想要调用一下，我们还要做最后一步操作：
- 移除自定义的插件 `cordova plugin rm [插件id]`
- 修改 index.js 文件
- 添加自定义插件 `cordova plugin add [插件绝对地址]`

![这里写图片描述](https://img-blog.csdn.net/20180112132614651?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```java
var app = {
    // Application Constructor
    initialize: function() {
        document.addEventListener('deviceready', this.onDeviceReady.bind(this), false);
    },

    // deviceready Event Handler
    //
    // Bind any cordova events here. Common events are:
    // 'pause', 'resume', etc.
    onDeviceReady: function() {
        //this.receivedEvent('deviceready');
        //在这里调用自定义的控件
        //alert("UUUU");

        CordovaPluginToast.test("KKKKKKK");

        //alert("MMMMMM");
    },

    // Update DOM on a Received Event
    receivedEvent: function(id) {
        var parentElement = document.getElementById(id);
        var listeningElement = parentElement.querySelector('.listening');
        var receivedElement = parentElement.querySelector('.received');

        listeningElement.setAttribute('style', 'display:none;');
        receivedElement.setAttribute('style', 'display:block;');

        console.log('Received Event: ' + id);
    }
};

app.initialize();
```

运行结果，正常无误！！

#### 自定义插件（注意）

**package.json** 文件内容正确填写，模式固定；

**plugin.xml** 文件选着规则和你在创建插件时的命令规则书写；有：
- `<param name="android-package" value="com.ztman.test.xiao.CordovaPluginToast"/>` 的**value** 对应的是自定义插件的那个类文件路径=**包名+类名(没有.java)**
- `<source-file src="src/android/CordovaPluginToast.java" target-dir="src/com/ztman/test/xiao" />` 中的**src** 指你通过字符命令创建插件，插件下的文件**CordovaPluginToast.java**所在的目录，而`./android/CordovaPluginToast.java` 这个目录和文件都是需要我们自己手动创建；**target-dir** 这个所指的目录是在`./platforms/android/app` 下的**CordovaPluginToast.java** 的文件路径。
- 
```
<js-module name="CordovaPluginToast" src="www/CordovaPluginToast.js"> 

        <clobbers target="plugin.target.CordovaPluginToast" /> 

</js-module> 中的**tartget** 下的内容，自动创建的是`plugin.target.CordovaPluginToast`  需要我们手动修改成插件名 `CordovaPluginToast` 才能在运行时候插件正常使用，即 `<clobbers target="CordovaPluginToast" />`
**[插件名].js**这里是 CordovaPluginToast.js，这个文件规范也很重要，我已刚从坑中爬出。`exec(null, null, "CordovaPluginToast", "test", [arg0]);` 特别指出这句代码，其中的第三个参数 **CordovaPluginToast** 必须要和插件的名字相同，否则，js调用原生肯定会无反应。另外，值得一说的是使用插件类直接调用的方法名字和第四个参数的名字一样。

**index.html** 中的代码`<meta http-equiv="Content-Security-Policy" content="default-src 'self' data: gap: https://ssl.gstatic.com 'unsafe-eval'; style-src 'self' 'unsafe-inline'; media-src *; img-src 'self' data: content:;">` 这一行要注释掉，在我这里是不注释掉不行，能运行，原生吐司无调用！
```













