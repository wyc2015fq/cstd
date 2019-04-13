
# numpy.digitize(data, bins, right = False) - 小花生的博客 - CSDN博客


2019年02月21日 14:48:01[Peanut_范](https://me.csdn.net/u013841196)阅读数：57标签：[digitize																](https://so.csdn.net/so/search/s.do?q=digitize&t=blog)[numpy																](https://so.csdn.net/so/search/s.do?q=numpy&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=digitize&t=blog)个人分类：[大道至简																](https://blog.csdn.net/u013841196/article/category/7890941)



得到label：
`import numpy as np
if __name__ == '__main__':
    bins = np.array(range(-45,48,3))
    labels = np.digitize([30,-20,15],bins)-1
    print(labels)`result：
[25  8 20]
---
numpy.digitize(data, bins, right = False)
参数：
`data : numpy数组
bins :一维单调数组，必须是升序或者降序
right：间隔是否包含最右`返回值：x在bins中的位置。

