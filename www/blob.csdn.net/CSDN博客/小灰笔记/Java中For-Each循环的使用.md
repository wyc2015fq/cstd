# Java中For-Each循环的使用 - 小灰笔记 - CSDN博客





2017年03月13日 01:15:27[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：2065








for-each这种集合的遍历方法最初是在Perl中接触的foreach。开始的时候受C语言的影响多少有些不习惯，不过写了两段代码之后就觉得十分适应了，因为这种描述方式与我们的日常表达语言十分相似。再接下来的接触自然是Python，感觉两种方式差不多，但是在Python我又习惯了当子元素作为一个集合的时候。

Java的遍历语句语法介于两者之间，但是由于本身是强数据类型的语言，在子元素的声明上需要加上一个数据类型。当然，或许是由于最新接触，相比之前已经习惯了的Perl和Python，现在的这种使用习惯还是感觉有一点奇怪。

测试代码：

**package**ForEachPack;



**publicclass** ForEachClass {



**publicstaticvoid** main(String[] 
args) {

// **TODO** Auto-generated method stub

**int**[] 
array_data ={1,5,3,6,2,4,7,8,4,12,55,22,99,47,12,53};



**for**(**int**num : array_data)

    {

        System.***out***.print(num +
", ");

    }

    }



}

    程序编译运行后很顺利依次输出了数组的各个元素。

       想想看，Perl以及Python语言的语法简洁的优势很大程度上应该是取决于脚本语言的特性，非强数据类型。但是毕竟脚本语言在运行速度以及可靠性方面跟编译型的语言还是有一定的差距。

       修改一下程序进行二维数组的测试，顺便对Java多维数组可以是不规则的这种特性做一下简单的测试。代码如下：

**package**ForEachPack;



**publicclass** ForEachClass {



**publicstaticvoid** main(String[] 
args) {

// **TODO** Auto-generated method stub

**int**[][] 
array_data ={{1,5,3},{6,2,4,7,8,4},{12,55,22,99,47,12,53}};



**for**(**int**[]
num : array_data)

    {

**for**(**int**sub_num : num)

        {

           System.***out***.print(sub_num +
", ");

        }

        System.***out***.println();

    }

    }



}

       代码编译运行：

1, 5,3, 

6, 2,4, 7, 8, 4, 

12,55, 22, 99, 47, 12, 53, 

       这是不同于C语言的一种很有意思的语言特性。在C语言中，虽然也可以通过模拟对多维数组进行仿真，但是C语言中要求数组中每个对象的数据类型都完全一致。而Java中这部分则相对更加灵活一些，不知道这样是否也会意味着与此相关的Bug增加。进行多维数组的访问相比脚本语言来说还真是有一些不是很方便的地方了，毕竟每一层的遍历都必须的知道相应元素的数据类型才好。



