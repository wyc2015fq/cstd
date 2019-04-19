# Puyo-Puyo设计文档 - 逍遥剑客 - CSDN博客
2007年03月20日 19:07:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：4279标签：[算法																[游戏																[colors																[list																[visio																[数据结构](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=visio&t=blog)](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=colors&t=blog)](https://so.csdn.net/so/search/s.do?q=游戏&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
个人分类：[游戏逻辑](https://blog.csdn.net/xoyojank/article/category/259557)
**Puyo-Puyo设计文档**
需求原文：
Purpose: 
----------
The goal of this exercise is to develop a JAVA or C/C++ version of Puyo-Puyo, a variation of the Tetris game.
Rules of Game: 
---------------
* Spheres come down from the top of the game board in pairs. A player can rotate them on their way down. 
* Spheres come in 4 different colors: red, blue, green, and yellow 
* Linking four spheres of the same color (horizontally, vertically or both(as in tetris)) removes them from the game board, allowing any sphere remaining to drop and fill the vacated space. 
This may lead to several possible "chain combos" if additional colors match. 
* The game board can be any size you want (we recommend 6x12) 
lNote that the pair of spheres are not linked to each other. If one sphere is blocked because there is something underneath, the other one will continue falling (with no player control over it) until it reaches something.
效果图:
![](http://home.qlsc.sdu.edu.cn/attachments/puyoLike_gk5mAKtsilvx.png)
思路：
刚拿到题时感觉这游戏以前玩过，不过规则不太一样，所以心里对游戏效果就有了一个比效好的认识。(要想做游戏，先要玩游戏，有道理啊)正好最近正在研究俄罗斯方块的算法，所以自然地在心中把两者联系到了一起。这里说说当时心中的比较结果吧：
1． 俄罗斯方块是单色的(当然有彩的)，其基本表示就是一个二维数组，数组中每个元素值取0和1，表示有没有方块。这个总体上来说也一样，只不过多取几个值来表示不同的颜色罢了。
0无1红2蓝3绿4黄
2． 俄罗斯方块一共有7种不同的积木，而这个就简单多了，只有一对球，呵呵。
3． 移动与碰撞检测两者几乎一样。就是那对球不是相连的，这个要另外考虑。
4． 消除算法。俄罗斯方块是一消一整行，算法上来说很好实现。而这个需要进行搜索，而且还会出现连消的情况，这应该是这个小游戏的一个难点吧。却实在实现时花了不少时间来想算法。
5． 失败条件两者一样。都是检测第一行是否有东西。。。。。。。
实现：
因为第一次全部是自己设计，所以不知道自己的开发流程怎么样，希望有经验的人指点一下：
游戏是什么？这个问题在以前的自己看来是个很复杂的问题，但是自从上学期学了《编译原理》后我就有个概念了(虽然学完了什么也不会，能过了就谢天谢地了)，游戏就是一个有限状态机!先想好游戏有几个不同的状态，而各种状态之间满足什么条件才能互相转换。虽然一开始我并没有从这个角度来思考问题，但是下一次我一定会的。不过这个游戏不用状态机也可以，我才设了三个状态，好像有点多此一举似的。不过我发现用了状态机之后逻辑上来说很清晰，写起来就轻松多了。有必要的话，可以用Visio先画个状态转换图。
从实现流程来说我是这样做的：
游戏后台数据表示(说白了就一个二维数组)--à
实现绘图部分(就是把数组画出来)---->
下落球生成算法---->
游戏失败判定---à
下落球移动算法(这里我没注意单个球下落时不能控制的情况，感谢“我”的提醒。注意：“我”不是我自己，是一个ID叫“我”的人-_-!)--à
下落球旋转算法--à
游戏操作实现--à
消除算法--à
连消实现(这个其实没什么，把前面写的方法按一定顺序调用一下就行了)
最把积分加上了，又调了几个BUG，加入了游戏暂停，算是完成了吧？我只是做个Demo，并不是做个实际去玩的游戏，所以很多方面没实现，后面会提到。
各部分的详细说明：
1． 下落球生成
每次生成一对球，这对球有以下属性：
两个球的颜色。初始各取随机值
两个球的方向。有水平和垂直两种，初始取水平
两个球的数目。因为有单球下落的情况，所以这也是一种状态。
两个球的坐标。以左球为准，垂直时以下面的球为准，单个球时以什么为准？还用问吗？!
私有的方法：
是否能左移，是否能右移，(是否能下移)
这对球对外的方法：
左移，
右移，
旋转，从水平旋转到垂直时交换两球颜色，反之不交换
||||R||||||G||
|----|----|----|----|----|----|----|----|----|----|----|
|R|G|G||G|R|R|| | | |
下移，如果能下落就反回true
掉落，这个简单，直接while(下移()){}搞定
绘制(这个按图形系统实现不同有所不同)
这里都要分方向和数目两种状态分别讨论
2． 消除算法
我承认自己数据结构学得不怎么样，算法也够差劲的，但是总算还是想出来了，效率可能差点吧，不过玩游戏的时候CPU占用率很少，也就没在意了J
先给定一种颜色
把数组复制一份，因为搜索的时候要设标志位
顺序遍历整个二维数组，如果遇到与给定着色相同的球，那么把它加入一个List，并把它的值设为0(就是无色，相当于删掉了，防止后面会被搜到)
对List中的每个元素，搜索与它邻的同色球，并加入List。加入的同时将其从数组中删除.
当List遍历完后，所有相邻的球都找出来，判断个数是否>=4，如果是，则从原数组中消去。(积分就是从这里算的)
清空List,按上述过程继续遍历二维数组
就这样每一次消除就拷贝一份数组，然后分4种不同的颜色遍历4遍。。。(如果你有更好的算法，请告诉我，谢谢!)
3.连索消除
在第一次消除之后可能要有继续下落的球(因为下面空了嘛!)继续下落算法：
从下到上扫描，如果当前坐标为空而当前坐标的上面非空，那么交换两个坐标对应的值。
这样就整体下落一格。每个时钟周期下落一次，就出现了逐渐下落的动画效果。(比一下跌下来Cool多了!)
还有什么需要补充的吗？应该够了吧？自己多想想，别老是看代码.
