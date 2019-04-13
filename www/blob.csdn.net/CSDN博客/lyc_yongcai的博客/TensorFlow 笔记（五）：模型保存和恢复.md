
# TensorFlow 笔记（五）：模型保存和恢复 - lyc_yongcai的博客 - CSDN博客


2017年06月15日 10:11:14[刷街兜风](https://me.csdn.net/lyc_yongcai)阅读数：546



# 保存与读取模型
在使用tf来训练模型的时候，难免会出现中断的情况。这时候自然就希望能够将辛辛苦苦得到的中间参数保留下来，不然下次又要重新开始。好在tf官方提供了保存和读取模型的方法。
保存模型的方法：
```python
# 之前是各种构建模型graph的操作(矩阵相乘，sigmoid等等....)
```
```python
saver = tf
```
```python
.train
```
```python
.Saver
```
```python
()
```
```python
# 生成saver
```
```python
with tf
```
```python
.Session
```
```python
() as sess:
    sess
```
```python
.run
```
```python
(tf
```
```python
.global
```
```python
_variables_initializer())
```
```python
# 先对模型初始化
```
```python
# 然后将数据丢入模型进行训练blablabla
```
```python
# 训练完以后，使用saver.save 来保存
```
```python
saver
```
```python
.save
```
```python
(sess,
```
```python
"save_path/file_name"
```
```python
)
```
```python
#file_name如果不存在的话，会自动创建
```
1
2
3
4
5
6
7
8
9
10
11
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
6
7
8
9
10
11
将模型保存好以后，载入也比较方便，如下所示：
```python
saver = tf
```
```python
.train
```
```python
.Saver
```
```python
()
with tf
```
```python
.Session
```
```python
() as sess:
```
```python
#参数可以进行初始化，也可不进行初始化。即使初始化了，初始化的值也会被restore的值给覆盖
```
```python
sess
```
```python
.run
```
```python
(tf
```
```python
.global
```
```python
_variables_initializer())     
    saver
```
```python
.restore
```
```python
(sess,
```
```python
"save_path/file_name"
```
```python
)
```
```python
#会将已经保存的变量值resotre到 变量中。
```
1
2
3
4
5
6
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
6
简单的说，就是**通过saver.save来保存模型，通过saver.restore来加载模****型。**






