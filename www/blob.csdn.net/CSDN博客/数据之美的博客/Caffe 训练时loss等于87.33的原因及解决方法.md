# Caffe 训练时loss等于87.33的原因及解决方法 - 数据之美的博客 - CSDN博客
2017年03月06日 18:39:23[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：839
如题，在caffe训练时，遇到这个特殊的数字之后，loss会一直就是这个数字。 
网上虽然有很多针对这个问题调参的trick，但少有详细的分析，因此，有必要研究一下caffe的源代码。 
softmax的公式为 
其中x为softmax前一层的输出
softmax的loss计算公式也很简单，就是对softmax之后预测的概率做对数似然函数 
其中y是label，若类别数为N，则y为N维。对于单label情况，N维中只有一维为1，其他为零，计算loss时仅需考虑label中非零那一维即可（实际使用中单label用一个数字记录） 
此时 
在softmax_loss_layer.cpp的原码中，就是由label的非零维直接计算loss的
```
loss -= log(std::max(prob_data[i * dim + label_value * inner_num_ + j],
                           Dtype(FLT_MIN)));
```
- 1
- 2
- 1
- 2
loss的最大值由FLT_MIN得到，FLT_MIN定义为1.17549435E-38F，这个数字的自然对数正好就是 
-87.3356，算loss时需要取负值，结果就能了87.3356。
这说明softmax计算得到概率值出现了零（由于float类型所能表示的最小数值是，比这个值还小的无法表示，只能是零）
而softmax是用指数函数计算的，指数函数的值都是大于零的。因此，我们有理由相信，计算过程中出现了float溢出等异常，出现了inf，nan等异常数值导致softmax输出为零
最后我们发现，当softmax之前的feature值过大时，由于softmax先求指数，会超出float数据范围，成为inf。inf与其他任何数值的和都是inf，softmax在做除法时任何正常范围的数值除以inf都会变为0。然后求loss时log一下就出现了87.3356这样的值。
以下是模拟训练的loss代码，观察feature数值范围对loss的影响
```cpp
#include <iostream>
#include <math.h>
#include <float.h>
using namespace std;
int main()
{
  float f[] = {100, 20};
  cout << "feature: " << f[0] << ", " << f[1] << endl;
  float f_exp[] = {expf(f[0]), expf(f[1])};
  cout << "exp: " << f_exp[0] << ", " << f_exp[1] << endl;
  float sum = f_exp[0] + f_exp[1];
  cout << "sum: " << sum << endl;
  float softmax[] = {f_exp[0]/sum, f_exp[1]/sum};
  cout << "softmax: " << softmax[0] << ", " << softmax[1] << endl;
  float loss[] = {-log(max(softmax[0], FLT_MIN)), -log(max(softmax[1], FLT_MIN))};
  cout << "loss: " << loss[0] << ", " << loss[1] << endl;
  return 0;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
解决方法： 
知道了原因，解决时就能对症下药。总体上看，softmax输入的feature由两部分计算得到：一部分是输入数据，另部分是各层权重参数。 
1、观察数据中是否有异常样本或异常label导致数据读取异常
2、调小初始化权重，以便使softmax输入的feature尽可能变小
3、降低学习率，这样就能减小权重参数的波动范围，从而减小权重变大的可能性。这条也是网上出现较多的方法。
4、如果有BN（batch normalization）层，finetune时最好不要冻结BN的参数，否则数据分布不一致时很容易使输出值变的很大。
