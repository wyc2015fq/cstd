
# MySQL数据库6：Go与MySQL事务 - 尹成的技术博客 - CSDN博客

2018年11月16日 09:27:53[尹成](https://me.csdn.net/yincheng01)阅读数：85


**事务概念**
事务是将批量增删改操作视为一个整体，作为一个整体，它们要么全部成功，要么全部失败——这一特性称为事务的原子性（原子是不可再分割的）；
事务的应用场景很多，例如在一次电商的交易中，只有资金出入表、物流表、交易记录表、订单表、购物车表等一系列表的数据变动全部严丝合缝分毫不差时，才能视为交易成功，此时可以提交事务；
只要有一点对不上号的地方，本次交易就不能视为成功，所有的数据应全部回滚至交易之前的状态；
**核心API**
`//连接数据库
db, _ := sqlx.Connect("mysql", "root:123456@tcp(localhost:3306)/mydb")
//开启事务
tx, _ := db.Begin()
//事务提交（所有增删改全部成功时）
tx.Commit()
//事务回滚（出现错误时）
tx.Rollback()`**导入依赖**
`import (
	//执行mysql包的init方法完成数据库引擎的加载
	_ "github.com/go-sql-driver/mysql"
	
	//导入sqlx包
	"github.com/jmoiron/sqlx"
	"fmt"
)`**一次数据操作成功的案例**
本例中，事务操作中的所有增删改全部成功，最终提交事务，所有增删改都被持久化
`func main021() {
	db, _ := sqlx.Connect("mysql", "root:123456@tcp(localhost:3306)/mydb")
	defer db.Close()
	//开启事务
	tx, _ := db.Begin()
	//执行系列增删改方法
	ret1, e1 := tx.Exec("insert into person(name,age,sex) values(?,?,?)", "咸鸭蛋", 20, true)
	ret2, e2 := tx.Exec("delete from person where name not like ?", "%蛋")
	ret3, e3 := tx.Exec("update person set name = ? where name=?", "卤蛋", "双黄蛋")
	//有任何错误都回滚事务，否则提交
	if e1 != nil || e2 != nil || e3 != nil {
		fmt.Println("事务执行失败，e1/e2/e3=", e1, e2, e3)
		//回滚事务
		tx.Rollback()
	} else {
		//提交事务
		tx.Commit()
		ra1, _ := ret1.RowsAffected()
		ra2, _ := ret2.RowsAffected()
		ra3, _ := ret3.RowsAffected()
		fmt.Println("事务执行成功，受影响的行=", ra1+ra2+ra3)
	}
}`**失败回滚的案例**
本例中在修改时由于nickname字段并不存在，因此会返回一个错误对象
整个事务也都因此而失败，事务回滚，所有的修改都不会入库
`func main() {
	db, _ := sqlx.Connect("mysql", "root:123456@tcp(localhost:3306)/mydb")
	defer db.Close()
	//开启事务
	tx, _ := db.Begin()
	//执行系列增删改方法
	ret1, e1 := tx.Exec("insert into person(name,age,sex) values(?,?,?)", "松花蛋", 20, true)
	ret2, e2 := tx.Exec("delete from person where name not like ?", "%蛋")
	ret3, e3 := tx.Exec("update person set nickname = ? where name=?", "茶鸡蛋", "卤蛋")
	//有任何错误都回滚事务，否则提交
	if e1 != nil || e2 != nil || e3 != nil {
		fmt.Println("事务执行失败，e1/e2/e3=", e1, e2, e3)
		//回滚事务
		tx.Rollback()
	} else {
		//提交事务
		tx.Commit()
		ra1, _ := ret1.RowsAffected()
		ra2, _ := ret2.RowsAffected()
		ra3, _ := ret3.RowsAffected()
		fmt.Println("事务执行成功，受影响的行=", ra1+ra2+ra3)
	}
}`学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116092700978.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

