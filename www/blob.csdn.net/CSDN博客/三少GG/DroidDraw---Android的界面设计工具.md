# DroidDraw---Android的界面设计工具 - 三少GG - CSDN博客
2011年11月29日 13:40:41[三少GG](https://me.csdn.net/scut1135)阅读数：2412
# [Android的界面设计工具——DroidDraw](http://www.cnblogs.com/wanlipeng/archive/2010/08/18/1802715.html)
首届 Google 暑期大学生博客分享大赛——2010 Andriod 篇
软件名称：DroidDraw
软件大小：489KB(Windows版本)
支持系统：Mac OS X/Windows/Linux
下载地址：[http://code.google.com/p/droiddraw/](http://code.google.com/p/droiddraw/)
      ADT中的界面开发工具实在是很烂，通常情况下都需要硬编码，对于程序员来说不但效率比较低下，而且调试起来极其不方便，还好在Google未推出GUI的“所见即所得”的工具之前，我们找到这款软件DroidDraw，这款软件采用Java开发而成。
      DroidDraw目前是一个公开的Google Code项目，可以免费在线下载([http://code.google.com/p/droiddraw/](http://code.google.com/p/droiddraw/))，也可以直接使用在线的版本（[http://www.droiddraw.org.sixxs.org/](http://www.droiddraw.org.sixxs.org/)[](http://www.droiddraw.org/)）。通过该软件可以很直接的进行拖拽，而且软件同时提供了对属性的修改和代码的生成。同时也提供了对string.xml和color.xml的修改。
       好了，多了不说了，如果你急不可待，快去自己下载一个吧。下面提供一个截图：
![](http://pic002.cnblogs.com/img/wanlipeng/201008/2010081818012069.jpg)
﻿﻿﻿﻿
**1、       GUI可视化设计器——DroidDraw**
Dr oidDraw是基于Java Swing的Android界面设计器，可以通过它生成复杂的Android Layout Xml文件。
2、AnDroidDrow
     AnDroidDrow是与DroidDraw集成的Android应用程序，它允许你从DroidDraw应用程序下载你的GUI，也允许你在一个AnDroidDraw设备上预览你的GUI。
      步骤零：
          下载AnDroidDraw.apk
           使用 adb install AnDroidDraw.apk把它安装到你的Android设备上。（参考CSDN做的笔记）
          安装一个端口转发规则：adb forward tcp:6100 tcp:7100
步骤一：
在你的Android设备上运行AnDroidDraw，你应该看到这样。
       步骤二：
在你的电脑上运行DroidDraw，并创建一个GUI，接下来从DroidDraw菜单中选择“project->send GUI to Device”。
步骤三：
现在可以在Android屏幕看到新创建的GUI的XML。如图。
步骤四：
点击“preview Layout”按钮预览你的GUI。
步骤五：
当你结束时，点击向后的箭头，来返回AnDroidDraw的主屏幕。
记住：如果你感兴趣，你可以在文件框中编辑该XML文件，并且你再次点击“Previewlayout”来查看修改。然而，这些修改并不会返回到DroidDraw。
步骤六：
OK。
**DroidDraw教程一：Currency Converter**
步骤零：
本教程讲给你一个简短的介绍，关于使用DroidDraw用户界面设计器来开发一个Android上的GUI应用程序。
步骤一：
登陆到DroidDraw UI Designer.
步骤二：
设置根布局为RelativeLayout（相对布局）
步骤三：
选择“layouts”标签
步骤四：
从Layouts面板中把一个LinearLayout对象拖放到屏幕顶部的中心位置。
步骤五：
选择该linearlayout对象并点击属性“properties“标签开始编辑Layout属性值，把Width属性值设为”200px“，Height属性值设为”130px“，点击”Apply“来应用改变。
步骤六：
转到“Widget“标签。
步骤七：
把两个TextView对象和两个EditView对象交替拖放到LinearLayout中。
步骤八：
把一个RadioGroup对象拖放到LinearLayout中，把两个RadioButton对象拖放到RadioGroup对象中。
步骤九：
把一个Button对象放到根RelativeLayout中，它在LinearLayout下面。他应该和LinearLayout对象的右边对齐。
步骤十：
编辑每个TextView对象的属性值，上面一个文本设置成“Dollars“，
并设置成“bold“字体样式。下面一个文本设置成“Euros“，并设置成“bold“字体样式。
步骤十一：
如以下内容编辑EditView的属性值：
      id修改成：“@+id/dollors“
      文本内容设置为空
      宽度修改为：“100px“
十一步半：
      把第二个EditView也加上如上设置，只不过id修改为“@+id/euros“。
步骤十二：
编辑第一个RadioButton属性：文本设为“Dollors To Euros“,并把它的的id设为”@+id/dtoe“.
编辑第二个RadioButton属性：为本设为“Euros To Dollors“,并把它的id设为”@+id/etod“.
**重要注意事项：**
你必须正确的获取id，因为这是你在代码中如何获取搜索到该UI元素的方式。
步骤十三：
编辑Button属性，文本修改为“Convert“，它的id设置为”@+id/convert“.
步骤十四：
点击“Generate“按钮生成XML布局。
步骤十五：
在Eclipse中创建一个新的Adroid工程，从DroidDraw剪切该XML并粘贴换到res/layout/main.xml的内容。
到这里你就可以运行你的GUI了。
步骤十六：
最后一步是实际的代码转换。它不多，你可以用代码查看一下你的GUI元素。
如下代码：
this.findViewById(R.id.<id>). 
Hereis the complete code for the CurrencyConverter activity:
import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.TextView;
public class CurrencyConverter extends Activityimplements OnClickListener {
        TextViewdollars;
        TextVieweuros;
    RadioButtondtoe;
    RadioButtonetod;
        Buttonconvert;
        /** Calledwhen the activity is first created. */
    @Override
    public voidonCreate(Bundle icicle) {
       super.onCreate(icicle);
       setContentView(R.layout.main);
        dollars =(TextView)this.findViewById(R.id.dollars);
        euros =(TextView)this.findViewById(R.id.euros);
        dtoe =(RadioButton)this.findViewById(R.id.dtoe);
       dtoe.setChecked(true);
        etod =(RadioButton)this.findViewById(R.id.etod);
        convert =(Button)this.findViewById(R.id.convert);
       convert.setOnClickListener(this);
    }
        public voidonClick(View v) {
               if(dtoe.isChecked()) {
                       convertDollarsToEuros();
               }
               if(etod.isChecked()) {
                       convertEurosToDollars();
               }
        }
        protectedvoid convertDollarsToEuros() {
               doubleval = Double.parseDouble(dollars.getText().toString());
               // ina real app, we'd get this off the 'net
               euros.setText(Double.toString(val*0.67));
        }
        protectedvoid convertEurosToDollars() {
               doubleval = Double.parseDouble(euros.getText().toString());
               // ina real app, we'd get this off the 'net
               dollars.setText(Double.toString(val/0.67));
        }
}


最终效果图：

**DroidDraw教程二：TableLayout**
步骤零：
本教程将介绍如何创建一个DroidDraw输入和TableLayout布局。
步骤一：
启动DroidDraw UI Designer
步骤二：
根布局设置为relativelayout。
步骤三：
选择Layout标签
步骤四：
把一个TableLayout对象从Layout面板拖放到屏幕中间。
步骤五：
双击“TableLayout”对象，设置其属性。把它的宽度设置为“fill_parent”。
步骤六：
把三个TableRow对象从Layout面板中拖放到TableLayout中。当你拖放“TableRow”
时你应该从弹出菜单中选择TableLayout。
步骤七：
每个TableRow拖放一个TextView.
步骤八：
双击每个TextView，设置其属性，如图所示：
步骤九：
每个TableRow拖放一个EditView.
步骤十：
选中TaleLayout，修改“stretchablecolumn”（可扩展栏）属性值为1，这将把所有的EditText Widget扩展开来，填充满整个Table域。
步骤十一：
双击EditText，修改其属性，Text设置为“”。
步骤十二：
把一个Button对象拖放到TableLayout下面右下角处。它应该在TableLayout的外面，并与它右对齐。
步骤十三：
编辑Botton的属性，文本设置为“OK”
步骤十四：
点击Generate生成XML文件。
步骤十五：
在Eclipse中创建一个项目。
步骤十六：
复制DroidDraw生成的XML，将其替换到“res/layout/main.xml”中。
步骤十七：
运行程序，最终效果图.

**DroidDraw教程三：使用ListView和array资源**
步骤零：
在Eclipse中创建一个工程
步骤一：创建初始化布局
        开启DroidDraw，创建一个新的布局。
        在Widget列表中拖放一个ListView到Layout中
        双击ListView，设置其属性。
        将其宽设为“fill_paratent”.
        点击apply。
步骤二：创建一个array资源
注意：这些使用是针对独立的DroidDraw可执行文件。
点击DroidDraw中的array标签。
点击NEW来添加一个新的Array数组。
当提示名称时，使用“items”
对于数组值，使用逗号隔开
点击Save按钮，并把其保存为array.xml,将其放入到“res/values”中。
步骤三：让你的数组与列表相连。
在第一步创建的ListView上双击
修改“Entry Array Id“属性为”@arrays/items“
点击“Apply“按钮
生成的Layout布局代码保存为“main.xml“,保存到”res/layouts“中。
步骤四：
在你的mainActivity.java中使用如下代码：
/** Called when the activity is first created. */
    @Override
    public voidonCreate(Bundle icicle) {
       super.onCreate(icicle);
       this.setTitle("DroidDraw");
       setContentView(R.layout.main);
    }
**Step Five - Done!**
Run your code in the Android emulator.
