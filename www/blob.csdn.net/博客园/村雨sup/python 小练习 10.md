# python 小练习 10 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)




给你一个十进制数a，将它转换成b进制数,如果b>10,用大写字母表示（10用A表示，等等）
a为32位整数，2 <= b <= 16
如a=3,b = 2, 则输出11
 AC:

```
dic = {10:'A',11:'B', 12:'C',13:'D',14:'E',15:'F'}
 
def decToother(num, basic):
    rsl = []
    while num != 0:
        mod = int(num % basic)
        if mod >  9:
            rsl.append(dic[mod])
        else:
            rsl.append(str(mod))
        num = num / basic
    return rsl[::-1]     #这样也可以得到进制转换的数，不用乘以10，只要将每次除得的余数放在一个列表里，最后再翻转就ok了。
 
def printRsl(sign,L):
    rsl = ''
    if sign == '-':
        rsl += '-'
    for value in L:  #L中都是字符串
        rsl += value
    print rsl
 
sign = '-' if a < 0 else '+'  #因为正负性从数字转换为字符串也要花点功夫
a = a if a >=0 else -a
 
printRsl(sign,decToother(a,b))
```













