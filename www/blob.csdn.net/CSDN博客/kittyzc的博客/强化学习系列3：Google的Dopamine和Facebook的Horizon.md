# 强化学习系列3：Google的Dopamine和Facebook的Horizon - kittyzc的博客 - CSDN博客





2018年11月28日 21:06:01[IE06](https://me.csdn.net/kittyzc)阅读数：384
所属专栏：[强化学习系列](https://blog.csdn.net/column/details/33845.html)









# 1. 简介

除了OpenAI，还有其他巨头在做强化学习的工作（尤其是深度强化学习），这里介绍Google和Facebook两家开源的工程，分别是Dopamine和Horizon，Github上地址分别为：[https://github.com/google/dopamine](https://github.com/google/dopamine) 和 [https://github.com/facebookresearch/Horizon](https://github.com/facebookresearch/Horizon)

# 2. Dopamine

2018年8月27日上线的Dopamine（多巴胺），据说在新能上和易用性上吊打OpenAI的gym。使用下列语句安装Dopamine：

```python
pip install tensorflow==1.8.0 #使用新版本会报错
pip install atari-py cmake dopamine-rl
```

Dopamine最主要的几个算法是DQN、Rainbow、C51、IQN（ICML 2018上的Implicit Quantile Networks），Atari游戏的测试基准对比图参照[这里](https://google.github.io/dopamine/baselines/plots.html)。另外几个相关文档见下：

ALE基准：
[https://arxiv.org/abs/1207.4708](https://arxiv.org/abs/1207.4708)
DQN（DeepMind）：
[https://storage.googleapis.com/deepmind-media/dqn/DQNNaturePaper.pdf](https://storage.googleapis.com/deepmind-media/dqn/DQNNaturePaper.pdf)

C51（ICML 2017）：
[https://arxiv.org/abs/1707.06887](https://arxiv.org/abs/1707.06887)

Rainbow：
[https://arxiv.org/abs/1710.02298](https://arxiv.org/abs/1710.02298)

Implicit Quantile Networks（ICML 2018）：
[https://arxiv.org/abs/1806.06923](https://arxiv.org/abs/1806.06923)

Google自己有一个[Colabs](https://colab.research.google.com/notebooks/welcome.ipynb)项目，是一个在云端运行的免费的 Jupyter 笔记本环境，不需要进行任何设置就可以使用。Dopamine的colab文件夹下，有很多文件可供参考：
[`utils.py`](https://github.com/google/dopamine/blob/master/dopamine/colab/utils.py)包含了进行实验展示的一系列方法。
[agents.ipyn](https://colab.research.google.com/github/google/dopamine/blob/master/dopamine/colab/agents.ipynb)介绍了如何创建[`DQN`](https://github.com/google/dopamine/blob/master/dopamine/agents/dqn/dqn_agent.py) agent或者新建一个agent。
[load_statistics.ipynb](https://colab.research.google.com/github/google/dopamine/blob/master/dopamine/colab/load_statistics.ipynb) 介绍了对统计数据进行加载和分析。
[tensorboard.ipynb](https://colab.research.google.com/github/google/dopamine/blob/master/dopamine/colab/tensorboard.ipynb)介绍了使用tensorboard进行图形化。
[cartpole.ipynb](https://colab.research.google.com/github/google/dopamine/blob/master/dopamine/colab/cartpole.ipynb)介绍了使用DQN和C51对Cartpole进行训练的例子。
下面举一个使用DQN来玩atari的例子。

```python
import numpy as np
import os
from dopamine.agents.dqn import dqn_agent
from dopamine.atari import run_experiment
from dopamine.colab import utils as colab_utils
from absl import flags
BASE_PATH = '/tmp/colab_dope_run' 
GAME = 'Asterix'
LOG_PATH = os.path.join(BASE_PATH, 'basic_agent', GAME)
def create_basic_agent(sess, environment, summary_writer=None):
    return dqn_agent.DQNAgent(sess,num_actions=environment.action_space.n) 
basic_runner = run_experiment.Runner( LOG_PATH,
                                      create_basic_agent,
                                      game_name=GAME,
                                      num_iterations=200,
                                      training_steps=10,
                                      evaluation_steps=10,
                                      max_steps_per_episode=100)
basic_runner.run_experiment()
random_dqn_data = colab_utils.read_experiment(LOG_PATH, verbose=True)
random_dqn_data['agent'] = 'MyRandomDQN'
random_dqn_data['run_number'] = 1
experimental_data[GAME] = experimental_data[GAME].merge(random_dqn_data,how='outer')
import seaborn as sns
import matplotlib.pyplot as plt
fig, ax = plt.subplots(figsize=(16,8))
sns.tsplot(data=experimental_data[GAME], time='iteration', unit='run_number',condition='agent', value='train_episode_returns', ax=ax)
plt.title(GAME)
plt.show()
```

# 3. Horizon介绍

Horizon的后台是facebook，曾将它用在优化网络360°全景视频和智能个性化推荐等实际产品上。它由Python编写，使用Pytorch进行建模和Caffe2进行训练，用于解决一些数据集很大、反馈回路很慢的实际强化学习问题。Horizon平台包含训练深度强化学习算法的工作流，像数据处理、特征转换、分布式训练、反事实策略评估、优化服务等。

Horizon和AlphaGo相比主要的特点是：从工程师指定的动作集合里面进行训练，而不是从零开始学习，避免对现实世界造成严重破坏。用Horizon的文档中介绍的案例，它是batch, off-policy RL。

根据介绍，Horizon支持的算法包括：- Discrete-Action DQN
- Parametric-Action DQN
- Double DQN, Dueling DQN, Dueling Double DQN
- DDPG (DDPG)
- Soft Actor-Critic (SAC)

前面3个是值函数法，后面2个是策略梯度法。






