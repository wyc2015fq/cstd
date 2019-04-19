# 镜像倒影特效Gallery - xqhrs232的专栏 - CSDN博客
2010年12月07日 22:44:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1188
原文地址::[http://yueguc.javaeye.com/blog/750986](http://yueguc.javaeye.com/blog/750986)
#### 效果展示
![](http://icekirin.com/wordpress/wp-content/uploads/2010/03/18.jpg)
本文档将介绍在android上如何实现一个倒影效果的Gallery。
为了达到上图所示的效果，
首先，是需要有一个自定义的gallery，实现了对Gallery的继承，通过重写getChildStaticTransformation方法来控制，每个子view有不同的缩放比例，形成阶梯状的展示。这个gallery是在坤庭的代码基础上实现的，我这里就不再重复介绍。
接下来，gallery中的每个view，都是一个自定义的MirrorView，由它来实现了画原图及画倒影。新的算法解决了性能问题，下面就重点说一下这部分内容:
镜像特效最近还蛮流行的，尤其在HTC 的Sense 介面上，常常都可以见到。大家可以看到，加了个镜像特效后，那感觉就很立体，感觉上好像是这些图片摆立在一个玻璃桌面上。
在Android 上要帮图片加上镜像特效，会不会很麻烦？一点也不麻烦，只要几行代码，就可以搞定。
因此，在开始看答案之前，我会建议你要先有Photoshop 的使用经验。想想，如果用Photoshop 要帮图片加上镜像特效，要如何做？我想一般不外乎是先复制个图片，并将其垂直翻转，接着再对这翻转的图片，加个由灰到黑的渐层mask 即可。
好了，让我们来看一下答案。底下就是帮图片加上镜像特效的程式范例。
Java代码 ![复制代码](http://yueguc.javaeye.com/images/icon_copy.gif)
- **public****class**  MirrorView **extends**  View {
   
- 
- Paint m_paint;   
- 
- **int** m_nShadowH;   
- 
- Drawable m_dw;   
- 
- Bitmap m_bitmap;   
- 
- Matrix mMatrix;   
- 
- **int** shadowHeight;   
- 
- **public** MirrorView(Context context, Bitmap bitmap) {   
- 
- **super** (context);   
- 
- m_bitmap = bitmap;   
- 
- _Init();   
- 
- }   
- 
- **private****void** _Init() {   
- 
- //m_dw = new BitmapDrawable(BitmapFactory.decodeResource(getResources(),  R.drawable.icon));
- 
- m_dw = **new** BitmapDrawable(m_bitmap);   
- 
- m_dw.setBounds(0,0,m_dw.getIntrinsicWidth(),m_dw.getIntrinsicHeight());   
- 
- m_nShadowH = m_dw.getIntrinsicHeight()/1;   
- 
- m_paint = **new** Paint(Paint.ANTI_ALIAS_FLAG );   
- 
- LinearGradient lg = **new** LinearGradient(0, 0, 0, m_nShadowH, 0xB0FFFFFF, 0×00000000, Shader.TileMode.CLAMP );
   
- 
- m_paint.setShader(lg);   
- 
- m_paint.setXfermode(**new** PorterDuffXfermode(PorterDuff.Mode.MULTIPLY ));   
- 
- mMatrix = **new** Matrix();   
- 
- }   
- 
- @Override
- 
- **public****void** onDraw(Canvas canvas) {   
- 
- **super** .onDraw(canvas);   
- 
- **int** nX = 0;   
- 
- **int** nY = 0;   
- 
- _DrawNormalImg(canvas, nX, nY);   
- 
- _DrawMirror(canvas, nX, nY);   
- 
- }   
- 
- **private****void** _DrawNormalImg(Canvas canvas, **int** nX, **int** nY) {
   
- 
- canvas.save(Canvas.MATRIX_SAVE_FLAG );   
- 
- canvas.translate(nX, nY);   
- 
- m_dw.draw(canvas);   
- 
- canvas.restore();   
- 
- }   
- 
- **private****void** _DrawMirror(Canvas canvas, **int** nX, **int** nY) {
   
- 
- **int** nW = m_dw.getIntrinsicWidth();   
- 
- **int** nH = m_dw.getIntrinsicHeight();   
- 
- shadowHeight=nH/2;   
- 
- **float** [] src={0, nH, nW, nH, nW,nH – m_nShadowH, 0, nH – m_nShadowH};
   
- 
- **float** [] dst={ 0, nH, nW, nH,nW, shadowHeight, 0, shadowHeight };
   
- 
- canvas.save();   
- 
- mMatrix.setPolyToPoly(src, 0, dst, 0, src.length >> 1);
   
- 
- canvas.concat(mMatrix);   
- 
- //draw mirror image 
- 
- canvas.save(Canvas.MATRIX_SAVE_FLAG );   
- 
- canvas.scale(1.0f, -1.0f);   
- 
- canvas.translate(nX, -(nY + nH * 2));   
- 
- canvas.clipRect(0, nH, nW, nH – m_nShadowH);   
- 
- m_dw.draw(canvas);   
- 
- canvas.restore();   
- 
- //draw mask 
- 
- canvas.save();   
- 
- canvas.translate(nX, nY + nH);   
- 
- canvas.drawRect(0, 0, nW, m_nShadowH, m_paint);   
- 
- canvas.restore();   
- 
- canvas.restore();   
- 
- }   
- 
- }  
```java
public  class  MirrorView extends  View {
Paint m_paint;
int m_nShadowH;
Drawable m_dw;
Bitmap m_bitmap;
Matrix mMatrix;
int shadowHeight;
public MirrorView(Context context, Bitmap bitmap) {
super (context);
m_bitmap = bitmap;
_Init();
}
private void _Init() {
//m_dw = new BitmapDrawable(BitmapFactory.decodeResource(getResources(),  R.drawable.icon));
m_dw = new BitmapDrawable(m_bitmap);
m_dw.setBounds(0,0,m_dw.getIntrinsicWidth(),m_dw.getIntrinsicHeight());
m_nShadowH = m_dw.getIntrinsicHeight()/1;
m_paint = new Paint(Paint.ANTI_ALIAS_FLAG );
LinearGradient lg = new LinearGradient(0, 0, 0, m_nShadowH, 0xB0FFFFFF, 0×00000000, Shader.TileMode.CLAMP );
m_paint.setShader(lg);
m_paint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.MULTIPLY ));
mMatrix = new Matrix();
}
@Override
public void onDraw(Canvas canvas) {
super .onDraw(canvas);
int nX = 0;
int nY = 0;
_DrawNormalImg(canvas, nX, nY);
_DrawMirror(canvas, nX, nY);
}
private void _DrawNormalImg(Canvas canvas, int nX, int nY) {
canvas.save(Canvas.MATRIX_SAVE_FLAG );
canvas.translate(nX, nY);
m_dw.draw(canvas);
canvas.restore();
}
private void _DrawMirror(Canvas canvas, int nX, int nY) {
int nW = m_dw.getIntrinsicWidth();
int nH = m_dw.getIntrinsicHeight();
shadowHeight=nH/2;
float [] src={0, nH, nW, nH, nW,nH – m_nShadowH, 0, nH – m_nShadowH};
float [] dst={ 0, nH, nW, nH,nW, shadowHeight, 0, shadowHeight };
canvas.save();
mMatrix.setPolyToPoly(src, 0, dst, 0, src.length >> 1);
canvas.concat(mMatrix);
//draw mirror image
canvas.save(Canvas.MATRIX_SAVE_FLAG );
canvas.scale(1.0f, -1.0f);
canvas.translate(nX, -(nY + nH * 2));
canvas.clipRect(0, nH, nW, nH – m_nShadowH);
m_dw.draw(canvas);
canvas.restore();
//draw mask
canvas.save();
canvas.translate(nX, nY + nH);
canvas.drawRect(0, 0, nW, m_nShadowH, m_paint);
canvas.restore();
canvas.restore();
}
}
```
_DrawMirror() 方法是关键。用Photoshop 要如何做出镜像特效？第一步是先画出垂直翻转的图片。
Android 绘图座标体系预设的原点在左上角，X 轴往右是越来越大的正值，而Y 轴往下，则是越来越大的正值。要画出垂直翻转的图片，其实也就是要垂直翻转整个绘图座标体系。在 Android 中，要如何做？答案就是 canvas.scale(1.0f, -1.0f)。很简单吧，没想到给scale() 函式一个负值，就可以翻转相对应的轴。
在Photoshop 中，做镜像特效的第二步是要对这翻转的图片，加个由灰到黑的渐层mask。
在Android 中，要画渐层色，那就一定得用LinearGradient 这个类别。至于要对背景图加上个mask，就请参考一下Paint 的setXfermode() 函式。 _Init() 这个函式，就是负责生成一个由灰到黑渐层mask 的m_paint 物件。
这个控件我测试过，200张图片加入adapter，在大数据量情况下性能也没有问题。
