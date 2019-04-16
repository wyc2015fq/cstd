# Android 组件化-模块化之路——在展示层搭建MVP结构 - ggabcda的博客 - CSDN博客





2017年08月20日 14:49:08[ggabcda](https://me.csdn.net/ggabcda)阅读数：204








## Android 组件化/模块化之路——在展示层搭建MVP结构

### 什么是MVP

**Model–View–Presenter** (**MVP**) 源于 Model–View–Controller (MVC) 的结构设计模式，它是用于**展示层（Presentation）**的结构。

MVP 中的 Presenter 是 “中间人”角色，它的功能是在 Model–View–Presenter 三者中起到连接和协作的作用。程序中的大部分的逻辑都应该在 Presenter 中实现。

![model–view–presenter](http://angrycode.qiniudn.com/Model_View_Presenter_GUI_Design_Pattern.png)
- Model 定义了数据的访问接口。例如之前文章 《App 组件化/模块化之路——Repository 模式》 就介绍了使用 Repository 实现数据访问的一种方式。

- View 定义了展示数据的接口，以及转发用户的指令 
- Presenter 是连接 Model 与 View 的桥梁，是它们的协作者。 

接下来我们就开始搭建通用的 MVP 结构了。

### Model

Model 的实现主要是根据业务，我们这里是推荐使用 Repository 来实现，可以参考文章 《App 组件化/模块化之路——Repository 模式》 。

### View

首先我们定义 View 接口，这里定义了一个请求的通用流程 
```
onStart()
```
、 
```
onFinished()
```
、 
```
onError()
```
 等方法。



```
public interface IView<T> {
    Activity getActivity();

    /**
     * 请求开始
     */
    void onStart();

    /**
     * 请求结束
     */
    void onFinished();

    /**
     * 请求出错
     * @param errorCode
     * @param message
     */
    void onError(int errorCode, String message);
}
```



### Presenter

先定义 IPresenter， 接口很简单，是 Presenter 创建和注销的过程。



```
public interface IPresenter {

    void onCreate();

    void onDestroy();
}
```



然后定义 BasePresenter，这个是 Presenter 是各个具体业务实现的基类。



```
public abstract class BasePresenter<T extends IView> implements IPresenter {

    public static final int ERROR_RX = 2000;

    public T view;

    public BasePresenter(T view) {
        this.view = view;
    }

    @Override
    public void onCreate() {

    }

    Activity getActivity() {
        return view.getActivity();
    }

    /**
     * 用于判断当前view是否已经退出
     *
     * @return
     */
    public boolean isViewDetached() {
        if (view == null) {
            return true;
        }
        if (view.getActivity() == null) {
            return true;
        }

        if (view.getActivity().isFinishing()) {
            return true;
        }
        return false;
    }

    public String getRxErrorText() {
        return view.getActivity().getString(R.string.error_network);
    }

    @Override
    public void onDestroy() {
        view = null;
    }
}
```



### Contract

使用 Contract 模式的好处就是，如果我们的业务需求很多，那么在**展示层(Presentation)**中就会出现了大量的 MVP 文件。这样对于后续维护工作是一件非常令人头痛的事情，所以使用 Contract 来规范 View 和 Presenter 文件。这样具体业务只要找到 Contract 文件，就可以知道这个业务的具体接口有哪些了。

其中 Contract 下面的 View 是定义具体业务的接口了。该类可以在 UI（例如Activity或Fragment）中实现它。



```
public interface DemoContract {

    interface View extends IView {
        void onGetDataFinished(String data);
        //other callbacks
    }

    interface Presenter extends IPresenter {
        void getData();
        //other mehtods
    }
}
```



那么在具体实现 Presenter 时，这里的 Repository 就是 Model了。使用它可以对数据进行访问和存储。



```
public class DemoPresenter extends BasePresenter<DemoContract.View> implements DemoContract.Presenter {
      DemoRepository repository;
    public DemoPresenter(DemoContract.View view) {
        super(view);
    }

    @Override
    public void getData() {
        view.onGetDataFinished("");
    }
}
```



### Client

客户端的使用基本上是在 Android View 组件。例如



```
public class DemoActivity extends AppCompatActivity implements DemoContract.View {

    DemoPresenter mPresenter;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mPresenter = new DemoPresenter(this);
        mPresenter.getData();//请求数据
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mPresenter.onDestroy();
    }

    @Override
    public Activity getActivity() {
        return this;
    }

    @Override
    public void onGetDataFinished(String data) {
        // 这里获取到数据
    }

    @Override
    public void onBegin() {
        //请求开始，可以显示loading等操作
    }

    @Override
    public void onFinished() {
        //请求结束，取消loading等操作
    }

    @Override
    public void onError(int errorCode, String message) {
        //处理出错
    }
}
```



#### 参考文献

https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93presenter

https://github.com/googlesamples/android-architecture
微信关注我们，可以获取更多
![](http://images2015.cnblogs.com/blog/427085/201702/427085-20170221211811976-670020216.png)



