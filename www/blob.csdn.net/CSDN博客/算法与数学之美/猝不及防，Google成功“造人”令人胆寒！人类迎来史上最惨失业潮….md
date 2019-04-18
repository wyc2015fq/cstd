# 猝不及防，Google成功“造人”令人胆寒！人类迎来史上最惨失业潮… - 算法与数学之美 - CSDN博客
2018年06月13日 21:03:32[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：39
> 
本文借助生物学中达尔文的进化理论来介绍遗传算法，并展示了通过简短的 Python 教程实现遗传算法的案例。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/KmXPKA19gWicSbrxGskbvW9Hcl9U82OyAdp4UtKODlCwO4vibLRFibficGDB01JzZMxdTYKlw8s1Lu2iaODl2yw7YcQ/640?wx_fmt=jpeg)
在本文中，我将会解释遗传算法的概念。首先，我会介绍它的起源与目标。接着，我会展示如何通过一个简短的 Python 教程实现它。
所以，问题就是：我们如何打造好的人工智能？
一个天真的解决方案就是创建由一系列规则构成的「经验算法」：「如果你遇到这些条件，就这么执行。」我可以想象到通过这样的足够多的规则，就能再现自然智能。但工作量会异常大，最终方案将永远无法使创建者满意。花大把时间创造一样东西却得知它不可能完美，还是挺让人沮丧的吧？
为了避免这一点，我有了一个新想法。如果我舍弃直接的方案而选择再现进化会怎么样。我们可以创造第一条史前鱼，将其放入适合进化的条件内，让它自由地向人类进化。这个想法被称为「遗传算法」，我们将在下文中亲自创建一个。那么，首先让我们回忆一下，试着理解达尔文的自然选择理论。
这个理论很简单：如果一个种群想要兴盛，它必须不断提升自己，这被称为适者生存。种群中最优秀的品质遗传给后代。但是为了保持一些多样性以及适应自然环境的变化，其他个体也不能被遗忘。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gWicSbrxGskbvW9Hcl9U82OyAlyLQ8me1P9Z3CxVlZCgrw6l3iaePNrFlEzgLF3Mpojj27ibIeO5qso0w/640?wx_fmt=png)
*从一代到下一代*
这一算法尤其擅长解决优化问题。
### **实例：背包问题**
背包优化是一个经典的算法问题。你有两样东西：一个容量为固定重量的背包和一系列不同重量和价值的盒子。目标是装满这个背包使其价值最大化却又不超出它的最大承载重量。自 1972 年以来，这一直是一个著名的数学问题。遗传算法可以很好地解决这一问题，因为它本质上是一个具有大量可能答案的优化问题。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gWicSbrxGskbvW9Hcl9U82OyAS6lZ6vy9bqS5KTdhKmtYrgVBKcJt3Jd8jCsH8G1jqQqs16fDj9aEeQ/640?wx_fmt=png)
*背包问题*
为了亲自测试这一算法的工作原理，我们用它解决一个简单的问题：如何破解同事的密码。
该算法用 Python 3 实现。你可以下载 Windows 版或者使用 brew install python3 、sudo apt-get install python3 或 sudo yum install python3 进行安装。我建议你在 Jupyter notebook 中运行这一代码。
### **选择一个适应度函数**
当你决定创建一个遗传算法时，要做的第一件事情就是创建评估函数，用来评估样本的成功：它允许我们把种群划分为丑小鸭和白天鹅。区分的目标在于成功的样本稍后将有更多机会被挑选来塑造下一代。这看起来很简单，但是不要被愚弄了。
我们的目标是什么？破解密码。因此我们函数的目标是把连续标记中的二值结果「失败或成功」从 0 转换到 100。
这里有一个最简单的方案：
fitness score = (number of char correct) / (total number of char)
如此，一个带有更大适合度结果的个体将比其他样本更接近成功。因此我们的适合度函数能够精确分类种群。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkwQAxftXCDFAmvIVdEXOUlRsYc1sbLqlCZAwSIKa0ZVPr4Nh8wrbV6kKjkxQyyMhJq2gK5S31rh1w/640?wx_fmt=png)
### **创建个体**
那么现在我们知道如何评估个体，但是又如何对其定义呢？这一部分确实需要一点技巧：我们的目标是知道什么是固定不变的规格参数表以及什么是变量。
此处，将其比作遗传会有一定帮助。DNA 实际上是由基因组成，而基因又来自不同的等位基因（该基因的不同版本）。因此你不得不创建你的种群的 DNA。
在我们的情况中，个体是单词（密码的长度当然是相同的），每个字母是一个基因，字母的值是等位基因。在单词「banana」中，「b」是第一个字母的等位基因。
这一创建的意义是什么？现在我们知道每一个个体保持良好形状（一个正确长度的单词），但是种群将会覆盖每一种可能性（每个可能带有这一长度的单词）。
### **创建第一个种群**
现在，我们知道个体的规格参数表，以及如何评估其表现。我们必须开始严格意义上的「进化」。
要记住的主要一点是我们不能把种群导向一个明显很好的方案。我们必须使种群尽可能宽广，覆盖尽可能多的可能性。第一个完美的种群应该覆盖每一个现有的等位基因。
因此，在这个案例中，我们将创建由随机字母组成的单词。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkwQAxftXCDFAmvIVdEXOUlROrjCNYjegVShmbFcEhYQSVJIEI9mibJrU9GlfBBibI2ApxAGvOUN7Aiag/640?wx_fmt=png)
### **从一代到下一代**
为此，我们有两件事要做：选择我们当前代中的一个特定部分，并且整合育种者孕育下一代。
#### **育种者选择**
我们有很多选择，但是你必须牢记两点：目标是选择前一代的最佳方案并且不把其他方案彻底放在一边。危害是：如果你在算法开始时只选择好的方案，你将很快向局部最小值而不是可能的最佳方案收敛。
我的方案是一方面选择更好的样本 N（在我的代码中，N = best_sample），另一方面选择无需区分适合度的 M 个随机个体（M = lucky_few）。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkwQAxftXCDFAmvIVdEXOUlRkYAlKQUI6OcYpNlO3Bicd6ib3hCMFV9ATgcjicfUbDQZwYibfhAgicFepWA/640?wx_fmt=png)
#### **繁衍**
我们继续用生物意义的繁衍进行类比。有性繁殖的目的是混合两个个体的 DNA，这里我们也在做同样的事情。我们有两个个体「Tom」和「Jerry」，他们的 DNA 由其等位基因定义。因此为了混合其 DNA，我们必须混合字母。我挑选了一种最简单的解决方案：随机选择「Tom」或「Jerry」的字母作为子对象的字母。
很明显，父母「Tom」和「Jerry」并不会只生一个孩子，为了确保种群数量稳定，你必须确定每对父母所生孩子的数量（0 代中的个体数量等于下一代中的个体数量）。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkwQAxftXCDFAmvIVdEXOUlRuDG3V6hlQI29t5zcYs7HVMibakkHQe4Gt7tKMrpOckyp9ZiaJuxAg8NQ/640?wx_fmt=png)
### **突变**
最后一步是个体的自然突变。繁衍之后，每一个个体都存在一定的概率：DNA 发生轻微改变。这一操作的目的是防止算法在局部极小值中受限。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkwQAxftXCDFAmvIVdEXOUlRohI1Kt41uu81g91zsTo0vicAyQB21jibk9AGqbEqFbib0V4LBFia8TsK5g/640?wx_fmt=png)
现在，你有了构建遗传算法的所有工具。请随意修改我的实现。每一步都有很多种可能的方案，请采取最合适的那个。
∑编辑 | Gemini
来源 | 校苑数模
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabky7x6u1VxMVMia4MLibNzC2nrumY3zDflTsCeoM04M1BrkvPny8tsw6hYkIicUr42iarLmadL2x6JwV6A/640?wx_fmt=png)
算法数学之美微信公众号欢迎赐稿
稿件涉及数学、物理、算法、计算机、编程等相关领域，经采用我们将奉上稿酬。
投稿邮箱：math_alg@163.com
