# Python Numpy shape 基础用法 - YZXnuaa的博客 - CSDN博客
2018年01月24日 14:03:08[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：487

shape函数是numpy.core.fromnumeric中的函数，它的功能是读取矩阵的长度，比如shape[0]就是读取矩阵第一维度的长度。它的输入参数可以使一个整数表示维度，也可以是一个矩阵。这么说你可能不太理解，我们还是用各种例子来说明他的用法：
- 
一维矩阵[1]返回值为(1L,)                   ![](https://imgsa.baidu.com/exp/pic/item/a6c7d7177f3e67093192d5f339c79f3df9dc55e9.jpg)
- 
二维矩阵，返回两个值                                  ![](https://imgsa.baidu.com/exp/pic/item/9304c888d43f8794af72d822d01b0ef41ad53ae9.jpg)
- 
一个单独的数字，返回值为空       ![](https://imgsa.baidu.com/exp/pic/item/5327ce160924ab188d9b713b37fae6cd7a890be9.jpg)
- 
我们还可以将shape作为矩阵的方法来调用，下面先创建了一个单位矩阵e     ![](https://imgsa.baidu.com/exp/pic/item/b25aae51f819861871ca7f8848ed2e738ad4e6ea.jpg)
- 
我们可以快速读取e的形状            ![](https://imgsa.baidu.com/exp/pic/item/58af236d55fbb2fb32a6c81e4d4a20a44723dcea.jpg)
- 
假如我们只想读取e的第一维度长度，使用shape[0]        ![](https://imgsa.baidu.com/exp/pic/item/5202e5f2b211931310ce6fbc67380cd790238dea.jpg)
