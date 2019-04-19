# OpenGL ES入门09-GLSL实现常见特效 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年03月03日 17:28:00[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1837
#### 本文转自简书，原文地址[http://www.jianshu.com/p/e4a8c83cd373](http://www.jianshu.com/p/e4a8c83cd373)
#### 前言
本文是关于OpenGL ES的系统性学习过程，记录了自己在学习OpenGL ES时的收获。
这篇文章的目标是用OpenGL ES 2.0实现常见的特效。
环境是Xcode8.1+OpenGL ES 2.0
目前代码已经放到github上面，[OpenGL ES入门09-GLSL实现常见特效](https://github.com/QinminiOS/OpenGLES)
###### 欢迎关注我的 [OpenGL ES入门专题](http://www.jianshu.com/c/30e2e76bc140)
#### 概述
这篇文章主要是使用OpenGL ES2.0实现一些常见的滤镜特效：1、灰度图；2、图像颠倒；3、图像漩涡；4、马赛克效果；5、浮雕效果。这些效果不是很难，但是对于学习GLSL是很有帮助的。
#### 灰度图
任何颜色都有红、绿、蓝三原色组成，假如原来某点的颜色为RGB(R，G，B)，那么，我们可以通过下面几种方法，将其转换为灰度：
1.浮点算法：Gray=R*0.3+G*0.59+B*0.11
2.整数方法：Gray=(R*30+G*59+B*11)/100
3.移位方法：Gray =(R*76+G*151+B*28)>>8;
4.平均值法：Gray=(R+G+B)/3;
5.仅取绿色：Gray=G；
通过上述任一种方法求得Gray后，将原来的RGB(R,G,B)中的R,G,B统一用Gray替换，形成新的颜色RGB(Gray,Gray,Gray)，用它替换原来的RGB(R,G,B)就是灰度图了。在这里我们使用GPUImage中使用的变换因子：
`const highp vec3 W = vec3(0.2125, 0.7154, 0.0721);`
![](http://upload-images.jianshu.io/upload_images/1860319-28ee5e3d3cc69461.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
灰度图.png
```
//灰度图
precision highp float;
uniform sampler2D image;
varying vec2 vTexcoord;
const highp vec3 W = vec3(0.2125, 0.7154, 0.0721);
void main()
{
    lowp vec4 textureColor = texture2D(image, vTexcoord);
    float luminance = dot(textureColor.rgb, W);
    gl_FragColor = vec4(vec3(luminance), textureColor.a);
}
```
#### 图像颠倒
图像颠倒实现比较简单，在文理采样的时候我们只需要反转UV坐标，便可以实现图像颠倒的效果。
![](http://upload-images.jianshu.io/upload_images/1860319-28e54764033e1e35.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
图像颠倒.png
```
precision mediump float;
varying vec2 vTexcoord;
uniform sampler2D image;
void main()
{
    vec4 color = texture2D(image, vec2(vTexcoord.x, 1.0 - vTexcoord.y));
    gl_FragColor = color;
}
```
#### 图像漩涡
图像漩涡主要是在某个半径范围里，把当前采样点旋转一定角度，旋转以后当前点的颜色就被旋转后的点的颜色代替，因此整个半径范围里会有旋转的效果。如果旋转的时候旋转角度随着当前点离半径的距离递减，整个图像就会出现漩涡效果。这里使用的了抛物线递减因子：(1.0-(r/Radius)*(r/Radius) )。
![](http://upload-images.jianshu.io/upload_images/1860319-7902d709aa923bc5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
漩涡.png
```
precision mediump float;
const float PI = 3.14159265;
uniform sampler2D image;
const float uD = 80.0; //旋转角度
const float uR = 0.5; //旋转半径
varying vec2 vTexcoord;
void main()
{
    ivec2 ires = ivec2(512, 512);
    float Res = float(ires.s);
    vec2 st = vTexcoord;
    float Radius = Res * uR;
    vec2 xy = Res * st;
    vec2 dxy = xy - vec2(Res/2., Res/2.);
    float r = length(dxy);
    float beta = atan(dxy.y, dxy.x) + radians(uD) * 2.0 * (-(r/Radius)*(r/Radius) + 1.0);//(1.0 - r/Radius);
    vec2 xy1 = xy;
    if(r<=Radius)
    {
        xy1 = Res/2. + r*vec2(cos(beta), sin(beta));
    }
    st = xy1/Res;
    vec3 irgb = texture2D(image, st).rgb;
    gl_FragColor = vec4( irgb, 1.0 );
}
```
#### 马赛克
马赛克效果就是把图片的一个相当大小的区域用同一个点的颜色来表示.可以认为是大规模的降低图像的分辨率,而让图像的一些细节隐藏起来。
![](http://upload-images.jianshu.io/upload_images/1860319-ab94ead6981ed2d5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
马赛克.png
```
precision mediump float;
varying vec2 vTexcoord;
uniform sampler2D image;
const vec2 TexSize = vec2(400.0, 400.0);
const vec2 mosaicSize = vec2(8.0, 8.0);
void main()
{
    vec2 intXY = vec2(vTexcoord.x*TexSize.x, vTexcoord.y*TexSize.y);
    vec2 XYMosaic = vec2(floor(intXY.x/mosaicSize.x)*mosaicSize.x, floor(intXY.y/mosaicSize.y)*mosaicSize.y);
    vec2 UVMosaic = vec2(XYMosaic.x/TexSize.x, XYMosaic.y/TexSize.y);
    vec4 color = texture2D(image, UVMosaic);
    gl_FragColor = color;
}
```
#### 浮雕效果
浮雕效果是指图像的前景前向凸出背景。实现思路：把图象的一个象素和左上方的象素进行求差运算，并加上一个灰度。这个灰度就是表示背景颜色。这里我们设置这个插值为128 (图象RGB的值是0-255)。同时,我们还应该把这两个颜色的差值转换为亮度信息，避免浮雕图像出现彩色像素。
![](http://upload-images.jianshu.io/upload_images/1860319-73d62d5981ccca41.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
浮雕.png
```
//浮雕效果
precision mediump float;
varying vec2 vTexcoord;
uniform sampler2D image;
const highp vec3 W = vec3(0.2125, 0.7154, 0.0721);
const vec2 TexSize = vec2(100.0, 100.0);
const vec4 bkColor = vec4(0.5, 0.5, 0.5, 1.0);
void main()
{
    vec2 tex = vTexcoord;
    vec2 upLeftUV = vec2(tex.x-1.0/TexSize.x, tex.y-1.0/TexSize.y);
    vec4 curColor = texture2D(image, vTexcoord);
    vec4 upLeftColor = texture2D(image, upLeftUV);
    vec4 delColor = curColor - upLeftColor;
    float luminance = dot(delColor.rgb, W);
    gl_FragColor = vec4(vec3(luminance), 0.0) + bkColor;
}
```
#### 完整代码
```
//
//  OpenGLESView.m
//  OpenGLES01-环境搭建
//
//  Created by qinmin on 2017/2/9.
//  Copyright © 2017年 qinmin. All rights reserved.
//
#import "OpenGLESView.h"
#import <OpenGLES/ES2/gl.h>
#import "GLUtil.h"
#include "JpegUtil.h"
@interface OpenGLESView ()
{
    CAEAGLLayer     *_eaglLayer;
    EAGLContext     *_context;
    GLuint          _colorRenderBuffer;
    GLuint          _frameBuffer;
    GLuint          _program;
    GLuint          _vbo;
    GLuint          _texture;
    GLuint          _texture1;
    int             _vertCount;
}
@end
@implementation OpenGLESView
+ (Class)layerClass
{
    // 只有 [CAEAGLLayer class] 类型的 layer 才支持在其上描绘 OpenGL 内容。
    return [CAEAGLLayer class];
}
- (void)dealloc
{
    glDeleteBuffers(1, &_vbo);
    glDeleteTextures(1, &_texture);
    glDeleteProgram(_program);
}
- (instancetype)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame]) {
        [self setupLayer];
        [self setupContext];
        [self setupGLProgram];
        [self setupVBO];
        [self setupTexure];
        [self setupTexure1];
    }
    return self;
}
- (void)layoutSubviews
{
    [EAGLContext setCurrentContext:_context];
    [self destoryRenderAndFrameBuffer];
    [self setupFrameAndRenderBuffer];
    [self render];
}
#pragma mark - Setup
- (void)setupLayer
{
    _eaglLayer = (CAEAGLLayer*) self.layer;
    // CALayer 默认是透明的，必须将它设为不透明才能让其可见
    _eaglLayer.opaque = YES;
    // 设置描绘属性，在这里设置不维持渲染内容以及颜色格式为 RGBA8
    _eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                     [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
}
- (void)setupContext
{
    // 设置OpenGLES的版本为2.0 当然还可以选择1.0和最新的3.0的版本，以后我们会讲到2.0与3.0的差异，目前为了兼容性选择2.0的版本
    _context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    if (!_context) {
        NSLog(@"Failed to initialize OpenGLES 2.0 context");
        exit(1);
    }
    // 将当前上下文设置为我们创建的上下文
    if (![EAGLContext setCurrentContext:_context]) {
        NSLog(@"Failed to set current OpenGL context");
        exit(1);
    }
}
- (void)setupFrameAndRenderBuffer
{
    glGenRenderbuffers(1, &_colorRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
    // 为 color renderbuffer 分配存储空间
    [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer];
    glGenFramebuffers(1, &_frameBuffer);
    // 设置为当前 framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
    // 将 _colorRenderBuffer 装配到 GL_COLOR_ATTACHMENT0 这个装配点上
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_RENDERBUFFER, _colorRenderBuffer);
}
- (void)setupGLProgram
{
    NSString *vertFile = [[NSBundle mainBundle] pathForResource:@"vert" ofType:@"glsl"];
    NSString *fragFile = [[NSBundle mainBundle] pathForResource:@"cameo" ofType:@"glsl"];
    _program = createGLProgramFromFile(vertFile.UTF8String, fragFile.UTF8String);
    glUseProgram(_program);
}
- (void)setupVBO
{
    _vertCount = 6;
    GLfloat vertices[] = {
        0.8f,  0.6f, 0.0f, 1.0f, 0.0f,   // 右上
        0.8f, -0.6f, 0.0f, 1.0f, 1.0f,   // 右下
        -0.8f, -0.6f, 0.0f, 0.0f, 1.0f,  // 左下
        -0.8f, -0.6f, 0.0f, 0.0f, 1.0f,  // 左下
        -0.8f,  0.6f, 0.0f, 0.0f, 0.0f,  // 左上
        0.8f,  0.6f, 0.0f, 1.0f, 0.0f,   // 右上
    };
    // 创建VBO
    _vbo = createVBO(GL_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(vertices), vertices);
    glEnableVertexAttribArray(glGetAttribLocation(_program, "position"));
    glVertexAttribPointer(glGetAttribLocation(_program, "position"), 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, NULL);
    glEnableVertexAttribArray(glGetAttribLocation(_program, "texcoord"));
    glVertexAttribPointer(glGetAttribLocation(_program, "texcoord"), 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*5, NULL+sizeof(GL_FLOAT)*3);
}
- (void)setupTexure
{
    NSString *path = [[NSBundle mainBundle] pathForResource:@"1" ofType:@"jpg"];
    unsigned char *data;
    int size;
    int width;
    int height;
    // 加载纹理
    if (read_jpeg_file(path.UTF8String, &data, &size, &width, &height) < 0) {
        printf("%s\n", "decode fail");
    }
    // 创建纹理
    _texture = createTexture2D(GL_RGB, width, height, data);
    if (data) {
        free(data);
        data = NULL;
    }
}
- (void)setupTexure1
{
    NSString *path = [[NSBundle mainBundle] pathForResource:@"noise" ofType:@"jpg"];
    unsigned char *data;
    int size;
    int width;
    int height;
    // 加载纹理
    if (read_jpeg_file(path.UTF8String, &data, &size, &width, &height) < 0) {
        printf("%s\n", "decode fail");
    }
    // 创建纹理
    _texture1 = createTexture2D(GL_RGB, width, height, data);
    if (data) {
        free(data);
        data = NULL;
    }
}
#pragma mark - Clean
- (void)destoryRenderAndFrameBuffer
{
    glDeleteFramebuffers(1, &_frameBuffer);
    _frameBuffer = 0;
    glDeleteRenderbuffers(1, &_colorRenderBuffer);
    _colorRenderBuffer = 0;
}
#pragma mark - Render
- (void)render
{
    glClearColor(1.0, 1.0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(2.0);
    glViewport(0, 0, self.frame.size.width, self.frame.size.height);
    // 激活纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glUniform1i(glGetUniformLocation(_program, "image"), 0);
    // 激活纹理
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _texture1);
    glUniform1i(glGetUniformLocation(_program, "image1"), 1);
    glDrawArrays(GL_TRIANGLES, 0, _vertCount);
    //将指定 renderbuffer 呈现在屏幕上，在这里我们指定的是前面已经绑定为当前 renderbuffer 的那个，在 renderbuffer 可以被呈现之前，必须调用renderbufferStorage:fromDrawable: 为之分配存储空间。
    [_context presentRenderbuffer:GL_RENDERBUFFER];
}
@end
```
#### 参考资料
[GPUImage](https://github.com/BradLarson/GPUImage)
