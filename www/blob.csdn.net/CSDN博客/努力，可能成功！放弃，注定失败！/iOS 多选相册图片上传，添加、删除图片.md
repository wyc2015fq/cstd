# iOS 多选相册图片上传，添加、删除图片 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年12月21日 13:45:04[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：83
通知：在使用下边的方法的时候出现的问题总结，猿友们看到后请自行更正。
问题一：有人反映打开本演示的时候第一次打不开相册，必须重新启动一下才可以打开。
此问题是由于初始化ZYQAssetPickerController懒加载造成的，在使用的时候不使用懒加载创建就可以解决
问题二：本地化图片
在演示中把图片确实本地化了一次，发现没有什么用。
```
[self.imageDataArray addObject:imageData];
```
因为这句代码已经把图片转换的字节流放到内存中了，再本地化一次感觉多此一举
问题三：如果图片是在iCloud中上面的话用ZYQAssetPickerController选择图片会崩溃
如果图片不在本地而是在icloud的上面，需要把图片下载到本地然后在从本地选取。在第三方的.m里面加上下面代码可以解决，加上之后虽然可以解决但是选择完图片在网络不好的情况下得等一会才可能看到选择的图片，方法还得自己写。
```
requestOptions.networkAccessAllowed =  YES;
```
以上是出现的问题，在经过对比之后朋友推荐一个更好的第三方TZImagePickerController，可以去github上下载对比一下，感觉瞬间完爆本演示。
上传图片功能在应用程序里面很常见，单选图片，多选图片，然后还让展示出来等等，总之要求很多了，以前自己的项目中也用到了这个多选上传工能，当时写的比较着急，写的不是很好，最近闲下来就重新写了这个需求，先看图
![](https://upload-images.jianshu.io/upload_images/2007045-29ffc1eae6ed7771.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/372/format/webp)
多选图片上传.gif注意
仔细想一下的话也没那么
麻烦首先多选相册里面的图片的话，我用的是第三方ZYQAssetPickerController有兴趣的可以去github搜一下，因为以前项目中用得是这个。
引入ZYQAssetPickerController，并遵守代理
```
self.pickerController = [[ZYQAssetPickerController alloc] init];
        _pickerController.maximumNumberOfSelection = 8;
        _pickerController.assetsFilter = ZYQAssetsFilterAllAssets;
        _pickerController.showEmptyGroups=NO;
        _pickerController.delegate=self;
        _pickerController.selectionFilter = [NSPredicate predicateWithBlock:^BOOL(id evaluatedObject, NSDictionary *bindings) {
            if ([(ZYQAsset*)evaluatedObject mediaType]==ZYQAssetMediaTypeVideo) {
                NSTimeInterval duration = [(ZYQAsset*)evaluatedObject duration];
                return duration >= 5;
            } else {
                return YES;
            }
```
接下来在需要打开相册的地方本出来就可以我的里面是这样的
```
[self presentViewController:self.pickerController animated:YES completion:nil];
```
这样在相册里面就可以多选图片了，接下里就是回调了实现代理方法
```
-(void)assetPickerController:(ZYQAssetPickerController *)picker didFinishPickingAssets:(NSArray *)assets{
```
在这里处理你选中的图片
```
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        for (int i=0; i<assets.count; i++) {
            ZYQAsset *asset=assets[i];
            [asset setGetFullScreenImage:^(UIImage *result) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    //由于iphone拍照的图片太大，直接存入数组中势必会造成内存警告，严重会导致程序崩溃，所以存入沙盒中
                    //压缩图片，这个压缩的图片就是做为你传入服务器的图片
                    NSData *imageData=UIImageJPEGRepresentation(result, 0.8);
                    [self.imageDataArray addObject:imageData];
                    [self WriteToBox:imageData];
                    //添加到显示图片的数组中
                    UIImage *image = [self OriginImage:result scaleToSize:CGSizeMake(80, 80)];
                    [self.imageArray addObject:image];
                    [self.collectionView reloadData];
                });
                
            }];
        }
       
        
    });
    
    
    [self dismissViewControllerAnimated:YES completion:^{
        [self.collectionView reloadData];
    }];
```
以上是代理方法中处理回调过来的图片，我这里面有两个数组，一个是imageArray，用于存储在集合视图显示的图片，一个是imageDataArray，用于存储压缩图片的字节流，我们不可能把选中的图片直接展示到集合视图上，iphone拍的照片太大（好几兆），尺寸也太大，这样放在我们集合视图那么小的imageview的上面会出问题的，所以先剪裁图片，同时我把压缩的imageDataArray里面的原图存储在沙盒中，以后用于上传到服务器
这个的英文存储到沙盒品中的代码
```
#pragma mark --------存入沙盒------------
- (void)WriteToBox:(NSData *)imageData{
    
    _i ++;
    NSArray *filePath = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    //获取Document文件的路径
    NSString *collectPath = filePath.lastObject;
   
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:collectPath]) {
        
        [fileManager createDirectoryAtPath:collectPath withIntermediateDirectories:YES attributes:nil error:nil];
        
    }
    //    //拼接新路径
    NSString *newPath = [collectPath stringByAppendingPathComponent:[NSString stringWithFormat:@"Picture_%ld.png",_i]];
    NSLog(@"++%@",newPath);
    [imageData writeToFile:newPath atomically:YES];
}
```
这是剪裁图片，用于集合视图显示
```
#pragma mark -----改变显示图片的尺寸----------
-(UIImage*) OriginImage:(UIImage *)image scaleToSize:(CGSize)size
{
    UIGraphicsBeginImageContext(size);  //size 为CGSize类型，即你所需要的图片尺寸
    
    [image drawInRect:CGRectMake(0, 0, size.width, size.height)];
    
    UIImage* scaledImage = UIGraphicsGetImageFromCurrentImageContext();
    
    UIGraphicsEndImageContext();
    
    return scaledImage;   //返回的就是已经改变的图片
}
```
再来就是集合视图展示了，集合视图创建就不说了，主要说一下里面的“+”号状语从句：图片删除
删除代码
```
#pragma mark --------删除图片-----------
- (void)deleteImage:(UIButton *)sender{
    NSInteger index = sender.tag - 100;
//    NSLog(@"index=%ld",index);
//    NSLog(@"+++%ld",self.imageDataArray.count);
//    NSLog(@"---%ld",self.imageArray.count);
   
    //移除显示图片数组imageArray中的数据
    [self.imageArray removeObjectAtIndex:index];
    //移除沙盒数组中imageDataArray的数据
    [self.imageDataArray removeObjectAtIndex:index];
    
    NSArray *filePath = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    //获取Document文件的路径
    NSString *collectPath = filePath.lastObject;
    NSFileManager * fileManager = [NSFileManager defaultManager];
    //移除所有文件
    [fileManager removeItemAtPath:collectPath error:nil];
    //重新写入
    for (int i = 0; i < self.imageDataArray.count; i++) {
        NSData *imgData = self.imageDataArray[i];
        [self WriteToBox:imgData];
    }
    
        [self.collectionView reloadData];
    
    
}
```
再来就是里面的“+”号了，这里我用了两个CollectionViewcell，一个用来展示图片，一个用来展示“+”号图片，可能有其他更好的方法不过我没想到......
```
- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section{
    return self.imageArray.count + 1 ;
}
- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath{
    AddCollectionViewCell *cell1 = [collectionView dequeueReusableCellWithReuseIdentifier:@"identifier" forIndexPath:indexPath];
    if (self.imageArray.count == 0) {
        return cell1;
        
    }else{
    
    CollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:@"cell" forIndexPath:indexPath];
        if (indexPath.item + 1 > self.imageArray.count ) {
            return cell1;
        }else{
            cell.imageV.image = self.imageArray[indexPath.item];
            [cell.imageV addSubview:cell.deleteButotn];
            cell.deleteButotn.tag = indexPath.item + 100;
            [cell.deleteButotn addTarget:self action:@selector(deleteImage:) forControlEvents:UIControlEventTouchUpInside];
        }
        return cell;
    }
}
```
以上就是主要代码了，可能写的有缺点，与各位共勉了
演示：[https](https://link.jianshu.com?t=https://git.oschina.net/T1_mine/UploadImage.git)：
//git.oschina.net/T1_mine/UploadImage.git用TZImagePickerController简单写的演示：[https](https://link.jianshu.com?t=https://git.oschina.net/T1_mine/OtherUploadImage.git)：//git.oschina.net/T1_mine/OtherUploadImage.git
作者：你去的联盟
链接：HTTPS：//www.jianshu.com/p/a3b1d217c42e 
来源：书繁简
繁简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
