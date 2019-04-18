# pyMySQL操作MySQL数据库 - wsp_1138886114的博客 - CSDN博客





2018年05月20日 16:33:16[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：338








### 一、利用pyMySQL模块操作MySQL数据库

（利用MySQL connector，MySQLdb模块，torndb这三种模块不用了）
- 安装pymysql
	
```python
pip install pymysql
```

- 导入模块
	
```python
import pymysql
```

- 链接数据库
	
```python
connect = pymysql.connect(host="localhost",user="root",password="password",db="db_name",charset="utf8")
```

- 创建游标
	
```python
cursor = connect.cursor()
```

- 执行SQL语句
	
```python
sql = "insert into tableName values('%s','%d')"
cursor.excute(sql(value1,value2))
```

- 获取数据
	
```python
data1 = cursor.fetchone()         #获取一条数据
data2 = cursor.fetchmany()        #获取多条数据
data3 = cursor.fetchall()         #获取所有数据
fields = cursor.description       #获取字段信息
```

- 提交并关闭
	
```python
cursor.close()            #关闭游标
connect.commit()          #提交数据更改,涉及数据的更改时需要提交
connect.close()           #关闭数据库连接
```


### 二、pyMySQL—数据插入—修改—查询—删除

创建数据库

```python
create table if not exists `trade`(
	`id` 	  int unsigned not null auto_increment,
	`name`    varchar(6)   not null comment'用户真实姓名',
	`account` varchar(11)  not null comment'银行储蓄账号',
	`saving`  decimal(8,2) unsigned not null default '0.00' comment'银行储蓄金额',
	`expend`  decimal(8,2) unsigned not null default '0.00' comment'账户支出总计',
	`income`  decimal(8,2) unsigned not null default '0.00' comment'账户收入总计',
	primary key(`id`),
	unique key `name_unique`(`name`)
	)
	engine = innoDB auto_increment = 2 default charset = utf8;             #设置数据库类型
        insert into `trade` values(1,'乔布斯','18054652315',0.00,0.00,0.00);    #插入数据
```

连接数据库并插入数据

```python
import pymysql
	connect = pymysql.connect(host='127.0.0.1',user='root',password='123456',db = 'testdb1',charset = 'utf8') #连接数据库
	cursor = connect.cursor();			#获取游标
	sql = "insert into trade (id,name,account,saving)values('%d','%s','%s',%.2f)"
	data = (2'雷军','13545684578',1000000)	        #插入数据(元组)
	cursor.execute(sql % data)
	connect.commit()                                #提交事务
	print ('成功插入',cursor.rowcount,'条数据')
```

修改数据

```python
sql = "updata trade set saving = %.2f where account = '%s'"
	data = (88888,'13545684578')
	cursor.execute(sql % data )
	connect.commit()
	print('成功修改',cursor.rowcount,'条数据')
```

查询数据

```python
sql = "select * from trade where account = '13545684578'"    #查询account=13545684578这一行数据
	cursor.execute(sql)
	row = cursor.fetchone()
	print (row)

        for row in cursor.fetchall():    #第一种查询数据方法
            print(row)
            print("name:%s\t saving:%.2f % row")
        print("共查找出",cursor.rowcount,"条数据")
    
        rows = cursor.fetchall()        #第二种查询数据方法
        print(json.dumps(row,ensure_ascii = false))
        for row in rows:
        print (json.dumps )
        connect.commit()
        print('成功修改',cursor.rowcount,'条数据')

# python + mysql 实现分页查询
```

```python
import pymysql
    connect = pymysql.connect(host="localhost",user="root",password="password",db="db_name",charset="utf8")  #获取链接
    cursor = connect.cursor()	            #获取游标对象
    currentPage = 1                         #定义变量,当前的页码
    pageNum = 10                            #定义变量,每页显示的记录数
    for page in range(1,21):
        sql = "select * from tb_name limit %d,%d"  #组装sql语句
        startIndex = (page-1)*pageNum               #每页记录数*(第几页-1)
        cursor.execute(sql%(startIndex,pageNum))    #执行sql语句
        print("*************第{}页************".format(page))
        for row in cursor.fetchall():
        print(row)
```

删除数据

```python
sql = "delete from trade WHERE account = '%s' limit %d"
	data = ('13545684578',1)
	cursor.execute(sql % data)
	connect.commit()
	print('成功删除{}条数据'.format(cursor.rowcount))
```

事务处理

```python
sq_l= "update trade set saving = saving + 1000 where account'13545684578'"
	sq_2= "update trade set extend = extend + 1000 where account'13545684578'"
	sq_3= "update trade set income = income + 1000 where account'13545684578'"
        
        try:
            cursor.excute(sql_1) #储蓄增加1000
            cursor.excute(sql_2) #储蓄支出1000
            cursor.excute(sql_3) #储蓄收入2000    
        except exception as e:
            connect.rollback()    #事务回滚
            print('事务处理失败，事务回滚！失败原因：',e)
        else:
            connect.commit()      #事务提交
            print ("事务处理成功", cursor.rowcount)    #这是每一个代码片段都必须有的代码
        cursor.close()
        connect.close()
```













