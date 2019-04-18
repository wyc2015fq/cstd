# 使用ScriptX控件进行Web打印 asp打印（源码） - 深之JohnChen的专栏 - CSDN博客

2008年11月20日 17:08:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：6822


** 使用ScriptX控件进行Web打印 asp打印（源码）**

最近做一小程序需要对Web打印进行精细的位置控制，IE自身的打印功能基本上算是半残废，无法自定义纸张，无法直接打印，并且由于浏览器版本或操作系统本身的设置问题，连最基本的边距设置都会有问题，总之打印已经成为Web程序的最大难题，因此使用ActiveX控件打印是最好的解决方案，但现在的控件动不动就价格几千上万，所幸目前免费的选择还有两个，一是windows系统本身带有WebBrowser.ExecWB控件，但这个由于系统的安全设置方面的限制，用起来不太灵光，二是使用第三方的控件ScriptX（[http://www.meadroid.com/scriptx/](http://www.meadroid.com/scriptx/)），ScriptX基本功能免费，但高级功能是收费的，下面我就把最近在项目中使用ScriptX的经验总结一下。更多细节您可以查看官方手册：[http://www.meadroid.com/scriptx/docs/printdoc.htm，需要源码在评论中留下E-mail](http://www.meadroid.com/scriptx/docs/printdoc.htm)

另注：ScriptX破解不太好找，需要ScriptX破解版的兄弟请留下您的email
因为是破解版，因此没有电子签名，需要将站加入信任站点当中，并修改部分安全规则

注：附件发送出现问题，现在传到CSDN空间下载地址：http://download.csdn.net/source/558406

**简单使用：**
在页面head中引入控件：
<object id=factory viewastext style="display:none"
classid="clsid:1663ed61-23eb-11d2-b92f-008048fdd814"
codebase="ScriptX.cab#Version=6,3,435,20">
</object>
设置打印参数：

```java
<script defer>
// -------------------基本功能，可免费使用-----------------------
factory.printing.header = "";//页眉
factory.printing.footer = "";//页脚

factory.printing.SetMarginMeasure(1);//页边距单位，1为毫米，2为英寸
//边距设置，需要注意大部分打印机都不能进行零边距打印，即有一个边距的最小值，一般都是6毫米以上
//设置边距的时候时候如果设置为零，就会自动调整为它的最小边距
factory.printing.leftMargin = 7;//左边距
factory.printing.topMargin = 7;//上边距
factory.printing.rightMargin = 7;//右边距
factory.printing.bottomMargin = 7;//下边距

factory.printing.portrait = true;//是否纵向打印，横向打印为false

//--------------------高级功能---------------------------------------------

factory.printing.printer = "EPSON LQ-1600KIII";//指定使用的打印机
//factory.printing.printer = "////cosa-data//HPLaserJ";//如为网络打印机，则需要进行字符转义

factory.printing.paperSource = "Manual feed";//进纸方式，这里是手动进纸
factory.printing.copies = 1;//打印份数
factory.printing.printBackground = false;//是否打印背景图片
factory.printing.SetPageRange(false, 1, 3); //打印1至3页

//---------------------常用函数--------------------------------
factory.printing.Print(false);//无确认打印，true时打印前需进行确认
factory.printing.PrintSetup();//打印设置
factory.printing.Preview();//打印预览
factory.printing.WaitForSpoolingComplete();//等待上一个打印任务完全送入打印池，在连续无确认打印时非常有用
factory.printing.EnumPrinters(index);//枚举已安装的所有打印机，主要用于生成打印机选择功能
</script>
示例截图
```

![ed68e31349b92b3bdc5401ba.jpg](http://hiphotos.baidu.com/my534/abpic/item/ed68e31349b92b3bdc5401ba.jpg)
不能用是因为控件被拦截，允许就可以了

![bd8d7e4e96b7f311b2de0584.jpg](http://hiphotos.baidu.com/my534/abpic/item/bd8d7e4e96b7f311b2de0584.jpg)

