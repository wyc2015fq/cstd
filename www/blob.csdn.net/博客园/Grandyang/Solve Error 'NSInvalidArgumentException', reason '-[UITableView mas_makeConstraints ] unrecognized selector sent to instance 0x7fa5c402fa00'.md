# Solve Error: 'NSInvalidArgumentException', reason: '-[UITableView mas_makeConstraints:]: unrecognized selector sent to instance 0x7fa5c402fa00' - Grandyang - 博客园







# [Solve Error: 'NSInvalidArgumentException', reason: '-[UITableView mas_makeConstraints:]: unrecognized selector sent to instance 0x7fa5c402fa00'](https://www.cnblogs.com/grandyang/p/7872696.html)







下面是iOS开发用第三方库可能出现的错误，及其解决方法：

1. **'NSInvalidArgumentException', reason: '-[UITableView mas_makeConstraints:]: unrecognized selector sent to instance 0x7fa5c402fa00'**

这个是使用Masonry库时遇到的，Masonry库是做Auto Layout的神器，封装的特别好，用几行代码就可以实现非常复杂的自动排布功能。这个错误的产生原因可能是因为直接将第三方库拖入项目中，那么还是需要设置target membership，解决方法如下：

- Choose every *.m file, go to **File Inspector**

- Go to **Target Membership**, check your app target.



2. **Terminating app due to uncaught exception 'NSInvalidArgumentException', reason: '-[UIImageView sd_setImageWithURL:placeholderImage:options:]: unrecognized selector sent to instance 0x7fd2f3f0f8c0'**

这个是使用SDWebImage库时遇到的，SDWebImage库是图片下载及缓存池管理的神器，全都封装好了，只需要给需要下载的图片的url，和占位图片就行了，这个错误产生的原因跟上面的一样，所以解决方法也一样：

- Choose every *.m file, go to **File Inspector**

- Go to **Target Membership**, check your app target.



3. **Incompatible block pointer types sending 'void (^)(NSInteger, NSInteger)' to parameter of type 'SDWebImageDownloaderProgressBlock _Nullable' (aka 'void (^)(NSInteger, NSInteger, NSURL * _Nullable __strong)')**

这个错误还是使用SDWebImage库时遇到的，原因是**SDWebImageDownloaderProgressBlock**的Signature在新版本中变了，之前是：

```
typedef void(^SDWebImageDownloaderProgressBlock)(NSInteger receivedSize, NSInteger expectedSize);
```

后来改成这样了：

```
typedef void(^SDWebImageDownloaderProgressBlock)(NSInteger receivedSize, NSInteger expectedSize, NSURL * _Nullable targetURL);
```

所以在调用的时候改一下就行了：

```
[self sd_setImageWithURL:[NSURL URLWithString:url] placeholderImage:[UIImage imageNamed:imageName] options:SDWebImageRetryFailed|SDWebImageLowPriority progress:^(NSInteger receivedSize, NSInteger expectedSize, NSURL * _Nullable targetURL) {
    // Do something here
} completed:^(UIImage *image, NSError *error, SDImageCacheType cacheType, NSURL *imageURL) {
    // Do something here
}];
```














