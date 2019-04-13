
# python tricks —— datetime 删除日期中的前导 0 - Zhang's Wikipedia - CSDN博客


2018年01月29日 00:33:56[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：1473



```python
>>
```
```python
from
```
```python
datetime
```
```python
import
```
```python
datetime
>> now = datetime.now()
>> now.strftime(
```
```python
'%Y-%m-%d'
```
```python
)
```
```python
'2018-01-29'
```
如果此时我们想转换为`2018-1-29`的形式，有如下的两种方式：
法 1：分别转换为 int 型以消除前导 0：
year =str(int(now.strftime('%Y')))
month =str(int(now.strftime('%m')))
day =str(int(now.strftime('%d')))
year+'-'+month+'-'+day
法二：使用 datetime 下的结构体：
>>now=now.timetuple()
>>nowtime.struct_time(tm_year=2018, tm_mon=1, tm_mday=29, tm_hour=0, tm_min=24, tm_sec=0, tm_wday=0, tm_yday=29, tm_isdst=-1)
>>year= str(now.tm_year)
>>month= str(now.tm_mon)
>>day= str(now.tm_mday)

