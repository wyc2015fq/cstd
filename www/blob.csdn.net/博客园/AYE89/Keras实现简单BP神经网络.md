
# Keras实现简单BP神经网络 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[Keras实现简单BP神经网络](https://www.cnblogs.com/eniac1946/p/7424737.html)
|Posted on|2017-08-24 18:35|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7424737)|[收藏](#)
|BP 神经网络的简单实现
|from keras.models import Sequential \#导入模型
from keras.layers.core import Dense \#导入常用层
train_x,train_y \#训练集
test_x,text_y \#测试集
model=Sequential() \#初始化模型
model.add(Dense(3,input_shape=(32,),activation='sigmoid'，init='uniform'))) \#添加一个隐含层，注：只是第一个隐含层需指定input_dim
model.add(Dense(1,activation='sigmoid')) \#添加输出层
model.compile(loss='binary_crossentropy', optimizer='sgd', metrics=['accuracy'])  \# 编译，指定目标函数与优化方法
model.fit(train_x,train_y ) \# 模型训练
model.evaluate(test_x,text_y ) \#模型测试
|常用层
|常用层对应于core模块，core内部定义了一系列常用的网络层，包括全连接、激活层等
|Dense层
|keras.layers.core.Dense(units, activation=None, use_bias=True, kernel_initializer='glorot_uniform', bias_initializer='zeros', kernel_regularizer=None, bias_regularizer=None, activity_regularizer=None, kernel_constraint=None, bias_constraint=None)
|Dense就是|常用的全连接层|，所实现的运算是|output = activation(dot(input, kernel)+bias)|。其中|activation|是逐元素计算的激活函数，|kernel|是本层的权值矩阵，|bias|为偏置向量，只有当|use_bias=True|才会添加。
|如果本层的输入数据的维度大于2，则会先被压为与|kernel|相匹配的大小。
|\#example
|\# as first layer in a sequential model:
||      model = Sequential()
||      model.add(Dense(32, input_shape=(16,)))
||      \# now the model will take as input arrays of shape (*, 16)
||      \# and output arrays of shape (*, 32)
||
||      \# after the first layer, you don't need to specify
||      \# the size of the input anymore:
||      model.add(Dense(32))
|Keras主要包括14个模块，本文主要对Models、layers、Initializations、Activations、Objectives、Optimizers、Preprocessing、metrics共计8个模块分别展开介绍。
|1. Model
|包：keras.models
|这是Keras中最主要的一个模块，用于对各个组件进行组装
|eg:
|from keras.models import Sequential
model=Sequential() \#初始化模型
model.add(...) \#可使用add方法组装组件|2. layers
|包：keras.layers
|该模块主要用于生成神经网络层，包含多种类型，如Core layers、Convolutional layers等
|eg:
|from keras.layers import Dense \#Dense表示Bp层
model.add(Dense(input_dim=3,output_dim=5)) \#加入隐含层|3. Initializations
|包：keras.initializations
|该模块主要负责对模型参数（权重）进行初始化，初始化方法包括：uniform、lecun_uniform、normal、orthogonal、zero、glorot_normal、he_normal等
|详细说明：|[http://keras.io/initializations/](http://keras.io/initializations/)
|eg:
|model.add(Dense(input_dim=3,output_dim=5,init='uniform')) \#加入带初始化（uniform）的隐含层|4. Activations
|包：keras.activations、keras.layers.advanced_activations（新激活函数）
|该模块主要负责为神经层附加激活函数，如linear、sigmoid、hard_sigmoid、tanh、softplus、relu、 softplus以及LeakyReLU等比较新的激活函数
|详细说明：|[http://keras.io/activations/](http://keras.io/activations/)
|eg:
|model.add(Dense(input_dim=3,output_dim=5,activation='sigmoid')) 加入带激活函数（sigmoid）的隐含层|Equal to:
|model.add(Dense(input_dim=3,output_dim=5)) 
model.add(Activation('sigmoid'))|5. Objectives
|包：keras.objectives
|该模块主要负责为神经网络附加损失函数，即目标函数。如mean_squared_error，mean_absolute_error ，squared_hinge，hinge，binary_crossentropy，categorical_crossentropy等，其中binary_crossentropy，categorical_crossentropy是指logloss
|注：目标函数的设定是在模型编译阶段
|详细说明：|[http://keras.io/objectives/](http://keras.io/objectives/)
|eg:
|model.compile(loss='binary_crossentropy', optimizer='sgd') \#loss是指目标函数|6. Optimizers
|包：keras.optimizers
|该模块主要负责设定神经网络的优化方法，如sgd。
|注：优化函数的设定是在模型编译阶段
|详细说明：|[http://keras.io/optimizers/](http://keras.io/optimizers/)
|eg:
|model.compile(loss='binary_crossentropy', optimizer='sgd') \#optimizer是指优化方法|7. Preprocessing
|包：keras.preprocessing.(image\sequence\text)
|数据预处理模块，不过本人目前尚未用过
|8. metrics
|包：keras.metrics
|与sklearn中metrics包基本相同，主要包含一些如binary_accuracy、mae、mse等的评价方法
|eg:
|predict=model.predict_classes(test_x) \#输出预测结果
keras.metrics.binary_accuracy(test_y,predict) \#计算预测精度







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
