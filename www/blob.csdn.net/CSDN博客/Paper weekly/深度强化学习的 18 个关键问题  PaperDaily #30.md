# 深度强化学习的 18 个关键问题 | PaperDaily #30 - Paper weekly - CSDN博客





2017年12月22日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：705












![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/?wxfrom=5&wx_lazy=1)





在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第 **30** 篇文章


# 关于作者：王凌霄（社区ID @Nevertiree），中国科学院自动化研究所实习生，研究方向为强化学习和多智能体。




这两天我阅读了两篇篇猛文 **A Brief Survey of Deep Reinforcement Learning** 和 **Deep Reinforcement Learning: An Overview**，作者排山倒海的引用了 200 多篇文献，阐述强化学习未来的方向。




■ 论文 | A Brief Survey of Deep Reinforcement Learning

■ 链接 | http://www.paperweekly.site/papers/922

■ 作者 | Nevertiree




■ 论文 | Deep Reinforcement Learning: An Overview


■ 链接 | http://www.paperweekly.site/papers/1372

■ 作者 | Nevertiree




原文归纳出深度强化学习中的常见科学问题，并列出了目前解法与相关综述，我在这里做出整理，抽取了相关的论文。 **这里精选 18 个关键问题，涵盖空间搜索、探索利用、策略评估、内存使用、网络设计、反馈激励等等话题**。




**本文精选了 73 篇论文（其中 2017 年的论文有 27 篇，2016 年的论文有 21 篇），为了方便阅读，原标题放在文章最后，可以根据索引找到。**




# 问题一：预测与策略评估



**prediction, policy evaluation **




万变不离其宗，**Temporal Difference 方法**仍然是策略评估的核心哲学【Sutton 1988】。TD的拓展版本和她本身一样鼎鼎大名—**1992 年的 Q-learning 与 2015 年的 DQN**。 




 美中不足，TD Learning 中很容易出现 Over-Estimate（高估）问题，具体原因如下： 




> 
*The max operator in standard Q-learning and DQN use the same values both to select and to evaluate an action. — van Hasselt *





旷世猛将 van Hasselt 先生很喜欢处理 Over-Estimate 问题，他先搞出一个 **Double Q-learning【van Hasselt 2010】**大闹 NIPS，六年后搞出深度学习版本的 **Double DQN【van Hasselt 2016a】**。




# 问题二：控制与最佳策略选择




**control, finding optimal policy **




目前解法有三个流派，一图胜千言：




![VBcD02jFhgk0uEJrCV3Oh5fTWyn1xN0iajnvTdqyPaicEHWx0EDj0V3ibj8IWmnvfxW4icgzxSPicYy3p8PCWe0lXeA](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgk0uEJrCV3Oh5fTWyn1xN0iajnvTdqyPaicEHWx0EDj0V3ibj8IWmnvfxW4icgzxSPicYy3p8PCWe0lXeA/)

△ 图1：台大李宏毅教授的 Slide




1. 最传统的方法是 Value-Based，就是选择有最优 Value 的 Action。最经典方法有：**Q-learning 【Watkins and Dayan 1992】、SARSA 【Sutton and Barto 2017】** 。




2. 后来 Policy-Based 方法引起注意，最开始是 REINFORCE 算法【Williams 1992】，后来策略梯度** Policy Gradient【Sutton 2000】**出现。




3. **最时行的 Actor-Critic **【Barto et al 1983】把两者做了结合。楼上 Sutton 老爷子的好学生、AlphaGo 的总设计师 David Silver 同志提出了 **Deterministic Policy Gradient**，表面上是 PG，实际讲了一堆 AC，这个改进史称 DPG【Silver 2014】。




![VBcD02jFhgnr10ovNWAxoL0Vhk2LRblTEmV1GSGTgIPvhrcICwB1EZWayGY5VhbkoTjl1K3NousonLnalDaibRw](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnr10ovNWAxoL0Vhk2LRblTEmV1GSGTgIPvhrcICwB1EZWayGY5VhbkoTjl1K3NousonLnalDaibRw/)

△ 图2：Actor-Critic 的循环促进过程





# 问题三：不稳定与不收敛问题




**Instability and Divergence when combining off-policy，function approximation，bootstrapping **




早在 1997 年 Tsitsiklis 就证明了**如果 Function Approximator 采用了神经网络这种非线性的黑箱，那么其收敛性和稳定性是无法保证的**。 




分水岭论文** Deep Q-learning Network【Mnih et al 2013】**中提到：虽然我们的结果看上去很好，但是没有任何理论依据（原文很狡猾的反过来说一遍）。 




> 
*This suggests that, despite lacking any theoretical convergence guarantees, our method is able to train large neural networks using a reinforcement learning signal and stochastic gradient descent in stable manner.*





![VBcD02jFhgnr10ovNWAxoL0Vhk2LRblTUrooQBnkxMHjrMicqHxuxicDicCjuhHhUicv8cCoYfibktBQ20vb7ETiblZQ](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnr10ovNWAxoL0Vhk2LRblTUrooQBnkxMHjrMicqHxuxicDicCjuhHhUicv8cCoYfibktBQ20vb7ETiblZQ/)

△ 图3：征服 Atari 游戏的 DQN




DQN 的改良主要依靠两个 Trick： 



**1. 经验回放【Lin 1993】**




虽然做不到完美的独立同分布，但还是要尽力减少数据之间的关联性 。





**2. Target Network【Mnih 2015】**




Estimated Network 和 Target Network 不能同时更新参数，应该另设 Target Network 以保证稳定性。




> 
*Since **the network Q being updated is also used in calculating the target value**, the Q update is prone to divergence.（为什么我们要用 Target Network）*





下面几篇论文都是 DQN 相关话题的： 




1. 经验回放升级版：**Prioritized Experience Replay 【Schaul 2016】**




2. 更好探索策略 【Osband 2016】 




3. DQN 加速 【He 2017a】 




4. 通过平均减少方差与不稳定性 **Averaged-DQN 【Anschel 2017】**




下面跳出 DQN 的范畴：




**Duel DQN【Wang 2016c】**（ICML 2016 最佳论文） 




Tips：阅读此文请掌握 DQN、Double DQN、Prioritized Experience Replay 这三个背景。



- 
**异步算法 A3C **【Mnih 2016】




- 
**TRPO** (Trust Region Policy Optimization)【Schulman 2015】




- 
Distributed Proximal Policy Optimization 【Heess 2017】 




- 
Policy gradient 与 Q-learning 的结合【O'Donoghue 2017、Nachum 2017、 Gu 2017、Schulman 2017】 




- 
GTD 【Sutton 2009a、Sutton 2009b、Mahmood 2014】 




- 
Emphatic-TD 【Sutton 2016】





# 问题四：End-to-End 下的训练感知与控制




**train perception and control jointly end-to-end **



现有解法是 Guided Policy Search 【Levine et al 2016a】。




# 问题五：数据利用效率




**data/sample efficiency **



**现有解法有： **


- 
Q-learning 与 Actor-Critic 




- 
经验回放下的actor-critic 【Wang et al 2017b】 




- 
PGQ，policy gradient and Q-learning 【O'Donoghue et al 2017】 




- 
Q-Prop, policy gradient with off-policy critic 【Gu et al 2017】 




- 
return-based off-policy control, Retrace 【Munos et al 2016】, Reactor 【Gruslyset al 2017】 




- 
learning to learn, 【Duan et al 2017、Wang et al 2016a、Lake et al 2015】





# 问题六：无法取得激励




**reward function not available **



现有解法基本上围绕模仿学习：



- 
吴恩达的逆强化学习【Ng and Russell 2000】 




- 
learn from demonstration 【Hester et al 2017】 




- 
imitation learning with GANs 【Ho and Ermon 2016、Stadie et al 2017】 （附TensorFlow 实现 [1]） 




- 
train dialogue policy jointly with reward model 【Su et al 2016b】





# 问题七：探索-利用问题




**exploration-exploitation tradeoff **




现有解法有： 



- 
unify count-based exploration and intrinsic motivation 【Bellemare et al 2017】 




- 
under-appreciated reward exploration 【Nachum et al 2017】 




- 
deep exploration via bootstrapped DQN 【Osband et al 2016)】 




- 
variational information maximizing exploration 【Houthooft et al 2016】





# 问题八：基于模型的学习




**model-based learning **



现有解法： 



- 
Sutton 老爷子教科书里的经典安利：Dyna-Q 【Sutton 1990】 




- 
model-free 与 model-based 的结合使用【Chebotar et al 2017】





# 问题九：无模型规划




**model-free planning **



比较新的解法有两个： 



**1. Value Iteration Networks【Tamar et al 2016】**是勇夺 NIPS2016 最佳论文头衔的猛文。




知乎上有专门的文章解说：Value iteration Network [2]**，**还有作者的采访：NIPS 2016 最佳论文作者：如何打造新型强化学习观 [3]。VIN 的 TensorFlow 实现 [4]。




![VBcD02jFhgnr10ovNWAxoL0Vhk2LRblTakBH89nvyBRv90SOgDa4b1KEOI0tBhxSibftK0n0OW1un61xRE5jAPQ](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnr10ovNWAxoL0Vhk2LRblTakBH89nvyBRv90SOgDa4b1KEOI0tBhxSibftK0n0OW1un61xRE5jAPQ/)

△ 图4：Value Iteration Network 的框架





2. DeepMind 的 Silver 大神发表的 Predictron 方法 【Silver et al 2016b】，附 TensorFlow 实现 [5]。




# 问题十：它山之石可以攻玉




**focus on salient parts **



@贾扬清 大神曾经说过：



> 
*伯克利人工智能方向的博士生，入学一年以后资格考试要考这几个内容：**强化学习和 Robotics、 统计和概率图模型、 计算机视觉和图像处理、 语音和自然语言处理、 核方法及其理论、 搜索，CSP，逻辑，Planning 等。***



*如果真的想做人工智能，建议都了解一下，不是说都要搞懂搞透，但是至少要达到开会的时候和人在 poster 前面谈笑风生不出错的程度吧。*





因此，一个很好的思路是**从计算机视觉与自然语言处理领域汲取灵感**，例如下文中将会提到的 unsupervised auxiliary learning 方法借鉴了 RNN+LSTM 中的大量操作。 




下面是 CV 和 NLP 方面的几个简介：**物体检测 【Mnih 2014】、机器翻译 【Bahdanau 2015】、图像标注【Xu 2015】、用 Attention 代替 CNN 和 RNN【Vaswani 2017】**等等。




# 问题十一：长时间数据储存




**data storage over long time, separating from computation **



最出名的解法是在 Nature 上大秀一把的 Differentiable Neural Computer【Graves et al 2016】。




# 问题十二：无回报训练




**benefit from non-reward training signals in environments **



现有解法围绕着无监督学习开展：



**Horde 【Sutton et al 2011】 **



极其优秀的工作：



**unsupervised reinforcement and auxiliary learning 【Jaderberg et al 2017】 **



 learn to navigate with unsupervised auxiliary learning 【Mirowski et al 2017】



**大名鼎鼎的 GANs 【Goodfellow et al 2014】**




# 问题十三：跨领域学习




**learn knowledge from different domains **



现有解法全部围绕迁移学习走：【Taylor and Stone, 2009、Pan and Yang 2010、Weiss et al 2016】，learn invariant features to transfer skills 【Gupta et al 2017】。




# 问题十四：有标签数据与无标签数据混合学习




**benefit from both labelled and unlabelled data **



现有解法全部围绕半监督学习：



- 
【Zhu and Goldberg 2009】 




- 
learn with MDPs both with and without reward functions 【Finn et al 2017)】 




- 
learn with expert's trajectories and those may not from experts 【Audiffren et al 2015】





# 问题十五：多层抽象差分空间的表示与推断




**learn, plan, and represent knowledge with spatio-temporal abstraction at multiple levels **



现有解法：


- 
多层强化学习 【Barto and Mahadevan 2003】 




- 
strategic attentive writer to learn macro-actions 【Vezhnevets et al 2016】 




- 
integrate temporal abstraction with intrinsic motivation 【Kulkarni et al 2016】 




- 
stochastic neural networks for hierarchical RL 【Florensa et al 2017】 




- 
lifelong learning with hierarchical RL 【Tessler et al 2017】




# 问题十六：不同任务环境快速适应




**adapt rapidly to new tasks **




现有解法基本上是** learn to learn learn：**


- 
a flexible RNN model to handle a family of RL tasks 【Duan et al 2017、Wang et al 2016a】 




- 
one/few/zero-shot learning 【Duan et al 2017、Johnson et al 2016、 Kaiser et al 2017b、Koch et al 2015、Lake et al 2015、Li and Malik 2017、Ravi and Larochelle, 2017、Vinyals et al 2016】





# 问题十七：巨型搜索空间




**gigantic search space **




现有解法依然是蒙特卡洛搜索，详情可以参考初代 AlphaGo 的实现【Silver et al 2016a】。





# 问题十八：神经网络架构设计




** neural networks architecture design**




 现有的网络架构搜索方法【Baker et al 2017、Zoph and Le 2017】，**其中 Zoph 的工作分量非常重。 **




 新的架构有【Kaiser et al 2017a、Silver et al 2016b、Tamar et al 2016、Vaswani et al 2017、Wang et al 2016c】。



# 相关链接




**[1] imitation learning with GANs 实现**

https://github.com/openai/imitation

**[2] Value iteration Network  **

https://zhuanlan.zhihu.com/p/24478944

**[3] **如何打造新型强化学习观****

http://www.sohu.com/a/121100017_465975

**[4] **Value Iteration Networks 实现****

https://github.com/TheAbhiKumar/tensorflow-value-iteration-networks

**[5] ******Predictron**** 实现****

https://github.com/zhongwen/predictron




#  参考文献




[1] Anschel, O., Baram, N., and Shimkin, N. (2017). Averaged-DQN: Variance reduction and stabilization for deep reinforcement learning. In the International Conference on Machine Learning (ICML).

[2] Audiffren, J., Valko, M., Lazaric, A., and Ghavamzadeh, M. (2015). Maximum entropy semisupervised inverse reinforcement learning. In the International Joint Conference on Artificial Intelligence (IJCAI).

[3] Bahdanau, D., Brakel, P., Xu, K., Goyal, A., Lowe, R., Pineau, J., Courville, A., and Bengio, Y. (2017). An actor-critic algorithm for sequence prediction. In the International Conference on Learning Representations (ICLR).

[4] Baker, B., Gupta, O., Naik, N., and Raskar, R. (2017). Designing neural network architectures using reinforcement learning. In the International Conference on Learning Representations (ICLR).

[5] Barto, A. G. and Mahadevan, S. (2003). Recent advances in hierarchical reinforcement learning. Discrete Event Dynamic Systems, 13(4):341–379.

[6] Barto, A. G., Sutton, R. S., and Anderson, C. W. (1983). Neuronlike elements that can solve difficult learning control problems. IEEE Transactions on Systems, Man, and Cybernetics, 13:835–846

[7] Bellemare, M. G., Danihelka, I., Dabney, W., Mohamed, S.,Lakshminarayanan, B., Hoyer, S., and Munos, R. (2017). The Cramer Distance as a Solution to Biased Wasserstein Gradients. ArXiv e-prints.

[8] Chebotar, Y., Hausman, K., Zhang, M., Sukhatme, G., Schaal, S., and Levine, S. (2017). Combining model-based and model-free updates for trajectory-centric reinforcement learning. In the International Conference on Machine Learning (ICML)

[9] Duan, Y., Andrychowicz, M., Stadie, B. C., Ho, J., Schneider, J.,Sutskever, I., Abbeel, P., and Zaremba, W. (2017). One-Shot Imitation Learning. ArXiv e-prints.

[10] Finn, C., Christiano, P., Abbeel, P., and Levine, S. (2016a). A connection between GANs, inverse reinforcement learning, and energy-based models. In NIPS 2016 Workshop on Adversarial Training.

[11] Florensa, C., Duan, Y., and Abbeel, P. (2017). Stochastic neural networks for hierarchical reinforcement learning. In the International Conference on Learning Representations (ICLR)

[12] Goodfellow, I., Pouget-Abadie, J., Mirza, M., Xu, B., Warde-Farley, D., Ozair, S., Courville, A., , and Bengio, Y. (2014). Generative adversarial nets. In the Annual Conference on Neural Information Processing Systems (NIPS), page 2672?2680.

[13] Graves, A., Wayne, G., Reynolds, M., Harley, T., Danihelka, I., Grabska-Barwinska, A., Col- ´ menarejo, S. G., Grefenstette, E., Ramalho, T., Agapiou, J., nech Badia, A. P., Hermann, K. M., Zwols, Y., Ostrovski, G., Cain, A., King, H., Summerfield, C., Blunsom, P., Kavukcuoglu, K., and Hassabis, D. (2016). Hybrid computing using a neural network with dynamic external memory. Nature, 538:471–476

[14] Gruslys, A., Gheshlaghi Azar, M., Bellemare, M. G., and Munos, R. (2017). The Reactor: A Sample-Efficient Actor-Critic Architecture. ArXiv e-prints

[15] Gu, S., Lillicrap, T., Ghahramani, Z., Turner, R. E., and Levine, S. (2017). Q-Prop: Sampleefficient policy gradient with an off-policy critic. In the International Conference on Learning Representations (ICLR).

[16] Gupta, A., Devin, C., Liu, Y., Abbeel, P., and Levine, S. (2017). Learning invariant feature spaces to transfer skills with reinforcement learning. In the International Conference on Learning Representations (ICLR).

[17] He, F. S., Liu, Y., Schwing, A. G., and Peng, J. (2017a). Learning to play in a day: Faster deep reinforcement learning by optimality tightening. In the International Conference on Learning Representations (ICLR)

[18] Heess, N., TB, D., Sriram, S., Lemmon, J., Merel, J., Wayne, G., Tassa, Y., Erez, T., Wang, Z., Eslami, A., Riedmiller, M., and Silver, D. (2017). Emergence of Locomotion Behaviours in Rich Environments. ArXiv e-prints

[19] Hester, T. and Stone, P. (2017). Intrinsically motivated model learning for developing curious robots. Artificial Intelligence, 247:170–86.

[20] Ho, J. and Ermon, S. (2016). Generative adversarial imitation learning. In the Annual Conference on Neural Information Processing Systems (NIPS).

[21] Houthooft, R., Chen, X., Duan, Y., Schulman, J., Turck, F. D., and Abbeel, P. (2016). Vime: Variational information maximizing exploration. In the Annual Conference on Neural Information Processing Systems (NIPS).

[22] Jaderberg, M., Mnih, V., Czarnecki, W., Schaul, T., Leibo, J. Z., Silver, D., and Kavukcuoglu, K. (2017). Reinforcement learning with unsupervised auxiliary tasks. In the International Conference on Learning Representations (ICLR).

[23] Johnson, M., Schuster, M., Le, Q. V., Krikun, M., Wu, Y., Chen, Z., Thorat, N., Viegas, F., Watten- ´berg, M., Corrado, G., Hughes, M., and Dean, J. (2016). Google’s Multilingual Neural Machine Translation System: Enabling Zero-Shot Translation. ArXive-prints.

[24] Kaiser, L., Gomez, A. N., Shazeer, N., Vaswani, A., Parmar, N., Jones, L., and Uszkoreit, J. (2017a). One Model To Learn Them All. ArXiv e-prints.

[25] Kaiser, Ł., Nachum, O., Roy, A., and Bengio, S. (2017b). Learning to Remember Rare Events. In the International Conference on Learning Representations (ICLR).

[26] Koch, G., Zemel, R., and Salakhutdinov, R. (2015). Siamese neural networks for one-shot image recognition. In the International Conference on Machine Learning (ICML).

[27] Kulkarni, T. D., Narasimhan, K. R., Saeedi, A., and Tenenbaum, J. B. (2016). Hierarchical deep reinforcement learning: Integrating temporal abstraction and intrinsic motivation. In the Annual Conference on Neural Information Processing Systems (NIPS)

[28] Lake, B. M., Salakhutdinov, R., and Tenenbaum, J. B. (2015). Human-level concept learning through probabilistic program induction. Science, 350(6266):1332–1338.

[29] Levine, S., Finn, C., Darrell, T., and Abbeel, P. (2016a). End-to-end training of deep visuomotor policies. The Journal of Machine Learning Research, 17:1–40.

[30] Li, K. and Malik, J. (2017). Learning to optimize. In the International Conference on Learning Representations (ICLR).

[31] Lillicrap, T. P., Hunt, J. J., Pritzel, A., Heess, N., Erez, T., & Tassa, Y., et al. (2015). Continuous control with deep reinforcement learning. Computer Science, 8(6), A187.

[32] Lin, L. J. (1993). Reinforcement learning for robots using neural networks.

[33] Mahmood, A. R., van Hasselt, H., and Sutton, R. S. (2014). Weighted importance sampling for off-policy learning with linear function approximation. In the Annual Conference on Neural Information Processing Systems (NIPS).

[34] Mirowski, P., Pascanu, R., Viola, F., Soyer, H., Ballard, A., Banino, A., Denil, M., Goroshin, R., Sifre, L., Kavukcuoglu, K., Kumaran, D., and Hadsell, R. (2017). Learning to navigate in complex environments. In the International Conference on Learning Representations (ICLR).

[35] Mnih, Volodymyr, Kavukcuoglu, Koray, Silver, David, Graves, Alex, Antonoglou, Ioannis, Wier- stra, Daan, and Riedmiller, Martin. Playing atari with deep reinforcement learning. arXiv preprint arXiv:1312.5602, 2013.

[36] Mnih, V., Heess, N., Graves, A., and Kavukcuoglu, K. (2014). Recurrent models of visual attention. In the Annual Conference on Neural Information Processing Systems (NIPS).

[37] Mnih, V., Kavukcuoglu, K., Silver, D., Rusu, A. A., Veness, J., Bellemare, M. G., Graves, A., Riedmiller, M., Fidjeland, A. K., Ostrovski, G., Petersen, S., Beattie, C., Sadik, A., Antonoglou, I., King, H., Kumaran, D., Wierstra, D., Legg, S., and Hassabis, D. (2015). Human-level control through deep reinforcement learning. Nature, 518(7540):529–533.

[38] Mnih, V., Badia, A. P., Mirza, M., Graves, A., Harley, T., Lillicrap, T. P., Silver, D., and Kavukcuoglu, K. (2016). Asynchronous methods for deep reinforcement learning. In the International Conference on Machine Learning (ICML)

[39] Munos, R., Stepleton, T., Harutyunyan, A., and Bellemare, M. G.(2016). Safe and efficient offpolicy reinforcement learning. In the Annual Conference on Neural Information Processing Systems (NIPS).

[40] Nachum, O., Norouzi, M., and Schuurmans, D. (2017). Improving policy gradient by exploring under-appreciated rewards. In the International Conference on Learning Representations (ICLR).

[41] Nachum, O., Norouzi, M., Xu, K., and Schuurmans, D. (2017). Bridging the Gap Between Value and Policy Based Reinforcement Learning. ArXive-prints.

[42] Ng, A. and Russell, S. (2000).Algorithms for inverse reinforcement learning. In the International Conference on Machine Learning (ICML).

[43] O'Donoghue, B., Munos, R., Kavukcuoglu, K., and Mnih, V. (2017). PGQ: Combining policy gradient and q-learning. In the International Conference on Learning Representations (ICLR).

[44] Osband, I., Blundell, C., Pritzel, A., and Roy, B. V. (2016). Deep exploration via bootstrapped DQN. In the Annual Conference on Neural Information Processing Systems (NIPS).

[45] Pan, S. J. and Yang, Q. (2010). A survey on transfer learning. IEEE Transactions on Knowledge and Data Engineering, 22(10):1345 – 1359.

[46] Ravi, S. and Larochelle, H. (2017). Optimization as a model for few-shot learning. In the International Conference on Learning Representations (ICLR).

[47] Schaul, T., Quan, J., Antonoglou, I., and Silver, D. (2016). Prioritized experience replay. In the International Conference on Learning Representations (ICLR).

[48] Schulman, J., Levine, S., Moritz, P., Jordan, M. I., and Abbeel, P. (2015). Trust region policy optimization. In the International Conference on Machine Learning (ICML).

[49] Schulman, J., Abbeel, P., and Chen, X. (2017). Equivalence Between Policy Gradients and Soft Q-Learning. ArXiv e-prints.

[50] Silver, D., Lever, G., Heess, N., Degris, T., Wierstra, D., & Riedmiller, M. (2014). Deterministic policy gradient algorithms. International Conference on International Conference on Machine Learning (pp.387-395). JMLR.org.

[51] Silver, D., Huang, A., Maddison, C. J., Guez, A., Sifre, L., Van Den Driessche, G., Schrittwieser, J., Antonoglou, I., Panneershelvam, V., Lanctot, M., et al. (2016a). Mastering the game of go with deep neural networks and tree search. Nature, 529(7587):484–489.

[52] Silver, D., van Hasselt, H., Hessel, M., Schaul, T., Guez, A., Harley, T., Dulac-Arnold, G., Reichert, D., Rabinowitz, N., Barreto, A., and Degris, T. (2016b). The predictron: End-to-end learning and planning. In NIPS 2016 Deep Reinforcement Learning Workshop.

[53] Stadie, B. C., Abbeel, P., and Sutskever, I. (2017).Third person imitation learning. In the International Conference on Learning Representations (ICLR).

[54] Sutton, R. S. and Barto, A. G. (2017). Reinforcement Learning: An Introduction (2nd Edition, in preparation). MIT Press.

[55] Sutton, R. S., McAllester, D., Singh, S., and Mansour, Y. (2000). Policy gradient methods for reinforcement learning with function approximation. In the Annual Conference on Neural Information Processing Systems
(NIPS).

[56] Sutton, R. S., Maei, H. R., Precup, D., Bhatnagar, S., Silver, D., Szepesvari, C., and Wiewiora, ´E. (2009a). Fast gradient-descent methods for temporal-difference learning with linear function approximation. In the International Conference on Machine Learning (ICML).

[57] Sutton, R. S., Szepesvari, C., and Maei, H. R. (2009b). A convergent O( ´ n) algorithm for off-policy temporal-difference learning with linear function approximation. In the Annual Conference on Neural Information Processing Systems (NIPS).

[58] Sutton, R. S., Modayil, J., Delp, M., Degris, T., Pilarski, P. M., White, A., and Precup, D. (2011). Horde: A scalable real-time architecture for learning knowledge from unsupervised sensorimotor interaction, , proc. of 10th. In International Conference on Autonomous Agents and Multiagent Systems (AAMAS).

[59] Sutton, R. S., Mahmood, A. R., and White, M. (2016). An emphatic approach to the problem of off-policy temporal-difference learning. The Journal of Machine Learning Research, 17:1–29

[60] Sutton, R. S. (1988). Learning to predict by the methods of temporal differences. Machine Learning,3(1):9–44.

Sutton, R. S. (1990). Integrated architectures for learning, planning, and reacting based on approximating dynamic programming. In the International Conference on Machine Learning (ICML).

[61] Tamar, A., Wu, Y., Thomas, G., Levine, S., and Abbeel, P. (2016). Value iteration networks. In the Annual Conference on Neural Information Processing Systems (NIPS).

[62] Taylor, M. E. and Stone, P. (2009). Transfer learning for reinforcement learning domains: A survey. Journal of Machine Learning Research, 10:1633–1685.

[63] Tessler, C., Givony, S., Zahavy, T., Mankowitz, D. J., and Mannor, S. (2017). A deep hierarchical approach to lifelong learning in minecraft. In the AAAI Conference on Artificial Intelligence (AAAI).

[64] van Hasselt, H. (2010). Double Q-learning. Advances in Neural Information Processing Systems 23:, Conference on Neural Information Processing Systems 2010.

[65] van Hasselt, H., Guez, A., , and Silver, D. (2016a). Deep reinforcement learning with double Qlearning. In the AAAI Conference on Artificial Intelligence (AAAI).

[66] Vaswani, A., Shazeer, N., Parmar, N., Uszkoreit, J., Jones, L., Gomez, A. N., Kaiser, L., and Polosukhin, I. (2017). Attention Is All You Need. ArXiv e-prints.

[67] Vezhnevets, A. S., Mnih, V., Agapiou, J., Osindero, S., Graves, A., Vinyals, O., and Kavukcuoglu, K. (2016). Strategic attentive writer for learning macro-actions. In the Annual Conference on Neural Information Processing Systems (NIPS).

[68] Vinyals, O., Blundell, C., Lillicrap, T., Kavukcuoglu, K., and Wierstra, D. (2016). Matching networks for one shot learning. In the Annual Conference on Neural Information Processing Systems (NIPS).

[69] Wang, J. X., Kurth-Nelson, Z., Tirumala, D., Soyer, H., Leibo, J. Z., Munos, R., Blundell, C., Kumaran, D., and Botvinick, M. (2016a). Learning to reinforcement learn. arXiv:1611.05763v1.

[70] Wang, S. I., Liang, P., and Manning, C. D. (2016b). Learning language games through interaction. In the Association for Computational Linguistics annual meeting (ACL)

[71] Wang, Z., Schaul, T., Hessel, M., van Hasselt, H., Lanctot, M., and de Freitas, N. (2016c). Dueling network architectures for deep reinforcement learning. In the International Conference on Machine Learning (ICML).

[72] Watkins, C. J. C. H. and Dayan, P. (1992). Q-learning. Machine Learning, 8:279–292

[73] Weiss, K., Khoshgoftaar, T. M., and Wang, D. (2016). A survey of transfer learning. Journal of Big Data, 3(9)

Williams, R. J. (1992). Simple statistical gradient-following algorithms for connectionist reinforcement learning. Machine Learning, 8(3):229–256.

[74] Xu, K., Ba, J. L., Kiros, R., Cho, K., Courville, A.,Salakhutdinov, R., Zemel, R. S., and Bengio,Y. (2015). Show, attend and tell: Neural image caption generation with visual attention. In the International Conference on Machine Learning (ICML).

[75] Zhu, X. and Goldberg, A. B. (2009). Introduction to semi-supervised learning. Morgan & Claypool

Zoph, B. and Le, Q. V. (2017). Neural architecture search with reinforcement learning. In the International Conference on Learning Representations (ICLR)







**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「阅读原文」即刻加入社区！**




我是彩蛋



**解锁新功能：热门职位推荐！**




PaperWeekly小程序升级啦




**今日arXiv√猜你喜欢√**热门职位****√****




找全职找实习都不是问题



** 解锁方式 **

1. 识别下方二维码打开小程序

2. 用PaperWeekly社区账号进行登陆

3. 登陆后即可解锁所有功能




** 职位发布 **

请添加小助手微信（**pwbot01**）进行咨询




**长按识别二维码，使用小程序**

*点击阅读原文即可注册




![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnwLopkg177jgoQCbq2j2UJqSZOScYnsaSZf7ibXORdFOUEicycYycARG6V9pvHMyY7jYpdZFKpxcSQ/?)







******关于PaperWeekly******




PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/ePTzepwoNWPuSZ5SBgtleovKV97Gn4cIicAMa4kDTwWw586xyoZVfJn4gWZ7nv4krxKxVjZQ8wWmI1iba4HCia8bg/?)




