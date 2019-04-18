# mybatis中模糊查询的使用以及一些细节问题的注意事项 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年07月07日 16:11:38[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：127


页面有个功能 为 根据 品牌名进行 关键字查询，对应到数据库的是brand表的name字段的模糊查询

如果用的是SSM框架，在mybatis中我们需要自己写sql语句，涉及到like的模糊查询，mybatis中我们通常会使用#{}或${}来获取pojo对象的变量值。

这两个区别为   #{} 会在 变量外侧 加上 单引号  如   select * from brand where name='牌1'

                       ${} 并不会 加单引号   如 select * from brand where name=牌1  显然这样会有问题。所有通常我们用${}需自己加上单引号 如 '${name}' 。 

但我们应尽量避免使用 ${} ，因为这个最终会将参数拼接到 sql语句上，存在sql注入的问题。但#{}不会，#{}类似于 hibernate中的    select * from brand where name=:name     然后再通过 query.setParameter('参数名',参数值)这种方式。

但是在 模糊查询中 会出现一个问题， 模糊查询通常都是 与 % 配合使用，  所以我们最终想要的结果是如下这种

select * from brand where name like '%牌%' ， 也就是说 我们需要将 两个百分号 与 ‘牌’ 这个字进行字符串拼接。

有人会 想到 用   '%${name}%'  用这种方式，但是这种方式并不好，上面说了，有sql注入的风险。

我们可以采用 and name like concat('%',#{name},'%') 这种方式来进行字符串拼接。

还有一个要说的是，我们在 mybatis 维护sql的 配置文件中。 肯定会判断 <if test="name!=null">   即 name这个变量是否为空。 但是如果这个对象 为 空字符串 就会大大影响我们查询的效率。

如下所示

![](https://img-blog.csdn.net/20180707155841898?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可以看到，我们依旧可以将数据查询出来，但是加了一个模糊查询的条件，这样就降低了查询的效率。

这种情况往往出现在 用户 没有输入 关键字就点击了搜索按钮，这样传到后端的就是一个 空字符串，这里需要注意，当第一次进入这个页面的时候 传到后端的 参数为null。如下图所示

![](https://img-blog.csdn.net/20180707160648356?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

所以为了不影响  数据库的查询效率，我们需要 在 controller层 对 空字符串做一下处理。

如果 为空字符串 我们将其设置 为null 

![](https://img-blog.csdn.net/20180707160838416?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

StringUtils 是一个 工具包，如果你也想使用 可以在maven中 添加 如下jar包

```java
<dependency>
		    <groupId>org.apache.commons</groupId>
		    <artifactId>commons-lang3</artifactId>
		    <version>3.4</version>
		</dependency>
```

