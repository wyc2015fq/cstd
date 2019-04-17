# API Guide：布局资源 - 一世豁然的专栏 - CSDN博客





2017年04月25日 21:50:36[一世豁然](https://me.csdn.net/Explorer_day)阅读数：286标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/Explorer_day/article/category/6289318)









布局资源定义UI在活动或组件中的结构。




未见位置：

res/layout/filename.xml


文件名将用作资源ID。





编译资源数据：


资源指向View（或子类）资源的指针。





资源参考：


In Java: `R.layout.*filename*`


`*In XML: `@[*package*:]layout/*filename*`*`

`*``*`

*语法：*



```
<?xml version="1.0" encoding="utf-8"?>
<ViewGroup
    xmlns:android="http://schemas.android.com/apk/res/android"
    android:id="@[+][package:]id/resource_name"
    android:layout_height=["dimension" | "match_parent" | "wrap_content"]
    android:layout_width=["dimension" | "match_parent" | "wrap_content"]
    [ViewGroup-specific attributes] >
    <View
        android:id="@[+][package:]id/resource_name"
        android:layout_height=["dimension" | "match_parent" | "wrap_content"]
        android:layout_width=["dimension" | "match_parent" | "wrap_content"]
        [View-specific attributes] >
        <requestFocus/>
    </View>
    <ViewGroup >
        <View />
    </ViewGroup>
    <include layout="@layout/layout_resource"/>
</ViewGroup>
```


注意：根元素可以是ViewGroup，View或一个<merge>元素，但是必须只有一个根元素，它必须包含xmlns：android属性和android命名空间，如图所示。





*元素：*

*1、<ViewGroup>*

其他View元素的容器。 有多种不同类型的ViewGroup对象，每一种都可以以不同的方式指定子元素的布局。 不同种类的ViewGroup对象包括LinearLayout，RelativeLayout和FrameLayout。





您不应该假设ViewGroup的任何派生都将接受嵌套视图。 一些ViewGroups是AdapterView类的实现，它只能从适配器确定它的子节点。





属性：


1）、android:id：资源ID。
 元素的唯一资源名称，可用于从应用程序获取对ViewGroup的引用。 详细了解android：id的值。


2）、android:layout_height：尺寸或必要关键字。
 组的高度，作为维度值（或维度资源）或关键字（“match_parent”或“wrap_content”）。 请参阅下面的有效值。


3）、android:layout_width：尺寸或必要关键字。
 组的宽度，作为维度值（或维度资源）或关键字（“match_parent”或“wrap_content”）。 请参阅下面的有效值。





ViewGroup基类支持更多属性，ViewGroup的每个实现都支持更多的属性。
 有关所有可用属性的参考，请参阅ViewGroup类的相应参考文档（例如，LinearLayout XML属性）。





2、<View>

单独的UI组件，通常称为“小部件”。
 不同类型的View对象包括TextView，Button和CheckBox。





属性：




1）、android:id：资源ID。
 元素的唯一资源名称，可用于从应用程序获取对View的引用。 详细了解android：id的值。


2）、android:layout_height：尺寸或必要关键字。 元素的高度，作为维度值（或维度资源）或关键字（“match_parent”或“wrap_content”）。 请参阅下面的有效值。


3）、android:layout_width：尺寸或必要关键字。 元素的宽度，作为维度值（或维度资源）或关键字（“match_parent”或“wrap_content”）。 请参阅下面的有效值。


View基类支持更多属性，View的每个实现都支持更多属性。
 阅读布局了解更多信息。 有关所有可用属性的参考，请参阅相应的参考文档（例如，TextView XML属性）。





3、<requestFocus>

表示View对象的任何元素都可以包含这个空的元素，它在屏幕上给出其父初始焦点。
 每个文件可以只有这些元素之一。





4、<include>

包含布局文件到此布局中。




属性：

layout：必要的布局资源。
 引用布局资源。


android:id：资源ID。
 覆盖在包含的布局中给出根视图的ID。


android:layout_height：尺寸或关键字。
 覆盖在包含的布局中给予根视图的高度。 只有当android：layout_width也被声明时才有效。


android:layout_width：尺寸或关键字。
 覆盖在包含的布局中给予根视图的宽度。 只有当android：layout_height也被声明时才有效。







您可以在包含布局中的根元素支持的<include>中包含任何其他布局属性，并覆盖根元素中定义的那些。




注意：如果要使用<include>标记覆盖布局属性，则必须覆盖android：layout_height和android：layout_width，以使其他布局属性生效。





包括布局的另一种方法是使用ViewStub。它是一个轻量级的视图，直到你明确地inflate它，否则它不会消耗没有布局空间，在这一点上，它包括由它的android：layout属性定义的布局文件。有关使用ViewStub的更多信息，请阅读按需加载视图。





5、<merge>

未在布局层次结构中绘制的替代根元素。当您知道此布局将放置在已包含适当的父视图的布局中，以用来包含<merge>元素的子元素时，将其用作根元素很有用。当您打算使用<include>将此布局包含在另一个布局文件中时，这特别有用，并且此布局不需要不同的ViewGroup容器。有关合并布局的详细信息，请阅读Re-using
 Layouts with <include/>。








android:id的值


对于ID值，您通常应使用以下语法形式：“@
 + id / name”。 加号+表示这是一个新的资源ID，而aapt工具将在R.java类中创建一个新的资源整数（如果它不存在）。 例如：



`<TextView android:id="@+id/nameTextbox"/>`

nameTextbox名称现在是附加到此元素的资源ID。 然后，您可以引用与Java关联的ID的TextView：



```java
findViewById(R.id.nameTextbox);
```
此代码返回TextView对象。




但是，如果您已经定义了一个ID资源（并且尚未使用），则可以通过排除android：id值中的加号来将该ID应用于View元素。







android:layout_height和android:layout_width的值

可以使用Android（px，dp，sp，pt，in，mm）支持的任何维度单位或以下关键字来表达高度和宽度值：


|值|描述|
|----|----|
|`match_parent`|将维度设置为与父元素匹配。 在API 8级中添加，以弃用fill_parent。|
|`wrap_content`|将维度设置为适合该元素的内容所需的大小。|



自定义视图元素

您可以创建自己的View和ViewGroup自定义元素，并将它们应用到与标准布局元素相同的布局。
 您还可以指定XML元素中支持的属性。 要了解更多信息，请参阅自定义组件开发人员指南。







例子：

保存在res
 / layout / main_activity.xml中的XML文件：



```
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
              android:layout_width="match_parent"
              android:layout_height="match_parent"
              android:orientation="vertical" >
    <TextView android:id="@+id/text"
              android:layout_width="wrap_content"
              android:layout_height="wrap_content"
              android:text="Hello, I am a TextView" />
    <Button android:id="@+id/button"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:text="Hello, I am a Button" />
</LinearLayout>
```


该应用程序代码将在onCreate（）方法中加载活动的布局：



```java
public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.main_activity);
}
```






