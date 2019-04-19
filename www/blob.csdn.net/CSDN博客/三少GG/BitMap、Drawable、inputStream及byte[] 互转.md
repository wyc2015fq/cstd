# BitMap、Drawable、inputStream及byte[] 互转 - 三少GG - CSDN博客
2011年12月12日 14:27:04[三少GG](https://me.csdn.net/scut1135)阅读数：3482标签：[byte																[api																[android																[resources																[iteye																[url](https://so.csdn.net/so/search/s.do?q=url&t=blog)](https://so.csdn.net/so/search/s.do?q=iteye&t=blog)](https://so.csdn.net/so/search/s.do?q=resources&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)
个人分类：[Android 常规](https://blog.csdn.net/scut1135/article/category/1057214)
好文：转载地址：
[http://www.iteye.com/topic/642128](http://www.iteye.com/topic/642128)
android在处理一写图片资源的时候，会进行一些类型的转换，现在有空整理一下：
1、Drawable → Bitmap
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)![](http://www.iteye.com/images/spinner.gif)
- publicstatic Bitmap drawableToBitmap(Drawable drawable) {  
- 
-         Bitmap bitmap = Bitmap  
-                         .createBitmap(  
-                                         drawable.getIntrinsicWidth(),  
-                                         drawable.getIntrinsicHeight(),  
-                                         drawable.getOpacity() != PixelFormat.OPAQUE ? Bitmap.Config.ARGB_8888  
-                                                         : Bitmap.Config.RGB_565);  
-         Canvas canvas = new Canvas(bitmap);  
- //canvas.setBitmap(bitmap);
-         drawable.setBounds(0, 0, drawable.getIntrinsicWidth(), drawable.getIntrinsicHeight());  
-         drawable.draw(canvas);  
- return bitmap;  
- }  
```java
public static Bitmap drawableToBitmap(Drawable drawable) {
        
        Bitmap bitmap = Bitmap
                        .createBitmap(
                                        drawable.getIntrinsicWidth(),
                                        drawable.getIntrinsicHeight(),
                                        drawable.getOpacity() != PixelFormat.OPAQUE ? Bitmap.Config.ARGB_8888
                                                        : Bitmap.Config.RGB_565);
        Canvas canvas = new Canvas(bitmap);
        //canvas.setBitmap(bitmap);
        drawable.setBounds(0, 0, drawable.getIntrinsicWidth(), drawable.getIntrinsicHeight());
        drawable.draw(canvas);
        return bitmap;
}
```
2、从资源中获取Bitmap
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)![](http://www.iteye.com/images/spinner.gif)
- Resources res=getResources();  
- 
- Bitmap bmp=BitmapFactory.decodeResource(res, R.drawable.pic);  
```java
Resources res=getResources();
Bitmap bmp=BitmapFactory.decodeResource(res, R.drawable.pic);
```
3、Bitmap → byte[]
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)![](http://www.iteye.com/images/spinner.gif)
- privatebyte[] Bitmap2Bytes(Bitmap bm){  
-     ByteArrayOutputStream baos = new ByteArrayOutputStream();    
-     bm.compress(Bitmap.CompressFormat.PNG, 100, baos);    
- return baos.toByteArray();  
-    }  
```java
private byte[] Bitmap2Bytes(Bitmap bm){
    ByteArrayOutputStream baos = new ByteArrayOutputStream();  
    bm.compress(Bitmap.CompressFormat.PNG, 100, baos);  
    return baos.toByteArray();
   }
```
4、 byte[] → Bitmap 
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)![](http://www.iteye.com/images/spinner.gif)
- private Bitmap Bytes2Bimap(byte[] b){  
- if(b.length!=0){  
- return BitmapFactory.decodeByteArray(b, 0, b.length);  
-             }  
- else {  
- returnnull;  
-             }  
-       }  
```java
private Bitmap Bytes2Bimap(byte[] b){
		    if(b.length!=0){
		    	return BitmapFactory.decodeByteArray(b, 0, b.length);
		    }
		    else {
		    	return null;
		    }
	  }
```
以上是我在实践中遇到的一些转换，以后遇到类似的就不用到处找了，希望对大家也有一点用处！
声明：ITeye文章版权属于作者，受法律保护。没有作者书面许可不得转载。 
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# [android中使用BitmapFactory的decodeStream()方法解码图片失败问题](http://www.cnblogs.com/Greenwood/archive/2011/05/27/2060299.html)
 从网络获取图片，数据为InputStream流对象，然后调用BitmapFactory的decodeStream（）方法解码获取图片。代码如下：
```
```java
private Bitmap getUrlBitmap(String url)
{
Bitmap bm;
try{
URL imageUrl=new URL(url);
HttpURLConnection conn=(HttpURLConnection)imageUrl.openConnection();
conn.connect();
InputStream is=conn.getInputStream();
//byte[] bt=getBytes(is); //注释部分换用另外一种方式解码
//bm=BitmapFactory.decodeByteArray(bt,0,bt.length);
bm=BitmapFactory.decodeStream(is); //如果采用这种解码方式在低版本的API上会出现解码问题
is.close();
conn.disconnect();
return bm;
}
catch(MalformedURLException e)
{
e.printStackTrace();
}
catch (IOException e)
{
e.printStackTrace();
}
return null;
}
```
```
结果在运行时编译器提示：          
```
DEBUG/skia(xxx):--- decoder->decode
 returnedfalse
```
已经确定从网络获取的数据流没有出现问题，而是在图片解码时出现错误。上网查找了不少资料，也没有得出确切的原因，不过有几条意见值得关注。
一种说法是在android 较低版本的api中会有不少内部的错误，我的代码运行时选择2.1API Level 7和2.2API Level 8都会出现这个问题，而选择2.3 API Level 9后能够正常解码图片。
我的另外一种做法是换用别的解码方式对图片解码，见代码中被注释的那俩行，使用decodeByteArray（）方法在低版本的API上也能够正常解码，解决了这个问题。
其中getBytes(InputStream is)是将InputStream对象转换为Byte[]的方法，具体代码如下：
```java
private byte[] getBytes(InputStream is) throws IOException {
ByteArrayOutputStream baos = new ByteArrayOutputStream();
byte[] b = new byte[1024];
int len = 0;
while ((len = is.read(b, 0, 1024)) != -1)
{
baos.write(b, 0, len);
baos.flush();
}
byte[] bytes = baos.toByteArray();
return bytes;
}
```

