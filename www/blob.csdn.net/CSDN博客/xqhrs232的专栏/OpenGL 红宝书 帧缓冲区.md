# OpenGL 红宝书 帧缓冲区 - xqhrs232的专栏 - CSDN博客
2011年07月03日 15:14:43[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：924
原文地址::[http://www.cppblog.com/summericeyl/archive/2009/05/01/81613.html](http://www.cppblog.com/summericeyl/archive/2009/05/01/81613.html)
# **第10章 帧缓存**
在OpenGL窗口中, 左下角的像素为(0, 0). 一般而言, 像素(x, y)占据的矩形区域左下角为(x, y), 右上角为(x+1, y+1).
## **10.1 缓存及其用途**
1) 颜色缓存,  左前,右前,左后,右后和任意数量的辅助颜色缓存;
2) 深度缓存
3) 模板缓存
4) 累积缓存 
注意:X窗口系统,RGBA模式至少保证1个颜色缓冲区,模板缓冲区,深度缓冲区,累计缓冲区
颜色索引模式至少保证1个眼色缓冲区,深度缓冲区,模板缓冲区.
可以使用glXGetConfig{}函数查询. 
用glGetIntegerv()查询每个像素占据的缓存空间的参数
GL_RED_BITS, GL_GREEN_BITS, GL_BLUE_BITS, GL_ALPHA_BITS --- 颜色缓存中R, G, B, A分量的位数
GL_INDEX_BITS --- 颜色缓存中每个颜色索引的位数
GL_DEPTH_BITS --- 深度缓存中每个像素的位数
GL_STENCIL_BITS --- 模板缓存中每个像素的位数
GL_ACCUM_RED_BITS, GL_ACCUM_GREEN_BITS, GL_ACCUM_BLUE_BITS, GL_ACCUM_ALPHA_BITS --- 累积缓存中R, G, B, A分量的位数.
### **10.1.1 颜色缓存**
1) 颜色缓存存储了颜色索引或RGB颜色数据, 还可能存储了alpha值. 
2) 支持立体观察(stereoscopic viewing)的OpenGL实现有左颜色缓存和右颜色缓存. 它们分别用于左立体图像和右立体图像.
3) 如不支持立体观察, 则只使用左颜色缓存.
4) 双颜色缓存有前缓存和后缓存, 单缓存系统只有前缓存.
5) 支持不可显示的辅助颜色缓存
6) 函数glGetBooleanv()查询是否支持立体观察和双缓存: GL_STEREO和GL_DOUBLE_BUFFER. 
   函数glGetIntegerv()查询多少个辅助缓存可用: GL_AUX_BUFFERES. 
### **10.1.2 深度缓存**
深度缓存 --- 存储了每个像素的深度值. 通常是离视点的距离, 因此深度值大的像素会被深度值小的像素覆盖. 
### **10.1.3 模板缓存**
用途之一: 绘图范围限制在屏幕的特定区域内. 
### **10.1.4 累积缓存**
累积缓存也存储RGBA颜色数据, 将一系列的图像合成一幅图像. 
可以对图像进行超量采样, 然后对样本进行平均, 并将结果写入颜色缓存中, 从而实现场景反走样. 不能将数据直接写入累积缓存.
累加操作总是针对一个矩形块的, 通常是将数据移入或移出颜色缓存. 
### **10.1.5 清空缓存**
void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void glClearIndex(GLfloat index);
void glClearDepth(GLclampd depth);
void glClearStencil(GLint s);
void glClearAccum(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
功能: 指定用于清除颜色缓存(RGBA模式或颜色索引模式), 深度缓存, 模板缓存和累积缓存的值. 
类型为GLclampf和GLclampd的参数值被截取到[0.0, 1.0]. 深度缓存的默认清除值为1.0, 其他缓存为0.0. 
设置清除值后, 便可以调用函数glClear()来清除缓存.
void glClear(GLbitfield mask);
功能: 清除指定的缓存. 
mask:GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT, GL_ACCUM_BUFFER_BIT的逻辑或(OR).
清除颜色缓存时, 如果启用了像素所有权测试, 裁剪测试和抖动操作, 它们都会在清除操作中执行. 
屏蔽操作(glColorMask()和glIndexMask())也会生效.alpha测试, 模版测试, 深度测试并不会影响glClear()函数的操作. 
### **10.1.6 指定要读写的颜色缓存**
指定要写入的缓存 glDrawBuffer();
指定要读取的缓存 glReadBuffer(); 
使用双缓存, 通常只绘制后缓存, 并在绘制完成后交换缓存. 你可能想将双缓存窗口视为单缓存窗口: 通过调用函数glDrawBuffer()使得可以同时绘制前缓存和后缓存.
**void glDrawBuffer(GLenum mode);**
功能: 指定要写入或消除的颜色缓存以及禁用之前被启用的颜色缓存. 可以一次性启用多个缓存.
GL_FRONT: 单缓存的默认值    
GL_FRONT_RIGHT:    
GL_NONE:
GL_FRONT_LEFT:
GL_FRONT_AND_BACK:
GL_RIGHT:
GL_AUXi: i表示第几个辅助缓存.
GL_LEFT:
GL_BACK_RIGHT:
GL_BACK: 双缓存的默认值
GL_BACK_LEFT:
注意: 启用多个缓存用于写操作时, 只要其中一个缓存存在, 就不会发生错误. 如果指定的缓存都不存在, 就发生错误. 
**void glReadBuffer(GLenum mode);**
功能: 选择接下来的函数调用glReadPixels(), glCopyPixels(), glCopyTexImage*(), glCopyTexSubImage*() 和 glCopyConvolutionFilter*()将读取的缓存.
      并启用以前被函数glReadBuffer()启用的缓存.
参数mode取值:
GL_FRONT: 单缓存默认
GL_FRONT_RIGHT:
GL_BACK_RIGHT:
GL_FRONT_LEFT:
GL_LEFT:
GL_AUX:
GL_BACK_LEFT:
GL_BACK: 双缓存默认
GL_RIGHT:
注意: 启用缓存用于读取操作时, 指定的缓存必须存在, 否则将发生错误. 
### **10.1.7 屏蔽缓存**
**void glIndexMask(GLuint mask);void glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);void glDepthMask(GLboolean flag);void glStencilMask(GLuint mask);**
功能: 设置掩码, 用于控制写入到指定缓存中的数据,
glIndexMask: 只用于颜色索引模式中, 掩码中1对应的数据位被写入颜色索引缓存中. 0对应的位不写入.
glColorMask: 只影响RGBA模式下的写入, red, green, blue, alpha决定是否写入相应的分量, GL_TRUE时写入.
glDepthMask(): 如果参数flag的值为GL_TRUE, 则启用深度缓存用于写入, 否则禁用深度缓存.
glStencilMask(): 参数mask的含义与函数glIndexMask()中相同. 
所有GLboolean参数的默认值是GL_TRUE, 两个GLuint参数的默认值都是1. 
禁用深度缓存: 如果背景很复杂, 则在背景绘制之后, 禁用深度缓存. 绘制新增的物体, 只要不相互重叠.. 下一帧时, 只需恢复树木图像, 无需恢复深度缓存中的值.
这种方法很有效. 
模板缓存的屏蔽操作让你能够使用一个多位模板缓存来存储多个模板(每位一个)
函数glStencilMask()指定的掩码用于控制哪些模板位面可写, 与函数glStencileFunc()的第三个参数指定的掩码无关, 后者指定模板函数将考虑哪些位面.
## **10.2 片段测试和操作**
测试顺序:
1. 裁剪测试 
2. alpha测试 
3. 模版测试
4. 深度测试
5. 混合
6. 抖动
7. 逻辑操作 
### **10.2.1 裁剪测试**
**void glScissor(GLint x, GLint y, GLsizei width, GLsizei height);**设置裁剪矩形的位置和大小. 需要启用GL_SCISSOR_TEST. 
### **10.2.2 alpha测试**
需要启用GL_ALPHA_TEST
**void glAlphaFunc(GLenum func, GLclampf ref);**
设置用于alpha测试的参考值和比较函数.
alpha测试可用于透明算法和贴花. 
### 10.2.3 模板测试
**void glStencilFunc(GLenum func, GLint ref, GLuint mask);void glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask);**
设置模板测试所使用的比较函数(func),参考值(ref),掩码(mask) 
**void glStencilOp(GLenum fail, GLenum zfail, GLenum zpass);void glStencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass);**指定当一个片段通过或未通过模板测试时, 模板缓冲区中的数据如何进行修改. 
下图设置模板为中间的方块 
![11](https://tntb8a.bay.livefilestore.com/y1mM1AmudqgcEDPVhuc8mgnt3frmD3fikNeCjgWFfQmE5iYMpYtZZleMYZVlTCfTdh2lI9Yw1nPO-MkPM3HCqyqQV8Q-0FgzTka52iQqLZlyPGKADWcqg44DPfMoemfcw6gBQ8IgCdDkwMEY0x08w92jg/11_thumb.jpg)
模板查询glGetInteger()可使用的参数:
GL_STENCIL_FUNC
GL_STENCIL_REF
GL_STENCIL_VALUE_MASK
GL_STENCIL_FAIL
GL_STENCIL_PASS_DEPTH_FAIL
GL_STENCIL_PASS_DEPTH_PASS 
### 10.2.4 深度测试
**void glDepthFunc(GLenum func);**
为深度测试设置比较函数. 
### **10.2.5 遮挡测试**
遮挡测试允许我们判断一组几何图形在进行深度测试之后是否可见.
步骤:
1. 为每个所需的遮挡查询生成一个查询ID
2. 调用glBeginQuery(), 表示开始一个遮挡查询
3. 渲染需要进行遮挡查询的几何图形
4. 调用glEndQuery(), 表示已经完成了遮挡查询
5. 提取通过遮挡查询的样本数量. 
生成查询对象
**void glGenQueries(GLsizei n, GLuint* ids);**
对遮挡查询对象进行初始化
**void glBeginQuery(GLenum target, GLuint id);void glEndQuery(GLenum target);**
target必须为GL_SAMPLES_PASSED. 
判断遮挡查询的结果
**void glGetQueryObjectiv(GLenum id, GLenum pname, GLint *params);void glGetQueryObjectuiv(GLenum id, GLenum pname, GLuint *params);**
清除遮挡查询对象
**void glDeleteQueries(GLsizei n, const GLuint *ids);**
### **10.2.6 混合,抖动和逻辑操作**
**void glLogicOp(GLenum opcode);**
选择需要执行的逻辑操作. 
## **10.3 累计缓冲区**
**void glAccum(GLenum op, GLfloat value);**
控制累积缓冲区
op参数:
GL_ACCUM--用glReadBuffer()所读取的当前缓冲区中读取每个像素.把R,G,B,A值乘以value.而后结果累加到累积缓冲区.
GL_LOAD--同上,只是用结果替换累积缓冲区中的值.
GL_RETURN--累积缓冲区中的值乘以value, 结果放在颜色缓冲区中.
GL_ADD和AL_MULT--累积缓冲区中的值与value相加或者相乘,结果写回累积缓冲区. 另GL_MULT的结果截取[-1.0. 1.0].GL_ADD的结果不截取.
### **10.3.1 场景抗锯齿**
首先清除累积缓冲区, 并启用前缓冲区用于读取和写入.
然后循环执行几次(例如n次)代码, 对图像进行微移和绘制.
对数据进行累积的方法:
glAccum(GL_ACCUM, 1.0/n);    // 绘制到不会显示的颜色缓冲区中, 避免显示中间图像.
并最终调用
glAccum((GL_RETURN, 1.0);    // 绘制到可显示的颜色缓冲区中(或即将交换的后缓冲区). 
可提供一用户接口, 来显示每次图像累积之后所获得的改善, 如图像足够满意, 可随时终止累积. 
本例主要是逐步在窗口累积各颜色分量.
一共累积八次,且其中每次都用j8数组的数据微移场景, 使用glFrustum函数可以是的我们场景不必对称. 
正交投影的偏移只需要用glTranslatef()移动一个像素内的偏移即可. 
下图为没有反锯齿没有用累积缓存的图像 
![2](https://tntb8a.bay.livefilestore.com/y1m3N-ncRLj6egSO92MVxeHwVsxH9Qr2OzFY_DGen8swplvpxAXtDbD1ENwYvWXeYcr7LuiS3RhdMV6ljxWnBE1IbiFdVGTM8BX8GdRti0cLjAwJGqvD6JslbYkfsjTOyXh0jpinpGODbYL9UCEKgr7Zw/2_thumb%5B9%5D.jpg)
下图为使用了累积缓存反锯齿的图像 
![1](https://tntb8a.bay.livefilestore.com/y1mXiT1EE45aD_aaagDO9bSALBqdgGjFD7d4ajZ231uef40npQ1cx9pFJn0I_MCGEr_x3Fmp2RqJU-6pT97I2prvz_T-Ip7AU1KbU36I5N2vEjlxWKiJiQ0ZRQdke8tHAf0P7fnuRsMBwYT72RgwBO7KQ/1_thumb%5B1%5D.jpg)
累积缓存我分步骤显示,看看效果 
![3](https://tntb8a.bay.livefilestore.com/y1mwFkNt58KfxcDkE83bm0-Vq-4TkjFsOVurZF50q0HsNE8kMPz2qdDBYh1ksvB_PKiGjiBZXWEgu2KyyAz1Q5dWXXdt2KZr3he1DzcdBZyDx7XUoMlIh84CI7TuX_NqBfGbNLH9LaUHyKrwfmY5f5UJA/3_thumb%5B1%5D.jpg)![4](https://tntb8a.bay.livefilestore.com/y1mww9jFwgAZag4KIh04eCbxE0JrIuWj_f0-zqEJVbAdCzLGUObi4eNQCCqTEJen9WYG9AlOXe-NVI1knrFuA6tmFdTS1Tu_QvYbK6vTUjNCdWoFpeIJFh3F3s9qnkeGLJrfNra42tIsmU1RGhNgEOxbg/4_thumb%5B1%5D.jpg)![5](https://tntb8a.bay.livefilestore.com/y1mCD7wyXFoyQdkRBapyOkZADBNigKBPB9rAY4HZup0tZ_8-DtouIup4abtZp16AbN-Df_XuBMDpm_aROjf3ae67hXwk4UJ9og1g2KbKOkBIN6-NdvxKsLlm6v6ZwRXDduhDQPFSB2WhQTWumIO75sCyw/5_thumb%5B1%5D.jpg)![6](https://tntb8a.bay.livefilestore.com/y1m9rZS8177acZs0zaJXvFGC0me3lJn2cI5xUYpei1YG13H-NE14VLb4Qn32wGjhaWcKwvQbsVPaI5fJ8MWt9WJPmUA4fMubDc2ra-N9oIFeBonXsUCFYhHgJt93WrnWVU8sqzDVg_uKGYakGOosqUxmQ/6_thumb%5B1%5D.jpg)![7](https://tntb8a.bay.livefilestore.com/y1maQgvFIrdg7ZF3pzaR2pd2w6hkkveNzkxwCxW67bF7E-AHtmUK-Mg9l_cQQA5LOGnZj0LeLmgBU1NEMFOvH5qvbztr4mYe6oXGEkL7qxkV1q10G4PMdzpb1LkT3NY_Oyiir3B-eH2Qnh2ymCMEm30lA/7_thumb%5B1%5D.jpg)
### **10.3.2 运动模糊**
按照相同的方式设置累积缓冲区, 但不是对图像进行空间上的微移, 而是进行时间上的微移.
**glAccum(GL_MULT, decayFactor);**
这样随着场景绘制到累积缓冲区中,整个场景越来越模糊. 其中decayFactor是个0.0到1.0之间的数字, 其值越小, 运动速度越快.
然后使用
**glAccum(GL_RETURN, 1.0);**
转移到眼色缓冲区中. 
![9](https://tntb8a.bay.livefilestore.com/y1mzZAA2BcTdTJgeTCtBTQLb075xgLdGqb2nDDrx68LNYRB1XP9kLwdEc6_Fk7YL0YsOqfPDILL6xHVcXggQiOrWURVC0M2SAiguQsS0u2J4aUpuJMQGde9HFEIVdVXc9qXvDVQX0E88LMeyCKKs7DLzQ/9_thumb.jpg)
### 10.3.3 景深
距离聚焦平面越远,物体就越模糊. 
accPerspective函数
第五个和第六个参数表示在x和y方向上微移, 实现场景抗锯齿
第九个参数设定聚焦平面.
模糊程度有第七个和第八个参数决定, 由这两个参数的乘积决定. 
![10](https://tntb8a.bay.livefilestore.com/y1mtV4yNJWVu3pPzEGXOm1DGlArm4z7uBrbLMphBpT6V67-0Z5_i89VEzGEX9tDIxolr6kTD_hgctmUFKjsrcV4atinHwwFgO8CYBtwnM1k0WNTfYG9Ds4ilLwBzeJgjc2lPVBfp45RQA-avvrknZxtcw/10_thumb%5B1%5D.jpg)
### **10.3.4 柔和阴影**
多个光源所产生的柔和阴影-- 可以多次渲染场景,每次只打开一个光源, 然后将渲染结果累积起来. 
### **10.3.5 微移**
样本的微移值
见表10-6 
