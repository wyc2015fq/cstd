# blender形变动画制作 - fighting！！！ - CSDN博客
2017年08月28日 22:02:56[dujiahei](https://me.csdn.net/dujiahei)阅读数：1247
blender形变动画制作_百度经验 http://jingyan.baidu.com/article/b2c186c82a5652c46ef6ff02.html
我们用blender制作动画的时候都是对物体进行缩放、移动、旋转，但是如果需要制作物体形态变化的动画，这就需要用到形态键，然后设置形态帧制作动画了。
![blender形变动画制作](http://e.hiphotos.baidu.com/exp/w=500/sign=19603ebc72899e51788e3a1472a6d990/b151f8198618367af62e929c27738bd4b21ce5c8.jpg)
## 方法/步骤
## 打开blender创建一个平面，并赋予材质颜色，作为“进度条”。
- 
![blender形变动画制作](http://c.hiphotos.baidu.com/exp/w=500/sign=c5e6f1d54f2309f7e76fad12420f0c39/11385343fbf2b21119bf610fc38065380dd78eda.jpg)
- 
右边切换到“**数据**”，添加一个“**形态键**”，这个作为基础形态。
![blender形变动画制作](http://b.hiphotos.baidu.com/exp/w=500/sign=e7b3fd5fb0389b5038ffe052b534e5f1/cdbf6c81800a19d86d5c63443afa828ba61e4610.jpg)
- 
再次创建一个形态键命名为“**finish**”。
![blender形变动画制作](http://b.hiphotos.baidu.com/exp/w=500/sign=7c18657133f33a879e6d001af65d1018/2e2eb9389b504fc27c435644ecdde71190ef6d57.jpg)
- 
选择“**finish**”形态键，进入“**编辑模式**”，选择平面一边把它移动缩小。
![blender形变动画制作](http://e.hiphotos.baidu.com/exp/w=500/sign=3cd62347a6c379317d688629dbc5b784/4d086e061d950a7b462eaf8503d162d9f2d3c95c.jpg)
- 
回到“**物体模式**”，我们左右拖动改变“**值**”，就会发现进度条的动画效果了。
![blender形变动画制作](http://e.hiphotos.baidu.com/exp/w=500/sign=fe30d982deca7bcb7d7bc72f8e086b3f/cb8065380cd791231be37435a4345982b2b78074.jpg)
- 
我们把时间轴移动到“1”，把“**值**”设为“**0.000**”，在“**值**”上右键“**插入关键帧**”。
![blender形变动画制作](http://c.hiphotos.baidu.com/exp/w=500/sign=ccd1024e8e94a4c20a23e72b3ef51bac/79f0f736afc37931100c6568e2c4b74543a9110e.jpg)
- 
再次把时间轴移动到最后，然后把“**值**”设置为“**1.00**”，右键“**插入关键帧**”。
![blender形变动画制作](http://f.hiphotos.baidu.com/exp/w=500/sign=2cd1f0e650ee3d6d22c687cb73176d41/37d3d539b6003af3476b3f213c2ac65c1138b69a.jpg)
- 
形变动画制作完成，现在我们移动时间轴，就可以看到进度条动画的效果了。
灵活使用我们就可以制作扇叶展开等等形态变化的动画效果。
![blender形变动画制作](http://c.hiphotos.baidu.com/exp/w=500/sign=78aab24905d79123e0e094749d355917/fcfaaf51f3deb48f3e7e439ff91f3a292df57858.jpg)

