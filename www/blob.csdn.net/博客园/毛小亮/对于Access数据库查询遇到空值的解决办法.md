# 对于Access数据库查询遇到空值的解决办法 - 毛小亮 - 博客园
# [对于Access数据库查询遇到空值的解决办法](https://www.cnblogs.com/xianerwonder/p/4440817.html)
1、Access数据库在office环境下对于null是识别的，但是，在开发环境下，Access数据库对于where xxx is null是不识别的。
2、查询空值解决办法:select * from table where len(field) < 1

