
# 数据库的时间类型 与 spring mvc 中 entity 属性类型的对应关系 - lin_tuer的博客 - CSDN博客


2017年11月06日 13:10:15[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：1653


# 数据库中的Date, DateTime, TimeStamp
## Date:
`仅日期值，没有时间部分；MySQL检索并且以'YYYY-MM-DD'格式显示DATE值，支持的范围是'1000-01-01'到'9999-12-31'`
## DateTime:
`包含日期和时间；ySQL检索并且以'YYYY-MM-DD HH:MM:SS'格式显示DATETIME值，支持的范围是'1000-01-01 00:00:00'到'9999-12-31 23:59:59'`
## TimeStamp
`提供一种类型，可以使用它自动地用当前的日期和时间标记INSERT或UPDATE的操作。`
---与java数据类型对应关系

---[参考资料](http://blog.csdn.net/seedshome/article/details/52316913)

---Date – java.sql.Date

---Time – java.sql.Time

---DateTime – java.sql.Timestamp


