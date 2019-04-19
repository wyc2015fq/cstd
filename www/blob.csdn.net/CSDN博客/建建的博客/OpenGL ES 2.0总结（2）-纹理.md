# OpenGL ES 2.0总结（2）-纹理 - 建建的博客 - CSDN博客
2018年03月19日 14:54:21[纪建](https://me.csdn.net/u013898698)阅读数：51
个人分类：[openGL](https://blog.csdn.net/u013898698/article/category/7028208)
## 1. 纹理初始化
- 1) glGenTextures()
生成纹理id，可以一次生成多个，后续操作纹理全靠这个id- 2) glBindTexture()
操作纹理，传入纹理id作为参数，每次bind之后，后续操作的纹理都是该纹理- 3) glTexParameteri()
指定纹理格式。这里包括纹理横向和纵向的重复方式 
GL_TEXTURE_WRAP_S 
GL_TEXTURE_WRAP_T
和纹理在放大和缩小（同样纹理离远和离近）时的处理，这种设置主要是为了避免同一个纹理反复使用时，远处的纹理反而比近处的清晰 
GL_TEXTURE_MAG_FILTER 
GL_TEXTURE_MIN_FILTER
- 4) GLUtils.texImage2D()
给纹理传入图像数据，至此，此纹理相关设置已经结束。后续想使用或者操作这个纹理，只要再glBindTexture这个纹理的id即可.
下面给一个较为完整的例子
```java
int[] textureHandle = new int[1];
GLES20.glGenTextures(1, textureHandle, 0);
int TextureID = textureHandle[0];
GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, TextureID);
GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
        GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap, 0);
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
## 2. 纹理单元
OpenGL可以同时使用多个纹理，这些纹理可以分别绑定到特定的纹理单元。每个纹理单元有其独立的图像、Filter参数等。
void glActiveTexture(GLenum texUnit)激活对应的纹理单元，之后所有的纹理操作都是针对这个纹理单元。参数GL_TEXTUREi，i取值0至k，最大值跟硬件相关。
绑定纹理到纹理单元举例:
```java
glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE2D,tex1);
   glActiveTexture(GL_TEXTURE3);
   glBindTexture(GL_TEXTURE2D,tex2);
```
- 1
- 2
- 3
- 4
此时GL_TEXTURE0(默认纹理单元)下绑定的是tex1，GL_TEXTURE3下绑定的是tex2
使用时的语句完全相同。在激活和绑定其他纹理之前，使用的都是该纹理单元和纹理。
## 3. 多重纹理与shader
- 使用多个纹理时，主要分以下几步
- 1）首先需要编译好shader。
shader中的fragment.glsl中应该包括多个纹理的采样器
例如
```
uniform sampler2D Map1;
    uniform sampler2D Map2;
```
- 1
- 2
- 2）在代码中获取到shader中改sampler的句柄
相当于建立起了代码和shader之间的通道，通过句柄可以给shader中的sampler赋值
```java
mMap1= GLES20.glGetUniformLocation(Program, "Map1");
mMap2= GLES20.glGetUniformLocation(Program, "Map2");
```
- 1
- 2
- 3）后设置纹理单元与shader中sampler的关系
通过赋值，可以指定sampler与纹理单元的关系，想让sampler对哪个纹理单元GL_TEXTUREi中的纹理进行采样/处理，就给它赋值i，如果纹理是GL_TEXTURE0，就给sampler2D赋值为0，以此类推。
```java
GLES20.glUniform1i(mMap1,0);
GLES20.glUniform1i(mMap2,3);
```
- 1
- 2
这里指定完毕之后，后续shader中的Map1的采样对象就是GL_TEXTURE0上绑定的纹理，而Map2的处理对象就是GL_TEXTURE3上绑定的纹理
- 4）绑定目标纹理
要使用相应的纹理，步骤和“2.纹理单元”中描述的完全一样（前提是这些纹理以已经完成了初始化），就是：
```java
glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE2D,tex1);
   glActiveTexture(GL_TEXTURE3);
   glBindTexture(GL_TEXTURE2D,tex2);
```
- 1
- 2
- 3
- 4
此时GL_TEXTURE0上绑定的纹理是tex1，对应在shader中的sampler是Map1；GL_TEXTURE3上绑定的纹理是tex2，对应在shader中的sampler是Map2
- 4）使用shader处理
通过上述操作，直接在shader里调用vec4 texture2D（sampler，vec2）即可取到相应的像素值并做处理，例如
```
vec4 color1 = texture2D(Map1,texCoord);
    vec4 color2 = texture2D(Map2,texCoord);
```
- 1
- 2
> 
注意，返回的是vec4，无论纹理像素是什么格式的，都返回vec4类型，具体如下：
|纹理图像类型|返回值|
|----|----|
|GL_RGB|(R,G,B,1.0)|
|GL_RGBA|(R,G,B,A)|
|GL_LUMINANCE|(L,L,L,1.0)|
|GL_LUMINANCE_ALPHA|(L,L,L,A)|
|GL_ALPHA|(0.0, 0.0, 0.0, A)|
- 参考
opengles2.0中的纹理 
[http://blog.csdn.net/ldpxxx/article/details/9253603](http://blog.csdn.net/ldpxxx/article/details/9253603)
gles官方文档 
[https://www.khronos.org/opengles/sdk/docs/man/](https://www.khronos.org/opengles/sdk/docs/man/)
OpenGL ES2.0 生成Mipmap纹理 
[http://xiaxveliang.blog.163.com/blog/static/297080342013467552467/](http://xiaxveliang.blog.163.com/blog/static/297080342013467552467/)
