# Android第四讲--用户界面 - Orisun - 博客园







# [Android第四讲--用户界面](https://www.cnblogs.com/zhangchaoyang/articles/1958864.html)





虽然放在第四讲，但这一讲是最简单的，应该跟在第一讲之后。

res/values下面可以创建一些简单的值如

strings.xml

<?xml version="1.0" encoding="utf-8"?>
<resources>
    <string name="hello">Hello World, ToDoList!</string>
    <string name="app_name">To Do List</string>
    <string name="app_new">Add New Item</string>
    <string name="remove">Remove Item</string>
    <string name="cancel">Cancel</string>
</resources>

colors.xml

<?xml version="1.0" encoding="utf-8"?>
<resources>
	<color name="notepad_paper">#AAFFFF99</color>
	<color name="notepad_lines">#FF0000FF</color>
	<color name="notepad_text">#AA0000FF</color>
	<color name="notepad_margin">#90FF0000</color>
</resources>


dimens.xml

<?xml version="1.0" encoding="utf-8"?>
<resources>
	<dimen name="notepad_margin">30px</dimen>
</resources>


还可以存放样式和主题

style.xml

<?xml version="1.0" encoding="utf-8"?>
<resources>
        <style name="BaseText">
                <item name="android:textSize">14sp</item>
                <item name="android:textColor">#111</item>
        </style>
        <style name="SmallText" parent="BaseText">
                <item name="android:textSize">8sp</item><item>
        </style>

	<style name="ToDoTheme" parent="@android:style/Theme.Black">
		<item name="android:textSize">12sp</item>
	</style>
</resources>


样式和主题都是style节点。并且parent属性可以指定样式之间的继承关

系，SmallText继承了BaseText，ToDoTheme继承了android:style/Theme.Black。在一个resources里面引用其他resources里面的东西要加@前缀。

我们定义好了这么多资源，在java代码里面怎么用起来呢？

Resources myResources=getResources();
CharSeqence styleText=myResources.getText(R.string.hello);
Drawablw icon=myResources.getDrawable(R.drawable.app_icon);
int opaqueBlue=myResources.getColor(R.color.notepad_paper);
float borderWidth=myResources.getDimension(R.dimen.notepad_margin);

String[] stringArray;
stringArray=myResources.getStringArray(R.array.string_array);
int[] intArray=myResources.getIntArray(R.array.integer_array);


资源的相互引用

attribute="@[packagename:]resourcetype/resourceidentifier"

<EditText
        android:text="@string/cancle"
        android:textColor="@color/opaqueBlue"
/>


使用系统资源

android:textColor="@android:color/dark_gray"


在当前主题中引用样式(style)

android:textColor="?android:textColor"


使用自定义的主题，在AndroidManifest文件里面：


<activity android:name=".ToDoList"
                  android:label="@string/app_name"
                  android:theme="@style/ToDoTheme">
</activity>


同ASP.NET一样，在Android里面你可以创建自定义控件，比如自定义一个类extends TextView或extends LinearLayout，重写里面的方法来更改其外观，或者是自定义的事件响应功能。也可以创建复合控件。












