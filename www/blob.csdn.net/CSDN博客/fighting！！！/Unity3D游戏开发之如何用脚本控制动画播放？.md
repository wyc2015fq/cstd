# Unity3D游戏开发之如何用脚本控制动画播放？ - fighting！！！ - CSDN博客
2017年06月16日 11:39:33[dujiahei](https://me.csdn.net/dujiahei)阅读数：10254
来源：http://tieba.baidu.com/p/4239597398
       Unity3D游戏开发中，如何用脚本控制动画播放的效果，下面菜鸟在线教育的李默涵就为大家简单的介绍一下操作方法及具体代码。
　　首先，你需要将FBX动画文件先导入到Unity3D游戏开发软件中，之后在inspector中选择Animations，并在Clips中添加和命名具体的剪辑，例如其中一个命名为walk。
　　将模型拖入场景，命名为myanim，控制脚本如下：
　　1.void Start () {
　　2.GameObject myanim = GameObject.Find ("myanim");
　　3.myanim.transform.animation.wrapMode = WrapMode.Loop;
　　4.myanim.transform.animation.Play ("walk");
　　5.}
　　以上脚本加入后，可能会出现Animation must be marked as legacy这样的错误，那该怎么办?其实你只需要在Assets中选择模型，然后在inspector中选择Rig，在Animation Type中切换为Legacy，以上错误就可以解决了。
　　看完此篇你是否已经学会，如果还想了解什么Unity3D游戏开发技术，可以给我留言，我会尽快请菜鸟在线教育的专业老师为大家解决，不过很多知识点已经才菜鸟在线官网有介绍，你可以先到官网上看看，还有很多免费的Unity3D游戏开发课程任你观看，这可都是很经典的课程，机会难得别错过了!
