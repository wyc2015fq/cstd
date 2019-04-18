# 创建FireFox扩展 - Orisun - 博客园







# [创建FireFox扩展](https://www.cnblogs.com/zhangchaoyang/articles/1794354.html)





把Mozilla MDN上的Building an Extention翻译了一下，英文网址[https://developer.mozilla.org/en/Building_an_Extension](https://developer.mozilla.org/en/Building_an_Extension)
**一、引言**

我们将开发一个最基本的FF Extention，它仅仅是在浏览器的状态栏上添加一个面板，面板上显示“Hello,World!”
**二、配置开发环境**

扩展要打包发布为zip文件，并以XPI（读作“zippy”）作为文件扩展名。举一个典型XPI文件的结构例子：


```
exampleExt.xpi:
              /install.rdf                   
              /components/*  
              /components/cmdline.js                   
              /defaults/
              /defaults/preferences/*.js     
              /plugins/*                        
              /chrome.manifest                
              /chrome/icons/default/*       
              /chrome/
              /chrome/content/
```


我们将要建立的文件结构跟上面的很类似。Let's begin。在你的磁盘上新建一个文件夹比如取名“My_FF_Extention"（此为扩展的根目录）。在其下建一个子文件夹chrome，在chrome下再建一个文件夹content。在扩展根目录下新建两个文件chrome.manifest和install.rdf。在chrome/content下新建文件sample.xul。

现在你的文件结构应该是这样的：


```
<ext path>\
          install.rdf
          chrome.manifest
          chrome\
             content\
                sample.xul
```




**三、创建安装清单**

打开install.rdf加入如下内容：




```
<?xml version="1.0"?>

<RDF xmlns="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
     xmlns:em="http://www.mozilla.org/2004/em-rdf#">

  <Description about="urn:mozilla:install-manifest">
    <em:id>sample@example.net</em:id>
    <em:version>1.0</em:version>
    <em:type>2</em:type>
   
    <!-- Target Application this extension can install into, 
         with minimum and maximum supported versions. --> 
    <em:targetApplication>
      <Description>
        <em:id>{ec8030f7-c20a-464f-9b0e-13a3a9e97384}</em:id>
        <em:minVersion>1.5</em:minVersion>
        <em:maxVersion>3.6.*</em:maxVersion>
      </Description>
    </em:targetApplication>
   
    <!-- Front End MetaData -->
    <em:name>sample</em:name>
    <em:description>A test extension</em:description>
    <em:creator>Your Name Here</em:creator>
    <em:homepageURL>http://www.example.com/</em:homepageURL>
  </Description>      
</RDF>
```




**sample@example.net**--扩展的ID。以邮箱的格式来标唯一识你的扩展，它可以不是你的真实邮箱，甚至没有必要是一个正确的邮箱格式，它可以形如example.example.example。

**<em:type>2</em:type>**--“2”说明你安装的是一个扩展，如果安装一个主题，这个值就应当是4。

**{ec8030f7-c20a-464f-9b0e-13a3a9e97384}**--火狐的应用ID。

**1.5**--扩展所支持的最代版本。最低版本上不要用*，这会导致你不可预期的事情发生。

**3.6.***--扩展可以工作的最高浏览器版本。

（如果你得到这样的消息：install.rdf是畸形的（malformed），那么你最好的火狐中使用“文件->打开”的方式，此时会报告xml错误。在我的安例中，我是在"<?xml"前面加了一个空格。）

**四、使用XUL扩展浏览器**

火狐的用户界面是用JavaScript或XUL写成的。XUL是一种XML方法，它提供很用户界面上的控件如按钮、菜单、工具条、树等等。用户的开为功能要通过JavaScript来实现。

为了扩展浏览器，我们需要通过增加或修改控件来修改浏览器的界面。通过向浏览器窗体中插入XUL DOM元素可以增加控件，而通过使用脚本或附加事件处理器可以修改它们。

浏览器在一个叫做browser.xul($FIREFOX_INSTALL_DIR/chrome/browser.jar包含了content/browser/browser.xul)的文件中被执行。在browser.xul中我们可以看到状态条形如：


```
<statusbar id="status-bar">
 ... <statusbarpanel>s ...
</statusbar>
```


**<statusbar id="status-bar">** 是为 XUL Overlay准备的接入点。

**什么是XUL Overlays？**

XUL Overlays是一种在运行时把其他用户UI控件插入到XUL文档中的方式。一个XUL Overlay就是一个.xul文件，它明确了XUL小部件插入到主文档中的接入点。这些小部件可以指明是插入、删除还是修改。举一个XUL Overlay文件的例子：


```
<?xml version="1.0"?>
<overlay id="sample" 
         xmlns="http://www.mozilla.org/keymaster/gatekeeper/there.is.only.xul">
 <statusbar id="status-bar">
  <statusbarpanel id="my-panel" label="Hello, World"  />
 </statusbar>
</overlay>
```


**<statusbar>**就是接入点，即我们要把我们所做的扩展放到浏览器的这个位置上去。

**<statusbarpanel>**这外子控件就是我们想要插入的元素。把刚才这个文件写入并保存到你的chrome/content/sample.xul中。

**五、Chrome URIs**

XUL文件是"Chrome Packages" 的一部分，"Chrome Packages" 是用户界面组件的集合，它通过chrome://URIs被加载。FF加载文件不是使用形如file:// URI（因为FF的系统文件位置在不同的操作系统上是不一样的），Mozilla的开发人员想了另外一个方法来让已安装的应用知道我们创建的URIs在哪里。

浏览器窗体是：

```
chrome://browser/content/browser.xul
```



把这个URL敲进你的FF地址栏中看看会发生什么！

Chrome URIs包含以下部分：

首先是URI模式（chrome），它用来告诉FF的网络library这是一个Chrome URI。它指明此URI的内容将被当作一个chrome来处理。把（chrome）比作（http），告诉火狐把这个URI当作一个web页面来对待。

第二是一个包名（browser），它指明了用户界面组件的集合。这个在你的应用中应该是唯一的，以防止不同的扩展发生冲突。

第三需要的数据类型。有三种类型：content（XUL, JavaScript, XBL bindings等等，来构成一个application UI的结构和行为），local（DTD, .properties文件等，包含了UI‘s本地化的字符串）， 和skin（构成UI主题的CSS和图片）。

最后是要加载的文件路径。

所以chrome://foo/skin/bar.png的意思是说从foo主题的skin域中加载文件 bar.png 。当你使用Chrome URI加载内容时，火狐使用Chrome Registry来把这些URIs映射成你的本地磁盘文件（也可能是在一个Jar包里面）。

**六、创建Chrome Manifest**

打开你在扩展根目录下建立的chrome.manifest文件，写入以下代码：


```
content     sample    chrome/content/
```




（不要忘记末尾的斜杠“/“！没有它包是不会被注册的。）

这条代码包含了以下内容：

1.chrome包里面的material的类型

2.chrome包的名称（包名全部用小写，因为Firefox/Thunderbird在2.0以及更早的版本中不支持mixed/camel case（大家应该知道camel-case命名法吧））3.chrome包文件的位置

所以上面这名代码的意思是说：有个chrome包叫sample，我们可以在chrome/content/中找到它的content，chrome/content/是对于chrome.manifest的一个相对路径。

local和skin文件必须放在content文件夹的下一层。保存该文件。当你打开带有此扩展的FF时，它会注册你的chrome包。

**七、注册Overlay**

你要让你的火狐浏览器展现任一何人个Overlay都要把它并入你的FF中。所以在chrome.manifest添加一面一行代码：


```
overlay chrome://browser/content/browser.xul chrome://sample/content/sample.xul
```


这名话告诉火狐，在加载browser.xul时把sample.xul并入到 browser.xul 中去。

**八、测试**

首先要告诉火狐你开发的扩展在哪里。把火狐指向你开发的扩展，这样每次启动火狐时都会去加载你的扩展。

1.把你的profile文件放到你想让它工作的profile文件夹下面（比如Firefox/Profiles/<profile_id>.default/）。

2.打开extentions/文件夹，必要时创建它。

3.创建一个新的文本文件，把完整路径放到你的扩展根目录下面（Windows用户要注意绝对路径斜线用”\"，相对路径斜线用"/"）。别忘了最的封闭斜线且末尾不要有空格。

4.用你的扩展id（比如sample@example.net）作为文件名保存它。没有文件扩展名。

现在你可以测试你的扩展了！启动火狐在状态栏就可以看到你的"Hello,World!"了。

**九、封包**

把扩展打包是为了部署和安装。

Windows XP用户可以使用压缩工具把你的扩展文件夹下面的内容（注意不是扩展文件夹本身，即选中根目录下的所有内容将其打包）压缩为zip（注意不是rar）包，以.xpi作为压缩包的扩展名。

**十、更多关于XUL Overlays**

为了把UI控件附加到接入点上，你可以在Overlays中这样操作XUL fragments

1.在接入点上修改属性。例如<statusbar id="status-bar" hidden="true" />（隐藏状态栏）

2.从主文档中移除接入点。例如<statusbar id="status-bar" removeelement="true" />

3.控制被插入控件的位置：


```
<statusbarpanel position="1" ...  />

<statusbarpanel insertbefore="other-id" ...  />

<statusbarpanel insertafter="other-id" ...  />
```


**十一、创建新的UI组件**

你还可以在单独的.xul文件中创建你自己的窗体和对话框，在js文件中执行用户操作，使用DOM方法操纵UI控件，使用CSS文件添加图片、设置颜色等。

**十二、Defaults Files**

Defaults Files应该放在扩展根目录/defaults/下面。默认的preferences .js文件放在defaults/preferences/下。这样火狐启动的时候它的preferences system就会自动加载它，然后你就可以通过Preferences API来使用preferences .js文件了。

举一个default preference的例子：


```
pref("extensions.sample.username", "Joe"); //a string pref
pref("extensions.sample.sort", 2); //an int pref
pref("extensions.sample.showAdvanced", true); //a boolean pref
```


**十三、调试**

**你的扩展调试分析工具**

1.DOM Inspector--监视实际生效的属性、DOM结构、CSS样式。

2.Venkman--在JavaScript中设置断点，监视调用堆栈。

3.arguments.callee.caller in JavaScript--获取一个函数的调用堆栈

**输出调试**

1.使用dump("string")

2.使用 Components.utils.reportError() 或 nsIConsoleService把事件注册到JavaScript控制台中












