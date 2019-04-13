
# python时间时分秒与秒数的互相转换 - anlcy - 博客园






# [python时间时分秒与秒数的互相转换](https://www.cnblogs.com/camilla/p/7161370.html)

#### 一：时间转成秒数
```python
st =
```
```python
"08:30:30"
et =
```
```python
"9:33:33"
```
```python
#方法一
```
```python
def
```
```python
t2s
```
```python
(t):
    h,m,s = t.strip().split(
```
```python
":")
```
```python
return int(h) *
```
```python
3600 + int(m) *
```
```python
60 + int(s)
print(t2s(st))
```
```python
#方法二
```
```python
import datetime
var = (
```
```python
"hours",
```
```python
"minutes",
```
```python
"seconds")
time2sec =
```
```python
lambda x:int(datetime.timedelta(**{k:int(v)
```
```python
for k,v
```
```python
in zip(var,x.strip().split(
```
```python
":"))}).total_seconds())
print(time2sec(st))
```
stackoverflow.com上还有更多的写法
[http://stackoverflow.com/questions/10663720/converting-a-time-string-to-seconds-in-python](http://stackoverflow.com/questions/10663720/converting-a-time-string-to-seconds-in-python)
[http://stackoverflow.com/questions/6402812/how-to-convert-an-hmmss-time-string-to-seconds-in-python](http://stackoverflow.com/questions/6402812/how-to-convert-an-hmmss-time-string-to-seconds-in-python)
#### 二:秒数转成时分秒:
下面的方法是从stackoverflow上抄过来的。
[http://stackoverflow.com/questions/775049/python-time-seconds-to-hms](http://stackoverflow.com/questions/775049/python-time-seconds-to-hms)
```python
m,
```
```python
s = divmod(seconds,
```
```python
60)
h,
```
```python
m = divmod(
```
```python
m,
```
```python
60)
```
```python
print (
```
```python
"%02d:%02d:%02d" % (h,
```
```python
m,
```
```python
s))
```





