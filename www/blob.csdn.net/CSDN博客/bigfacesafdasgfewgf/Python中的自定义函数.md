# Python中的自定义函数 - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 16:51:11[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：573
个人分类：[Python](https://blog.csdn.net/puqutogether/article/category/2595125)









两种情况：
1）

def rev(revlist): #计算逆序数 

    count = 0

    revlen = len(revlist)  #the length of revlist

    for i in range(revlen):  #依次遍历

        for j in range(i+1,revlen):

            if revlist[j] < revlist[i]:  #如果后面的比前面的要小，则算一次逆序

                count += 1

    return count  




调用时：

revnum = rev(revlist)  #计算逆序数




2)

def share(tar,temp,keywords): 

...

...

...

     return count,revnum,len(revlist)  #返回相同词数目，逆序数，逆序列表长度




调用时：

shareword,revnum,comnum =share(tarwordlist,temp,keywords)




总结：

自定义函数以def开头，形参用（）包含

return可以返回多个值

调用时取返回值（等号左边）直接列出来

返回值一定要在main函数中有几个返回几个，也就是等于左边变量的个数要和def中return的个数一样，否则就会出现后面的返回值存储在前面的变量中。






















