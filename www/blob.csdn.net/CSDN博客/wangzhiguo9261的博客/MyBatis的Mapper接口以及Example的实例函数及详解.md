
# MyBatis的Mapper接口以及Example的实例函数及详解 - wangzhiguo9261的博客 - CSDN博客


2018年06月27日 14:12:25[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：23个人分类：[mybatis](https://blog.csdn.net/wangzhiguo9261/article/category/7755745)



### 一、mapper接口中的方法解析
mapper接口中的函数及方法
|方法|功能说明|
|---|---|
|int countByExample(UserExample example) thorws SQLException|按条件计数|
|int deleteByPrimaryKey(Integer id) thorws SQLException|按主键删除|
|int deleteByExample(UserExample example) thorws SQLException|按条件删除|
|String/Integer insert(User record) thorws SQLException|插入数据（返回值为ID）|
|User selectByPrimaryKey(Integer id) thorws SQLException|按主键查询|
|ListselectByExample(UserExample example) thorws SQLException|按条件查询|
|ListselectByExampleWithBLOGs(UserExample example) thorws SQLException|按条件查询（包括BLOB字段）。只有当数据表中的字段类型有为二进制的才会产生。|
|int updateByPrimaryKey(User record) thorws SQLException|按主键更新|
|int updateByPrimaryKeySelective(User record) thorws SQLException|按主键更新值不为null的字段|
|int updateByExample(User record, UserExample example) thorws SQLException|按条件更新|
|int updateByExampleSelective(User record, UserExample example) thorws  SQLException|按条件更新值不为null的字段|
### 二、example实例解析
mybatis的逆向工程中会生成实例及实例对应的example，example用于添加条件，相当where后面的部分
xxxExample example = new xxxExample();
Criteria criteria = new Example().createCriteria();
|方法|说明|
|---|---|
|example.setOrderByClause(“字段名 ASC”);|添加升序排列条件，DESC为降序|
|example.setDistinct(false)|去除重复，boolean型，true为选择不重复的记录。|
|criteria.andXxxIsNull|添加字段xxx为null的条件|
|criteria.andXxxIsNotNull|添加字段xxx不为null的条件|
|criteria.andXxxEqualTo(value)|添加xxx字段等于value条件|
|criteria.andXxxNotEqualTo(value)|添加xxx字段不等于value条件|
|criteria.andXxxGreaterThan(value)|添加xxx字段大于value条件|
|criteria.andXxxGreaterThanOrEqualTo(value)|添加xxx字段大于等于value条件|
|criteria.andXxxLessThan(value)|添加xxx字段小于value条件|
|criteria.andXxxLessThanOrEqualTo(value)|添加xxx字段小于等于value条件|
|criteria.andXxxIn(List<？>)|添加xxx字段值在List<？>条件|
|criteria.andXxxNotIn(List<？>)|添加xxx字段值不在List<？>条件|
|criteria.andXxxLike(“%”+value+”%”)|添加xxx字段值为value的模糊查询条件|
|criteria.andXxxNotLike(“%”+value+”%”)|添加xxx字段值不为value的模糊查询条件|
|criteria.andXxxBetween(value1,value2)|添加xxx字段值在value1和value2之间条件|
|criteria.andXxxNotBetween(value1,value2)|添加xxx字段值不在value1和value2之间条件|
### 三、应用举例
#### 1.查询
① selectByPrimaryKey()
```python
User user = XxxMapper.selectByPrimaryKey(100); //相当于
```
```python
select
```
```python
*
```
```python
from
```
```python
user
```
```python
where
```
```python
id =
```
```python
100
```
1
② selectByExample() 和 selectByExampleWithBLOGs()
```python
UserExample example =
```
```python
new
```
```python
UserExample();
Criteria criteria = example.createCriteria();
criteria.andUsernameEqualTo(
```
```python
"wyw"
```
```python
);
criteria.andUsernameIsNull();
example.setOrderByClause(
```
```python
"username asc,email desc"
```
```python
);
List<?>list = XxxMapper.selectByExample(example);
//相当于：
```
```python
select
```
```python
* from user where username =
```
```python
'wyw
```
```python
'
```
```python
and
```
```python
username
```
```python
is
```
```python
null
```
```python
order by username asc,email desc
```
1
2
3
4
5
6
7
注：在iBator逆向工程生成的文件XxxExample.java中包含一个static的内部类Criteria，Criteria中的方法是定义SQL 语句where后的查询条件。
#### 2.插入数据
①insert()
```python
User user = new User();
user.setId("dsfgsdfgdsfgds");
user.setUsername("admin");
user.setPassword("admin")
user.setEmail("wyw@163.com");
XxxMapper.
```
```python
insert
```
```python
(
```
```python
user
```
```python
);
```
```python
//相当于：
```
```python
insert
```
```python
into
```
```python
user
```
```python
(ID,username,password,email)
```
```python
values
```
```python
(
```
```python
'dsfgsdfgdsfgds'
```
```python
,
```
```python
'admin'
```
```python
,
```
```python
'admin'
```
```python
,
```
```python
'wyw@126.com'
```
```python
);
```
1
2
3
4
5
6
7
#### 3.更新数据
①updateByPrimaryKey()
```python
User user =new User();
user.setId(
```
```python
"dsfgsdfgdsfgds"
```
```python
);
user.setUsername(
```
```python
"wyw"
```
```python
);
user.setPassword(
```
```python
"wyw"
```
```python
);
user.setEmail(
```
```python
"wyw@163.com"
```
```python
);
XxxMapper.updateByPrimaryKey(user);
//相当于：update user
```
```python
set
```
```python
username=
```
```python
'wyw'
```
```python
, password=
```
```python
'wyw'
```
```python
, email=
```
```python
'wyw@163.com'
```
```python
where id=
```
```python
'dsfgsdfgdsfgds'
```
1
2
3
4
5
6
7
②updateByPrimaryKeySelective()
```python
User user = new User();
user.setId("dsfgsdfgdsfgds");
user.setPassword("wyw");
XxxMapper.updateByPrimaryKey(user);
//相当于：
```
```python
update
```
```python
user
```
```python
set
```
```python
password=
```
```python
'wyw'
```
```python
where
```
```python
id=
```
```python
'dsfgsdfgdsfgds'
```
1
2
3
4
5
③ updateByExample() 和 updateByExampleSelective()
```python
UserExample example = new UserExample();
Criteria criteria = example.createCriteria();
criteria.andUsernameEqualTo("admin");
User user = new User();
user.setPassword("wyw");
XxxMapper.updateByPrimaryKeySelective(user,example);
//相当于：
```
```python
update
```
```python
user
```
```python
set
```
```python
password=
```
```python
'wyw'
```
```python
where
```
```python
username=
```
```python
'admin'
```
1
2
3
4
5
6
7
updateByExample()更新所有的字段，包括字段为null的也更新，建议使用 updateByExampleSelective()更新想更新的字段
#### 4.删除数据
①deleteByPrimaryKey()
```python
XxxMapper.deleteByPrimaryKey(1);  //相当于：
```
```python
delete
```
```python
from
```
```python
user
```
```python
where
```
```python
id=
```
```python
1
```
1
②deleteByExample()
```python
UserExample example = new UserExample();
Criteria criteria = example.createCriteria();
criteria.andUsernameEqualTo("admin");
XxxMapper.deleteByExample(example);
//相当于：
```
```python
delete
```
```python
from
```
```python
user
```
```python
where
```
```python
username=
```
```python
'admin'
```
1
2
3
4
5
#### 5.查询数据数量
①countByExample()
```python
UserExample example = new UserExample();
Criteria criteria = example.createCriteria();
criteria.andUsernameEqualTo("wyw");
int count = XxxMapper.countByExample(example);
//相当于：
```
```python
select
```
```python
count
```
```python
(*)
```
```python
from
```
```python
user
```
```python
where
```
```python
username=
```
```python
'wyw'
```

