# EF TMD - 左直拳的马桶_日用桶 - CSDN博客
2014年09月11日 11:38:11[左直拳](https://me.csdn.net/leftfist)阅读数：3843
TMD
几个月前，本着学习的心态，首次在项目中应用EF。因为这里老是赶、赶、赶，当时只是匆匆而就，浅尝辄止，搞到现在对EF一知半解，每次在新项目使用，都担惊受怕，大费周折，不知道什么时候，在什么地方就会冒出一个错误来。
TMD
这两天又在搞，果然又遇到了一些问题，着实费了不少时间。记录心得如下：
1、生成ADO.NET实体对象的时候，系统会自动在app.config里添加连接字符串，形如：
`<add name="testEntities" connectionString="metadata=res://*/testEntities.csdl|res://*/testEntities.ssdl|res://*/testEntities.msl;provider=System.Data.SqlClient;provider connection string="data source=(local)\sql2012;initial catalog=test;integrated security=True;MultipleActiveResultSets=True;App=EntityFramework"" providerName="System.Data.EntityClient" />`
如果我们是独立一个项目来存放这个实体对象，由其他项目来调用这里的EF对象，那么要将这个连接串拷贝到其他项目的配置文件中。
2、存放EF对象的项目，固然要应用Entity Framework，而实际调用项目，也要安装才行。否则会报
"
# [No Entity Framework provider found for the ADO.NET provider with invariant name 'System.Data.SqlClient'](http://stackoverflow.com/questions/18455747/no-entity-framework-provider-found-for-the-ado-net-provider-with-invariant-name)"
之类的错
引用Entity Framework，可以使用nuget控制台
PM> Install-Package EntityFramework  解决方案中的项目名称
