# R语言入门视频笔记--4--R的数据输入 - 默槑 - 博客园







# [R语言入门视频笔记--4--R的数据输入](https://www.cnblogs.com/modaidai/p/6653550.html)





 输入

 R的数据输入可以大体三种：

　　　　　　　　1、键盘输出

　　　　　　　　2、从文本文件导入

　　　　　　　　3、从Excel中导入数据



一、从键盘输入

首先创建一个数据框，玩玩嘛，瞎建一个

mydata <- data.frame(age =numeric(0),gender= character(0),weight=numeric(0))    #建一个空数据框，但已经声明过元素类型

1、mydata <- edit(mydata)                        #可以进行编辑

2、fix(mydata)　　　　　　　　　　　　　　　#跟上面一样可以进行编辑，·就是比edit方便一些　　

　　　　还有这c和scan函数

　　　　c就不用多说了 x <- c(1,2,3,4,"abc")这个地球人都知道

　　　　咱们说一个比他稍微高一那么一点点层次的函数

3、scan（）

　　　　你输入scan（）之后它等着你输入 你可以使用空格当做分隔符 好比函数c的逗号分隔符

　　比如: x <- scan()然后你可以输入下面我输入的那些东西中间用空格隔开 

　　1: 1 2 1 3 4 5 6 7 8 9 9 9 9 9
　　15:

　　scan函数也可以读取外部文件，蛋是不推荐。

　　比如 x <- scan(file="xx路径")  加入原文件的数据之间有逗号等分隔符，用scan读入应该去掉这些分隔符。

　　如：x <- scan(file ="xx路径"，sep= ",")

4、data.entry()

　　可以修改文件，蛋是没发保存，没有什么卵用



二、从文本文件输入数据


　　　data <- read.table("路径"，header= TRUE,sep=",")　　　　　　#注意文件需要是ANSI编码的，而且需要注意文件内容是用都还是空格作为间隔的



三、从Excel中输入数据

       data1 <- read.csv("路径，需要注意反斜线和斜线",header=TURE,sep=",")    　　#文件用csv格式的  挺好 

 ##要用/  这个斜线  而不是  \   反斜线

四、从网络读取数据

1、url函数

不知道怎么用 ，以后再说



输出

一、write系列函数

write.table (student,"student.txt")  　　　　#把数据student保存到student.txt文件中

write.csv (student,"student.csv")　　　　　 #把数据student保存到student.csv文件中

还有很多，以后再一一介绍







五、总结

1、R的输入输出有非常多的函数 ，应对不同类型的函数，满足不同的需求慢慢学吧，我也啥都不会呢，都需要慢慢记，再到慢慢会用，还有很长的路要走，一起就加油吧！

2、没有第二个了

3、第三个更没有了














