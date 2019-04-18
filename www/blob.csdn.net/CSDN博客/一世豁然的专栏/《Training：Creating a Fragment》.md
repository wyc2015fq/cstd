# 《Training：Creating a Fragment》 - 一世豁然的专栏 - CSDN博客





2017年04月21日 11:18:32[一世豁然](https://me.csdn.net/Explorer_day)阅读数：279标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/Explorer_day/article/category/6289318)









注：本人非专业翻译人员，编写此博客只是为了自学使用，如有疑问，请参考官方文档







您可以将片段视为活动的模块化部分，它具有自己的生命周期，接收自己的输入事件，以及在活动运行时可以添加或删除的活动（类似于“子活动”，您可以 在不同的活动中重用）。 本课程展示了如何使用支持库扩展Fragment类，以便您的应用程序与运行系统版本低至Android 1.6的设备兼容。





在开始本课之前，您必须将Android项目设置为使用支持库。如果以前没有使用支持库，通过以下的 Support Library Setup document课程中的使用v4库来建立你的的项目。但是，您还可以通过使用与Android 2.1（API级别7）兼容的v7 appcompat库（此库还包含碎片API），将应用程序栏包含在活动中。





一、创建一个碎片 类

要创建碎片，请扩展Fragment类，然后覆盖密钥生命周期方法来插入应用程序逻辑，类似于使用Activity类的方式。





创建片段时的一个区别是您必须使用onCreateView（）回调来定义布局。 事实上，这是为了使片段运行所需的唯一回调。 例如，这是一个简单的片段，它指定了自己的布局：




```java
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.ViewGroup;

public class ArticleFragment extends Fragment {
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
        Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.article_view, container, false);
    }
}
```


就像一个活动一样，作为被从活动中删除或添加，以及活动在其生命周期状态之间转换，碎片应该实现其他的生命周期回调，允许你管理它的状态。例如，当活动的onPause（）方法被调用时，活动中的任何片段都会接收到onPause（）的调用。




有关碎片生命周期和回调方法的更多信息，请参阅“片段开发人员指南”。








二、使用XML将片段添加到活动

虽然碎片是可重用的，但模块化UI组件，Fragment类的每个实例必须与父FragmentActivity相关联。 您可以通过在活动布局XML文件中定义每个片段来实现此关联。





注意：FragmentActivity是支持库中提供的特殊活动，用于处理早于API级别11的系统版本上的碎片。如果您支持的最低系统版本为11级或更高版本，则可以使用常规活动。





这是一个示例布局文件，当设备屏幕被认为是“大”（由目录名中的大限定符指定）时，会向活动添加两个片段。


res/layout-large/news_articles.xml




```
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:orientation="horizontal"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent">

    <fragment android:name="com.example.android.fragments.HeadlinesFragment"
              android:id="@+id/headlines_fragment"
              android:layout_weight="1"
              android:layout_width="0dp"
              android:layout_height="match_parent" />

    <fragment android:name="com.example.android.fragments.ArticleFragment"
              android:id="@+id/article_fragment"
              android:layout_weight="2"
              android:layout_width="0dp"
              android:layout_height="match_parent" />

</LinearLayout>
```


提示：有关为不同屏幕尺寸创建布局的详细信息，请参阅支持不同的屏幕尺寸。




然后将布局应用于您的活动：




```java
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;

public class MainActivity extends FragmentActivity {
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.news_articles);
    }
}
```


如果您使用的是v7 appcompat库，那么您的活动应该是扩展AppCompatActivity，它是FragmentActivity的一个子类。 有关更多信息，请参阅添加应用程序栏）。




注意：通过在布局XML文件中定义片段将碎片添加到活动布局时，不能在运行时删除碎片。 如果您计划在用户交互期间交换您的碎片，则必须在活动首次启动时将碎片添加到活动中，如下一课所示。




