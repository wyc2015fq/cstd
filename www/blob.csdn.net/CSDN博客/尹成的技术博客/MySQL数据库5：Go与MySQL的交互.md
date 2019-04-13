
# MySQL数据库5：Go与MySQL的交互 - 尹成的技术博客 - CSDN博客

2018年11月16日 09:46:52[尹成](https://me.csdn.net/yincheng01)阅读数：24个人分类：[GO语言](https://blog.csdn.net/yincheng01/article/category/7679307)[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)[](https://blog.csdn.net/yincheng01/article/category/7679307)



**下载第三方依赖**
`go get github.com/jmoiron/sqlx
go get github.com/go-sql-driver/mysql`[其中github.com/jmoiron/sqlx是我们需要用到的主要类库](http://xn--github-9v7ih8t.com/jmoiron/sqlx%E6%98%AF%E6%88%91%E4%BB%AC%E9%9C%80%E8%A6%81%E7%94%A8%E5%88%B0%E7%9A%84%E4%B8%BB%E8%A6%81%E7%B1%BB%E5%BA%93)
[github.com/go-sql-driver/mysql是作为MySQL的驱动程序存在的，我们只需要执行包的init方法即可](http://github.com/go-sql-driver/mysql%E6%98%AF%E4%BD%9C%E4%B8%BAMySQL%E7%9A%84%E9%A9%B1%E5%8A%A8%E7%A8%8B%E5%BA%8F%E5%AD%98%E5%9C%A8%E7%9A%84%EF%BC%8C%E6%88%91%E4%BB%AC%E5%8F%AA%E9%9C%80%E8%A6%81%E6%89%A7%E8%A1%8C%E5%8C%85%E7%9A%84init%E6%96%B9%E6%B3%95%E5%8D%B3%E5%8F%AF)
**引入依赖**
`import (
	"github.com/jmoiron/sqlx"
	//执行mysql包的init方法
	_"github.com/go-sql-driver/mysql"
	"fmt"
)`**建立测试数据库表**
`create database mydb charset=utf8;
use mydb;
create table person(
  id int primary key auto_increment,
  name varchar(20) unique not null,
  age int default 0,
  dollar float default 1
);
alter table person add sex bool;
alter table person add schoolday date;
alter table person add birthtime datetime;`**定义与数据表向匹配的结构体**
`/*
id | name   | poem | age  | dollar  | sex  | schoolday  | birthtime
*/
type Person struct {
	//所有属性必须公开——框架会转化查询结果为结构体对象
	//标签名写法：`db:"表字段名"`
	Name string `db:"name"`
	Age int `db:"age"`
	Dollar float32 `db:"dollar"`
}`**执行增删改查操作**
增删改的方式是db.Exec(sql)
查询的方式是：db.select(&model,sql)
`func main() {
	//连接数据库
	db, _ := sqlx.Open("mysql", "root:123456@tcp(127.0.0.1:3306)/mydb")
	defer db.Close()
	//执行增删改，获得受影响的行数
	result, _ := db.Exec("insert into person(name,age,sex,schoolday,birthtime) values(?,?,?,?,?);", "双黄蛋", 35, false, 20080813, 20180814164300)
	rowsAffected, _:= result.RowsAffected()
	lastInsertId, _ := result.LastInsertId()
	fmt.Println("受影响的行数",rowsAffected,"最后一条记录的id",lastInsertId)
	//执行增删改
	db.Exec("insert into person(name,age,sex,schoolday,birthtime) values(?,?,?,?,?);", "双黄蛋", 35, false, 20080813, 20180814164300)
	db.Exec("delete from person where id=?;", 2)
	db.Exec("update person set name=? where name=?;", "张俩蛋","张全蛋")
	//执行查询，获得person对象
	//用于接收的数据类型是【结构体切片】，必须包含查询字段对应的公开属性，属性标签应写作：`db:"表字段名"`
	var ps []Person
	//第一个参数是【结构体切片】的地址
	err := db.Select(&ps, "select name,age,dollar from person where name like ?;", "%蛋")
	if err!=nil{
		fmt.Println("err=",err)
	}
	fmt.Printf("%T,%v\n", ps, ps)
	fmt.Println("执行成功！")
}`学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116092700978.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

