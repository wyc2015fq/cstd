# 关于MapReduce的理解？ - z69183787的专栏 - CSDN博客
2018年02月13日 18:09:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：176
个人分类：[大数据-Hadoop](https://blog.csdn.net/z69183787/article/category/5758435)
其实我们可以从word count这个实例来理解MapReduce。
MapReduce大体上分为六个步骤：
input, split, map, shuffle, reduce, output。
细节描述如下：
 输入(input)：如给定一个文档，
包含如下四行：
 Hello Java 
Hello C 
Hello Java
 Hello C++ 
拆分(split)：
将上述文档中每一行的内容转换为key-value对，
即：
 0 - Hello Java 
1 - Hello C 
2 – Hello Java 
3 - Hello C++ 
3. 映射(map)：
将拆分之后的内容转换成新的key-value对，
即： 
(Hello , 1) 
(Java , 1)
 (Hello , 1) 
(C , 1)
 (Hello , 1) 
(Java , 1) 
(Hello , 1) 
(C++ , 1) 
4. 派发(shuffle)：
将key相同的扔到一起去，
即：
 (Hello , 1) 
(Hello , 1)
 (Hello , 1)
 (Hello , 1) 
(Java , 1)
 (Java , 1)
 (C , 1) 
(C++ , 1) 
注意：这一步需要移动数据，原来的数据可能在不同的datanode上，这一步过后，相同key的数据会被移动到同一台机器上。
最终，它会返回一个list包含各种k-value对，
即：
 { Hello: 1,1,1,1} 
{Java: 1,1} 
{C: 1}
 {C++: 1} 
5. 缩减(reduce)：
把同一个key的结果加在一起。
如：
 (Hello , 4)
 (Java , 2) 
(C , 1) 
(C++,1) 
6. 输出(output): 输出缩减之后的所有结果。
