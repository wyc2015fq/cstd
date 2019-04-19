# myeclipse安装、优化与插件安装 - 零度的博客专栏 - CSDN博客
2015年04月15日 15:20:23[零度anngle](https://me.csdn.net/zmx729618)阅读数：1337
**Myeclipse10下载与破解**
Genuitec 公司发布了MyEclipse 10，一款Genuitec旗下的商业化Eclipse集成开发工具的升级版本。MyEclipse
 10基于Eclipse Indigo构建，为Java和JavaEE项目提供了Maven3的支持。本次发布的版本中还加入了对JaveEE
 6、HTML5、JPA2和JSF 2的支持。版本号10是为了庆祝即将到来的Eclipse的10周年诞辰（MyEclipse的首次发布是在8年前）。
我现在用的还是6.5的版本，6.5的版本是我觉得最好用的一个版本。我装上了，还没感受到有哪些好用，就是感觉体积庞大，和IBM
的WID一样，是个多面手，啥事都能干，其实经常能使用的也就其中那么几个功能，要是能像插件一样，即插即用就好了。
MyEclipse 10使用最高级的桌面和Web开发技术，包括 HTML5
和 Java EE 6，支持 JPA 2.0、JSF 2.0
、Eclipselink 2.1
以及 OpenJPA 2.0.
而对 IBM WebSphere
用户来说，MyEclipse Blue
支持最新版本的 WebSphere Portal Server 7.0、WebSphere 8
以及以前的版本，无缝的支持 IBM DB2
数据库。 MyEclipse
是一个十分优秀的用于开发Java, J2EE的 Eclipse
插件集合，MyEclipse的功能非常强大，支持也十分广泛，尤其是对各种开元产品的支持十分不错。MyEclipse目前支持Java
 Servlet,AJAX, JSP, JSF, Struts,Spring, Hibernate,EJB3,JDBC数据库链接工具等多项功能。可以说MyEclipse几乎囊括了目前所有主流开元产品的专属eclipse开发工具。
以下是下载与破解链接(右击迅雷下载):
[Myeclipse10
正式下载](http://downloads.myeclipseide.com/downloads/products/eworkbench/indigo/installers/myeclipse-blue-10.0-offline-installer-windows.exe)
[Myeclipse10 Bule版下载](http://downloads.myeclipseide.com/downloads/products/eworkbench/indigo/installers/myeclipse-10.0-offline-installer-windows.exe)
[Myeclipse9/10破解包](http://download.csdn.net/detail/shimiso/3906897)(内附详细破解步骤)
另附Myeclipse8/9/10 Verycd下载出处:[Myeclipse_Verycd下载](http://www.verycd.com/topics/2833718/)
**二.Myeclipse10修改字体**
MyEclipse10 是基于Eclipse3.7内核，但在Eclipse的Preferences-〉general-〉Appearance->Colors
 and Fonts 中并没有找到Courier New字体,它采用的是Consolas字体,中文看着非常小非常别扭,在Windows7下，系统自带虽然有Courier
 New字体，但是并没有激活显示，需要手动激活，才能在软件中使用
激活方法如下：
在win7的控制面板->字体，找到Courier New，右键，显示。然后在eclipse的fonts列表中就可以选择了。上两张图
**三.Myeclipse10插件安装**
说到插件,myeclipse自8.X开始,插件安装就变得巨难用,通常最好还是用离线安装,在线安装很可能出问题,下面仅以SVN安装为例,其他诸如ADT15,Aptana等都可采用
1.首先下载SVN包：[svn-1.6](http://download.csdn.net/detail/shimiso/3907090)
2.解压SVN包，然后找到其中的两个文件夹：features
和 plugins
3.随意建一个文件夹（位置和名称自己定就好了，我的是E:\myEclipsePlugin\svn），然后把第二步的解压好的features
和 plugins放到这个文件夹下
4.找到myeclipse的安装目录，下面有一个configuration\org.eclipse.equinox.simpleconfigurator\bundles.info
文件。现在需要做的就是在该文件内添加的东西
5.添加的内容用下面的类生成：
[view plain](http://blog.csdn.net/shimiso/article/details/7061000)
- <span style="font-size:13px;">import java.io.File;     
- import java.util.ArrayList;      
- import java.util.List;      
- /**    
-  * MyEclipse9 插件配置代码生成器    
-  *    
-  *    
-  */     
- public class PluginConfigCreator     
- {      
-     public PluginConfigCreator()     
-     {      
-     }      
-     public void print(String path)     
-     {      
-         List<String> list = getFileList(path);     
-         if (list == null)      
-         {      
-             return;      
-         }      
-         int length = list.size();     
-         for (int i = 0; i < length; i++)     
-         {      
-             String result = "";      
-             String thePath = getFormatPath(getString(list.get(i)));     
-             File file = new File(thePath);     
-             if (file.isDirectory())     
-             {      
-                 String fileName = file.getName();     
-                 if (fileName.indexOf("_") < 0)     
-                 {      
-                     print(thePath);     
-                     continue;      
-                 }      
-                 String[] filenames = fileName.split("_");     
-                 String filename1 = filenames[0];     
-                 String filename2 = filenames[1];     
-                 result = filename1 + "," + filename2 + ",file:/" + path + "/"    
-                         + fileName + "\\,4,false";     
-                 System.out.println(result);     
-             } else if (file.isFile())     
-             {      
-                 String fileName = file.getName();     
-                 if (fileName.indexOf("_") < 0)     
-                 {      
-                     continue;      
-                 }      
-                 int last = fileName.lastIndexOf("_");// 最后一个下划线的位置     
-                 String filename1 = fileName.substring(0, last);     
-                 String filename2 = fileName.substring(last + 1, fileName     
-                         .length() - 4);     
-                 result = filename1 + "," + filename2 + ",file:/" + path + "/"    
-                         + fileName + ",4,false";     
-                 System.out.println(result);     
-             }      
-         }      
-     }      
-     public List<String> getFileList(String path)     
-     {      
-         path = getFormatPath(path);     
-         path = path + "/";      
-         File filePath = new File(path);     
-         if (!filePath.isDirectory())     
-         {      
-             return null;      
-         }      
-         String[] filelist = filePath.list();     
-         List<String> filelistFilter = new ArrayList<String>();     
-         for (int i = 0; i < filelist.length; i++)     
-         {      
-             String tempfilename = getFormatPath(path + filelist[i]);     
-             filelistFilter.add(tempfilename);     
-         }      
-         return filelistFilter;      
-     }      
-     public String getString(Object object)     
-     {      
-         if (object == null)      
-         {      
-             return "";      
-         }      
-         return String.valueOf(object);     
-     }      
-     public String getFormatPath(String path)     
-     {      
-         path = path.replaceAll("\\\\", "/");     
-         path = path.replaceAll("//", "/");     
-         return path;      
-     }      
-     public static void main(String[] args)     
-     {      
-         /*你的SVN的features 和 plugins复制后放的目录*/    
-             String plugin = "E:/myEclipsePlugin/svn/";     
-         new PluginConfigCreator().print(plugin);     
-     }      
- }  </span>  
6.把以上生成的字符串（一大堆）添加到第四步bundles.info文件的后面，然后重启myeclipse即可。
此外Adobe Flash Builder 4.6也完美支持Myeclipse10,只是插件在安装的时候不是采用以上方式,根据官方英文文档说明,
首先要将Adobe Flash Builder 4.6安装完成,
然后进安装目录~\Adobe\Adobe Flash Builder 4.6\utilities运行Adobe Flash Builder 4.6 Plug-in Utility.exe,
最后根据安装向导设置插件安装到myeclipse10的目录~\Genuitec\MyEclipse 10,必须确保该目录下包含dropins文件
如此即可完成最新的Adobe Flash Builder 4.6插件的安装,最后启动myeclipse10,界面也随之变成中文,但有时候也会有出问题,此时进入到~\Genuitec\MyEclipse
 10\Uninstall Adobe Flash Builder 4.6 Plug-in目录,运行Uninstall Adobe Flash Builder 4.6 Plug-in.exe卸载插件,重新安装,第二次必定成功,myeclipse10对插件的兼容性没有eclipse的好,千万不可尝试按
 eclipse的配置去暴力修改里面的参数,否则极有可能再也起不来了!
**四.Myeclipse10优化**
最后谈谈如何优化Myeclipse10
1、window-preferences-MyEclipse Enterprise Workbench-Maven4MyEclipse-Maven，将Maven
 JDK改为电脑上安装的JDK，即不使用myeclipse提高的JDK
登记add按钮，选择你的电脑上的JDK即可（注意：不是JRE，我的值为：Java6.014）
2、window-preferences-MyEclipse Enterprise Workbench-Matisse4Myeclipse/Swing,将Design-time
 information(dt.jar) location 改用电脑安装的JDK的dt.jar
（即不使用myeclipse提供的dt.jar，我的值为：C:\Java6.014\lib\dt.jar）
经过以上的优化，myeclipse的启动时间可以减少2/3，Tomcat的启动速度可以减少1/2(视具体情况而定)
第一步: 取消自动validation
validation有一堆，什么xml、jsp、jsf、js等等，我们没有必要全部都去自动校验一下，只是需要的时候才会手工校验一下！
取消方法：
windows–>perferences–>myeclipse–>validation
除开Manual下面的复选框全部选中之外，其他全部不选
手工验证方法：
在要验证的文件上，单击鼠标右键–>myeclipse–>run validation
第二步：取消Eclipse拼写检查
1、拼写检查会给我们带来不少的麻烦，我们的方法命名都会是单词的缩写，他也会提示有错，所以最好去掉，没有多大的用处
windows–>perferences–>general–>validation->editors->Text Editors->spelling
第三步：取消myeclipse的启动项
myeclipse会有很多的启动项，而其中很多我们都用不着，或者只用一两个，取消前面不用的就可以
windows–>perferences–>general–>startup and shutdown  (详见底端介绍)
第四步：更改jsp默认打开的方式
安装了myeclipse后，编辑jsp页面，会打开他的编辑页面，同时也有预览页面，速度很慢，不适合开发。所以更改之windows–>perferences–>general–>editors->file associations
在下方选择一种编辑器，然后点击左边的default按钮
第五步：更改代码提示快捷键(不建议使用增强提示,使用Ctrl+/在自己需要的时候提示更佳)
现在的代码提示快捷键，默认为ctrl+space，而我们输入法切换也是，所以会有冲突。谁叫myeclipse是外国人做的呢。。根本不需要切换输入法.
windows–>perferences–>general–>Keys
更改 content assist 为 alt+/
同时由于alt+/已经被word completion占用，所以得同时修改word completion的快捷键值
好了,现在的速度及方便性是不是提高了。
第六步: 更改内存使用文件
1、打开 myeclipse.ini
[view plain](http://blog.csdn.net/shimiso/article/details/7061000)
- -vmargs   
- -Xms256m   
- -Xmx1024m      
- -XX:PermSize=128M   
- -XX:MaxPermSize=256M  
把下面的那个 -XX:MaxPermSize 调大，比如 -XX:MaxPermSize=512M，再把 -XX:PermSize 调成跟 -XX:MaxPermSize一样大
原因：大家一定对这个画面很熟悉吧：
几乎每次 eclipse 卡到当都是因为这个非堆内存不足造成的，把最大跟最小调成一样是因为不让 myeclipse 频繁的换内存区域大小
注意：XX:MaxPermSize 和 Xmx 的大小之和不能超过你的电脑内存大小
以下是有关内存的一些知识扩展:
1.堆(Heap)和非堆(Non-heap)内存
按照官方的说法：“Java 虚拟机具有一个堆，堆是运行时数据区域，所有类实例和数组的内存均从此处分配。堆是在 Java
虚拟机启动时创建的。”“在JVM中堆之外的内存称为非堆内存(Non-heap memory)”。可以看出JVM主要管理两种类型的内存：堆和非堆。简单来说堆就是Java代码可及的内存，是留给开发人员使用的；非堆就是JVM留给自己用的，所以方法区、JVM内部处理或优化所需的内存(如JIT编译后的代码缓存)、每个类结构(如运行时常数池、字段和方法数据)以及方法和构造方法的代码都在非堆内存中。
2.堆内存分配
JVM初始分配的内存由-Xms指定，默认是物理内存的1/64；JVM最大分配的内存由-Xmx指定，默认是物理内存的1/4。默认空余堆内存小于
 40%时，JVM就会增大堆直到-Xmx的最大限制；空余堆内存大于70%时，JVM会减少堆直到-Xms的最小限制。因此服务器一般设置-Xms、
 -Xmx相等以避免在每次GC
后调整堆的大小。
3.非堆内存分配
JVM使用-XX:PermSize设置非堆内存初始值，默认是物理内存的1/64；由XX:MaxPermSize设置最大非堆内存的大小，默认是物理内存的1/4。
4.JVM内存限制(最大值)
首先JVM内存限制于实际的最大物理内存，假设物理内存无限大的话，JVM内存的最大值跟操作系统有很大的关系。简单的说就32位处理器虽然可控内存空间有4GB,但是具体的操作系统会给一个限制，这个限制一般是2GB-3GB（一般来说Windows系统下为1.5G-2G，Linux系统下为2G-
 3G），而64bit以上的处理器就不会有限制了。
举例说明含义：
-Xms128m 表示JVM Heap(堆内存)最小尺寸128MB，初始分配
-Xmx512m 表示JVM Heap(堆内存)最大允许的尺寸256MB，按需分配。
说明：如果-Xmx不指定或者指定偏小，应用可能会导致java.lang.OutOfMemory错误，此错误来自JVM不是Throwable的，无法用try...catch捕捉。
PermSize和MaxPermSize指明虚拟机为java永久生成对象（Permanate generation）如，class对象、方法对象这些可反射（reflective）对象分配内存限制，这些内存不包括在Heap（堆内存）区之中。
-XX:PermSize=64MB 最小尺寸，初始分配
-XX:MaxPermSize=256MB 最大允许分配尺寸，按需分配
过小会导致：java.lang.OutOfMemoryError: PermGen space
MaxPermSize缺省值和-server -client选项相关。-server选项下默认MaxPermSize为64m
  -client选项下默认MaxPermSize为32m
PS:不同厂家的jdk垃圾回收算法不一样。在sun的jdk下，Xms和Xmx设置一样，可以减轻伸缩堆大小带来的压力，但在ibm的jdk下面，设置为一样会增大堆碎片产生的几率。
第七步: 修改Struts-config.xml文件打开错误
有时点击myeclipse里的struts的xml配置文件，会报错：
Error opening the editorUnable to open the editor ,unknow the editor id…..
把这个窗口关闭后才出正确的xml文件显示，这个我们这样改：
windows–>perferences–>general–>editors->file associations选择*.xml，选择myeclipse xml editor点default，ok
第八步: 取消自动验证,该成手动验证
windows-->perferences-->myeclipse-->validation
将Build下全部勾取消,保留Manual(手动) 如果你需要验证某个文件的时候，我们可以单独去验证它。方法是，在需要验证的文件上( 右键 -> MyEclipse -> Run Validation   
第九步: 取消Maven更新(启动更新)
Window > Preferences > Myeclipse Enterprise Workbench > Maven4Myeclipse > Maven>
禁用Download repository index updates on startup
**五、编码格式调整**
一、设置新建常见文件的默认编码格式，也就是文件保存的格式。
在不对MyEclipse进行设置的时候，默认保存文件的编码，一般跟简体中文操作系统（如windows2000，windowsXP）的编码一致，即GBK。
在简体中文系统下，ANSI
编码代表 GBK编码;在日文操作系统下，ANSI
编码代表 JIS 编码。
Window-->Preferences-->General -->content Types
在右侧窗口中展开每一个子项,依次输入编码格式，如“UTF-8”点击“update”就设置好了。
例如：在以上设置中，设置jsp默认编码格式为“UTF-8”，那么以后新建的jsp文件，都是以“UTF-8”的格式保存的。
同样设置java默认编码格式为“UTF-8”，那么以后新建的java文件，都是以“UTF-8”的格式保存的。
设置html默认编码格式为“UTF-8”，那么以后新建的html文件，都是以“UTF-8”的格式保存的。
以上设置了文件的保存编码格式，默认的打开方式（解码方式）会跟保存编码格式一样。
二、设置新建其他文件的默认编码格式，即文件保存格式。
在第一项设置中，只是设置了常用文件类型的编码格式。如果新建一个文件，没有后缀名，也就是不指名文件类型，那么MyEclipse无法判断该用何种编码
格式保存，就用默认文件保存编码格式GBK进行保存。这个默认保存编码格式，可用如下方法修改：
window-->preferences-->general-->workspace 在右侧 Text file encoding ->Other    选择UTF-8
三、单个文件查看编码格式（也就是打开文件用的编码格式）更改。这个方法没有更改文件的编码格式，只是告诉MyEclipse用何种编码格式进行解码。
在要查看的文件上，如文件Test.java上右键-->Properties-->Resource在右侧Text
 file encoding ->Other，可以进行修改。
比如说，你在“一”里面设置*.java文件的编码格式为“UTF-8”，那么你打开后缀名为java的文件，默认就会以“UTF-8”格式进行解码。如果有人给了你一个
编写好的java文件，但是他是用GBK编码保存的，那么你现在打开的时候，会显示乱码。在这个文件上右键-->Properties-->Resource在右侧Text
 file
encoding ->Other ->GBK，就可以正常显示里面的内容了。
四、指定MyEclipse编写文件时页面内容数据编码格式，如html,或者jsp网页中提交表单前，用户输入的信息编码格式，配置文件xml中的参数编码格式等。
主要是通知浏览器或服务器，传输的数据的编码格式。
window-->Preferences-->MyEclipse-->Files and Editors-->选择子项-->Encoding
例如：选择UTF-8,
新建jsp文件的时候，contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"就会自动生成，不用每次去手工更改了。
新建html文件的时候，http-equiv="Content-Type" content="text/html; charset=UTF-8"
新建xml文件的时候，<?xml version="1.0" encoding="UTF-8"?>
