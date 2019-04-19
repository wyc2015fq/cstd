# windows远程桌面实现之一 （抓屏技术总览 MirrorDriver，DXGI，GDI) - 建建的博客 - CSDN博客
2018年05月08日 16:22:56[纪建](https://me.csdn.net/u013898698)阅读数：1805
个人分类：[视频编解码学习](https://blog.csdn.net/u013898698/article/category/6703370)
作者
要实现远程桌面功能,首先要做的就是桌面图片的截取,而且要达到比较流畅的视觉效果,
需要到达每秒20帧以上。 
截取桌面图片,就是定时截取windows桌面的图片,随便怎么都能做到,好像挺简单的。 
通用的做法就是GetDC(GetDesktokWindow() )获取桌面的DC, 
然后使用CreateDIBSection创建一个设备无关位图以及内存DC,使用BitBlt把桌面DC的翻转到内存DC, 
这样通过内存DC就能直接获取到原始RGB数据。 
这个就是GDI函数实现的通用做法,能在所有windows平台实现。 
通用归通用,截取的效率则是有点低,尤其是要达到每秒20帧以上的截取,占用CPU有点高, 
当然配置好的机器倒也看不出多大区别(目前的电脑配置有可能让人感觉不到GDI的效率问题)。 
GDI不能获取鼠标,需要在截取的图像中把鼠标画上去。 
windows 8 以上的系统,则实现了一个DXGI,用来截取桌面,它是集成在DirectX之中,成为DX的一个子功能。 
你需要简单熟悉DirectX技术才能使用DXGI,他通过一堆啰嗦的创建和查询各种接口,最终获取到 IDXGIOutputDuplication 接口。 
截屏的时候,使用这个接口的AcquireNextFrame 函数获取当前桌面图像, 
当然这个接口还提供 GetFrameDirtyRects等函数获取发生了变化的矩形区域。 
这应该算是一个比较好的截屏方法,CPU占用也极低,可惜只支持windows 8 以上的平台,而win7,winxp等系统不支持。 
MirrorDriver 这是个驱动截取方法,MirrorDriver就是显示镜像驱动,这个来源于windows2000以上的系统, 
属于XPDM模型的显示驱动,这是个很老的驱动模型,只支持到WinXP系统, 
随着计算机图形技术的发展尤其是3D技术的发展,老的XPDM模型已经不再适应最新3D技术。 
因此在win7之后,就不再支持 XPDM模型的显示驱动,而采用WDDM模型的显示驱动,到了win10, WDDM已经达到了2.0版本。 
不知道何原因,微软估计是在WDDM驱动中没有提供类似镜像驱动模型。 
然而在高效截屏中需要用到驱动技术,因此才保留了XPDM模型的镜像驱动跟WDDM显示驱动兼容。 
仅仅只是XPDM镜像驱动跟WDDM驱动兼容,XPDM显示驱动不能运行在WIN7以上系统中。 
简单的一句话说就是: MirrorDriver驱动可以安全和方便的运行在WINXP,  WIN7, WIN8, WIN10 等系统中。 
因此MirrorDriver的通用性也是跟GDI一样支持所有windows平台,但是不支持古老的使用DirectDraw加速的程序, 
这些程序在MirrorDriver环境下甚至都无法运行。 
MirrorDriver跟DXGI一样,截取图像数据以及更新过的矩形区域,占用的CPU都是极低的。 
或者说他们在windows内核中的实现本质上都是一样的。 
然而都挺可惜,以上三种办法都没法截取处于屏幕独占模式下的游戏图像。 
也就是对显卡硬件加速的比如Direct3D等程序不太友好。 
DXGI不是太确定,我没试过win10运行全屏模式游戏能否使用DXGI截取到图像。 
所谓独占模式,就是全屏下的游戏 
(当然有些全屏游戏可以截取,那它应该属于那种伪独占模式,看起来在全屏中运行,实际上还是属于某个窗口)。 
处理这种截屏,估计只有Hook DirectX(或者OpenGL)中函数库。 
这种HOOk,需要开发一个DLL,把这个DLL注入到具体的程序中,然后挂钩程序调用的DirectX 渲染函数,从而获取到图像数据。 
DLL的注入技术倒是不难,难的是我对DirectX或者说是3D图形技术并不熟练,不清楚哪些程序使用哪些渲染函数, 
而且DirectX的版本又多,每个版本估计都得处理一下。 
windows平台下还有其他一些偏门办法来截图,几乎都没以上三种办法好,甚至更糟糕。 
windows平台没能提供一个完善的抓屏解决方案,需要对各种特殊情况做查漏补缺似的修补。 
因此套用别人的一个做法是最好的了: 
使用一个高清摄像机对着windows电脑屏幕拍,然后一切都能拍下来,什么都能解决了,而且还兼容各种操作系统桌面 ![windows远程桌面实现之一 （抓屏技术总览 MirrorDriver，DXGI，GDI)_Windows](http://aliyunzixunbucket.oss-cn-beijing.aliyuncs.com/jpg/c63265173fddabe9e70729c1de7ac8cc.jpg?x-oss-process=image/resize,p_100/auto-orient,1/quality,q_90/format,jpg/watermark,image_eXVuY2VzaGk=,t_100,g_se,x_0,y_0) 。 
然而有没有一个通用的办法,在所有windows平台下能截取到所有程序的显示的图像呢? 
有一个并不值得推荐的办法就是开发虚拟显卡驱动程序。 
支持win7以上的系统得开发WDDM模型的驱动,如果要支持winxp,还得开发XPDM的虚拟显卡驱动。 
到目前为止,我还没研究过WDDM模型驱动,只熟悉MirrorDriver驱动开发。 
WDDM模型分为应用层的显示驱动部分和内核层的小端口驱动部分, 
他们需要实现的回调函数有起码几十个,看着都觉得恐怖。 
暂时还不想栽倒在WDDM模型研究中,再说研究显示驱动需要有扎实的图形技术基础,等闲下来再说。 
有了虚拟显卡驱动,把它装到电脑上,然后选择虚拟显卡驱动为主显卡, 
接着电脑所有显示的数据都进入到我们的虚拟显卡中,从而就能截取到所有的图像数据了。 
然而电脑显卡已经不是真实的显卡,本地显示器上将是黑屏,我们只能把截获的图像数据通过各种通讯线路, 
比如USB(采用USB通讯的我们可以简单称它为USB显卡),网络等通讯手段还原到远端设备上显示出来。 
虚拟显卡这种办法在我们平常使用的远程桌面中不大适用,谁也不想自己电脑控制时候,显示器是黑屏,而且还不能切换回来。 
但是这为云桌面的构建,提供了一个基础组件。 
云桌面加了个“云”字,似乎显得有点高大上,其实就跟我们平常使用的远程桌面差不多的功能, 
当然被控制端的实现方式跟普通远程桌面的被控制端有点不大相同。 
首先被控端系统被安装到虚拟机容器中,简单的说,就是一个物理服务器, 
上边装类似vmware,hyper-v,virtualbox,kvm等虚拟机软件, 
然后在这些软件里再安装运行几十个虚拟机系统, 
这样一台强大的服务器就可以为用户提供几十个虚拟系统,让用户选择自己喜欢的系统爱干什么干什么。 
当然每个系统都有远程桌面功能,可以直接给用户提供远程桌面连接,但是这样显得有点原始,而且不利于各种需求的扩展。 
因此就需要提供一整套解决方案,包装起来,让使用者更方便更容易扩展, 
比如一个使用者登录一个桌面进行操作,其他用户同样登录到这个桌面看这位使用者的操作过程,这有点像远程办公或者培训之类的需求。 
类似各种各样的需求,基本目的是让使用者在各种终端设备上使用,而且感觉在使用自家的PC电脑一样。 
回到正题,举个容易理解的例子vmware,我们在使用vmware安装虚拟机系统比如 windows10, 
这个虚拟机系统windows10的桌面都会最终显示到vmware所在的宿主机上的一个窗口里边。这里边主要就是虚拟显卡的功劳。 
vmware专门为虚拟windows系统提供了一个叫“VMware SVGA 3D”的虚拟显卡(不同的vmware版本,名字可能不一样)。 
安装在vmware虚拟机里边的windows系统的显示数据,都会朝VMWARE SVGA 3D的显卡绘画图像, 
结果这块虚拟显卡接收到虚拟机的所有桌面图像,把图像数据通过某种手段(比如共享内存)共享给宿主机上的vmware管理软件。 
然后vmware管理软件就在某个窗口中把图像还原出来,从而我们就能看到虚拟机里边的windows系统的桌面内容了。 
而在云桌面实现中,我们只要把虚拟显卡共享给宿主机的图像数据截获下来 
(每种虚拟机都提供了专门的API函数提供每个虚拟机图像的截获, 
而且能安全和毫无遗漏的获取,不会像windows平台自身的桌面截获那么让人不放心) 
再通过某种压缩算法,一般采用H264压缩(或者改进后的压缩算法), 
通过网络(可以用公开的网络协议,或者私有协议,这个随你的兴趣)传递给客户端。 
因为H264压缩质量和压缩率都很高,如果服务器机器足够强大, 
基本上拥有10M带宽的客户端就可以流畅的观看高清视频和玩游戏,视频和游戏都流畅了,其他办公操作自然就不在话下。 
因此云桌面能实现,另一个核心组件就是先进的图像的压缩算法。 
只有把基本和核心的桌面显示解决流畅了,才能有接下来的音频重定向,鼠标键盘重定向,USB设备重定向等内容。 
自然我们在自己的普通远程桌面中利用H264压缩图像,保证效果也很好,而且还能超过VNC之类的远程控制软件, 
能提供流畅的远程高清视频观看,可惜我无法获取全屏模式游戏图像数据,无法知道全屏游戏远程之后是啥感觉。 
说的有点远,回到我们的windows截屏上来。 
虽说GDI截屏占用CPU有点高,但是大部分场合下使用它,已经足够了。 
在我的电脑上,CPU是 I5-3320M,购买于5年前,显卡是集成的, 
WIN7 64位平台,以每秒25帧固定频率使用GDI函数抓屏,CPU占用率在 2%-4%之间波动。 
5年前电脑的CPU占用都不高,可想而知现在的电脑CPU估计更不是问题。 
然后再以 x264开源库编码成 H264(当然先使用小块扫描办法确定屏幕图像数据有没有更新,没更新则不压缩), 
进行一般的操作,就是打开文档,看看文章,写些文档什么的,整个从抓屏到压缩,CPU使用率在 10%左右, 
如果再打开视频远程端流畅的观看视频,抓屏和x264压缩的CPU占用大概飚升到20-30%甚至更高。 
1600X900,全屏下远程视频大概 500KB/s 字节不到的网络速度,根据不同的H264压缩参数这个速度会有所不同。 
GDI抓屏函数使用伪代码如下: 
   HDC hdc = GetDC(NULL); //屏幕DC 
   HDC memdc = CreateCompatibleDC(hdc);//兼容内存DC 
   BITMAPINFOHEADER bi; 
   ....//设置位图参数  
   byte* rgb_buffer; //CreateDIBSection成功后,rgb_buffer指向存储图像RGB原始数据的指针,可以从这个内存区直接读取图像数据; 
   HBITMAP hbmp = CreateDIBSection(hdc, (BITMAPINFO*)&;bi, DIB_RGB_COLORS, (void**)&;rgb_buffer, NULL, 0);
   SelectObject(memdc,hbmp);// 把位图和memdc关联起来 
至此,初始化操作就完成了。 
定时截屏: 
   HDC hdc = GetDC(NULL); 
    BitBlt( memdc, 0, 0, ScreenX, ScreenY, hdc, 0, 0, SRCCOPY | CAPTUREBLT); // 屏幕DC翻转到memdc 
    ReleaseDC(NULL, hdc); 
然后就直接从 rgb_buffer指向的内存区读取图像数据。 
使用起来是非常的方便和简洁,光从这个截屏方法来说是非常值得称赞的,可以跟某些linux函数的简洁性媲美了。 
这里需要注意一点是  CAPTUREBLT 参数,要成功截取 layered windows, 必须设置这个参数,否则将有一大部分透明窗口截取不到, 
但是使用这参数,频繁截屏时候(比如每秒25帧)鼠标闪烁的比较厉害,基本没有好的解决鼠标闪烁的办法。 
win8, win10之类的系统DXGI截屏伪代码如下: 
   ID3D11Device*             d11dev;       ///D3D设备对象 
   ID3D11DeviceContext*      d11ctx;   ///对象上下文 
   IDXGIOutputDuplication*   dxgi_dup;///这个就是我们需要的采集屏幕数据的接口 
   D3D11CreateDevice(.... ..., &;d11dev, ... &;d11ctx...); //调用D3D11CreateDevice 创建设备对象 
   IDXGIDevice* dxdev = 0; 
   QueryInterface(__uuidof(IDXGIDevice), (void**)&;dxdev); 
   IDXGIAdapter* DxgiAdapter = 0; 
   dxdev->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&;DxgiAdapter)); 
   IDXGIOutput* DxgiOutput = 0; 
   DxgiAdapter->EnumOutputs(nOutput, &;DxgiOutput); 
   IDXGIOutput1* DxgiOutput1 = 0; 
   DxgiOutput->QueryInterface(__uuidof(IDXGIOutput1), reinterpret_cast<void**>(&;DxgiOutput1)); 
   DxgiOutput1->DuplicateOutput(d11dev, &;dxgi_dup);    ///经过上边啰嗦的步骤,终于获取到我们需要的IDXGIOutputDuplication接口 
至此初始化大致完成。 
定时截屏: 
    IDXGIResource* DesktopResource = 0; 
    DXGI_OUTDUPL_FRAME_INFO FrameInfo; 
    ///这个比较奇特,当我们调用AcquireNextFrame获取一帧图像,之后肯定要进行压缩传输等处理,这中间会花去不少时间 
    ///这段时间电脑屏幕肯定有变化,为了保证AcquireNextFrame之后的获取的图像数据不变, 
    ///因此我们不能调用 AcquireNextFrame之后就调用ReleaseFrame释放,而是在下次AcquireNextFrame之前再ReleaseFrame释放。 
    dxgi_dup->ReleaseFrame(); //先释放,然后再获取,保证释放和获取间隔最短 
    dxgi_dup->AcquireNextFrame( 0, &;FrameInfo, &;DesktopResource);  ///截取屏幕数据,但是还不能直接访问原始数据, 
    ID3D11Texture2D* image2d = 0; 
    DesktopResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&;image2d);    //获取纹理2D 
    ID3D11Texture2D* dxgi_text2的;                  //创建一个新的2D纹理对象,用于把 image2d的数据copy进去 
    d11dev->CreateTexture2D(&;frameDescriptor, NULL, &;dxgi_text2d); 
    d11ctx->CopyResource( dxgi_text2d, image2d); ///获取整个帧的数据 
    IDXGISurface* dxgi_surf;          // 获取这个dxgi_text2d的表面 
    dxgi_text2d->QueryInterface(__uuidof(IDXGISurface), (void**)&;dxgi_surf); /// 
    DXGI_MAPPED_RECT mappedRect; 
    dxgi_surf->Map(&;mappedRect, DXGI_MAP_READ);  /////映射锁定表面,从而获取表面的数据地址 
    这个时候 mappedRect.pBits 指向的内存就是原始的图像数据,因为DXGI固定为 32位深度色, 
    因此 mappedRect.pBits 指向的就是 BGRA 元数组。 
    通过判断 FrameInfo里边的 TotalMetadataBufferSize 确定图像有没发生局部变化, 
    然后调用 GetFrameDirtyRects等函数获取脏矩形区域。 
    然而我在实际使用中,发现不太好用,估计是没用对, 
    在win8上使用 GetFrameDirtyRects 正常,到win10 使用GetFrameDirtyRects获取的脏矩形区域,远端显示的结果是乱七八糟的。 
    因此最终还是只获取整个一帧屏幕数据之后, 
    采用前后缓存对比办法获取变化的矩形区域,用小块128X8矩形块对比扫描的办法获取脏矩形区域。 
下篇继续讲解MirrorDriver驱动开发, 
当初为了兴趣研究了MirrorDriver驱动,结果为了测试MirrorDriver驱动,需要实现一个简单的远程桌面, 
当实现远程桌面之后,发现使用H264编码压缩图像,能达到一个非常理想的效果,比起经常使用的VNC更流畅,还能流畅看视频。
因此决定继续远程桌面开发,把windows屏幕的抓取办法扩展到 DXGI, GDI,MirrorDriver三种办法,互补长短。 
记得这也是大学刚毕业时候,非常热衷的事情,记得当时使用 GDI函数抓屏的办法实现了一个粗浅的远程桌面控制程序, 
当时电脑配置不够,当时的热情高涨归热情高涨,毕竟技术水平还不够,因此传输的图像效果很差而且很占网络带宽。 
敬请关注稍后公布到CSDN上的DXGI,GID,Mirrordriver抓屏代码, 最后来张图片, 
这个是在windows抓取到屏幕数据之后, 
简单的压缩成jpeg图片,通过 HTTP协议头的multipart/x-mixed-replace ,实现连续不断的传递JPEG图片, 
然后在手机的浏览器中打开连接就看到远程桌面效果了。 
这是最快最简单实现直播流效果了,当然也很简陋占用带宽也高,是MJPG摄像头网络传输的一种方式。 
稍后公布的接口代码中简单的实现了这部分功能。 
![windows远程桌面实现之一 （抓屏技术总览 MirrorDriver，DXGI，GDI)_Windows](http://aliyunzixunbucket.oss-cn-beijing.aliyuncs.com/jpg/d8dcdd96ee858503b53afb2e4ed24f08.jpg?x-oss-process=image/resize,p_100/auto-orient,1/quality,q_90/format,jpg/watermark,image_eXVuY2VzaGk=,t_100,g_se,x_0,y_0)
