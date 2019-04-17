# POCO库中文编程参考指南（6）Poco::Timestamp - DoubleLi - 博客园






## 1 类型别名

三个时间戳相关的类型别名，`TimeDiff`表示两个时间戳的差，第二个是以微秒为单位的时间戳，第三个是以 100 纳秒（0.1 微妙）为单位的时间戳：

```
typedef Int64 TimeDiff;   /// difference between two timestamps in microseconds
typedef Int64 TimeVal;    /// monotonic UTC time value in microsecond resolution
typedef Int64 UtcTimeVal; /// monotonic UTC time value in 100 nanosecond resolution
```

## 2 构造函数

当前时间的时间戳：

```
Timestamp();
```

指定时间的时间戳：

```
Timestamp(TimeVal tv);
```

拷贝构造函数：

```
Timestamp(const Timestamp& other);
```

## 3 重载运算符

赋值运算符：

```
Timestamp& operator = (const Timestamp& other);
Timestamp& operator = (TimeVal tv);
```

比较运算符：

```
bool operator == (const Timestamp& ts) const;
bool operator != (const Timestamp& ts) const;
bool operator >  (const Timestamp& ts) const;
bool operator >= (const Timestamp& ts) const;
bool operator <  (const Timestamp& ts) const;
bool operator <= (const Timestamp& ts) const;
```

算术运算符与算术赋值运算符

```
Timestamp  operator +  (TimeDiff d) const;
Timestamp  operator -  (TimeDiff d) const;
TimeDiff   operator -  (const Timestamp& ts) const;
Timestamp& operator += (TimeDiff d);
Timestamp& operator -= (TimeDiff d);
```

## 4 获取不同格式表示的时间戳

获取 std::time_t 格式的时间戳：

```
std::time_t epochTime() const;
```

获取 UTC-based time 格式的时间戳：

```
UtcTimeVal utcTime() const;
```

获取 TimeVal 格式（微秒）的时间戳：

```
TimeVal epochMicroseconds() const;
```

## 5 其他成员函数

交换时间戳：

```
void swap(Timestamp& timestamp);
```

更新时间戳为当前时间：

```
void update();
```

此时时间戳与这个时间戳的差（TimeStamp() - *this）：

```
TimeDiff elapsed() const;
```

判断一段时间是否已经过去：

```
bool isElapsed(TimeDiff interval) const;
```

## 6 静态成员函数

用`std::time_t`对象创建一个`Timestamp`:

```
static Timestamp fromEpochTime(std::time_t t);
```

用`UtcTimeVal`对象创建一个`Timestamp`：

```
static Timestamp fromUtcTime(UtcTimeVal val);
```

返回时间解析度，即 Units per second。因为 Poco::TimeStamp 的最小解析度为微妙，所以该函数都返回 1000000：

```
static TimeVal resolution();
```

-

from：[Blog.CSDN.net/Poechant](http://blog.csdn.net/Poechant)









