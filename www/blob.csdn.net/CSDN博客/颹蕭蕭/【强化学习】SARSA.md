
# 【强化学习】SARSA - 颹蕭蕭 - CSDN博客


2019年04月02日 22:44:34[颹蕭蕭](https://me.csdn.net/itnerd)阅读数：24


![在这里插入图片描述](https://img-blog.csdnimg.cn/20190402221201938.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==,size_16,color_FFFFFF,t_70)
`import``matplotlib``.``pylab``as``plt``#%matplotlib inline``import``random``import``gym
env``=``gym``.``make``(``'Taxi-v1'``)``env``.``render``(``)``Q``=``{``}``for``s``in``range``(``env``.``observation_space``.``n``)``:``for``a``in``range``(``env``.``action_space``.``n``)``:``Q``[``(``s``,``a``)``]``=``0.0``def``epsilon_greedy``(``state``,``epsilon``)``:``if``random``.``uniform``(``0``,``1``)``<``epsilon``:``return``env``.``action_space``.``sample``(``)``else``:``return``max``(``list``(``range``(``env``.``action_space``.``n``)``)``,``key``=``lambda``x``:``Q``[``(``state``,``x``)``]``)``alpha``=``0.85``gamma``=``0.90``epsilon``=``1``R``=``[``]``for``i``in``range``(``800``)``:``# we store cumulative reward of each episodes in r``r``=``0``# initialize the state,``state``=``env``.``reset``(``)``# select the action using epsilon-greedy policy``action``=``epsilon_greedy``(``state``,``epsilon``)``while``True``:``# env.render()``# then we perform the action and move to the next state, and receive the reward``nextstate``,``reward``,``done``,``_``=``env``.``step``(``action``)``# again, we select the next action using epsilon greedy policy``nextaction``=``epsilon_greedy``(``nextstate``,``max``(``0.01``,``epsilon``/``(``i``+``1``)``)``)``# we calculate the Q value of previous state using our update rule``Q``[``(``state``,``action``)``]``+=``alpha``*``(``reward``+``gamma``*``Q``[``(``nextstate``,``nextaction``)``]``-``Q``[``(``state``,``action``)``]``)``# finally we update our state and action with next action and next state``action``=``nextaction
        state``=``nextstate``# store the rewards``r``+=``reward``# we will break the loop, if we are at the terminal state of the episode``if``done``:``break``print``(``"total reward: "``,``r``)``R``.``append``(``r``)``env``.``close``(``)``plt``.``plot``(``R``)``plt``.``show``(``)`![在这里插入图片描述](https://img-blog.csdnimg.cn/20190402222213122.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==,size_16,color_FFFFFF,t_70)
sarsa 和 Q-learning 的区别就在一个 max 上：
它们在采取实际动作时，都是用
$$
\epsilon-greedy
$$
策略
在更新 Q 函数时，sarsa 仍采用
$$
\epsilon-greedy
$$
策略得到下一个动作，而 Q-learning 使用的是
$$
greedy
$$
策略。基于这一点，称 sarsa 是 on-policy 的，而 Q-learning 是 off-policy 的。

