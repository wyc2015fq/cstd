# java:for循环删除集合元素的陷阱|浮点数相减少引起的误差|误用八进制|死循环|初始化静态成员变量的陷阱 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2016年12月05日 10:20:17[QuJack](https://me.csdn.net/A784586)阅读数：510








Java开发之道学习总结

                  By和自己的约定 

01-for循环删除集合元素的陷阱

**原因**: for删除集合中的元素会导致神域元素的索引重新排序

**案例：**

**  for**(**int**i=0;i

     tempList.remove(i);

i-=1;//注释这一句，最终大小非0，为1

   }

**解决办法**：1.全部删除用。clear（）；

         2.i-=1;每次删除第一个元素



02-浮点数相减少引起的误差

**原因：**double属于floatingbinary point types，也就是说都double型的数值在相加减的时候，会将数值转换成二进制的数值如10001.10010110011这种表示发再做相加减，但是在转换成二进制代码表示的时候，存储小数部分的位数会有不够的现象，即无限循环小数，这就是造成微差距的主要原因

**案例：**1.doublea=3.00;

       double b=2.60;

       double c=a-b;// 0.3999999999999999**解决办法:**1.用bigDecimal类

          2.转换为整数再运算

//有误差。效率低

**double** a=3.00;

**double** b=2.60;

**double** c=0.00;

c=a-b;

System.out.println(c);

//解决1

BigDecimalnum1=**new**BigDecimal("3.00");

BigDecimalnum2=**new**BigDecimal("2.60");

BigDecimalnum3=num1.subtract(num2);

System.out.println(num3);

//解决2，效率高

**long** longa=(**long**)(a*100);

**long** longb=(**long**)(b*100);

**long** longc=longa-longb;

**double** d=longc/100.0;

System.out.println(d);



![](http://s7.sinaimg.cn/large/002cdaLCgy70VtVPNdk96&690)



03-误用八进制

**原因：**多写个0，十进制变为了八进制；

**案例：int**num1=80;

**int**num2=060;

    System.out.println(num1*num2);//3840**解决办法：**不能误用



05-死循环

**原因：**误用分号

**案例：do...while(...);**

**     while(...){}**//while后不能有分号

**解决办法：**不能误用



06-初始化静态成员变量的陷阱

**原因：**静态成员属于类而不是实例，只被初始化一次

**案例：privatestatic** StringstrName="zhang";

**     public** Test(){}//无参构造

**     public** Test(StringstrName){

          strName=strName;  }  //有参数构造

**    public** StringgetName(){

          return strName；  }



**Test t1 = new Test("li");**

System.out.println(t1.getName());

//想得到默认的“zhang”,得到“li”

**Test t2 = new Test();**

System.out.println(t2.getName());



**解决办法：**1.取消静态关键字static

          2.创建test时候都用有参数构造

          3.改写无参构造函数

**public** Test(){StringstrName="zhang"}



