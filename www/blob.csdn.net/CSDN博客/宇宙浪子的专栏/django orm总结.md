# django orm总结 - 宇宙浪子的专栏 - CSDN博客
2016年03月28日 16:58:39[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：379
原文：[django orm总结](http://www.cnblogs.com/linjiqin/p/3817954.html)
目录
1.1.1 生成查询
1.1.2 创建对象
1.1.3 保存修改的对象
1.1.4 保存 ForeignKey 和 ManyToManyField 字段
1.1.5 检索对象
1.1.6 检索所有的对象
1.1.7 过滤检索特定对象
1.1.8 链接过滤
1.1.9 过滤结果集是唯一 
1.2.1 结果集是延迟的 
1.2.2 其他的QuerySet方法
1.2.3 限制 QuerySets
1.2.4 字段查找
1.2.5 跨关系查询
1.2.6 过滤器可参考模型字段
1.2.7 缓存查询集
1.2.8 比较对象
1.2.9 删除对象
1.3.1 一次修改多个对象
1.3.2 关系对象
1.3.3 One-to-many关系
1.3.4 Many-to-many关系
1.3.5 One-to-one关系
**1.1.1 生成查询**
你创建完数据模型，django会自动提供给你数据库抽象的API，可以创建、获取、修改、删除对象，本篇文档讲解如何使用API。
我们参考下面模型，一个weblog：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
#博客
class Blog(models.Model):
    name = models.CharField(max_length=100)
    tagline = models.TextField()
    def __unicode__(self):
        return self.name
#作者
class Author(models.Model):
    name = models.CharField(max_length=50)
    email = models.EmailField()
    def __unicode__(self):
        return self.name
#目录
class Entry(models.Model):
    blog = models.ForeignKey(Blog)
    headline = models.CharField(max_length=255)
    body_text = models.TextField()
    pub_date = models.DateTimeField()
    authors = models.ManyToManyField(Author)
    n_comments = models.IntegerField()
    n_pingbacks = models.IntegerField()
    rating = models.IntegerField()
    def __unicode__(self):
        return self.headline
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**1.1.2 创建对象**
用python对象描述数据库表的数据,django使用一个直观的系统，一个模型类描述一个数据表，一个类的实例描述表的一条详细记录。使用模型的save()方法将对象创建到数据库。
```
from mysite.blog.models import Blog
b = Blog(name='Beatles Blog', tagline='All the latest Beatles news.')
b.save()
```
只有执行save方法时，django才会执行sql把对象写入数据库。
**1.1.3 保存修改的对象**
保存修改仍然使用save()方法
```
b5.name = 'New name'
b5.save()
```
**1.1.4 保存 ForeignKey 和 ManyToManyField 字段**
```
cheese_blog = Blog.objects.get(name="Cheddar Talk")
entry.blog = cheese_blog #为 ManyToManyField 增加记录
entry.save()
joe = Author.objects.create(name="Joe")
entry.authors.add(joe) #为 ForeignKey 增加记录
```
**1.1.5 检索对象**
从数据库里检索对象，可以通过模型的Manage来建立QuerySet,一个QuerySet表现为一个数据库中对象的结合，他可以有0个一个或多个过滤条件，在SQL里QuerySet相当于select语句用where或limit过滤。你通过模型的Manage来获取QuerySet，每个模型至少有一个Manage
**1.1.6 检索所有的对象**
检索表中所有数据，最简单的方式是用all().
```
all_entries = Entry.objects.all()
```
**1.1.7 过滤检索特定对象**
检索过滤特定查询结果，有两个方法。
filter(**kwargs) 返回一个新的匹配查询参数后的QuerySet 
exclude(**kwargs) 返回一个新的不匹配查询参数后的QuerySet 
```
Entry.objects.filter(pub_date__year=2006)
```
**1.1.8 链接过滤**
```
Entry.objects.filter(headline__startswith='What')
         .exclude(pub_date__gte=datetime.now())
         .filter(pub_date__gte=datetime(2005, 1, 1))
```
**1.1.9 过滤结果集是唯一**
```
每次你完成一个QuerySet，你获得一个全新的结果集，不包括前面的。每次完成的结果集是可以贮存，使用或复用
q1 = Entry.objects.filter(headline__startswith="What")
q2 = q1.exclude(pub_date__gte=datetime.now())
q3 = q1.filter(pub_date__gte=datetime.now())
```
三个QuerySets是分开的，第一个是headline以"What"单词开头的结果集，第二个是第一个的子集，即pub_date不大于现在的，第三个是第一个的子集 ，pub_date大于现在的
**1.2.1 结果集是延迟的**
QuerySets是延迟的，创建QuerySets不会触及到数据库操作，你可以多个过滤合并到一起，直到求值的时候django才会开始查询。如：
```
q = Entry.objects.filter(headline__startswith="What")
q = q.filter(pub_date__lte=datetime.now())
q = q.exclude(body_text__icontains="food")
print q
```
虽然看起来执行了三个过滤条件，实际上最后执行print q的时候，django才开始查询执行SQL到数据库。
**1.2.2 其他的QuerySet方法**
大多数情况你使用all()、filter()和exclude()
**1.2.3 限制 QuerySets**
使用python的数组限制语法限定QuerySet，如：
取前5个
```
Entry.objects.all()[:5]
```
取第五个到第十个
```
Entry.objects.all()[5:10]
```
一般的，限制QuerySet返回新的QuerySet，不会立即求值查询，除非你使用了"step"参数
```
Entry.objects.all()[:10:2]
Entry.objects.order_by('headline')[0]
Entry.objects.order_by('headline')[0:1].get()
```
**1.2.4 字段查找**
字段查找是指定SQL语句的WHERE条件从句，通过QuerySet的方法filter(), exclude()和get()指定查询关键字。
基本查询field__lookuptype=value
例如：
```
Entry.objects.filter(pub_date__lte='2006-01-01')
```
转换为SQL：
```
SELECT * FROM blog_entry WHERE pub_date <= '2006-01-01';
```
如果你传了无效的参数会抛异常
数据库API 支持一些查询类型，下面体验一下：
a、exact
```
Entry.objects.get(headline__exact="Man bites dog")
```
等价于
```
SELECT ... WHERE headline = 'Man bites dog';
```
如果查询没有提供双下划线，那么会默认 __exact=
```
Blog.objects.get(id__exact=14) # Explicit form
Blog.objects.get(id=14) # __exact is implied
```
b、iexact——忽略大小写
```
Blog.objects.get(name__iexact="beatles blog")
```
blog title会匹配 "Beatles Blog", "beatles blog", 甚至 "BeAtlES blOG".
c、contains——包含查询，区分大小写
```
Entry.objects.get(headline__contains='Lennon')
```
转化为SQL
```
SELECT ... WHERE headline LIKE '%Lennon%';
```
icontains 不区分大小写
startswith，endswith，istartswith，iendswith
前模糊匹配，后模糊匹配
**1.2.5 跨关系查询**
```
Entry.objects.filter(blog__name__exact='Beatles Blog')
```
这个可以跨越你想要的深度。
反向跨关系查询
```
Blog.objects.filter(entry__headline__contains='Lennon')
```
如果跨越多层关系查询，中间模型没有值，django会作为空对待不会发生异常。
```
Blog.objects.filter(entry__author__name='Lennon');
Blog.objects.filter(entry__author__name__isnull=True);
Blog.objects.filter(
entry__author__isnull=False,
entry__author__name__isnull=True);
```
**1.2.6 过滤器可参考模型字段**
目前给的例子里，我们建立了过滤，比照模型字段值和一个固定的值，但是如果我们想比较同一个模型里的一个指端和另一个字段的值，django提供F()——专门取对象中某列值的操作
```
from django.db.models import F
Entry.objects.filter(n_pingbacks__lt=F('n_comments'))
```
注：n_pingbacks、n_comments为模型Entry属性
django支持加减乘除和模计算
```
Entry.objects.filter(n_pingbacks__lt=F('n_comments') * 2) 
Entry.objects.filter(rating__lt=F('n_comments') + F('n_pingbacks')) 
Entry.objects.filter(author__name=F('blog__name'))
```
主键查询捷径
```
Blog.objects.get(id__exact=14) # Explicit form
Blog.objects.get(id=14) # __exact is implied
Blog.objects.get(pk=14) # pk implies id__exact
```
不仅限于__exact 查询
```
# Get blogs entries with id 1, 4 and 7
Blog.objects.filter(pk__in=[1,4,7])
# Get all blog entries with id > 14
Blog.objects.filter(pk__gt=14)
```
跨越查询
```
Entry.objects.filter(blog__id__exact=3) # Explicit form
Entry.objects.filter(blog__id=3) # __exact is implied
Entry.objects.filter(blog__pk=3) # __pk implies __id__exact
```
like语句转义百分号
```
Entry.objects.filter(headline__contains='%')
```
转义为
```
SELECT ... WHERE headline LIKE '%\%%';
```
**1.2.7 缓存查询集**
每个QuerySet都包含一个缓存，以尽量减少对数据库的访问。理解他的工作原理很重要，可以写出最高效的代码。
在最新创建的QuerySet里，缓存是空的。在第一次QuerySet被取值，因此数据库查询发生，django把查询结果放入缓存，并返回给请求，随后的查询取值会复用缓存中的结果。
保持缓存的思想习惯，因为如果你不正确使用查询缓存会有麻烦。例如下面例子会创建两个QuerySet
```
print [e.headline for e in Entry.objects.all()]
print [e.pub_date for e in Entry.objects.all()]
```
这样意味着数据库查询会执行两次，实际两次数据库加载
为了避免这个问题，简单保存QuerySet复用
```
queryset = Poll.objects.all()
print [p.headline for p in queryset] # Evaluate the query set.
print [p.pub_date for p in queryset] # Re-use the cache from the evaluation.
```
**1.2.8 比较对象**
比较两个模型实例，使用python标准的运算符，两个等号==
```
some_entry == other_entry
some_entry.id == other_entry.id
some_obj == other_obj
some_obj.name == other_obj.name
```
**1.2.9 删除对象**
删除方法是很方便的，方法名为delete()，这个方法直接删除对象没有返回值
```
e.delete()
```
你也可以批量删除对象，每个QuerySet有一个delete()方法，能删除 QuerySet里所有对象
**1.3.1 一次修改多个对象**
有时候你想给QuerySet里所有对象的一个字段赋予特定值，你可以使用 update()方法
例如:
```
# Update all the headlines with pub_date in 2007.
Entry.objects.filter(pub_date__year=2007).update(headline='Everything is the same')
```
**这个方法只能用于无关联字段和外键**
```
b = Blog.objects.get(pk=1)
# Change every Entry so that it belongs to this Blog.
Entry.objects.all().update(blog=b)
```
update()方法不返回任何值，QuerySet不支持save方法，如果要执行save，可以如下：
```
for item in my_queryset:
item.save()
```
update也可以使用F()
```
# THIS WILL RAISE A FieldError
Entry.objects.update(headline=F('blog__name'))
```
**1.3.2 关系对象**
当你在model里定义一个关系时，模型实例会有一个方便的API来访问关系对象。用本页上面的模型举个例子，一个Entry
对象可以得到blog对象，访问blog属性e.blog。
django也创建API来访问关系对象的另一边，一个blog对象访问Entry列表b.entry_set.all().
**1.3.3 One-to-many关系**
如果一个对象有ForeignKey，这个模型实例访问关系对象通过简单的属性
```
e = Entry.objects.get(id=2)
e.blog # Returns the related Blog object.
```
你可以凭借外键属性获取和赋值，修改外键值知道执行save()方法才会保存到数据库
```
e = Entry.objects.get(id=2)
e.blog = some_blog
e.save()
```
如果ForeignKey 设置了null=True 你可以赋值为None
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
e = Entry.objects.get(id=2)
print e.blog # Hits the database to retrieve the associated Blog.
print e.blog # 不会在向数据库取; 使用缓存中的值.
e = Entry.objects.select_related().get(id=2)
print e.blog # 不会在向数据库取; 使用缓存中的值.
print e.blog # 不会在向数据库取; 使用缓存中的值.
b = Blog.objects.get(id=1)
b.entry_set.all() # 返回所有blog的关联对象.
# b.entry_set is a Manager that returns QuerySets.
b.entry_set.filter(headline__contains='Lennon')
b.entry_set.count()
b = Blog.objects.get(id=1)
b.entries.all() # 返回所有blog的关联对象
# b.entries is a Manager that returns QuerySets.
b.entries.filter(headline__contains='Lennon')
b.entries.count()
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
add(obj1, obj2, ...) 增加多个关系对象
create(**kwargs) 建立新对象
remove(obj1, obj2, ...) 去除多个关系对象
clear() 清理所有关系对象
```
b = Blog.objects.get(id=1)
b.entry_set = [e1, e2]
```
**1.3.4 Many-to-many关系**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
e = Entry.objects.get(id=3)
e.authors.all() # 返回Entry所有authors .
e.authors.count()
e.authors.filter(name__contains='John')
a = Author.objects.get(id=5)
a.entry_set.all() # 返回Author所有entry .
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**1.3.5 One-to-one关系**
```
class EntryDetail(models.Model):
    entry = models.OneToOneField(Entry)
    details = models.TextField()
ed = EntryDetail.objects.get(id=2)
ed.entry # 返回 Entry 对象.
```
