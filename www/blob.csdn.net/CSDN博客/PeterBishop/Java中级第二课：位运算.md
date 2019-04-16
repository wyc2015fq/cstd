# Java中级第二课：位运算 - PeterBishop - CSDN博客





2019年01月21日 09:20:44[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：22
个人分类：[Java中级](https://blog.csdn.net/qq_40061421/article/category/8626230)









  这课我们讲位运算。



  位运算是指对于整数将其转化为二进制然后按位进行运算。



  位运算符有七个

![](https://img-blog.csdnimg.cn/20190121091833919.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)





这里仅以位与举例详细说明

A = 60 = 0011 1100

B = 13 = 0000 1101

A&B  =0000 1100

相对应的位都是1结果才会是1

0000 1100的值就是12啦



Demo:
import org.junit.Test;



//位运算

public class BitCalculate {



    @Test

    public void method()

    {

        int a = 60, b = 12;

        System.*out*.println("a&b = " + (a&b));

        //12

    }

}




然后说下 >>>
- >>2就是A按位右移两位空位补零

A是0011 1100，右移两位就是xx00 1111,xx表示空位，然后补0，就是0000 1111，结果就是15啦



