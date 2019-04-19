# 自定django登录跳转限制 - _天枢 - 博客园
## [自定django登录跳转限制](https://www.cnblogs.com/yhleng/p/8572284.html)
2018-03-15 10:50 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8572284)
**django自定义一个登录跳转限制：**
**解释：**
**　　不登录不让跳转到其它页面。**
**原理：**
**　　做一个装饰器，在访问路由时，先进行判断，登录session是否存在，如果存在则跳转，不存在则返回到登录页面。或者你可以不登录返回404错误页面。**
```
#验证登录跳转页面
def login_limit(func):
    def war(request):
        try:
           request.session['username'] #session不存在会报错
           ret = func(request)
           return ret
        except:
            return HttpResponseRedirect('/')
    return war
```
**注意事项：**
**　　如果，前端，发起请求是ajax发起，后台函数要进行装饰@csrf_exempt  ，这是跨站请求伪造。不加这个会报错。**
**       那么问题就来了。加了这个之后，如果在加上我们自己写的，防跳转装饰器。如果加在@csrf_exempt之前，如下这样。**
**　　那么恭喜你，会报个302之类的错误。那问题怎么解决：**
```
@login_limit
@csrf_exempt
```
**解决办法：将我们防跳转装饰器，放在跨站请求伪造装饰器之后。**
```
#删除测试用例
@csrf_exempt
@login_limit
def case_delete_data(request):
    case_id = int(request.POST.get('caseid'))
    model_class = ModelClass(case_interface_table)
    model_class.delete_data(case_id)
    '''
    #分页代码
    '''
    curPage = int(request.GET.get('page','1'))
    allCount = case_interface_table.objects.all().count()
    fpage = pageInfo(curPage,allCount,5)
    return HttpResponse(request,"OK")
```
