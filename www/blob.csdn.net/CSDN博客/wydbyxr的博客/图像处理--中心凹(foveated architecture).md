# 图像处理--中心凹(foveated architecture) - wydbyxr的博客 - CSDN博客
2018年10月31日 09:43:11[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：211
# 中心凹(foveated architecture)
  人眼中分为中心视力（foveal）和周边（peripheral）视力两部分，中心视力用来提供精确和细节视觉内容，而周边视力则为广角视野范围的画面，两者的视敏度差别引出了视网膜中心凹形渲染系统（foveated rendering system），这是通过追踪人眼，用低画质渲染周边视力的画面，以突出中心视力凝视的内容，但现有的相关渲染技术会造成视觉障碍，带来闪烁、模糊或“隧道视觉”，这其实是与人真实情况不同的。
  NVIDIA研究了人眼周边视力画面中的颜色、对比度、边缘和动态等元素，提出了新的中心凹形渲染技术，该技术不再简单粗暴把周边视觉闪烁或模糊化，而是在模糊中融入了反差保持（contrast preservation），这可以让用户在VR中仍可分辨出凹视力和周边视觉，更接近我们人眼看真实世界的画面。
## 直观感受
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181030104054826.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181030104107235.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181030104122185.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
  从上至下依次是：没有中心视力的画面；只模糊处理的中心视力画面；用反差保留的中心视力画面。
