# android竖屏显示 - xqhrs232的专栏 - CSDN博客
2012年02月18日 12:54:49[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：571
原文地址::[http://blog.csdn.net/knock/article/details/5335767](http://blog.csdn.net/knock/article/details/5335767)
本文原创，来自[http://blog.csdn.net/knock](http://blog.csdn.net/knock)，转载请保留本行
屏是LANDSCAPE的，要让它默认显示为PORTRAIT.
1.kernel里要旋转FrameBuffer.
  启动参数里加入fbcon=rotate:1    (0:正常屏; 1:顺时钟转90度; 2:转180度; 3:顺时钟转270度;)
最后生成的autoconf.h里有类似项：
#define CONFIG_CMDLINE "console=ttySAC0,115200 fbcon=rotate:1"
此项的解析在$(kernel)/drivers/video/console/fbcon.c
static int __init fb_console_setup(char *this_opt);
只是去初始化变量initial_rotation，然后initial_rotation会传递给其他需要的结构。
注意：参考$(kernel)/documentation/fb/fbcon.txt
2.android OS旋转屏幕
系统默认是针对竖屏的，而MID使用的是横屏，所以需要做一个转换的动作。
PORTRAIT               LANDSCAPE         <------屏幕显示方式
ROTATION_0             ROTATION_90
ROTATION_90        ROTATION_180
ROTATION_180        ROTATION_270
ROTATION_270        ROTATION_0
而source code里对ROTATION_180和ROTATION_270的处理比较少，只在sensor和KeyQueue部分，所以如果只是要让系统显示为竖屏，将android中的Surface.ROTATION_0改为Surface.ROTATION_90，而Surface.ROTATION_90改为Surface.ROTATION_0。 这样，启动后的屏幕就是竖屏的了。
改动后，启动时还是LANDSCAPE显示的，进入HOME也是，很快就会自动旋转到PORTRAIT模式，这是由于
$(cupcake)/frameworks/base/services/java/com/android/server/WindowManagerService.java
中enableScreenAfterBoot()->performEnableScreen()->mPolicy.enableScreenAfterBoot(), mPolicy为父类指针，可以指向
PhoneWindowManager或者MidWindowManager，由配置文件$(cupcake)/build/target/product/core.mk中
PRODUCT_POLICY := android.policy_phone
//PRODUCT_POLICY := android.policy_mid
来指定。
PhoneWindowManager::enableScreenAfterBoot()->updateRotation(Surface.FLAGS_ORIENTATION_ANIMATION_DISABLE)->mWindowManager.setRotation()完成设置旋转并清除LOGO.
3.启动过程中竖屏
启动过程中，默认是按照屏的width和height显示的，不会旋转，要使它显示logo时就是竖屏的，也就是旋转90度，需要做如下工作：
$(cupcake)/frameworks/base/libs/surfaceflinger/SurfaceFlinger.cpp
status_t SurfaceFlinger::readyToRun()中
    //const uint32_t w = hw.getWidth();
    //const uint32_t h = hw.getHeight();
//swap w&h for portrait display in landscape panel. jeff.
    const uint32_t h = hw.getWidth();  
    const uint32_t w = hw.getHeight();
交换一下width和height，这样后面用OpenGL创建的ViewPort形状就是竖的了。修改后面的函数参数也可以，不过太多了，交换一下省事。但是怎么让这个竖的viewport旋转90度呢？这里就要用到GraphicPlane::mGlobalTransform这个Transform了。它指示当前最终要旋转的结果。 所以要在创建GraphicPlane时初始化mGlobalTransform为旋转90度。
GraphicPlane::GraphicPlane()
    : mHw(0)
{
//add by jeff. for default rotate angel 90 
 mOrientationTransform.reset();
 mOrientation = ISurfaceComposer::eOrientation90;
 mGlobalTransform = mOrientationTransform * mTransform; 
}
此段从status_t GraphicPlane::setOrientation(int orientation)复制过来，注意修改mGlobalTransform：
    if (orientation == ISurfaceComposer::eOrientation90) { //ISurfaceComposer::eOrientationDefault //jeff
        // make sure the default orientation is optimal
        mOrientationTransform.reset();
        mOrientation = orientation;
        //mGlobalTransform = mTransform;
        mGlobalTransform = mOrientationTransform * mTransform; //jeff
        return NO_ERROR;
    }
注意mOrientationTransform.reset();要修改为默认旋转90度。参照status_t GraphicPlane::orientationToTransfrom
中的设置，修改为：
void Transform::reset() { 
    mTransform.reset();
    mType = 0;
 set(0,-1,1,0);  //jeff
 set(800,0);
}
参考：
status_t GraphicPlane::orientationToTransfrom(
        int orientation, int w, int h, Transform* tr)
{    
    float a, b, c, d, x, y;
    switch (orientation) {
    case ISurfaceComposer::eOrientationDefault:
        a=1; b=0; c=0; d=1; x=0; y=0;
        break;
    case ISurfaceComposer::eOrientation90:
        a=0; b=-1; c=1; d=0; x=w; y=0;
        break;
    case ISurfaceComposer::eOrientation180:
        a=-1; b=0; c=0; d=-1; x=w; y=h;
        break;
    case ISurfaceComposer::eOrientation270:
        a=0; b=1; c=-1; d=0; x=0; y=h;
        break;
    default:
        return BAD_VALUE;
    }
    tr->set(a, b, c, d);
    tr->set(x, y);
    return NO_ERROR;
}
修改之后，默认就是竖屏（旋转90度）显示了。
