# 二维码之zxing仿新浪微博二维码 - z69183787的专栏 - CSDN博客
2017年12月05日 18:27:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：245
个人分类：[条形/二维码-Zxing](https://blog.csdn.net/z69183787/article/category/5711423)
[http://blog.csdn.net/xyz_fly/article/details/8053815](http://blog.csdn.net/xyz_fly/article/details/8053815)
在前言中最后部分，提到了二维码开发工具资源ZXing。网上有它最新1.7版的源码，感兴趣的可以下载下来看看，要打包生成core比较麻烦，网上有相关教程。嫌麻烦的朋友，可以去我的资源里下载Java版的core.jar，地址前言最后已经给出。
今天开始介绍利用android生成普通二维码，以及仿新浪微博二维码。话说新浪微博也是采用了ZXing的技术，而腾讯微信，我推测它好像是通过服务器生成后下载下来的。因为每次生成二维码，如果没有网络的情况下就无法得到。
补一句：因为都是java开发语句，所以开发j2me和j2se的也可以参考，创建原理是一样的，只是在最后对生成图片的处理略有不同。
首先，在生成二维码前要设置一些配置参数，也就是要告诉系统你要生成什么样的二维码。关于二维码参数的介绍不是本篇重点，我会放在后面的文章中做系统介绍。
ZXing采用Hashtable方式来保存设置参数，比如我们这里设置的纠错能力为H级别，设置编码类型为UTF-8：
[java][view
 plain](http://blog.csdn.net/xyz_fly/article/details/8053815#)[copy](http://blog.csdn.net/xyz_fly/article/details/8053815#)
- // 用于设置QR二维码参数
- Hashtable<EncodeHintType, Object> qrParam = new Hashtable<EncodeHintType, Object>();  
- // 设置QR二维码的纠错级别——这里选择最高H级别
- qrParam.put(EncodeHintType.ERROR_CORRECTION, ErrorCorrectionLevel.H);  
- // 设置编码方式
- qrParam.put(EncodeHintType.CHARACTER_SET, "UTF-8");  
接下来，我们还要告诉系统二维码扫描后的内容，以及采用的编码形式，生成图片的大小：
[java][view
 plain](http://blog.csdn.net/xyz_fly/article/details/8053815#)[copy](http://blog.csdn.net/xyz_fly/article/details/8053815#)
- // 参数顺序分别为：编码内容，编码类型，生成图片宽度，生成图片高度，设置参数
- BitMatrix bitMatrix = new MultiFormatWriter().encode(content,  
-                     BarcodeFormat.QR_CODE, QRCODE_SIZE, QRCODE_SIZE, qrParam);  
最后，我们就会得到生成QR二维码数据。但这里只是得到一个由true和false组成的数组，然后你就可以根据此生成图片。下面给出的是android采用Bitmap方式生成的黑白图片：
[java][view
 plain](http://blog.csdn.net/xyz_fly/article/details/8053815#)[copy](http://blog.csdn.net/xyz_fly/article/details/8053815#)
- // 开始利用二维码数据创建Bitmap图片，分别设为黑白两色
- int w = bitMatrix.getWidth();  
- int h = bitMatrix.getHeight();  
- int[] data = newint[w * h];  
- 
- for (int y = 0; y < h; y++) {  
- for (int x = 0; x < w; x++) {  
- if (bitMatrix.get(x, y))  
-             data[y * w + x] = 0xff000000;// 黑色
- else
-             data[y * w + x] = -1;// -1 相当于0xffffffff 白色
-     }  
- }  
- 
- // 创建一张bitmap图片，采用最高的图片效果ARGB_8888
- Bitmap bitmap = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);  
- // 将上面的二维码颜色数组传入，生成图片颜色
- bitmap.setPixels(data, 0, w, 0, 0, w, h);  
以下是Android版完整生成二维码的代码：
[java][view
 plain](http://blog.csdn.net/xyz_fly/article/details/8053815#)[copy](http://blog.csdn.net/xyz_fly/article/details/8053815#)
- /**
-  * 创建QR二维码图片
-  */
- private Bitmap createQRCodeBitmap() {  
- // 用于设置QR二维码参数
-     Hashtable<EncodeHintType, Object> qrParam = new Hashtable<EncodeHintType, Object>();  
- // 设置QR二维码的纠错级别——这里选择最高H级别
-     qrParam.put(EncodeHintType.ERROR_CORRECTION, ErrorCorrectionLevel.H);  
- // 设置编码方式
-     qrParam.put(EncodeHintType.CHARACTER_SET, "UTF-8");  
- 
- // 设定二维码里面的内容，这里我采用我微博的地址
-     String content = "sinaweibo://userinfo?uid=2568190010";  
- 
- // 生成QR二维码数据——这里只是得到一个由true和false组成的数组
- // 参数顺序分别为：编码内容，编码类型，生成图片宽度，生成图片高度，设置参数
- try {  
-         BitMatrix bitMatrix = new MultiFormatWriter().encode(content,  
-                 BarcodeFormat.QR_CODE, QRCODE_SIZE, QRCODE_SIZE, qrParam);  
- 
- // 开始利用二维码数据创建Bitmap图片，分别设为黑白两色
- int w = bitMatrix.getWidth();  
- int h = bitMatrix.getHeight();  
- int[] data = newint[w * h];  
- 
- for (int y = 0; y < h; y++) {  
- for (int x = 0; x < w; x++) {  
- if (bitMatrix.get(x, y))  
-                     data[y * w + x] = 0xff000000;// 黑色
- else
-                     data[y * w + x] = -1;// -1 相当于0xffffffff 白色
-             }  
-         }  
- 
- // 创建一张bitmap图片，采用最高的效果显示
-         Bitmap bitmap = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);  
- // 将上面的二维码颜色数组传入，生成图片颜色
-         bitmap.setPixels(data, 0, w, 0, 0, w, h);  
- return bitmap;  
-     } catch (WriterException e) {  
-         e.printStackTrace();  
-     }  
- returnnull;  
- }  
至此，你已经可以创建出二维码了。接下来，讲一下新浪微博二维码方式。
在前言中，已经对新浪微博二维码做了分析，它只是合理的利用了二维码的规则，在二维码基础上增加了一些装饰，也就是说，在生成好的二维码上，贴了张头像照。如果原理大家明白了的话，自己也就可以实现了。我这里附上在二维码图片基础上增加头像的方法：
[java][view
 plain](http://blog.csdn.net/xyz_fly/article/details/8053815#)[copy](http://blog.csdn.net/xyz_fly/article/details/8053815#)
- /**
-  * 在二维码上绘制头像
-  */
- privatevoid createQRCodeBitmapWithPortrait(Bitmap qr, Bitmap portrait) {  
- // 头像图片的大小
- int portrait_W = portrait.getWidth();  
- int portrait_H = portrait.getHeight();  
- 
- // 设置头像要显示的位置，即居中显示
- int left = (QRCODE_SIZE - portrait_W) / 2;  
- int top = (QRCODE_SIZE - portrait_H) / 2;  
- int right = left + portrait_W;  
- int bottom = top + portrait_H;  
-     Rect rect1 = new Rect(left, top, right, bottom);  
- 
- // 取得qr二维码图片上的画笔，即要在二维码图片上绘制我们的头像
-     Canvas canvas = new Canvas(qr);  
- 
- // 设置我们要绘制的范围大小，也就是头像的大小范围
-     Rect rect2 = new Rect(0, 0, portrait_W, portrait_H);  
- // 开始绘制
-     canvas.drawBitmap(portrait, rect2, rect1, null);  
- }  
这里有几点要注意：
1、如果要采用在二维码中添加头像，那么生成的二维码最好采用最高等级H级别的纠错能力，目的有两个：一是增加二维码的正确识别能力；二是扩大二维码数据内容的大小。
2、头像大小最好不要超过二维码本身大小的1/5，而且只能放在正中间部位。这是由于二维码本身结构造成的。所以说新浪微博的二维码只是合理的利用了规则而已。
3、如果要仿照腾讯微信，在二维码边上增加装饰框，记得一定要在装饰框和二维码之间留出白边，这是为了二维码可识别。
最后附上截图比对一下，截图上为正常二维码，下面为增加头像的二维码。与新浪微博生成二维码的对比一下，基本一样。
自己生成的二维码：
![](https://img-my.csdn.net/uploads/201210/09/1349788052_8067.png)
新浪微博二维码：
![](https://img-my.csdn.net/uploads/201210/08/1349672184_9107.jpg)
附上完整代码工程：[仿新浪微博二维码](http://download.csdn.net/detail/xyz_fly/4630063)
