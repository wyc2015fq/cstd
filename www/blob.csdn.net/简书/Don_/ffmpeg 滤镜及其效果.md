# ffmpeg 滤镜及其效果 - 简书


# ffmpeg 滤镜及其效果



## 相关

[ffmpeg代码中使用滤镜](https://link.jianshu.com?t=http://blog.csdn.net/dangxw_/article/details/48999109)
[命令行使用滤镜](https://link.jianshu.com?t=http://www.ffmpeg.org/ffmpeg-filters.html#Video-Filters)

## 滤镜及其效果

原素材：






![](https://upload-images.jianshu.io/upload_images/1802307-5102ebbdfa06fbe2)





1.boxblur

功能：模糊处理。

接受参数有：


> 
luma_radius, lrluma_power, lpchroma_radius, crchroma_power, cpalpha_radius, aralpha_power, ap


另外参数值可以使用：可以使用w,h等变量表达式。

使用举例：

> 
boxblur=2:1:cr=0:ar=0


效果：






![](https://upload-images.jianshu.io/upload_images/1802307-20bff7accc0f1df8)




2.blend 是视频叠加，在代码中无法直接使用之前的初始化滤镜使用，查看过ffmpeg实现之后发现其实现就是历遍每个yuv值，通过两个视频帧的权重来加权叠加。

3.colorbalance ,颜色平衡 调整rgb得值权重 分为三个阶层  用于调整饱和度和调整颜色偏移值。

接受的参数有：

> 
rsgsbsrmgmbmrhghbh


参数不接受表达式。

使用举例：

> 
colorbalance=rs=.3


解释：就是降低红色的权重，使r值乘以0.3，效果上就是使视频变冷。

效果如下：






![](https://upload-images.jianshu.io/upload_images/1802307-9becd9c97449e710)





（蓝色加绿色会调出黄色，红色降低，所以会成为这个效果。）

4.colorlevels

并没有理解这个所对应的三组参数是什么意思。

但是官网给出使用的例子和效果：
> 
Make video output darker:colorlevels=rimin=0.058:gimin=0.058:bimin=0.058

Increase contrast:colorlevels=rimin=0.039:gimin=0.039:bimin=0.039:rimax=0.96:gimax=0.96:bimax=0.96

Make video output lighter:colorlevels=rimax=0.902:gimax=0.902:bimax=0.902

Increase brightness:colorlevels=romin=0.5:gomin=0.5:bomin=0.5
5.colorchannelmixer  可以设置rgba四个分量的变换关系，共接受16个参数。

> 
rrrgrbragrgggbgabrbgbbbaaragabaa


参数本质就是加权求值时候的权重，每个参数接受0-1得值，滤镜本质是将每个颜色值加权求值.公式：red=red*rr+blue*rb+green*rg+alpha*ra 。green/blue和alpha同理。参数默认值对应得是1，其他为0.如：rr，rg，rb，ra。其中rr默认为1，其他为0。

使用例子：

> 
colorchannelmixer=.3:.4:.3:0:.3:.4:.3:0:.3:.4:.3


效果：






![](https://upload-images.jianshu.io/upload_images/1802307-a402f1687c2c46af)





之所以会是灰阶效果，原因是将每个颜色分量相等，所以rgb混合后为透明度不同得黑色

褐色处理：

> 
colorchannelmixer=.393:.769:.189:0:.349:.686:.168:0:.272:.534:.131;


任何三个颜色值根据该权重计算后都为不同程度的褐色

效果：






![](https://upload-images.jianshu.io/upload_images/1802307-a81e7fb0619c5c7f)





6.crop 裁剪帧，而且可以接受很多变量的表达式，甚至包括帧的下标和时间。

使用举例：

> 
crop=w=100:h=100:x=50:y=50


效果






![](https://upload-images.jianshu.io/upload_images/1802307-eee44f3338404c33)




7.dctdnoiz 对视频降噪。

感觉没有什么作用，因为处理速度太慢了，而且效果不明显，倒不如加点噪音参生怀旧感。增加噪音在之后中。

8.decimate 降帧，实现原理就是按照固定间隔取帧，假如想要实现这个功能，直接在跳过该frame就行了，没必要拿来过滤镜。

9.deshake 删除相机抖动带来的负面影响。

10.delogo 去除视频某处的logo。其实实现原理就是将给出区域进行高斯模糊处理。

使用举例：

> 
delogo=x=0:y=0:w=100:h=77:band=10


x,y,w,h 构成一个Rect，band是模糊强度。




效果如下：




![](https://upload-images.jianshu.io/upload_images/1802307-693540a3a0ddb751)




11.drawbox 添加边框。

所接受参数如下：

> 
drawbox=x=10:y=10:w=100:h=100:color=pink@0.5:t=max


效果如下：






![](https://upload-images.jianshu.io/upload_images/1802307-2f64c718233b4aff)





可以将边框设置成实心的边框从而达到覆盖一层遮罩的目的。

12.drawgrid 画网格

给出起始坐标 和 网格的宽高颜色就可以绘画出一个网格覆盖在视频上。

使用举例：
> 
drawgrid=width=100:height=100:thickness=2:color=black@0.9


效果：






![](https://upload-images.jianshu.io/upload_images/1802307-d813ece3c92fa67f)




13.drawtext 字幕添加

功能强大，除了设置字体位置等常规参数外还可以设置字体，但是需要拥有字体对应的数据库。

参数很多，还是看[官网](https://link.jianshu.com?t=http://www.ffmpeg.org/ffmpeg-filters.html#drawtext-1)吧：
14.edgedetect 边缘检测

感觉这个最好还是当作中间操作来用，因为单纯过这个滤镜并不会有什么美感，但是边缘检测却是实现其他操作的必经之路，比如：油画效果，动画片效果等等。

使用举例：
> 
edgedetect=low=0.1:high=0.4


效果：






![](https://upload-images.jianshu.io/upload_images/1802307-bb4f6195f2d353cb)





边缘检测之后填充油画效果：






![](https://upload-images.jianshu.io/upload_images/1802307-c8a0cfbce2e04a26)




15.eq 这个功能很强大，应用也很广泛，可以用来调整亮度、饱和度、对比度、gama变换。

接受参数：

cantrast ：对比度  取值：（-2,2）；

brightness：亮度  取值同上。

saturation：饱和度 取值同上。

gama变换参数如下：
> 
gammaSet the gamma expression. The value must be a float in range 0.1 to 10.0. The default value is "1".gamma_rSet the gamma expression for red. The value must be a float in range 0.1 to 10.0. The default value is "1".gamma_gSet the gamma expression for green. The value must be a float in range 0.1 to 10.0. The default value is "1".gamma_bSet the gamma expression for blue. The value must be a float in range 0.1 to 10.0. The default value is "1".


另外还接受几个变量不一一列举。

给出调出的几个效果：

高亮





![](https://upload-images.jianshu.io/upload_images/1802307-cfae9aa8075ffa7c)





低对比度






![](https://upload-images.jianshu.io/upload_images/1802307-d875f491d311fec8)





高饱和：





![](https://upload-images.jianshu.io/upload_images/1802307-1e00ce95b33c4944)





gama变换，变冷：





![](https://upload-images.jianshu.io/upload_images/1802307-eb1b4406f8abb8e7)




16.fade  这个是控制出场和入场时慢慢消失或渐渐显示的效果的。

所跟参数有：

t  取值为in或out ，就是类型选择。

s  开始位置，以帧为单位，默认值是0；

n  是描述该动画会持续多少帧，单位是帧，而不是秒；

st 以秒为单位描述起始位置。

d 也是描述动画持续多久，但是单位是秒

alpha 透明度的极值；

color  隐藏时补上的颜色，默认是黑色。

使用举例：
> 


fade=in:0:30

意思是 从第0帧开始播放动画，共持续三十帧。

效果（这张图片是第十帧时候的效果）：








![](https://upload-images.jianshu.io/upload_images/1802307-a3467f2879d895a3)




17.fieldorder 是用来梳理帧的，可以使视频变得更加小，测试发现效果并不明显，当然，梳理效果取决于原视频的梳理程度。

18.format 帧的format的改变，但是压缩h264和h265时候只接受yuv420p的格式，所以改过之后就不能再拿来压缩成视频了。但是假如有某种需要转换成rgb的需求，可以使用该滤镜，比如需要使用opencv ，或让移动端显示，都只能转换成rgb格式。该滤镜和之后要提的scale滤镜都是使用ffmpeg中swscale 来实现的，其实使用swscale使用起来也很简单，没必要用滤镜实现该功能。

19.fps 改变帧率，不知道内部如何实现，但是应该不是单纯跳帧这么简单，因为耗时很久，所以不推荐使用。

20.scale  缩放。

使用举例：
> 


scale=200:200

缩放成200*200 的长宽，假如不符合原比例，会变形，而且scale中的w和h两个参数接受奇数值（这个我想不通，yuv420格式的长宽怎么能是奇数呢，如果有知道原因的，求告知。）

其中接受很多参数和变量，比较简单，不在赘述。

效果：








![](https://upload-images.jianshu.io/upload_images/1802307-9ee0352a56eaa9d5)




21.framestep  快进，目测实现就是跳帧显示而已。

22.geq  功能特别强大，因为它不仅仅因为它接受的参数众多，而且表达式也很多，所以可以动态的设置很多东西，甚至可以跟播放进度产生关系，进而达到视频闪烁的效果。

详情请看官网：[](https://link.jianshu.com?t=http://www.ffmpeg.org/ffmpeg-filters.html#geq)http://www.ffmpeg.org/ffmpeg-filters.html#geq

冷暖分割的效果（仅仅是使用了x变量，使用其他变量会调出更多效果）：
> 


geq=r='X/Wr(X,Y)':g='(1-X/W)g(X,Y)':b='(H-Y)/H*b(X,Y)'

就是根据位置变换该点的rgb的增强和减弱。

效果：








![](https://upload-images.jianshu.io/upload_images/1802307-fcb3c30ea876adaf)




但是这样通过表达式确定rgb的用法，通过一帧会消耗很多时间，因为每个帧都要先转变成rgb再通过表达式对每个点进行变换，变换因为并不是加减而是乘除法，所以会相对而言会消耗时间更多。

还可以不使用变量，调出其他效果，给出浮雕：
> 


geq：format=gray,geq=lum_expr='(p(X,Y)+(256-p(X-4,Y-4)))/2

但是相对于frei0r中的emboss 而言效果很差，就是因为前面所提到的原因，所以geq速度很慢。

效果：







![](https://upload-images.jianshu.io/upload_images/1802307-05b0ed5aa6dbfe40.png)




算法意思是检测颜色的跳变，跳变高于一定程度 时候设置其颜色偏移，否则全部设置成深灰也就是石头的颜色。

23.histep  智能优化对比度。

效果：








![](https://upload-images.jianshu.io/upload_images/1802307-471bcfe1b611c817)




24.hqdn3d 降噪，提高压缩，而且速度很快还有效。

25.hqx 放大镜，速度较慢。

其实就是选择某个区域后调用swscale 缩放。

26.hflip  横向的倒置像素。其实geq也可以实现，算法不用讲了，相对较为简单。另外还有vflip，是纵向的倒置。

效果：








![](https://upload-images.jianshu.io/upload_images/1802307-330876288b578dd9)



27.hue  其中最根本的是h这个参数但是并没有搞懂是什么意思，个人感觉功能很强大，给出几个官网所列出的使用方法和效果。








![](https://upload-images.jianshu.io/upload_images/1802307-89029df733e514ee)









![](https://upload-images.jianshu.io/upload_images/1802307-844b325e2797445d)




可以和时间和当前帧下标产生函数关系，达到闪光灯的效果。

28.il：就是将视频复制然后同屏显示。

效果：








![](https://upload-images.jianshu.io/upload_images/1802307-75f04c6f215a7130)




提供多种显示模式，更改后可以这样：








![](https://upload-images.jianshu.io/upload_images/1802307-b10698ff18115edb)



29.inflate 就是将帧和之后的帧按行切割，然后交错组成新的帧，同时也会使总帧数缩短相应的倍数。

显示格式如下：








![](https://upload-images.jianshu.io/upload_images/1802307-8a4e847ea3500491.png)




30.lueyuv /lue /luergb

可以更改yuv的值，这个功能也很强大，提供了很多变量:

常用的参数有：

lueyuv：y、u、v

luergb：r、g、b、a

接受变量：
> 


whThe input width and height.valThe input value for the pixel component.clipvalThe input value, clipped to the minval-maxval range.maxvalThe maximum value for the pixel component.minvalThe minimum value for the pixel component.negvalThe negated value for the pixel component value, clipped to the minval-maxval range; it corresponds to the expression "maxval-clipval+minval".clip(val)The computed value in val, clipped to the minval-maxval range.gammaval(gamma)The computed gamma correction value of the pixel component value, clipped to the minval-maxval range. It corresponds to the expression "pow((clipval-minval)/(maxval-minval),gamma)*(maxval-minval)+minval"

由于我所用的都是h264编码，所以使用的都是lueyuv，当然也可以使用luergb但是ffmpeg需要先转码，这样会相对耗时更久。

使用举例：
> 


lutyuv="y=maxval+minvalval:u=maxval+minval-val:v=maxval+minval-val"

其实他是将所有值反转，也可以直接使用negate滤镜，效果一样。

效果：








![](https://upload-images.jianshu.io/upload_images/1802307-46ab73f998c0c1ef)




因为lueyuv中可以直接调整y 所以很轻松的调整亮度

调暗效果：







![](https://upload-images.jianshu.io/upload_images/1802307-2f5c79fffa1ef476.png)




使用luergb可以很轻松的增强某个颜色分量，对于调冷暖颜色是很轻松的，给出较为极端的效果，去除gb颜色效果：







![](https://upload-images.jianshu.io/upload_images/1802307-e72a408362be7a8a.png)



另外可以极大增强某个颜色的分量，增强红色效果：







![](https://upload-images.jianshu.io/upload_images/1802307-dc8770bea41f322c.png)




31.negate 顾名思义，底片效果，与上一个类似。不再提了。

32.noize 添加噪音，会出现噪点，但是效果一般，假如想要添加类似美拍摩登时代中mv的效果，建议还是视频元素叠加实现。

效果：







![](https://upload-images.jianshu.io/upload_images/1802307-c8302683343c85aa.png)



32.overlay 这个用途很广，配合moive 可以添加水印。但是使用ffmpge命令行可以达到视频叠加的效果，但是该效果是ffmpeg程序实现的，使用ffmpeg的库写代码时候，不能这样使用，即便添加元素是个视频，也只会添加第一帧。

水印举例：







![](https://upload-images.jianshu.io/upload_images/1802307-256c7394712aaa3e.png)



添加水印时，水印的位置接受如下参数：

main_w  代表原视频的宽

main_h  高

overlay_w  水印元素的宽

overlay_h  高

0.0坐标是左上角。所以例如：
> 


overlay=x=main_w-overlay_w-10:y=main_h-overlay_h-10

意思是叠加到又上角，举例边界10个像素。而且其x和y接受负值，可以叠加到视频之外，部分在视频之上。

33.pad ：添加面板，可以通过该滤镜使得帧的长宽变大，然后叠加其他素材上去。

举例：
> 


pad=width=640:height=480:x=0:y=40:color=red

效果：







![](https://upload-images.jianshu.io/upload_images/1802307-9dce9506105bd3b1.png)



34.removelogo ，消除logo水印，和delogo 类似。

35.rotate 旋转视频帧，但是面板大小仍然是原视频大小，所以当没有刻意增大面板时旋转之后，除了会有部分黑色区域外，还会有部分是显示不出来的。

它接受t和n作为变量，通过表达式可以实现动态的控制旋转角度。

举例：
> 


rotate=Asin(2PI/T*t)

根据播放时间的增加，视频会不停旋转，因为每一帧的旋转角度会根据播放时间改变。

效果：







![](https://upload-images.jianshu.io/upload_images/1802307-8be46cfa38a92341.png)









![](https://upload-images.jianshu.io/upload_images/1802307-156e8340778f0ff8.png)









![](https://upload-images.jianshu.io/upload_images/1802307-22f07bc94332ae70.png)




36.sab 可以降低颜色和亮度的模糊度，降低锯齿，但是不建议使用，因为速度太慢。

37.showpalette  在视频中显示一个颜色板，感觉很有科技感，但是与frei0r 的效果相比，差很多。
[](https://link.jianshu.com?t=http://www.cnblogs.com/nlsoft/archive/2013/05/07/3065311.html)freior的效果

38.histogram  生成一个直方图，用来表示视频颜色的走向。效果也和frei0r 差很多。

效果：







![](https://upload-images.jianshu.io/upload_images/1802307-15d9bd9e15ffe772.png)



39.shuffleplanes  翻译过来是变换面板，但是没有理解它的原理。给出官网上的例子
> 


shuffleplanes=0:2:1:3

效果：







![](https://upload-images.jianshu.io/upload_images/1802307-bba36ac072566041.png)



40.smartblur  制造模糊的同时不影响轮廓。

41.stereo3d  这个比较酷炫是分割视频再融合。可以选择分割和合并的方式。参数如下：

in 和out

in 即为分割方式，接受值有：
> 


‘sbsl’side by side parallel (left eye left, right eye right)‘sbsr’side by side crosseye (right eye left, left eye right)

‘sbs2l’side by side parallel with half width resolution (left eye left, right eye right)

‘sbs2r’side by side crosseye with half width resolution (right eye left, left eye right)

‘abl’above-below (left eye above, right eye below)

‘abr’above-below (right eye above, left eye below)

‘ab2l’above-below with half height resolution (left eye above, right eye below)

‘ab2r’above-below with half height resolution (right eye above, left eye below)

‘al’alternating frames (left eye first, right eye second)

‘ar’alternating frames (right eye first, left eye second)

‘irl’interleaved rows (left eye has top row, right eye starts on next row)

‘irr’interleaved rows (right eye has top row, left eye starts on next row)

默认值是sbsr

out 的接受值：
> 


‘arbg’anaglyph red/blue gray (red filter on left eye, blue filter on right eye)

‘argg’anaglyph red/green gray (red filter on left eye, green filter on right eye)

‘arcg’anaglyph red/cyan gray (red filter on left eye, cyan filter on right eye)

‘arch’anaglyph red/cyan half colored (red filter on left eye, cyan filter on right eye)

‘arcc’anaglyph red/cyan color (red filter on left eye, cyan filter on right eye)

‘arcd’anaglyph red/cyan color optimized with the least squares projection of dubois (red filter on left eye, cyan filter on right eye)

‘agmg’anaglyph green/magenta gray (green filter on left eye, magenta filter on right eye)

‘agmh’anaglyph green/magenta half colored (green filter on left eye, magenta filter on right eye)

‘agmc’anaglyph green/magenta colored (green filter on left eye, magenta filter on right eye)

‘agmd’anaglyph green/magenta color optimized with the least squares projection of dubois (green filter on left eye, magenta filter on right eye)

‘aybg’anaglyph yellow/blue gray (yellow filter on left eye, blue filter on right eye)

‘aybh’anaglyph yellow/blue half colored (yellow filter on left eye, blue filter on right eye)

‘aybc’anaglyph yellow/blue colored (yellow filter on left eye, blue filter on right eye)

‘aybd’anaglyph yellow/blue color optimized with the least squares projection of dubois (yellow filter on left eye, blue filter on right eye)

‘ml’mono output (left eye only)

‘mr’mono output (right eye only)

‘chl’checkerboard, left eye first

‘chr’checkerboard, right eye first

‘icl’interleaved columns, left eye first

‘icr’interleaved columns, right eye first

列出几个调出的效果，不同组合会有不同效果。

左右分割，然后叠加融合，一个选择黄色，一个选择蓝色。







![](https://upload-images.jianshu.io/upload_images/1802307-add91616936453f1.png)


![Uploading 14_946006.png . . .]


上下分割，左右合并，效果：







![](https://upload-images.jianshu.io/upload_images/1802307-feea407120e2e8eb.png)



上下分割，叠加融合。效果：







![](https://upload-images.jianshu.io/upload_images/1802307-6f9b317d0d29d055.png)



41.super2xsai  放大。

42.swapuv  本质是交换uv两个分量。

效果如下：







![](https://upload-images.jianshu.io/upload_images/1802307-450db11023bc7b8c.png)




43.telecine 改变比特率。

44.thumbnail  顾名思义是取缩略图，感觉用处不打，因为既然能解出帧，直接就缩放一下就是缩略图了。

45.tinterlace 合并帧，合并方式有多种。

有一个方式参数：mode

接受值有：

‘merge, 0’速度提升两倍，因为每两帧融合为一帧，按行融合。模式如下：







![](https://upload-images.jianshu.io/upload_images/1802307-2a16a0938acf44fc.png)



44444‘drop_odd, 1’速度提升两倍，跳帧显示，保留偶数pts的帧。模式如下：







![](https://upload-images.jianshu.io/upload_images/1802307-3e294cf5f3f7326f.png)



44444‘drop_even, 2’速度提升两倍，跳帧显示，保留奇数pts的帧。模式如下：







![](https://upload-images.jianshu.io/upload_images/1802307-5c1c6c1085c8d08b.png)



33333‘pad, 3’速度不提升，模式如下：







![](https://upload-images.jianshu.io/upload_images/1802307-037c18788ac93d11.png)



44444‘interleave_top, 4’速度提升两倍，模式如下：







![](https://upload-images.jianshu.io/upload_images/1802307-ca93c148df6ccc8b.png)



44444‘interleave_bottom, 5’与上个类似，只是行像素的顺序变了。

凡是交叉行融合帧的模式，输出都会有略微的不清楚现象。

46.trim 剪切视频，是时间上的裁剪。

47.tile 选取帧同屏显示。但是速度会相应倍数的提升。

效果如图：







![](https://upload-images.jianshu.io/upload_images/1802307-13a350aa5acd7c11.png)



48.unsharp  描边视频。

对它并不是特别懂，给出官网上例子的效果：







![](https://upload-images.jianshu.io/upload_images/1802307-2088bd4413886dfb.png)



49.vignette 光晕效果

可以调整角度，中心点位置等等。

举例：

vignette=PI/4

效果：







![](https://upload-images.jianshu.io/upload_images/1802307-c83c59e79629b73e.png)



50.vfilp ：这个是与之前的hflip类似，甚至其他滤镜也可以实现。就是纵向倒置

效果如下：







![](https://upload-images.jianshu.io/upload_images/1802307-e4df65aa58f29f87.png)



另外还有很多，共137个，我是针对自己比较熟悉的列举用法和效果。

另外如果编译进去frei0r的话，它拥有更多的特效，而且效果酷炫，不过处理速度较慢。
[](https://link.jianshu.com?t=http://www.cnblogs.com/nlsoft/archive/2013/05/07/3065311.html)frei0r的效果


























