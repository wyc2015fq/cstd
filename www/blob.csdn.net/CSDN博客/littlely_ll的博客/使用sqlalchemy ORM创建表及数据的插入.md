# 使用sqlalchemy ORM创建表及数据的插入 - littlely_ll的博客 - CSDN博客





2018年09月14日 17:58:38[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：860








## 1.载入库

```python
import datetime
import pandas as pd
import sqlalchemy
from sqlalchemy import create_engine
from sqlalchemy.sql import text, func
from sqlalchemy.orm import sessionmaker,relationship
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import Column, String, Integer, Float, TIMESTAMP
```

## 2.创建数据库引擎

```php
#create_engine(‘数据库类型+数据库驱动名称://用户名:口令@地址:端口号/数据库名)
engine = create_engine(“"oracle or mysql", echo=False)
conn = engine.connect()
```

## 3.使用原生sql语句

```
#使用连接后的引擎抽取数据
df_list = conn.execute("select * from dual")
#获取一条数据
df_list.fetchone()
#获取所有数据，是一个元组列表
df_list.fetchall()
#转化为dataframe
df = pd.DataFrame(df_list)
#列名默认是序列数字，添加列名
df.columns = ['your col_name',...]

#使用pandas直接获取数据
df1 = pd.read_sql("select * from dual", con=engine)
```

## 4. 使用对象关系映射(ORM)

### 4.1 声明映射

```python
Base = declarative_base()
DBSession = sessionmaker(bind=engine)
session = DBSession()

#Student类相当于在sql中创建的一张表
class Student(Base):
    __tablename__ = "student"
    id = Column(Integer, primary_key=True)
    name = Column(String(20))
    sex = Column(String(2))
    clas = Column(String(20))
    grade = Column(Float(32))

    def __repr__(self):
        return "<Student(id='%s', name='%s', sex='%s',clas='%s',grade='%s')>" % (self.id, self.name, self.sex, self.clas, self.grade)
```

**在使用 declarative创建Student类中，必须要有一个`__tablename__`属性和至少一个具有主键的字段，否则会出现错误。**

### 4.2 创建表

```
#Base.metadata.create_all会找到BaseModel的所有子类，在数据库中建立这些表，相当于'create table'
Base.metadata.create_all(engine) #类似的也有Base.metadata.drop_all(engine)
```

### 4.3 插入数据

```
#插入单条数据
new_student = Student(name='lily',sex='F',clas=2, grade=97)
#只添加，还没有提交，如果出错还可以撤回(rollback)
session.add(new_student)
#提交到数据库
session.commit()
#插入数据的回撤
new_student2 = Student(name='lily',sex='F',clas=2, grade=97)
session.add(new_student2)
session.rollback()

#插入批量数据，以pandas的dataframe为例
df = pd.DataFrame({"name":['Mike','John','Mary'],"sex":['M','M','F'],'clas':[4,7,1],'grade':[78,96,85]})
#第一种插入方法(pandas to_sql)
#在使用to_sql时注意if_exists参数，如果是replace的话它会先drop掉表，然后再创建表，最后插入数据
df.to_sql('student',con=engine,if_exists='append',index=False)
```

其实pandas的to_sql还是挺快的，但是在有主键的时候可能插入速度非常慢(使用oracle测试100万数据用几个小时)

```
#第二中方法(原生方法)：
a = []
for i in df.iterrows():
    a.append(dict(i[1]))
engine.execute(Student.__table__.insert(),a)
#还有其他方法
```

### 4.4 创建关联表

```python
class Book(Base):
    __tablename__ = 'book'
    bid = Column(Integer, primary_key=True)
    bname = Column(String(20))
    price = Column(Float(32))
    #建立外键student_id
    student_id = Column(Integer, ForeignKey(Student.id))
    #告诉ORM, Book类要关联Student类
    student = relationship(Student)
    insert_time = Column(TIMESTAMP(timezone=False), nullable=False, server_default=text("NOW()"))

    def __repr__(self):
        return "<Book(bid='%s', bname='%s', price='%s',student_id='%s')>" % (
        self.bid, self.bname, self.price, self.student_id)

Base.metadata.create_all(engine)
books = pd.DataFrame({"bname":['gone with wind','good by','game of throne','king of ring'],"price":[128,22,67,190],'student_id':[1,1,3,2]})
books.to_sql('book',engine,if_exists='append',index=False)
```

在这一块设置`insert_time`字段时需要非常注意：

> - 如果是`insert_time = Column(TIMESTAMP(timezone=False),default=func.now()`这样设置的话是不起作用的，根据mysql实验，这样设置插入时间的默认值仍然为`NULL`。
- 如果是`insert_time = Column(TIMESTAMP(timezone=False),nullable=False`这样设置的话，MySQL 会在extra中自动加入`on update CURRENT_TIMESTAMP` ，每次更新数据的时候后面的时间也跟着更新，这样设置的话可以作为数据的更新时间。
- 如果是`insert_time = Column(TIMESTAMP(timezone=False),nullable=False，server_default=text("NOW()")`或`insert_time = Column(TIMESTAMP(timezone=False),nullable=False，server_default=text("CURRENT_TIMESTAMP")`或`insert_time = Column(TIMESTAMP(timezone=False),server_default=func.now()`，则本字段就不会自动更新时间，这样设置的话可以作为数据的插入时间。
- 
如果既要插入更新时间字段也要插入插入时间字段，则更新时间字段必须在插入时间字段之前，也就是先`update_time =Column(TIMESTAMP(timezone=False), nullable=False)`,然后
```
insert_time = Column(TIMESTAMP(timezone=False), nullable=False, server_default=text("NOW()"))
```
,反过来则会出错。比较下两图： 

  更新时间在前： 
![更新在前](https://img-blog.csdn.net/20180914175443936?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdHRsZWx5X2xs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

  更新时间在后： 
![这里写图片描述](https://img-blog.csdn.net/20180914175453853?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdHRsZWx5X2xs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- 
插入时间戳（TIMESTAMP）的时候注意时区的问题，虽然默认的时区为`False`,但如果不使用时区的话需要明确设置为`False`，在oracle测试中，如果不加入参数`timezone=False`的话它仍会加入时区设置。


## 5 默认值的设置

如果要在mysql中设置默认值，在sqlalchemy编写类的字段时使用`default`参数是行不通的，例如：

```
class Book(Base):
    __tablename__ = 'book'
    bid = Column(Integer, primary_key=True)
    bname = Column(String(20))
    price = Column(Float(32))
    #建立外键student_id
    student_id = Column(Integer, ForeignKey(Student.id))
    #告诉ORM, Book类要关联Student类
    student = relationship(Student)
    insert_time = Column(TIMESTAMP(timezone=False), nullable=False, server_default=text("NOW()"))
    flag = Column(String(1),default=text('0'))
```

相比上一个`Book`类，这个类多加了一个`flag`字段，但使用`default`是插入不了mysql中的默认值`0`的，如下图显示： 
![这里写图片描述](https://img-blog.csdn.net/20180915110528242?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdHRsZWx5X2xs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
但是如果把`flag`字段的参数`default`改为`server_default`就会设置默认值了：

```
class Book(Base):
    __tablename__ = 'book'
    bid = Column(Integer, primary_key=True)
    bname = Column(String(20))
    price = Column(Float(32))
    #建立外键student_id
    student_id = Column(Integer, ForeignKey(Student.id))
    #告诉ORM, Book类要关联Student类
    student = relationship(Student)
    insert_time = Column(TIMESTAMP(timezone=False), nullable=False, server_default=text("NOW()"))
    flag = Column(String(1),server_default=text('0'))
```

mysql的book数据结构显示为： 
![这里写图片描述](https://img-blog.csdn.net/20180915111003531?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdHRsZWx5X2xs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

所以在设置默认值的时候要使用`server_default`参数来进行设置。
**参考**
[SQLAlchemy and MySQL TIMESTAMP](https://blog.zengrong.net/post/2628.html)
[SQLAlchemy default DateTime](https://stackoverflow.com/questions/13370317/sqlalchemy-default-datetime)
[SQLAlchemy/Pandas: Can not insert default time in MySQL](https://stackoverflow.com/questions/52327311/sqlalchemy-pandas-can-not-insert-default-time-in-mysql/52330323#52330323)








