# 写出易调试的 SQL - 文章 - 伯乐在线
原文出处： [蒋奎](http://www.cnblogs.com/anuo/p/6241557.html)
#### 1.前言
相比高级语言的调试如C# , 调试SQL是件痛苦的事 . 特别是那些上千行的存储过程, 更是我等码农的噩梦.
在将上千行存储过程的SQL 分解到 C# 管理后, 也存在调试的不通畅, 如何让调试流畅些呢, 请看后续
#### 2.常见调试
2.1 通常在Dapper 里面一个断点下去, 抓到类似如下SQL:

```
SELECT 
	a.* 
FROM  dbo.ptype a
INNER JOIN dbo.PType_Price b ON a.typeId=b.PTypeID
LEFT JOIN dbo.PType_Units c ON a.typeId=c.UnitsId
WHERE a.typeId=@typeid AND a.CreateDate=@Area 
	AND preprice1=@preprice1 AND deleted=@deleted
```
各种@符号, 需要手工替换后才能调试(麻烦), 要是能抓到最终SQL就好了
2.2 庆幸的是可以通过SQLServer Profiler 来抓到最终SQL
![](http://jbcdn2.b0.upaiyun.com/2017/01/d39ff7d238a717d061e131847fe99214.png)
但是生产环境中的SQLServer,  并发执行的SQL 非常多, 如上图所见, 在一大堆SQL 里面找到你刚才执行的SQL也比较麻烦, 即使可以Ctrl + F 调出搜索框来搜索, 也要想个好的关键字来搜索 , 麻烦.
#### 3.解决方案
既然我们想要最终的SQL , 为毛不在丢给Dapper 执行前, 就已经是最终SQL了呢, 上工具代码:

```
public class SqlHelper
    {
        public Dictionary<string, object> Param = new Dictionary<string, object>();
        public string ReplaceParam(ref string sql)
        {
            if (Param.Count == 0)
            {
                return sql;
            }
            StringBuilder sb = new StringBuilder();
            sb.Append(sql);
            foreach (var item in Param)
            {
                var paramName = item.Key;
                var paramValue = item.Value;
                var type = paramValue.GetType();
                if (type == typeof(string) || type == typeof(DateTime))
                {
                    //字符串
                    sb.Replace($"@{paramName}", $"'{paramValue}'");
                }
                else if (type == typeof(bool))
                {
                    //bool 类型
                    if (paramValue.ToString() == "True")
                    {
                        sb.Replace($"@{paramName}", "1");
                    }
                    else
                    {
                        sb.Replace($"@{paramName}", "0");
                    }
                }
                else
                {
                    //数值
                    sb.Replace($"@{paramName}", paramValue.ToString());
                }
            }
            sql = sb.ToString();
            return sql;
        }
    }
```
调用示例:

```
public IEnumerable<Ptype> GetPtypeDetail()
        {
            var sql = @"
SELECT a.* 
FROM  dbo.ptype a
INNER JOIN dbo.PType_Price b ON a.typeId=b.PTypeID
LEFT JOIN dbo.PType_Units c ON a.typeId=c.UnitsId
WHERE a.typeId=@Typeid AND a.CreateDate=@CreateDate 
AND preprice1=@preprice1 AND deleted=@deleted
";
            var sqlHelper = new SqlHelper();
            sqlHelper.Param.Add("Typeid", "001");
            sqlHelper.Param.Add("CreateDate", DateTime.Now);
            sqlHelper.Param.Add("preprice1", 3.62M);
            sqlHelper.Param.Add("deleted", true);
            sqlHelper.ReplaceParam(ref sql);
            IEnumerable<Ptype> plist = new List<Ptype>();
            using (var con = SQLServerHelper.GetConnection())
            {
                plist = con.Query<Ptype>(sql);
            }
            return plist;
        }
```
这样丢给Dapper 执行的SQL 始终是最终SQL, 就不用煞费苦心去抓了.
PS: 有人可能会质疑这样替换的效率,不用担心已测试 , C#的字符串替换是非常快的, 上面的调用实例, 当时的测试结果是 微妙和纳秒级别, 有兴趣的看管可以再测试.
#### 4. 最后
现在丢给Dapper执行的不再是 充满@参数的SQL ,  而是一个替换好的最终SQL .
这样当老板隔老远吼道你说: 小蒋, 你tm 有个XX bug ,赶紧看看 .
你可以不慌不忙的在 Dapper Query处打个断点
![](http://jbcdn2.b0.upaiyun.com/2017/01/e5da262369787ac490db41b2342af7d7.png)
鼠标放在SQL变量上, 轻松的拿到最终SQL进行调试, 而不是, 手动去替换@参数, 又或则在SQLServer Profiler 里面大海捞针了!!!
