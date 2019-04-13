
# spark读取gz文件与parquet文件 - bitcarmanlee的博客 - CSDN博客


2017年08月30日 09:52:55[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：5088



## 1.spark读取hdfs gz的压缩文件
spark1.5以后的版本支持直接读取gz格式的文件，与读取其他纯文本文件没区别。
启动spark shell的交互界面，按读取普通文本文件的方式读取gz文件：
```python
sc.textFile(
```
```python
"/your/path/*.gz"
```
```python
).map{
```
```python
...
```
```python
}
```
以上的代码就能搞定读取gz压缩文件的需求。
## 2.spark读取parquet格式文件
spark天然就支持parquet格式的文件。
同样进入spark shell的交互界面，然后执行以下操作：
```python
val
```
```python
parquetFile = sqlContext.parquetFile(
```
```python
"/your/path/*.parquet"
```
```python
)
```
打印parquet文件的schema：
```python
parquetFile
```
```python
.printSchema
```
```python
()
```
![这里写图片描述](https://img-blog.csdn.net/20170830094918442?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
查看具体内容：
```python
parquetFile
```
```python
.take
```
```python
(
```
```python
2
```
```python
)
```
```python
.foreach
```
```python
(println)
```
就可以查看文件中的具体内容。
## 3.使用Parquet-Tools
[https://github.com/apache/parquet-mr/tree/master/parquet-tools](https://github.com/apache/parquet-mr/tree/master/parquet-tools)
首先下载相应的jar包。
然后在本地执行：
```python
alias
```
```python
parquetview=
```
```python
'hadoop
```
```python
--cluster c3prc-hadoop jar /path/to/your/downloaded/parquet-tools-1.8.1.jar'
```
接下来使用 meta查看schema，head查看数据
```python
parquetview meta /hdfs/path/single/
```
```python
file
```
```python
/faster
parquetview head /hdfs/path/single/
```
```python
file
```
```python
/faster
```

