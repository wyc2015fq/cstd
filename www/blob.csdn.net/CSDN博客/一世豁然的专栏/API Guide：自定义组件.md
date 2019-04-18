# API Guide：自定义组件 - 一世豁然的专栏 - CSDN博客





2017年04月29日 10:41:55[一世豁然](https://me.csdn.net/Explorer_day)阅读数：504标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/Explorer_day/article/category/6289318)









Android提供了一个复杂而强大的基于基本的布局类：View和ViewGroup的组件化模型，以用来构建您的UI。 为了实现这些，平台包括各种预构建的View和ViewGroup子类 - 分别称为小部件和布局 - 可用于构建你的UI。




可用小部件的部分列表包括Button，TextView，EditText，ListView，CheckBox，RadioButton，Gallery，Spinner以及更专用的AutoCompleteTextView，ImageSwitcher和TextSwitcher。





可用的布局有LinearLayout，FrameLayout，RelativeLayout等。 有关更多示例，请参阅通用布局对象。





如果没有预先构建的小部件或布局满足您的需要，则可以创建自己的View子类。 如果只需要对现有的窗口小部件或布局进行小的调整，那么可以简单地对窗口小部件或布局进行子类化，并覆盖其方法。





创建您自己的View子类可以精确控制屏幕元素的外观和功能。 通过自定义视图来了解您获得的控制，以下是您可以使用它们的一些示例：


1、您可以创建一个完全自定义渲染的视图类型，例如使用2D图形渲染的“音量控制”旋钮，并且类似于模拟电子控件。

2、您可以将一组视图组件组合到一个新的单个组件中，也许可以使一些类似ComboBox（弹出列表和自由条目文本字段的组合），双窗格选择器控件（左窗格和右窗格，列表为 每个可以重新分配哪个项目在哪个列表中），等等。

3、您可以覆盖EditText组件在屏幕上呈现的方式（记事本教程使用此功能获得良好效果，创建一个内衬记事本页面）。

4、您可以捕获其他事件，例如按键，并以某种自定义方式（如游戏）来处理它们。




以下部分介绍如何创建自定义视图并在应用程序中使用它们。 有关详细参考信息，请参阅View类。








一、基本方法

以下是开始创建自己的View组件所需了解的高级概述：


1、使用自己的类扩展现有的View类或子类。

2、覆盖超类中的一些方法。 超类的方法以“on”开头，例如onDraw（），onMeasure（）和onKeyDown（））。 这与Activity或ListActivity中的on ...事件类似，您将为生命周期和其他功能钩子覆盖。

3、使用你的新扩展类。 一旦完成，您的新扩展类可以用于代替基于它的视图。




提示：扩展类可以在使用它们的活动中定义为内部类。 这是有用的，因为它控制对它们的访问，但不是必需的（也许您想要创建一个新的公共视图，以便在您的应用程序中更广泛地使用）。








二、完全定制的组件

完全自定义的组件可用于创建出现的图形组件。 也许是一个图形VU表，看起来像一个旧的模拟量表，或一个长长的文本视图，弹跳球沿着这些词移动，所以你可以和一个卡拉OK机一起唱歌。 无论哪种方式，你想要的东西，内置的组件不会做，无论你如何组合它们。





幸运的是，您可以轻松地以任何您喜欢的方式创建外观和行为的组件——屏幕的尺寸，可用的处理电源（请记住，最终您的应用程序可能必须运行在比桌面工作站电源低得多的东西上）。





要创建一个完全自定义的组件：


1、您可以扩展的最通用的视图是View，所以通常开始扩展它来创建新的超级组件。

2、您可以提供一个可以从XML中获取属性和参数的构造函数，还可以使用自己的这些属性和参数（也可以使用VU表的颜色和范围，或针的宽度和阻尼等）。

3、您可能希望在组件类中创建自己的事件侦听器，属性访问器和修饰符，以及可能更复杂的行为。

4、您几乎肯定要覆盖onMeasure（），如果您希望组件显示某些东西，也可能需要重写onDraw（）。 虽然两者都有默认行为，但是默认的onDraw（）将不会执行任何操作，默认的onMeasure（）将始终设置为100x100的大小 - 这可能不是您想要的。

5、其他的on...方法也可以根据需要被覆盖。




一）、扩展onDraw（）和onMeasure（）

onDraw（）方法提供了一个Canvas，您可以在其上实现所需的任何内容：2D图形，其他标准或自定义组件，样式文本或任何您可以想到的内容。





注意：这不适用于3D图形。 如果要使用3D图形，则必须扩展SurfaceView而不是View，并从单独的线程绘制。 有关详细信息，请参阅GLSurfaceViewActivity示例。





onMeasure（）有一点复杂。onMeasure（）是您的组件与其容器之间的渲染契约的关键部分。 onMeasure（）应该被覆盖，以有效和准确地报告其包含的部分的测量。 通过父级（传递给onMeasure（）方法）的限制要求，以及一旦计算出测量的宽度和高度，就调用setMeasuredDimension（）方法的要求，这一点稍微复杂一些。 如果您无法从覆盖的onMeasure（）方法调用此方法，则结果将在测量时间发生异常。





在高水平上，实现onMeasure（）看起来像这样：


1、onMeasure（）方法使用宽度和高度度量规格（widthMeasureSpec和heightMeasureSpec参数，都是代表尺寸的整数代码）而被调用，它们应该被视为对应该产生的宽度和高度测量的限制的要求。参考这些规范可以要求的限制类型可以在View.onMeasure（int，int）的参考文档中找到（本参考文档对解释整个测量操作也是一个非常好的工作）。

2、您的组件的onMeasure（）方法应该计算渲染组件所需的测量宽度和高度。 它应该尽量保持在传递的规范内，尽管它可以选择超出它们（在这种情况下，父级可以选择做什么，包括剪切，滚动，抛出异常，或者要求onMeasure（）再次尝试， 也许具有不同的测量规格）。

3、一旦计算了宽度和高度，就必须使用计算的度量来调用setMeasuredDimension（int width，int height）方法。 否则将导致抛出异常。




以下是框架调用视图的其他一些标准方法的总结：



|类别|方法|描述|
|----|----|----|
|Creation|Constructors|当从代码创建视图时，调用该构造函数的形式，当从布局文件中增加视图时调用该窗体。 第二种形式应解析并应用布局文件中定义的任何属性。|
|``[onFinishInflate()](https://developer.android.google.cn/reference/android/view/View.html#onFinishInflate())``|在一个View以及所有它的子类都被来自XML的文件充斥了时调用。| |
|Layout|``[onMeasure(int, int)](https://developer.android.google.cn/reference/android/view/View.html#onMeasure(int,%20int))``|确定此视图及其所有子项的大小要求。|
|``````[onLayout(boolean, int, int, int, int)](https://developer.android.google.cn/reference/android/view/View.html#onLayout(boolean,%20int,%20int,%20int,%20int))``````|当此视图为其所有子项分配大小和位置时调用。| |
|``````[onSizeChanged(int, int, int, int)](https://developer.android.google.cn/reference/android/view/View.html#onSizeChanged(int,%20int,%20int,%20int))``````|当此视图的大小发生变化时调用。| |
|Drawing|``[onDraw(Canvas)](https://developer.android.google.cn/reference/android/view/View.html#onDraw(android.graphics.Canvas))``|当视图呈现其内容时调用。|
|Event processing|``````[onKeyDown(int, KeyEvent)](https://developer.android.google.cn/reference/android/view/View.html#onKeyDown(int,%20android.view.KeyEvent))``````|发生新的密钥事件时调用。|
|``````[onKeyUp(int, KeyEvent)](https://developer.android.google.cn/reference/android/view/View.html#onKeyUp(int,%20android.view.KeyEvent))``````|当钥匙发生事件时调用。| |
|``[onTrackballEvent(MotionEvent)](https://developer.android.google.cn/reference/android/view/View.html#onTrackballEvent(android.view.MotionEvent))``|当轨迹球运动事件发生时调用。| |
|``[onTouchEvent(MotionEvent)](https://developer.android.google.cn/reference/android/view/View.html#onTouchEvent(android.view.MotionEvent))``|当触摸屏运动事件发生时调用。| |
|Focus|``````[onFocusChanged(boolean, int, Rect)](https://developer.android.google.cn/reference/android/view/View.html#onFocusChanged(boolean,%20int,%20android.graphics.Rect))``````|当视图获得或失去焦点时调用。|
|``[onWindowFocusChanged(boolean)](https://developer.android.google.cn/reference/android/view/View.html#onWindowFocusChanged(boolean))``|当包含视图的窗口增益或失去焦点时调用。| |
|Attaching|``[onAttachedToWindow()](https://developer.android.google.cn/reference/android/view/View.html#onAttachedToWindow())``|当视图附加到窗口时调用。|
|``[onDetachedFromWindow()](https://developer.android.google.cn/reference/android/view/View.html#onDetachedFromWindow())``|当视图与其窗口分离时调用。| |
|``[onWindowVisibilityChanged(int)](https://developer.android.google.cn/reference/android/view/View.html#onWindowVisibilityChanged(int))``|当包含视图的窗口的可视性已更改时调用。| |


二）、自定义视图示例

API Demos中的CustomView示例提供了自定义视图的示例。 CustomView在LabelView类中定义。



LabelView示例演示了自定义组件的许多不同方面：


1、扩展一个完全自定义组件的View类。

2、参数化构造函数，用于查看通货膨胀参数（XML中定义的参数）。 其中一些被传递给View超类，但更重要的是，有一些定义和用于LabelView的自定义属性。

3、你会期望看到一个标签公共方法的类型的标签组件，例如setText（），setTextSize（），setTextColor（）等等。

4、一种重写的onMeasure方法来确定和设置组件的渲染大小。 （请注意，在LabelView中，实际工作是通过私有的measureWidth（）方法完成的。）

5、重写的onDraw（）方法将标签绘制到所提供的画布上。




您可以在示例的custom_view_1.xml中看到LabelView自定义视图的一些示例用法。 特别是，您可以看到android: namespace参数和自定义 app: namespace参数的混合。 这些app: parameters是LabelView识别和使用的自定义参数，并且定义在样本R资源定义类的样式内部类中。








二、复合控制

如果您不想创建一个完全自定义的组件，而是希望将由一组现有控件组成的可重用组件放在一起，则创建一个复合组件（或复合控件）可能适合该帐单。 简而言之，这将一些更多的原子控制（或视图）汇集成一系列可以被视为单一事物的逻辑组合。 例如，组合框可以被认为是单行EditText字段和具有附加PopupList的相邻按钮的组合。 如果您按下按钮并从列表中选择某些内容，则会填充EditText字段，但如果用户喜欢，用户还可以直接在EditText中键入内容。





在Android中，实际上还有两个其他视图可以实现：Spinner和AutoCompleteTextView，但无论如何，Combo Box的概念是一个容易理解的例子。





创建复合组件：


1、通常的起点是某种布局，因此创建一个扩展布局的类。 也许在组合框的情况下，我们可能会使用一个水平方向的LinearLayout。 请记住，其他布局可以嵌套在内部，因此复合组件可以是任意复杂和结构化的。 请注意，就像使用Activity一样，您可以使用声明式（基于XML）的方法来创建包含的组件，也可以使用编程方式将其嵌套在代码中。




2、在新类的构造函数中，使用超类预期的任何参数，并将其传递给超类构造函数。 然后，您可以设置其他视图以在新组件中使用; 这是您创建EditText字段和PopupList的位置。 请注意，您还可以将自己的属性和参数引入可以由构造函数提取和使用的XML。




3、您还可以为包含的视图可能生成的事件创建侦听器，例如，如果创建了列表选择，则列表项单击侦听器的侦听器方法可更新EditText的内容。




4、您还可以使用访问器和修饰符创建自己的属性，例如，允许最初在组件中设置EditText值，并在需要时查询其内容。




5、在扩展Layout的情况下，您不需要重写onDraw（）和onMeasure（）方法，因为布局将具有可能正常工作的默认行为。 但是，如果需要，您仍然可以覆盖它们。




6、您可以覆盖其他on ...方法，如onKeyDown（），以便在按下某个键时从组合框的弹出列表中选择某些默认值。




总而言之，使用“布局”作为自定义控件的基础具有许多优点，包括：


1、您可以使用声明性XML文件指定布局，就像使用活动屏幕一样，或者您可以以编程方式创建视图，并将其嵌入到代码中的布局中。

2、onDraw（）和onMeasure（）方法（加上其他大部分的on...方法）可能会有合适的行为，所以你不必重写它们。

3、最后，您可以很快地构建任意复杂的复合视图，并重新使用它们，就像它们是单个组件一样。




一）、复合控制的例子

在SDK附带的API Demos项目中，有两个列表示例 - 视图/列表下的示例4和示例6演示了一个SpeechView，它扩展了LinearLayout以创建显示语音引号的组件。 示例代码中的相应类是List4.java和List6.java。








三、修改现有视图类型

有一个更容易的选择是创建一个在某些情况下有用的自定义视图。 如果有一个已经非常类似于你想要的组件，你可以简单地扩展该组件，并且只是覆盖你想要改变的行为。 您可以使用完全自定义的组件执行所有操作，但是从View层次结构中的更专门的类开始，您还可以免费获取大量可能完全符合您想要的行为。





例如，SDK中的样本中包含一个NotePad应用程序。 这展示了使用Android平台的许多方面，其中包括扩展一个EditText视图，使一个内衬的记事本。 这不是一个完美的例子，这样做的API可能会从这个早期的预览中改变，但它确实说明了原理。





如果您还没有这样做，请将NotePad样本导入Android Studio（或使用提供的链接查看源代码）。 特别看看NoteEditor.java文件中MyEditText的定义。





这里有一些要点


1、定义

该类定义如下：


public static class MyEditText extends EditText


I、它被定义为NoteEditor活动中的内部类，但它是公共的，因此如果需要，可以从NoteEditor类的外部作为NoteEditor.MyEditText访问。


II、它是静态的，这意味着它不会产生所谓的“合成方法”，允许它从父类访问数据，这反过来意味着它真的表现为一个单独的类，而不是与NoteEditor强烈相关的东西。 如果不需要从外部类访问状态，使生成的类保持较小，并允许其从其他类轻松使用，这是创建内部类的更简洁的方法。

III、它扩展了EditText，这是我们在这种情况下选择定制的视图。 完成后，新类将可以替换普通的EditText视图。




2、类初始化

一如以往，超类被第一个调用。 此外，这不是一个默认构造函数，而是一个参数化的构造函数。 当使用这些参数从XML布局文件中充实时，使用这些参数创建EditText，因此，我们的构造函数需要将它们传递给超类构造函数。





3、覆盖方法

在此示例中，只有一种方法可以被覆盖：onDraw（） - 但是当您创建自己的自定义组件时，可能很容易需要其他方法。





对于NotePad示例，覆盖onDraw（）方法允许我们在EditText视图画布上绘制蓝线（画布被传递到覆盖的onDraw（）方法）。 在方法结束之前调用super.onDraw（）方法。 应该调用超类方法，但在这种情况下，我们在绘制我们要包括的行之后最后再做。





4、使用自动组件

我们现在有了我们的定制组件，但是我们如何使用它？ 在NotePad示例中，自定义组件直接从声明性布局使用，因此请查看res / layout文件夹中的note_editor.xml。




```
<view
  class="com.android.notepad.NoteEditor$MyEditText"
  id="@+id/note"
  android:layout_width="fill_parent"
  android:layout_height="fill_parent"
  android:background="@android:drawable/empty"
  android:padding="10dip"
  android:scrollbars="vertical"
  android:fadingEdge="vertical" />
```


I、自定义组件被创建为XML中的通用视图，并且使用完整的包指定类。 还要注意，我们定义的内部类是使用NoteEditor $ MyEditText符号来引用的，这是一种标准方法来引用Java编程语言中的内部类。

如果您的自定义View组件未定义为内部类，那么您也可以使用XML元素名声明View组件，并排除该类属性。 例如：




```
<com.android.notepad.MyEditText
  id="@+id/note"
  ... />
```
请注意，MyEditText类现在是一个单独的类文件。 当类嵌套在NoteEditor类中时，此技术将无法正常工作。


II、定义中的其他属性和参数是传递给自定义组件构造函数的属性和参数，然后传递给EditText构造函数，因此它们与EditText视图使用的参数相同。 请注意，您也可以添加自己的参数，下面将再次介绍。




这就是它的一切。 诚然，这是一个简单的例子，但这就是要点 - 创建自定义组件只是你需要的那么复杂。





更复杂的组件可以覆盖更多的...方法，并介绍一些自己的帮助方法，大大定制其属性和行为。 唯一的限制是你的想象力和你需要组件做什么。




