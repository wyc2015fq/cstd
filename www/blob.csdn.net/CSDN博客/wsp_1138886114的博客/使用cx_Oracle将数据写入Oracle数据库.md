# 使用cx_Oracle将数据写入Oracle数据库 - wsp_1138886114的博客 - CSDN博客





2019年03月22日 14:26:33[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：70








### 一、最简单方法（插入单条数据）

```python
#encoding=utf-8
# python2.7

import cx_Oracle
import time

def save_data():
    try:
        db = cx_Oracle.connect('username/password@ip:port/sid')
        cr = db.cursor()
    except:
        print('database connection faile')

# 需要插入的数据
'''
   数据库中的字段类型对应：int,varchar2,timestamp,timestamp,int
'''
    id = ''
    name_id = "5df455ggrg4e5r5sagf5666f"
    Get_time = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
    Start_time = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
    mark = 1

#sql
    Table_sql = "insert into Invoice_Business_Log_Table values ('" + id + "','" + name_id + "'," \
          + "to_timestamp('" + Get_time + "','yyyy-mm-dd hh24:mi:ss')" + "," \
          + "to_timestamp('" + Start_time + "','yyyy-mm-dd hh24:mi:ss')" + "," \
          +  str(mark) + ")"

    cr.execute(Table_sql)
    print('Table is ok')
    cr.close()
    db.commit()
    db.close()

if __name__ == '__main__':
	save_data()
```

### 二、批量插入数据

```python
#encoding=utf-8
# python2.7

import cx_Oracle
import time

def save_data():
    try:
        db = cx_Oracle.connect('username/password@ip:port/sid')
        cr = db.cursor()
    except:
        print('database connection faile')

# 需要插入的数据
    param = [("","5df4f",time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(),str(12)),
             ("","5df4g",time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(),str(13)),
             ("","5df4h",time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(),str(24))]
             
#sql
    Table_sql = "insert into Invoice_Business_Log_Table values(:id,:name_id,:end_time,:mark)"

    cr.executemany(Table_sql,param)
    print('Table is ok')
    cr.close()
    db.commit()
    db.close()

if __name__ == '__main__':
	save_data()
```

### 三、创建类直接使用

```python
#  coding=utf-8
import cx_Oracle
import os
import json
os.environ['NLS_LANG'] = 'SIMPLIFIED CHINESE_CHINA.UTF8'
"""python version 3.7"""

class TestOracle(object):
    def  __init__(self,user,pwd,ip,port,sid):
        self.connect=cx_Oracle.connect(user+"/"+pwd+"@"+ip+":"+port+"/"+sid)
        self.cursor=self.connect.cursor()

    """处理数据二维数组，转换为json数据返回"""
    def select(self,sql):
        list=[]
        self.cursor.execute(sql)
        result=self.cursor.fetchall()
        col_name=self.cursor.description
        for row in result:
            dict={}
            for col in range(len(col_name)):
                key=col_name[col][0]
                value=row[col]
                dict[key]=value
            list.append(dict)
        js=json.dumps(list,ensure_ascii=False,indent=2,separators=(',',':'))
        return js
    def disconnect(self):
        self.cursor.close()
        self.connect.close()
    def insert(self,sql,list_param):
        try:
            self.cursor.executemany(sql,list_param)
            self.connect.commit()
            print("插入ok")
        except Exception as e:
            print(e)
        finally:
            self.disconnect()
    def update(self,sql):
        try:
            self.cursor.execute(sql)
            self.connect.commit()

        except Exception as e:
            print(e)
        finally:
            self.disconnect()
    def delete(self,sql):
        try:
            self.cursor.execute(sql)
            self.connect.commit()
            print("delete ok")
        except Exception as e:
            print(e)
        finally:
            self.disconnect()


if __name__ =="__main__":
    test_oracle=TestOracle('SCOTT','pipeline','127.0.0.1','1521','orcl')
    param=[('ww1','job003',1333,2),('ss1','job004',1444,2)]
    #test_oracle.insert("insert into bonus(ENAME,JOB,SAL,COMM)values(:1,:2,:3,:4)",param)#也可以下面这样解决orc-1036非法变量问题
    test_oracle.insert("insert into bonus(ENAME,JOB,SAL,COMM)values(:ENAME,:JOB,:SAL,:COMM)",param)
    test_oracle1=TestOracle('SCOTT','pipeline','127.0.0.1','1521','orcl')
    test_oracle1.delete("delete from bonus where ENAME='ss1' or ENAME='ww1'")
    test_oracle3=TestOracle('SCOTT','pipeline','127.0.0.1','1521','orcl')
    js=test_oracle3.select('select * from bonus')
    print(js)
```



