# 《Training:Building a Simple User Interface》 - 一世豁然的专栏 - CSDN博客





2017年04月12日 21:13:43[一世豁然](https://me.csdn.net/Explorer_day)阅读数：284标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/Explorer_day/article/category/6289318)









注：本人非专业翻译人员，编写此博客只是为了自学使用，如有疑问，请参考官方文档




一、Create a Linear Layout

在本课中，您将以XML格式创建一个包含文本字段和按钮的布局。在下一课程，当按钮通过发送将文本字段的内容发送到另一个活动的方式被按下时，你的app将响应。





Android app的图形用户界面是使用View和ViewGroup对象的层次结构构建的。 查看对象通常是UI小部件，如按钮或文本字段。 ViewGroup对象是不可见的视图容器，用于定义子视图的布局方式，如网格或垂直列表。





Android提供了与View和ViewGroup的子类相对应的XML词汇表，因此您可以使用UI元素的层次结构在XML中定义UI。





Layouts是ViewGroup的子类。 在本练习中，您将使用LinearLayout。


![](https://img-blog.csdn.net/20170410214056634?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图1. ViewGroup对象如何在布局中形成分支并包含其他View对象的图示。








一、创建一个Linear Layout

1、从`res/layout/`目录中，打开activity_main.xml文件。此XML文件定义您的活动的布局。 它包含默认的“Hello World”文本视图。




2、当你打开布局文件时，首先在布局编辑器中显示的是设计编辑器。 对于本课程，您可以直接使用XML，因此单击文本选项卡切换到文本编辑器。




3、用以下XML替换文件的内容：



```
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout
    xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:orientation="horizontal">
</LinearLayout>
```



LinearLayout是一个视图组（ViewGroup的子类），它以垂直或水平方向显示子视图，如android：orientation属性所指定。 LinearLayout的每个孩子按照XML中显示的顺序显示在屏幕上。





为了指定它们的大小，所有视图都需要另外两个属性android：layout_width和android：layout_height。





因为LinearLayout是布局中的根视图，所以应该通过将宽度和高度设置为“match_parent”来填充应用程序可用的整个屏幕区域。 该值声明视图应该扩展其宽度或高度以匹配父视图的宽度或高度。





有关布局属性的更多信息，请参阅Layout指南。






二、Add a Text Field

在activity_main.xml文件中的<LinearLayout>元素中，添加以下<EditText>元素：




```
<LinearLayout
    xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:orientation="horizontal">
    <EditText android:id="@+id/edit_message"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:hint="@string/edit_message" />
</LinearLayout>
```






以下是您添加的<EditText>属性的描述：

android:id

这为视图提供了唯一的标识符，您可以使用它来从app代码引用对象，例如读取和操作对象（下一课将看到）。

当您引用XML中的任何资源对象时，@符号是必需的。 之后是资源类型（在这种情况下为id），斜杠，然后是资源名称（edit_message）。


资源类型之前的加号（+）仅在您首次定义资源ID时才需要。 编译app时，SDK工具使用ID名称在项目的gen / R.java文件中创建一个引用EditText元素的新资源ID。 通过这种方式声明资源ID，对ID的其他引用不需要加号。 仅当指定新的资源ID并且不需要具体的资源（如字符串或布局）时，才需要使用加号。 有关资源对象的更多信息，请参阅侧边框。





android:layout_width和android:layout_height

“wrap_content”值不是为宽度和高度使用特定的大小，而是指定视图应该只是视需要的那么大，以适应视图的内容。 如果你改为使用“match_parent”，那么EditText元素会填满屏幕，因为它会匹配父线程LinearLayout的大小。 有关详细信息，请参阅Layouts指南。





android:hint

这是当文本字段为空时显示的默认字符串。“@ string / edit_message”值是指在单独文件中定义的字符串资源， 而不是使用硬编码字符串作为值。因为这是指具体资源（不仅仅是一个标识符），它不需要加号。 但是，由于您尚未定义字符串资源，您将首先看到编译器错误。 您将在下一节中通过定义字符串来解决此问题。





注意：此字符串资源与元素ID：edit_message具有相同的名称。 但是，对资源的引用总是由资源类型（例如id或string）限定，因此使用相同的名称不会导致冲突。







三、Add String Resources

默认情况下，您的Android项目在res / values / strings.xml中包含字符串资源文件。 在这里，您将添加两个新的字符串。


1、在res / values /目录中，打开strings.xml。




2、添加两个字符串，使如下所示



```
<?xml version="1.0" encoding="utf-8"?>
<resources>
    <string name="app_name">My First App</string>
    <string name="edit_message">Enter a message</string>
    <string name="button_send">Send</string>
</resources>
```






对于用户界面中的文本，请始终将每个字符串指定为资源。 字符串资源允许您在单个位置管理所有UI文本，这使得文本更容易查找和更新。 外部化字符串还允许您通过为每个字符串资源提供替代定义来将您的app本地化为不同的语言。


有关使用字符串资源将你的app本地化成其他语言的更多信息，请参阅Supporting Different Devices课程。








四、Add a Button

返回到activity_main.xml文件，并在<EditText>之后添加一个按钮。 如下所示：




```
<LinearLayout
    xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:orientation="horizontal"
    android:layout_width="match_parent"
    android:layout_height="match_parent">
        <EditText android:id="@+id/edit_message"
          android:layout_width="wrap_content"
          android:layout_height="wrap_content"
          android:hint="@string/edit_message" />
        <Button
          android:layout_width="wrap_content"
          android:layout_height="wrap_content"
          android:text="@string/button_send" />
</LinearLayout>
```






注意：此按钮不需要android：id属性，因为它不会从活动代码引用。




布局目前的设计使得EditText和Button小部件只需要适合其内容的大小，如下所示。
![](https://img-blog.csdn.net/20170412210016535?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


按钮可以正常工作，但对于文本字段来说并非如此，因为用户可能会输入更长的字符串。 用文本字段填充未使用的屏幕宽度是很好的。 您可以在LinearLayout中使用weight属性来执行此操作，您可以使用android：layout_weight属性指定。





权重值是一个数字，指定每个视图相对于兄弟视图消耗的数量来说应消耗的剩余空间量。 这种做法像饮料配方中的成分一样：“2份苏打，1份糖浆”是指三分之二的饮料是苏打水。 例如，如果给一个视图权重为2，另一个权重为1，则总和为3，因此第一个视图填充剩余空间的2/3，第二个视图填充其余视图。 如果您添加第三个视图并给予其权重1，则第一个视图（重量为2）现在获得1/2的剩余空间，而剩下的两个视图获得1/4。





所有视图的默认权重为0，因此，如果仅将一个权值指定为大于0的权重，则该视图会填充在所有视图被赋予所需视图空间之后的剩余空间。








五、Make the Input Box Fill in the Screen Width

在activity_main.xml中，修改<EditText>，使属性如下所示：




```
<EditText android:id="@+id/edit_message"
    android:layout_weight="1"
    android:layout_width="0dp"
    android:layout_height="wrap_content"
    android:hint="@string/edit_message" />
```
因为权重值需要另一个宽度计算来填充剩余空间，所以将宽度设置为零（0dp）可提高布局性能——使用“wrap_content”作为宽度要求系统计算最终不相关的宽度。
![](https://img-blog.csdn.net/20170412210925036?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



完整的activity_main.xmllayout文件应该如下所示：




```
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
   xmlns:tools="http://schemas.android.com/tools"
   android:orientation="horizontal"
   android:layout_width="match_parent"
   android:layout_height="match_parent">
    <EditText android:id="@+id/edit_message"
        android:layout_weight="1"
        android:layout_width="0dp"
        android:layout_height="wrap_content"
        android:hint="@string/edit_message" />
    <Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="@string/button_send" />
</LinearLayout>
```









六、Run Your App

此布局由创建项目时SDK工具生成的默认Activity类应用。




要运行应用程序并查看结果，请单击工具栏中的![](https://img-blog.csdn.net/20170412211137476?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)Run“app”。










