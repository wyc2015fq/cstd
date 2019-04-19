# Fluently NHibernate映射多个实体程序集 - 左直拳的马桶_日用桶 - CSDN博客
2016年02月28日 16:02:54[左直拳](https://me.csdn.net/leftfist)阅读数：2022
Fluently NHibernate有个好处就是可以在代码里定义实体类，而不必写冗长的XML。
但通常，一个项目对应的所有的实体类，都编译成一个DLL。如果有个项目，是在某个父项目的基础上再扩展，那么这个子项目，一方面有自己的实体类DLL，还有父项目的DLL，要如何映射？
网上寻寻觅觅，找不到啥资料，只有一个老外提到了类似的问题。也没其他人回答，他自己给出了答案。当然他那个可能是JAVA的，我是.NET派，但还是感谢得到了他的启发。英语文章，猛一看让人顿生怯意，但硬着头皮看下去，还是能看懂一些。要习惯看英文了，无论是装逼还是实际研究，都需要。
我是酱紫写的： 
![这里写图片描述](https://img-blog.csdn.net/20160228155916959)
重复用了2个 .AddFromAssembly
```
public static FluentConfiguration GetConfiguration(string connString)
        {
            return Fluently
                .Configure()
                .Database(FluentNHibernate.Cfg.Db.OracleClientConfiguration.Oracle10
                    .ConnectionString(connString)
                    .ShowSql())
                .ExposeConfiguration(f => f.SetInterceptor(new 叉叉叉.Data.Infrastructure.ConnectionHelper.SqlStatementInterceptor()))
                .Mappings(m => m.FluentMappings
                    .AddFromAssembly(Assembly.Load("子项目.Data"))
                    .AddFromAssembly(Assembly.Load("父项目.Data"))
                    .Conventions.AddFromAssemblyOf<叉叉叉.Data.Infrastructure.ConnectionHelper.EnumConvention>())
                .Cache(c => c.UseQueryCache().UseSecondLevelCache().ProviderClass<RedisCacheProvider>());
        }
```
