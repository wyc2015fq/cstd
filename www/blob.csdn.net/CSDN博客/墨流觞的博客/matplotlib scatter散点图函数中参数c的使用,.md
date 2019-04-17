# matplotlib scatter散点图函数中参数c的使用, - 墨流觞的博客 - CSDN博客





2018年09月09日 21:47:04[墨氲](https://me.csdn.net/dss_dssssd)阅读数：714标签：[python																[matplotlib																[scatter](https://so.csdn.net/so/search/s.do?q=scatter&t=blog)
个人分类：[Python numpy](https://blog.csdn.net/dss_dssssd/article/category/8031969)





```python
import matplotlib.pyplot as plt

    x = [1, 2, 3,4, 5, 6]
    y = [0,0,1,1,2,2]

    plt.scatter(x, y, c=y)

    plt.ylim(-1, 3)
    plt.xlim(0,7)
    plt.show()
```

![这里写图片描述](https://img-blog.csdn.net/20180909214218158?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

说明，参数c就是color，赋值为可迭代参数对象，长度与x，y相同，根据值的不同使得（x,y）参数对表现为不同的颜色。简单地说，按x,y值其中某一个值来区分颜色就好， 

比如上边想按照y值来区分，所以直接c=y就可以了， 

又比如画三维图像时，按照z值区分，直接将c=z就好了](https://so.csdn.net/so/search/s.do?q=matplotlib&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)




