# VC++编程实现对火焰的计算机动态仿真 - 深之JohnChen的专栏 - CSDN博客

2006年03月24日 13:20:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2754


VC++编程实现对火焰的计算机动态仿真

来源: [天极网](http://www.yesky.com/)

**摘要**：本文通过对真实火焰物理特性的分析，建立了火焰动态燃烧的数学模型，并在此数学模型基础之上借助于DirectDraw技术对图形显示的加速，在VC++ 6.0下对火焰作了效果非常逼真的计算机动态仿真。

**关键词**：火焰；DirectDraw；计算机仿真 

**引言**

　　计算机仿真技术的基本原理都是一样的，神秘复杂的核爆同水波、火焰、烟雾等非常平常的自然现象在仿真处理过程中并没有什么太大的区别。都是经历了从实体对象到物理特性的总结，再由此建立数学模型并在数学模型基础之上提出仿真算法，最后通过计算机将其动态仿真出来等一系列步骤。本文以火焰作为仿真对象，通过对热源、热扩散以及对流等特性的分析对其建立了数学模型及仿真算法，为了能充分发挥计算机对图形的硬件加速，使用DirectDraw技术对仿真结果显示进行了加速，使之能逼真、流畅地对火焰的燃烧过程实行动态模拟。 

**简单近似模型设计**

　　虽然火焰在自然界是一种极普通的自然现象，但根据流体力学的相关知识，火焰可以表达为一个相当复杂的三维动态流体系统。如要在计算机中对这样一个复杂的流体系统做出精确的仿真将需要有相当庞大复杂的数学模型为基础，而且运算量将非常巨大，在现有的微型计算机中几乎很难保证其动态实时性，这也就失去了仿真的意义。因此，在仿真时应用尽可能简单的模型来实现尽可能逼真的效果。

　　从物理角度分析，要产生火焰，首先要有火源，其次为了产生"焰"的效果，需要以火源为中心向上、向四周扩散，而且由于在扩散过程中逐渐远离火源，温度会逐渐下降，表现在视觉上就是火焰的冷却变暗。另外，由于火焰的高温使周围空气受热膨胀比重下降，因此会有空气的对流出现，这将把火焰向上"吹"起，使火焰向四周扩散的距离要远小于向上扩散的距离。基于以上几点认识，可以采取对应的仿真措施：对火源的设置可以用一幅二值位图来标识，非火源以低亮度像素填充、火源点则设以高亮度像素，通过对位图像素值的判别可以断定当前点是否为火源。

`![](http://cimg.163.com/catchpic/E/E9/E9B49CC2DDA9378A0AB027BD5B79D4A6.jpg)`
　　对于火源的温度高低可用其所在点的亮度来描述；对于火焰扩散的模拟，为尽量减少运算量，在此简单地用某火源点(x,y)及其前后左右邻近四点的均值来近似，即Pixel(x,y)=(Pixel(x,y)+Pixel(x,y-1)+Pixel(x,y+1)+Pixel(x-1,y)+Pixel(x+1,y))/5，虽然该近似算法没有采取正余弦的方法精确，但运算速度极快，而且在后续的实验效果上同采用正余弦的方法几乎没有任何差别；由于在仿真过程中对火焰的温度是通过改变其亮度值来实现的，因此对于扩散过程的冷却可对像素点降低一个固定的亮度值来实现。衰减值的大小需要视所希望火焰冷却速度的快慢而定；对流对火焰产生的直接影响就是使火焰始终保持向上燃烧，因此可通过将当前火焰上滚一至两个像素来加以实现。根据前面描述的仿真运算法则，可将火焰的扩散和对流融合在一起实现，这将在一定程度上减少运算量，使产生的火焰在视觉上更加真实。实现上述近似模型的伪代码可大致设计如下： 

```
ARRAY_OF_BYTES: buffer1(xsize*ysize)，buffer2(xsize*ysize)
While(TRUE){
　for(y=1;y　　for(x=1;x　　　n1 = buffer1(x+1, y) //读取4相临像素值
　　　n2 = buffer1(x-1, y)
　　　n3 = buffer1(x, y+1)
　　　n4 = buffer1(x, y-1)
　　　p = ((n1+n2+n3+n4+p) /5)； //四临像素均值
　　　p = p-c； //同一固定冷却衰减值相减
　　　if(p<0)
　　　　p=0
　　　buffer2(x,y-1)=p 
　　}
　}
　copy buffer2 to the screen ; //显示下一帧
　copy buffer2 to buffer1； //更新Buffer1
}
```

**火焰非均匀冷却的改进模型**

　　根据上述近似模型可对火焰进行一定程度上的仿真，但由于没有引入随机分布火焰往往看上去相当单调规则，而且火焰总呈线性上升，冷却速度也严格地保持统一速率。要消除以上问题，可通过引入随机非均匀因素来解决。一种途径是随机布置各点冷却值使火焰冷却过程非均匀化。但由于火焰的模拟过程是实时进行的，为确保动态模拟过程中能顺畅进行，最好用预先创建的冷却位图（见右图）来代替。一般采用在屏幕上随机撒布几千个亮度不同的点并对其应用平滑处理等方法对冷却位图加以填充。通过冷却图中获取的数值来代替原来固定的冷却衰减值效果要好的多，此时的冷却过程改进为Pixel(x,y)=Pixel(x,y)-Coolingmap(x,y) 这样的衰减结果将使火焰的冷却衰减效果更加真实：

```
p = lightBuf2+imgWidth*2;
pp = coolMap + coolMapWidth*2;
p1 = lightBuf1+imgWidth*2;
p2 = p1 - imgWidth;
p3 = p1 - 1;
p4 = p1 + 1;
p5 = p1 + imgWidth;
for(i=0;i　//计算某点及其四邻像素均值
　c1=(unsigned char)(((UINT)*p1+(UINT)*p2+(UINT)*p3+(UINT)*p4+(UINT)*p5)/5);
　c2 = *pp;
　if(c1>c2)
　　c1 -= c2;
　*p = c1;
　pp++,p++,p1++,p2++,p3++,p4++,p5++; //内存指针修正
}
```

　　由于火焰在进行冷却衰减的同时也在进行着火焰的扩散与对流因此必须使这几种效果保持同步，这需要以同对流速度相同的速度向上滚动冷却位图来实现。为减少不必要的操作，滚动是在内存中通过改变冷却位图的垂直偏移量来加以实现：

`memcpy(lightBuf1,lightBuf1+imgWidth*3,imgWidth*(imgHeight-3));`
　　经过以上几步处理虽有一定程度的改善，但仍存在一些缺陷，比如生存期、火焰上升速度恒定、在整个空间燃烧等。为使仿真效果更加逼真，可通过设置种子点来对上述缺陷加以改进。同样出于处理速度的考虑，最好将种子点也以位图的形式预先设定，在仿真时直接在内存中通过移动指针来完成对种子点的访问，其主要代码大致如下：

```
int t = RAND_MAX/5;
topX = (imgWidth - seedMapWidth)/2; //seedMapWidth种子位图宽度
topY = (imgHeight - seedMapHeight)/2; //seedMapHeight种子位图高度
p = lightBuf1 + (topY+2)*imgWidth + topX; //p, unsigned char型指针
ps = seedMap + seedMapWidth*2; //ps, unsigned char型指针
for(j=0;j<(seedMapHeight-4);j++) {
　p1 = p; //p1, unsigned char型指针
　for(k=0;k　　if(*ps != 0){ //ps, unsigned char型指针
　　　if(rand() < t)
　　　　*p1 = 255;
　　}
　　p1++,ps++; //指针修正
　}
　p += imgWidth; //指针修正
}
```

**图形加速显示**　　前面的算法设计中一直很注意减少不必要的运算量以期获得尽可能高的处理速度，但仅靠好的算法远不能取得满意的视觉效果。不少大型游戏尽管场景非常复杂，场景变化快，但玩家很少能感觉到游戏有难以忍受的停顿感。这不仅因为游戏采取了好的算法更重要的是游戏在同玩家交互的过程中大量采用了Direct X技术，该技术是Direct Draw、Direct Sound、Direct 3D等诸多技术的总称。DirectDraw是其中最主要的一个部件，主要负责对图形的加速，并允许程序员可以直接操作显存、硬件位图映射以及硬件覆盖和换页技术。而且该技术还支持双缓冲和图形换页、3D z-buffers （z缓存）以及z方向（z-ordering）硬件辅助覆盖等许多重要功能。可以看出，通过使用Direct Draw技术将极大改善仿真结果的图形输出效果，能非常流畅地对火焰进行实时的仿真。使用该技术之前必须先进行初试化等预处理工作：

//创建DirectDraw对象（为突出程序流程，以下均对错误检测进行了省略）
DirectDrawCreate( NULL, &lpDD, NULL ); 
//取得全屏独占模式
lpDD->SetCooperativeLevel(hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );
//设置显示器显示模式
lpDD->SetDisplayMode( 640,480, 16 );
//填充主页面信息
ddsd.dwSize = sizeof( ddsd );
ddsd.dwFlags = DDSD_CAPS|DDSD_BACKBUFFERCOUNT;
ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |DDSCAPS_FLIP | DDSCAPS_COMPLEX;
ddsd.dwBackBufferCount = 1; //一个后台页面
//创建主页面
lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL );
ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
lpDDSPrimary->GetAttachedSurface(&ddscaps,&lpDDSBack);
DDPIXELFORMAT pixelFormat;
pixelFormat.dwSize = sizeof(DDPIXELFORMAT);
lpDDSPrimary->GetPixelFormat(&pixelFormat);
……
　　初始化完成后可以通过在后台页面绘图，并在绘制完毕后将后台页面复制到主页面完成对一帧图像的显示：

lpDDSBack->Blt(NULL,NULL,NULL,DDBLT_COLORFILL|DDBLT_WAIT, &ddbltfx);
ddrval = lpDDSBack->Lock(NULL, &ddsd, 0, NULL) //锁定后台页面
while (ddrval== DDERR_WASSTILLDRAWING);
if( ddrval == DD_OK ){ 
　fire.render((WORD*)ddsd.lpSurface); //完成对一帧火焰的渲染
　lpDDSBack->Unlock(NULL); //解锁后台页面
}
while( 1 ) {
　ddrval = lpDDSPrimary->Flip( NULL, 0 ); //换页
　if( ddrval == DD_OK )
　　break;
　if( ddrval == DDERR_SURFACELOST ){
　　ddrval = lpDDSPrimary->Restore(); //恢复主页面
　if( ddrval != DD_OK )
　　break;
}
if( ddrval != DDERR_WASSTILLDRAWING )
　break;
}
　　根据以上程序算法对火焰进行了仿真实验，在速度和仿真结果在视觉的逼真程度上都获得了非常好的效果。右图是从仿真过程中截取的一帧画面，从图中可以看出，虽然在前面的算法设计过程中多处采用了看似过分的近似处理，但并未因此产生负面效果。实验表明，本文采用的在数据缓冲区中对图象进行处理的方法在程序运算和显示的速度上与仿真对象--火焰的复杂程度是无关的，因此用类似的方法完全可以比较容易地实现对其他复杂物理、自然现象的仿真模拟。

![](http://dev.yesky.com/imagelist/05/12/c346q015e6ma.jpg)

**结论**　　本文通过对火焰的计算机仿真模拟实现过程，对仿真模拟类程序一般的设计实现过程做了简要介绍。通过对本文所述程序设计思路与实现方法的理解，可以用类似的方法结合实际情况灵活选用诸如OpenGL、Direct3D等不同的软件接口对其他一些自然现象进行仿真模拟。本文所述程序在Windows 98下，由Microsoft Visual C++ 6.0调试通过

