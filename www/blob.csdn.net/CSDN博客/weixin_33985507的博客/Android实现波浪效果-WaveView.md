# Android实现波浪效果 - WaveView - weixin_33985507的博客 - CSDN博客
2015年09月14日 14:26:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
## 效果图
先上效果图
![870483-adfc2146c4b54c95.gif](https://upload-images.jianshu.io/upload_images/870483-adfc2146c4b54c95.gif)
screenshot.gif
## 实现
### WaveView的属性
![870483-532a93537477fea7..png](https://upload-images.jianshu.io/upload_images/870483-532a93537477fea7..png)
WaveView的属性
> - Wate Level(水位) - 波浪静止时水面距离底部的高度
- Amplitude(振幅) - 波浪垂直振动时偏离水面的最大距离
- Wave Length(波长) - 一个完整的波浪的水平长度
- Wave Shift(偏移) - 波浪相对于初始位置的水平偏移
### 实现思路
设想我们有一个画好波形的图片，那么我们只需要用这张图片填充（X轴方向重复，Y轴方向延伸）整个View，然后水平移动图片，就可以得到波浪效果了。
所以要做的事很简单：绘制一个波形图，填充到View里，移动波形图。
**1. 绘制初始波形**
```
private void createShader() {
    ...
    Bitmap bitmap = Bitmap.createBitmap(getWidth(), getHeight(), Bitmap.Config.ARGB_8888);
    Canvas canvas = new Canvas(bitmap);
    // Draw default waves into the bitmap
    // y=Asin(ωx+φ)+h
    float waveX1 = 0;
    final float wave2Shift = mDefaultWaveLength / 4;
    final float endX = getWidth();
    final float endY = getHeight();
    ...
    while (waveX1 < endX) {
        double wx = waveX1 * mDefaultAngularFrequency;
        int startY = (int) (mDefaultWaterLevel + mDefaultAmplitude * Math.sin(wx));
        // draw bottom wave with the alpha 40
        canvas.drawLine(waveX1, startY, waveX1, endY, wavePaint1);
        // draw top wave with the alpha 60
        float waveX2 = (waveX1 + wave2Shift) % endX;
        canvas.drawLine(waveX2, startY, waveX2, endY, wavePaint2);
        waveX1++;
    }
    // use the bitamp to create the shader
    mWaveShader = new BitmapShader(bitmap, Shader.TileMode.REPEAT, Shader.TileMode.CLAMP);
    mViewPaint.setShader(mWaveShader);
}
```
首先一个长宽恰等于WaveView的Bitmap：`Bitmap.createBitmap(getWidth(), getHeight(), Bitmap.Config.ARGB_8888)`。
在Bitmap中使用默认的属性绘制出初始波形。初始波形的属性：Wate Level(水位)为WaveView高度的1/2；Amplitude(振幅)为WaveView高度的1/20；Wave Length(波长)等于WaveView的宽度。
绘制好的初始波形是下面这个样子：
![870483-74e338d00f985081..png](https://upload-images.jianshu.io/upload_images/870483-74e338d00f985081..png)
初始波形
代码第 9 ~ 27 行进行初始波形的绘制。波形由wave1和wave2两个波组成，wave2就是wave1向左偏移1/4的wave length，所以不需要重复计算。
最后把这个Bitmap设置成为Paint的Shader。设置Shader相当于设定画笔的形状，使用设置了Shader的Paint绘制图形时，实际上是在使用Bitmap填充绘制的区域。X轴的填充方式为`TileMode.REPEAT`，即重复填充；Y轴的填充方式为`TileMode.CLAMP`，即使用边缘的色值延伸填充。
**2. 调整Bitmap的大小并填充到WaveView**
有了初始波形，当WaveView的属性改变时，只需要对初始波形进行相应的拉伸/压缩和位移就可以得到用户想要的波形。
```
// sacle shader according to mWaveLengthRatio and mAmplitudeRatio
// this decides the size(mWaveLengthRatio for width, mAmplitudeRatio for height) of waves
mShaderMatrix.setScale(
        mWaveLengthRatio / DEFAULT_WAVE_LENGTH_RATIO,
        mAmplitudeRatio / DEFAULT_AMPLITUDE_RATIO,
        0,
        mDefaultWaterLevel);
// translate shader according to mWaveShiftRatio and mWaterLevelRatio this decides the start position(mWaveShiftRatio for x, mWaterLevelRatio for 
// this decides the start position(mWaveShiftRatio for x, mWaterLevelRatio for y) of waves
mShaderMatrix.postTranslate(
        mWaveShiftRatio * getWidth(),
        (DEFAULT_WATER_LEVEL_RATIO - mWaterLevelRatio) * getHeight());
// assign matrix to invalidate the shader
mWaveShader.setLocalMatrix(mShaderMatrix);
float radius = getWidth() / 2f
        - (mBorderPaint == null ? 0f : mBorderPaint.getStrokeWidth());
canvas.drawCircle(getWidth() / 2f, getHeight() / 2f, radius, mViewPaint);
```
代码 3 ~ 6 行对Shader进行拉伸/压缩，10 ~ 12 行对Shader进行水平/竖直平移。
代码 17 ~ 19 行用Shader填充成想要的形状。
**3. 动画**
```
// horizontal animation.
// wave waves infinitely.
ObjectAnimator waveShiftAnim = ObjectAnimator.ofFloat(
        mWaveView, "waveShiftRatio", 0f, 1f);
waveShiftAnim.setRepeatCount(ValueAnimator.INFINITE);
waveShiftAnim.setDuration(1000);
waveShiftAnim.setInterpolator(new LinearInterpolator());
animators.add(waveShiftAnim);
// vertical animation.
// water level increases from 0 to center of WaveView
ObjectAnimator waterLevelAnim = ObjectAnimator.ofFloat(
        mWaveView, "waterLevelRatio", 0f, 0.5f);
waterLevelAnim.setDuration(10000);
waterLevelAnim.setInterpolator(new DecelerateInterpolator());
animators.add(waterLevelAnim);
// amplitude animation.
// wave grows big then grows small, repeatedly
ObjectAnimator amplitudeAnim = ObjectAnimator.ofFloat(
        mWaveView, "amplitudeRatio", 0f, 0.05f);
amplitudeAnim.setRepeatCount(ValueAnimator.INFINITE);
amplitudeAnim.setRepeatMode(ValueAnimator.REVERSE);
amplitudeAnim.setDuration(5000);
amplitudeAnim.setInterpolator(new LinearInterpolator());
animators.add(amplitudeAnim);
```
代码 3 ~ 8 行让波形一直向右移动，效果就是波形一直在波动。
代码 12 ~ 16 行让水位从0逐渐涨到WaveView高度的一半。
代码 20 ~ 26 行波浪的大小从大变小，再从小变大。
## 源代码
代码在github：[WaveView](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fgelitenight%2FWaveView)
## 打个广告
美团平台及酒旅事业群招人啦，欢迎加入我们！
我可以帮忙内推，简历请发到我邮箱[gelitenight@gmail.com](https://link.jianshu.com?t=mailto%3Agelitenight%40gmail.com)
> 
【美团网】高级Android开发工程师
工作内容: 负责美团酒店、旅游产品 Android 客户端的设计、开发与改进。
- 3年以上工作经验，2年以上Android开发经验；
- 熟悉Android系统，熟悉Android软件的开发、测试、分发流程；
- 良好的编程风格，扎实的编程基础和数据结构算法基础；
- 熟悉移动网络的特性，对网络编程和常用网络协议有较深刻理解和经验；
- 有一定的架构设计能力，良好的编码能力，编写文档能力；
- 热爱互联网和新技术，具有极强的快速学习能力；
- 有以下特征优先考虑：
- 有开源作品或技术博客（需原创技术文章）；
- 熟悉Socket编程。
北京、上海、厦门、成都都有职位，更多职位请见[职位列表](https://link.jianshu.com?t=https%3A%2F%2Fjob.meituan.com%2Fjob-list%3Fcity%3D001001%26department%3D5%26jobFamily%3D26%26pageNo%3D1)。
> 
本文遵循“[署名-非商业性使用-相同方式共享](https://link.jianshu.com?t=http%3A%2F%2Fcreativecommons.org%2Flicenses%2Fby-nc-sa%2F3.0%2Fdeed.zh)”的创作共同协议，欢迎转载，转载时请注明作者和出处。
作者: [gelitenight](https://link.jianshu.com?t=mailto%3Agelitenight%40gmail.com)
出处: [http://gelitenight.github.io/wave-view/](https://link.jianshu.com?t=http%3A%2F%2Fgelitenight.github.io%2Fwave-view%2F)
