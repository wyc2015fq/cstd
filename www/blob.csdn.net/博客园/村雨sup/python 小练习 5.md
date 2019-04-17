# python 小练习 5 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)




Py从小喜欢奇特的东西，而且天生对数字特别敏感，一次偶然的机会，他发现了一个有趣的四位数2992，
这个数，它的十进制数表示，其四位数字之和为2+9+9+2=22，它的十六进制数BB0，其四位数字之和也为22，
同时它的十二进制数表示1894，其四位数字之和也为22，啊哈，真是巧啊。
Py非常喜欢这种四位数，由于他的发现，所以这里我们命名其为Py数。
现在给你一个十进制4位数n，你来判断n是不是Py数，若是，则输出Yes，否则输出No。
如n=2992，则输出Yes； n = 9999，则输出No。
AC 1 def digsum(n,m):         #进制转换都可以用这种方法//加余数
    sum=0
while n>0:
        sum=sum+n%m
        n=n//m           #python中"/"代表浮点数整除，“//”代表传统的整数相除取整数部分   例如3/2 = 0.6666....  3//2 = 0
return(sum)
if digsum(n,10)==digsum(n,16)  and  digsum(n,10)==digsum(n,12):
print 'Yes'
else:
print 'No' 


判断一个数是否是素数
def sushu(n):
for i in range(2,int(n**0.5)+1):
if n%i==0:
return(-1)
else:                          #这个else和for同缩进，意思是如果前面没有能return一个数的话就执行执行else，return 1，学到了。
return(1)










