# AI找bug不是梦，育碧火狐都在用 - 知乎
# 



> 郭一璞 发自 凹非寺
量子位 报道 | 公众号 QbitAI

Coding一时爽，debug火葬场。
![](https://pic3.zhimg.com/v2-82253a7cd72991e1d37b67eca0359baa_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='363' height='290'></svg>)
八阿哥（bug）是程序员们最讨厌的人，debug有时候能占到70%的开发成本，而且找bug费时费力，实在没有成就感。

不如，让AI去找bug？
![](https://pic4.zhimg.com/v2-cf342414be6bee3a8461a57d723f709f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='261'></svg>)
能行，而且这次不会（像Yelp一样）删库跑路。

育碧和Mozilla（就是开发火狐浏览器的那个机构）最近合作了一款找bug的AI，名叫**Clever-Commit**，利用机器学习技术，从代码库的错误和回归数据中学到了找bug的能力，可以在新代码提交的时候找到并标记可能的bug。

育碧已经在用这个AI开发3A大作，目前正在将其整合到其他产品线上去；而Mozilla也将其用在了火狐浏览器的版本更新过程中。
![](https://pic4.zhimg.com/v2-a48043c13ca9e46e5638c9c06f0d9d8f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='480'></svg>)
火狐浏览器每6~8周就会更新一次，工程团队将开始在编码、测试和发布过程中使用Clever-Commit，预期能找到60%~80%的bug。

这款产品首先将被应用在代码审查阶段，如果确实好用的话，就会在写代码的其他阶段也用上这个AI，尤其是在自动化过程中。

## **来自博士小哥的脑洞**

这个AI工具的核心贡献者，是一位名叫**Mathieu Nayrolles**的小哥。
![](https://pic2.zhimg.com/v2-63c19a82afd9c1bf0445ff57bec0f58d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='200' height='200'></svg>)
从2011年起，这位小哥就一直在想，怎么才能搞一个工具，自动找bug。三年后，他去了康考迪亚大学读博士。

博士读到第三年的时候，小哥加入了育碧的AI研发机构**Ubisoft La Forge**，利用工业界的数据，把这个想法变成了现实，开发了一款AI找bug工具，名叫**Commit-Assistant**，用强化学习和大数据技术找到代码中的bug。
![](https://pic1.zhimg.com/v2-aa7e7282f9fb17ca0354205818992a74_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='333'></svg>)
这款工具去年3月正式发布，它能找出60%的bug，如果数据集更大一点，找出率还能更高，整体上可以节约开发团队20%时间。
![](https://pic3.zhimg.com/v2-8950da3cc634c0b58e3b58809a1842e6_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='332'></svg>)
测试成功后，育碧想让这个工具能在全公司使用，于是就把Commit-Assistant原型转移到了技术部门。

之后，Mozilla加入了进来。

这款产品本来只支持育碧使用的C#、C、C++和Java，因为Mozilla的产品火狐浏览器是用C++、JavaScript和Rust写的，所以Mozilla为育碧的这个AI工具提供了自家用的编程语言的相关知识，并且用在了自家的火狐浏览器上。

自此，这个产品也就迭代为了我们开头提到的Clever-Commit。

## **传送门**

前面提到，这个AI找bug工具的最初版本是育碧和康考迪亚大学合作的，小哥和他的博士导师还将一篇相关的论文发在了MSR 2018上。

**CLEVER: Combining Code Metrics with Clone Detection for Just-In-Time Fault Prevention and Resolution in Large Industrial Projects**

作者：Mathieu Nayrolles（育碧），Abdelwahab Hamou-Lhadj（康考迪亚大学）

[https://montreal.ubisoft.com/wp-content/uploads/2018/03/clever-commit-msr18.pdf](https://link.zhihu.com/?target=https%3A//montreal.ubisoft.com/wp-content/uploads/2018/03/clever-commit-msr18.pdf)

论文里说，他们会把产品开源，放到育碧的GitHub主页上。不过量子位前去探查了一下，并没有发现这个项目的痕迹。

不知道Mozilla怎么想。

—**完**—

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


