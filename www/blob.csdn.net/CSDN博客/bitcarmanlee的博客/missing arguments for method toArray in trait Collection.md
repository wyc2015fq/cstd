
# missing arguments for method toArray in trait Collection - bitcarmanlee的博客 - CSDN博客


2017年09月19日 17:00:20[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：486标签：[scala																](https://so.csdn.net/so/search/s.do?q=scala&t=blog)[spark																](https://so.csdn.net/so/search/s.do?q=spark&t=blog)[toArray																](https://so.csdn.net/so/search/s.do?q=toArray&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=spark&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=scala&t=blog)个人分类：[scala																](https://blog.csdn.net/bitcarmanlee/article/category/6211331)
[
																								](https://so.csdn.net/so/search/s.do?q=scala&t=blog)


spark 任务中有如下一段代码：
```python
.combineByKey
```
```python
((v: String) => {
                val map =
```
```python
new
```
```python
util.TreeMap[String, String]()
                val (timestamp, phone) = (v.split(
```
```python
"\t"
```
```python
)(
```
```python
1
```
```python
), v.split(
```
```python
"\t"
```
```python
)(
```
```python
0
```
```python
))
                map.put(timestamp, phone)
                map
            },
                (x: util.TreeMap[String, String], v: String) => {
                    val (timestamp, phone) = (v.split(
```
```python
"\t"
```
```python
)(
```
```python
1
```
```python
), v.split(
```
```python
"\t"
```
```python
)(
```
```python
0
```
```python
))
```
```python
if
```
```python
(x.size() <
```
```python
20
```
```python
) { x.put(timestamp, phone) }
                    x
                },
                (x: util.TreeMap[String, String], y: util.TreeMap[String, String]) => {
```
```python
if
```
```python
(x.size() <
```
```python
20
```
```python
) { x.putAll(y) }
                    x
                }
            )
```
```python
.
```
```python
map
```
```python
(x => {
                val (imeimd5, phones) = (x._1, x._2.values.toArray[String])
                val arrayBuffer = ArrayBuffer[String]()
                ...
            })
```
编译的时候，会提示如下错误：
```python
error: missing arguments
```
```python
for
```
```python
method
```
```python
toArray
```
```python
in
```
```python
trait
```
```python
Collection
```
```python
;
```
```python
[INFO] follow this
```
```python
method
```
```python
with
```
```python
`_'
```
```python
if
```
```python
you
```
```python
want
```
```python
to
```
```python
treat
```
```python
it
```
```python
as
```
```python
a
```
```python
partially
```
```python
applied
```
```python
function
```
```python
[
```
```python
INFO
```
```python
]
```
```python
val
```
```python
(imeimd5, phones)
```
```python
=
```
```python
(x._1, x._2.values.toArray[String])
```
分析了一下，造成以上错误的主要原因如下：
`TreeMap.values`得到的是一个Collection。而Collection接口中，自己定义有toArray的方法，方法签名为`Object[] toArray();`。这个时候，调用的就是他自己的toArray方法，返回的类型为`Object[]`，并不是我们需要的类型。此时，我们只需要做一个简单的改变即可：
```python
val (imeimd5, phones) = (
```
```python
x
```
```python
._1,
```
```python
x
```
```python
._2
```
```python
.values
```
```python
.asScala
```
```python
.toArray
```
```python
[String])
```
再编译，就没有问题！

