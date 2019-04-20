# 比AlphaGo Zero更强的AlphaZero来了！8小时解决一切棋类！ - 人工智能学家 - CSDN博客
2017年12月07日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：211
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWwKRoianFiavKSxINEQSJDvGsbiapXwmWaOMhJGVlT8SGA1icYhsSibqXYo5pFOYho4xJjzVIodFnRlgw/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
*来源：**本文作者PENG Bo（http://t.cn/RY3MKSS），本文首发于作者的知乎专栏《技术备忘录》*
读过AlphaGo Zero论文的同学，可能都惊讶于它的方法的简单。另一方面，深度神经网络，是否能适用于国际象棋这样的与围棋存在诸多差异的棋类？MCTS（蒙特卡洛树搜索）能比得上alpha-beta搜索吗？许多研究者都曾对此表示怀疑。
但今天AlphaZero来了（https://arxiv.org/pdf/1712.01815.pdf），它破除了一切怀疑，通过使用与AlphaGo Zero一模一样的方法（同样是MCTS+深度网络，实际还做了一些简化），它从零开始训练：
4小时就打败了国际象棋的最强程序Stockfish！
2小时就打败了日本将棋的最强程序Elmo！
8小时就打败了与李世石对战的AlphaGo v18！
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/bicdMLzImlibTsszX89LoVEsxPVICNp4aruibibmqEub5nTlSTict2cnUXCjUjpgBpyD70umiaAInxZ6qO7ibjbNXWorA/?)
在训练后，它面对Stockfish取得100盘不败的恐怖战绩，而且比之前的AlphaGo Zero也更为强大（根据论文后面的表格，训练34小时的AlphaZero胜过训练72小时的AlphaGo Zero）。
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/bicdMLzImlibTsszX89LoVEsxPVICNp4arQgVWf2QrdnWn0vS4Y1OiaUoqibdU8CUNoEh8f22eBXsribHT4iahhia17Uw/?)
## 这令人震惊，因为此前大家都认为Stockfish已趋于完美，它的代码中有无数人类精心构造的算法技巧。
## 然而现在Stockfish就像一位武术大师，碰上了用枪的AlphaZero，被一枪毙命。
在reddit的国象版面的讨论中（Mastering Chess and Shogi by Self-Play with a General Reinforcement Learning Algorithm • r/chess）（http://t.cn/RY3x1kK），大家纷纷表示AlphaZero已经不是机器的棋了，是神仙棋，非常优美，富有策略性，更能深刻地谋划（maneuver），完全是在调戏Stockfish。
喜欢国象的同学注意了：AlphaZero不喜欢西西里防御。
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/bicdMLzImlibTsszX89LoVEsxPVICNp4arWTQAoAU4q3nbKVvDAw3SmH7cKRibUu4hicoaHNSv0IcJhuZfdvD6bl5g/?)
训练过程极其简单粗暴。超参数，网络架构都不需要调整。无脑上算力，就能解决一切问题。
Stockfish和Elmo，每秒种需要搜索高达几千万个局面。
AlphaZero每秒种仅需搜索几万个局面，就将他们碾压。深度网络真是狂拽炫酷。
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/bicdMLzImlibTsszX89LoVEsxPVICNp4ar8Y0Q0C9zVibj6W1fFb8Btd9eqJa416OO2sTRBibYanL2s1AYZ1zqH3dw/?)
当然，训练AlphaZero所需的计算资源也是海量的。这次Deepmind直接说了，需要5000个TPU v1作为生成自对弈棋谱。
不过，随着硬件的发展，这样的计算资源会越来越普及。未来的AI会有多强大，确实值得思考。
个人一直认为MCTS+深度网络是非常强的组合，因为MCTS可为深度网络补充逻辑性。我预测，这个组合未来会在更多场合显示威力，例如有可能真正实现自动写代码，自动数学证明。
## 为什么说编程和数学，因为这两个领域和下棋一样，都有明确的规则和目标，有可模拟的环境。
（在此之前，深度学习的调参党和架构党估计会先被干掉...... 目前的很多灌水论文，电脑以后自己都可以写出来）
## 也许在5到20年内，我们会看到《Mastering Programming and Mathematics by General Reinforcement Learning》。然后许多人都要自谋出路了......
未来智能实验室致力于研究互联网与人工智能未来发展趋势，观察评估人工智能发展水平，由互联网进化论作者，计算机博士刘锋与中国科学院虚拟经济与数据科学研究中心石勇、刘颖教授创建。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎支持和加入我们。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBXoz49H9w8lfGXmBjcGga9W15bmYAvZDqbbI8p0liaLcxEh1iao7YXIgJvp6MLHpskd7jHq8ibX0QzDQ/640?wx_fmt=png)
