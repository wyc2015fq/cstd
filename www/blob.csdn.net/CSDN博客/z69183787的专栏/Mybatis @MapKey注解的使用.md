# Mybatis @MapKey注解的使用 - z69183787的专栏 - CSDN博客
2018年08月27日 21:45:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：628
[https://blog.csdn.net/weixin_41023230/article/details/80103990](https://blog.csdn.net/weixin_41023230/article/details/80103990)
[https://blog.csdn.net/ClementAD/article/details/50589459](https://blog.csdn.net/ClementAD/article/details/50589459)
### 业务场景
从数据库查出多条数据，包括id和name两个字段。希望可以把结果直接用Map接收，然后通过map.get(id)方便地获取name的值。
- 如果使用下面代码，那么只能返回一条数据,如果返回多条数据mybatis回报错*selectOne but found …*。
`Map<String, Object> m = abcDao.getNamesByIds(idList);`- 1
解决
**方式1**
将返回值类型改为`List<Map<String String>>`
**方式2**
使用`@MapKey`注解，上代码 
```
@MapKey("id") 
Map<String, Map<String, String>> ywtest(String id);
```
id就是返回map的key ,里面的map就是这种格式`（"id":123）`,通过key就可以获取到对象。**注意！id不能重复，重复了就覆盖了**
