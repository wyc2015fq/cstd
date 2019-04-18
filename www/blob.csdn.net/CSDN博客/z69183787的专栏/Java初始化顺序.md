# Java初始化顺序 - z69183787的专栏 - CSDN博客
2013年04月01日 14:08:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：725
个人分类：[Java-知识](https://blog.csdn.net/z69183787/article/category/2175279)
1
  无继承情况下的Java初始化顺序：
****class**Sample**
{
      Sample(String s)
      {
            System.*out*.println(s);
      }
      Sample()
      {
            System.*out*.println("Sample默认构造函数被调用");
      }
}
**class**Test{
**static**Sample*sam*=**new**Sample("静态成员sam初始化");
      Samplesam1=**new**Sample("sam1成员初始化");
**static**{
            System.*out*.println("static块执行");
**if**(*sam*==**null**)System.*out*.println("sam
 is null");
*sam*=**new**Sample("静态块内初始化sam成员变量");
            }
      Test()
      {
            System.*out*.println("Test默认构造函数被调用");
      }
}
//主函数
**public****static****void** 
 main(String  str[])
{
     Testa=**new**Test();
 }
输出结果为：
*静态成员sam初始化    *-----静态成员初始化
*static块执行          *-----静态块被执行
*静态块内初始化sam成员变量*----静态块执行
*sam1成员初始化      -----*普通成员初始化
*Test默认构造函数被调用  -*----构造函数执行
由此可以得出结论：
a静态成员变量首先初始化（注意，Static可以看做一个静态成员，其执行顺序和其在类中申明的顺序有关）
b 普通成员初始化
c 执行构造函数。
对于静态成员（static块可以看成普通的一个静态成员，其并不一定在类初始化时首先执行）和普通成员，其初始化顺序只与其在类定义中的顺序有关，和其他因素无关。
例如下面的例子：
**class**Test{
**static**{
            System.*out*.println("static
 块 1  执行");
            }
**static**Sample*staticSam1*=**new**Sample("静态成员staticSam1初始化");
      Samplesam1=**new**Sample("sam1成员初始化");
**static**Sample*staticSam2*=**new**Sample("静态成员staticSam2初始化");
**static**{
            System.*out*.println("static
 块 2  执行");
            }
      Test()
      {
            System.*out*.println("Test默认构造函数被调用");
      }
      Samplesam2=**new**Sample("sam2成员初始化");
}
则结果为：
static
 块 1  执行
静态成员staticSam1初始化
静态成员staticSam2初始化
static
 块 2  执行 
  
                    --------静态成员
sam1成员初始化
sam2成员初始化
  
                    --------普通成员
Test默认构造函数被调用
  
                    --------构造函数
2 Java继承情况下的初始化顺序：
**class**Test{
**static**{
            System.*out*.println("父类static
 块 1  执行");
            }
**static**Sample*staticSam1*=**new**Sample("父类
 静态成员staticSam1初始化");
      Samplesam1=**new**Sample("父类
 sam1成员初始化");
**static**Sample*staticSam2*=**new**Sample("父类
 静态成员staticSam2初始化");
**static**{
            System.*out*.println("父类
 static 块 2  执行");
            }
      Test()
      {
            System.*out*.println("父类
 Test默认构造函数被调用");
      }
      Samplesam2=**new**Sample("父类
 sam2成员初始化");
}
**class**TestSub**extends**Test
{
**static**Sample*staticSamSub*=**new**Sample("子类
 静态成员staticSamSub初始化");
      TestSub()
      {
            System.*out*.println("子类
 TestSub 默认构造函数被调用");
      }
      Samplesam1=**new**Sample("子类
 sam1成员初始化");
**static**Sample*staticSamSub1*=**new**Sample("子类
 静态成员staticSamSub1初始化");
**static**{System.*out*.println("子类
 static 块  执行");}
      Samplesam2=**new**Sample("子类
 sam2成员初始化");
}
执行结果：
父类
 static 块 1  执行
父类
 静态成员staticSam1初始化
父类
 静态成员staticSam2初始化
父类
 static 块 2  执行
--------父类静态成员初始化
子类
 静态成员staticSamSub初始化
子类
 静态成员staticSamSub1初始化
子类
 static 块  执行
-------子类静态成员初始化
父类
 sam1成员初始化
父类
 sam2成员初始化
父类
 Test默认构造函数被调用       
-------父类普通成员初始化和构造函数执行
子类
 sam1成员初始化
子类
 sam2成员初始化
子类
 TestSub 默认构造函数被调用
-------子类普通成员初始化和构造函数执行
由此得出Java初始化顺序结论：
1 继承体系的所有静态成员初始化（先父类，后子类）
2 父类初始化完成（普通成员的初始化-->构造函数的调用）
3 子类初始化（普通成员-->构造函数）
**Java初始化顺序如图：**

![](https://img-my.csdn.net/uploads/201304/01/1364796940_2896.png)
