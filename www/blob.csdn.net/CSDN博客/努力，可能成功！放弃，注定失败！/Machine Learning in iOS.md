# Machine Learning in iOS - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年06月22日 13:45:30[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：554
[本文来自简书，原文地址:http://www.jianshu.com/p/981774d46d93](http://www.jianshu.com/p/981774d46d93)
`人工智能`今年到底有多火？谁都不知道，但一定是炙手可热。自从`AlphaGo`打败世界最顶级的围棋选手后，这股旋风就一直没有停过。各家厂商也都推出了自己的平台上的支持，如
```
Google
 I/O 2017
```
上宣布的`Mobile First To AI First`，`Tensorflow`和
```
AI
 in Android
```
等一系列重拳。这样一场最尖端科技的对垒，怎少得了`Apple`，这不
```
WWDC
 2017
```
，`iOS 11`正式推出了
```
Core
 ML
```
。
由于个人水平有限，本文将会浅谈一下以下几个议题：
- 人工智能和机器学习是什么
- iOS上如何玩机器学习
### 人工智能和机器学习
其实提到人工智能，很多人都会联想到电影里那些强悍的未来机器人，因此会觉得这是一项非常前沿的、崭新的科技。而事实上，人工智能的概念在1956年就已经提出，但由于当时计算能力、存储能力的限制，一直都没有太多太大的发展。
而机器学习，从某种意义上来说，就是一种实现人工智能的方法。与我们传统的为解决特定任务而进行的硬编码的软件程序不同，机器学习是通过使用大量的数据进行`训练`，通过各种算法从数据中学习，来达到完成目的任务的。
简单而言，就是：
`数据->算法->模型； 需要判断的数据->模型->给出预测`
即，使用大量的数据，通过一定的算法来生成一项数据模型；一旦数据模型生成成功后，就可以利用它来对需要判断的数据给出相应的预测了。
### iOS下的机器学习
以上我们简单了解了一下机器学习的一个主要的过程。而事实上，通过特定算法，使用大量数据来进行模型生成的这个过程，是非常地消耗计算和存储资源的；特别是现在的一些特大规模的神经网络，一般单个的普通机器根本扛不住，甚至连跑都跑不起来。
因此在这样一种情况下，根本不能奢侈`训练`这样一个过程能够在iOS设备上进行，更多的情况我们都是放在后端的HBase、Cassandra这种数据库中，然后再通过诸如Caffe,
 Scikit-Learn, Tensorflow这些来进行相应的训练，最后生成我们想要的模型。拿到模型后，对于数据的判断，移动端的设备似乎就能够做一些力所能及之事了。
而对于iOS 11 的Core ML，正是这样一个存在。它提出了一个模型构造的标准，让移动端开发者不用过于关心模型内部的实现细节，而只关注其输入和输出的参数，就达到了使用机器学习的目的。事实上个人在模型使用的过程中，它更像是一个函数或者一个类的使用；而就是这样一些简单的函数调用，就让你的App瞬间拥有了机器学习这样一个高大上的概念，何不也试试？
下面我们就也来做个Demo玩玩吧。我这里用的OC，如果更熟悉Swift，其实WWDC上的Demo也许也能够帮你达到学习的目的。
这里我们会分几个步骤来做：
- 做一个模型
- 写好壳APP用来装模型，并提供输入和输出的地方
#### 做一个模型
会有很多人疑问，刚才都已经提到了，训练模型是个非常消耗计算和存储资源的事情，那我们怎么做模型？
庆幸的是，Apple早已想到这一点，目前已经提供了一些已经训练好的Core ML 模型供我们进行测试使用。可以访问[Apple Machine Learning](https://developer.apple.com/machine-learning/)，我们可以看到，目前提供有以下几种图像识别模型：
- Places205-GoogleNet
- ResNet50
- Inception v3
- VGG16
从介绍中，我们了解到，这些模型都是用于图像的物体识别的。有兴趣可以看看details，我这里选择了`ResNet50`作为我的Demo
 Model。
#### 写一个壳APP
我们选择了一个图像识别的模型，因此这个壳的APP需要具备拍照或者照片选取的功能。本着一切就简的原则，立马想到了`UIImagePickerController`来达到目的。
```
@interface ViewController ()<UIImagePickerControllerDelegate, UINavigationControllerDelegate>
@property (nonatomic, strong)UIImagePickerController *imagePickerController;
...
@end
@implementation ViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    UIBarButtonItem *photoBtnItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemCamera
                                                                                  target:self
                                                                                  action:@selector(takeCamera)];
    self.navigationItem.rightBarButtonItem = photoBtnItem;
    self.imagePickerController = [[UIImagePickerController alloc] init];
    self.imagePickerController.delegate = self;
    self.imagePickerController.modalTransitionStyle = UIModalTransitionStyleFlipHorizontal;
    self.imagePickerController.allowsEditing = YES;
}
...
- (void)takeCamera {
    self.imagePickerController.sourceType = UIImagePickerControllerSourceTypeCamera;
    self.imagePickerController.mediaTypes = @[(NSString *)kUTTypeImage];
    self.imagePickerController.cameraCaptureMode = UIImagePickerControllerCameraCaptureModePhoto;
    [self.navigationController presentViewController:self.imagePickerController
                                            animated:YES
                                          completion:nil];
}
```
到这里，很简单地就把`UIImagePickerController`给搞定了，那么下面来看看这个
```
Resnet50
 Core ML Model
```
。
![](http://upload-images.jianshu.io/upload_images/296122-d886b0d863ac363c.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
MachineLearningIniOS_1.jpg
从这张图中，我们主要需要关注以下几点：
- inputs
- outputs
- generated source
首先，我们从inputs 可以看到，这个模型需要输入的是一个 224 * 224 的Image 图片模型；输出则是预测归类标签等信息。这里，如果我们还看不大明白的话，可以点击
```
Resnet50(Objective-C
 generated source)
```
后面的箭头，来看看生成的OC代码。
```
//
// Resnet50.h
// 
// This file was automatically generated and should not be edited.
// 
#import <Foundation/Foundation.h>
#import <CoreML/CoreML.h>
#include <stdint.h>
NS_ASSUME_NONNULL_BEGIN
/// Model Prediction Input Type
@interface Resnet50Input : NSObject<MLFeatureProvider>
/// Input image of scene to be classified as BGR image buffer, 224 pixels wide by 224 pixels high
@property (readwrite, nonatomic) CVPixelBufferRef image;
- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithImage:(CVPixelBufferRef)image;
@end
/// Model Prediction Output Type
@interface Resnet50Output : NSObject<MLFeatureProvider>
/// Probability of each category as dictionary of strings to doubles
@property (readwrite, nonatomic) NSDictionary<NSString *, NSNumber *> * classLabelProbs;
/// Most likely image category as string value
@property (readwrite, nonatomic) NSString * classLabel;
- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithClassLabelProbs:(NSDictionary<NSString *, NSNumber *> *)classLabelProbs classLabel:(NSString *)classLabel;
@end
/// Class for model loading and prediction
@interface Resnet50 : NSObject
@property (readonly, nonatomic, nullable) MLModel * model;
- (nullable instancetype)initWithContentsOfURL:(NSURL *)url error:(NSError * _Nullable * _Nullable)error;
/// Make a prediction using the standard interface
/// @param input an instance of Resnet50Input to predict from
/// @param error If an error occurs, upon return contains an NSError object that describes the problem. If you are not interested in possible errors, pass in NULL.
/// @return the prediction as Resnet50Output
- (nullable Resnet50Output *)predictionFromFeatures:(Resnet50Input *)input error:(NSError * _Nullable * _Nullable)error;
/// Make a prediction using the convenience interface
/// @param image Input image of scene to be classified as BGR image buffer, 224 pixels wide by 224 pixels high:
/// @param error If an error occurs, upon return contains an NSError object that describes the problem. If you are not interested in possible errors, pass in NULL.
/// @return the prediction as Resnet50Output
- (nullable Resnet50Output *)predictionFromImage:(CVPixelBufferRef)image error:(NSError * _Nullable * _Nullable)error;
@end
NS_ASSUME_NONNULL_END
```
啊哈，有了这些代码，那么再来搞这个东西就很简单了。于是乎：
```
#import "ViewController.h"
#import <CoreML/CoreML.h>
#import "Resnet50.h"
@import CoreVideo;
@import MobileCoreServices;
...
#pragma mark -
#pragma mark - UIImageController Delegate
- (void)imagePickerController:(UIImagePickerController *)picker
didFinishPickingMediaWithInfo:(NSDictionary<NSString *,id> *)info {
    NSString *mediaType=[info objectForKey:UIImagePickerControllerMediaType];
    if ([mediaType isEqualToString:(NSString *)kUTTypeImage]){
        CGSize thesize = CGSizeMake(224, 224);
        UIImage *theimage = [self image:info[UIImagePickerControllerEditedImage] scaleToSize:thesize];
        self.imageView.image = theimage;
        CVPixelBufferRef imageRef = [self pixelBufferFromCGImage:theimage.CGImage];
        Resnet50 *resnet50Model = [[Resnet50 alloc] init];
        NSError *error = nil;
        Resnet50Output *output = [resnet50Model predictionFromImage:imageRef
                                                              error:&error];
        if (error == nil) {
            self.photoNameLabel.text = output.classLabel;
        } else {
            NSLog(@"Error is %@", error.localizedDescription);
        }
    }
    UIImagePickerController *imagePickerVC = picker;
    [imagePickerVC dismissViewControllerAnimated:YES completion:^{
    }];
}
```
这样基本就完成了主要代码的编写。不过，这个过程中需要注意几点的是：
- 模型的Input Image是有宽和高限制的，因此输入时，需要转换为224 * 224大小才能够正确识别
- `CVPixelBufferRef`这种图像格式的处理与UIImage,
 CGImageRef的处理需小心，容易造成内存泄漏。这里用的转换代码都来自于网络。
```
#pragma mark - Image Helpful Tools
- (CVPixelBufferRef) pixelBufferFromCGImage: (CGImageRef) image {
    NSDictionary *options = @{
                              (NSString*)kCVPixelBufferCGImageCompatibilityKey : @YES,
                              (NSString*)kCVPixelBufferCGBitmapContextCompatibilityKey : @YES,
                              };
    CVPixelBufferRef pxbuffer = NULL;
    CVReturn status = CVPixelBufferCreate(kCFAllocatorDefault, CGImageGetWidth(image),
                                          CGImageGetHeight(image), kCVPixelFormatType_32ARGB, (__bridge CFDictionaryRef) options,
                                          &pxbuffer);
    if (status!=kCVReturnSuccess) {
        NSLog(@"Operation failed");
    }
    NSParameterAssert(status == kCVReturnSuccess && pxbuffer != NULL);
    CVPixelBufferLockBaseAddress(pxbuffer, 0);
    void *pxdata = CVPixelBufferGetBaseAddress(pxbuffer);
    CGColorSpaceRef rgbColorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(pxdata, CGImageGetWidth(image),
                                                 CGImageGetHeight(image), 8, 4*CGImageGetWidth(image), rgbColorSpace,
                                                 kCGImageAlphaNoneSkipFirst);
    NSParameterAssert(context);
    CGContextConcatCTM(context, CGAffineTransformMakeRotation(0));
    CGAffineTransform flipVertical = CGAffineTransformMake( 1, 0, 0, -1, 0, CGImageGetHeight(image) );
    CGContextConcatCTM(context, flipVertical);
    CGAffineTransform flipHorizontal = CGAffineTransformMake( -1.0, 0.0, 0.0, 1.0, CGImageGetWidth(image), 0.0 );
    CGContextConcatCTM(context, flipHorizontal);
    CGContextDrawImage(context, CGRectMake(0, 0, CGImageGetWidth(image),
                                           CGImageGetHeight(image)), image);
    CGColorSpaceRelease(rgbColorSpace);
    CGContextRelease(context);
    CVPixelBufferUnlockBaseAddress(pxbuffer, 0);
    return pxbuffer;
}
- (UIImage*)image:(UIImage *)image scaleToSize:(CGSize)size{
    UIGraphicsBeginImageContext(size);
    [image drawInRect:CGRectMake(0, 0, size.width, size.height)];
    UIImage* scaledImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return scaledImage;
}
```
BTW，由于iOS 的权限升级，记得在info.plist里添加`Privacy - Camera Usage Description`的提示。不然会挂
到此，基本就结束啦！快拿你的App去拍张如哈士奇的照片，看看Label会显示出什么？
我也把代码放[Github](https://github.com/cendywang/MLPhoto)了，纯当娱乐有兴趣的同学也可以玩玩。
