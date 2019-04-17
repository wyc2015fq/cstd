# 测试 之Java单元测试、Android单元测试 - DEVELOPER - CSDN博客





2018年06月23日 01:15:14[学术袁](https://me.csdn.net/u012827205)阅读数：387








![这里写图片描述](https://img-blog.csdn.net/20180622232112125?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**我的目的，旨在介绍一个不一样的“单元测试” ！**

其实对于单元测试这一块，我很早已经开始关注了，也搜罗了好多这方面技术的博客。要么只有文字性的，要么都是代码的逻辑类，笼统、没有什么脉络可循。之后依然半解，放到项目中，用起来还是不方便，就是觉得这样比我直接运行在模拟器（真机）之后的过程打印、调试要慢好多！ 

因此，就导致后来的放弃。以及今天的再次拾起，并做一个系统点的介绍，希望特别想要使用单元测试的朋友能够用得着。
# **不一样 、不一样的单元测试**

![这里写图片描述](https://img-blog.csdn.net/20180622235721263?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上面，我截取了一个项目中module的目录结构图。看完之后，映入眼帘的是**红色框中那`两个括弧`**中的内容
> 
androidTest 

  test


她们俩是做什么的？我们就只从JUit出发，进行详细无死角的介绍

## **JUnit 测试**

一般而言，使用`Android Studio`做安卓开发的我们对项目做测试，无论项目搭建的准备工作或是项目开发进行中的功能性测试。都无非用到两种环境下的测试：
|test|androidTest|
|----|----|
|Java环境下的测试|Android环境下的测试|

### 区别

#### 环境配置上的区别

module下的build.gradle文件中那行行代码 

1，支持能够Java虚拟机设备环境下的默认**环境配置**；

```java
/** build.gradle/dependencies{}中 **/
testCompile 'junit:junit:4.12'
```

2，支持能够Android设备环境下的默认**环境配置**；

```java
/** build.gradle/dependencies{}中 **/
androidTestCompile('com.android.support.test.espresso:espresso-core:2.2.2', {
        exclude group: 'com.android.support', module: 'support-annotations'
    })
/** 且android/defaultConfig{}节点要加上 **/
testInstrumentationRunner "android.support.test.runner.AndroidJUnitRunner"
```

#### 代码类配置的区别

她们之间有明眼的区别，class注解上 的`@RunWith(AndroidJUnit4.class)`

即可看到，IDE在两个包内也为我们生成的测试代码类 

在Android设备上执行的范例`(androidTest)包`下 && 在Java虚拟机设备上执行的范例`(test)包`下
好的，我们该如何使用呢？这是个关键问题!接下来看两种使用方式

### test包下的Java环境下的测试

针对的是安卓项目。在基于MVP架构的基础上，使用`OkHttp`作网络数据请求，并对其做简单封装，以使用见最简单方式来实现与后台的数据交互。 
**测试目的：测试get和post两种请求方式是否成功**

构建测试方式一 在要测试的类中 `右击鼠标/Go To/Test/Create New Test..`

之后，你会发现新弹出的页面|Testing library|Class|Superclass|Destination Package|Generate|Generate test methods for|member|
|----|----|----|----|----|----|----|
|JUnit4|测试的类名|测试类的父类名|包名|勾选|不勾选|选择性勾选|

随便展示下图片效果，如图<图片很随便，内容真诚> 
![这里写图片描述](https://img-blog.csdn.net/20180624125509819?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

点击`确定`进入新的页面框，走入目录 
![这里写图片描述](https://img-blog.csdn.net/20180624211527204?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**当然是选择java虚拟机设备下的测试方式哦！**

然后测试类则生成，但是需要注意的是。你使用了这种方式并不会一定会生成你想要的效果。为什么？ 

答案很简单，使用单元测试，测试的**代码逻辑块**必须是独立的。 

拿此例来说，使用okhttp的post方法做测试是否与后台能联通？那么，就必须让测试的**代码逻辑块**与无关的类解耦。比如在测试方法中以简洁的**代码逻辑块** 实现。或者，写一个管理类能直接管理post调用的操作。 

好，我上代码表示，以简洁的**代码逻辑块** 实现post请求
```java
/**
     * 功能：使用post方式进行http请求的测试
     */
    @Test
    public void post() {
        MediaType JSON
                = MediaType.parse("application/json; charset=utf-8");

        OkHttpClient client = new OkHttpClient();
        String url = "http://httpbin.org/post";
        RequestBody body = RequestBody.create(JSON, "{\"name\":\"xueshuyuan\"}");
        Request request = new Request.Builder()
                .url(url)
                .post(body)
                .build();
        Response response = null;
        try {
            response = client.newCall(request).execute();
            System.out.println("输出get方式请求的结果：==>>" + response.body().string());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
```

当然也能使用一个类先对上面的**代码逻辑块** 封装，然后在测试中一行代码逻辑搞定。

一般的测试结构，就是生成了这样的效果。这种结构基本是固定的，所以自己完全可以在(test)包中手动创建。

```java
public class xxxTest {

    ...

    /**
     * 功能：在get()/post()方法执行之前优先执行
     * @throws Exception
     */
    @Before
    public void setUp() throws Exception {
        ..
    }


    @Test
    public void post() throws Exception {
        ..
    }
    @Test
    public void XXMethod() throws Exception {
        ..
    }
     /**
     * 功能：在加注解@Before的方法setUp()执行之后立即执行
     * @throws Exception
     */
    @after
    public void TearDown() throws Exception {
        ..
    }

}
```

其中，上标注了`@Before``@after` 方法名是固定的，并且执行顺序也是固定的。

比如对**方法post**进行测试，右击鼠标，选中执行`Run 'post()'`
![这里写图片描述](https://img-blog.csdn.net/2018062413374966?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

打印结果： 
![这里写图片描述](https://img-blog.csdn.net/20180623011427197?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如果我换一个单元测试类，这时用到了Rxjava进行了线程的调度，但依然是基于Java环境的单元测试

```java
/**
 * @Title:RxjavaTestInJava 
 * @Auther:YJH
 * @Email:yuannunhua@gmail.com
 * @Date:2018/6/23 20:13
 */
public class RxjavaTestInJava {

    @Before
    public void setUp() throws Exception {
        Thread.currentThread().setName("currentThread");
    }

    @Test
    public void schedulerTest() {
        //观察者（订阅者）
        final Subscriber<String> subscriber = new Subscriber<String>() {
            @Override
            public void onCompleted() {

                System.out.println("onCompleted=" + Thread.currentThread().getName());
            }

            @Override
            public void onError(Throwable e) {
                System.out.println("onError=" + Thread.currentThread().getName());
                e.printStackTrace();
            }

            @Override
            public void onNext(String result) {
                System.out.println("onNext=" + Thread.currentThread().getName());
                System.out.println("onNext=" + result);

            }
        };

        //被观察者
        final Observable observable = Observable.create(new Observable.OnSubscribe<Subscriber>() {

            @Override
            public void call(Subscriber subscriber1) {

                System.out.println("Observable-call=" + Thread.currentThread().getName());
                subscriber1.onStart();
                subscriber1.onNext("hello world");
                subscriber1.onCompleted();
            }
        });


        observable.subscribeOn(Schedulers.io()) //指生产事件在当前的线程中进行
                .observeOn(AndroidSchedulers.mainThread()) //指消费事件在主线程中进行
                .subscribe(subscriber);

    }

    public class User {
        private String name;


        public String getName() {
            return name;
        }

        public void setName(String name) {
            this.name = name;
        }

        @Override
        public String toString() {
            return "User{" +
                    "name='" + name + '\'' +
                    '}';
        }
    }

}
```

接下来你会看到报错、报错 
![这里写图片描述](https://img-blog.csdn.net/20180624140704626?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
但如果，我把`第56行代码`换掉，换作`.observeOn(Schedulers.newThread())`结果又会不同且正常执行。那是为什么呢？

### androidTest 包下的Android环境下的测试

原因是，在`第56行代码`使用了Android的API，已经不是能在Java环境下正常执行的了。所以为了执行该代码逻辑块，看看是否是我们想要的逻辑代码。这时就需要执行在Android环境下。即在`(androidTest)包`下新建一个同样的类逻辑，只做些许的必要修改 （改变参照两种测试环境的不同而做改变） 

1，类名上添加注解 @RunWith(AndroidJUnit4.class) 

2，增加一些Android平台的注释 Log.e 以做日志打印
```java
/**
 * @Title:RxjavaTestInJava 
 * @Auther:YJH
 * @Email:yuannunhua@gmail.com
 * @Date:2018/6/23 20:13
 */

@RunWith(AndroidJUnit4.class)
public class RxjavaTestInJava {
 @Before
    public void setUp() throws Exception {
        Thread.currentThread().setName("currentThread");
    }


    @Test
    public void schedulerTest() {
        final String tag = "test";
        //观察者（订阅者）
        final Subscriber<String> subscriber = new Subscriber<String>() {
            @Override
            public void onCompleted() {

                System.out.println("onCompleted=" + Thread.currentThread().getName());
                Log.e(tag, "onCompleted=" + Thread.currentThread().getName());
            }

            @Override
            public void onError(Throwable e) {
                System.out.println("onError=" + Thread.currentThread().getName());
                e.printStackTrace();
            }

            @Override
            public void onNext(String result) {
                System.out.println("onNext=" + Thread.currentThread().getName());
                System.out.println("onNext=" + result);
                Log.e(tag, "onNext=" + result);

            }
        };

        //被观察者
        final Observable observable = Observable.create(new Observable.OnSubscribe<Subscriber>() {

            @Override
            public void call(Subscriber subscriber1) {

                System.out.println("Observable-call=" + Thread.currentThread().getName());
                Log.e(tag, "Observable-call=" + Thread.currentThread().getName());
                subscriber1.onStart();
                subscriber1.onNext("hello world");
                Log.e(tag, "hello world");
                subscriber1.onCompleted();
            }
        });


        observable.subscribeOn(Schedulers.io()) //指生产事件在当前的线程中进行
                .observeOn(AndroidSchedulers.mainThread()) //指消费事件在主UI线程中进行
                .subscribe(subscriber);


    }


    public class User {
        private String name;


        public String getName() {
            return name;
        }

        public void setName(String name) {
            this.name = name;
        }

        @Override
        public String toString() {
            return "User{" +
                    "name='" + name + '\'' +
                    '}';
        }
    }
}
```

然后同样的执行方式，在方法`schedulerTest`上**右击鼠标**并**run ‘schedulerTest()’**执行测试！ 
![这里写图片描述](https://img-blog.csdn.net/20180624210754944?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后你会看到弹窗 
![这里写图片描述](https://img-blog.csdn.net/20180624203139952?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后你会发现该指示是让你启动一个Android模拟器设备，显然是要运行到上面了。然而结果却是app并不会到模拟器上启动，而是”后台运行”。以这种方式执行了我们的单元测试。 

欣赏下`Run`下的执行结果 
![这里写图片描述](https://img-blog.csdn.net/20180624204922982?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后欣赏下`Android Monitor`下的执行结果 
![这里写图片描述](https://img-blog.csdn.net/20180624205018752?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

说明一切都在我们的掌握之中，能够让我看到该单元测试结果，并能够确认该逻辑是否是我们想要的正确逻辑，从而起到单元测试的作用！











