# 浅析Python里的str.format - Machine Learning with Peppa - CSDN博客





2018年04月16日 19:29:33[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：79








format方法被用于字符串的格式化输出。
` print('{0}+{1}={2}'.format(1,2,1+2))   #in``1+2=3   #out`
可见字符串中大括号内的数字分别对应着format的几个参数。



若省略数字：

```
print('{}+{}={}'.format(1,2,1+2))   #in
```

可以得到同样的输出结果。但是替换顺序默认按照[0],[1],[2]...进行。



若替换{0}和{1}：
`print('{1}+{0}={2}'.format(1,2,1+2))   #in``2+1=3   #out`


输出字符串:
`print('{0} am {1}'.format('i','alex'))  ``i am alex   #out`


输出参数的值:

```
1 length = 4
2 name = 'alex'
3 print('the length of {0} is {1}'.format(name,length))
```
`the length of alex is 4`


精度控制：
`print('{0:.3}'.format(1/3))``0.333`


宽度控制：
`print('{0:7}{1:7}'.format('use','python'))``use    python `


精宽度控制(宽度内居左)：
`print('{0:<7.3}..'.format(1/3))   ``0.333  ..`
其实精宽度控制很类似于C中的printf函数。

同理'>'为居右，'^'为居中。符号很形象。



