# iOS，如何模仿一个App - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年02月21日 14:27:33[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：33
## 前言
作为一个IOS程序员，闲的时候也想自己做一个app练练手，又苦于没有UI设计，也没有好的idea，所以只能先模仿别人的App了，但是也会遇到以下问题
**
①不知道怎么入手？怎么设计
**
**
②app中的素材、图标要自己找吗、没有UI妹子给你切图怎么办，
**
**
③没有数据怎么办？假数据也不会做怎么办
**
**
④编写程序中该注意些什么
**
### ** 以下就从上面的三点开始介绍**
Tips:一下均为我个人的见解，有疑问或者建议请告诉我
以我的高仿项目[半糖App](https://www.jianshu.com/p/7b57eb0c4abe)为例
**
**
### 一、不知道怎么入手？怎么设计
首先，整体浏览一下官方的[半糖App](https://link.jianshu.com?t=http://www.ibantang.com),
![](https://upload-images.jianshu.io/upload_images/1299512-d02348459fbb12d0.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/320/format/webp)
![ ![ ![IMG_0095.PNG](http://upload-images.jianshu.io/upload_images/1299512-202a765f086ca351.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)](http://upload-images.jianshu.io/upload_images/1299512-fc3a2b9ccf1b9894.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)](http://upload-images.jianshu.io/upload_images/1299512-ca997e779c4318c4.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
上面几张图片分别对应的是 首页、广场、消息中心、个人中心，，rootViewController是 UITabBarController,以首页为例
![](https://upload-images.jianshu.io/upload_images/1299512-69ab39b354c62f1c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/327/format/webp)
tmp696dc36e.png
Tips:
- 1 整体上分为 两大部分 headerView 和UICollectionView
- 2 每次点击标签 "最新" "热门"等，collectionView 都会滚动到对应标签的tableView中，
**collectionView结构上来看就是这样：**
![](https://upload-images.jianshu.io/upload_images/1299512-1ec9970356f439fe.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/762/format/webp)
tmp59d229d4.png
也就是 tableview 添加到collectionViewCell.contentView中显示出来
*headerView的结构如下*
![](https://upload-images.jianshu.io/upload_images/1299512-19dd06f7a0b5f760.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/325/format/webp)
*tmp5b876e7d.png*
这一部分组合的自由度较大，然后每个按钮都设置不同tag，然后所有按钮添加相同的方法，比如
```
- (void)clickCenterWithTag:(UIButton *)sender{
  if(sender.tag == 按钮tag值){
    }
}
```
当然也可以设置一个block回调，参数为按钮tag值，在ViewController中 给block赋值的时候，比如
```
headerView.clickCenterBlock = ^(Int tag){
  //在这里进行判断，tag等于多少，进行什么操作
}
```
还有通知、delegate等方法都可以，就不一一细说了。。
之后的页面没有太过于复杂的结构，对照首页，也可以一一将View剥离出来
**
**
### 二、app中的素材、图标要自己找吗、没有UI妹子给你切图怎么办
① app中好多按钮的图标、背景图片哪里找
**大招**：电脑上在iTunes中下载你要模仿的app，找到它的存储目录(具体方法百度。)
![](https://upload-images.jianshu.io/upload_images/1299512-7593ba963100b291.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/112/format/webp)
tmp09a816eb.png
接着，把 后缀.ipa 改为.zip ,然后把 zip文件打开
![](https://upload-images.jianshu.io/upload_images/1299512-88813da3b9ed3f1a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/384/format/webp)
tmp062cdaed.png
显示包内容后，可以看到大部分你所需要的素材图片了
② 切图标注问题(切图我就不说了，我只会截图，ps什么的都不会)
手机安装你要模仿的app，然后截屏，把图片发到电脑上，然后安装**Mark Man**工具，
![](https://upload-images.jianshu.io/upload_images/1299512-e6278fa083cfbac7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/90/format/webp)
tmp1e090874.png
，然后将图片拖到Mark Man中，
![](https://upload-images.jianshu.io/upload_images/1299512-ff66476405710e99.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700/format/webp)
tmp1be40fe1.png
然后就可以随心所欲地标注，取色等等
**
**
### 三、没有数据怎么办？该怎么去抓包
没有数据有两个方法：
① 做假数据，数据写死，这个不是很现实，因为你要模仿的app肯定有图片，这么做假数据得到猴年马月。。。
② 抓包，其实过程不是很繁琐
下面讲一下抓包的过程：
* 1 工具
![](https://upload-images.jianshu.io/upload_images/1299512-a5ea0ed9c0ba754c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/106/format/webp)
tmp4f89f5bc.png
Charles 百度上很多可以下载，试用版即可，不需要破解
**使用过程:**
①将手机和电脑连上同一个wifi，打开抓包工具
![](https://upload-images.jianshu.io/upload_images/1299512-e5025717b7666b80.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/962/format/webp)
tmp1be62615.png
②打开半糖app
![](https://upload-images.jianshu.io/upload_images/1299512-45f1dd3ca0742236.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/877/format/webp)
tmp11e3e193.png
抓到半糖app的数据，点进去如下：
![](https://upload-images.jianshu.io/upload_images/1299512-1092bf90c5678583.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/386/format/webp)
tmp6226dd3d.png
这就是半糖app的数据，如果没有你想要的，别急，你先浏览你要抓数据的页面，这时候会发现这个目录有新的数据返回，点进去可以看到
![](https://upload-images.jianshu.io/upload_images/1299512-24e906d1350288ac.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
tmp7a92a51b.png
点击 JavaScript按钮可以看到原始的json数据
![](https://upload-images.jianshu.io/upload_images/1299512-7f6120f56d33748f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/617/format/webp)
tmp7e5e4b14.png
，
将json数据 拷贝到文件中存储起来，这就是本地数据
②下面说一下 **如何解析数据**，以swift为例，OC类似参考一下。
比如我们将刚刚的json存为 test.rtf(别问我为什么是rtf，因为mac上记事本只有这么一个,当然你也可以存为txt格式等)，然后把test.rtf拖到项目中来
![](https://upload-images.jianshu.io/upload_images/1299512-c85e52b34db68ea9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/601/format/webp)
tmp4554b12a.png
上代码：
Tips: 在这里，我们首先要明白一点，字典即为对象的一种表现形式。
先把模型写好：
```
class TopicModel: NSObject {
  //先写两个属性
  var id: String?
  var title: String?
  //从json中读取数据 并给model赋值(以下为Swift代码，OC请参考)，封装成类方法更方便
  class fun readDataFromJSON() -> TopicModel{
    //创建返回的model 
    let topicModel = TopicModel()
    //读取JSON文件
    let path = NSBundle.mainBundle().pathForResource("test", ofType: "rtf")
    let nsData = NSData(contentsOfFile: path!)
     /// json整体转换为字典
     let Dict = ( try! NSJSONSerialization.JSONObjectWithData(nsData!, options:.AllowFragments) ) as! NSDictionary
  //看上面的图片中，你要抓得数据处于 四层目录下，一步一步来
  //先把 "data"从json文件中 剥离出来
    let Data = Dict.objectForKey("data") as! NSDictionary
  
  //从Data 中把 "topic"数组剥离出来,由图片可以看到 topic是一个数组 ，存了好多个相同类型的数据
    let topicData = Data.objectForKey("topic") as! NSArray
  //topic数据已经获取，我们先将第一个元素剥离出来，
  // 定义一个object对象,并转换为字典类型
  let obj = topicData[0] as! NSDictionary
    
// 现在将obj字典的值 赋给 我们定义的topicModel
  topicModel.id = obj.objectForKey("id") as? String
  topicModel.title = obj.objectForKey("title") as? String
    //end: 到现在 给model赋值就可以了，返回model即可
  return topicModel
  }
}
```
在需要用到model的地方，直接调用 Topic.readFromJSON 就会返回你要的model
**现在看来，抓包并没有我们想象中的繁琐 **
### 四、编写程序应该注意什么
Tips: 这一部分只是参照我的经验来看，如有建议或者意见请与我交流，望不吝赐教
**① 代码尽量要复用，封装控件前看看是否几个控件相似，可以放到一起，比如：**
这个是 广场中的标签栏
![](https://upload-images.jianshu.io/upload_images/1299512-6f0e6bcb53bfa569.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/320/format/webp)
tmp154d38d0.png
这个是个人中心的标签栏
![](https://upload-images.jianshu.io/upload_images/1299512-bfbd47b136aaa47b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/324/format/webp)
tmp05e7ce45.png
这个是清单详情的标签栏
![](https://upload-images.jianshu.io/upload_images/1299512-7af7cce080989029.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/641/format/webp)
tmp69f75d0d.png
三个控件有不相同的地方，比如标签中的分割线，上面的边框，字体大小，**但是**，控件中的所有逻辑都一样，这就够了，可以写一个控件，加一些参数，比如说 是否显示边框，frame大小，中间是否有分割线，这样做的话，封装一个控件就可以满足 整个app中类似的控件
**② 方法，或者称之为函数，封装也尽量放到一起，比如**
app中有很多对图片进行的操作，诸如 放大，裁剪，加边框，可以这样：
```
//将 这些方法都封装为 类方法 ，OC中也就是 "+"方法
class ImageOperationCenter: NSObejct {
    class func ScaleToFize(image: UIImage) -> UIImage {
      //对图片进行裁剪处理，并返回
    }
    class func MakeScale(image: UIImage,scale: CGFloat) -> UIImage {
      //对图片进行放大处理，并返回
    }
}
```
Tips：如上，将对图片的处理方法，都放在 `ImageOperationCenter` 这个类中，调用的时候只需要像这样 `let image = ImageOperationCenter.MakeScale(源照片，放大倍数)`即可，用着也方便
**③ 与第一点类似，很多相似的控件，Cell尽量复用**
诸如
![](https://upload-images.jianshu.io/upload_images/1299512-7af7cce080989029.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/641/format/webp)
tmp69f75d0d.png
半糖精选的cell 与 用户推荐的cell 类似，但是有不同的地方，而且用到的model也不同，这样我们就要重写两种cell吗，**不需要**，只要 "用户推荐"的cell 继承 "半糖精选" 的cell，重写里面的方法即可，省了好多功夫，所以写程序一定**要在不损失质量的前提下**怎么省事怎么来
再来看一个例子。
![](https://upload-images.jianshu.io/upload_images/1299512-6ddaca9337de3424.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/313/format/webp)
tmp63efa8a5.png
这个通常会将它作为`collectionView`处理，其实，只有8个button的情况下，将button封装为一个View即可，这样，`ViewController`中会**少了一堆代理方法和一堆初始化collectionView方法**
这个也就因人而异吧，要灵活运用哦~
**⑤ 其实看到一个页面要先把整个页面的结构确定好，不要写了一半才发现这样写好像太麻烦，或者这样写写不出来**
我在我的高仿半糖app中 首页就遇到了这样的问题，开始想着 用`UIScrollView`当做容器，里面添加`UITableView`显示内容，这样做未尝不可，但是考虑到复用的问题，可以用三个tableView完成复用，但是这样做过于麻烦，所以后来就用collectionView当做容器，将tableView 添加到collectionViewCell中显示出来，这样复用的问题就解决了
### 写在后面的话
我的开发经验也不多，上面完全是我在做项目中自己的感悟和体会，有些可能会有问题，请参考即可
作者：ManoBoo
链接：https://www.jianshu.com/p/a634b66cb180
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
