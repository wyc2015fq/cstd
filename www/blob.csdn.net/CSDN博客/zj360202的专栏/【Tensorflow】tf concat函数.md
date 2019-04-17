# 【Tensorflow】tf.concat函数 - zj360202的专栏 - CSDN博客





2017年12月06日 15:24:18[zj360202](https://me.csdn.net/zj360202)阅读数：506标签：[tensorflow																[concat																[tf.concat																[incompatible																[合并](https://so.csdn.net/so/search/s.do?q=合并&t=blog)
个人分类：[tensorflow](https://blog.csdn.net/zj360202/article/category/6872046)






tf.concat是连接两个矩阵的操作


-----------------------------------------------------------------------------------------------------


----   tensorflow 1.3.0 以前


-----------------------------------------------------------------------------------------------------



### `tf.concat(concat_dim, values, name='concat')`
除去name参数用以指定该操作的name，与方法有关的一共两个参数：


`第一个参数concat_dim：必须是一个数，表明在哪一维上连接`

`     如果`concat_dim`是0，那么在某一个shape的第一个维度上连，对应到实际，就是叠放到列上`



[python][view
 plain](http://blog.csdn.net/mao_xiao_feng/article/details/53366163#)[copy](http://blog.csdn.net/mao_xiao_feng/article/details/53366163#)



- t1 = [[1, 2, 3], [4, 5, 6]]  
- t2 = [[7, 8, 9], [10, 11, 12]]  
- tf.concat(0, [t1, t2]) == > [[1, 2, 3], [4, 5, 6], [7, 8, 9], [10, 11, 12]]  

             如果``concat_dim`是1，`那么在某一个shape的第二个维度上连``


``````



[python][view
 plain](http://blog.csdn.net/mao_xiao_feng/article/details/53366163#)[copy](http://blog.csdn.net/mao_xiao_feng/article/details/53366163#)



- t1 = [[1, 2, 3], [4, 5, 6]]  
- t2 = [[7, 8, 9], [10, 11, 12]]  
- tf.concat(1, [t1, t2]) ==> [[1, 2, 3, 7, 8, 9], [4, 5, 6, 10, 11, 12



             如果有更高维，最后连接的依然是指定那个维：


             values[i].shape = [D0, D1, ... Dconcat_dim(i), ...Dn]``连接后就是：``````[D0,
 D1, ... Rconcat_dim, ...Dn]





[python][view
 plain](http://blog.csdn.net/mao_xiao_feng/article/details/53366163#)[copy](http://blog.csdn.net/mao_xiao_feng/article/details/53366163#)



- # tensor t3 with shape [2, 3]
- # tensor t4 with shape [2, 3]
- tf.shape(tf.concat(0, [t3, t4])) ==> [4, 3]  
- tf.shape(tf.concat(1, [t3, t4])) ==> [2, 6]  






`第二个参数values：就是两个或者一组待连接的tensor了`


```

```



```
/×××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××/
```


`这里要注意的是：如果是两个向量，它们是无法调用`

````



[python][view
 plain](http://blog.csdn.net/mao_xiao_feng/article/details/53366163#)[copy](http://blog.csdn.net/mao_xiao_feng/article/details/53366163#)



- tf.concat(1, [t1, t2])  

来连接的，因为它们对应的shape只有一个维度，当然不能在第二维上连了，虽然实际中两个向量可以在行上连，但是放在程序里是会报错的


`如果要连，必须要调用tf.expand_dims来扩维：`

``



[python][view
 plain](http://blog.csdn.net/mao_xiao_feng/article/details/53366163#)[copy](http://blog.csdn.net/mao_xiao_feng/article/details/53366163#)



- t1=tf.constant([1,2,3])  
- t2=tf.constant([4,5,6])  
- #concated = tf.concat(1, [t1,t2])这样会报错
- t1=tf.expand_dims(tf.constant([1,2,3]),1)  
- t2=tf.expand_dims(tf.constant([4,5,6]),1)  
- concated = tf.concat(1, [t1,t2])#这样就是正确的








tf.concat是连接两个矩阵的操作




-----------------------------------------------------------------------------------------------------


----   tensorflow 1.3.0 以后


----   否则会报如下错误


----   Shapes (2, 2, 3) and () are incompatible


-----------------------------------------------------------------------------------------------------



### `tf.concat(values, concat_dim, name='concat')`
除去name参数用以指定该操作的name，与方法有关的一共两个参数：


`第二个参数concat_dim：必须是一个数，表明在哪一维上连接`

`     如果`concat_dim`是0，那么在某一个shape的第一个维度上连，对应到实际，就是叠放到列上`



[python][view
 plain](http://blog.csdn.net/mao_xiao_feng/article/details/53366163#)[copy](http://blog.csdn.net/mao_xiao_feng/article/details/53366163#)



- t1 = [[1, 2, 3], [4, 5, 6]]  
- t2 = [[7, 8, 9], [10, 11, 12]]  
- tf.concat([t1, t2], 0) == > [[1, 2, 3], [4, 5, 6], [7, 8, 9], [10, 11, 12]]  

             如果``concat_dim`是1，`那么在某一个shape的第二个维度上连``


``````



[python][view
 plain](http://blog.csdn.net/mao_xiao_feng/article/details/53366163#)[copy](http://blog.csdn.net/mao_xiao_feng/article/details/53366163#)



- t1 = [[1, 2, 3], [4, 5, 6]]  
- t2 = [[7, 8, 9], [10, 11, 12]]  
- tf.concat([t1, t2], 1) ==> [[1, 2, 3, 7, 8, 9], [4, 5, 6, 10, 11, 12



             如果有更高维，最后连接的依然是指定那个维：


             values[i].shape = [D0, D1, ... Dconcat_dim(i), ...Dn]``连接后就是：``````[D0,
 D1, ... Rconcat_dim, ...Dn]





[python][view
 plain](http://blog.csdn.net/mao_xiao_feng/article/details/53366163#)[copy](http://blog.csdn.net/mao_xiao_feng/article/details/53366163#)



- # tensor t3 with shape [2, 3]
- # tensor t4 with shape [2, 3]
- tf.shape(tf.concat([t3, t4], 0)) ==> [4, 3]  
- tf.shape(tf.concat([t3, t4], 1)) ==> [2, 6]  






`第二个参数values：就是两个或者一组待连接的tensor了`


```

```



```
/×××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××/
```


`这里要注意的是：如果是两个向量，它们是无法调用`

````



[python][view
 plain](http://blog.csdn.net/mao_xiao_feng/article/details/53366163#)[copy](http://blog.csdn.net/mao_xiao_feng/article/details/53366163#)



- tf.concat([t1, t2],
 1)  

来连接的，因为它们对应的shape只有一个维度，当然不能在第二维上连了，虽然实际中两个向量可以在行上连，但是放在程序里是会报错的


`如果要连，必须要调用tf.expand_dims来扩维：`

``



[python][view
 plain](http://blog.csdn.net/mao_xiao_feng/article/details/53366163#)[copy](http://blog.csdn.net/mao_xiao_feng/article/details/53366163#)



- t1=tf.constant([1,2,3])  
- t2=tf.constant([4,5,6])  
- #concated = tf.concat(1, [t1,t2])这样会报错
- t1=tf.expand_dims(tf.constant([1,2,3]),1)  
- t2=tf.expand_dims(tf.constant([4,5,6]),1)  
- concated = tf.concat([t1,t2], 1)#这样就是正确的](https://so.csdn.net/so/search/s.do?q=incompatible&t=blog)](https://so.csdn.net/so/search/s.do?q=tf.concat&t=blog)](https://so.csdn.net/so/search/s.do?q=concat&t=blog)](https://so.csdn.net/so/search/s.do?q=tensorflow&t=blog)




