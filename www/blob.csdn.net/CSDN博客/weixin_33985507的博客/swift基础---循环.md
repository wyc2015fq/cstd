# swift基础---循环 - weixin_33985507的博客 - CSDN博客
2016年12月15日 23:54:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
- 在开发中经常会需要循环
- 常见的循环有:for/while/do while.
- 这里我们只介绍for/while,因为for/while最常见
# for循环的写法
```
//一般写法
       for（int  i= 0， i<10,i ++）{
          print（“老赵你这个”）
        }
    //swift 中的写
      for var i = 0; i < 10; i++ {
       print(i)
        }
      //这里可以回顾下上一结讲到的 开区间 和闭区间
      for i in 0...10 {
       print(老赵你这个）
        }
        //其实这里还有一种开挂式的写法
        //下划线符号_(替代循环中的变量)能够忽略具体的值，并且不提供循环遍历时对值的访问。
        for _ in 0...10{
         print("老赵你这个"）  
        }
```
# while和do while循环
- 
while循环
- while的判断句必须有正确的真假,没有非0即真
- while后面的()可以省略
- 
do while循环
- 
使用repeat关键字来代替了do
```
var sun = 10
   while sun<20 {
      sun += 1  // 需要注意的是在swift3中不能使用 运算符“++”了会报一下错误哦   
                //  '++' is unavailable: it has been removed in Swift 3
     print("小丁")
   }
```
- 
do while
- 
循环使用repeat关键字来代替了do
```
let b = 0
     repeat {
        print(b) b++
      } while b < 20
```
