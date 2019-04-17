# 【强化学习】DQN(Deep reinforcement learning) Basic - zkq_1986的博客 - CSDN博客





2017年08月18日 10:49:01[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：223








DQN(Deep reinforcement learning) Basic




# **1 DQN’s architecture**

![](https://img-blog.csdn.net/20170818104706331)

【input】

84*84*4 image pixels. The input to the neural network consists of an 84*84*4 image produced by the preprocessing map .  

【hidden layer】

The first hidden layer convolves 32 filters of 8*8 with stride 4 with the input image and applies a rectifier nonlinearity31,32. The second hidden layer convolves 64 filters of 4*4 with stride 2, again followed by a rectifier
 nonlinearity. This is followed by a third convolutional layer that convolves 64 filters of 3*3 withstride 1 followed by a rectifier. The final hidden layer is fully-connected and consists of 512 rectifier units.

【output】

The output layer is a fully-connected linear layer with a single output for each valid action. The number of valid actions varied between 4 and 18 on the games we considered.



【loss function】

The loss function(object function) of DQN is

![](https://img-blog.csdn.net/20170818104712501)

in which gamma is the discount factor determining the agent’s horizon, theta are the parameters of the Q-network at iteration i and theta - are the network parameters used to compute the target at iteration i. The target network
 parameters theta - are only updated with the Q-network parameters theta every C steps and are held fixed between individual updates.





# **2 Algorithm**

![](https://img-blog.csdn.net/20170818104716951)

# **3 Conclusion**

DQN use DNN to store policy, pi, which is a sequence of mapping state to action.



**3.2 为什么使用DNN**

解决输入的高维问题





