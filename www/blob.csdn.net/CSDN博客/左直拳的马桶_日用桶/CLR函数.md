# CLR函数 - 左直拳的马桶_日用桶 - CSDN博客
2011年05月23日 11:52:00[左直拳](https://me.csdn.net/leftfist)阅读数：1514
1、clr函数里面，如果要
想访问存储过程，要加上声明：
SystemDataAccess = SystemDataAccessKind.Read
想访问数据表，加上声明：
DataAccess = DataAccessKind.Read
2、clr函数里，不能INSERT/UPDATE/DELETE；里面如果有访问存储过程，存储过程包含有此类语句也不行。
总而言之，函数，不管CLR函数也好，还是在SQL里直接写的自定义函数也好，只能读取、返回数据。休想在里面修改数据。
3、如果填充的方法FillRowMethod参数类型、数量有所变化，那么SQL里面对应的调用函数也应该相应修改，否则无法更新程序集。
4、
