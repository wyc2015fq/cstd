
# 快手科技——Animoji - 小花生的博客 - CSDN博客


2018年11月11日 19:49:49[Peanut_范](https://me.csdn.net/u013841196)阅读数：296


###### 快手科技——Animoji
演讲者：快手科技李岩
摘录——机器之心
我们知道视频是视觉、听觉、文本多种模态综合的信息形式，而用户的行为也是另外一种模态的数据，所以视频本身就是一个多模态的问题，再加上用户行为就更是一种更加复杂的多模态问题。所以多模态的研究对于快手来说，是非常重要的课题。
2D 图像驱动 3D 建模实现 Animoji 效果
通过苹果的发布会，大家应该都了解 Animoji 这项技术，iphoneX 有一个标志性的功能，就是通过结构光摄像头实现 Animoji，现在国内手机厂商也越来越多地采用结构光的方式去实现 Animoj。而快手是国内较早实现不使用结构光，只用 RGB 图像信息就实现 Animoji 效果的企业。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111194550278.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
用户不必去花上万元去买 iphoneX，只要用一个千元的安卓手机，就可在快手的产品上体验 Animoji 的特效，从而能够在不暴露脸部信息的同时展现细微的表情变化，例如微笑、单只眼睛睁单只眼睛闭等，让原来一些羞于表演自己才艺的人，也可以非常自如地表达。我们觉得做技术有一个非常快乐的事情，就是让原来少数人才能用的技术，变得更普惠。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111194651469.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
其实解决这样一个问题是非常难的，因为即使是像苹果这样的公司，也是采用了结构光这样配置额外硬件的方式来解决。想让每一个用户都能享受到最尖端的技术，快手面临着硬件的约束，只能通过 2D 的 RGB 视觉信息对问题进行建模、求解，这里面包括了像 Landmark 人脸关键点检测、实时重建人脸三维模型等技术，把 2D 和 3D 两种不同模态的信息做建模、做对齐。
[
](https://img-blog.csdnimg.cn/20181111194651469.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)我们也能看到现在市场上可能有一些小型的 APP 在做类似的事情，但体验很差，而我们的整体体验还是非常好非常流畅的，这也需要归功于深度神经网络模型的量化，通过压缩和加速解决手机性能问题，可适配任意机型。
[
            ](https://img-blog.csdnimg.cn/20181111194651469.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)

