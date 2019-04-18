# pymongo操作MongoDB数据库 - wsp_1138886114的博客 - CSDN博客





2018年05月22日 16:03:06[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：764








### 一、python在MongoDB中插入数据

        insert_one(doc)
        insert_many(doc,ordered = True/False)    #批量插入
            #ordered:  True，顺序插入，若第一条失败，则批量插入失败
                              False，不启用顺序插入，插入失败的数据不影响其他数据的插入。

####       安装pymongo:       


            >pip install pymongo    安装执行（C:\Windows\System32\cmd.exe）        



####       启动MongoDB服务器：
            >cd /d E:\MongoDB\bin                #进入bin文件夹
            >mongod -dbpath E:\MongoDB\data\db    #这是默认端口处启动服务器，并指定在E:\MongoDB\db1 处储存数据   (这个窗口不能关)

####       python中利用MongoDBClient连接MongoDB服务器

```python
#encoding:utf-8
            import pymongo
            from pymongo import MongoClient         #导入MongoDb第三方库
            connect = MongoClient('localhost')      #连接本地MongoDB服务器/127.0.0.1
            db = connect.mongo_demo                 #连接（创建）mongo_demo数据库
            employees = db.employees                #连接（创建）employees数据集
            employees.remove(None)                  #先清空一下数据集
```

####        插入一条记录

```python
zhangsan = {
                'name':'zhangsan',
                'age':'30',
                'sex':'boy',
                'contact':{
                    'email1':'abc@def.com',
                    'email2':'dec@dcf.com'
                }
            }
            employees.insert_one(zhangsan)
```

####        插入多条记录（多个文档）      

```python
lisi = {
                'name':'lisi',
                'habit':{
                    'habit1':'eat',
                    'habit2':'sleep'
                }    
            }      
            wangwu = {
                'name':'wangwu',
                'age':20,
                'sex':'boy'    
                }    
            result = [lisi,wangwu]
            employees.insert_many(result,True)
            print("插入成功")
```

### 二、python在MongoDB中查询数据

**查询：find(filter)**

```python
$gt	#Greater Than		>
			$lt	#Less Than 		<
			$gte	#Greater Than or Equal	>=
			$lte	#Less Than or Equal  	<=
			
			$in	在
			$nin	不在
			$nor	不或
			$or	或
			$size	大小
```

#####               方式1：无条件查询（查询所有）          

```python
cursor = employees.find({})    #查询所有        
                for employee in cursor:        #遍历输出
                    print(employee)
```

#####               方式2：按名字查询          

```python
cursor = employees.find({'name':'zhangsan'})    
                for employee in cursor:
                    print(employee)
```

#####               方式3：张三或者李四
**$in的使用**

```python
cursor = employees.find({'name':{"$in":['zhangsan','lisi']}})    
                for employee in cursor:
                    print(employee)
```

#####               方式4：$gt的使用                  

```python
cursor = employees.find({'age':{"$gt":25}})    
                for employee in cursor:
                    print(employee)        #age>25
```

#####               方式5：and多条件查询（符合A并且符合B）

```python
cursor = employees.find({                
                    'name':{"$in":['zhangsan','lisi']}，**    #and的使用**
                    'age':{"$gt":25}
                })                                        
                for employee in cursor:
                    print(employee)
```

#####               方式6：（符合A或者符合B）    

```python
cursor = employees.find({                
                    'name':{"$or":[{'name':{'$in':['zhangsan','lisi']}},    **#or 的使用**
                    'age':{"$gt":30}
                })                                        
                for employee in cursor:
                    print(employee)         #查找名字叫（张三或者李四）或者年纪（大于30）
```

#####               方式7：子条件查询（嵌套查询）

```python
employeecursor = employees.find({'contact.email1':'abc@def.com'})
                for employee in employeecursor:
                    print(employee)
```

### 三、python在MongoDB中数据更新
        update_one(filter,update,upset = True/False)
        update_many(filter,update,upset = True/False)


```python
示例1：将wangwu的'sex'字段改为'girl' 
            employees.update_one{
                {'name':'wangwu'},
                {$set:{'sex':'girl'}}    #  $set 更新操作
                }
```

```python
示例2：
            #    $inc 添加字段    如果记录中没有这个字段，增加此字段
            students.update_many(
                {},                       #条件，若为空，相当于无条件，即更新所有
                {'$inc':
                        {'age':2}         #在原来的基础上增加2岁，若无则年纪为2
                }
            )
```

```python
示例3：          
        #    $min 与当前值相比，取最小值进行更新(小于当前值则不更新)
            employees.update_many(
                {'name':
                    {'$in':['xhangsan','lisi']}    
                },
                {'$min':{'age':20}}        $min 与当前值相比，取最小值进行更新
            )
```

```python
示例4：
        #    $currentDate    代表时间戳模式
        employees.update_many(
            {'name':
                {'$in':['zhangsan':'lisi']}
            },
            {'$currentDate':
                {'create_time':True,                #相当于是：{'$type':'date'},生产日期时间段
                'mod_time':{'$type':'timestamp'}    #产生时间戳格式字段
            }
        )
```

```python
示例5：    更新内嵌文档部分字段
        employees.update_one(
            {'name':'lisi'},
            {
                'set':{'habit.habit1':'watch tv'}
            }
        )
```

#####             删除数据
                delete_one(filter)
                delete_many(filter)

                示例：
                employees.delete_many({'name':'lisi'})    #删除符合条件的所有数据
                employees.delete_one('age':20)            #删除符合条件的第一条数据

#####             查询同时更新
                find_one_and_replace
                find_one_and_delete
                find_one_and_update

                演示查询更新
                    #查询所有记录，将查询到的结果增加一条字段：locked：1，
                    #同时将age字段里的值增加2，返回的结果中只包含age,name,
                    #同时将返回结果按age降序排列，最后返回该记录修改之前(之后)的状态。

```python
record = employees.find_one_and_update(
            {},
            {'$set':{'lock':1},
            '$inc':{'age':2]
            }，
            projection = {'_id':False,'age':True,'name':True},    #其中'_id':False,为不显示ID
            sort = [('age',pymongo.descending)],
            return_document = pymongo.ReturnDocument.before/after    
        )
```
                加载外部.json文件

```python
import json
        from pymongo import MongoClient
        
        connect = MongoClient('localhost')
        db = connect.rddb
        col = db.zips
        col.remove(None)
        file = open('.\zips.json')    #打开文件
        for line in file:
            x = json.loads(line)
            #print x
            col.insert_one(x)
        file.close()
```
封装函数：查询--添加数据


```python
#encoding:utf-8
        from pymongo import MongoClient
        class Connect():
            def __init__(self,dbName,collectionName):
                self.connect = MongoClient('localhost')
                self.db = self.connect[dbName]
                self.collection = self.db[collectionName]
            def get(self):
                return self.collection
```






