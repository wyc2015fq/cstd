# Python 里边yield和全局变量global的用法 - sxf_123456的博客 - CSDN博客
2017年08月08日 20:19:58[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：353
*#yield用法,类似迭代生成器*与return不同的是迭代返回值
return只返回一次结束,yield可以返回多个值
**def **g(n):
    **for **i **in **range(n):
        **yield  **i**2
**if **__name__==**'__main__'**:
    **for **i **in **g(5):
        print(i,**":"**)
**def **fab(value):
    a,b = 0,1
**while **a <value:
        **yield **a
        a,b = b,a+b
**if **__name__ == **'__main__'**:
    **for **i **in **fab(20):
        print(i,**":"**)
*#全局变量用法*NUM = 1
**def **global_fun():
    **global  **NUM
    NUM = NUM + 1
**if **__name__ == **'__main__'**:
    global_fun()
    print(NUM)    *#2*
