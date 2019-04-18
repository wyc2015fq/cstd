# pyMongo+Excel+爬虫——练习题 - wsp_1138886114的博客 - CSDN博客





2018年05月24日 11:48:20[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：116








### 一、读取MongoDB数据

```python
#encoding:utf-8
from pymongo import MongoClient            
class Connect():                                        #创建类并封装函数方便以后使用
    def __init__(self,dbName,collectionName):
        self.connect = MongoClient('localhost')
        self.db = self.connect[dbName]
        self.collection = self.db[collectionName]
    def get(self):
        return self.collection
```

```python
from day16_pymongo.def_mongo_class import Connect       #导入文件中的类
grade_1_3 = Connect('school','grade_1_3').get()         #连接服务器->数据库school->表（集合）grade_1_3
students = grade_1_3.find({},{'_id':0})                 #mongo语句：查询所有，不显示_id
for doc in students:                                    #遍历输出
    print(doc)
```

### 二、综合练习

##### 2.1读取rooms.csv文件的中的数据，并将数据插入到MongoDB数据库的rooms表中


```python
方法1：
    from pymongo import MongoClient
    import csv
    #from day16_pymongo.def_mongo_class import Connect
    #rooms = Connect('localhost','building','rooms').get()
    connect = MongoClient("localhost")
    db = connect.building
    rooms = db.rooms                              #导入模块和连接服务器->连接（创建）数据库->连接（集合）

    def getRooms():                                
        with open("rooms.csv") as csvFile:        #将csv文件中的数据以字典的形式读出来
            data = []
            reader = csv.DictReader(csvFile)      #使用 Dictreader
            for room in reader:
                data.append(dict(room))
        return data
    data = getRooms()
    for room in data:            #将字段'Price'的值转成int 类型添加到MongoDB中，方便对字段'Price'的值进行排序
        room["Price"] = int(room["Price"])
    # print(data)
    rooms.insert_many(data)                       #将多条数据一次性添加到MongoDB数据库中

方法2：
    from day16_pymongo.def_mongo_class import Connect
    rooms = Connect('localhost','building','rooms').get()
    
    with open("rooms.csv","r") as csvfile:
        reader=csv.reader(csvfile)
        all_data=[]
        for i in reader:
            all_data.append(i)              #获取csv文件数据
    # print(all_data)
    keys=all_data[0]                        #所有keys值
    all_values=all_data[1:]                 #所有values值
    data={}
    datas=[]
    for i in range(len(all_values)):                    #控制行（这里是写入values的行数）
        for j in range(len(keys)):                      #控制列（写入values值对应标题的列）
            data.update({keys[j]: all_values[i][j]})    #字典update()方法
        datas.append(data)
    print(datas)
    for i in range(len(datas)):
        rooms.insert_one(datas[i])
        print("执行完成！")
```

##### 2.2 对Square Footage字段求和-将数据按照Price字段排序

```python
from pymongo import MongoClient

	connect = MongoClient("localhost")
	db = connect.building
	rooms = db.rooms
	result = rooms.find({},{"Square Footage":1,"_id":0})    #不显示id

	total = 0
	for item in list(result):                               #遍历list(result),每个元素都是一个列表（元素）
		total += int(item["Square Footage"])            #转成int类型再求和
	print("Square Footage总和：",total)

        result = rooms.find({},{"_id":0},sort=[("Price",pymongo.DESCENDING)])    #对数据按照字段'Price'排序
        for room in result:
            print(room)
```




