# 用Python实现BP神经网络（附代码） - 知乎
# 



***-------插播几句-------***

***七月在线元旦活动将于2018年1月1日零点开启，届时：***

***1，精品课程免费送***

***2，2018年度VIP年会员限时特价抢购***

***3，30份实物大奖先到先得！***

***4，更多福利活动，点击开启>>>[https://www.julyedu.com/sale/pre_new_year](https://link.zhihu.com/?target=https%3A//www.julyedu.com/sale/pre_new_year)***

**用Python实现出来的机器学习算法都是什么样子呢？ 前两期线性回归及逻辑回归项目已发布（见文末链接），今天来讲讲BP神经网络。**




**BP神经网络**

全部代码

> [https://github.com/lawlite19/MachineLearning_Python/blob/master/NeuralNetwok/NeuralNetwork.py](https://link.zhihu.com/?target=https%3A//github.com/lawlite19/MachineLearning_Python/blob/master/NeuralNetwok/NeuralNetwork.py)




**神经网络model**

先介绍个三层的神经网络，如下图所示 

输入层（input layer）有三个units（
![](https://pic1.zhimg.com/v2-5f592a8c737afbfd374b376fe5a95740_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='18' height='14'></svg>)
为补上的bias，通常设为1）
![](https://pic1.zhimg.com/v2-51e7d64f92828deb7fba39f6e20096cc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='26' height='26'></svg>)
表示第j层的第i个激励，也称为为单元unit
![](https://pic2.zhimg.com/v2-9a9930838cbc608ae090482939de0e29_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='24' height='19'></svg>)
为第j层到第j+1层映射的权重矩阵，就是每条边的权重 
![](https://pic4.zhimg.com/v2-b19241fda2d0d5f49f27ada1868a48a3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='632' height='386'></svg>)
所以可以得到：

隐含层：
![](https://pic2.zhimg.com/v2-8ac4dfeb02db44e23149657fc81c620d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='286' height='28'></svg>)![](https://pic2.zhimg.com/v2-21ee55c77c6a2ee07dd496403db98a89_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='286' height='28'></svg>)![](https://pic1.zhimg.com/v2-d43951f7eed630701d33b27bbdd99724_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='286' height='28'></svg>)
输出层
![](https://pic3.zhimg.com/v2-5e9a5d0fb72949f32a15ba4ef9f0c302_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='382' height='28'></svg>)
，

其中，S型函数
![](https://pic2.zhimg.com/v2-a0ece87c6952f109b8cd62443f9d8efd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='96' height='33'></svg>)
，也成为激励函数

可以看出
![](https://pic3.zhimg.com/v2-b50c62ce20693c1fd9637a4fe5f26822_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='23' height='19'></svg>)
为3x4的矩阵，
![](https://pic2.zhimg.com/v2-2a1d5be15fa18c211ad7d3856571ca45_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='24' height='19'></svg>)
为1x4的矩阵
![](https://pic2.zhimg.com/v2-9a9930838cbc608ae090482939de0e29_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='24' height='19'></svg>)
==》j+1的单元数x（j层的单元数+1）




**代价函数**

假设最后输出的
![](https://pic4.zhimg.com/v2-68fd1fcdc80ec54512834be2e8e5fb73_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='92' height='22'></svg>)
，即代表输出层有K个单元 
![](https://pic1.zhimg.com/v2-cfb88483da9a6a07b58f44cffaf31b74_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='507' height='46'></svg>)
，

其中，
![](https://pic4.zhimg.com/v2-f07f1f7606c4a31f98b8959d94c8563b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='65' height='23'></svg>)
代表第i个单元输出与逻辑回归的代价函数
![](https://pic4.zhimg.com/v2-13999c88401e963c5fc51fb09fb0fec7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='439' height='50'></svg>)



差不多，就是累加上每个输出（共有K个输出）




**正则化**

L-->所有层的个数 



![](https://pic3.zhimg.com/v2-68cea105931110c40ba936fdadf0c6a2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='17' height='18'></svg>)
-->第l层unit的个数

正则化后的代价函数为
![](https://pic1.zhimg.com/v2-8656f3d190bac2652cb4bf2982287508_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='546' height='49'></svg>)![](https://pic4.zhimg.com/v2-cf6c3e4e4d1d3cd8aca65bc1234c904f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='9' height='15'></svg>)
共有L-1层，然后是累加对应每一层的theta矩阵，注意不包含加上偏置项对应的theta(0)

正则化后的代价函数实现代码：




> # 代价函数
def nnCostFunction(nn_params,input_layer_size,hidden_layer_size,num_labels,X,y,Lambda):
   length = nn_params.shape[0] # theta的中长度
   # 还原theta1和theta2
   Theta1 = nn_params[0:hidden_layer_size*(input_layer_size+1)].reshape(hidden_layer_size,input_layer_size+1)
   Theta2 = nn_params[hidden_layer_size*(input_layer_size+1):length].reshape(num_labels,hidden_layer_size+1)

   # np.savetxt("Theta1.csv",Theta1,delimiter=',')

   m = X.shape[0]
   class_y = np.zeros((m,num_labels))      # 数据的y对应0-9，需要映射为0/1的关系
   # 映射y
   for i in range(num_labels):
       class_y[:,i] = np.int32(y==i).reshape(1,-1) # 注意reshape(1,-1)才可以赋值

   '''去掉theta1和theta2的第一列，因为正则化时从1开始'''    
   Theta1_colCount = Theta1.shape[1]    
   Theta1_x = Theta1[:,1:Theta1_colCount]
   Theta2_colCount = Theta2.shape[1]    
   Theta2_x = Theta2[:,1:Theta2_colCount]
   # 正则化向theta^2
   term = np.dot(np.transpose(np.vstack((Theta1_x.reshape(-1,1),Theta2_x.reshape(-1,1)))),np.vstack((Theta1_x.reshape(-1,1),Theta2_x.reshape(-1,1))))

   '''正向传播,每次需要补上一列1的偏置bias'''
   a1 = np.hstack((np.ones((m,1)),X))      
   z2 = np.dot(a1,np.transpose(Theta1))    
   a2 = sigmoid(z2)
   a2 = np.hstack((np.ones((m,1)),a2))
   z3 = np.dot(a2,np.transpose(Theta2))
   h  = sigmoid(z3)    
   '''代价'''    
   J = -(np.dot(np.transpose(class_y.reshape(-1,1)),np.log(h.reshape(-1,1)))+np.dot(np.transpose(1-class_y.reshape(-1,1)),np.log(1-h.reshape(-1,1)))-Lambda*term/2)/m   

   return np.ravel(J)




**反向传播BP**

上面正向传播可以计算得到J(θ),使用梯度下降法还需要求它的梯度 

BP反向传播的目的就是求代价函数的梯度

假设4层的神经网络,
![](https://pic2.zhimg.com/v2-94607b25c8485c0ab7dda64c6ceeae65_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='22' height='29'></svg>)
记为-->l层第j个单元的误差
![](https://pic3.zhimg.com/v2-0f9307e96c60e6c1f4e5a2455791c41a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='103' height='31'></svg>)
《===》
![](https://pic4.zhimg.com/v2-2a7236ac9ce357b5415ffafd444d5937_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='99' height='23'></svg>)
（向量化）
![](https://pic2.zhimg.com/v2-b956e80d4309c88d0b0effd241dc60b1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='177' height='23'></svg>)![](https://pic3.zhimg.com/v2-8bba0121b8f0b7d6ef3ef163a810c64e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='176' height='23'></svg>)



没有
![](https://pic4.zhimg.com/v2-5708a06c33878da9e839529eec85c373_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='23' height='19'></svg>)
，因为对于输入没有误差

因为S型函数
![](https://pic3.zhimg.com/v2-c96e635760c8c5256ba33ac689c61146_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='30' height='19'></svg>)
的倒数为：
![](https://pic2.zhimg.com/v2-db285cef8bc18a0b76a2ff06715e21dd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='160' height='19'></svg>)
，

所以上面的
![](https://pic4.zhimg.com/v2-45c0159be66cac1698bffcb13806ddcb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='48' height='23'></svg>)
和
![](https://pic1.zhimg.com/v2-aecee92421f8c6312efb475a232f3378_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='47' height='23'></svg>)
可以在前向传播中计算出来

反向传播计算梯度的过程为：
![](https://pic3.zhimg.com/v2-953bfcbd3892550264492ce8b52995d2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='57' height='29'></svg>)
（
![](https://pic3.zhimg.com/v2-d3582613bcf5ded4bdeebcd83f70edea_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='15' height='15'></svg>)
是大写的
![](https://pic4.zhimg.com/v2-13a2f09282bf94f90c064d1e786ddaff_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='9' height='15'></svg>)
）

for i=1-m:-
![](https://pic1.zhimg.com/v2-694657cd0fba9bd20ba7a4c208fb447c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='67' height='19'></svg>)
-正向传播计算
![](https://pic4.zhimg.com/v2-da48274306afdc1f4213790162ef7e9f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='22' height='19'></svg>)
（l=2,3,4...L）

-反向计算
![](https://pic1.zhimg.com/v2-e234fef7131140ac16f8ebce24213b2c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='27' height='19'></svg>)
、
![](https://pic1.zhimg.com/v2-c63739b2c8c530b7f634e87b18e78de4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='43' height='19'></svg>)
...
![](https://pic1.zhimg.com/v2-c3e50fdd91c912952683de18b903e8fc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='24' height='19'></svg>)
；

-
![](https://pic3.zhimg.com/v2-3d18549fe1d46d65b8df1ba682b4f77a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='159' height='32'></svg>)
-
![](https://pic2.zhimg.com/v2-0bac8cc6ff1e1ceb1b7c5381594df10d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='192' height='31'></svg>)![](https://pic3.zhimg.com/v2-d2b783d78a0ada3d7f29bac034d5bbc2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='224' height='29'></svg>)
最后
![](https://pic4.zhimg.com/v2-70c88a0a8b12f49203cf672b98ecf5f3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='92' height='49'></svg>)
，即得到代价函数的梯度

实现代码：

> # 梯度
def nnGradient(nn_params,input_layer_size,hidden_layer_size,num_labels,X,y,Lambda):
   length = nn_params.shape[0]
   Theta1 = nn_params[0:hidden_layer_size*(input_layer_size+1)].reshape(hidden_layer_size,input_layer_size+1)
   Theta2 = nn_params[hidden_layer_size*(input_layer_size+1):length].reshape(num_labels,hidden_layer_size+1)
   m = X.shape[0]
   class_y = np.zeros((m,num_labels))      # 数据的y对应0-9，需要映射为0/1的关系    
   # 映射y
   for i in range(num_labels):
       class_y[:,i] = np.int32(y==i).reshape(1,-1) # 注意reshape(1,-1)才可以赋值

   '''去掉theta1和theta2的第一列，因为正则化时从1开始'''
   Theta1_colCount = Theta1.shape[1]    
   Theta1_x = Theta1[:,1:Theta1_colCount]
   Theta2_colCount = Theta2.shape[1]    
   Theta2_x = Theta2[:,1:Theta2_colCount]

   Theta1_grad = np.zeros((Theta1.shape))  #第一层到第二层的权重
   Theta2_grad = np.zeros((Theta2.shape))  #第二层到第三层的权重

   Theta1[:,0] = 0;
   Theta2[:,0] = 0;
   '''正向传播，每次需要补上一列1的偏置bias'''
   a1 = np.hstack((np.ones((m,1)),X))
   z2 = np.dot(a1,np.transpose(Theta1))
   a2 = sigmoid(z2)
   a2 = np.hstack((np.ones((m,1)),a2))
   z3 = np.dot(a2,np.transpose(Theta2))
   h  = sigmoid(z3)

   '''反向传播，delta为误差，'''
   delta3 = np.zeros((m,num_labels))
   delta2 = np.zeros((m,hidden_layer_size))
   for i in range(m):
       delta3[i,:] = h[i,:]-class_y[i,:]
       Theta2_grad = Theta2_grad+np.dot(np.transpose(delta3[i,:].reshape(1,-1)),a2[i,:].reshape(1,-1))
       delta2[i,:] = np.dot(delta3[i,:].reshape(1,-1),Theta2_x)*sigmoidGradient(z2[i,:])
       Theta1_grad = Theta1_grad+np.dot(np.transpose(delta2[i,:].reshape(1,-1)),a1[i,:].reshape(1,-1))

   '''梯度'''
   grad = (np.vstack((Theta1_grad.reshape(-1,1),Theta2_grad.reshape(-1,1)))+Lambda*np.vstack((Theta1.reshape(-1,1),Theta2.reshape(-1,1))))/m
   return np.ravel(grad)




**BP可以求梯度的原因**

实际是利用了链式求导法则 

因为下一层的单元利用上一层的单元作为输入进行计算

大体的推导过程如下，最终我们是想预测函数与已知的y非常接近，求均方差的梯度沿着此梯度方向可使代价函数最小化。可对照上面求梯度的过程。 
![](https://pic1.zhimg.com/v2-a1bdc5533d784da6d31efb1af0d3d8b8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='720' height='771'></svg>)



求误差更详细的推导过程： 
![](https://pic3.zhimg.com/v2-c494e688e5033933a39cf4bc8a4448f2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='319'></svg>)



**梯度检查**

检查利用BP求的梯度是否正确 

利用导数的定义验证：
![](https://pic2.zhimg.com/v2-ed7f86da6dcb8b52c556047a2e2da471_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='177' height='39'></svg>)



求出来的数值梯度应该与BP求出的梯度非常接近

验证BP正确后就不需要再执行验证梯度的算法了




实现代码：

> # 检验梯度是否计算正确
# 检验梯度是否计算正确
def checkGradient(Lambda = 0):
   '''构造一个小型的神经网络验证，因为数值法计算梯度很浪费时间，而且验证正确后之后就不再需要验证了'''
   input_layer_size = 3
   hidden_layer_size = 5
   num_labels = 3
   m = 5
   initial_Theta1 = debugInitializeWeights(input_layer_size,hidden_layer_size); 
   initial_Theta2 = debugInitializeWeights(hidden_layer_size,num_labels)
   X = debugInitializeWeights(input_layer_size-1,m)
   y = 1+np.transpose(np.mod(np.arange(1,m+1), num_labels))# 初始化y

   y = y.reshape(-1,1)
   nn_params = np.vstack((initial_Theta1.reshape(-1,1),initial_Theta2.reshape(-1,1)))  #展开theta 
   '''BP求出梯度'''
   grad = nnGradient(nn_params, input_layer_size, hidden_layer_size, 
                    num_labels, X, y, Lambda)  
   '''使用数值法计算梯度'''
   num_grad = np.zeros((nn_params.shape[0]))
   step = np.zeros((nn_params.shape[0]))
   e = 1e-4
   for i in range(nn_params.shape[0]):
       step[i] = e
       loss1 = nnCostFunction(nn_params-step.reshape(-1,1), input_layer_size, hidden_layer_size, 
                             num_labels, X, y, 
                             Lambda)
       loss2 = nnCostFunction(nn_params+step.reshape(-1,1), input_layer_size, hidden_layer_size, 
                             num_labels, X, y, 
                             Lambda)
       num_grad[i] = (loss2-loss1)/(2*e)
       step[i]=0
   # 显示两列比较
   res = np.hstack((num_grad.reshape(-1,1),grad.reshape(-1,1)))
   print res




**权重的随机初始化**

神经网络不能像逻辑回归那样初始化theta为0,因为若是每条边的权重都为0，每个神经元都是相同的输出，在反向传播中也会得到同样的梯度，最终只会预测一种结果。 

所以应该初始化为接近0的数




实现代码

> # 随机初始化权重theta
def randInitializeWeights(L_in,L_out):
   W = np.zeros((L_out,1+L_in))    # 对应theta的权重
   epsilon_init = (6.0/(L_out+L_in))**0.5
   W = np.random.rand(L_out,1+L_in)*2*epsilon_init-epsilon_init # np.random.rand(L_out,1+L_in)产生L_out*(1+L_in)大小的随机矩阵
   return W




**预测**

正向传播预测结果 




实现代码

> # 预测
def predict(Theta1,Theta2,X):
   m = X.shape[0]
   num_labels = Theta2.shape[0]
   #p = np.zeros((m,1))
   '''正向传播，预测结果'''
   X = np.hstack((np.ones((m,1)),X))
   h1 = sigmoid(np.dot(X,np.transpose(Theta1)))
   h1 = np.hstack((np.ones((m,1)),h1))
   h2 = sigmoid(np.dot(h1,np.transpose(Theta2)))

   '''
   返回h中每一行最大值所在的列号
   - np.max(h, axis=1)返回h中每一行的最大值（是某个数字的最大概率）
   - 最后where找到的最大概率所在的列号（列号即是对应的数字）
   '''
   #np.savetxt("h2.csv",h2,delimiter=',')
   p = np.array(np.where(h2[0,:] == np.max(h2, axis=1)[0]))  
   for i in np.arange(1, m):
       t = np.array(np.where(h2[i,:] == np.max(h2, axis=1)[i]))
       p = np.vstack((p,t))
   return p 




**输出结果**

梯度检查： 
![](https://pic1.zhimg.com/v2-acc6257787b52a59a29e318a1467b2f0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='421' height='553'></svg>)






随机显示100个手写数字
![](https://pic4.zhimg.com/v2-f150aa3b287ef6ac4236fa9d7544585f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='429' height='413'></svg>)
显示theta1权重
![](https://pic4.zhimg.com/v2-836779ccae6b68a63987e213ca287ee7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='458' height='400'></svg>)
训练集预测准确度
![](https://pic4.zhimg.com/v2-fc9d4e8f640c838779e57f4113668f77_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='171'></svg>)
归一化后训练集预测准确度
![](https://pic3.zhimg.com/v2-f33875226184faf72b3acba6ba09a2d2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='80'></svg>)



作者：lawlite19

来源：

[https://github.com/lawlite19/MachineLearning_Python#](https://link.zhihu.com/?target=https%3A//github.com/lawlite19/MachineLearning_Python%23)




