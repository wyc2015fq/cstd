
# 交互式AR趣味学习软件 - intflojx的博客 - CSDN博客


2019年01月02日 16:04:16[paulpanwang](https://me.csdn.net/intflojx)阅读数：87


传统增强现实技术可以生成教材中的三维交互，但是对于教材中的图片逐一进行匹配建模又增加了软件开发了时间成本，亟需新的技术来改善性能。随着计算机视觉技术和深度学习技术的快速发展，将AR与深度学习相融合将会碰撞出新的深蓝市场。本系统结合OCR技术将教材的文字图片转为字符可以减轻手工输入的工作量、可以用于文字搜索，结合语音合成即可实现朗读功能；由于不同教材中存在着较多的物品种类相同的不同图片（特别是儿童AR图册），本作品使用深度学习中发展迅猛的物品识别技术代替特征点匹配检测图片，能一定程度减轻程序开发负担。同时在AR交互方面，本作品引入了“百度语音识别”和“Leap Motion手势识别”用语音或者手势来操作AR模型进行移动、缩放、旋转等操作。为了更好帮助学习，系统还支持曲线统计学习情况、获取未来几天的天气并制定学习计划的提醒、定时学习等功能。
经测试，系统能较好完成OCR、物品检测与识别、能生成AR模型并支持虚拟按键的模型控制、能整合百度语音识别SDK和接收手势指令，能很好完成其他附带功能，对于中小学生学习可以产生一定的帮助。随着人们对学习方式多样化的逐步追求，将时下热门的AR技术与计算机视觉相结合具有较好
本作品主要致力于中小学生的兴趣培养和空间想象能力的提高，具备两项主要功能，第一：能够提供学习规划、学习提醒、学习时间曲线反馈等功能来提高学习的自觉性和有效性。第二：能够在学习过程中提升学习体验。通过OCR技术、物品识别、AR技术、手势和语音交互将静态的文字、图片读物立体化，通过虚拟按键的旋转、移动、缩放增加阅读的互动性、趣味性。其中虚拟按键的交互取代了头戴式交互式设备，物品识别技术使得模型的应用更一般化能有效减少开发成本，学习统计能定制更好的学习方案起到良性循环。
![](https://img-blog.csdnimg.cn/2019010216025118.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
图1. 增强现实应用场景（作品效果图）
基于计算机视觉的学霸养成系统，是一款集合多种图像处理技术于一体的智能相机，主要功能模块分为OCR识别，物品识别，AR增强现实，报表分析和其他相关子功能。
![](https://img-blog.csdnimg.cn/20190102160259369.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
图2. 总体功能结构图的现实意义。
![](https://img-blog.csdnimg.cn/20190102160310172.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
效果如下所示：
![](https://img-blog.csdnimg.cn/20190102160335254.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)

