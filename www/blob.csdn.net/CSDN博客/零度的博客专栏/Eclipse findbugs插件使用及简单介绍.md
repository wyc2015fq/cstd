# Eclipse findbugs插件使用及简单介绍 - 零度的博客专栏 - CSDN博客
2016年06月17日 10:11:30[零度anngle](https://me.csdn.net/zmx729618)阅读数：4622
    FindBugs是一个可以在Java程序中发现Bugs的程序。它是专门用来寻找处于"Bug Patterns"列表中的代码的。Bug Patterns指很有可能是错误的代码的实例。
   打开Bug Details视图Windows => Show View => Other… => FindBugs => BugDetails。
![](http://images.cnblogs.com/cnblogs_com/doit8791/201210/201210222322478953.jpg)
     在Package Explorer或Navigator视图中，选中你的Java项目，右键，可以看到"Find Bugs"菜单项，子菜单项里有"FindBugs"和"Clear Bug Markers"两项内容，如下图所示：
我们建立一个简单的测试文件Test.java 内容如下：
`<span style="font-family:SimSun;font-size:12px;"></span><table style="background: rgb(230, 230, 230); border-collapse: collapse;" border="0"><colgroup><col style="width: 400px;" /></colgroup><tbody valign="top"><tr><td valign="middle" style="padding: 2px; border: 0.75pt outset currentColor;"><p><span style="font-family:SimSun;font-size:12px;">public class Test  </span><span style="font-family:SimSun;font-size:12px;">{  </span></p><p><span style="font-family:SimSun;font-size:12px;">    private String[] name;  </span></p><p><span style="font-family:SimSun;font-size:12px;">    public String[] getName() </span><span style="font-family:SimSun;font-size:12px;">{  </span></p><p><span style="font-family:SimSun;font-size:12px;">         return name;  </span></p><p><span style="font-family:SimSun;font-size:12px;">    }  </span></p><p><span style="font-family:SimSun;font-size:12px;">    public void setName(String[] name) </span><span style="font-family:SimSun;font-size:12px;">{  </span></p><p><span style="font-family:SimSun;font-size:12px;">        this.name = name;  </span></p><p><span style="font-family:SimSun;font-size:12px;">    }  </span></p><p><span style="font-family:SimSun;font-size:12px;">} </span></p><p><span style="font-family:SimSun;font-size:12px;"></span> </p></td></tr></tbody></table>`
     我们点中"Find Bugs"，运行时会出现如下进度框：运行结束后可以在Problems中看到增加了如下的警告信息内容
FindBugs运行后的警告信息内容不仅在Problems视图中显示，而且将标记在源代码标记框中，在源代码编辑器中我们可以看到警告标识，如下图： 
![](http://images.cnblogs.com/cnblogs_com/doit8791/201210/201210222322488854.jpg)
     当光标指向你的警告信息的代码上面时，就会有相应的错误提示信息，与Eclipse本身的错误或警告信息提示类似。  选中Problems视图里出现的相应问题，就会在代码编辑器里切换到相应的代码上去，方便根据相应的提示信息进行代码的修改。  在Problems视图里，选中相应的问题条目，右键，在弹出的菜单中，可以看到"Show
 Bug Details"。 点中它，会切换到Bug Details视图上去，显示更加详细的提示信息。 当然，在代码编辑窗口中，点击带有警告提示信息的图标时，也会自动切换到Bud Details窗口去，查看详细的警告信息。 根据这里详细的信息，你可以得到FindBugs为什么会对你的代码报警告信息，及相应的处理办法，根据它的提示，你可以快速方便地进行代码修改。
    根据提示，我们将代码修改成如下，再运行就不会报有警告信息了。
public class Test{ 
     private String[] name;  
     public String[] getName(){ 
        String[] temp = name; 
        return temp;  
     }  
     public void setName(String[] name) { 
       String[] temp = name; 
       this.name = temp;  
     }  
} 
**配置FindBugs**
    选择你的项目，右键 => Properties => FindBugs =>
![](http://images.cnblogs.com/cnblogs_com/doit8791/201210/201210222322535294.jpg)
       可以配置的信息包括如上图所示的四个选项的相关设置：
       1. Run FindBugs Automatically开关 
        当此项选中后，FindBugs将会在你修改Java类时自动运行，如你设置了Eclipse自动编译开关后，当你修改完Java文件保存，FindBugs就会运行，并将相应的信息显示出来。 当此项没有选中，你只能每次在需要的时候自己去运行FindBugs来检查你的代码。 
       2. Minimum priority to report选择项 
       这个选择项是让你选择哪个级别的信息进行显示，有Low、Medium、High三个选择项可以选择，很类似于Log4J的级别设置啦。 比如： 
你选择了High选择项，那么只有是High级别的提示信息才会被显示。 
你选择了Medium选择项，那么只有是Medium和High级别的提示信息才会被显示。 
你选择了Low选择项，那么所有级别的提示信息都会被显示。
        3. Enable bug categories选择项 
在这里是一些显示Bug分类的选择： 
Correctness关于代码正确性相关方面的 
Performance关于代码性能相关方面的 
Internationalization关于代码国际化相关方面的 
Multithreaded correctness关于代码多线程正确性相关方面的
Style关于代码样式相关方面的
Malicious code vulnerability关于恶意破坏代码相关方面的 
比如：如果你把Style的检查框去掉不选择中它，那么与Style分类相关的警告信息就不会显示了。其它的类似。
       4. Select bug patterns to check for选择项 
在这里你可以选择所要进行检查的相关的Bug Pattern条目 
可以从Bug codes、Detector name、Detector description中看到相应的是要检查哪些方面的内容，你可以根据需要选择或去掉相应的 检查条件。 
**三、详细说明**
       Findbugs是一个静态分析工具，它检查类或者JAR 文件，将字节码与一组缺陷模式进行对比以发现可能的问题。Findbugs自带检测器，其中有60余种Bad practice，80余种Correctness，1种 Internationalization，12种Malicious code vulnerability，27种Multithreaded correctness，23种Performance，43种Dodgy。
**Bad practice 坏的实践**
一些不好的实践，下面列举几个： 
**HE：** 类定义了equals()，却没有hashCode()；或类定义了equals()，却使用
Object.hashCode()；或类定义了hashCode()，却没有equals()；或类定义了hashCode()，却使用Object.equals()；类继承了equals()，却使用Object.hashCode()。 
**SQL：**Statement 的execute方法调用了非常量的字符串；或Prepared Statement是由一个非常量的字符串产生。 
**DE：** 方法终止或不处理异常，一般情况下，异常应该被处理或报告，或被方法抛出。 
**Correctness 一般的正确性问题**
可能导致错误的代码，下面列举几个： 
**NP：** 空指针被引用；在方法的异常路径里，空指针被引用；方法没有检查参数是否null；null值产生并被引用；null值产生并在方法的异常路径被引用；传给方法一个声明为@NonNull的null参数；方法的返回值声明为@NonNull实际是null。 
**Nm：** 类定义了hashcode()方法，但实际上并未覆盖父类Object的hashCode()；类定义了tostring()方法，但实际上并未覆盖父类Object的toString()；很明显的方法和构造器混淆；方法名容易混淆。 
**SQL：**方法尝试访问一个Prepared Statement的0索引；方法尝试访问一个ResultSet的0索引。 
**UwF：**所有的write都把属性置成null，这样所有的读取都是null，这样这个属性是否有必要存在；或属性从没有被write。 
**Internationalization 国际化**
当对字符串使用upper或lowercase方法，如果是国际的字符串，可能会不恰当的转换。 
**Malicious code vulnerability 可能受到的恶意攻击**
如果代码公开，可能受到恶意攻击的代码，下面列举几个： 
**FI：** 一个类的finalize()应该是protected，而不是public的。 
**MS：**属性是可变的数组；属性是可变的Hashtable；属性应该是package protected的。 
**Multithreaded correctness 多线程的正确性**
多线程编程时，可能导致错误的代码，下面列举几个： 
**ESync：**空的同步块，很难被正确使用。 
**MWN：**错误使用notify()，可能导致IllegalMonitorStateException异常；或错误的 
使用wait()。  
**No：** 使用notify()而不是notifyAll()，只是唤醒一个线程而不是所有等待的线程。 
**SC：** 构造器调用了Thread.start()，当该类被继承可能会导致错误。 
**Performance 性能问题**
可能导致性能不佳的代码，下面列举几个： 
**DM：**方法调用了低效的Boolean的构造器，而应该用Boolean.valueOf(…)；用类似 
Integer.toString(1) 代替new Integer(1).toString()；方法调用了低效的float的构造器，应该用静态的valueOf方法。 
**SIC：**如果一个内部类想在更广泛的地方被引用，它应该声明为static。 
**SS：** 如果一个实例属性不被读取，考虑声明为static。 
**UrF：**如果一个属性从没有被read，考虑从类中去掉。 
**UuF：**如果一个属性从没有被使用，考虑从类中去掉。 
**Dodgy 危险的**
具有潜在危险的代码，可能运行期产生错误，下面列举几个： 
**CI：** 类声明为final但声明了protected的属性。 
**DLS：**对一个本地变量赋值，但却没有读取该本地变量；本地变量赋值成null，却没有读取该本地变量。 
**ICAST：** 整型数字相乘结果转化为长整型数字，应该将整型先转化为长整型数字再相乘。 
**INT：**没必要的整型数字比较，如X <= Integer.MAX_VALUE。 
**NP：** 对readline()的直接引用，而没有判断是否null；对方法调用的直接引用，而方法可能返回null。 
**REC：**直接捕获Exception，而实际上可能是RuntimeException。 
**ST：** 从实例方法里直接修改类变量，即static属性。
