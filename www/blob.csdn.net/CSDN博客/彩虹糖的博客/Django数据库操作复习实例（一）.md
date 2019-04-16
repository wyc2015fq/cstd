# Django数据库操作复习实例（一） - 彩虹糖的博客 - CSDN博客





2017年11月10日 08:28:13[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：217








       最近想复习一下与Django数据库有关的操作方法，于是便重新搭建了一个以博客为目的的Django服务，只是一个Demo演示，没有前端，数据通过Postman请求，以Json的形式返回，也没有考虑数据不存在等异常情况。

       首先，Django的关联字段有OneToOneField,ForeignKey和ManyToManyField，分别对应一对一关系，多对一关系，和多对多关系，目前我们用到的是ForeignKey和ManyToManyField，这两个字段都是可以与自身做多对一，多对多关系的。在博客系统中，“好友”这个属性显然是一个对自身的多对多属性，其他的博客作者，评论对象，点赞对象都是明显的多对一属性。

       在ForeignKey中有一个属性是related_name，通过这个属性，与它相关联的字段可以通过这个属性值找到这个字段。



```python
#blog.models
#encoding:utf-8
from __future__ import unicode_literals

from django.db import models


class Author(models.Model):
    author_id = models.IntegerField(null=True)
    name = models.CharField(max_length=100)
    age = models.IntegerField()
    friends = models.ManyToManyField("self")            #关联自身的多对多属相，朋友

    def __str__(self):
        return self.name

    #获取作者详细信息
    def getAuthorDetail(self):
        data = {
            'author_id': self.author_id,
            'name': self.name,
            'age': self.age,
        }
        return data


class Blog(models.Model):
    author = models.ForeignKey(Author,null=True,related_name="author")    #将博文与作者关联起来
    title = models.CharField(max_length=100)
    abstract = models.TextField()

    def __str__(self):
        return self.title

    #获取博文的信息
    def getBlogDetail(self):
        data = {
            'author': self.author.name,
            'title': self.title,
            'abstract': self.abstract,
        }
        return data


class Comment(models.Model):
    author = models.ForeignKey(Author,null=True,related_name="comment_author")  #评论关联作者
    article = models.ForeignKey(Blog,null=True,related_name="article")           #评论关联博文
    content = models.TextField()

    def __str__(self):
        return self.content

    #获取评论详情
    def get_comment_detail(self):
        data = {
            'author': self.author.name,
            'content':self.content,
        }
        return data


class Likes(models.Model):
    blog = models.ForeignKey(Blog,null=True,related_name="blog")              #点赞关联文章
    author = models.ForeignKey(Author,null=True,related_name="likes_author") #点赞关联作者
    amount = models.IntegerField(default=0)

    def __str__(self):
        return self.author

    #获取点赞详情
    def get_likes_detail(self):
        data = {
            'author': self.author.name,
            'blog':self.blog.title,
            'amount':self.amount,
        }
        return data
```
    在admin中加入相关字段，便于后台管理，本节直接在后台添加数据，暂不使用Post方法来添加数据。



遇到的一个小坑是在添加数据时后台管理对中文添加的不支持，加入一下三行就好了



```python
import sys  
reload(sys)  
sys.setdefaultencoding('utf8')
```

```python
#blog.admin  
from django.contrib import admin  
  
from django.contrib import admin  
from .models import Author, Blog, Comment, Likes  
import sys  
reload(sys)  
sys.setdefaultencoding('utf8')  
  
admin.site.register(Author)  
  
admin.site.register(Blog)  
  
admin.site.register(Comment)
```
![](https://img-blog.csdn.net/20171111092205012?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2Fveml4dWFuOTg3MjQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


以类似的方法添加blog,comment等。



```python
#blog.views 
#encoding:utf-8  
   
from django.http import JsonResponse  
 
from blog.models import *  
  
  
def get_friends(request):  
    result = {'status': 200}  
    friends_list = []  
    author_id = request.POST['id']  
    author = Author.objects.get(author_id=author_id )  
    friends = author.friends.all()  
    for friend in friends:  
        friends_list.append(friend.getAuthorDetail())  
    result['friends'] = friends_list  
    return JsonResponse(result, status=200)  
  
  
def get_article(request):  
    result = {'status': 200}  
    articles_list = []  
    author_id = request.POST['id']  
    author = Author.objects.get(author_id=author_id)  
    articles = author.author.all()                   #通过article的关联字段author找到author对应的文章  
    for article in articles:  
        articles_list.append(article.getBlogDetail())  
    result['articles'] = articles_list  
    return JsonResponse(result, status=200)  
  
  
def get_all_comment_of_author(request):  
    result = {'status': 200}  
    comment_list = []  
    author_id = request.POST['id']  
    author = Author.objects.get(author_id=author_id)  
    articles = author.author.all()  
    for article in articles:  
        comments = article.article.all()           #通过comment的关联字段article让article找到对应的评论  
        for comment in comments:  
            comment_list.append(comment.get_comment_detail())  
    result["comments"] = comment_list  
    return JsonResponse(result,status=200)
```


注意其中对ForeignKey和MantToManyKey的一些用法。  

这样，开启Django服务，我们就能看到想要的结果： 

![](https://img-blog.csdn.net/20171110090522318?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2Fveml4dWFuOTg3MjQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171110090631432?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2Fveml4dWFuOTg3MjQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



接下来会考虑介绍详细的有关查询的操作和考虑博客访问权限的问题。









