# 常见的图像特效处理算法（基于android） - 简书


# 常见的图像特效处理算法（基于android）



### 素材





![](https://upload-images.jianshu.io/upload_images/1802307-c8703def2c952d3b.jpg)




### 圆角处理

`public static Bitmap getRoundedCornerBitmap(Bitmap bitmap, float roundPx) { Bitmap output = Bitmap.createBitmap(bitmap.getWidth(), bitmap.getHeight(), Config.ARGB_8888); Canvas canvas = new Canvas(output); final int color = 0xff424242; final Paint paint = new Paint(); final Rect rect = new Rect(0, 0, bitmap.getWidth(), bitmap.getHeight()); final RectF rectF = new RectF(rect); paint.setAntiAlias(true); canvas.drawARGB(0, 0, 0, 0); paint.setColor(color); canvas.drawRoundRect(rectF, roundPx, roundPx, paint); paint.setXfermode(new PorterDuffXfermode(Mode.SRC_IN)); canvas.drawBitmap(bitmap, rect, rect, paint); return output; }`

实际就是先画了一个圆角矩形的过滤框，于是形状有了，再将框中的内容填充为图片。对于[Mode.SRC_IN的讲解](https://link.jianshu.com?t=http://www.cnblogs.com/sank615/archive/2013/03/12/2955675.html)

输出效果：




![](https://upload-images.jianshu.io/upload_images/1802307-919eba28a2d5d2f2.jpg)

圆角


### 灰白处理

`public static Bitmap toGrayscale(Bitmap bmpOriginal) { int width, height; height = bmpOriginal.getHeight(); width = bmpOriginal.getWidth(); Bitmap bmpGrayscale = Bitmap.createBitmap(width, height, Bitmap.Config.RGB_565); Canvas c = new Canvas(bmpGrayscale); Paint paint = new Paint(); ColorMatrix cm = new ColorMatrix(); cm.setSaturation(0); ColorMatrixColorFilter f = new ColorMatrixColorFilter(cm); paint.setColorFilter(f); c.drawBitmap(bmpOriginal, 0, 0, paint); return bmpGrayscale; }`

就是利用了ColorMatrix 类自带的设置饱和度的方法setSaturation()。不过其方法内部实现的更深一层是利用颜色矩阵的乘法实现的。

输出效果：




![](https://upload-images.jianshu.io/upload_images/1802307-17d948eea5195a26.jpg)

灰白


### 黑白处理

`public static Bitmap toHeibai(Bitmap mBitmap) { int mBitmapWidth = 0; int mBitmapHeight = 0; mBitmapWidth = mBitmap.getWidth(); mBitmapHeight = mBitmap.getHeight(); Bitmap bmpReturn = Bitmap.createBitmap(mBitmapWidth, mBitmapHeight, Bitmap.Config.ARGB_8888); int iPixel = 0; for (int i = 0; i < mBitmapWidth; i++) { for (int j = 0; j < mBitmapHeight; j++) { int curr_color = mBitmap.getPixel(i, j); int avg = (Color.red(curr_color) + Color.green(curr_color) + Color .blue(curr_color)) / 3; if (avg >= 100) { iPixel = 255; } else { iPixel = 0; } int modif_color = Color.argb(255, iPixel, iPixel, iPixel); bmpReturn.setPixel(i, j, modif_color); } } return bmpReturn; }`

阙值变换。其实看图片效果就能看出来，这张图片不同于灰白处理的那张，不同之处是灰白处理虽然没有了颜色，但是黑白的程度层次依然存在，而此张图片连层次都没有了，只有两个区别十分明显的黑白颜色。实现的算法也很简单，对于每个像素的rgb值求平均数，如果高于100算白色，低于100算黑色。

输出效果：




![](https://upload-images.jianshu.io/upload_images/1802307-1bf7c5d0ebc13253.jpg)

黑白


### 镜像处理

`public static Bitmap createReflectionImageWithOrigin(Bitmap bitmap) { final int reflectionGap = 4; int width = bitmap.getWidth(); int height = bitmap.getHeight(); Matrix matrix = new Matrix(); matrix.preScale(1, -1); Bitmap reflectionImage = Bitmap.createBitmap(bitmap, 0, height / 2, width, height / 2, matrix, false); Bitmap bitmapWithReflection = Bitmap.createBitmap(width, (height + height / 2), Config.ARGB_8888); Canvas canvas = new Canvas(bitmapWithReflection); canvas.drawBitmap(bitmap, 0, 0, null); Paint deafalutPaint = new Paint(); canvas.drawRect(0, height, width, height + reflectionGap, deafalutPaint); canvas.drawBitmap(reflectionImage, 0, height + reflectionGap, null); Paint paint = new Paint(); LinearGradient shader = new LinearGradient(0, bitmap.getHeight(), 0, bitmapWithReflection.getHeight() + reflectionGap, 0x70ffffff, 0x00ffffff, TileMode.CLAMP); paint.setShader(shader); // Set the Transfer mode to be porter duff and destination in paint.setXfermode(new PorterDuffXfermode(Mode.DST_IN)); // Draw a rectangle using the paint with our linear gradient canvas.drawRect(0, height, width, bitmapWithReflection.getHeight()+ reflectionGap, paint); return bitmapWithReflection; }`

就是将原图片反转一下，调整一  下它的颜色作出倒影效果，再将两张图片续加在一起。

输出效果：




![](https://upload-images.jianshu.io/upload_images/1802307-0299b195d07049ad.jpg)

镜像


### 泛黄处理

`public static Bitmap testBitmap(Bitmap bitmap) { Bitmap output = Bitmap.createBitmap(bitmap.getWidth(),bitmap.getHeight(), Config.RGB_565); Canvas canvas = new Canvas(output); Paint paint = new Paint(); ColorMatrix cm = new ColorMatrix(); float[] array = {1,0,0,0,50, 0,1,0,0,50, 0,0,1,0,0, 0,0,0,1,0}; cm.set(array); paint.setColorFilter(new ColorMatrixColorFilter(cm)); canvas.drawBitmap(bitmap, 0, 0, paint); return output; }`

其实每张图片的存储都是存的每个像素的rgba值，而对其操作的时候又将其四个数值定位一个5行1列的矩阵，最后一行值为1，这样一来利用矩阵对其操作确实方便了好多，矩阵的乘法可以轻松的实现某个或全部分量按比例或加常熟的增加或减少。 比如现有一张图片，其每个point的rgba值为{100，100，100,255}也就是灰色全图，我们希望其红色部位增加一倍，剩余部分增加十。就可以将其值虚拟为五行一列矩阵：{100 ，100,100,255,1} 再让这个矩阵：{2,0,0,0,0换行 0,1,0,0,10换行 0,0,1,0,10换行 0,,0,0,1,10}     乘以它。得到{  200,110,100,100} 。  这个泛黄照片的处理算法原理就是让每个像素点rg值增加50，rg值相混合就得到了黄色。

输出效果：




![](https://upload-images.jianshu.io/upload_images/1802307-ff82b9e61b07da37.jpg)

泛黄


### 哈哈镜处理

`jintArray Java_com_spore_meitu_jni_ImageUtilEngine_toHahajing (JNIEnv* env,jobject thiz, jintArray buf, jint width, jint height,jint centerX, jint centerY, jint radius, jfloat multiple) { jint * cbuf; cbuf = (*env)->GetIntArrayElements(env, buf, 0); int newSize = width * height; jint rbuf[newSize]; float xishu = multiple; int real_radius = (int)(radius / xishu); int i = 0, j = 0; for (i = 0; i < width; i++) { for (j = 0; j < height; j++) { int curr_color = cbuf[j * width + i]; int pixR = red(curr_color); int pixG = green(curr_color); int pixB = blue(curr_color); int pixA = alpha(curr_color); int newR = pixR; int newG = pixG; int newB = pixB; int newA = pixA; int distance = (int) ((centerX - i) * (centerX - i) + (centerY - j) * (centerY - j)); if (distance < radius * radius) { int src_x = (int) ((float) (i - centerX) / xishu); int src_y = (int) ((float) (j - centerY) / xishu); src_x = (int)(src_x * (sqrt(distance) / real_radius)); src_y = (int)(src_y * (sqrt(distance) / real_radius)); src_x = src_x + centerX; src_y = src_y + centerY; int src_color = cbuf[src_y * width + src_x]; newR = red(src_color); newG = green(src_color); newB = blue(src_color); newA = alpha(src_color); } newR = min(255, max(0, newR)); newG = min(255, max(0, newG)); newB = min(255, max(0, newB)); newA = min(255, max(0, newA)); int modif_color = ARGB(newA, newR, newG, newB); rbuf[j * width + i] = modif_color; } } jintArray result = (*env)->NewIntArray(env, newSize); (*env)->SetIntArrayRegion(env, result, 0, newSize, rbuf); (*env)->ReleaseIntArrayElements(env, buf, cbuf, 0); return result; }`

根据哈哈镜的半径，以中心点为圆心，每个像素点的坐标位移并扩展，离中心点越近的就扩展越大。

输出效果：




![](https://upload-images.jianshu.io/upload_images/1802307-761a0d9e986efbd3.jpg)

哈哈镜


### 放大镜处理

`jintArray Java_com_spore_meitu_jni_ImageUtilEngine_toFangdajing (JNIEnv* env,jobject thiz, jintArray buf, jint width, jint height,jint centerX, jint centerY, jint radius, jfloat multiple) { jint * cbuf; cbuf = (*env)->GetIntArrayElements(env, buf, 0); int newSize = width * height; jint rbuf[newSize]; float xishu = multiple; int real_radius = (int)(radius / xishu); int i = 0, j = 0; for (i = 0; i < width; i++) { for (j = 0; j < height; j++) { int curr_color = cbuf[j * width + i]; int pixR = red(curr_color); int pixG = green(curr_color); int pixB = blue(curr_color); int pixA = alpha(curr_color); int newR = pixR; int newG = pixG; int newB = pixB; int newA = pixA; int distance = (int) ((centerX - i) * (centerX - i) + (centerY - j) * (centerY - j)); if (distance < radius * radius) { int src_x = (int)((float)(i - centerX) / xishu + centerX); int src_y = (int)((float)(j - centerY) / xishu + centerY); int src_color = cbuf[src_y * width + src_x]; newR = red(src_color); newG = green(src_color); newB = blue(src_color); newA = alpha(src_color); } newR = min(255, max(0, newR)); newG = min(255, max(0, newG)); newB = min(255, max(0, newB)); newA = min(255, max(0, newA)); int modif_color = ARGB(newA, newR, newG, newB); rbuf[j * width + i] = modif_color; } } jintArray result = (*env)->NewIntArray(env, newSize); (*env)->SetIntArrayRegion(env, result, 0, newSize, rbuf); (*env)->ReleaseIntArrayElements(env, buf, cbuf, 0); return result; }`

### 浮雕处理

`public static Bitmap toFuDiao(Bitmap mBitmap) { int mBitmapWidth = 0; int mBitmapHeight = 0; mBitmapWidth = mBitmap.getWidth(); mBitmapHeight = mBitmap.getHeight(); Bitmap bmpReturn = Bitmap.createBitmap(mBitmapWidth, mBitmapHeight, Bitmap.Config.RGB_565); int preColor = 0; int prepreColor = 0; preColor = mBitmap.getPixel(0, 0); for (int i = 0; i < mBitmapWidth; i++) { for (int j = 0; j < mBitmapHeight; j++) { int curr_color = mBitmap.getPixel(i, j); int r = Color.red(curr_color) - Color.red(prepreColor) +127; int g = Color.green(curr_color) - Color.red(prepreColor) + 127; int b = Color.green(curr_color) - Color.blue(prepreColor) + 127; int a = Color.alpha(curr_color); int modif_color = Color.argb(a, r, g, b); bmpReturn.setPixel(i, j, modif_color); prepreColor = preColor; preColor = curr_color; } } Canvas c = new Canvas(bmpReturn); Paint paint = new Paint(); ColorMatrix cm = new ColorMatrix(); cm.setSaturation(0); ColorMatrixColorFilter f = new ColorMatrixColorFilter(cm); paint.setColorFilter(f); c.drawBitmap(bmpReturn, 0, 0, paint); return bmpReturn; }`

其实浮雕的特点就是在颜色有跳变的地方就刻条痕迹。127，127,127为深灰色，近似于石头的颜色，此处取该颜色为底色。算法是将上一个点的rgba值减去当前点的rgba值然后加上127得到当前点的颜色。

输出效果：




![](https://upload-images.jianshu.io/upload_images/1802307-0851055aaafa5292.jpg)

浮雕


### 底片处理

`jintArray Java_com_spore_meitu_jni_ImageUtilEngine_toDipian (JNIEnv* env,jobject thiz, jintArray buf, jint width, jint height) { jint * cbuf; cbuf = (*env)->GetIntArrayElements(env, buf, 0); int newSize = width * height; jint rbuf[newSize]; int count = 0; int preColor = 0; int prepreColor = 0; int color = 0; preColor = cbuf[0]; int i = 0; int j = 0; int iPixel = 0; for (i = 0; i < width; i++) { for (j = 0; j < height; j++) { int curr_color = cbuf[j * width + i]; int r = 255 - red(curr_color); int g = 255 - green(curr_color); int b = 255 - blue(curr_color); int a = alpha(curr_color); int modif_color = ARGB(a, r, g, b); rbuf[j * width + i] = modif_color; } } jintArray result = (*env)->NewIntArray(env, newSize); (*env)->SetIntArrayRegion(env, result, 0, newSize, rbuf); (*env)->ReleaseIntArrayElements(env, buf, cbuf, 0); return result; }`

输出效果：





![](https://upload-images.jianshu.io/upload_images/1802307-518414fc5f00cccf.jpg)

底片


### 油画处理

`public static Bitmap toYouHua(Bitmap bmpSource) { Bitmap bmpReturn = Bitmap.createBitmap(bmpSource.getWidth(), bmpSource.getHeight(), Bitmap.Config.RGB_565); int color = 0; int Radio = 0; int width = bmpSource.getWidth(); int height = bmpSource.getHeight(); Random rnd = new Random(); int iModel = 10; int i = width - iModel; while (i > 1) { int j = height - iModel; while (j > 1) { int iPos = rnd.nextInt(100000) % iModel; color = bmpSource.getPixel(i + iPos, j + iPos); bmpReturn.setPixel(i, j, color); j = j - 1; } i = i - 1; } return bmpReturn; }`

油画因为是用画笔画的，彩笔画的时候没有那么精确会将本该这点的颜色滑到另一个点处。算法实现就是取一个一定范围内的随机数，每个点的颜色是该点减去随机数坐标后所得坐标的颜色。

输出效果：




![](https://upload-images.jianshu.io/upload_images/1802307-471c8373f63c3709.jpg)

油画


### 模糊

`public static Bitmap toMohu(Bitmap bmpSource, int Blur) { int mode = 5; Bitmap bmpReturn = Bitmap.createBitmap(bmpSource.getWidth(), bmpSource.getHeight(), Bitmap.Config.ARGB_8888); int pixels[] = new int[bmpSource.getWidth() \* bmpSource.getHeight()]; int pixelsRawSource[] = new int[bmpSource.getWidth()\* bmpSource.getHeight() \* 3]; int pixelsRawNew[] = new int[bmpSource.getWidth()\* bmpSource.getHeight() \* 3]; bmpSource.getPixels(pixels, 0, bmpSource.getWidth(), 0, 0, bmpSource.getWidth(), bmpSource.getHeight()); for (int k = 1; k <= Blur; k++) { for (int i = 0; i < pixels.length; i++) { pixelsRawSource[i \* 3 + 0] = Color.red(pixels[i]); pixelsRawSource[i \* 3 + 1] = Color.green(pixels[i]); pixelsRawSource[i \* 3 + 2] = Color.blue(pixels[i]); } int CurrentPixel = bmpSource.getWidth() \* 3 + 3; for (int i = 0; i < bmpSource.getHeight() - 3; i++) { for (int j = 0; j < bmpSource.getWidth() \* 3; j++) { CurrentPixel += 1; int sumColor = 0; sumColor = pixelsRawSource[CurrentPixel- bmpSource.getWidth() \* 3]; sumColor = sumColor + pixelsRawSource[CurrentPixel - 3]; sumColor = sumColor + pixelsRawSource[CurrentPixel + 3]; sumColor = sumColor+ pixelsRawSource[CurrentPixel+ bmpSource.getWidth() \* 3]; pixelsRawNew[CurrentPixel] = Math.round(sumColor / 4); } } for (int i = 0; i < pixels.length; i++) { pixels[i] = Color.rgb(pixelsRawNew[i \* 3 + 0], pixelsRawNew[i \* 3 + 1], pixelsRawNew[i \* 3 + 2]); } } bmpReturn.setPixels(pixels, 0, bmpSource.getWidth(), 0, 0, bmpSource.getWidth(), bmpSource.getHeight()); return bmpReturn; }`

取每三点的平均值做为当前点颜色。真正的高斯模糊应该取周围点点加权平均，而且权重符合正态分布。不过处理速度实在是太慢了，对于及时性较强的高斯模糊应该是用opengl着色实现渲染。

输出效果：




![](https://upload-images.jianshu.io/upload_images/1802307-e3922c4fddc172a0.jpg)

模糊



注：以上算法并非全部原创，但是很早以前接触的，实在找不到出处了，致歉。























