# pyMySQL+Excel+爬虫——练习题 - wsp_1138886114的博客 - CSDN博客





2018年05月24日 11:47:50[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：142








### 一、获取MySQL数据库数据写入excel

```python
import pymysql                #导入pymysql
import xlwt                   #导入xlwt

#一、获取链接对象
connect = pymysql.connect(host="localhost",user="root",password="password",db="db_name",charset="utf8")
cursor = connect.cursor()	         #获取游标
sql = "SELECT b.sname,c.cname,a.score \
        FROM score a INNER JOIN students b ON a.sno=b.sno \
        INNER JOIN course c ON a.cno=c.course"       #三表联查

cursor.execute(sql)	                #执行sql语句
all = cursor.fetchall()                 #查询所有

wbk = xlwt.Workbook()			#创建工作簿
sheet = wbk.add_sheet("学生成绩表")	#向工作簿添加工作表命名为：学生成绩表
fields = ["姓名","课程","成绩"]		#写入表头数据
for i in range(len(fields)):
    sheet.write(0,i,fields[i])          #第0行，第i列（表头）
#print(all)
for rows in range(len(all)):
    for cols in  range(len(all[rows])):
        sheet.write(rows+1,cols,all[rows][cols])    #第1行，第col列写起（第0行是表头）
wbk.save("./studentResult.xls")        #保存数据
```

```python
print('---------------获取MySQL数据库的数据写入Excel-----------------------')
import pymysql
import xlwt
connect= pymysql.connect(host='localhost',user='root',password='password',\
						 db='database_name',charset='utf8')
cursor = connect.cursor()       		#获取游标
sql2="select *from table_name"        
cursor.execute(sql2)            		#执行sql语句

fields=cursor.description	    		#获取表头数据
list1=[]
for field in fields:
    list1.append(field[0])      		#获取表头数据存入list1

info_data=cursor.fetchall()     		#获取所有数据
workbook = xlwt.Workbook()              #创建Excel表
sheet = workbook.add_sheet('表01')      #为Excel添加表单"表01"

for i in range(len(list1)):             #写入表头
    sheet.write(0,i, list1[i])

for i in range(1,len(info_data)+1):     #写入数据：从第二行开始写
    for j in range(len(list1)):
        sheet.write(i,j,info_data[i-1][j])

workbook.save('movieRank.xls')
```

### 二、查询数据

```python
print('----------------从数据库中分页查询数据------------------')
import pymysql
connect = pymysql.connect(host="localhost",user="root",\
password="password",db="table_name",charset="utf8")

cursor = connect.cursor()		        #获取游标
currentPage = 1					#当前的页码
pageNum = 10					#每页显示的记录数

for page in range(1,21):				#查询1-20页的数据
    sql = "select * from jobs limit %d,%d"		#组装sql语句
    startIndex = (page-1)*pageNum			#每页数据条数*(当前页-1)
    cursor.execute(sql%(startIndex,pageNum))
    print("************第{}页**********".format(page))
    for row in cursor.fetchall():
        print(row)
```

### 三、使用封装函数执行MySQL操作

```python
import pymysql
class Connect(object):			#创建Connect类，封装pymysql	
    def __init__(self,host,user,password,db,charset):
        self.host = host
        self.user = user
        self.password = password
        self.db = db
        self.charset = charset
    def getconnect(self):
        connect = pymysql.connect(host = self.host, user = self.user,\
                  password = self.password,db = self.db,charset = self.charset)
        return connect
if __name__ == '__main__':
    print('这是这是测试代码')

print('-----------使用封装函数--------插入mysql数据-------------------')
#from day15_pymsql.def_mysql_01 import Connect
def insert(data):
    con1 = Connect('localhost','root','password','database_name','utf8')  #实例化
    conn = con1.getconnect()    		# 获取连接
    cursor = conn.cursor()      		# 创建游标
    sql = "insert into table_name values('%s','%d','%f','%d','%f')"
    cursor.execute(sql % data)
    conn.commit()               		# 提交
    print('成功插入', cursor.rowcount, '条数据')
data = ('犬之岛',617.35,0.0908,2,139),('湮灭',135.34,0.0199,9,557)  # 插入的数据
insert(data)

print('-----------使用封装函数-------修改mysql数据字段类型/属性---------')
#from day15_pymsql.def_mysql_01 import Connect
def modityData():
    con2 = Connect('localhost','root','password','database_name','utf8')  #实例化
    conn = con2.getconnect()    		# 获取连接
    cursor = conn.cursor()  			# 创建游标

    sql2 = "update commoditytype set stu_name='%s' where ct_id='%d'"
    cursor.execute(sql2%data) 			#执行
    conn.commit()
    print('成功修改', cursor.rowcount, '条数据')
data = ('科技产品',0)
modityData()
	
print('---------使用封装函数---------查找mysql数据-------------------')
#from day15_pymsql.def_mysql_01 import Connect
def selectAll():
    con3 = Connect('localhost','root', 'password','database_name','utf8')  # 实例化
    conn = con3.getconnect()  			# 获取连接
    cursor = conn.cursor()  			# 创建游标
    sql3 = "select * from table_name"
    cursor.execute(sq3) 			#执行
    allProduct = cursor.fetchall()		#查询所有数据
    for row in allProduct:
        print(row)
selectAll()
```






