# iPhone开发之UIImage应用与内存管理 - weixin_33985507的博客 - CSDN博客
2012年03月04日 20:53:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
用UIImage加载图像的方法很多，最常用的是下面两种：
　　1、用imageNamed函数
　　[UIImage imageNamed:ImageName];
　　2、用NSData的方式加载，例如：
　　1. NSString *filePath = [[NSBundle mainBundle] pathForResource:fileName ofType:extension];
　　2. NSData *image = [NSData dataWithContentsOfFile:filePath];
　　3. [UIImage imageWithData:image];
　　由于第一种方式要写的代码比较少，可能比较多人利用imageNamed的方式加载图像。其实这两种加载方式都有各自的特点。
　　1)用imageNamed的方式加载时，系统会把图像Cache到内存。如果图像比较大，或者图像比较多，用这种方式会消耗很大的内存，而且释放图像的内存是一件相对来说比较麻烦的事情。例如：如果利用imageNamed的方式加载图像到一个动态数组NSMutableArray，然后将将数组赋予一个UIView的对象的animationImages进行逐帧动画，那么这将会很有可能造成内存泄露。并且释放图像所占据的内存也不会那么简单。但是利用imageNamed加载图像也有自己的优势。
　　对于同一个图像系统只会把它Cache到内存一次，这对于图像的重复利用是非常有优势的。例如：你需要在一个TableView里重复加载同样一个图标，那么用imageNamed加载图像，系统会把那个图标Cache到内存，在Table里每次利用那个图像的时候，只会把图片指针指向同一块内存。这种情况使用imageNamed加载图像就会变得非常有效。
　　2)利用NSData方式加载时，图像会被系统以数据方式加载到程序。当你不需要重用该图像，或者你需要将图像以数据方式存储到数据库，又或者你要通过网络下载一个很大的图像时，请尽量使用imageWithData的方式加载图像。
　　无论用哪种方式加载图像，图像使用结束后，一定要记得显示释放内存。
