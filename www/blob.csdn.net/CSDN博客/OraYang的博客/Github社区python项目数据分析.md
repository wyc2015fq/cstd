
# Github社区python项目数据分析 - OraYang的博客 - CSDN博客

2018年01月18日 12:38:25[OraYang](https://me.csdn.net/u010665216)阅读数：4248所属专栏：[python科学计算、游戏开发、后台开发](https://blog.csdn.net/column/details/18109.html)



# 引言
最近看到CSDN上各种做数据分析的，相关博文层出不穷：什么分析CSDN小姐姐一年发的文章，什么分析猎场的火热程度，比比皆是。于是乎，我们也来凑个热闹，分析下github上火热的python项目。
# 使用API调用数据
首先我们打开自己的浏览器，在地址栏中输入下面的命令：
> [https://api.github.com/search/repositories?q=language:python&sort=stars](https://api.github.com/search/repositories?q=language:python&sort=stars)
然后我们会得到下面的信息（信息会更新，你们得到的可能与我的不同）：
![这里写图片描述](https://img-blog.csdn.net/20180118095320241?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
首先我们来解释下咱们的https调用,第一部分（[https://api.github.com/](https://api.github.com/)）是将请求发送带github网站中响应API的部分；接下来的一部分（search/repositories）是让API搜索Github中所有的仓库。repositories后面的问好指出了我们要传递的第一个实参。q=表示查询，language:python表示查询项，&sort=stars，表示查询结果按照stars排序。
从我们张贴的图片可知：
现在github上面总共有2274030个项目，items下面就是最受欢迎的项目了。
# 处理API响应
接下来我们需要借助request库来请求API响应，并获取最受欢迎的github项目信息：
```python
r = requests.get(url)
```
```python
# In[11]:
```
```python
print(
```
```python
"Status code:"
```
```python
,r.status_code)
```
```python
# In[12]:
```
```python
response_dict = r.json()
```
```python
# In[14]:
```
```python
print(response_dict.keys())
```
```python
# In[15]:
```
```python
print(
```
```python
"Total repositories:"
```
```python
, response_dict[
```
```python
'total_count'
```
```python
])
```
```python
# In[16]:
```
```python
# Explore information about the repositories.
```
```python
repo_dicts = response_dict[
```
```python
'items'
```
```python
]
print(
```
```python
"Repositories returned:"
```
```python
,len(repo_dicts))
```
```python
# In[17]:
```
```python
repo_dict = repo_dicts[
```
```python
0
```
```python
]
print(
```
```python
"\nKeys:"
```
```python
,len(repo_dict))
```
```python
for
```
```python
key
```
```python
in
```
```python
sorted(repo_dict.keys()):
    print(key)
```
```python
# In[18]:
```
```python
print(
```
```python
"\nSelected information about first repository:"
```
```python
)
print(
```
```python
'Name:'
```
```python
,repo_dict[
```
```python
'name'
```
```python
])
print(
```
```python
'Owner:'
```
```python
,repo_dict[
```
```python
'owner'
```
```python
][
```
```python
'login'
```
```python
])
print(
```
```python
'Stars:'
```
```python
,repo_dict[
```
```python
'stargazers_count'
```
```python
])
print(
```
```python
'Repository:'
```
```python
,repo_dict[
```
```python
'html_url'
```
```python
])
print(
```
```python
'Created:'
```
```python
,repo_dict[
```
```python
'created_at'
```
```python
])
print(
```
```python
'Updated:'
```
```python
,repo_dict[
```
```python
'updated_at'
```
```python
])
print(
```
```python
'Description:'
```
```python
,repo_dict[
```
```python
'description'
```
```python
])
```
信息如下：
![这里写图片描述](https://img-blog.csdn.net/20180118112509011?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20180118112509011?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其中我们可以或者，最受欢迎的项目名称为：awesome-python，创建于2014年，截止到我写这篇博客时，依旧有更新，图片最后一行是对这个项目的描述：一组很棒的Python框架、库、软件和资源的列表。所以，这就是个各个python资料的链接中心。。。
[

](https://img-blog.csdn.net/20180118112509011?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 利用pygal可视化受欢迎的github仓库
[
](https://img-blog.csdn.net/20180118112509011?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)这里我们要创建一个交互式的条形图svg：条形图高度表示了项目获得了多少颗星。单击条形图还能跳转到项目在github上的主页。图片如下：
![这里写图片描述](https://img-blog.csdn.net/20180118113922462?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 项目源码
github ：[传送门](https://github.com/shawshany/pygal-webapi)

[
](https://img-blog.csdn.net/20180118112509011?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
