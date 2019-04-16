# 地铁译：Spark for python developers ---构建Spark批处理和流处理应用前的数据准备 - 我相信...... - CSDN博客





2016年05月17日 06:54:45[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：14285
所属专栏：[Python 开发者的Spark](https://blog.csdn.net/column/details/12815.html)









> 
从github，twitter，meetup 获得数据，并理解数据，由于GFS， 代码的执行要翻墙的……


使用PySpark和PyData相关库构建应用来分析社交网络中含有Spark的交互信息.  我们从GitHub收集有关Apache  Spark的信息, 在Twitter上检查相关的tweets, 使用 Meetup从更广泛的开源社区得到更多Spark 相关感受。  本章中,  我们将概览各种信息和数据源，理解他们的结构，从批处理到流处理介绍数据处理流水线，要点如下: 

 +  从批处理到流处理介绍数据处理管道, 有效的描述准备构建的应用架构. 

+  获取各种数据源 (GitHub, Twitter, 和Meetup),  包括他们的数据结构(JSON, 结构信息, 非结构化文本, 地理信息,时序数据等)和复杂性. 

 我们也讨论了三种API的连接工具,所以科可以构建自己的混搭数据。 下面重点关注Twitter.
# 架构数据密集型应用

我们已经定义了数据密集型应用框架的架构，回顾一下各种软件组件的应用场景. 这是数据密集型应用框架中软件组件的解释。

![highlight integration](https://img-blog.csdn.net/20160510145439876)

Spark 是非常高效的分布式计算框架 ，为了展示它全部的能力，需要相应地架构我们的解决方案。出于性能的约束, 整体方案需要关注CPU,存储和网络的使用率。

影响架构的必要条件: 

 + 延迟: 该架构要兼顾快速和慢速处理.批处理模式中历史数据是慢处理，这叫做闲置数据（data in rest）. 对于持续流入系统的数据构建欲计算模型和数据模式需要适应快速处理。数据的快速处理或者流数据的实时分析指的是数据处于运动状态。批处理模式的闲置数据处理有较大的延迟. 运动数据指的是获取数据进行实时流计算. 

+ 伸缩性: 通过分布式内存计算框架，Spark是天然线性扩展的。数据库和数据存储与Spark 的交互需要更加数据的增长线性伸缩。 

+ 容错:  由于硬件,软件, 或网络的原因发生故障时, 架构要有足够的弹性并在所有时间内可用. 

+ 灵活性:  架构中的数据管道拥有适配能力，可以根据使用条件快速更新。 


Spark是唯一允许批处理和流处的统一平台。考虑两种数据处理流水线: 

 • 首先是闲置数据处理，专注数据的批处理分析 

 • 第二个是运动数据, 目标是基于预计算模型和数据模式进行实时数据处理 

## 处理闲置数据

 让我们来理解一下闲置数据处理流水线. 该流水线的目的是从Twitter,GitHub,和Meetup 获取数据; 以机器学习引擎Spark  MLlib准备数据; 基于基本模型在批处理模式生成见解。 

 下图解释了闲置数据处理的数据流水: 
![2-2闲置数据处理](https://img-blog.csdn.net/20160511083605735)
## 处理运动数据

运动数据处理引入了新的复杂性，就像故障的可能性那样。如果考虑到伸缩性, 我们需要使用例如 Kafka这样的分布式消息队列.  我们专有一章来理解流式分析.  

下图解释运动数据处理的流水线: 
![2-3处理运动数据](https://img-blog.csdn.net/20160511084003702)
# 交互性探索数据

构建数据密集型应用没有直接暴露数据库的web接口。在建立闲置数据和运动数据处理期间，要充分利用 Spark’s 的能力来交互式的分析数据，使用机器学习和流式处理提炼数据的内涵和质量.  这里，为了得到我们使用感兴趣的数据集，我们要审视数据采集，提炼和调查的一个迭代周期.  

## 连接社交网络

 我们深入到数据密集型应用架构集成层的第一步。重点是保证所采集数据的一致性，为下一步Spark进行批处理或流处理做准备。此阶段分为5步:  connect, correct, collect, compose和consume.  数据探索的这些迭代步骤是我们能够熟悉数据，帮助我们为将来的处理提炼数据结构. 


下图解释了数据获取和提炼的迭代流程： 
![2-4 数据集成6C](https://img-blog.csdn.net/20160511084613001)

我门连接的社交网络有: Twitter, GitHub, 和Meetup.  讨论 各个API的访问模式，以及如何创建这些服务的 RESTful 风格连接，同时关注这些网络的访问限制.  消息的交换主要依于JSON格式.  RESTful  APIs和web  services 实现了4个主要的法GET, PUT, POST和DELETE. GET 拥有从给定的URI中提取元素或集合，PUT更新内容，POST允许创建新的条目, DELETE删除集合.   

## 获得Twitter 数据

授权协议OAuth允许应用安全地调用Twitter服务访问用户的相关数据，访问注册用户的tweet 服务。为了创建连接，首先要在twitter上创建一个应用，网址 [https://apps.twitter.com/app/new](https://apps.twitter.com/app/new). 
![2-5 在twitter上创建应用](https://img-blog.csdn.net/20160511085415508)
一旦应用创建成功, 使用 Twitter产生的4个键来访问Twitter的空间:  

```
CONSUMER_KEY  =  'GetYourKey@Twitter' 
CONSUMER_SECRET  =  '  GetYourKey@Twitter'
 OAUTH_TOKEN  =  '  GetYourToken@Twitter' 
 OAUTH_TOKEN_SECRET  =  '  GetYourToken@Twitter'
```

如果希望自如地使用RESTful的各种查询, 可以在dev  console上学习一下Twitter API，网址：[https://dev.twitter.com/rest/tools/console](https://dev.twitter.com/rest/tools/console): 
![2-6 Twitter REST API](https://img-blog.csdn.net/20160511085945944)
我们将使用后面的代码可以在Twitter上生成一个编程时的连接，这里激活了 OAuth的访问并在一定限制下使用 Twitter  API.  在流模式,  GET请求是受限的.   

## 获得GitHub数据

GitHub 使用了与Twitter类似的认证过程. 先到这个开发者网站注册，然后获得证书[https://developer.github.com/v3/](https://developer.github.com/v3/):  

![2-7 获得GitHub数据](https://img-blog.csdn.net/20160511090511446)

## 获得Meetup数据

Meetup.com 会员可以使用token访问开发者资源.  从开发者网站可以获得Meetup  API所需的token和OAuth证书，网址 [https://secure.meetup.com/meetup_api](https://secure.meetup.com/meetup_api): 
![2-8 获得Meetup数据](https://img-blog.csdn.net/20160511091039874)
# 分析数据

让我们先感受一下从社交网络获得的数据，理解每个数据源的数据结构.  

## tweets剖析

 通过 Twitter API建立连接有两种模式:  REST  API 和流式API,  REST API允许根据指定的词或标签搜索历史tweets，流式API在一定限制下实时地传递 tweets. 


为了更好地理解如何操作 Twitter  API,  完成以下步骤: 

 1. 安装Twitter  Python库.  

2.  通过OAuth 建立Twitter的可编程连接  

3.  查询Apache  Spark 搜索最近的 tweet并浏览得到的结果.  

4.  判定关键属性，从JSON输出中提取信息. 


循序渐进: 

 1. 安装 Python  Twitter  库，键入命令: ` $  pip  install  twitter `
 2. 创建 Python  Twitter  API  类，其基本方法用于认证，搜素和解析结果。 self.auth  从 Twitter获得证书，以self.api 创建注册的。实现了两个方法： 第一个是根据指定查询搜索，第二个是解析输出提取信息例如tweet ID,  tweet 文本,  和tweet作者.  代码如下：

```python
import  twitter 
import  urlparse 
from  pprint  import  pprint  as  pp  
class  TwitterAPI(object): 
""" TwitterAPI  class  allows  the  Connection  to  Twitter  via  OAuth once  you  have  registered  with  Twitter  and  receive  the necessary  credentiials """  
  #  initialize  and  get  the  twitter  credentials 
 def  __init__(self): 
     consumer_key  =  'Provide  your  credentials' 
     consumer_secret  =  'Provide  your  credentials' 
     access_token  =  'Provide  your  credentials' 
      access_secret  =  'Provide  your  credentials'  
      self.consumer_key  =  consumer_key
    self.consumer_secret  =  consumer_secret 
    self.access_token  =  access_token
    self.access_secret  =  access_secret

 #
 #  authenticate  credentials  with  Twitter  using  OAuth
    self.auth  =  twitter.oauth.OAuth(access_token,  access_
secret,  consumer_key,  consumer_secret)
 #  creates  registered  Twitter  API
    self.api  =  twitter.Twitter(auth=self.auth)
 #
 #search Twitter with query q(i.e."ApacheSpark") and max. result
def  searchTwitter(self,  q,  max_res=10,**kwargs):
    search_results  =  self.api.search.tweets(q=q,  count=10,
**kwargs)
    statuses  =  search_results['statuses']
    max_results  =  min(1000,  max_res)

    for  _  in  range(10):
    try:
        next_results =  search_results['search_metadata'['next_results']
    except  KeyError  as  e:
        break

    next_results  =  urlparse.parse_qsl(next_results[1:])
    kwargs  =  dict(next_results)
    search_results  =  self.api.search.tweets(**kwargs)
    statuses  +=  search_results['statuses']

    if  len(statuses)  >  max_results:
        break
    return  statuses
 #
 # parse tweets as it is collected to extract id, creation
 #  date,  user  id,  tweet  text
def  parseTweets(self,  statuses):
    return  [  (status['id'],
                status['created_at'],
                status['user']['id'],
                status['user']['name'],
                status['text'],  url['expanded_url'])
                    for  status  in  statuses
                        for  url  in  status['entities']['urls']
```

3 实例化所需认证的类: 
`t=  TwitterAPI()`

4 运行查询条目为 Apache  Spark的搜索: 
```
q="ApacheSpark"
tsearch  =  t.searchTwitter(q)
```

5 分析  JSON 输出:

```python
pp(tsearch[1])

{u'contributors':  None,
u'coordinates':  None,
u'created_at':  u'Sat  Apr  25  14:50:57  +0000  2015',
u'entities':  {u'hashtags':  [{u'indices':  [74,  86],  u'text':
u'sparksummit'}],
u'media':  [{u'display_url':  u'pic.twitter.com/
WKUMRXxIWZ',
u'expanded_url':  u'http://twitter.com/
bigdata/status/591976255831969792/photo/1',
u'id':  591976255156715520,
u'id_str':  u'591976255156715520',
u'indices':  [143,  144],
u'media_url':
...(snip)...
u'text':  u'RT  @bigdata:  Enjoyed  catching  up  with  @ApacheSpark
users  &amp;  leaders  at  #sparksummit  NYC:  video  clips  are  out
http://t.co/qrqpP6cG9s  http://t\u2026',
u'truncated':  False,
u'user':  {u'contributors_enabled':  False,
u'created_at':  u'Sat  Apr  04  14:44:31  +0000  2015',
u'default_profile':  True,
u'default_profile_image':  True,
u'description':  u'',
u'entities':  {u'description':  {u'urls':  []}},
u'favourites_count':  0,
u'follow_request_sent':  False,
u'followers_count':  586,
u'following':  False,
u'friends_count':  2,
u'geo_enabled':  False,
u'id':  3139047660,
u'id_str':  u'3139047660',
u'is_translation_enabled':  False,
u'is_translator':  False,
u'lang':  u'zh-cn',
u'listed_count':  749,
u'location':  u'',
u'name':  u'Mega  Data  Mama',
u'notifications':  False,
u'profile_background_color':  u'C0DEED',
u'profile_background_image_url':  u'http://abs.twimg.
com/images/themes/theme1/bg.png',
u'profile_background_image_url_https':  u'https://abs.
twimg.com/images/themes/theme1/bg.png',
...(snip)...
u'screen_name':  u'MegaDataMama',
u'statuses_count':  26673,
u'time_zone':  None,
u'url':  None,
u'utc_offset':  None,
u'verified':  False}}
```

6.Parse  the  Twitter  output  to  retrieve  key  information  of  interest:

```python
tparsed  =  t.parseTweets(tsearch)
pp(tparsed)

[(591980327784046592,
u'Sat  Apr  25  15:01:23  +0000  2015',
63407360,
u'Jos\xe9  Carlos  Baquero',
u'Big  Data  systems  are  making  a  difference  in  the  fight  against
cancer.  #BigData  #ApacheSpark  http://t.co/pnOLmsKdL9',
u'http://tmblr.co/ZqTggs1jHytN0'),
(591977704464875520,
u'Sat  Apr  25  14:50:57  +0000  2015',
3139047660,
u'Mega  Data  Mama',
u'RT  @bigdata:  Enjoyed  catching  up  with  @ApacheSpark  users  &amp;
leaders  at  #sparksummit  NYC:  video  clips  are  out  http://t.co/
qrqpP6cG9s  http://t\u2026',
u'http://goo.gl/eF5xwK'),
(591977172589539328,
u'Sat  Apr  25  14:48:51  +0000  2015',
2997608763,
u'Emma  Clark',
u'RT  @bigdata:  Enjoyed  catching  up  with  @ApacheSpark  users  &amp;
leaders  at  #sparksummit  NYC:  video  clips  are  out  http://t.co/
qrqpP6cG9s  http://t\u2026',
u'http://goo.gl/eF5xwK'),
...     (snip)...
(591879098349268992,
u'Sat  Apr  25  08:19:08  +0000  2015',
331263208,
u'Mario  Molina',
u'#ApacheSpark  speeds  up  big  data  decision-making  http://t.
co/8hdEXreNfN',
u'http://www.computerweekly.com/feature/Apache-Spark-speeds-up-
big-data-decision-making')]
```

## 探索GitHub 世界

为了更好地理解如何操作  GitHub  API,  完成如下步骤: 

1. 安装 GitHub  Python  library. 

2. 使用从开发者网站提供歌token访问API . 

3. 从Apache  foundation  托管的spark  仓库中获取一些重要的信息. 

 具体步骤如下:  

 1. 安装 Python  PyGithub  library. 键入命令: ` pip  install  PyGithub `
 2. 实例化  GitHub  API 创建一个客户端: 


```python
from  github  import  Github  
    #  Get  your  own  access  token  
    ACCESS_TOKEN  =  'Get_Your_Own_Access_Token'  
    #  We  are  focusing  our  attention  to  User  =  apache  and  Repo  =  spark  
    USER  =  'apache'  
    REPO  =  'spark'  
    g  =  Github(ACCESS_TOKEN,  per_page=100) 
    user  =  g.get_user(USER)
    repo  =  user.get_repo(REPO)
```
- 从Apache  User.中提取重要信息，  GitHub中有640个Apache仓库:  

```
repos_apache  =  [repo.name  for  repo  in  g.get_user('apache').get_ repos()]  
 len(repos_apache)  
 640
```
- 从Spark 仓库中获取信息,   Spark  repo中使用的编程语言如下 :

```python
pp(repo.get_languages())  
{u'C':  1493, u'CSS':  4472, u'Groff':  5379, u'Java':  1054894, u'JavaScript':  21569, u'Makefile':  7771, u'Python':  1091048, u'R':  339201, u'Scala':  10249122, u'Shell':  172244}
```
- 提取  Spark  GitHub  仓库中的主要参与者本书写作时Apache  Spark仓库有3,738  参与者. 网络巨大，第一人是 Matei  Zaharia,在Berkeley读PhD的时候成为了Spark 项目的联合创始人.  

```python
stargazers  =  [  s  for  s  in  repo.get_stargazers()  ]
 print  "Number  of  stargazers",  len(stargazers)
  Number  of  stargazers  3738
    [stargazers[i].login  for  i  in  range  (0,20)]
     [u'mateiz', u'beyang', u'abo', u'CodingCat', u'andy327', u'CrazyJvm', u'jyotiska', u'BaiGang', u'sundstei', u'dianacarroll', u'ybotco',
u'xelax',
u'prabeesh',
u'invkrh',
u'bedla',
u'nadesai',
u'pcpratts',
u'narkisr',
u'Honghe',
u'Jacke']
```

## 通过Meetup理解社区

为了更好地理解如何操作  Meetup  API,  完成如下步骤:  

1. 使用认证Token 创建一个Python  程序调用  Meetup  API  . 

2. 从meetup 用户组例如London  Data  Science 提取过去的活动信息. 

3. 提取 meetup 成员的用户简介以便分析 类似  meetup  组的参与者. 
具体步骤:  
- 由于没有 Meetup  API  Python 库,  我们要编写一个客户端程序来实例化Meetup  API:   

```
import  json 
    import  mimeparse 
    import  requests
 import  urllib 
  MEETUP_API_HOST  =  'https://api.meetup.com' 
  EVENTS_URL  =  MEETUP_API_HOST  +  '/2/events.json' 
  MEMBERS_URL  =  MEETUP_API_HOST  +  '/2/members.json' 
  GROUPS_URL  =  MEETUP_API_HOST  +  '/2/groups.json'  
   RSVPS_URL  =  MEETUP_API_HOST  +  '/2/rsvps.json'  
    PHOTOS_URL  =  MEETUP_API_HOST  +  '/2/photos.json' 
 #  GROUP_URLNAME  =  'London-Machine-Learning-Meetup'  #  'Data- Science-London'
    class  MeetupAPI(object):
    """
    Retrieves  information  about  meetup.com
    """
    def  __init__(self,  api_key,  num_past_events=10,  http_
    timeout=1,http_retries=2):
    """
    Create  a  new  instance  of  MeetupAPI
    """
        self._api_key  =  api_key
        self._http_timeout  =  http_timeout
        self._http_retries  =  http_retries
        self._num_past_events  =  num_past_events

    def  get_past_events(self):
    """
    Get  past  meetup  events  for  a  given  meetup  group
    """
        params  =  {'key':  self._api_key,
        'group_urlname':  GROUP_URLNAME,
        'status':  'past',
        'desc':  'true'}
        if  self._num_past_events:
        params['page']  =  str(self._num_past_events)

        query  =  urllib.urlencode(params)
        url  =  '{0}?{1}'.format(EVENTS_URL,  query)
        response  =  requests.get(url,  timeout=self._http_timeout)
        data  =  response.json()['results']
        return  data

    def  get_members(self):
    """
    Get  meetup  members  for  a  given  meetup  group
    """
        params  =  {'key':  self._api_key,
        'group_urlname':  GROUP_URLNAME,
        'offset':  '0',
        'format':  'json',
        'page':  '100',
        'order':  'name'}
        query  =  urllib.urlencode(params)
        url  =  '{0}?{1}'.format(MEMBERS_URL,  query)
        response  =  requests.get(url,  timeout=self._http_timeout)
        data  =  response.json()['results']
        return  data

    def  get_groups_by_member(self,  member_id='38680722'):
        """
         Get  meetup  groups  for  a  given  meetup  member
        """
        params  =  {'key':  self._api_key,
        'member_id':  member_id,
        'offset':  '0',
        'format':  'json',
        'page':  '100',
        'order':  'id'}
        query  =  urllib.urlencode(params)
        url  =  '{0}?{1}'.format(GROUPS_URL,  query)
        response  =  requests.get(url,  timeout=self._http_timeout)
        data  =  response.json()['results']
        return  data
```

2.然后,  从一个给定的Meetup 组中提取过去的事件:

```python
m  =  MeetupAPI(api_key='Get_Your_Own_Key')
    last_meetups  =  m.get_past_events()
    pp(last_meetups[5])

    {u'created':  1401809093000,
    u'description':  u"<p>We are hosting  a  joint  meetup  between  Spark London  and  Machine  Learning  London.  Given  the  excitement  in  the machine  learning  community  around  Spark  at  the  moment  a  joint meetup  is  in  order!</p>  <p>Michael  Armbrust  from  the  Apache  Spark core  team  will  be  flying  over  from  the  States  to  give  us  a  talk  in person.\xa0Thanks  to  our  sponsors,  Cloudera,  MapR  and  Databricks for  helping  make  this  happen.</p>  <p>The  first  part  of  the  talk will  be  about  MLlib,  the  machine  learning  library  for  Spark,\xa0and  the  second  part,  on\xa0Spark  SQL.</p>  <p>Don't  sign  up  if you  have  already  signed  up  on  the  Spark  London  page  though!</p>
    <p>\n\n\nAbstract  for  part  one:</p>  <p>In  this  talk,  we\u2019ll introduce  Spark  and  show  how  to  use  it  to  build  fast,  end-to-end machine  learning  workflows.  Using  Spark\u2019s  high-level  API,  we can  process  raw  data  with  familiar  libraries  in  Java,  Scala  or Python  (e.g.  NumPy)  to  extract  the  features  for  machine  learning. Then,  using  MLlib,  its  built-in  machine  learning  library,  we  can run  scalable  versions  of  popular  algorithms.  We\u2019ll  also  cover upcoming  development  work  including  new  built-in  algorithms  andR  bindings.</p> <p>\n\n\n\nAbstract  for  part  two:\xa0</p>  <p>In this  talk,  we'll  examine  Spark  SQL,  a  new  Alpha  component  that  is part  of  the  Apache  Spark  1.0  release.  Spark  SQL  lets  developers natively  query  data  stored  in  both  existing  RDDs  and  external sources  such  as  Apache  Hive.  A  key  feature  of  Spark  SQL  is  the ability  to  blur  the  lines  between  relational  tables  and  RDDs,
    making  it  easy  for  developers  to  intermix  SQL  commands  that  query external  data  with  complex  analytics.  In addition  to  Spark  SQL, we'll  explore  the  Catalyst optimizer  framework,  which  allows Spark  SQL  to automatically  rewrite  query  plans  to  execute  more efficiently.</p>",

    u'event_url':  u'http://www.meetup.com/London-Machine-Learning-
    Meetup/events/186883262/',
    u'group':  {u'created':  1322826414000,
    u'group_lat':  51.52000045776367,
    u'group_lon':  -0.18000000715255737,
    u'id':  2894492,
    u'join_mode':  u'open',
    u'name':  u'London  Machine  Learning  Meetup',
    u'urlname':  u'London-Machine-Learning-Meetup',
    u'who':  u'Machine  Learning  Enthusiasts'},
    u'headcount':  0,
    u'id':  u'186883262',
    u'maybe_rsvp_count':  0,
    u'name':  u'Joint  Spark  London  and  Machine  Learning  Meetup',
    u'rating':  {u'average':  4.800000190734863,  u'count':  5},
    u'rsvp_limit':  70,
    u'status':  u'past',
    u'time':  1403200800000,
    u'updated':  1403450844000,
    u'utc_offset':  3600000,
    u'venue':  {u'address_1':  u'12  Errol  St,  London',
    u'city':  u'EC1Y  8LX',
    u'country':  u'gb',
    u'id':  19504802,
    u'lat':  51.522533,
    u'lon':  -0.090934,
    u'name':  u'Royal  Statistical  Society',
    u'repinned':  False},
    u'visibility':  u'public',
    u'waitlist_count':  84,
    u'yes_rsvp_count':  70}
```

3 获取Meetup的成员信息:

```python
members  =  m.get_members()

    {u'city':  u'London',
    u'country':  u'gb',
    u'hometown':  u'London',
    u'id':  11337881,
    u'joined':  1421418896000,
    u'lat':  51.53,
    u'link':  u'http://www.meetup.com/members/11337881',
    u'lon':  -0.09,
    u'name':  u'Abhishek  Shivkumar',
    u'other_services':  {u'twitter':  {u'identifier':  u'@
    abhisemweb'}},
    u'photo':  {u'highres_link':  u'http://photos3.meetupstatic.com/
    photos/member/9/6/f/3/highres_10898643.jpeg',
    u'photo_id':  10898643,
    u'photo_link':  u'http://photos3.meetupstatic.com/
    photos/member/9/6/f/3/member_10898643.jpeg',
    u'thumb_link':  u'http://photos3.meetupstatic.com/
    photos/member/9/6/f/3/thumb_10898643.jpeg'},
    u'self':  {u'common':  {}},
    u'state':  u'17',
    u'status':  u'active',
    u'topics':  [{u'id':  1372,  u'name':  u'Semantic  Web',  u'urlkey':
    u'semweb'},
    {u'id':  1512,  u'name':  u'XML',  u'urlkey':  u'xml'},
    {u'id':  49585,
    u'name':  u'Semantic  Social  Networks',
    u'urlkey':  u'semantic-social-networks'},
    {u'id':  24553,
    u'name':  u'Natural  Language  Processing',
    ...(snip)...
    u'name':  u'Android  Development',
    u'urlkey':  u'android-developers'}],
    u'visited':  1429281599000}
```
















