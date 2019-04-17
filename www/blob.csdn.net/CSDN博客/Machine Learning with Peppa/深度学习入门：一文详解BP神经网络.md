# 深度学习入门：一文详解BP神经网络 - Machine Learning with Peppa - CSDN博客





2018年04月19日 16:41:57[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：2484








今天来讲BP神经网络，神经网络在机器学习中应用比较广泛，比如函数逼近，模式识别，分类，数据压缩，数据

挖掘等领域。接下来介绍BP神经网络的原理及实现。



Contents



  1. BP神经网络的认识

  2. 隐含层的选取

  3. 正向传递子过程

  4. 反向传递子过程

  5. BP神经网络的注意点

  6. BP神经网络的C++实现





1. BP神经网络的认识



   BP（Back Propagation）神经网络分为两个过程



      （1）工作信号正向传递子过程

      （2）误差信号反向传递子过程



   在BP神经网络中，单个样本有![](http://images.cnitblog.com/blog/571227/201411/231402442962501.png)个输入，有![](http://images.cnitblog.com/blog/571227/201411/231403217184302.png)个输出，在输入层和输出层之间通常还有若干个隐含层。实际
   上，1989年Robert Hecht-Nielsen证明了对于任何闭区间内的一个连续函数都可以用一个隐含层的BP网

   络来逼近，这就是万能逼近定理。所以一个三层的BP网络就可以完成任意的![](http://images.cnitblog.com/blog/571227/201411/231406073128779.png)维到![](http://images.cnitblog.com/blog/571227/201411/231406211712412.png)维的映射。即这三层分
   别是输入层（I），隐含层（H），输出层（O）。如下图示



![](http://images.cnitblog.com/blog/571227/201411/231429324377605.png)





2. 隐含层的选取



   在BP神经网络中，输入层和输出层的节点个数都是确定的，而隐含层节点个数不确定，那么应该设置为多少

   才合适呢？实际上，隐含层节点个数的多少对神经网络的性能是有影响的，有一个经验公式可以确定隐含层

   节点数目，如下



![](http://images.cnitblog.com/blog/571227/201412/191145471412994.png)



   其中![](http://images.cnitblog.com/blog/571227/201412/191145158911113.png)为隐含层节点数目，![](http://images.cnitblog.com/blog/571227/201411/231434129068554.png)为输入层节点数目，![](http://images.cnitblog.com/blog/571227/201411/231434303909312.png)为输出层节点数目，![](http://images.cnitblog.com/blog/571227/201411/231435141876266.png)为![](http://images.cnitblog.com/blog/571227/201411/231435510155181.png)之间的调节常数。




3. 正向传递子过程



   现在设节点![](http://images.cnitblog.com/blog/571227/201412/191151463763576.png)和节点![](http://images.cnitblog.com/blog/571227/201412/191152193919028.png)之间的权值为![](http://images.cnitblog.com/blog/571227/201412/191153014238502.png)，节点![](http://images.cnitblog.com/blog/571227/201412/191153440631862.png)的阀值为![](http://images.cnitblog.com/blog/571227/201412/191154200946304.png)，每个节点的输出值为![](http://images.cnitblog.com/blog/571227/201412/191156149544061.png)，而每个节点的输出
   值是根据上层所有节点的输出值、当前节点与上一层所有节点的权值和当前节点的阀值还有激活函数来实现

   的。具体计算方法如下



![](http://images.cnitblog.com/blog/571227/201412/191423407359095.png)



   其中![](http://images.cnitblog.com/blog/571227/201412/191203372986732.png)为激活函数，一般选取S型函数或者线性函数。



   正向传递的过程比较简单，按照上述公式计算即可。在BP神经网络中，输入层节点没有阀值。





4. 反向传递子过程



在BP神经网络中，误差信号反向传递子过程比较复杂，它是基于Widrow-Hoff学习规则的。假设输出层

   的所有结果为![](http://images.cnitblog.com/blog/571227/201412/191437012828933.png)，误差函数如下



![](http://images.cnitblog.com/blog/571227/201412/191701487517139.png)



   而BP神经网络的主要目的是反复修正权值和阀值，使得误差函数值达到最小。Widrow-Hoff学习规则

   是通过沿着相对误差平方和的最速下降方向，连续调整网络的权值和阀值，根据梯度下降法，权值矢量

   的修正正比于当前位置上E(w,b)的梯度，对于第![](http://images.cnitblog.com/blog/571227/201412/191448372354752.png)个输出节点有



![](http://images.cnitblog.com/blog/571227/201412/191451343134484.png)

   假设选择激活函数为



![](http://images.cnitblog.com/blog/571227/201412/191459283446336.png)



   对激活函数求导，得到



![](http://images.cnitblog.com/blog/571227/201412/191556189854573.png)



   那么接下来针对![](http://images.cnitblog.com/blog/571227/201412/191500003132603.png)有



![](http://images.cnitblog.com/blog/571227/201412/191702462048025.png)



  其中有

![](http://images.cnitblog.com/blog/571227/201412/191600406735354.png)



   同样对于![](http://images.cnitblog.com/blog/571227/201412/191641548762837.png)有



![](http://images.cnitblog.com/blog/571227/201412/191643504382508.png)



   这就是著名的![](http://images.cnitblog.com/blog/571227/201412/191603072988848.png)学习规则，通过改变神经元之间的连接权值来减少系统实际输出和期望输出的误差，这个规

   则又叫做Widrow-Hoff学习规则或者纠错学习规则。



   上面是对隐含层和输出层之间的权值和输出层的阀值计算调整量，而针对输入层和隐含层和隐含层的阀值调

   整量的计算更为复杂。假设![](http://images.cnitblog.com/blog/571227/201412/191704574236879.png)是输入层第k个节点和隐含层第i个节点之间的权值，那么有



![](http://images.cnitblog.com/blog/571227/201412/191729030012012.png)

   其中有



![](http://images.cnitblog.com/blog/571227/201412/191730391573353.png)



   这样对![](http://images.cnitblog.com/blog/571227/201412/191730562515010.png)学习规则理解更为深刻了吧。



   有了上述公式，根据梯度下降法，那么对于隐含层和输出层之间的权值和阀值调整如下



![](http://images.cnitblog.com/blog/571227/201412/191830324545227.png)



   而对于输入层和隐含层之间的权值和阀值调整同样有



![](http://images.cnitblog.com/blog/571227/201412/191834597664525.png)



   至此BP神经网络的原理基本讲完。





5. BP神经网络的注意点



   BP神经网络一般用于分类或者逼近问题。如果用于分类，则激活函数一般选用Sigmoid函数或者硬极限函

   数，如果用于函数逼近，则输出层节点用线性函数，即![](http://images.cnitblog.com/blog/571227/201412/191908593134685.png)。



   BP神经网络在训练数据时可以采用增量学习或者批量学习。



   增量学习要求输入模式要有足够的随机性，对输入模式的噪声比较敏感，即对于剧烈变化的输入模式，训

   练效果比较差，适合在线处理。批量学习不存在输入模式次序问题，稳定性好，但是只适合离线处理。



   标准BP神经网络的缺陷：



   （1）容易形成局部极小值而得不到全局最优值。

       BP神经网络中极小值比较多，所以很容易陷入局部极小值，这就要求对初始权值和阀值有要求，要使

       得初始权值和阀值随机性足够好，可以多次随机来实现。

   （2）训练次数多使得学习效率低，收敛速度慢。

   （3）隐含层的选取缺乏理论的指导。

   （4）训练时学习新样本有遗忘旧样本的趋势。



   BP算法的改进：



   （1）增加动量项

       引入动量项是为了加速算法收敛，即如下公式



![](http://images.cnitblog.com/blog/571227/201412/201532434541008.png)



       动量因子![](http://images.cnitblog.com/blog/571227/201412/201534002048721.png)一般选取![](http://images.cnitblog.com/blog/571227/201412/201534549071933.png)。


   （2）自适应调节学习率

   （3）引入陡度因子



   通常BP神经网络在训练之前会对数据归一化处理，即将数据映射到更小的区间内，比如[0,1]或[-1,1]。





6. BP神经网络的C++实现



BP神经网络的C++文件如下



![](http://images.cnitblog.com/blog/571227/201412/201350135324704.png)



BP.h：



[cpp][view
 plain](https://blog.csdn.net/acdreamers/article/details/44657439#)[copy](https://blog.csdn.net/acdreamers/article/details/44657439#)



- #ifndef _BP_H_
- #define _BP_H_
- 
- #include <vector>
- 
- #define LAYER    3        //三层神经网络
- #define NUM      10       //每层的最多节点数
- 
- #define A        30.0
- #define B        10.0     //A和B是S型函数的参数
- #define ITERS    1000     //最大训练次数
- #define ETA_W    0.0035   //权值调整率
- #define ETA_B    0.001    //阀值调整率
- #define ERROR    0.002    //单个样本允许的误差
- #define ACCU     0.005    //每次迭代允许的误差
- 
- #define Type double
- #define Vector std::vector
- 
- struct Data  
- {  
-     Vector<Type> x;       //输入数据
-     Vector<Type> y;       //输出数据
- };  
- 
- class BP{  
- 
- public:  
- 
- void GetData(const Vector<Data>);  
- void Train();  
-     Vector<Type> ForeCast(const Vector<Type>);  
- 
- private:  
- 
- void InitNetWork();         //初始化网络
- void GetNums();             //获取输入、输出和隐含层节点数
- void ForwardTransfer();     //正向传播子过程
- void ReverseTransfer(int);  //逆向传播子过程
- void CalcDelta(int);        //计算w和b的调整量
- void UpdateNetWork();       //更新权值和阀值
-     Type GetError(int);         //计算单个样本的误差
-     Type GetAccu();             //计算所有样本的精度
-     Type Sigmoid(const Type);   //计算Sigmoid的值
- 
- private:  
- int in_num;                 //输入层节点数
- int ou_num;                 //输出层节点数
- int hd_num;                 //隐含层节点数
- 
-     Vector<Data> data;          //输入输出数据
- 
-     Type w[LAYER][NUM][NUM];    //BP网络的权值
-     Type b[LAYER][NUM];         //BP网络节点的阀值
- 
-     Type x[LAYER][NUM];         //每个神经元的值经S型函数转化后的输出值，输入层就为原值
-     Type d[LAYER][NUM];         //记录delta学习规则中delta的值
- };  
- 
- #endif  //_BP_H_



BP.cpp：





[cpp][view
 plain](https://blog.csdn.net/acdreamers/article/details/44657439#)[copy](https://blog.csdn.net/acdreamers/article/details/44657439#)



- #include <string.h>
- #include <stdio.h>
- #include <math.h>
- #include <assert.h>
- #include "BP.h"
- 
- //获取训练所有样本数据
- void BP::GetData(const Vector<Data> _data)  
- {  
-     data = _data;  
- }  
- 
- //开始进行训练
- void BP::Train()  
- {  
-     printf("Begin to train BP NetWork!\n");  
-     GetNums();  
-     InitNetWork();  
- int num = data.size();  
- 
- for(int iter = 0; iter <= ITERS; iter++)  
-     {  
- for(int cnt = 0; cnt < num; cnt++)  
-         {  
- //第一层输入节点赋值
- for(int i = 0; i < in_num; i++)  
-                 x[0][i] = data.at(cnt).x[i];  
- 
- while(1)  
-             {  
-                 ForwardTransfer();       
- if(GetError(cnt) < ERROR)    //如果误差比较小，则针对单个样本跳出循环
- break;  
-                 ReverseTransfer(cnt);    
-             }  
-         }  
-         printf("This is the %d th trainning NetWork !\n", iter);  
- 
-         Type accu = GetAccu();  
-         printf("All Samples Accuracy is %lf\n", accu);  
- if(accu < ACCU) break;  
-     }  
-     printf("The BP NetWork train End!\n");  
- }  
- 
- //根据训练好的网络来预测输出值
- Vector<Type> BP::ForeCast(const Vector<Type> data)  
- {  
- int n = data.size();  
-     assert(n == in_num);  
- for(int i = 0; i < in_num; i++)  
-         x[0][i] = data[i];  
- 
-     ForwardTransfer();  
-     Vector<Type> v;  
- for(int i = 0; i < ou_num; i++)  
-         v.push_back(x[2][i]);  
- return v;  
- }  
- 
- //获取网络节点数
- void BP::GetNums()  
- {  
-     in_num = data[0].x.size();                         //获取输入层节点数
-     ou_num = data[0].y.size();                         //获取输出层节点数
-     hd_num = (int)sqrt((in_num + ou_num) * 1.0) + 5;   //获取隐含层节点数
- if(hd_num > NUM) hd_num = NUM;                     //隐含层数目不能超过最大设置
- }  
- 
- //初始化网络
- void BP::InitNetWork()  
- {  
-     memset(w, 0, sizeof(w));      //初始化权值和阀值为0，也可以初始化随机值
-     memset(b, 0, sizeof(b));  
- }  
- 
- //工作信号正向传递子过程
- void BP::ForwardTransfer()  
- {  
- //计算隐含层各个节点的输出值
- for(int j = 0; j < hd_num; j++)  
-     {  
-         Type t = 0;  
- for(int i = 0; i < in_num; i++)  
-             t += w[1][i][j] * x[0][i];  
-         t += b[1][j];  
-         x[1][j] = Sigmoid(t);  
-     }  
- 
- //计算输出层各节点的输出值
- for(int j = 0; j < ou_num; j++)  
-     {  
-         Type t = 0;  
- for(int i = 0; i < hd_num; i++)  
-             t += w[2][i][j] * x[1][i];  
-         t += b[2][j];  
-         x[2][j] = Sigmoid(t);  
-     }  
- }  
- 
- //计算单个样本的误差
- Type BP::GetError(int cnt)  
- {  
-     Type ans = 0;  
- for(int i = 0; i < ou_num; i++)  
-         ans += 0.5 * (x[2][i] - data.at(cnt).y[i]) * (x[2][i] - data.at(cnt).y[i]);  
- return ans;  
- }  
- 
- //误差信号反向传递子过程
- void BP::ReverseTransfer(int cnt)  
- {  
-     CalcDelta(cnt);     
-     UpdateNetWork();  
- }  
- 
- //计算所有样本的精度
- Type BP::GetAccu()  
- {  
-     Type ans = 0;  
- int num = data.size();  
- for(int i = 0; i < num; i++)  
-     {  
- int m = data.at(i).x.size();  
- for(int j = 0; j < m; j++)  
-             x[0][j] = data.at(i).x[j];  
-         ForwardTransfer();  
- int n = data.at(i).y.size();  
- for(int j = 0; j < n; j++)  
-             ans += 0.5 * (x[2][j] - data.at(i).y[j]) * (x[2][j] - data.at(i).y[j]);  
-     }  
- return ans / num;  
- }  
- 
- //计算调整量
- void BP::CalcDelta(int cnt)  
- {  
- //计算输出层的delta值
- for(int i = 0; i < ou_num; i++)  
-         d[2][i] = (x[2][i] - data.at(cnt).y[i]) * x[2][i] * (A - x[2][i]) / (A * B);  
- //计算隐含层的delta值
- for(int i = 0; i < hd_num; i++)  
-     {  
-         Type t = 0;  
- for(int j = 0; j < ou_num; j++)  
-             t += w[2][i][j] * d[2][j];  
-         d[1][i] = t * x[1][i] * (A - x[1][i]) / (A * B);  
-     }  
- }  
- 
- //根据计算出的调整量对BP网络进行调整
- void BP::UpdateNetWork()  
- {  
- //隐含层和输出层之间权值和阀值调整
- for(int i = 0; i < hd_num; i++)  
-     {  
- for(int j = 0; j < ou_num; j++)  
-             w[2][i][j] -= ETA_W * d[2][j] * x[1][i];   
-     }  
- for(int i = 0; i < ou_num; i++)  
-         b[2][i] -= ETA_B * d[2][i];  
- 
- //输入层和隐含层之间权值和阀值调整
- for(int i = 0; i < in_num; i++)  
-     {  
- for(int j = 0; j < hd_num; j++)  
-             w[1][i][j] -= ETA_W * d[1][j] * x[0][i];  
-     }  
- for(int i = 0; i < hd_num; i++)  
-         b[1][i] -= ETA_B * d[1][i];  
- }  
- 
- //计算Sigmoid函数的值
- Type BP::Sigmoid(const Type x)  
- {  
- return A / (1 + exp(-x / B));  
- }  


Test.cpp：



[cpp][view
 plain](https://blog.csdn.net/acdreamers/article/details/44657439#)[copy](https://blog.csdn.net/acdreamers/article/details/44657439#)



- #include <iostream>
- #include <string.h>
- #include <stdio.h>
- 
- #include "BP.h"
- 
- usingnamespace std;  
- 
- double sample[41][4]=   
- {   
-     {0,0,0,0},   
-     {5,1,4,19.020},   
-     {5,3,3,14.150},   
-     {5,5,2,14.360},   
-     {5,3,3,14.150},   
-     {5,3,2,15.390},   
-     {5,3,2,15.390},   
-     {5,5,1,19.680},   
-     {5,1,2,21.060},   
-     {5,3,3,14.150},   
-     {5,5,4,12.680},   
-     {5,5,2,14.360},   
-     {5,1,3,19.610},   
-     {5,3,4,13.650},   
-     {5,5,5,12.430},   
-     {5,1,4,19.020},   
-     {5,1,4,19.020},   
-     {5,3,5,13.390},   
-     {5,5,4,12.680},   
-     {5,1,3,19.610},   
-     {5,3,2,15.390},   
-     {1,3,1,11.110},   
-     {1,5,2,6.521},   
-     {1,1,3,10.190},   
-     {1,3,4,6.043},   
-     {1,5,5,5.242},   
-     {1,5,3,5.724},   
-     {1,1,4,9.766},   
-     {1,3,5,5.870},   
-     {1,5,4,5.406},   
-     {1,1,3,10.190},   
-     {1,1,5,9.545},   
-     {1,3,4,6.043},   
-     {1,5,3,5.724},   
-     {1,1,2,11.250},   
-     {1,3,1,11.110},   
-     {1,3,3,6.380},   
-     {1,5,2,6.521},   
-     {1,1,1,16.000},   
-     {1,3,2,7.219},   
-     {1,5,3,5.724}   
- };   
- 
- int main()  
- {  
-     Vector<Data> data;  
- for(int i = 0; i < 41; i++)  
-     {  
-         Data t;  
- for(int j = 0; j < 3; j++)  
-             t.x.push_back(sample[i][j]);  
-         t.y.push_back(sample[i][3]);  
-         data.push_back(t);  
-     }  
-     BP *bp = new BP();  
-     bp->GetData(data);  
-     bp->Train();  
- 
- while(1)  
-     {  
-         Vector<Type> in;  
- for(int i = 0; i < 3; i++)  
-         {  
-             Type v;  
-             scanf("%lf", &v);  
-             in.push_back(v);  
-         }  
-         Vector<Type> ou;  
-         ou = bp->ForeCast(in);  
-         printf("%lf\n", ou[0]);  
-     }  
- return 0;  
- }  




Makefile：



[cpp][view
 plain](https://blog.csdn.net/acdreamers/article/details/44657439#)[copy](https://blog.csdn.net/acdreamers/article/details/44657439#)



- Test : BP.h BP.cpp Test.cpp  
-     g++ BP.cpp Test.cpp -o Test  
- 
- clean:  
-     rm Test  











