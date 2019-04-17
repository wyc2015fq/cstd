# 《Training:Starting Another Activity》 - 一世豁然的专栏 - CSDN博客





2017年04月13日 22:38:33[一世豁然](https://me.csdn.net/Explorer_day)阅读数：244标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/Explorer_day/article/category/6289318)









注：本人非专业翻译人员，编写此博客只是为了自学使用，如有疑问，请参考官方文档




完成上一课后，您有一个显示文本字段和按钮的活动（单个屏幕）应用程序。 在本课程中，您将向MainActivity添加一些代码，以在用户单击“发送”按钮时启动新的活动。




Respond to the Send Button


1、在文件res / layout / activity_main.xml中，将android：onClick属性添加到<Button>元素中，如下所示：



```
<Button
      android:layout_width="wrap_content"
      android:layout_height="wrap_content"
      android:text="@string/button_send"
      android:onClick="sendMessage" />
```






该属性告诉系统每当用户点击按钮时，在活动中调用sendMessage（）方法。




2、在文件java / com.example.myfirstapp / MainActivity.java中，添加sendMessage（）方法，如下所示：



```java
public class MainActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    /** Called when the user clicks the Send button */
    public void sendMessage(View view) {
        // Do something in response to button
    }
}
```






为了使系统将此方法与给予android：onClick的方法名称进行匹配，签名必须完全以上代码所呈现的那样。 具体来说，该方法必须：

I、公有的（public）

II、空返回值

III、有一个View的唯一参数（这将是被点击的视图）




接下来，您将填写此方法来读取文本字段的内容，并将该文本传递给另一个活动。








Build an Intent


意图是一个对象，它提供两个独立组件（如两个活动）之间的运行时绑定。 意图表示应用程序的“打算做某事”。 您可以使用意图进行各种各样的任务，但在本课程中，您的意图开始另一个活动。





在MainActivity.java中，将以下显示的代码添加到sendMessage（）：




```java
public class MainActivity extends AppCompatActivity {
    public final static String EXTRA_MESSAGE = "com.example.myfirstapp.MESSAGE";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    /** Called when the user clicks the Send button */
    public void sendMessage(View view) {
        Intent intent = new Intent(this, DisplayMessageActivity.class);
        EditText editText = (EditText) findViewById(R.id.edit_message);
        String message = editText.getText().toString();
        intent.putExtra(EXTRA_MESSAGE, message);
        startActivity(intent);
    }
}
```






注意：Android Studio将显示无法解析符号错误，因为代码引用了类似于Intent和EditText的类，还没有导入。 要导入这些类，您可以通过按Alt + Enter（Mac上的Option + Return）或者在文件顶部手动添加导入语句来使用Android Studio的“导入类”功能。




在sendMessage（）中有很多事情要做，所以我们来解释一下代码。




Intent构造函数有两个参数：

I、一个Context作为其第一个参数（这是因为Activity类是Context的一个子类）

II、系统应该提供Intent的应用程序组件的类（在这种情况下，活动应该是启动的）。




注意：对DisplayMessageActivity的引用会在Android Studio中引发错误，因为该类还不存在。 现在忽略错误; 你会很快创建课程。




putExtra（）方法将EditText的值添加到intent。 意图可以携带作为作为名为extras的键值对的数据类型。 您的密钥是公共常量EXTRA_MESSAGE，因为下一个活动使用密钥来检索文本值。 使用您的应用程序的程序包名称作为前缀定义键值的一个很好的做法。 这样可确保密钥是独一无二的，以防您的应用与其他应用程序进行交互。




startActivity（）方法启动Intent指定的DisplayMessageActivity的实例。 现在你需要创建该类。







Create the Second Activity

1、在Project窗口中，右键单击应用程序文件夹，然后选择New > Activity > Empty Activity。




2、在Configure Activity 窗口中，为“活动名称”输入“DisplayMessageActivity”，然后单击Finish。




Android Studio自动执行三件事情：

I、使用所需的onCreate（）方法的实现创建类DisplayMessageActivity.java。

II、创建相应的布局文件activity_display_message.xml

III、在AndroidManifest.xml中添加所需的<activity>元素。




如果您运行该应用程序并单击第一个活动的“发送”按钮，则第二个活动将启动但为空。 这是因为第二个活动使用模板提供的默认空格式。







Display the Message

现在您将修改第二个活动以显示第一个活动传递的消息。

1、在DisplayMessageActivity.java中，将以下代码添加到onCreate（）方法中：



```java
@Override
protected void onCreate(Bundle savedInstanceState) {
   super.onCreate(savedInstanceState);
   setContentView(R.layout.activity_display_message);

   Intent intent = getIntent();
   String message = intent.getStringExtra(MainActivity.EXTRA_MESSAGE);
   TextView textView = new TextView(this);
   textView.setTextSize(40);
   textView.setText(message);

   ViewGroup layout = (ViewGroup) findViewById(R.id.activity_display_message);
   layout.addView(textView);
}
```






2、按Alt + Enter（Mac上的option + return）导入缺少的类。




这里有很多事情，所以让我们来解释一下：




1、调用getIntent（）获取开始活动的意图。 无论用户在那里导航，每个活动都由Intent调用。 调用getStringExtra（）从第一个活动检索数据。




2、您以编程方式创建一个TextView并设置其大小和消息。




3、您将TextView添加到由R.id.activity_display_message标识的布局。 您将布局转换为ViewGroup，因为它是所有布局的超类，并且包含addView（）方法。




注意：以前版本的Android Studio生成的XML布局可能不包括android：id属性。 如果布局没有android：id属性，调用findViewById（）将失败。 如果是这种情况，打开activity_display_message.xml并将属性android：id =“@ + id / activity_display_message”添加到layout元素中。




您现在可以运行该app。 打开时，在文本字段中键入消息，然后单击发送。 第二个活动将在屏幕上替换第一个活动，显示您在第一个活动中输入的消息。









