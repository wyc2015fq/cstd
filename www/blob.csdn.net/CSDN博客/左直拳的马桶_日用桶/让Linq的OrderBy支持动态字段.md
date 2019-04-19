# 让Linq的OrderBy支持动态字段 - 左直拳的马桶_日用桶 - CSDN博客
2015年09月14日 19:48:35[左直拳](https://me.csdn.net/leftfist)阅读数：6554
使用linq的OrderBy，如果明确知道是哪个字段，当然很容易：
```
IQueryable<User> userQuery = ...;
 userQuery.OrderBy(u => u.Code)
```
但假如我们想写一个通用方法，预先并不知道要用哪个字段排序呢？
在网上寻寻觅觅，有许多国内的博客互相抄袭，信誓旦旦，但其实那些代码都运行不了。
还是老外的好使： 
[http://www.4byte.cn/question/33782/dynamic-orderby-using-linq-dynamic.html](http://www.4byte.cn/question/33782/dynamic-orderby-using-linq-dynamic.html)
我拿来修改了一下，可以用。主要思想是扩展Queryable。但里面的东西有许多我都看不懂。
```
public static class QueryableExtension
    {
        public static IOrderedQueryable<T> OrderBy<T>(this IQueryable<T> query, string propertyName)
        {
            return _OrderBy<T>(query, propertyName, false);
        }
        public static IOrderedQueryable<T> OrderByDescending<T>(this IQueryable<T> query, string propertyName)
        {
            return _OrderBy<T>(query, propertyName, true);
        }
        static IOrderedQueryable<T> _OrderBy<T>(IQueryable<T> query, string propertyName,bool isDesc)
        {
            string methodname = (isDesc) ? "OrderByDescendingInternal" : "OrderByInternal";
            var memberProp = typeof(T).GetProperty(propertyName);
            var method = typeof(QueryableExtension).GetMethod(methodname)
                                       .MakeGenericMethod(typeof(T), memberProp.PropertyType);
            return (IOrderedQueryable<T>)method.Invoke(null, new object[] { query, memberProp });
        }
        public static IOrderedQueryable<T> OrderByInternal<T, TProp>(IQueryable<T> query, PropertyInfo memberProperty)
        {//public
            return query.OrderBy(_GetLamba<T, TProp>(memberProperty));
        }
        public static IOrderedQueryable<T> OrderByDescendingInternal<T, TProp>(IQueryable<T> query, PropertyInfo memberProperty)
        {//public
            return query.OrderByDescending(_GetLamba<T, TProp>(memberProperty));
        }
        static Expression<Func<T, TProp>> _GetLamba<T, TProp>(PropertyInfo memberProperty)
        {
            if (memberProperty.PropertyType != typeof(TProp)) throw new Exception();
            var thisArg = Expression.Parameter(typeof(T));
            var lamba = Expression.Lambda<Func<T, TProp>>(Expression.Property(thisArg, memberProperty), thisArg);
            return lamba;
        }
    }
```
调用：
```
IQueryable<User> userQuery = ...;
//正序
userQuery = userQuery.OrderBy("Code");
//降序
userQuery = userQuery.OrderByDescending("Code");
```
