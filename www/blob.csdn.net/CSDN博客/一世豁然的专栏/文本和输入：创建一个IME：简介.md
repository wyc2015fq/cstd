# 文本和输入：创建一个IME：简介 - 一世豁然的专栏 - CSDN博客





2017年05月08日 09:55:22[一世豁然](https://me.csdn.net/Explorer_day)阅读数：2738








输入法编辑器（IME）是用户可以控制，以让用户输入文本的编辑器。 Android提供了一个可扩展的输入法框架，允许应用程序为用户提供替代输入法，如屏幕键盘，甚至语音输入。 安装所需的IME后，用户可以从系统设置中选择要使用的IME，并在整个系统中使用它; 一次只能启用一个IME。




要向Android系统添加IME，您可以创建一个包含扩展InputMethodService类的Android应用程序。 此外，您通常会创建一个将设置传递给IME服务的“设置”活动。 您还可以定义显示为系统设置一部分的设置UI。





本指南涵盖以下内容：


1、IME生命周期。




2、在应用程序清单中声明TIME组件。




3、IME API。




4、设计IME UI。




5、从IME发送文本到应用程序。




6、使用IME子类型。




如果您以前没有使用IME，您应该首先阅读介绍性的屏幕输入法。 此外，SDK中包含的SoftKeyboard示例应用程序包含可以修改的示例代码，以便你可以开始构建自己的IME。








一、IME生命周期

下图描述了IME的生命周期：


![](https://img-blog.csdn.net/20170508090738693?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图1. IME的生命周期。





以下部分介绍如何实现与此生命周期之后的IME相关联的UI和代码。








二、在清单中声明IME组件

在Android系统中，IME是一个包含特殊IME服务的Android应用程序。 应用程序的清单文件必须声明服务，请求必需的权限，提供与action.view.InputMethod操作匹配的意图过滤器，并提供定义IME特征的元数据。 另外，为了提供允许用户修改IME的行为的设置界面，您可以定义可以从“系统设置”启动的“设置”活动。





以下代码段声明了IME服务。 它要求BIND_INPUT_METHOD允许服务将IME连接到系统，设置一个与操作android.view.InputMethod匹配的意图过滤器，并定义IME的元数据：




```
<!-- Declares the input method service -->
    <service android:name="FastInputIME"
        android:label="@string/fast_input_label"
        android:permission="android.permission.BIND_INPUT_METHOD">
        <intent-filter>
            <action android:name="android.view.InputMethod" />
        </intent-filter>
        <meta-data android:name="android.view.im"
android:resource="@xml/method" />
    </service>
```


下一个代码段声明了IME的设置活动。 它具有ACTION_MAIN的意图过滤器，表示此活动是IME应用程序的主要入口点：



```
<!-- Optional: an activity for controlling the IME settings -->
    <activity android:name="FastInputIMESettings"
        android:label="@string/fast_input_settings">
        <intent-filter>
            <action android:name="android.intent.action.MAIN"/>
        </intent-filter>
    </activity>
```


您还可以直接从其用户界面提供对IME设置的访问。







三、输入法API

特定于IME的类可以在android.inputmethodservice和android.view.inputmethod包中找到。 KeyEvent类对于处理键盘字符很重要。





IME的中心部分是一个服务组件，一个扩展InputMethodService的类。 除了实现正常的服务生命周期之外，该类还提供了回调，以提供您的IME的UI，处理用户输入，并将文本传递到当前具有焦点的字段。 默认情况下，InputMethodService类提供了用于管理IME的状态和可见性以及与当前输入字段通信的大多数实现。





以下类也很重要：


[BaseInputConnection](https://developer.android.google.cn/reference/android/view/inputmethod/BaseInputConnection.html)


将从InputMethod返回到正在接收其输入的应用程序的通信通道定义。 您可以使用它来读取光标周围的文本，将文本提交到文本框，并将原始密钥事件发送到应用程序。 应用程序应该扩展这个类，而不是实现基本接口InputConnection。





[KeyboardView](https://developer.android.google.cn/reference/android/inputmethodservice/KeyboardView.html)


视图的扩展，用于呈现键盘并响应用户输入事件。 键盘布局由键盘实例指定，您可以在XML文件中定义。








四、设计输入法UI

IME有两个主要的视觉元素：输入视图和候选视图。 您只需要实现与您正在设计的输入法相关的元素。





一）、输入视图

输入视图是UI，用户以键盘，手写或手势的形式输入文本。 当IME第一次显示时，系统调用onCreateInputView（）回调。 在实现此方法时，您将创建要在IME窗口中显示的布局，并将布局返回到系统。 此代码段是实现onCreateInputView（）方法的示例：




```java
@Override
public View onCreateInputView() {
    MyKeyboardView inputView =
        (MyKeyboardView) getLayoutInflater().inflate(R.layout.input, null);

    inputView.setOnKeyboardActionListener(this);
    inputView.setKeyboard(mLatinKeyboard);

    return mInputView;
}
```


在此示例中，MyKeyboardView是一个呈现键盘的KeyboardView的自定义实现的实例。 如果您正在构建传统的QWERTY键盘，请参阅KeyboardView类。




二）、候选视图

候选视图是UI，其中IME显示潜在的单词更正或用户要选择的建议。 在IME生命周期中，当准备好显示候选视图时，系统调用onCreateCandidatesView（）。 在实现此方法时，返回显示单词建议的布局，如果不想显示任何内容，则返回null。 空响应是默认行为，因此如果不提供建议，则不需要实现。





有关提供用户建议的示例实现，请参阅[SoftKeyboard](https://android.googlesource.com/platform/development/+/master/samples/SoftKeyboard/)示例应用程序。





三）、UI设计考虑

本节介绍IME的一些特定UI设计注意事项。





1、处理多个屏幕尺寸

您的IME的UI必须能够扩展不同的屏幕尺寸，并且还必须处理横向和纵向方向。 在非全屏IME模式下，为应用程序留出足够的空间来显示文本字段和任何相关的上下文，以便IME不超过屏幕的一半占用。 在全屏IME模式下，这不是一个问题。





2、处理不同的输入类型

Android文本字段允许您设置特定的输入类型，例如自由格式的文本，数字，URL，电子邮件地址和搜索字符串。 当您实现新的IME时，您需要检测每个字段的输入类型并为其提供适当的界面。 但是，您不必设置IME来检查用户输入的输入类型的有效文本; 这是拥有文本字段的应用程序的责任。





例如，以下是Android平台提供的拉丁语IME提供的文本和电话号码输入的界面截图：


![](https://img-blog.csdn.net/20170508092320534?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170508092324096?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图2.拉丁语IME输入类型。





当输入字段接收到焦点并且您的IME启动时，系统调用onStartInputView（），传递一个EditorInfo对象，该对象包含有关文本字段的输入类型和其他属性的详细信息。 在此对象中，inputType字段包含文本字段的输入类型。





输入类型字段是包含各种输入类型设置的位模式的int。 要测试文本字段的输入类型，请使用常量TYPE_MASK_CLASS进行掩码，如下所示：




```java
inputType & InputType.TYPE_MASK_CLASS
```


输入类型位模式可以有几个值之一，包括：

[TYPE_CLASS_NUMBER](https://developer.android.google.cn/reference/android/text/InputType.html#TYPE_CLASS_NUMBER)


用于输入数字的文本字段。 如上一屏幕截图所示，拉丁语IME显示此类型字段的数字键盘。





[TYPE_CLASS_DATETIME](https://developer.android.google.cn/reference/android/text/InputType.html#TYPE_CLASS_DATETIME)


用于输入日期和时间的文本字段。





[TYPE_CLASS_PHONE](https://developer.android.google.cn/reference/android/text/InputType.html#TYPE_CLASS_PHONE)


用于输入电话号码的文本字段。





[TYPE_CLASS_TEXT](https://developer.android.google.cn/reference/android/text/InputType.html#TYPE_CLASS_TEXT)


用于输入所有支持的字符的文本字段。





这些常量在[InputType](https://developer.android.google.cn/reference/android/text/InputType.html)的参考文档中有更详细的描述。




inputType字段可以包含指示文本字段类型变体的其他位，例如：


[TYPE_TEXT_VARIATION_PASSWORD](https://developer.android.google.cn/reference/android/text/InputType.html#TYPE_TEXT_VARIATION_PASSWORD)


用于输入密码的TYPE_CLASS_TEXT的变体。 输入法将显示dingbats而不是实际的文本。





[TYPE_TEXT_VARIATION_URI](https://developer.android.google.cn/reference/android/text/InputType.html#TYPE_TEXT_VARIATION_URI)


用于输入Web URL和其他统一资源标识符（URI）的TYPE_CLASS_TEXT的变体。





[TYPE_TEXT_FLAG_AUTO_COMPLETE](https://developer.android.google.cn/reference/android/text/InputType.html#TYPE_TEXT_FLAG_AUTO_COMPLETE)


用于从字典，搜索或其他工具输入应用程序“自动完成”的文本的TYPE_CLASS_TEXT的变体。





当您测试这些变体时，请记住使用适当的常量屏蔽输入类型。 可用的掩码常量在输入类型的参考文档中列出。





注意：在您自己的IME中，确保在将其发送到密码字段时正确处理文本。 在输入视图和候选视图中隐藏用户界面中的密码。 还要记住，您不应该在设备上存储密码。 要了解更多信息，请参阅“[安全设计指南](https://developer.android.google.cn/training/articles/security-tips.html)”。








五、发送文本到应用程序

当用户使用IME输入文本时，您可以通过发送单个键事件或通过在应用程序的文本字段中编辑光标周围的文本来向应用程序发送文本。 在这两种情况下，您都可以使用InputConnection实例来传递文本。 要获取此实例，请调用InputMethodService.getCurrentInputConnection（）。





一）、编辑光标周围的文字

当您处理文本字段中现有文本的编辑时，BaseInputConnection中的一些更有用的方法是：


[getTextBeforeCursor()](https://developer.android.google.cn/reference/android/view/inputmethod/BaseInputConnection.html#getTextBeforeCursor(int,%20int))


返回包含当前光标位置之前所请求字符数的CharSequence。





[getTextAfterCursor()](https://developer.android.google.cn/reference/android/view/inputmethod/BaseInputConnection.html#getTextAfterCursor(int,%20int))


返回一个包含当前光标位置后所请求字符数的CharSequence。





[deleteSurroundingText()](https://developer.android.google.cn/reference/android/view/inputmethod/BaseInputConnection.html#deleteSurroundingText(int,%20int))


删除当前光标位置之前和之后的指定字符数。





[commitText()](https://developer.android.google.cn/reference/android/view/inputmethod/BaseInputConnection.html#commitText(java.lang.CharSequence,%20int))


将一个CharSequence提交到文本字段并设置一个新的光标位置。





例如，以下代码段显示了如何使用文本“Hello！”替换光标左侧的四个字符：




```java
InputConnection ic = getCurrentInputConnection();
ic.deleteSurroundingText(4, 0);
ic.commitText("Hello", 1);
ic.commitText("!", 1);
```



二）、提交前撰写文字

如果您的IME进行文字预测或需要多个步骤来组合字形或单词，则可以在文本字段中显示进度，直到用户提交单词，然后可以将部分组合替换为已完成的文本。 当您将其传递给setComposingText（）时，可以通过添加“span”来对文本进行特殊处理。





以下代码段显示了如何在文本字段中显示进度：




```java
InputConnection ic = getCurrentInputConnection();
ic.setComposingText("Composi", 1);
ic.setComposingText("Composin", 1);
ic.commitText("Composing ", 1);
```


以下屏幕截图显示了用户如何显示：

![](https://img-blog.csdn.net/20170508093639718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170508093705062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170508093712681?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图3.提交前撰写文字。





三）、拦截硬件关键事件

即使输入法窗口没有明确的焦点，它首先接收硬件密钥事件，也可以选择使用它们或将其转发到应用程序。 例如，您可能需要使用方向键在UI中导航，以便在组合期间进行候选人选择。 您可能还需要捕获后退键以关闭源自输入法窗口的任何弹出窗口。





要截取硬件密钥，请覆盖onKeyDown（）和onKeyUp（）。 请参阅[软键盘](https://android.googlesource.com/platform/development/+/master/samples/SoftKeyboard/)示例应用程序作为示例。





记住对于你不想处理自己的键，要调用super（）方法。








六、创建IME子类型

子类型允许IME公开IME支持的多种输入模式和语言。 子类型可以表示：


1、一个区域设置，如en_US或fr_FR。




2、输入模式，如语音，键盘或手写。




3、特定于IME的其他输入样式，表单或属性，如10键或Qwerty键盘布局。




基本上，模式可以是诸如“键盘”，“声音”等的任何文本。 子类型也可以暴露这些的组合。





子类型信息用于通知栏中可用的IME切换器对话框，也用于IME设置。 该信息还允许框架直接提出IME的特定子类型。 构建IME时，使用子类型工具，因为它可以帮助用户识别和切换不同的IME语言和模式。





您可以使用<subtype>元素在输入法的XML资源文件之一中定义子类型。 以下代码片段定义了一个具有两个子类型的IME：美国英语区域设置的键盘子类型，法国法语区域的另一个键盘子类型：




```
<input-method xmlns:android="http://schemas.android.com/apk/res/android"
        android:settingsActivity="com.example.softkeyboard.Settings"
        android:icon="@drawable/ime_icon">
    <subtype android:name="@string/display_name_english_keyboard_ime"
            android:icon="@drawable/subtype_icon_english_keyboard_ime"
            android:imeSubtypeLanguage="en_US"
            android:imeSubtypeMode="keyboard"
            android:imeSubtypeExtraValue="somePrivateOption=true" />
    <subtype android:name="@string/display_name_french_keyboard_ime"
            android:icon="@drawable/subtype_icon_french_keyboard_ime"
            android:imeSubtypeLanguage="fr_FR"
            android:imeSubtypeMode="keyboard"
            android:imeSubtypeExtraValue="foobar=30,someInternalOption=false" />
    <subtype android:name="@string/display_name_german_keyboard_ime" ... />
</input-method>
```


要确保您的子类型在UI中正确标记，请使用％s获取与子类型的区域标签相同的子类型标签。 这在下面的两个片段中演示。 第一个代码片段显示了输入法XML文件的一部分：



```
<subtype
    android:label="@string/label_subtype_generic"
    android:imeSubtypeLocale="en_US"
    android:icon="@drawable/icon_en_us"
    android:imeSubtypeMode="keyboard" />
```


下一个片段是IME的strings.xml文件的一部分。 由输入法UI定义用于设置子类型的标签的字符串资源label_subtype_generic定义为：


`<string name="label_subtype_generic">%s</string>`

此设置使子类型的显示名称与区域设置相匹配。 例如，在任何英文区域中，显示名称为“英语（美国）”。




一）、从通知栏中选择TIME子类型


Android系统管理所有IME公开的所有子类型。 IME子类型被视为他们所属的IME模式。 在通知栏中，用户可以为当前设置的IME选择可用的子类型，如以下屏幕截图所示：


![](https://img-blog.csdn.net/20170508094735733?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图4.从通知栏中选择IME子类型。





![](https://img-blog.csdn.net/20170508094814672?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图5.在系统设置中设置子类型首选项。





二）、从系统设置中选择IME子类型

用户可以在“系统设置”区域的“语言和输入”设置面板中控制子类型的使用方式。 在SoftKeyboard示例应用程序中，InputMethodSettingsFragment.java文件包含一个在IME设置中促进子类型启用程序的实现。 有关如何在IME中支持输入法子类型的更多信息，请参阅Android SDK中的SoftKeyboard示例应用程序。


![](https://img-blog.csdn.net/20170508094934798?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图6.选择IME的语言。








七、在IME子类型之间切换

您可以通过提供切换键（例如地球形语言图标）作为键盘的一部分，允许用户在多个IME子类型之间轻松切换。 这样做大大提高了键盘的可用性，并可以帮助避免用户的沮丧。 要启用此类切换，请执行以下步骤：


1、声明支持在输入法的XML资源文件中切换ToNextInputMethod =“true”。 您的声明应与以下代码片段相似：



```
<input-method xmlns:android="http://schemas.android.com/apk/res/android"
        android:settingsActivity="com.example.softkeyboard.Settings"
        android:icon="@drawable/ime_icon"
        android:supportsSwitchingToNextInputMethod="true">
```


2、调用shouldOfferSwitchingToNextInputMethod（）方法。




3、如果方法返回true，则显示切换键。




4、当用户点击切换键时，调用switchToNextInputMethod（），将false传递给第二个参数。 值false表示系统平等对待所有子类型，不管它们属于什么IME。 指定true需要系统在当前IME中循环遍历子类型。




注意：在Android 5.0（API级别21）之前，switchToNextInputMethod（）不知道supportsSwitchingToNextInputMethod属性。 如果用户切换到没有切换键的IME，他或她可能会卡在该IME中，无法轻松切换出来。








八、一般IME注意事项

在实施TIME时，还有其他一些需要考虑的事项：

1、为用户提供一种直接从IME的用户界面设置选项的方法。




2、由于可能会在设备上安装多个TIME，为用户提供一种直接从输入法UI切换到不同IMEI的方式。




3、快速启动IME的UI。 预先加载或加载任何大量资源，以便用户在点击文本字段后立即看到IME。 缓存资源和视图，以便随后调用输入法。




4、相反，您应该在隐藏输入法窗口后立即释放大量内存分配，以便应用程序可以有足够的内存来运行。 如果IME处于隐藏状态几秒钟，请考虑使用延迟消息来释放资源。




5、确保用户可以为与IME相关联的语言或语言环境输入尽可能多的字符。 请记住，用户可能会使用密码或用户名中的标点符号，因此您的IME必须提供许多不同的字符，以便用户输入密码并访问设备。



