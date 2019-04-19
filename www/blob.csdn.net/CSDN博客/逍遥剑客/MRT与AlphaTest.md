# MRT与AlphaTest - 逍遥剑客 - CSDN博客
2011年05月14日 21:08:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2581
想把DeferredShading的MRT改成A8R8G8B8的, 于是把Normal和Depth做了一下压缩
然后问题就来了, 有很多像素丢失了:
![](http://hi.csdn.net/attachment/201105/14/0_1305378167xaHC.gif)
当时想了很久没想明白, 后来还是用GPA看了一下pixel history才知道alphatest失败了
看了下render state的确是alpha test开启了, 关掉就正常了.
查了一下资料原来固定管线的alpha test使用rendertarget0的alpha做test, 而这里的0是normal depth的RTT
于是乎就悲剧了.
关掉alphatest的state, 在PS里使用clip/discard做alphatest, 解决.
另外, 这里有一些关于early z culling的讨论, 没想到能在哪里用上:
[http://www.opengpu.org/bbs/forum.php?mod=viewthread&tid=3098&page=1](http://www.opengpu.org/bbs/forum.php?mod=viewthread&tid=3098&page=1)
