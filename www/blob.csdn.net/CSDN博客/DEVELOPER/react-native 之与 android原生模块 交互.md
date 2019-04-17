# react-native 之与"android原生模块"交互 - DEVELOPER - CSDN博客





2018年06月14日 23:27:42[学术袁](https://me.csdn.net/u012827205)阅读数：1454
所属专栏：[react-native](https://blog.csdn.net/column/details/23827.html)









使用`react-native`写移动端项目，怎么能够少的了与原生模块的交互。否则，混合开发，那不是白瞎了这个名字喽。

# **使用react-native 与android原生模块交互方式**

> 
1,使用回调的Callback方式实现与android原生模块交互 

  2,使用JavaScript的Promise方式实现与android原生模块交互 

  3,使用react-native的DeviceEventEmitter 方式实现与android原生模块交互
在描述三种交互方式之前，先描述一下我的交互逻辑。这里我通过从`react-native`的js代码中启用安卓原生代码，打开相册并获取其中一张照片并回调到js页面方法中并进行接收、赋值、更新页面。 

原生代码[打开相册并获取图片地址功能](https://github.com/LuckSiege/PictureSelector)我引入了github上的一个[第三方库](https://github.com/LuckSiege/PictureSelector)，在`react-native`项目的android/app/build.gradle中进行依赖处理，然后编写原生java代码逻辑调用打开相册功能，并实现`react-native`中的js与java的互相访问调用来实现交互。

## 原生模块的逻辑实现

> 
编写原生逻辑 

  暴露原模块接口向js 

  注册原生模块 

  导出、导入react-native原生模块
### step导入原生的android模块

首先导入原生的android模块项目代码到Android Studio，像这样 
![这里写图片描述](https://img-blog.csdn.net/20180615122444765?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

build成功之后，引入相册功能的gralde依赖到`android/app/build.gradle`，像这样加入 
`compile 'com.github.LuckSiege.PictureSelector:picture_library:v2.1.4'//引入相机功能第三方`

然后在`android/build.gradle`中配置`maven {url 'https://jitpack.io'}`当然这些在第三方依赖库有使用说明，我就不多描述了。说一点略微重要的，在使用第三方库时候，她们使用的依赖方式是`implementation` 而不是 `compile`这个是你的IDE高版本的时候方法，这是如果在使用引入方式是`compile`无法正常通过编译，那么你可以使用该库的**之前版本**，可到`Branch`中查看。只是为了解决`Filed to resolve:com.android.support:support-xx:xx`找不到该版本号这种问题。 
![这里写图片描述](https://img-blog.csdn.net/2018061514101782?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

若在刚导入了原生代码，build上有问题请[查看博客](https://blog.csdn.net/junhuahouse/article/details/80686730)，或许会有帮助。
### step编写原生的android模块代码

```java
/**
 * @Title:ImageCrop
 * @Package:com.imoocchapterone
 * @Description:选择剪切图片的接口
 * @Auther:YJH
 * @Email:yuannunhua@gmail.com
 * @Date:2018/6/1415:28
 */
public interface ImageSelector {

    /**
     * @param errorCallback
     * @param successCallback
     */
    void chooseImgCallback(Callback successCallback, Callback errorCallback);

    /**
     * @param promise
     */
    void chooseImgPromise(Promise promise);
}
```

```java
/**
 * @Title:ImageCropImp
 * @Package:com.imoocchapterone
 * @Description:选择剪切图片的具体类
 * @Auther:YJH
 * @Email:yuannunhua@gmail.com
 * @Date:2018/6/1415:30
 */
public class ImageSelectorimpl implements ImageSelector, ActivityEventListener {

    private Activity mActivity;
    private Promise mPromise;
    private Callback errorCallback;
    private Callback successCallback;

    private ImageSelectorimpl(Activity activity) {
        this.mActivity = activity;
    }


    /**
     * 功能：获取 ImageSelectorimpl 对象的实例
     * @param activity
     * @return
     */
    public static ImageSelectorimpl of(Activity activity) {
        return new ImageSelectorimpl(activity);
    }

    /**
     * 使用Promise回调方式
     * 功能：实现ImageCrop接口中的方法
     * @param promise
     */
    @Override
    public void chooseImgPromise(Promise promise) {
        this.mPromise = promise;
        getSelectAlbum();
    }

    /**
     * 使用Callback回调方式
     * 功能：实现ImageCrop接口中的方法
     * @param promise
     */
    @Override
    public void chooseImgCallback(Callback successCallback, Callback errorCallback) {
        this.successCallback = successCallback;
        this.errorCallback = errorCallback;
        getSelectAlbum();
    }

    public void updateActivity(Activity activity) {
        this.mActivity = activity;
    }


    /**
     * 功能：实现ActivityEventListener接口中的方法
     * @param activity
     * @param requestCode
     * @param resultCode
     * @param data
     */
    @Override
    public void onActivityResult(Activity activity, int requestCode, int resultCode, Intent data) {
        if (resultCode == RESULT_OK) {
            switch (requestCode) {
                case PictureConfig.CHOOSE_REQUEST:
                    // 图片选择结果回调
                    selectList = PictureSelector.obtainMultipleResult(data);
//                    successCallback.invoke(selectList.get(0).getPath());//Callback回调
                    mPromise.resolve(selectList.get(0).getPath());//mPromise回调
                    break;
            }
        } else {
            mPromise.reject("no img get.");//mPromise回调
//            errorCallback.invoke(CODE_ERROR_CROP, "裁剪失败");//Callback回调
        }
    }

    /**
     * 功能：实现ActivityEventListener接口中的方法
     *
     * @param intent
     */
    @Override
    public void onNewIntent(Intent intent) {
//   无操作
    }

    private int chooseMode = PictureMimeType.ofAll();
    private List<LocalMedia> selectList = new ArrayList<>();

    //打开相册功能
    public void getSelectAlbum() {
        // 进入相册 以下是例子：用不到的api可以不写
        PictureSelector.create(mActivity)
                .openGallery(chooseMode)//全部.PictureMimeType.ofAll()、图片.ofImage()、视频.ofVideo()、音频.ofAudio()
                .theme(R.style.picture_default_style)//主题样式(不设置为默认样式) 也可参考demo values/styles下 例如：R.style.picture.white.style
                .maxSelectNum(1)// 最大图片选择数量 int
                .minSelectNum(1)// 最小选择数量 int
                ...
                .forResult(PictureConfig.CHOOSE_REQUEST);//结果回调onActivityResult code

    }
}
```

以上是在原生代码中去实现操作原生模块逻辑的java代码 

其中在类`ImageSelectorimpl.class`中实现了接口`ActivityEventListener`，并由此重写其中方法`onActivityResult(..)`，作为用户执行`startActivityForResult(..)`程序的回调。关键的是，在`onActivityResult(..)`中拿到原生逻辑的Activity回调，是要将结果回调到js代码逻辑页面以提供使用。由此需要有设置对方法`onActivityResult(..)`的监听。则后面会通过`addActivityEventListener(..)`方法来实现。

```java
/**
 * @Title:ImageSelectorModule
 * @Package:com.imoocchapterone
 * @Description:实现对react-native的接口暴露
 * @Auther:YJH
 * @Email:yuannunhua@gmail.com
 * @Date:2018/6/1415:59
 */
public class ImageSelectorModule extends ReactContextBaseJavaModule implements ImageSelector {

    private ImageSelectorimpl mImageSelectorimpl;

    public ImageSelectorModule(ReactApplicationContext reactContext) {
        super(reactContext);
    }

    @Override
    public String getName() {
        return "ImageSelector";
    }


    /**
     * 功能：向React Native暴露的接口，并执行数据交互操作
     */
    @ReactMethod
    @Override
    public void chooseImgCallback(Callback successCallback, Callback errorCallback) {
        if(null == mImageSelectorimpl){
            mImageSelectorimpl = ImageSelectorimpl.of(getCurrentActivity());
            getReactApplicationContext().addActivityEventListener(mImageSelectorimpl);
        }
        mImageSelectorimpl.chooseImgCallback(successCallback, errorCallback);
    }

    @ReactMethod
    @Override
    public void chooseImgPromise(Promise promise) {
        if(null == mImageSelectorimpl){
            mImageSelectorimpl = ImageSelectorimpl.of(getCurrentActivity());
            getReactApplicationContext().addActivityEventListener(mImageSelectorimpl);
        }

//        mImageSelectorimpl.updateActivity(getCurrentActivity());
        mImageSelectorimpl.chooseImgPromise(promise);
    }


}
```

当然对于`ImageSelectorModule.class` 和 `ImageSelectorimpl.class`其实可以合成一个类并同时继承类`ReactContextBaseJavaModule`和实现接口`ActivityEventListener`也是可以的。暂不多说。。 

在这个类中继承了一个抽象类`ReactContextBaseJavaModule`，实现了在其抽象父类`BaseJavaModule`中的关键方法`getName()`。返回一个字符串，当然这个返回值的名字，你可以随便起，但时请记住，这个字符串名字最后是要与你在js中调用的`组件NativeModules`中的一个**子组件名字**是要保持一致的。后面会分析到！ 

在这个类中继承了一个抽象类`ReactContextBaseJavaModule`，通过注解标识`@ReactMethod`实现对js暴露。 

在这个类中继承了一个抽象类`ReactContextBaseJavaModule`，看其源码
```java
public abstract class ReactContextBaseJavaModule extends BaseJavaModule {
 ...

  /**
   * Subclasses can use this method to access catalyst context passed as a constructor
   */
  protected final ReactApplicationContext getReactApplicationContext() {
    return mReactApplicationContext;
  }

  /**
   * Get the activity to which this context is currently attached, or {@code null} if not attached.
   *
   * DO NOT HOLD LONG-LIVED REFERENCES TO THE OBJECT RETURNED BY THIS METHOD, AS THIS WILL CAUSE
   * MEMORY LEAKS.
   *
   * For example, never store the value returned by this method in a member variable. Instead, call
   * this method whenever you actually need the Activity and make sure to check for {@code null}.
   */
  protected @Nullable final Activity getCurrentActivity() {
    return mReactApplicationContext.getCurrentActivity();
  }
}
```

关键的两个方法`getReactApplicationContext()`和`getCurrentActivity()`。 

方法`getCurrentActivity()`从注释上看，就是拿到一个activity来供我们操作。 

方法`getReactApplicationContext()`从注释上看，”子类可以使用此方法访问作为构造函数传递上下文”。其实就是为了**提供将原生模块注册到`react-native`组件NativeModule的队列并绑定接口`ReactPackage`所使用的**。 

看下面代码的具体实现
```java
/**
 * @Title:ImageReactPackage
 * @Package:com.imoocchapterone
 * @Description:注册、导出react-native的原生模块<格式是基本固定的>
 * @Auther:YJH
 * @Email:yuannunhua@gmail.com
 * @Date:2018/6/1416:44
 */
public class ImageReactPackage implements ReactPackage {

    @Override
    public List<NativeModule> createNativeModules(ReactApplicationContext reactContext) {
        List<NativeModule> modules = new ArrayList<>();
        modules.add(new ImageSelectorModule(reactContext));
        return modules;

    }

  ...
}
```

实现注册，且只有在实现了接口`ReactPackage`才有资格注册。

```java
public class MainApplication extends Application implements ReactApplication {

 ...
    @Override
    protected List<ReactPackage> getPackages() {
      return Arrays.<ReactPackage>asList(
          new MainReactPackage(),
          new ImageReactPackage()
      );
    }
  };

...
}
```

以上就是在android模块引入第三方类库，并实现可与`react-native`的js进行交互的基础逻辑配置。 

创建一个js类ImageSelector，导出`NativeModules.ImageSelector`备后面使用。`NativeModules`我们看到是个组件，此时`ImageSelector`也既有可能成为了组件类型。并且`ImageSelector`这个单词的形成是在原生代码模块的类`ImageSelectorModule.class`中的方法`getName()`中得到！并且`ImageSelector`能调用暴露java接口方法说明`js 类 ImageSelector`好像继承(具备)了`java 抽象类 ReactContextBaseJavaModule`的功能。即从继承`java 抽象类 ReactContextBaseJavaModule`的类作为调用的交互入口。

```java
/**
 * Created by YJH on 2018/6/14.
 */
import {NativeModules} from 'react-native';
export default NativeModules.ImageSelector;
```

当然有导出，只在我们需要使用的地方**导入**就可以使用了 
`import ImageSelector from '../../nativeJs/ImageSelector';`

## `1`使用回调的Callback方式实现与android原生模块交互

在一个组件的按钮被点击时调用方法

```
...

onPress={() => this.onSelectPicturesCallback()}

...

/**
* Callback 方式
* 功能：获取使用原生打开相册并获取图片的回调地址方式
*/
onSelectPicturesCallback() {
   ImageSelector.chooseImgCallback((result) => {
        console.log(result);
        this.setState({
             imgUrl: result
        })
   }, (error) => {
        console.log(error);
        this.setState({
            imgUrl: error
        })
  })
}

...
```

通过这种方式可以在state中成功更新的imgUrl值。

## `2`使用JavaScript的Promise方式实现与android原生模块交互

```
...

onPress={() => this.onSelectPicsFucX()}

...

/**
* Promise 方式1
* 功能：获取使用原生打开相册并获取图片的回调地址方式
*/
onSelectPicsFuc1(){
    ImageSelector.chooseImgPromise().then((result) => {
        this.setState({
            imgUrl: result
        })
    }).catch(error => {
        console.log(error);
        this.setState({
             imgUrl: error
        })
    });
}
/**
* Promise 方式2
* 功能：获取使用原生打开相册并获取图片的回调地址方式
*/
async onSelectPicsFuc2() {
    var _promise =  await ImageSelector.chooseImgPromise();
    _promise.then((result) => {
         this.setState({
             imgUrl: result
         })
    }).catch(error => {
        console.log(error);
        this.setState({
             imgUrl: error
        })
   });
}

/**
* Promise 方式3
* 功能：获取使用原生打开相册并获取图片的回调地址方式
*/
async onSelectPicsFuc3() {
     var imgPromise = await ImageSelector.chooseImgPromise();
     this.setState({
         imgUrl: imgPromise
     })
}

...
```

可以看到这里展示了使用promise获取与原生模块交互的逻辑回调。并使用了三方种方法来实现！ 

其中`方式1`和`方式3`可以获得Promise对象并在state中更新的imgUrl值。但是`方式2`是不能实现的，而且是错误并提醒报错的。因为使用了`asycn/await`方式之后，返回的是一个内容结果，而不是Promise对象。 
**注意，被桥接的原生方法的最后一个参数是一个Promise对象，那么该JS方法会返回一个Promise对象。且，不需在js中传入promise对象参数。**
## `3`使用react-native的DeviceEventEmitter 方式实现与android原生模块交互

使用以上两种方式回去与原生模式的交互回调，只能执行一次。一次启动，带动一次回调，然后结束！如果想要在原生模块向js发送多次事件。那么，就要用到**事件发射器**。她可实现，原生模块可以在没有被直接调用的情况下就可以往JavaScript发送消息事件。好的实现一下 

在js类中实现

```
...

componentDidMount() {
   //注册扫描监听
   DeviceEventEmitter.addListener('onScanningResult', this.onScanningResult);
}

onScanningResult = (eventName, params) => {

   //收到原生代码的发射通知，弹出toast提示
   this.toast.show(eventName, DURATION.LENGTH_SHORT);
};

componentWillUnmount() {
   DeviceEventEmitter.removeListener('onScanningResult', this.onScanningResult);//移除扫描监听
}
...
```

在原生模块实现

```
public interface ImageSelector {

   ...

    void sendEventMethod(String eventName, @Nullable WritableMap params);
    void sendEventMethod(ReactContext reactContext,String eventName, @Nullable WritableMap params);
}
```

```java
public class ImageSelectorimpl implements ImageSelector, ActivityEventListener {

  ...
    /**
     * 功能：发射器实现通知功能的-具体方法
     * @param reactContext
     * @param eventName
     * @param params
     */
    @Override
    public void sendEventMethod(ReactContext reactContext, String eventName, @Nullable WritableMap params) {
        reactContext.getJSModule(DeviceEventManagerModule.RCTDeviceEventEmitter.class)
                .emit(eventName, params);
    }
    @Override
    public void sendEventMethod(String eventName, @Nullable WritableMap params) {
    }
   ...
}
```

```java
public class ImageSelectorModule extends ReactContextBaseJavaModule implements ImageSelector {

    ...

    /**
     * 功能：发射器实现通知功能方法
     * @param eventName
     * @param params
     */
    @Override
    public void sendEventMethod(String eventName, @Nullable WritableMap params) {
        this.sendEventMethod(reactContext, eventName, params);
    }

    @Override
    public void sendEventMethod(ReactContext reactContext, String eventName, @Nullable WritableMap params) {
        mImageSelectorimpl.sendEventMethod(reactContext, eventName, params);
    }
}
```

只是在原先的代码中添加了写方法，来实现可多次进行发射器发送通知的功能：
`ImageSelectorModule实例.sendEventMethod(String eventName, @Nullable WritableMap params);`
参考资料： 
[https://www.hellojava.com](https://www.hellojava.com/article/13436)
[https://stackoverflow.com/questions/](https://stackoverflow.com/questions/39553274/possible-unhandled-promise-rejection-react-native-with-firebase)
[https://github.com/crazycodeboy/RNStudyNotes/blob/](https://github.com/crazycodeboy/RNStudyNotes/blob/master/React%20Native%E5%8E%9F%E7%94%9F%E7%BB%84%E4%BB%B6%E5%BC%80%E5%8F%91/RN+Android%20%E5%8E%9F%E7%94%9F%E7%BB%84%E4%BB%B6%E5%BC%80%E5%8F%91.md)
[http://www.lcode.org/react-native](http://www.lcode.org/react-native%E8%BF%9B%E9%98%B6%E4%B9%8B%E5%8E%9F%E7%94%9F%E6%A8%A1%E5%9D%97%E7%89%B9%E6%80%A7%E7%AF%87%E8%AF%A6%E8%A7%A3-%E9%80%82%E9%85%8Dandroid/)
[http://www.devio.org/2017/01/22/React-Native-Android](http://www.devio.org/2017/01/22/React-Native-Android%E5%8E%9F%E7%94%9F%E6%A8%A1%E5%9D%97%E5%BC%80%E5%8F%91%E5%AE%9E%E6%88%98-%E6%95%99%E7%A8%8B-%E5%BF%83%E5%BE%97/)











