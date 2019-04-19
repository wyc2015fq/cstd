# YAML常用语法 - Big Smile - CSDN博客
2018年07月15日 20:34:57[王啸tr1912](https://me.csdn.net/tr1912)阅读数：588
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
        前几天，小编学习了springboot和springcloud中的一些配，其中有很多的博客和书籍都使用的yml文件来作为项目配置文件编写的，小编在github上创建的博客，hexo框架也是用yml编写的，所以小编觉得很有必要学习和总结一下yaml类型文件的格式和写法，所以出此文。
# 一、基本语法
        yaml类型的文件，都是以k-v形式存在的，并且使用换行和空格来表示键值和属性的区分。
     k:(空格)v：表示一对键值对（空格必须有）；
     以空格的缩进来控制层级关系；只要是左对齐的一列数据，都是同一个层级的
```
server:
    port: 8081
    path: /hello
```
注意：属性和值是大小写敏感的。
# 二、值的写法
## 字面量：普通值（数字，字符，布尔）
         key：value  直接写。
- 字符串默认不用加上单引号或者双引号
- "" 加上双引号，表示不会转义里面的特殊字符，特殊字符会作为本身的意思表示：
      name:   "zhangsan \n lisi"：输出；zhangsan 换行  lisi
- '' 单引号会转义特殊字符，特殊字符显示为普通字符
name:   ‘zhangsan \n lisi’：输出；zhangsan \n  lisi
## 对象，（键值对）：
     key：value 在下一行写对象的属性和值的关系，注意缩进  (两个tab)
               对象实体还是k:v形式
```
friends:
        lastName: zhangsan
        age: 20
```
行内写法：
`friends: {lastName: zhangsan,age: 18}`
## 数组（List，set）：
用短横线加上空格来表示一个元素：
- cat
```
pets:
 - cat
 - dog
 - pig
```
行内写法：
`pets: [cat,dog,pig]`
