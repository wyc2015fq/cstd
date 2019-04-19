# Django1.8文档阅读手记 - =朝晖= - 博客园
# [Django1.8文档阅读手记](https://www.cnblogs.com/dhcn/p/7116232.html)
     主要集中在新特性上。
      Django1.8好像开始推荐使用python3
      Django的QuerySet是延迟加载的，这个文档里面一般会明言，通过Connection SQL显示[测试](http://lib.csdn.net/base/softwaretest)，外键对象也是延迟加载的。
      Django1.8的get filter等方法继续在aptana pydev下error：undefined variables from import，不过收费的pycharm似乎解决自django1.7以后出现的这个问题。
  Web development is often broad, not deep – problems span many domains. 
**One-to-one relationships**:This is most useful on the primary key of an object when that object “extends” another object in some way.It is Model inheritance too.
 A field name cannot contain more than one underscore in a row, due to the way Django’s query lookup syntax works
[limiting the queryset](https://docs.djangoproject.com/en/1.8/topics/db/queries/#limiting-querysets) using an array slice or an index will not populate the cache.
    If you need to execute more complex queries (for example, queries with OR statements), you can use [Q objects](https://docs.djangoproject.com/en/1.8/ref/models/querysets/#django.db.models.Q).
    Note that [delete()](https://docs.djangoproject.com/en/1.8/ref/models/querysets/#django.db.models.query.QuerySet.delete) is the only [QuerySet](https://docs.djangoproject.com/en/1.8/ref/models/querysets/#django.db.models.query.QuerySet) method that is not exposed on a [Manager](https://docs.djangoproject.com/en/1.8/topics/db/managers/#django.db.models.Manager) itself. This is a safety mechanism to prevent you from accidentally requesting Entry.objects.delete(), and deleting all the entries. 
    Forward access to one-to-many relationships is cached the first time the related object is accessed.
    对Model层的重新认识，以前一直认为Django的Model层不过是一个Active Record模式的Data Access Layer层，最近重新通篇阅读Guide文档，发现认识有所不足：它不仅是一个自下而上设计实现的数据访问设施，而且是一个面向领域业务模型自上而下设计的模型实现层.
     However, when a values() clause is used to constrain the columns that are returned in the result set, the method for evaluating annotations is slightly different. Instead of returning an annotated result for each result in the original QuerySet, the original results are grouped according to the unique combinations of the fields specified in the values() clause.
    Class inheritance and model managers aren’t quite a perfect match for each other.
**Avoid catching exceptions inside **atomic**!**
    As well as caching of the whole QuerySet, there is caching of the result of attributes on ORM objects. In general, attributes that are not callable will be cached,But in general, callable attributes cause DB lookups every time.

