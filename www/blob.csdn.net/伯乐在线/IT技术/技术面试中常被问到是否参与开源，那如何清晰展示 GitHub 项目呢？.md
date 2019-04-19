# 技术面试中常被问到是否参与开源，那如何清晰展示 GitHub 项目呢？ - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [小米云豆粥](http://www.jobbole.com/members/buntamarui) 翻译。未经许可，禁止转载！
英文出处：[thehftguy](https://thehftguy.com/2016/10/24/heres-how-to-make-a-good-github-project-for-your-resume/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
你面试的公司找你要 GitHub 链接，猎头找你要 GitHub 链接。“你参与过开源项目么？”是目前 IT 技术面试中的常见问题之一。
既然他们要 GitHub，你就得给。本文将阐述如何在面试中展示你在 GitHub 上的项目。
以下的建议有两个用途：应聘者可以学习怎么写软件的简介（不一定非要是GitHub）；招聘者（程序员）可以学习可以从哪些关键点来评价一个程序员的水平和软件的质量。
![submit application form with a GitHub link](https://thehftguy.files.wordpress.com/2016/10/github-submit-application1.png?w=300&h=239)When having a GitHub is mandatory, just like having a name.
### 链接一个特定的项目
在你的简历和每一份申请里面都放上你的GitHub链接。
该链接要直链到一个项目。不要给你GitHub的主页地址，那个页面没有有用信息，很难让人继续浏览。
你必须只展示一个项目。一个就够了，不需要更多。
这个项目将是你的“登录页面”，是招聘者看到的第一个页面。他们很少去看其他页面（也没有这个必要）所以该页面本身就应该展示一切信息。如果他们去看了其他页面，说明这个主页成功引起了他们的兴趣，他们想知道更多项目细节。
我们要写一个可以充分展示你的能力，并能给人留下好的第一印象的项目页面。
### 项目结构
用5秒时间看软件的架构就可以知道项目优劣。
你一眼就能看出来一个程序员没有经验，他们的项目往往没有结构。项目文件要么在随便一个地方或者都在根目录。
**一个好的项目结构必须将源代码，测试，库文件，可执行文件分开放在不同的文件夹中**。
命名的细节，例如是用‘doc’还是用‘docs’并不重要。这里是GitHub项目的[文件夹结构](https://GitHub.com/kriasoft/Folder-Structure-Conventions)：


```
.
├── build                   # Compiled files (alternatively `dist`)
├── docs                    # Documentation files (alternatively `doc`)
├── src                     # Source files (alternatively `lib` or `app`)
├── test                    # Automated tests (alternatively `spec` or `tests`)
├── tools                   # Tools and utilities
├── LICENSE
└── README.md
```
![software project structure](https://thehftguy.files.wordpress.com/2016/10/github-project-structure.png?w=809)A well-organized project
### 有一个项目简介文件
项目简介文件可以提供：
- 项目目标
- 截屏或视频
- 使用说明
- 软件下载页面
### 在简介文件里面放软件界面截屏
一张图比一千个字更能说明问题。
人们不会因为要看软件界面就去安装软件。请提供软件界面截图。
### 在简介中放视频
一张图能抵一千个字，那么一个视频顶一千张图。
![snake game preview animated gif](https://thehftguy.files.wordpress.com/2016/10/github-snake-gif.gif?w=809)Great demo from [a random snake project on GitHub](https://GitHub.com/DyegoCosta/snake-game)
**注意**：GitHub不允许插入视频，你可以使用动态图。
### 网页链接或安装文件链接
如果该项目是网络应用，请提供网页地址。当然，所有人都应该能访问该地址。
如果该项目是单机程序，请提供安装文件下载地址。如果安装文件看上去不专业，用户是不会安装它的。很多时候，是否专业跟发布方式有关。
### 整合GitHub工具
GitHub提供了一整套免费工具用于编连，打包，测试和其他功能。这些工具在专业软件开发中都是必须的。
以前想要实现这些功能非常困难，但如今通过GitHub一切都非常简单。你没有理由不用它们。
![GitHub-integration-icons](https://thehftguy.files.wordpress.com/2016/10/github-integration-icons.png?w=809)
这是一个由C++实现的四子棋的例子。按钮从左到右代表：
- 在Linux上编连（[Travis CI](https://travis-ci.org/)）
- 在Windows上编连（[AppVeyor](https://www.appveyor.com/)）
- 单元测试和测试覆盖分析（[Coveralls](https://coveralls.io/)）
### 如何处理源代码？
“没人在乎你的源代码——得知这个事实的时候我也很吃惊。我在整理代码上花了不少时间，但发现没人在乎。重要的不是代码，而是产品” （[出处](https://mortoray.com/2015/04/20/nobody-cares-about-your-code/)）
比起猜，读一段软件功能描述要快10倍。比起自己弄明白软件用法，观看一个介绍基本用法的视频可以快100倍。比起逆向工程，读设计图表要快1000倍。你可以通过读源代码来搞清楚一切，但这个过程要费事得多。读源代码（解码源代码）是非常耗时的。只有在走投无路的时候才应该用。
经验教训1：没人在乎你的代码，也不会有人去读你的代码。
经验教训2：别指望别人会去读你的代码，也别强迫别人去读代码。
### 如果我没有像样的大项目要怎么办？
挺好的。简单的项目更容易展示，更容易解释，面试官也更容易理解。比如大家都知道四子棋是怎么回事。
即使第一眼看上去很简单，但每一个项目都包含了很多工作量：写一个好的用户界面，调整颜色，允许两个人一起游戏，加上对下一步棋的走子建议，加上 AI 选手等等。
这个游戏规则很简单但是要把它做成好的软件要做很多工作。这些都是你在面对面的面试中可以跟面试官深入探讨的地方。
你知道玩四子棋的时候先走的选手一定会赢么？【如果他不犯错的话】你知道如果第一个选手第一步棋不走中间的话，第二个选手一定可以将棋走成平局么？
来源：[A Knowledge-based Approach of Connect-Four](http://www.informatik.uni-trier.de/~fernau/DSL0607/Masterthesis-Viergewinnt.pdf), *The Game is Solved: White Wins*, Victor Allis
### 面试官真的会去看GitHub么？
事实上**他们不会**。
![GitHub traffic statistics](https://thehftguy.files.wordpress.com/2016/10/github-traffic.png?w=809)GitHub Traffic Statistics
我们测试过了。这个图是我们发出一堆简历之后的数据统计。这三个点击是我自己点的。在写这篇文章的时候我没有登录就去点了GitHub。我的错。
从我上次找工作的经验来说，经过十几个电话面试（每次电话有一个技术人员）和几个现场面试（4到7个技术人员），只有一个人去看了我的GitHub页面。
结论：没人关心GitHub，没人会看GitHub。每人都会问，是因为别人也在这样做。
好处：既然没有人真的去看，你也可以拒绝参加这场GitHub的假面舞会。你可以用这个[hello world 终极版本库](https://GitHub.com/leachim6/hello-world) 去回应所有的GitHub要求。这个版本库包含用一百种语言实现的“Hello world”。
### 小抄
- 项目要有结构
- 要有项目简介文件
- 写一段说明软件功能的介绍
- 放上软件界面截图和使用说明视频
- 提供安装文件下载地址或者网络应用的网址
- 整合发开工具（CI，单元测试，打包工具等等）
这世上还有其他软件工程的好范例，不仅仅局限于GitHub。
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/07/2ec965670e006b5d53c221a7cdcf0108.jpg)![](http://jbcdn2.b0.upaiyun.com/2016/07/a5e36df34a1aea088f06fb503c7ce93d.jpg)
