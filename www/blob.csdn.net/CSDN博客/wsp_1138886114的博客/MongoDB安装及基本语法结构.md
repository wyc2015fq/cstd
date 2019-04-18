# MongoDB安装及基本语法结构 - wsp_1138886114的博客 - CSDN博客





2018年05月22日 16:04:46[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：77








### 一、MongoDB 数据库的特点及安装
        面向文档，模式自由
        json数据模式（bson）（可以初略理解为字典）
        多级引索
        高可用复制集
        水平扩展
        跨平台、多种语言接口
        弱事务类型

        大数据、高并发、弱事务的web2.0互联网应用。

**优点**：弱一致性，文档格式存储方式，内置gridFS（分布式文件系统）


** 缺点**：不支持事务，空间占用大 


**安装**：一路点击next，finish

**配置环境变量**：将bin目录配置到环境变量中

        执行：cmd>mongod –v        #测试环境变量是否配置好

        新建如下文件：


![](https://img-blog.csdn.net/20180521160536666)

![](https://img-blog.csdn.net/20180521160643313)![](https://img-blog.csdn.net/20180521161351726)

```python
dbpath=C:\Program Files\MongoDB\Server\3.6\data               #数据库路径 软件安装目录（我的是默认目录）  
    logpath=C:\Program Files\MongoDB\Server\3.6\log\mongo.cfg     #日志输出文件路径   
    logappend=true     #错误日志采用追加模式    
    journal=true       #启用日志文件，默认启用    
    quiet=true         #这个选项可以过滤掉一些无用的日志信息，若需要调试使用请设置为false    
    port=27017         #端口号 默认为27017
```

**安装MongoDB服务**：(cmd以管理员身份执行)
        mongod --config "C:\Program Files\MongoDB\Server\3.6\mongo.conf" --install --serviceName "MongoDB"
**启动服务**：cmd 管理员方式启动>net start MongoDB


### 二、基本语法：

**创建库**：use db_name       #使用/创建数据库（若不存在则创建该数据库）
                     db                        #检查当前选择的数据库
                     show dbs              #显示当前有哪些数据库
                     show tables/show collections     #查看数据表

**删除库** :  db.dropDatabase()    #这将删除选定的数据库。如果没有选择任何数据库，默认删除Test数据库。

**创建集合:**
            在mongodb里面没有表的概念，集合（collections）就相当于传统数据库的表。
            语法：db.createCollection(name,option)    
            演示：


```python
>use test                               #进入该数据库
                switched to db test
                >db.createCollection("mycollection")    #创建myCollection 集合
                {"ok",1}
                >show collections                       #检查创建集合的命令
                mycollection
                system.indexes
```
**删除集合:**

```python
>use mydb                                #检查可用的集合在数据库 mydb
                switched to db mydb
                >show collections                        #检查集合
                    mycol
                    mycollection
                >db.mycollection.drop()                  #删除集合
                true
```
                drop()方法返回true，成功丢弃；否则返回false

```python
>show collections                        #再次检查数据库中集合列表
                    mycol
                system.indexes
                yiibai
```

**MongoDB常见数据类型**
            String：           最常用的储存的数据类型。MongoDB中字符串必须是utf-8。
            Integer：         用来储存数值。（32位/64位）
            Boolean：       储存布尔值（true/false）
            Double:           储存浮点值。
            Min/Max keys:用来对Bson元素最低最高值比较。
            Arrays：           使用此类型的数组/列表/多个值储存到一个键
            Timestamp：    时间戳。（方便记录文件已被修改/添加）

            Object：          用于嵌入式文件。
            Null：              储存null值。
            symbol：         用于字符串相同，但它通常保留给特定符号类型的语言使用。
            Date：             储存当前日期/时间的Unix时间格式。可以指定自己的日期和时间/日期和年月日到创建对象。
            ObjectID：      用于储存文档的ID。
            Binary data :    储存二进制数据。
            Code：            用于储存到文档中的Javascript代码。
            regular expression:    用于储存正则表达式


**MongoDB插入文档** Insert()/save方法
            语法：insert基本语法如下
                      >db.COLLECTION_NAME.insert(document)

**     插入单个文档:**
                演示:


```python
db.mycol.insert({
                title:        'MongoDB Overview',
                description:'MongoDB id no sql database',
                by:            'tutorials yiibai',
                url:        'http://www.yiibai.com',
                tags:        ['mongodb','database','NoSQL'],
                like:        100
                })
                db.collection_name.find()            #显示插入的数据
```

**     插入多个文档 :**   db.mycol.insert([{},{}])

```python
db.score.insert([{name:"张三",subject:"地理",score:88},
                               {name:"李四",subject:"语文",score:87}])
```

####        MongoDB更新文档

            update()/save()方法，updata()更新现有文档值 / save()用于替换现有文档值。 
**updata()基本语法如下**：
            >db.COLLECTION_NAME.update(selection_criteria,updated_data)

**update()演示**


```python
#mycol集合文件：
                {"_id":ObjectID(4685481851451we55),"title":"MongoDB overview"}
                {"_id":ObjectID(4685481851451we56),"title":"NoSQL overview"}
                {"_id":ObjectID(4685481851451we57),"title":"tutorials overview"}
            
                >db.mycol.update({"title":"MongoDB overview"},
                {$set: {"title":"tutorials overview"}})                 #更新标题
            
                >db.mycol.update({"title":"MongoDB overview"},
                {$set: {"title":"tutorials overview"}},{multi:true})    #要实现多个更新在后面加**{multi:true}**
                >db.mycol.find()                                        #查询插入的数据
```
**save()方法**    替换现有符合此ID文档的所有数据
                >db.collection_name.save({id.ObjectID(),new_data})

**save()演示：**

```python
db.mycol.save({"_id":ObjectID(5689745956adf56as7),
                            "title":"yiibai new topic",
                            "by":"yiibai"})
                >db.mycol.find()                                        #查询插入的数据
```

####        MongoDB删除文档

            remove()方法，用于从集合中删除文档。remove()方法接受两个参数，一是删除criteria，二是justOne标识。
            deletion criteria:（可选）删除标准，根据文件将被删除。
            justOne：（可选）如果设置为true /1，然后只删除一个文件。

**语法**：remove基本语法如下
                >**db.collection_name.remove**(deletion_criteria,justOne)


```python
mycol集合文件：
                {"_id":ObjectID(4685481851451we55),"title":"MongoDB overview"}
                {"_id":ObjectID(4685481851451we56),"title":"NoSQL overview"}
                {"_id":ObjectID(4685481851451we57),"title":"tutorials overview"}    
            
                >db.mycol.remove(
                {'title':'MongoDB Overview'})            #删除符合此id的所有文件
        
                >db.mycol.find()                         #查看移除后的集合
                {"_id":ObjectID(4685481851451we56),
                 "title":"NoSQL overview"}
                {"_id":ObjectID(4685481851451we57),
                 "title":"tutorials overview"}
```
            如果有多条记录且要删除第一条记录，
            设置remove()方法中justOne参数:    
                >db.collection_name.remove(deletion_criteria,1)

            演示：
                先增加两条记录：>db.mycol.insert({'title':'a','click':2})
                                           >db.mycol.insert({'title':'a','click':3})
                再删除：    
                        db.mycol.remove({'title':'a'},1)
                发现只删除第一条，
                若不加参数1，则两条记录都会被删除！

####        MongoDB查询文档
|操作|语法|示例|RDBMS等效语句|
|----|----|----|----|
| | | | |
|相等|`{<key>:<value>}`|`db.mycol.find({"by":"yiibai"}).pretty()`|`where by = 'yiibai'`|
|小于|`{<key>:{$lt:<value>}}`|`db.mycol.find({"likes":{$lt:50}}).pretty()`|`where likes < 50`|
|小于等于|`{<key>:{$lte:<value>}}`|`db.mycol.find({"likes":{$lte:50}}).pretty()`|`where likes <= 50`|
|大于|`{<key>:{$gt:<value>}}`|`db.mycol.find({"likes":{$gt:50}}).pretty()`|`where likes > 50`|
|大于等于|`{<key>:{$gte:<value>}}`|`db.mycol.find({"likes":{$gte:50}}).pretty()`|`where likes >= 50`|
|不等于|`{<key>:{$ne:<value>}}`|`db.mycol.find({"likes":{$ne:50}}).pretty()`|`where likes != 50`|
            MongoDB查询文档--find()
                语法：>db.collection_name.find()            #将查询结果非结构化显示文件
            MongoDB查询文档--pretty()                    
                语法：>db.collection_name.find().pretty()    #将查询结果格式化显示文件
                示例:

```python
>db.collection_name.find().pretty()
            {    
                "title":    "MongoDB overview"，
                'description':'MongoDB is no sql',
                'by':        'tutorials yiibai',
                'url':        'http://www.yiibai.com',
                'tags':        ['mongodb','database','NoSQL'],
                'like':        100
             }
```
            findOne()方法，只返回查询到的符合结果的第一个文件。

            and 在MongoDB中的用法
                语法：>db.mycol.find({key1:value1,key2:value2,}).pretty()    #and条件查询 类似于MySQL中的where

            or 在MongoDB中的用法
                语法：>db.mycol.find({**$or**:[{key1:value1},{key2:value2}]}).pretty()

            示例：

```python
>db.mycol.find(                        
            {$or:                                                #$or方法查询
                [{'by':'yiibai'},{"title":"MongoDB overview"}]
            }
        ).pretty()
        
        {
            'description':'MongoDB is no sql',
            'by':        'yiibai',
            'url':        'http://www.yiibai.com',
            'tags':        ['mongodb','database','NoSQL'],
            'like':        100
        }
```
            limit限制记录
                limit()方法：该方法接受数字型参数，这是要显示的文档数。
                语法：>db.collection_name.find().limit(number)
                示例：
                    mycol集合有以下数据

```python
{'title':'a','clicks':1}
                    {'title':'h','clicks':2}
                    {'title':'c','clicks':3}
                >db.mycol.find().limit(2)        #limit限制查询
                    {'title':'a','clicks':1}
                    {'title':'h','clicks':2}
```
            skip()跳过文档数
                除limit()外，skip()也接受数字型参数,跳过几条记录，取剩下的记录
                语法：>db.collection_name.find().limit(number).skip(number)
















