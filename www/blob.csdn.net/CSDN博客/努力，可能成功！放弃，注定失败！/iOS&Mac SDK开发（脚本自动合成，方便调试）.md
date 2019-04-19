# iOS&Mac SDK开发（脚本自动合成，方便调试） - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年12月21日 13:48:53[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：74标签：[公司最近开发SDK，最开始使用原工程、SDK工程、SDK集成](https://so.csdn.net/so/search/s.do?q=公司最近开发SDK，最开始使用原工程、SDK工程、SDK集成&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
公司最近开发SDK，最开始使用原工程、SDK工程、SDK集成Demo验证三个工程分开的方式开发，每次都要在原工程开发实现效果后，将代码拖到SDK工程生成Framework，再将Framework导出到SDK集成Demo进行验证，做了很多不必要的机械性工作。本篇通过WorkSpace+SDK自动生成脚本相结合实现代码效果与SDK制作实现同步操作。
Demo地址[点这里](https://github.com/jingyiqiujing/iOS-SDK-Develope-AutoLink-EasyDebug)
# 1. 工作区（WorkSpace）结构
![](https://upload-images.jianshu.io/upload_images/8536890-a48e4bf7ae563eeb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/460/format/webp)
SDK之WorkSpace工程结构
## 1.1 创建工作区
新建一个目录DevSDKWorkSpace,创建WorkSpace，选择刚才创建的目录。
![](https://upload-images.jianshu.io/upload_images/8536890-36b64eae9324c15a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/768/format/webp)
创建workspace
## 1.2 新建一个Project
首先创建原工程，选择刚刚创建的WorkSpace。
![](https://upload-images.jianshu.io/upload_images/8536890-5c4ab66bb1bc936f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
新建Project
按照这样，创建好SDK集成Demo。
## 1.3 创建SDK工程之Framework工程
### 1.3.1 新建Project
选择CocoaTouchFramework，选择第一步创建的WorkSpace，这样就创建好了产生.framework和.a文件的SDK工程。
![](https://upload-images.jianshu.io/upload_images/8536890-2b9d25c3c3ecd3ba.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/730/format/webp)
SDK之SDK创建
### 1.3.2 修改配置
创建好SDK中Framework工程后，修改其配置：
在Other Linker Flags 下添加一个-fembed-bitcode，增加Bitcode支持
![](https://upload-images.jianshu.io/upload_images/8536890-9b448d72542d0806.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/691/format/webp)
SDK之Framework配置
### 1.3.3 新建一个Aggregate
![](https://upload-images.jianshu.io/upload_images/8536890-2f70eeb856b94cdd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
新建一个Aggregate
选中刚刚创建的Aggregare,然后选中右侧的Build Phases,点击左边的+号，选择New Run Script Phases
![](https://upload-images.jianshu.io/upload_images/8536890-a8af7f6bb0af0b17.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/877/format/webp)
SDK之Aggregate嵌入脚本
### 1.3.4 将下面的脚本复制进去
```
if [ "${ACTION}" = "build" ]
then
#要build的target名
target_Name=${PROJECT_NAME}
echo "target_Name=${target_Name}"
#build之后的文件夹路径
build_DIR=${SRCROOT}/build
echo "build_DIR=${build_DIR}"
#真机build生成的.framework文件路径
DEVICE_DIR_A=${build_DIR}/Release-iphoneos/${PROJECT_NAME}.framework
echo "DEVICE_DIR_A=${DEVICE_DIR_A}"
#模拟器build生成的.framework文件路径
SIMULATOR_DIR_A=${build_DIR}/Release-iphonesimulator/${PROJECT_NAME}.framework
echo "SIMULATOR_DIR_A=${SIMULATOR_DIR_A}"
#目标文件夹路径
INSTALL_DIR=${SRCROOT}/Products/${PROJECT_NAME}
echo "INSTALL_DIR=${INSTALL_DIR}"
#目标.framework路径
INSTALL_DIR_A=${SRCROOT}/Products/${PROJECT_NAME}/${PROJECT_NAME}.framework
echo "INSTALL_DIR_A=${INSTALL_DIR_A}"
#判断build文件夹是否存在，存在则删除
if [ -d "${build_DIR}" ]
then
rm -rf "${build_DIR}"
fi
#判断目标文件夹是否存在，存在则删除该文件夹
if [ -d "${INSTALL_DIR}" ]
then
rm -rf "${INSTALL_DIR}"
fi
#创建目标文件夹  -p会创建中间目录
mkdir -p "${INSTALL_DIR}"
#build之前clean一下
xcodebuild -target ${target_Name} clean
#模拟器build
xcodebuild -target ${target_Name} -configuration Release -sdk iphonesimulator
#真机build
xcodebuild -target ${target_Name} -configuration Release -sdk iphoneos
cp -R "${DEVICE_DIR_A}" "${INSTALL_DIR_A}"
#合成模拟器和真机.framework包
lipo -create "${DEVICE_DIR_A}/${PROJECT_NAME}" "${SIMULATOR_DIR_A}/${PROJECT_NAME}" -output "${INSTALL_DIR_A}/${PROJECT_NAME}"
#打开目标文件夹
open "${INSTALL_DIR}"
fi
```
![](https://upload-images.jianshu.io/upload_images/8536890-d9ee6dc830155b04.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/878/format/webp)
SDK之脚本嵌入
### 1.3.5 直接编译
Command+B直接编译，产生.framework。
![](https://upload-images.jianshu.io/upload_images/8536890-3f3e7b92e442f6d1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
SDK之Aggregate编译
### 1.3.6 添加新文件
添加新文件，将需要暴露的文件进行配置。
![](https://upload-images.jianshu.io/upload_images/8536890-c89d8ee373d305b5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
SDK之Framework暴露文件配置
## 1.4 创建SDK工程之.a工程
### 1.4.1 创建.a工程及配置
基本步骤与FrameWork工程一致，第一步要选择CocoaTouchStaticLibrary。
然后，直接跳到第四步，将如下脚本复制进去。
```
if [ "${ACTION}" = "build" ]
then
#要build的target名
target_Name=${PROJECT_NAME}
echo "target_Name=${target_Name}"
#build之后的文件夹路径
build_DIR=${SRCROOT}/build
echo "build_DIR=${build_DIR}"
#真机build生成的头文件的文件夹路径
DEVICE_DIR_INCLUDE=${build_DIR}/Release-iphoneos/include/${PROJECT_NAME}
echo "DEVICE_DIR_INCLUDE=${DEVICE_DIR_INCLUDE}"
#真机build生成的.a文件路径
DEVICE_DIR_A=${build_DIR}/Release-iphoneos/lib${PROJECT_NAME}.a
echo "DEVICE_DIR_A=${DEVICE_DIR_A}"
#模拟器build生成的.a文件路径
SIMULATOR_DIR_A=${build_DIR}/Release-iphonesimulator/lib${PROJECT_NAME}.a
echo "SIMULATOR_DIR_A=${SIMULATOR_DIR_A}"
#目标文件夹路径
INSTALL_DIR=${SRCROOT}/Products/${PROJECT_NAME}
echo "INSTALL_DIR=${INSTALL_DIR}"
#目标头文件文件夹路径
INSTALL_DIR_Headers=${SRCROOT}/Products/${PROJECT_NAME}/Headers
echo "INSTALL_DIR_Headers=${INSTALL_DIR_Headers}"
#目标.a路径
INSTALL_DIR_A=${SRCROOT}/Products/${PROJECT_NAME}/lib${PROJECT_NAME}.a
echo "INSTALL_DIR_A=${INSTALL_DIR_A}"
#判断build文件夹是否存在，存在则删除
if [ -d "${build_DIR}" ]
then
rm -rf "${build_DIR}"
fi
#判断目标文件夹是否存在，存在则删除该文件夹
if [ -d "${INSTALL_DIR}" ]
then
rm -rf "${INSTALL_DIR}"
fi
#创建目标文件夹
mkdir -p "${INSTALL_DIR}"
#build之前clean一下
xcodebuild -target ${target_Name} clean
#模拟器build
xcodebuild -target ${target_Name} -configuration Release -sdk iphonesimulator
#真机build
xcodebuild -target ${target_Name} -configuration Release -sdk iphoneos
#复制头文件到目标文件夹
cp -R "${DEVICE_DIR_INCLUDE}" "${INSTALL_DIR_Headers}"
#合成模拟器和真机.a包
lipo -create "${DEVICE_DIR_A}" "${SIMULATOR_DIR_A}" -output "${INSTALL_DIR_A}"
#打开目标文件夹
open "${INSTALL_DIR}"
fi
```
### 1.4.2 编译
Command+B直接编译，产生.a及暴露的头文件（Headers文件夹中）。
![](https://upload-images.jianshu.io/upload_images/8536890-b78f1eaa81a0a3a0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
SDK之Aggregate编译.a
### 1.4.3 添加新文件
添加新文件，将需要暴露的文件进行配置。
![](https://upload-images.jianshu.io/upload_images/8536890-e62ab36c841a524d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
SDK之静态.a生成所有要暴露的h都添加在这里
## 1.5 目录结构
目录的最终结构如图所示：
> 
ADreamClusive为原工程
ADreamClusiveSDK和ADreamClusiveStasticSDK为(分别产生Framework和.a)SDK工程
ADSDKDemo为SDK集成Demo。
![](https://upload-images.jianshu.io/upload_images/8536890-e4d86100abfeddb1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/464/format/webp)
SDK之目录
# 2. 将1.3和1.4产生的SDK与SDK集成Demo进行关联
将1.3产生的.framework拖到ADSDKDemo工程中
使用SDK之.framework运行ADSDKDemo。
![](https://upload-images.jianshu.io/upload_images/8536890-29820e9f059b405c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
SDK之Framework调用
修改SDK中HomeViewController内容，再次编译运行ADSDKDemo查看效果😊！！！
集成.a的方式与.framework类型，不再赘述。
# 3. 创建使用Bundle资源包
我们的项目中难免会用到图片资源和xib，storyboatd资源，我们可以将这些资源全部归类到bundle文件中，便于管理。
## 3.1 创建Bundle
![](https://upload-images.jianshu.io/upload_images/8536890-1d31edd4608f778f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
SDK之Bundle创建
## 3.2 配置
- 删除安装目录Bundle文件不需要安装
- 设置COMBINE_HIDPI_IMAGES参数为NO,不然图片会被打包成.tiff后缀
- 设置bundle包与framework的关联
![](https://upload-images.jianshu.io/upload_images/8536890-c308edde3a26e3a0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/882/format/webp)
SDK之bundle配置
> 
这样bundle的修改就能实时反映出来
## 3.3 生成SDK及使用
编译运行产生bundle文件，找到bundle文件，并将bundle包拖到测试demo的目录下面（选择Create folder reference）。
![](https://upload-images.jianshu.io/upload_images/8536890-43c2f20c7489c8e3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
SDK之Bundle生成使用
## 3.4 使用bundle中的资源
```
NSString *bundlePath = [[NSBundle mainBundle] pathForResource:@"ADreamClusiveBundle" ofType:@"bundle"];
NSBundle *resourceBundle = [NSBundle bundleWithPath:bundlePath];
// VC的nib资源
UIViewController *vc = [[UIViewController alloc] initWithNibName:@"vc_name" bundle:resourceBundle];
// 图片资源
UIImageView *imgView2 = [[UIImageView alloc] initWithFrame:CGRectMake(120, 50, 100, 50)];
imgView2.image = [UIImage imageNamed:@"buynew" inBundle:resourceBundle compatibleWithTraitCollection:nil];
[self.view addSubview:imgView2];
UIImageView *imgView3 = [[UIImageView alloc] initWithFrame:CGRectMake(100, 100, 100, 100)];
imgView3.image = [UIImage imageNamed:@"demo"];
[self.view addSubview:imgView3];
```
## 3.5 此外还可以直接创建一个bundle
省去很多配置，也可以达到上述效果
![](https://upload-images.jianshu.io/upload_images/8536890-9fac61530819b2eb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/730/format/webp)
SDK之bundle直接方式
# 参考文献
- [【iOS开发】在Xcode中做一个.a包合成脚本](https://www.jianshu.com/p/69a9b6d9875e)
- [iOS SDK~ 打包framework、开发与调试](https://blog.csdn.net/shihuboke/article/details/78450955)
- [iOS 项目中SDK的开发（framework形式）](https://www.jianshu.com/p/72381d79891f)
作者：ADreamClusive
链接：https://www.jianshu.com/p/c8a05bfe5700
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
