# 没有依赖库也能跑机器学习模型！推荐一个强大工具m2cgen - 知乎
# 



> 晓查 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

**m2cgen**（Model 2 Code Generator）是一个轻量级代码生成器，它可以将训练好的机器学习模型转换成无需依赖库的本地代码。目前支持转化为**Python**、**Java**、**C**三种语言。

比如你用PyTorch训练了一个机器学习模型，但是需要把它部署在一台没有安装或者不兼容PyTorch的设备上，应该怎么办？这时候m2cgen能帮你解决问题。



![](https://pic2.zhimg.com/v2-d1be9289289a1f7af7e0358d80efc349_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='938' height='710'></svg>)



有人说，这不就是另一种形式的ONNX嘛？No！

ONNX能做的是将PyTorch训练的模型转换到Caffe2上，代码只是换了一种依赖库。而m2cgen生成的代码**不需要在本地安装任何依赖库**！

## **无需依赖库**

那么给计算机安装上依赖库不就好了吗，为何还要这么麻烦？

如果你在实际中遇到以下几种情况：
- 生产环境没有相应的语言，比如缺失Python runtime；
- 设备性能不够强大，比如微控制器(MCU)，无法安装框架和依赖库，而且数据需要在本地计算，不能传回远程服务器；
- 对预测速度有要求，需要直接调用参数值而不是依赖库；

这时候m2cgen就能派上用场，只要设备能运行Python、C、Jave当中的任何一种语言，你就能直接部署训练好的机器学习模型。

## **运行原理**

理论上，训练好的模型已经有了拟合参数的数值，运行起来也只需要矩阵乘法和一些激活函数。可是为了运行它们，却要先安装体积超过GB的框架和依赖库。

m2cgen项目的实质，是以某种方式将模型的权重、偏置分解为一个额外的依赖或文件，在非常大的模型上实现“代码”和“数据”之间的分离。

总的来说，替代方法就是使用底层的编程语言库，来做一些矩阵数学运算。

目前m2cgen支持的模型种类包括：



![](https://pic1.zhimg.com/v2-c5577a7921387296dda3814be8c655f0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='504'></svg>)



## **使用方法**

m2cgen的安装非常方便，直接用pip：

`$ pip install m2cgen`

在转换Python代码前，需要用import导入m2cgen：


```python
from sklearn.datasets import load_boston
from sklearn import linear_model
import m2cgen as m2c

boston = load_boston()
X, y = boston.data, boston.target

estimator = linear_model.LinearRegression()
estimator.fit(X, y)

code = m2c.export_to_java(estimator)
```


然后在终端用m2cgen命令转换代码：



```bash
m2cgen <pickle_file> --language <language> [--indent <indent>]
         [--class_name <class_name>] [--package_name <package_name>]
         [--recursion-limit <recursion_limit>]
```


pickle_file是你要转换的源代码文件，必填选项—language后面填入你的目标语言，比如上面的Python代码，你想转换成Java，就输入：


```bash
m2cgen <path_to_file> --language java
```


然后它就变成了一串Java代码：


```java
public class Model {

    public static double score(double[] input) {
        return (((((((((((((36.45948838508965) + ((input[0]) * (-0.10801135783679647))) + ((input[1]) * (0.04642045836688297))) + ((input[2]) * (0.020558626367073608))) + ((input[3]) * (2.6867338193449406))) + ((input[4]) * (-17.76661122830004))) + ((input[5]) * (3.8098652068092163))) + ((input[6]) * (0.0006922246403454562))) + ((input[7]) * (-1.475566845600257))) + ((input[8]) * (0.30604947898516943))) + ((input[9]) * (-0.012334593916574394))) + ((input[10]) * (-0.9527472317072884))) + ((input[11]) * (0.009311683273794044))) + ((input[12]) * (-0.5247583778554867));
    }
}
```


## **传送门：**

开源地址：

[BayesWitnesses/m2cgen​github.com![图标](https://pic1.zhimg.com/v2-47d3bb861c8e24342d4b808e8cb4ef2c_ipico.jpg)](https://link.zhihu.com/?target=https%3A//github.com/BayesWitnesses/m2cgen/)
与m2cgen类似的代码转换项目**sklearn porter**，能将训练好的scikit-learn模型转换成Java、C、JavaScript、Go、Ruby代码：

[nok/sklearn-porter​github.com![图标](https://pic1.zhimg.com/v2-22eb1a9c261847d99f701e5a1d0bd62c_ipico.jpg)](https://link.zhihu.com/?target=https%3A//github.com/nok/sklearn-porter)
— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


