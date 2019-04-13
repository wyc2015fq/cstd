
# tensorflow 注意事项 - 机器学习的小学生 - CSDN博客


2018年01月02日 19:44:41[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：232


注意事项：
```python
_, l,acc,y1,y2 = session.run([optimizer,loss,accuracy,y_,y],
                                                    feed_dict=feed_dict)
```
在使用tensorflow时，run函数内的变量名和外面的幅值的变量千万不要使用同一个。例如应该：l-loss, acc-accuracy,y1-y_,y2-y

