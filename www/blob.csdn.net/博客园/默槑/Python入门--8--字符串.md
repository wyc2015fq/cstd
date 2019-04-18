# Python入门--8--字符串 - 默槑 - 博客园







# [Python入门--8--字符串](https://www.cnblogs.com/modaidai/p/6840416.html)





一、创建、修改字符串

str1='呆呆 槑槑 木木 林林'

str1[1]　　　　#输出呆

str1[2]　　　　#输出' '，也就是空值

str1=str[:5]+'插入乖呆 '+str1[5:]　　　　#修改字符串



二、字符串函数

str2=‘daixiaochou’

str2.capitalize()　　　　#首字母变大写



str3='SFFS'　　　　　　　　　　#这两个函数都是输出一个结果，并没有改变字符串原本的值

str3.casefold()　　　　　#所有字母变小写

str3.center(40)　　　　#将字符串居中，并在两边补充40个空格


str3.count('FF')　　　　　#查找这个字符串在总字符串中出现的次数

　　　　　　　　　　　　　#可以str2.count('FF',1,3)，1和3是起始、结束的位置

str3.swith('S')　　　　　　#看字符串是不是以大写的s结尾，也有count的参数选择



str4='i'm\tdaidai\ta'

str4.expandtabs()　　　　#把字符串中的制表符（\t），变为空格。

　　　　　　　　　　　　　 #默认参数为tabsize=8  也就是八个空格

str4.find(sub,start,end)　　#看字符串中有没有这个值，并返回它的索引值

　　　　　　　　　　　　　　#找不到返回-1，找到返回索引值

str4.index(sub,start,end)　　#跟find差不多，只是如果找不到的话，index会显示异常

str4.istitle()　　　　　　　　#看str4是不是标题（字符串都是首字母大写，其余小写）

str4.join('12345')　　　　　　#在12、23、34、45之间插入str4

str4.ljust()　　　　　　　　#左对齐，参数与center一样

str4.lower()　　　　　　　　#字符串变为小写

str4.lstrip()　　　　　　　　　　#去掉字符串左边所有空格

str4.partition(sub)　　　　#找到子字符串 sub,把字符串分成一个3 元组 (pre_sub,sub, fol_sub)

　　　　　　　　　　　　　　　　　　#如果字符串不包含 sub 则返回 ('原字符串', '', '')　　　　　　

str4.replace(old,new,[count])　　　　#替换

str4.rfind(sub,[strat,end])　　　　　　#与上面的find功能一样，

　　　　　　　　　　　　　　　　　　　　#只不过是从右边开始找

str4.split(sep=None,maxsplit=-1)　　　#切，默认是以空格为隔断，

　　　　　　　　　　　　　　　　　　　　#分割字符串也可以（‘12’）：以12为分割

str4.splitline()　　　　　　　　　　　　#有换行符（\n）为隔断，分割字符串


str4,startswith()　　　　　　　　　　#参数与endswith一样，看是否以sub为开头

str4.strip()　　　　　　　　　#除去字符串左边所有空格

str4.swapcase()　　　　　　#把字符串中的大写变小写，小写变得大写

str4.title()　　　　　　　　　#把字符串标题化


str4.translate(str.maketrans('a','b'))　　#用法就像前面，把a变b

str4.upper()　　　　　　　　#小写变大写

str4.zfill(width)　　　　　　# 返回长度为 width 的字符串,

　　　　　　　　　　　　　　#原字符串右对齐,前边用 0 填充。



补充：

定义一个跨行的字符串：方法有三

1、'''#$#####$@@$'''　　　　　　#三个引号

2、'23112321434\

　　sdafdsfgsdg\

　　fasfasdffasfas'　　　　　　　　　　#在行的末端加一个'\'


3、'dafdsfdsfaasdf'

　　'dafasfsdafsadf'

　　'sfsdafsafdsafsa'　　　　　　　　　　#每行都在两端加引号














