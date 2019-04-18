# 【报道】Julia中文社区用户见面会28日在北京软件所举行 - 知乎
# 

> **文章作者：李优**
**责任编辑：李优**
**审稿人：**[@覃含章](https://www.zhihu.com/people/866e63341ae3873b7a4ce0390767dc74)
文章已发表于**微信公众号【运筹OR帷幄】：**[【报道】Julia语言中文社区2018用户见面会28日在北京软件所举行](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/twjrFSZ4CRQ6GwCIWYADxg)
*欢迎原链接转发，转载请私信*[@留德华叫兽](https://www.zhihu.com/people/961e8cc4f7512fda1ea6626ce9a05e8e)*获取信息，盗版必究。*
敬请关注和扩散本专栏及同名公众号，会邀请**全球知名学者**发布运筹学、人工智能中优化理论等相关干货、[知乎Live](https://www.zhihu.com/lives/users/961e8cc4f7512fda1ea6626ce9a05e8e)及行业动态：[『运筹OR帷幄』大数据人工智能时代的运筹学](https://zhuanlan.zhihu.com/operations-research)

近年来人工智能的相关研究在如火如荼地进行，越来越多学者和开发人员开始思考如何超越目前机器学习领域python与C++结合的模式，寻找一种既快速实现想法，又不损失计算的效率编程方法。Julia就是一种专门为并行数据处理而设计的编程语言，创作者Karpinski说：“我们渴望一种语言的速度与C++一样快，但又有Python、R或Matlab的高级功能。于是，我们开发出了这样一种语言：Julia，它让我们可以用同一种语言来进行原型设计和实际开发。” 而在实践中，Julia语言也正在被广泛采用，尤其是国外高校以及研究机构。

2018年7月28日，Julia中文社区的召开了2018年用户见面会，本次见面会是Julia中文社区第三次召开见面会，参会人员涵盖国内Julia社区的群贤，包括 Viral Shah（Julia语言发起者之一，Julia Computing公司CEO ） ，张常有（中国科学院软件研究所教授），覃含章，（麻省理工学院计算科学与程博在读）等，本次会议由Julia中文社区主办，承办单位 是集智俱乐部和中国科学院软件所（提供资金和场地）。

Julia创始人Viral Shah通过远程视频介绍了Julia在机器学习中的应用；覃含章介绍了JuMP在优化中的应用；“千里冰封”介绍了他在Jetbrain平台为Julia打造IDE插件时的一些心得；罗秀哲介绍了Julia利用多重派发实现面向对象程序设计和元编程的方法，刘金国讨论了Julia的性能进步过程，张常有介绍了Julia在聚类算法中性能优化的方法，斯大卫则分享了Julia中的编程实践。本次见面会在B站进行了同步直播（罗秀哲同学还提前一个晚上在B站进行了Julia语言的简介），相关资源可以在GitHub上查阅，[https://codeload.github.com/JuliaCN/MeetUpMaterials/zip/v0.1.0](https://link.zhihu.com/?target=https%3A//codeload.github.com/JuliaCN/MeetUpMaterials/zip/v0.1.0)。

JuMP是Julia最为活跃的板块之一，JuMP的特点是能够提供简洁快速建立数学规划模型，作为使用者只需使用JuMP就可以为不同的求解器进行建模，从而快速验证不同求解器对同一数学问题的求解效果。经验证，JuMP的模型生成速度已经比肩与商业的求解器。Coursera的课程Discrete Optimization就推荐使用JuMP作为数学优化编程入门的工具。

[https://www.coursera.org/learn/discrete-optimization](https://link.zhihu.com/?target=https%3A//www.coursera.org/learn/discrete-optimization)

另外，目前Julia1.0.0已经发布，该版承诺了该版本语言 API 稳定性： Julia 1.0的代码将可以在 Julia的后续版本中运行。核心语言开发者和社区可以基于这个稳定的基础去构建软件包、工具和新特性。Julia 1.0 还引入了一些新的、强大的和创新的语言功能， 包括：

全新的内置包管理器，新的缺失值表示规范， 内置 String 类型现在可以安全地保存任意数据等。Julia 1.0将广播扩展为自定义类型，使其能够在GPU和其他矢量化硬件上能够实现高效运算，为将来更高效的性能提升铺平了道路。
![](https://pic4.zhimg.com/v2-90909437bbaf559a604af8ffe5016b43_b.jpg)![](https://pic4.zhimg.com/v2-6dcdd25068a8414b204114da988d4dcf_b.jpg)

**我们渴望一种语言的速度与C++一样快，但又有Python、R或Matlab的高级功能。于是，我们开发出了这样一种语言：Julia，它让我们可以用同一种语言来进行原型设计和实际开发。”**

***-Karpinski***

此外，围绕 Julia 1.0 的新特性，还正在构建许多新的外部软件包，以改进数据处理和操作生态系统。

当然，Julia在1.0的更新中的优化项远比我们所列出的多。详细情况可以查看官网：

Julia 1.0：[http://julialang.org/blog/2018/08/one-point-zero](https://link.zhihu.com/?target=http%3A//julialang.org/blog/2018/08/one-point-zero)

怎么样，从事运筹优化事业的你，是不是已经忍不住要上手了试一试了？为此，【运筹OR帷幄】公众号特意邀请Julia社区的核心成员覃含章同学还将为本订阅号特地撰写一篇文章，具体讲解JuMP的用法，该教程面向对于优化建模零基础的初学者，介绍JuMP——Julia中最流的优化建模语 。内容将涉及线性规划论初步，如何 JuMP调 商 /开源求解 ，进线性规划，锥规划，鲁棒优化，整数规划 的建模。针对Gurobi，介绍如何 JuMP在(混合整数)线性规划中同时进 variable/constraint generation，另有丰富实例作为快速上 Julia 泛进 优化建模和求解的教程。

[【学界】JuMP: 用Julia进行优化建模及求解](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/NybrZ-0dh1s-ilQ2qLnGHw)

> **如需转载请在公众号后台获取转载须知**
**友情提醒：本文由运筹OR帷幄编译整理，不作为商业用途如有内容侵权，我们将随时删除。**

如果你是**运筹学|人工智能**硕博或在读，请在下图公众号后台留言：“**加微信群**”。系统会进一步提示，邀请您进全球运筹或AI学者群（群内学界、业界大佬云集）。
同时我们有：【运筹|优化】【供应链|物流】【人工智能】【数据科学|分析】爱好者千人QQ群，请关注下方公众号点击“加入社区”按钮，获得入群传送门。
![](https://pic4.zhimg.com/v2-c97fbb902c744c89c4fb78b33aadf3b7_b.jpg)

