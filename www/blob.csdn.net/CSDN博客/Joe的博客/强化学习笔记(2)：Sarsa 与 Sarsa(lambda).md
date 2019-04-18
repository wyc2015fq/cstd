# 强化学习笔记(2)：Sarsa 与 Sarsa(lambda) - Joe的博客 - CSDN博客





2018年05月31日 13:24:30[Joe-Han](https://me.csdn.net/u010089444)阅读数：3635








# 1.Sarsa 算法

上篇文章中介绍了Q-Learning算法， 而Sarsa 和 Q-Learning是非常类似的，二者在决策环节都是基于Q表，挑选值较大的动作值施加在环境中来换取回报。但是区别在于 Sarsa 的更新方式不一样。

下面以[强化学习笔记(1)](https://blog.csdn.net/u010089444/article/details/80508356)中老鼠走迷宫为例，对Sarsa算法流程进行说明(Sarsa 和 Q-Learning的不同之处用粗体标出)：
- 首先初始化决策表$Q$，令$Q=0$。
- $For\ each \ episode:$
- 初始化状态s 为迷宫入口位置。
- 在当前状态s的所有可能动作中选取一个动作a：即以$\varepsilon $的概率选择Q表中评分最高的动作，以$1-\varepsilon $的概率随机选择动作。
- 若当前状态s 不等于迷宫出口，则重复执行以下步骤： 
- 执行动作a并获得下一个状态s’ 和回报r（当老鼠拿到奶酪时，r为1，其余情况均为0）。
- **在当前状态s’ 的所有可能动作中选取一个动作a’**
- **更新Q表：$\mathbf{Q(s,a)=Q(s,a)+\alpha [r+\gamma\cdot  Q(s',a')-Q(s,a)]}$。**
- **更新状态和动作：$\mathbf{s = s' ，a = a' }$。**


进一步给出Q-Learning和Sarsa算法的对比：  

![这里写图片描述](https://img-blog.csdn.net/20180530212805971?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTAwODk0NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


当Sarsa 和 Q-Learning处在状态s时，均选择可带来最大回报的动作a，这样可到达状态s’。而在下一步，如果使用Q-Learning， 则会观察在s’上哪个动作会带来最大回报（不会真正执行该动作，仅用来更新Q表），在s’上做决定时, 再基于更新后的Q表选择动作。而 Sarsa 是实践派，在s’  这一步估算的动作也是接下来要执行的动作，所以 Q(s, a) 的现实值也会稍稍改动，去掉maxQ，取而代之的是在s’ 上实实在在选取的a’ 的Q值，最后像Q-Learning一样求出现实和估计的差距并更新Q表里的Q(s, a)。

因为 Sarsa 是说到做到型，所以称它为 on-policy(在线学习)，即边做边学；而Q-Learning是说到但并不一定做到，其可以通过观察别人的经历来学习，所以它也叫作 Off-policy(离线学习)。

另外，Q-Learning因为有了 maxQ，所以也是一个特别勇敢的算法，原因在于它永远都会选择最近的一条通往成功的道路，不管这条路会有多危险。而 Sarsa 则是相当保守，它会选择离危险远远的，这就是使用Sarsa方法的不同之处。

# 2. Sarsa(lambda)算法

Sarsa(lambda)算法是Sarsa 的改进版，二者的主要区别在于：
- **在每次take action获得reward后，Sarsa只对前一步Q(s,a)进行更新，Sarsa(lambda) 则会对获得reward之前的步进行更新。**

Sarsa(lambda)算法的流程如下：


![图片名称](https://img-blog.csdn.net/20180531123123154?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTAwODk0NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


从上图可以看出，和Sarsa相比，Sarsa(lambda)算法中多了一个矩阵E (eligibility trace)，它是用来保存在路径中所经历的每一步，因此在每次更新时也会对之前经历的步进行更新。

参数lambda取值范围为[0, 1] ，如果 lambda = 0，Sarsa(lambda) 将退化为Sarsa，即只更新获取到 reward 前经历的最后一步；如果 lambda = 1，Sarsa(lambda) 更新的是获取到 reward 前的所有步。lambda 可理解为脚步的衰变值，即离奶酪越近的步越重要，越远的步则对于获取奶酪不是太重要。

和Sarsa相比，Sarsa(lambda)算法有如下优势：
- Sarsa虽然会边走边更新，但是在没有获得奶酪之前，当前步的Q值是没有任何变化的，直到获取奶酪后，才会对获取奶酪的前一步更新，而之前为了获取奶酪所走的所有步都被认为和获取奶酪没关系。Sarsa(lambda)则会对获取奶酪所走的步都进行更新，离奶酪越近的步越重要，越远的则越不重要（由参数lambda控制衰减幅度）。因此，Sarsa(lambda) 能够更加快速有效的学到最优的policy。
- 在算法前几回合，老鼠由于没有头绪, 可能在原地打转了很久，从而形成一些重复的环路，而这些环路对于算法的学习没有太大必要。Sarsa(lambda)则可解决该问题，具体做法是：在E(s,a)←E(s,a)+1这一步之前，可先令E(s)=0，即把状态s对应的行置为0，这样就只保留了最近一次到达状态s时所做的action。




