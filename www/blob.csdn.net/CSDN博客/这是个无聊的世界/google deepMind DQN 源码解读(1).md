# google deepMind DQN 源码解读(1) - 这是个无聊的世界 - CSDN博客





2016年06月12日 16:37:42[lancelot_vim](https://me.csdn.net/lancelot_vim)阅读数：5489








# google DQN 源码解读(1)

首先train_agent.lua文件进入，执行第一行代码:

```
if not dqn then
    require "initenv"
end
```

由于没有dqn这个变量，之后会跳转initenv.lua文件，对环境进行配置

进入这个文件之后，瞬间可以得到一个table: dqn = {} 

之后require一些文件，其中torch,nn,nngraph是torch自带文件，分别是 

1. torch的数据结构以及一些常用函数 

2. 网络模块 

3. 网络显示模块
然后你可以看到代码： `require 'nnutils'`

此后，自然进入nnutils.lua文件

文件源码如下： 
![2016-06-07-164116_599x418_scrot.png-37.8kB](http://static.zybuluo.com/lancelot-vim/1vdp4g36f210zl53ln806hsg/2016-06-07-164116_599x418_scrot.png)
![2016-06-07-164123_423x64_scrot.png-5.3kB](http://static.zybuluo.com/lancelot-vim/8qbap168c6fbzkwiocfjibij/2016-06-07-164123_423x64_scrot.png)
![2016-06-07-164132_376x58_scrot.png-5.6kB](http://static.zybuluo.com/lancelot-vim/6zg4vtsiklzasoxm41ci3bqz/2016-06-07-164132_376x58_scrot.png)
![2016-06-07-164140_639x258_scrot.png-40.4kB](http://static.zybuluo.com/lancelot-vim/msyi6sd1985xpqehalgdvh68/2016-06-07-164140_639x258_scrot.png)
主要是这5个函数：递归映射， 绝对值平均， 最大的绝对值， 得到weight的归一化值，以及得到梯度的归一化值

解读这个文件的代码，应该从最后两个函数开始，注意这两个函数是一样的，所以就以get_weight_norms(module)作为范例 

首先看注释： 在给定的网络中，构建一个平均绝对权重值的字符串给所要使用的模块 

也就是说这个函数是用来得到一个字符串的，这个字符串表达的内容是平均权重值，那么，他是如何实现的呢，注意到代码都是调用了函数recursive_map，并且将一个函数对象作为参数传入了该函数，因此我们要回到recursive_map来理解这里是如何实现的
第一行代码是定义了一个空字符串

之后判断传入的模块里面有没有field或者moudle.moudules这两个对象，注意对于get_weight_norms这个函数调用来说，就是判断有没有moudle[‘weight’]或者module.modules，如果已经有了，那么这好办，就直接把模块名连接在str后面。

之后又是一个判断，有没有moudle[‘weight’],如果有，那么就调用func函数，对于第一次调用recursive_map(module, “weight”, abs_mean)，实际上就是调用了函数abs_mean(w)，这个函数直接返回了weight的绝对值的平均值(注意返回了浮点型，但是对于字符串连接操作也可以直接使用)

然后接下来这个判断是判断是不是有模块，如果有，首先是在字符串后面连接一个”[“，当然最后必然会连接一个”]”，中间部分是一个递归，怎么做的呢？ 

首先遍历模块中的子模块，然后自己调用自己 

local submodule_str = recursive_map(submodule, field, func) 

注意，这里是递归，可以返回一个表达这个子模块的字符串，连接在str后面，注意一个细节是，每个子字符串用空格隔开，也就是这段代码：
```
if i < #module.modules and string.len(submodule_str) > 0 then
    str = str .. " "
```

最后的效果就是，这个模块里面所有的数据，以及子模块里面的数据，都以func这个函数的约定展开成了字符串

然后你就愉快地跳出了这个文件，回到了initenv.lua文件，执行下一行,image是torch自带文件，这个文件定义了对图像操作的一些函数，执行之后进入了’Scale.lua’文件，这个文件里面的代码相对简单，分析如下： 

这个文件先注册一个类nn.Scale，这个类继承于nn.Module类 

我希望你能明白torch.class的用法，从torch官方文档上截图了他们给出的demo 
![2016-06-07-172141_1060x828_scrot.png-74.2kB](http://static.zybuluo.com/lancelot-vim/g5jd22w4o2c5hy5j9saox6v6/2016-06-07-172141_1060x828_scrot.png)
特别地

```
x = image.rgb2y(x)
x = image.scale(x, self.width, self.height, 'bilinear')
```

第一行表示把图像从rgb三通道转换到y亮度通道(可以认为是灰度化)，下面这行是对图像进行双线性插值，把尺寸变换到self.width以及self.height的大小

## NeuralQLearner.lua

回到initenv.lua文件，下一行是require ‘NeuralQLearner’，自然是核心代码定义dqn网路

由于之前已经运行了intienv.lua，有个全局的表dqn，所以进入这个文件后，不执行判断里面的代码，直接执行local nql = torch.class(‘dqn.NeuralQLearner’) 

也就是注册了一个类别:dqn.NeuralQLearner,而这个类别的本地代理名叫做nql

接下来自然是初始化函数：function nql:__init(args) 

对于初始化函数，我想如果你要理解它，应该先去研究run_cpu.sh或者run_gpu.sh 

以下就是它的run_gpu.sh 
![2016-06-07-185605_1830x850_scrot.png-335kB](http://static.zybuluo.com/lancelot-vim/exa254vkealupu9seu79c5p9/2016-06-07-185605_1830x850_scrot.png)
![2016-06-07-185622_1892x394_scrot.png-160kB](http://static.zybuluo.com/lancelot-vim/6hfiqyfj7o3s4fums2g5sei3/2016-06-07-185622_1892x394_scrot.png)
下面是train_agent.lua里面配置命令行的代码 
![2016-06-07-185807_1048x928_scrot.png-213.2kB](http://static.zybuluo.com/lancelot-vim/w00maifey4c4yb54dk5kzixf/2016-06-07-185807_1048x928_scrot.png)

cmd:text()是输出一行，如果带有字符串参数，就会输出字符串 

cmd:option(‘-env’, ”, ‘name of environment to use’)的意思是配置列表中key值为env的值，第二个参数就是你要配置的值，默认为空字符，第三个参数是提示文本，这个会在你配置参数的时候告诉你这个配置参数的含义 

这里一共有22个参数分别代表一下意思： 

1. framework     训练框架名:给你训练的这个东西取一个名字，在shell里面取名为alewrap 

2. env           环境: 游戏名字 

3. game_path     游戏路径 

4. env_params    环境参数：是否是RGB等 

5. pool_frms     帧池： 这个参数下需要配置两个参数：尺寸，类型(你可以看到deepmind在shell里面在前面设置了size和type,后面吧这两个参数统一了，分别是19,20和39行 

6. actrep        重复动作次数，这个参数在第15行配置为了4 

7. random_starts 随机初始位置，这个参数用来设置一个值，使得随机数出现在0到这个值之间，作为每次训练的开始位置 

8. name          给你的训练网络和训练的历史参数保存文件取个名 

9. network       预训练网络名 

10. agent        机器人的名字 

11. agent_params 机器人参数，这个参数需要设置一大堆,分别有（这里就枚举一下比较难理解的参数） 

        lr:学习率 ep:贪心策略退火算法起点  ep_end:贪心退火策略终点， 

其他参数请自行研究
回到函数nql:__init(args)，前面部分都是在对参数进行配置 
![2016-06-12-101209_702x891_scrot.png-125.5kB](http://static.zybuluo.com/lancelot-vim/lzvb82kw9mqmeyxkrbib1b0j/2016-06-12-101209_702x891_scrot.png)
![2016-06-12-101225_797x249_scrot.png-45.8kB](http://static.zybuluo.com/lancelot-vim/d7xv9sz8v1okgdn77o8mvd8r/2016-06-12-101225_797x249_scrot.png)
之后是检测网络是否是一个网络文件，实际上网络文件在先前的shell里面已经配置了，文件名叫做convnet_atari3.lua，后面用pcall(require, self.network)将文件加载进来，我简单介绍一下pcall这个函数的使用

> 
pcall需要传入一个函数对象作为第一个参数，后续参数为传入函数的所需的参数


比如require()这个函数，需要传入一个文件名(string类型)，所有以pcall第一个参数是require,第二个参数是self.network

> 
pcall 有两个返回值，第一个返回值是返回传入函数是否调用成功，为一个boolen类型的对象 

  第二个参数，调用成功返回传入函数调用后的返回值，失败返回错误原因


如果还不清楚，请自行谷歌或者翻看lua文档

所以这里为了确保后续工作不会因为调用require调用失败无法进行返回一个未知的错误，使用了pcall进行函数调用

### convnet.lua

既然进入了网络定义的文件，在这里就阐述一下网络具体是如何定义的 

首先通过pcall调用require(‘convnet_atari3.lua’),进入convnet_atari3.lua文件，这个文件又调用convnet.lua文件 

特别的，在convnet_atari3.lua文件中，定义了网络参数，然后调用了convnet.lua中的create_network(args)函数 

值得注意的是，这个文件本身是一个return function，也就是说，这个文件的返回值是一个函数，这个函数是
```
function(args)
    args.n_units        = {32, 64, 64}
    args.filter_size    = {8, 4, 3}
    args.filter_stride  = {4, 2, 1}
    args.n_hid          = {512}
    args.nl             = nn.Rectifier
    return create_network(args)
end
```

在convnet.lua文件中，只有一个函数，这个函数是用来定义网络的 

第一行是定义网络为串行网络，然后用nn.Reshape()修正输入维度，输入维度为用户传入的维度，是一个Table,用unpack解开，得到Table里面的值 

只有定义函数对象convLayer = nn.SpatialConvolution,如果有用显卡会定义为nn.SpatialConvolutionCUDA，并且会在Reshape层后面加一层维度转换层：nn.Transpose({1,2},{2,3},{3,4}) 

这样就把第一维度换到了最后，其他顺序不变
之后自然是加入卷积层 
![2016-06-12-155239_986x348_scrot.png-53kB](http://static.zybuluo.com/lancelot-vim/9pjlj4ainc9nfsjhzri36tla/2016-06-12-155239_986x348_scrot.png)
值得注意的是，对于卷积层的输出部分对于是否使用显卡有不同的配置 
![2016-06-12-155517_972x304_scrot.png-46.9kB](http://static.zybuluo.com/lancelot-vim/3vus2wicbpvp29m4e2vyzubl/2016-06-12-155517_972x304_scrot.png)

后面是全连接层 
![2016-06-12-155751_761x394_scrot.png-51.7kB](http://static.zybuluo.com/lancelot-vim/wm95zg9l0xlna0v7vnrcm0z8/2016-06-12-155751_761x394_scrot.png)

最终，NeuralQLearner.lua文件中，通过调用 msg, err = pcall(require, self.network)，得到了一个神经网络保存到了err中

实际上可以注意到，如果是其他情况（导致pcall返回失败信息），那么判断是否可以使用torch.load()加载网络，如果还是错误，才抛出错误 

如果加载成功，那么判断self.best和exp.best_model会不会有一个为空，如果都不是空，那么将exp.best_model作为最终网络，如果有一个是空，那么吧exp.model作为最终网络 

当然，如果第一步也就是pcall(require, self.network)已经执行成功，那么直接得到一个神经网络 
![2016-06-12-160734_775x136_scrot.png-18.3kB](http://static.zybuluo.com/lancelot-vim/v5bzy9r0xjh0yv2h94ib5k53/2016-06-12-160734_775x136_scrot.png)

值得注意的是，之前在convnet_atari3.lua文件的这个返回函数，需要传入一个参数，而截图中的调用方式self::network()，实际上是把self作为参数传了进去

之后用同样的方法，加载预处理网路，这个文件很简单，加载了Scale.lua,然后自身就几行 
![2016-06-12-161514_487x197_scrot.png-19.7kB](http://static.zybuluo.com/lancelot-vim/elsaavks0bllou0pwt2ncwpe/2016-06-12-161514_487x197_scrot.png)

最后直接返回一个函数，效果是把图片灰度化，然后拉伸成84 × 84的图

执行到这里之后，执行到了dqn.TransitionTable这个类，因此我们需要转到文件TransitionTable.lua中














