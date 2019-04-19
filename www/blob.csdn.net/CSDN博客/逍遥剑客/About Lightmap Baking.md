# About Lightmap Baking - 逍遥剑客 - CSDN博客
2010年02月25日 20:38:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：3967标签：[算法																[工具](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
个人分类：[图形技术](https://blog.csdn.net/xoyojank/article/category/337624)
- [Light Mapping - Theory and Implementation](http://www.flipcode.com/archives/Light_Mapping_Theory_and_Implementation.shtml)- 基础理论, 入门必备. 知道原理才能写代码嘛
- 但是呢, 感觉这个方法不是很好计算, 很多问题也没有提及
- [Light mapping](http://www.ogre3d.org/wiki/index.php/Light_mapping)- 一个基于三角形光栅化的例子(跟UE3算法差不多)
- 带裂缝修复. 不过, 这种裂缝修复效果很不好, 会产生错误的像素
- [Light baking](http://www.ogre3d.org/forums/viewtopic.php?f=1&t=37128&sid=c0adae340a4dd4639d41b62ee1a49e76)- 跟上面那个原理相同, 只不过把光栅化交给GPU去做了
- 同样会有裂缝, 只能自己模糊一下, 跟上面的问题一样
- 在GPU端不方便模拟复杂的光照模型
- 可能是Lightsprint的实现思路
- D3DX: UVAtalas+ID3DXTextureGutterHelper 
- 最经济的方案. 不需要自己写算法, 效果还过得去
- [gile[s]](http://www.frecle.net/index.php?show=&section=giles&sub=about)- 免费的第三方工具
- 资料比较少, 需要转换成它所能识别的格式
- 增加一个工具就意味着增加一道流程
- 无法支持太大的场景
- UnrealEngine3 
- 虽然很多人手上有代码, 但是...反正我没全看懂
- [Beast](http://www.illuminatelabs.com/)- 这个商业方案可能是做这个做得最牛的了吧, 有钱可以考虑
- [Lightsprint](lightsprint.com)- 跟上面相比优势是实时, 但对于离线的, 效果就比不上了吧(个人猜测)
个人感觉, 最终效果的好坏, 跟UV的展开效果直接相关. 反正实验UVAtlas比max自动展得好, 听说有个商业插件Unwrella, 还没实验, 因为只支持2008以上的版本
谁还有什么好的资料, 欢迎交流交流
