# Unity中使用Vuforia做识别图 - weixin_33985507的博客 - CSDN博客
2016年12月20日 11:34:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：28
Vision VR/AR Summit Asia 2016大会的Keynote主题演讲中，PTC Vuforia总裁Jay Wright上台分享了Unity与PTC的合作，在Unity编辑器中内置集成Vuforia增强现实平台。
![1843097-b61495b5480fef21.png](https://upload-images.jianshu.io/upload_images/1843097-b61495b5480fef21.png)
Paste_Image.png
为了帮助开发者更好地进行AR内容开发，Vuforia创建了AR Starter Kit，[可在Asset Store下载](https://link.jianshu.com?t=https://www.assetstore.unity3d.com/en/?utm_source=unity3d&utm_medium=blog&utm_campaign=vuforia_2016-12#!/content/74050)。 该资源包拥有多个AR场景，每个场景实现不同的功能，您可以自行实验或添加到现有的Unity项目中使用。
这里有一篇Vuforia-AR-Starter-Kit的官方使用文档，想看官方说明的可以直接看这里：
[https://library.vuforia.com/articles/Training/Vuforia-AR-Starter-Kit](https://link.jianshu.com?t=https://library.vuforia.com/articles/Training/Vuforia-AR-Starter-Kit)
下载完Vuforia-AR-Starter-Kit后，我们需要到官网注册一个Vuforia账号
注册地址：[https://developer.vuforia.com/user/register](https://link.jianshu.com?t=https://developer.vuforia.com/user/register)
注册完成信息填写完成并在邮箱激活后我们就可以登录Vuforia管理后台了。
## 创建一个Vuforia案例
## 1、获取License Key
在后台管理中我们点击Develop,Add license key
![1843097-24369c5e68dd666d.png](https://upload-images.jianshu.io/upload_images/1843097-24369c5e68dd666d.png)
Paste_Image.png
这里有个选择你的项目类型，我们选择Development(开发者版本)，剩下的两个是消费者版本和企业版本功能更强大但需要收费。
![1843097-567d7726507be032.png](https://upload-images.jianshu.io/upload_images/1843097-567d7726507be032.png)
Paste_Image.png
然后设置好应用名字，设备选择Mobile而不是数字眼镜。
![1843097-687f1eee1a98007f.png](https://upload-images.jianshu.io/upload_images/1843097-687f1eee1a98007f.png)
Paste_Image.png
勾上后点击Confirm,表示同意Vuforia开发者协议
![1843097-6bdf5bbe59e8cf89.png](https://upload-images.jianshu.io/upload_images/1843097-6bdf5bbe59e8cf89.png)
Paste_Image.png
## 2、创建目标识别数据库
接下来点击Target Manager,创建自己的目标识别数据库
![1843097-4852797f3cba6191.png](https://upload-images.jianshu.io/upload_images/1843097-4852797f3cba6191.png)
Paste_Image.png
设置好名字后点击Create创建。
![1843097-c2cf61dcc871d311.png](https://upload-images.jianshu.io/upload_images/1843097-c2cf61dcc871d311.png)
Paste_Image.png
点击刚刚创建的数据库，Add Target。
![1843097-e912663278c92382.png](https://upload-images.jianshu.io/upload_images/1843097-e912663278c92382.png)
Paste_Image.png
![1843097-69d011d8ac7feec3.png](https://upload-images.jianshu.io/upload_images/1843097-69d011d8ac7feec3.png)
Paste_Image.png
目标图形的种类,我们选择单张图片,图片宽度我们可以右击查看
![1843097-6437be440310c3df.png](https://upload-images.jianshu.io/upload_images/1843097-6437be440310c3df.png)
Paste_Image.png
![1843097-b2aae55e0789c497.png](https://upload-images.jianshu.io/upload_images/1843097-b2aae55e0789c497.png)
Paste_Image.png
确定后，这样我们的一个识别目标数据库的内容就创建好了，当然你可以继续往数据库中添加想要的识别文件。
我们要尽量使用清晰，对比度高的识别图，这样才能尽可能获得高的星级，建议使用星级4以上的图片，这样才能使识别的时候成功率更高。接下来下载这个数据库，我们是准备在Unity中使用，所以选择Unity Editor。
![1843097-6fec1a3d18c184f7.png](https://upload-images.jianshu.io/upload_images/1843097-6fec1a3d18c184f7.png)
Paste_Image.png
![1843097-e6e51c371c1a3835.png](https://upload-images.jianshu.io/upload_images/1843097-e6e51c371c1a3835.png)
Paste_Image.png
## 3、创建Vuforia案例
1、导入项目
由于我们创建的是一个最基本的Vuforia项目，为免给新手造成疑惑，这里暂时不用Vuforia-AR-Starter-Kit包，我们从官网下载基本的Unity SDK并将其和上面下载的目标识别数据库导入Unity中。
[https://developer.vuforia.com/downloads/sdk](https://link.jianshu.com?t=https://developer.vuforia.com/downloads/sdk)
![1843097-de7c4ae03a893504.png](https://upload-images.jianshu.io/upload_images/1843097-de7c4ae03a893504.png)
Paste_Image.png
导入成功后如下图所示
![1843097-cc7cd74b14d3fb63.png](https://upload-images.jianshu.io/upload_images/1843097-cc7cd74b14d3fb63.png)
Paste_Image.png
2、创建项目
将Vuforia->Prefabs中的ARCamera和ImageTarget两个预制体拖入层级视图中，并将Main Camera删除。
![1843097-adaeb7bf7e3dd36e.png](https://upload-images.jianshu.io/upload_images/1843097-adaeb7bf7e3dd36e.png)
Paste_Image.png
预制体ARCamera上带有AR摄像机以及App相关设置；预制体ImageTarget代表一张识别图像，并且带有识别事件处理相关脚本。
点击ARCamera,复制我们之前创建的license key，填入第一个箭头指的位置，并将第二个箭头位置LOAD 识别库名字和Active打上√。
![1843097-1423e58e06c4c3cf.png](https://upload-images.jianshu.io/upload_images/1843097-1423e58e06c4c3cf.png)
Paste_Image.png
点击ImageTarget，在下图脚本处修改，将DataBase改成之前自己创建的，并在Image Target处选择自己创建的目标图形
![1843097-039a83a1cf1273dd.png](https://upload-images.jianshu.io/upload_images/1843097-039a83a1cf1273dd.png)
Paste_Image.png
在ImageTarget下放一个物体模型，当做识别后显示出来的物体，这里我们选择一个Cube，当然你也可以选择其他模型。然后调整Cube的transform使它处于合适位置与比例，并使ARCamera以一个合适的角度照射到此Target。
![1843097-1148f0f53cb3c4df.png](https://upload-images.jianshu.io/upload_images/1843097-1148f0f53cb3c4df.png)
Paste_Image.png
![1843097-5f589607846843be.png](https://upload-images.jianshu.io/upload_images/1843097-5f589607846843be.png)
Paste_Image.png
3、运行测试
点击运行按钮，然后将识别图放在电脑摄像头前(不要吐槽我像素渣，笔记本自带的摄像头，穷o(╯□╰)o，不过也充分说了Vuforia的识别能力是非常强的)。可以看到实验非常成功。
![1843097-d1f4f19e1b4ad13f.png](https://upload-images.jianshu.io/upload_images/1843097-d1f4f19e1b4ad13f.png)
Paste_Image.png
接下来我们发布到手机上，发布安卓的环境配置这里就不介绍了，请自行搜索。
按下图点击，找到Identification，将其中的 com.compan.ProductName的compan随便做改动，然后点Build,成功生成APK包后在手机端安装。
![1843097-8bf253f1a4840a17.png](https://upload-images.jianshu.io/upload_images/1843097-8bf253f1a4840a17.png)
Paste_Image.png
手机端实验：
![1843097-5354aa08fc89e307.png](https://upload-images.jianshu.io/upload_images/1843097-5354aa08fc89e307.png)
Paste_Image.png
也成功运行。
# 4、摄像头对焦：
由于默认情况下摄像机不能对焦，我们可以给ARCamera写一个脚本挂在上面，在Start的时候开启对焦。
void Start () {
Vuforia.CameraDevice.Instance.SetFocusMode(Vuforia.CameraDevice.FocusMode.FOCUS_MODE_CONTINUOUSAUTO
);
