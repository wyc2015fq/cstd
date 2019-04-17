# cvSave in VS2010 or Linux - Grandyang - 博客园







# [cvSave in VS2010 or Linux](https://www.cnblogs.com/grandyang/p/4355089.html)







cvSave这个函数是OpenCV中用来保存某个数据类型到文件中常用的函数，它原本共有五个参数，但是在VS2010中只需要填前两个，而在Linux必须填满五个，否则会出错，如下：



```
// VS2010
cvSave("m.xml", mat);

// Linux
cvSave("m.xml", mat, NULL, NULL, cvAttrList(0, 0));
```














