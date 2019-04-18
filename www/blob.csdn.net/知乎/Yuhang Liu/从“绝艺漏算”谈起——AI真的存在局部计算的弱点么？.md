# 从“绝艺漏算”谈起——AI真的存在局部计算的弱点么？ - 知乎
# 



想起写这篇文章，是看到胡耀宇老师写的棋评文章里面提到，柯洁与辜梓豪的对局中，辜梓豪下出的一步妙手，甚至被绝艺所漏算。
[胡耀宇：柯洁：小心辜梓豪的内力​zhuanlan.zhihu.com![图标](https://pic4.zhimg.com/v2-6f61884b2bcdb73415305b6e8e0d906f_180x120.jpg)](https://zhuanlan.zhihu.com/p/44277618)


![](https://pic3.zhimg.com/v2-3730019d36c185bc11ad91bff1e9ae66_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='0' height='0'></svg>)
具体也就是上图中的一手夹——有人提到这手夹对于高手来说不难想到，难的是5手棋之前就看到这里的变化。但不管怎么说，这手棋被绝艺、Leela忽略也是事实。

然后就有棋友质疑AI是不是局部计算会存在弱点或者盲区，而且 [@张乐陶](https://www.zhihu.com/people/1261169957a648cd9fee665962da032e) 认为这是深度学习训练出的AI的“胎毒”。似乎大家印象里AI是大局观、形势判断强大，而局部算路未必完美。不过说实话，这与我之前看AI对局的印象并不太相符，我想举的是下面这盘棋：
[https://www.youtube.com/watch?v=_XLi1SjnSow​www.youtube.com](https://link.zhihu.com/?target=https%3A//www.youtube.com/watch%3Fv%3D_XLi1SjnSow)
不好意思上不了youtube的朋友们我也帮不了你们，但是建议能看到的仔细看看这个短短12分钟的视频——其实这是下半部分解说，还有上半部分。反正我当时看完之后有种惊掉下巴的感觉。这盘棋是AlphaGo master执黑对AlphaGo zero，zero执白中盘胜。不过，也并不是一开始就能看出白棋一定能赢的：
![](https://pic2.zhimg.com/v2-b5ece111e023297474d2e452a2148edd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1102' height='658'></svg>)
这时候也未必看出白棋有明显优势吧？

然后再过一段时间：
![](https://pic3.zhimg.com/v2-82f1b8d96a24e021865a10e455b50576_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='702' height='556'></svg>)
此时轮白走，白棋准备动手了，棋盘的焦点在中腹黑空。能猜出白棋会采取什么手段破黑棋中腹么？

我直接贴最后的结果吧：
![](https://pic1.zhimg.com/v2-89ee5acf2664cafe272a637e0813b4fc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='606' height='557'></svg>)
在上图中，我标记出来的白子，最后都对白棋的作战起到了缺一不可的辅助作用——而其中有几步可能是20手以前就下出来的。。比如最下面打二还一的那颗白子。**白棋利用黑棋气紧的缺陷，在变化错综复杂的中腹，算到20手以后的变化，使出一系列组合手筋，一举击溃黑棋**。

顺便对不熟的朋友提一下，被击溃的黑棋，就是在2016年末以网络快棋形式取得对人类顶尖棋手60连胜的大名鼎鼎的AlphaGo master。**master自己在对古力老师的对局中，也曾经在中腹展示过算路精准的组合妙手**。

所以看到这些棋以后，我真的很难相信AI的局部算路有盲点。我认为对我来说唯一合理的解释是绝艺还不够强，不够完善，和AlphaGo的最高版本相比可能还存在一些质上的差距。

不知道各位棋友怎么看呢？我自己业余低段，很多地方都看不明白。 [@胡耀宇](https://www.zhihu.com/people/3428cd3b23884e5d12ffb62c5972dd8f)  请胡老师指点一二～ 另外如果有朋友真的想看原视频又看不到的，我可以下载下来发送给你们。


