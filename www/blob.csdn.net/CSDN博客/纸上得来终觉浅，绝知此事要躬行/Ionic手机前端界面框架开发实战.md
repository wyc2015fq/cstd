# Ionic手机前端界面框架开发实战 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年01月06日 11:44:13[boonya](https://me.csdn.net/boonya)阅读数：2468








官方网站：[http://ionicframework.com](http://ionicframework.com)

文章来源：[http://blog.csdn.net/i348018533/article/details/47258449/](http://blog.csdn.net/i348018533/article/details/47258449/)


ionicframework简称之为Ionic。Ionic是基于[AngularJS](http://lib.csdn.net/base/angularjs)框架开发的，所以在使用Ionic之前需要对angularjs有一定的了解。

可以看我另一篇文章：

[http://blog.csdn.net/i348018533/article/details/50779248](http://blog.csdn.net/i348018533/article/details/50779248)

步骤：

## 1.Ionic环境安装

Ionic开发是依赖于Nodejs环境的，所以在开发之前我们需要安装好Nodejs。下载安装：[http://nodejs.org/](http://nodejs.org/)

安装完成之后打开PowerShell输入命令node -v和npm -v验证是否安装成功，如果返回版本号则说明成功。 

在PowerShell命令行中执行：
`npm install -g cordova ionic `
这是使用npm包管理器安装cordova和ionic，-g的意思是全局安装，全局安装后在PowerShell中任何目录下都可以使用cordova和ionic的命令。

*PS. 安装过程需要打开VPN翻墙，貌似是资源被墙了，无语。*

## 2.创建Ionic项目

Ionic官网提供了三个项目模板blank、tabs和sideMenu ，参阅：[http://ionicframework.com/getting-started/](http://ionicframework.com/getting-started/)

我们使用blank空模板创建一个我们的应用，并且完成一个简单的tabs导航的小应用，打开PowerShell cd到开发目录，执行：
`ionic start myIonic blank`
*其中myIonic为我们的项目名称*

执行过程它会从github下载项目源码，请等待。。。 

执行完毕后你到开发目录下查看会发现多了一个名为myIonic的文件夹，这个文件夹就是Ionic项目的所在目录了。 

目录下有以下文件：

```
hooks           //google之后这个目录应该是在编译cordova时自定义的脚本命令，方便整合到我们的编译系统和版本控制系统中
plugins         //cordova插件的目录，插件的安装下一节详述
scss            //scss文件，发布时编译这个目录下的文件输出到www的css目录中
www             //我们的开发目录，页面、样式、脚本和图片都放在这个目录下
--css
--img
--js
--lib
--index.html
bower.json      //bower配置文件
config.xml      //Ionic的配置文件 ⱃ可以配置app的id，名称、描述起始页和一起其他配置
gulpfile.js     //gulp构建工具的执行文件，在这个文件中创建任务实现编译scss，css、js优化等
ionic.project   //Ionic的项目文件可以配置Ionic命令中livereload的监控文件
package.json    //npm配置文件
```

在开发初期的调试我们一般会使用浏览器作为我们的调试工具，Ionic提供了一个serve命令为我们的应用创建web站点可以非常方便的调试应用， cd到开发目录下，执行命令：
`ionic serve`
执行完毕后Ionic会自动帮我们打开我们的默认浏览器并跳转到我们的应用页面，浏览器打开页面时Ionic已经给我们开启了livereload模式，开启之后我们编辑`www`下的文件后保存时Ionic会通过websocket通知浏览器刷新页面，我们就不用每次修改完毕手动刷新页面了，又极大的提高了我们的工作效率。

*PS. 开启livereload后Ionic会默认监听`www`下面的文件，如果需要自定义的话请编辑`ionic.project`的`watchPatterns`来设置需要监听的范围*

```
{
  "name": "myIonic",
  "app_id": "",
  "watchPatterns": [
        "www/js/*",
        "www/js/*/*",
        "www/*.html",
        "www/templates/*.html",
        "www/css/*.css"
   ]
}
```

`www/index.html`作为我们的程序入口文件，首先编辑这个文件，在`body`标签中加入

```
<ion-nav-bar class="bar-positive">
  <ion-nav-back-button>
  </ion-nav-back-button>
</ion-nav-bar>
<ion-nav-view></ion-nav-view>
```

这段标签对于对Ionic不熟悉的人来说可能有点陌生，Ionic针对移动端自定义了一套样式库，并且使用Angular的指令封装了各个模块，`ion-nav-bar`是导航页面头部、`ion-nav-view`是导航内容页面。`inde.html`入口页面我们已经写完了，接下来我们新建两个我们自己的页面。我们在`www`目录下新建一个`templates`文件夹，存放我们的页面并新建页面：`tabs.html`
 作为我们tab应用的主页面：

**tabs.html**

```
<ion-tabs class="tabs-icon-top tabs-positive">

  <ion-tab title="Home" icon="ion-home" href="#/tab/home">
    <ion-nav-view name="home-tab"></ion-nav-view>
  </ion-tab>
  <ion-tab title="About" icon="ion-ios-information" href="#/tab/about">
    <ion-nav-view name="about-tab"></ion-nav-view>
  </ion-tab>

</ion-tabs>
```

`ion-tab`为tab应用的底部菜单项 `title`是菜单按钮显示的文字，`icon`是菜单按钮图标，`href`是点击菜单的跳转地址，子元素`ion-nav-view`是点击菜单后子页面显示的位置，`ion-nav-view`属性`name`为该tab的名称，后面添加子页面会详细描述。

然后在`app.js`中加入该页面的路由

```
$stateProvider
      .state('tabs', {
          url: "/tab",
          templateUrl: "templates/tabs.html",
      })
$urlRouterProvider.otherwise("/tab");
```

保存之后页面会自动刷新显示的应该是介个样子： 
![介个样子](https://img-blog.csdn.net/20150804133523768)

哈，我们tab应用的大概样子已经出来了，但是现在点击下面的菜单并没有什么反应，接下来我们就要开始添加我们的子页面了，在`templates`文件夹中新建：`home.html`、`facts.html`、`about.html`
**home.html 首页**

```xml
<ion-view view-title="Home">
    <ion-content class="padding">
      <p>
        <a class="button icon icon-right ion-chevron-right" href="#/tab/facts">Scientific Facts</a>
      </p>
    </ion-content>
</ion-view>
```

**facts.html**

```xml
<ion-view view-title="Facts">
    <ion-content class="padding">
      <p>
        <a class="button icon icon-right ion-chevron-right" href="#/tab/home">Home</a>
      </p>
    </ion-content>
</ion-view>
```

**about.html**

```xml
<ion-view view-title="About">
    <ion-content class="padding">
      <p>
        About Us
      </p>
    </ion-content>
</ion-view>
```

`ion-view`是我们子页面的内容，属性`view-title`可以设置页面头部的标题名称，添加好文件后我们需要在`app.js`中加入各个页面的路由，并且需要对之前的tabs路由做一个调整。

```
$stateProvider
      .state('tabs', {
          url: "/tab",
          abstract: true,
          templateUrl: "templates/tabs.html",
      })
      .state('tabs.home', {
          url: "/home",
          views: {
            'home-tab': {
              templateUrl: "templates/home.html"
            }
          }
      })
      .state('tabs.facts', {
          url: "/facts",
          views: {
            'home-tab': {
              templateUrl: "templates/facts.html"
            }
          }
      })
      .state('tabs.about', {
          url: "/about",
          views: {
            'about-tab': {
              templateUrl: "templates/about.html"
            }
          }
      })


   $urlRouterProvider.otherwise("/tab/home");
```

大家发现`tabs`路由增加了一个`abstract`字段，该字段在angular中的解释是抽象页面，不能独立作为页面展示只能作为其他页面父页面，当加载子页面之前加载父页面。

其他子页面的路由和`tabs`路由的配置有所不同名称命名是`父路由.子路由`的形式，并且增加了一个`views`字典，字段的key值对应着我们前面所说的`tabs.html`中`ion-nav-view`的`name`属性，表示该路由跳转的页面属于某个tab的子页面，同时修改我们的`otherwise`默认页为`/tab/home`。

保存之后浏览器自动刷新，我们来看一下效果。 
![介个样子](https://img-blog.csdn.net/20150804140831873)

好了，我们来点点看吧，页面切换Ionic已经帮我们实现了类似原生app的切换动画效果，是不是有一点原生app的感觉了？下面章节我们会实现在移动端的部署。

## 3.部署到手机和cordova插件的使用

一个简单的Web应用我们已经开发完成了，接下来我们就要做一些部署到移动端之前的准备了。 

Ionic支持两个平台[iOS](http://lib.csdn.net/base/ios)、[Android](http://lib.csdn.net/base/android)，默认的Ionic项目并没有添加这两个平台，我们需要手动添加，cd到开发目录下执行命令：

```
ionic platform add android 
ionic platform add ios
```

一般项目都需要两个平台同时部署，所以我们就添加两个，执行完毕之后执行命令查看你已经添加的平台列表：

```php
ionic platform list
```

结果：

```
Installed platforms: android 4.0.0, ios 3.8.0
Available platforms: amazon-fireos, blackberry10, browser, firefoxos, windows, windows8, wp8
```

接下来的部署我们分开讲述：

**Android**

Android部署分真机和虚拟机，其实方法类似，在部署之前需要安装JDK、Android SDK到开发环境中，配置好JDK的`JAVA_HOME`和Android SDK的`ANDROID_HOME`到环境变量中去，并把Android SDK安装目录添加到`Path`中。

配置好之后打开PowerShell输入命令返回正确：
`adb version`
接下来就简单了，把手机连接到电脑，并开启usb调试模式，执行命令：
`ionic run android `
Ionic开始编译项目生成apk并远程安装到手机上后自动打开应用，是不是很简单？

**IOS**

ios部署需要一台mac电脑，在mac电脑上面根据本篇ionic的安装过程安装环境之后，添加ios平台到项目，因为apple开发的证书限制我们在没有申请购买apple账号的情况下，使用虚拟机来部署我们的应用，在部署之前需要使用npm安装一个ios-sim插件用来调用模拟器的，执行命令：
`sudo npm install -g ios-sim`
安装好之后，cd 到开发目录下执行：

```bash
sudo ionic build ios
```

开始编译项目，编译完成之后用Xcode打开开发目录下`platform`->`ios`->`myIonic.xcodeproj`的项目文件，Xcode中选择要运行的虚拟机版本并执行快捷键cmd+R运行虚拟机，虚拟机打开后会自动运行你应用。

*PS. 为什么不用run ios 命令直接启动虚拟机呢，因为我的环境下执行run会报错，虚拟机会打开但是无法运行程序，找了半天解决解决方案没有结果，索性直接用Xcode了，效果是一样样的*

到了这一步我们的app已经部署完毕了，接下来我们要为我们的app添加一个拍照功能，需要引用到cordova插件，而Ionic把一些常用到的插件用angularjs服务做了一层封装：[http://ngcordova.com/](http://ngcordova.com/) 我们只需要在项目中安装ngCordova插件就可以想使用angularjs的服务一样在我们的项目中调用了，我们先安装ngCordova（安装需要bower包管理器，并打开VPN。。。）：
`bower install ngCordova`
安装完毕之后再www/index.html中引用

```xml
<script type="text/javascript" src="lib/ngCordova/dist/ng-cordova.js"></script>
<script type="text/javascript" src="cordova.js"></script>
```

*PS. ng-cordova.js需要在cordova.js引用之前*

并在app.js中添加依赖注入关系，另外我发现一个问题如果安装的ngCordova的0.1.18-alpha版本，在引用时会报错，我撤回0.1.17-alpha版本就没有问题了，不知道是什么原因。
`angular.module('starter', ['ionic', 'ngCordova'])`
接下来cd到开发目录，我们使用ionic命令来安装插件：
`cordova plugin add cordova-plugin-camera`
安装完毕之后会在项目目录下的`plugins`文件夹中多出一个`cordova-plugin-camera`，这就是我们插件目录。

下面我要在home页面中增加一点按钮调用摄像头，我们先在`www/js`中新增一个`controller.js`文件编写以下代码，并在`index.html`中引用：

```java
(function () {
    'use strict'
    angular
    .module('starter')
    .controller("homeCtrl", homeCtrl, ['$cordovaCamera'])

    function homeCtrl ($scope, $cordovaCamera) {
        $scope.openCamera = function () {
            var options = {
              quality: 50,
              destinationType: Camera.DestinationType.DATA_URL,
              sourceType: Camera.PictureSourceType.CAMERA,
              allowEdit: true,
              encodingType: Camera.EncodingType.JPEG,
              targetWidth: 100,
              targetHeight: 100,
              popoverOptions: CameraPopoverOptions,
              saveToPhotoAlbum: false
            };

            $cordovaCamera.getPicture(options).then(function(imageData) {
              var image = document.getElementById('myImage');
              image.src = "data:image/jpeg;base64," + imageData;
            }, function(err) {
              // error
            });
        }
    }

})();
```

我们新建了一个`homeCtrl`并添加`$cordovaCamera`的引用，在controller中定义一个`openCamera`方法来调用摄像头，在`home.html`中加入以下代码：

```xml
<a class="button icon icon-right ion-chevron-right" href="#/tab/facts">Scientific Facts</a>
<a class="button icon icon-right ion-chevron-right" ng-click="openCamera()">Camera</a>
```

![这里写图片描述](https://img-blog.csdn.net/20150804174857233)

增加了一个按钮去调用我们定义的方法`openCamera`，最后一步在`app.js`中绑定home路由和controller，在`app.js`中修改代码：

```
.state('tabs.home', {
   url: "/home",
    views: {
      'home-tab': {
        templateUrl: "templates/home.html",
        controller:"homeCtrl"
      }
    }
})
```

到此我们的app增加的打开摄像头功能已经开发完了，我们来[测试](http://lib.csdn.net/base/softwaretest)下，运行ionic的run方法来看看效果吧。
![这里写图片描述](https://img-blog.csdn.net/20150804174911375)

## 4.调试

我们在前面已经说过，在开发前期我们一般会在web中调试，而Ionic为我们提供了非常方面的web调试环境，只需要执行`ionic serve`就可以实现lievereload，而在部署到移动端后也可以开启livereload的，我们只需要修改下我们run命令参数：
`ionic run android --livereload -c -s`
这是android的调试模式，无论在虚拟机上还是在手机上都可以实现livereload，ios则只需要把android修改为ios即可，不用每次开发完成编译再部署！`-c`是开启客户端日志输出，`-s`是开启服务器端日志输出。–livereload参数的的意义在于修改生成包中的起始页面地址，修改为远程服务器地址这样就可以做到修改即通知客户端刷新页面了。

## 5.ios打包、android打包

能使用Ionic来开发app的很可能对Ios和android的app打包发布流程不是很熟悉，我这里简单说明下： 
**Android**

Android的打包比较简单执行命令：
`ionic build android -release `
找到`www/platform`目录下生成的apk包，此时的apk还不能被安装到手机上，还需要一步签名操作，大家可以参考： 
[http://www.cnblogs.com/qianxudetianxia/archive/2011/04/09/2010468.html](http://www.cnblogs.com/qianxudetianxia/archive/2011/04/09/2010468.html)

**IOS**

ipa文件的打包需要现在apple develop网站上注册一个账号，在网站申请生产环境的证书导入到你的开发机中和开发机绑定才可以使用xcode导出，详情参阅： 
[http://blog.csdn.net/songrotek/article/details/8448331](http://blog.csdn.net/songrotek/article/details/8448331)
[http://www.mexiqq.com/2014/07/25/xcode6+ios8%E6%9C%80%E6%96%B0%E7%9C%9F%E6%9C%BA%E8%B0%83%E8%AF%95%E6%95%99%E7%A8%8B/](http://www.mexiqq.com/2014/07/25/xcode6+ios8%E6%9C%80%E6%96%B0%E7%9C%9F%E6%9C%BA%E8%B0%83%E8%AF%95%E6%95%99%E7%A8%8B/)

## android使用crosswalk

android内置浏览器大家都知道性能是硬伤，尤其是低端机，所有ionic支持使用crosswalk浏览器代替系统内置浏览器内核，使用crosswalk后性能提升效果比较明显，但是apk的包体积会增大大约20M，是不用取决于项目使用场景。

使用crosswalk非常简单：
`ionic browser add crosswalk`
执行完成后在插件列表里会增加一个`cordova-plugin-crosswalk-webview`插件。 

安装完成后在打包时会自动下载对应的crosswalk包，如果下载失败尝试使用vpn，下载完成集成过程中我遇到了一些报错，报错的原因大致就是我的android sdk没有安装完全，这个大家在使用时需要注意下。

## 结束语

Ionic作为一个hybrid应用的开发框架总体来说还是很不错的，因为它的开发效率很高，并且可以跨平台的发布这两点可以让很多人心动了。另外Ionic在实际应用中的表现情况是这样的，在IOS平台中运行效率和原生的效果非常好，但是在Android中的就不是太理想了尤其是Android的低端机，不过Ionic的团队对于这方面的性能优化还在不断的努力，希望Ionic未来更牛逼。

另外从这次的项目中体会到了尤其是一些新兴技术在国内的资料太少，遇到问题不得不翻墙去找国外资料，但是碍于英文水平往往过程是非常痛苦的，所以学好英语是非常重要的！！！

最后介绍下这个应用是一款中介行业的房源管理软件，集成了gps、推送（最好用极光推送他们有对phonegap插件的支持）、摄像头拍照、时间控件、文件上传等原生API




