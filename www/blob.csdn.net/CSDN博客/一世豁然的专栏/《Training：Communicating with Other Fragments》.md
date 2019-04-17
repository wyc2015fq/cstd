# 《Training：Communicating with Other Fragments》 - 一世豁然的专栏 - CSDN博客





2017年04月21日 14:00:45[一世豁然](https://me.csdn.net/Explorer_day)阅读数：248标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/Explorer_day/article/category/6289318)









注：本人非专业翻译人员，编写此博客只是为了自学使用，如有疑问，请阅读官方文档。




为了重用Fragment UI组件，您应该将每个构建为完全独立的模块化组件，以定义其自身的布局和行为。 一旦定义了这些可重用的碎片，就可以将它们与Activity进行关联，并将它们与应用程序逻辑连接起来，以实现整体复合UI。





通常，您将需要一个碎片与另一个碎片通信，例如，根据用户事件更改内容。 所有碎片到碎片的通信都是通过相关的Activity完成的。 两个碎片不应该直接通信。





一、定义界面

要允许碎片与其活动通信，您可以在Fragment类中定义一个接口，并在Activity中实现。 碎片在其onAttach（）生命周期方法中捕获接口实现，然后可以调用接口方法以与Activity进行通信。





以下是碎片到活动通信的示例：




```java
public class HeadlinesFragment extends ListFragment {
    OnHeadlineSelectedListener mCallback;

    // Container Activity must implement this interface
    public interface OnHeadlineSelectedListener {
        public void onArticleSelected(int position);
    }

    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);

        // This makes sure that the container activity has implemented
        // the callback interface. If not, it throws an exception
        try {
            mCallback = (OnHeadlineSelectedListener) activity;
        } catch (ClassCastException e) {
            throw new ClassCastException(activity.toString()
                    + " must implement OnHeadlineSelectedListener");
        }
    }

    ...
}
```


现在，片段可以使用OnHeadlineSelectedListener接口的mCallback实例调用onArticleSelected（）方法（或接口中的其他方法）将消息传递给活动。




例如，当用户单击列表项时，调用片段中的以下方法。 该片段使用回调接口将事件传递给父活动。




```java
@Override
    public void onListItemClick(ListView l, View v, int position, long id) {
        // Send the event to the host activity
        mCallback.onArticleSelected(position);
    }
```






二、实现接口

为了从碎片接收事件回调，承载它的活动必须实现碎片类中定义的接口。




例如，以下活动实现了上述示例中的接口。




```java
public static class MainActivity extends Activity
        implements HeadlinesFragment.OnHeadlineSelectedListener{
    ...

    public void onArticleSelected(int position) {
        // The user selected the headline of an article from the HeadlinesFragment
        // Do something here to display that article
    }
}
```






三、向碎片传递消息

主机活动可以通过使用findFragmentById（）捕获Fragment实例来将消息传递到碎片，然后直接调用碎片的公共方法。





例如，假设上面显示的活动可能包含用于显示由上述回调方法返回的数据指定的项目的另一个碎片。 在这种情况下，活动可以将回调方法中接收到的信息传递给将显示该项的另一个碎片：




```java
public static class MainActivity extends Activity
        implements HeadlinesFragment.OnHeadlineSelectedListener{
    ...

    public void onArticleSelected(int position) {
        // The user selected the headline of an article from the HeadlinesFragment
        // Do something here to display that article

        ArticleFragment articleFrag = (ArticleFragment)
                getSupportFragmentManager().findFragmentById(R.id.article_fragment);

        if (articleFrag != null) {
            // If article frag is available, we're in two-pane layout...

            // Call a method in the ArticleFragment to update its content
            articleFrag.updateArticleView(position);
        } else {
            // Otherwise, we're in the one-pane layout and must swap frags...

            // Create fragment and give it an argument for the selected article
            ArticleFragment newFragment = new ArticleFragment();
            Bundle args = new Bundle();
            args.putInt(ArticleFragment.ARG_POSITION, position);
            newFragment.setArguments(args);

            FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();

            // Replace whatever is in the fragment_container view with this fragment,
            // and add the transaction to the back stack so the user can navigate back
            transaction.replace(R.id.fragment_container, newFragment);
            transaction.addToBackStack(null);

            // Commit the transaction
            transaction.commit();
        }
    }
}
```





