# 开发摩托罗拉PDA MC3100 应用程序 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年06月10日 14:20:39[boonya](https://me.csdn.net/boonya)阅读数：3106









From:[http://blog.csdn.net/luoyanqing119/article/details/25708037](http://blog.csdn.net/luoyanqing119/article/details/25708037)



一.开发工具包。

   1.开发平台（PDA模拟器）：Windows Mobile 5.0 Pocket PC SDK

   2.扫描所用的Symbol.dll，Symbol.Barcode.dll文件及扫描的所有案例,需要安装：Symbol Mobility Developer Kit V1.7 for .NET。安装后所有的sample参照以下路径：

     C:\Program Files\Symbol Mobility Developer Kit for .NET\v1.7\Windows CE\Samples VS2005\C#


二.关于PDA MC50最后的部署问题

   MC50在.NET平台上面开发的话。必需的几个包：

(1).NETCFv2.ppc.armv4.cab--这是Framework里面精简框构

    参照：C:\Program Files\Microsoft Visual Studio 8\SmartDevices\SDK\CompactFramework\2.0\v2.0\WindowsCE\wce400\armv4


(2).sqlce30.ppc.wce4.armv4.CAB--这是sqlce3.0安装包。如果用于PDA里面的数据库的话。这个包是必装的.

    参照：C:\Program Files\Microsoft Visual Studio 8\SmartDevices\SDK\SQL Server\Mobile\v3.0\wce400\armv4

(3).System_SR_CHS.CAB--这个好像是与系统集成的一个包。具体是干什么用的。我也不是很清楚。不过这个包也是必须装的。

    参照：C:\Program Files\Microsoft Visual Studio 8\SmartDevices\SDK\CompactFramework\2.0\v2.0\WindowsCE\Diagnostics 

(4).调用PDA扫描功能，所需的包：symbol.all.arm.cab。该包包含了Symbol.dll，Symbol.Barcode.dll等激活扫描头的dll文件。

    参照：C:\Program Files\Symbol Mobility Developer Kit for .NET\v1.7\SDK\Smart Devices\wce400\armv4


把这几个包和开发的程序包一起放到PDA的缓存里去。Application文件夹里面。那么系统从置的话也可以安装。值得提一句的话。这三个CAB文件的安装包。安装完成后。自动的删除了。所以最好在SD存储卡内把他们都设置成只读和存档的。


-----------------------------------------------------------------------------------------------------------------------------




三、若用VS2008


1、  我用的开发环境是的vs2008。


2、  下载Symbol开发包，虽然盘点机是motorola牌子的，但开发包的名称还是叫Symbol。下载地址：[https://docs.symbol.com/downloads/SMDK-M-1.07.03.exe](https://docs.symbol.com/downloads/SMDK-M-1.07.03.exe)


3、  安装PC机与盘点机的通信软件Microsoft ActiveSync 4.5 中文版安装。


二、选择数据库


环境安装好了，很兴奋，以为这就可以开发程序了。刚开始确实挺顺利的，建个空窗体，编译正常，还可以部署上机子上，正常运行。接下来，按照开发Windows程序的思路开始动作了。


刚开始数据库选择了sqlite3，也挺好用的，主要是因没有机子，所以写的是winform程序，读取数据库，保存数据都正确。结果等拿到机子后，傻眼了。机子不支持sqlite数据库，要安装wince版本的运行库，试了几个没有成功，可能是与我用的vs2008有关。由于是新手，也不知道怎么修正，就换了microsoft推荐的sqlce数据库。Sqlce数据库在wince上能使用，同样也可以开发winform程序，导出数据后，然后拷到wince上使用。本来想用文本文件然后再到wince上，再保存到sqlce数据库中，结果发现速度太慢。


后来发现，由于商品信息格式相对固定，还不如保存到Arraylist中，然后通过二分法查找。实际测试的结果，速度还是挺快的，这是我最满意的结果。


三、激光扫描


程序好了，读取商品信息，保存盘点结果，一切正常。高兴呀O(∩_∩)O~。拿了条码一试，天那，不发红光，要晕倒了。


因为刚开始不知道要安装symbol开发包。从网上搜索motorola开发，没有找到想要的结果，后来想到motorola收购了symbol，会不会是symbol开发呢？结果正是想要的。下载安装，查看例子。发现写的测试条码扫描功能的例子，太复杂了。结果就直接用barcode控件，挺好，功能正常。继续设置屏幕亮度，坏了，包有冲突。继续查看例子，去掉不必要的代码，到最后才发现，只有几行代码是有效的，太经典了。定义编量


private Symbol.Barcode.Reader MyReader = null;


private Symbol.Barcode.ReaderData MyReaderData = null;


初始化

```
private void InitRead()

        {

            MyReader = new Symbol.Barcode.Reader();

            MyReaderData =

             new Symbol.Barcode.ReaderData(Symbol.Barcode.ReaderDataTypes.Text,

             Symbol.Barcode.ReaderDataLengths.DefaultText);

            MyReader.ReadNotify += new EventHandler(MyReader_ReadNotify);//定义接收事件

            MyReader.Actions.Enable();

            MyReader.Actions.Read(MyReaderData);

        }
```




显示扫描的条码


```
private void MyReader_ReadNotify(object sender, EventArgs e)

        {

            txt_barcode.Text = MyReaderData.Text;

            txt_barcode.Focus();

            txt_barcode.Select(13, 1);

            MyReaderData.Dispose();//这一句很重要，一定要释放内存

            //以下两句话，实现连续扫码功能

            MyReaderData = MyReader.GetNextReaderData();

            MyReader.Actions.Read(MyReaderData);

        }
```
到此程序完成，可以正常使用。


四、部署到盘点机


通过vs开发环境部署程序中可以看到需要安装哪些CAB包，把这些包拷到机子上，运行，提示‘不是一效的windows ce安装程序’，晕了。最后才发现是我拷的目录不正确。因为机子空间太小，所以想拷到扩展卡上安装，结果又绕了弯路，有一些包必须在系统目录下才能安装。


MC1000的机型应用程序的安装需要以下几个CAB文件： 

a) NETCFv2.wce5.armv4i.cab，这是.NET Compact Framework v2.0的一个安装包；或者NETCFv35.wce.armv4.cab ，这是.NET Compact Framework v3.5 的一个安装包；


b) symbol.all.arm.cab，这是SYMBOL的一个安装包，在Symbol提供的软件开发包中；


c) sqlce30.dev.CHS.wce5.armv4i.CAB，sqlce30.repl.wce5.armv4i.CAB，sqlce30.wce5.armv4i.CAB，这三个CAB安装包是SQL CE数据库必须的。如果不是有SQL CE 数据库，则这些文件不需要安装。 

System_SR_CHS.CAB，这是调试用到的一个安装包，不是必须的。


用.NET制作的应用程序部署到PDA上之前，必须安装.NET Compact Framework v2.0 或者.NET Compact Framework v3.5。由实际设备项目程序开发的版本来定，也就是上面的安装包，此外还要装SYMBOL的一个安装包（symbol.all.arm.cab），这些都是运行应用程序必须的环境。如果没有用到SQL CE，则其他几个安装包不是必须的。


由于CAB包在安装后，如果没有设置为只读，系统会自动删除，所以就想写一个自动调用的程序，安装后就会删除这些包了，那么下次就不会再安装了，如果机子冷启动了，就再次拷贝这些CAB包，为的是节省系统空间（劳神呀）。


在startup目录下增加installcab.run。内容类似下面内容Wceload /NOASKDEST \Application\CAB\NETCFv2.wce5.armv4i.cab，但当再次启动时会提示找不到cab文件，不理想。


继续查资料，发现可以在注册表中增加启动安装项。在application下增加netct.reg，内容如下：


; For Windows CE .NET devices


[HKEY_CURRENT_USER\Software\Symbol\Startup\Programs\Prog14]


"Name"="\windows\wceload.exe"


"Command"="/noaskdest \Application\cab\NETCFv2.wce5.armv4i.cab"


"Continue"=dword:0


"ColdBootOnly"=dword:1


这两种方法只有在冷启动时才执行，因而不能满足要求。


后来发现可以用bat文件，放在StartUP目录下，内容类似Wceload /NOASKDEST \Application\CAB\NETCFv2.wce5.armv4i.cab，这样每次启动机子都会执行，也就实现自动安装功能了。


五、其它知识


.cpy文件拷贝快捷方式命令如下例：


\application\PDlnk.lnk    >    \%WSM%\PDlnk.lnk 

\application\PDlnk.lnk    >    \%WDT%\PDlnk.lnk 

注：Windows System


Windows DeskTop


.cpy文件是冷启时用来复制文件到指定目录的命令文件。CE系统快捷方式为.lnk文件，命令行中必须使用全名（包含后缀），否则失败。


.lnk文件直接通过ActiveSync由设备拷贝出来即可。


六、安装程序


程序一般都很小，还要设置快捷方式，又不想做成CAB包（主要是VS2003制作CAB太不方便了），所以就采用了一种另类的方法。把程序拷到 PDA中，设置好快捷方式，然后把部署需要的CAB包，及程序和快捷方式对应的目录结构拷下来。当PDA冷启动，数据丢失后，直接把这些文件拷到机了上，重启一下，就会自动安装了。



