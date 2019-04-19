# mgo使用指南 - 三少GG - CSDN博客
2015年12月30日 11:37:35[三少GG](https://me.csdn.net/scut1135)阅读数：3181
[https://godoc.org/labix.org/v2/mgo](https://godoc.org/labix.org/v2/mgo)
# mgo使用指南[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#mog)
## mgo简介[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#mgo)
mgo（音mango）是[MongoDB](http://www.mongodb.org/)的[Go语言](http://golang.org/)驱动，它用基于Go语法的简单API实现了丰富的特性，并经过良好测试。
官方网站：[http://labix.org/mgo](http://labix.org/mgo)。
[golang.tc](http://www.golang.tc/)（[golangtc.com](http://www.golangtc.com/)）网站的数据存储就是是用的MongoDB+mgo。近一年使用下来表现良好。
## API文档[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#api)
下面是mgo、mgo/bson、mgo/txn的在线文档。
- mgo [GoDoc](http://godoc.org/labix.org/v2/mgo)[GoWalker](http://gowalker.org/labix.org/v2/mgo)
- mgo/bson [GoDoc](http://godoc.org/labix.org/v2/mgo/bson)[GoWalker](http://gowalker.org/labix.org/v2/mgo/bson)
- mgo/txn [GoDoc](http://godoc.org/labix.org/v2/mgo/txn)[GoWalker](http://gowalker.org/labix.org/v2/mgo/txn)
## 安装[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#id1)
安装mgo之前请先安装Golang和MongoDB，安装过程不再赘述。
安装[bzr](http://bazaar.canonical.com/en/)版本工具（mgo使用Bazaar作为版本控制系统，因安装的时候需要去Bazaar拉取代码）。
安装命令
go get labix.org/v2/mgo
## 示例[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#id1)
下面的代码是个示例。
package main
import("fmt""labix.org/v2/mgo""labix.org/v2/mgo/bson")
type Personstruct{NamestringPhonestring}
func main(){
    session, err := mgo.Dial("")if err !=nil{
        panic(err)}
    defer session.Close()// Optional. Switch the session to a monotonic behavior.
    session.SetMode(mgo.Monotonic,true)
    c := session.DB("test").C("people")
    err = c.Insert(&Person{"Ale","+55 53 8116 9639"},&Person{"Cla","+55 53 8402 8510"})if err !=nil{
        panic(err)}
    result :=Person{}
    err = c.Find(bson.M{"name":"Ale"}).One(&result)if err !=nil{
        panic(err)}
    fmt.Println("Phone:", result.Phone)}
启动MongoDB，把上面代码复制了跑一下，如果输出下面内容，说明安装成功。
Phone:+555381169639
具体代码什么意思先不用管，后面讲解每个方法的用法。
## 如何使用[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#id1)
下面介绍如何使用mgo，主要介绍集合的操作。对数据库，用户等操作，请自行查看文档。
第一步当然是先导入mgo包
import("labix.org/v2/mgo""labix.org/v2/mgo/bson")
### 连接服务器[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#id2)
通过方法Dial()来和MongoDB服务器建立连接。Dial()定义如下：
func Dial(url string)(*Session, error)
具体使用：
session, err := mgo.Dial(url)
如果是本机，并且MongoDB是默认端口27017启动的话，下面几种方式都可以。
session, err := mgo.Dial("")
session, err := mgo.Dial("localhost")
session, err := mgo.Dial("127.0.0.1")
session, err := mgo.Dial("localhost:27017")
session, err := mgo.Dial("127.0.0.1:27017")
如果不在本机或端口不同，传入相应的地址即可。如：
mongodb://myuser:mypass@localhost:40001,otherhost:40001/mydb
### 切换数据库[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#id3)
通过Session.DB()来切换相应的数据库。
func (s *Session) DB(name string)*Database
如切换到test数据库。
db := session.DB("test")
### 切换集合[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#id4)
通过Database.C()方法切换集合（Collection），这样我们就可以通过对集合进行增删查改操作了。
func (db *Database) C(name string)*Collection
如切换到`users`集合。
c := db.C("users")
### 对集合进行操作[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#id5)
介绍插入、查询、修改、删除操作。
先提一下ObjectId，MongoDB每个集合都会一个名为_id的主键，这是一个24位的16进制字符串。对应到mgo中就是bson.ObjectId。
这里我们定义一个struct，用来和集合对应。
type Userstruct{Id_       bson.ObjectId`bson:"_id"`Namestring`bson:"name"`Ageint`bson:"age"`JoinedAt   time.Time`bson:"joined_at"`Interests[]string`bson:"interests"`}
注解
注意User的字段首字母大写，不然不可见。通过bson:”name”这种方式可以定义MongoDB中集合的字段名，如果不定义，mgo自动把struct的字段名首字母小写作为集合的字段名。如果不需要获得id_，Id_可以不定义，在插入的时候会自动生成。
#### 插入[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#id6)
插入方法定义如下：
func (c *Collection)Insert(docs ...interface{}) error
下面代码插入两条集合数据。
err = c.Insert(&User{Id_:       bson.NewObjectId(),Name:"Jimmy Kuu",Age:33,JoinedAt:  time.Now(),Interests:[]string{"Develop","Movie"},})if err !=nil{
    panic(err)}
err = c.Insert(&User{Id_:       bson.NewObjectId(),Name:"Tracy Yu",Age:31,JoinedAt:  time.Now(),Interests:[]string{"Shoping","TV"},})if err !=nil{
    panic(err)}
这里通过bson.NewObjectId()来创建新的ObjectId，如果创建完需要用到的话，放在一个变量中即可，一般在Web开发中可以作为参数跳转到其他页面。
通过MongoDB客户端可以发现，两条即可已经插入。
{"_id":ObjectId("5204af979955496907000001"),"name":"Jimmy Kuu","age":,"joned_at":Date(),"interests":["Develop","Movie"]}{"_id":ObjectId("5204af979955496907000002"),"name":"Tracy Yu","age":,"joned_at":Date(),"interests":["Shoping","TV"]}
#### 查询[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#id7)
通过func (c *Collection) Find(query interface{}) *Query来进行查询，返回的Query
 struct可以有附加各种条件来进行过滤。
通过Query.All()可以获得所有结果，通过Query.One()可以获得一个结果，注意如果没有数据或者数量超过一个，One()会报错。
条件用bson.M{key: value}，注意key必须用MongoDB中的字段名，而不是struct的字段名。
##### 无条件查询[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#id8)
**查询所有**
var users []User
c.Find(nil).All(&users)
fmt.Println(users)
上面代码可以把所有Users都查出来：
[{ObjectIdHex("5204af979955496907000001")JimmyKuu332013-08-0917:00:07.95+0800 CST [DevelopMovie]}{ObjectIdHex("5204af979955496907000002")TracyYu312013-08-0917:00:07.971+0800 CST [Shoping TV]}]
##### 根据ObjectId查询[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#objectid)
id :="5204af979955496907000001"
objectId := bson.ObjectIdHex(id)
user :=new(User)
c.Find(bson.M{"_id": objectId}).One(&user)
fmt.Println(user)
结果如下：
&{ObjectIdHex("5204af979955496907000001")JimmyKuu332013-08-0917:00:07.95+0800 CST [DevelopMovie]}
更简单的方式是直接用FindId()方法：
c.FindId(objectId).One(&user)
注解
注意这里没有处理err。当找不到的时候用One()方法会出错。
###### 单条件查询[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#id9)
=($eq)[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#eq)
c.Find(bson.M{"name":"Jimmy Kuu"}).All(&users)
!=($ne)[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#ne)
c.Find(bson.M{"name": bson.M{"$ne":"Jimmy Kuu"}}).All(&users)
>($gt)[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#gt)
c.Find(bson.M{"age": bson.M{"$gt":32}}).All(&users)
<($lt)[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#lt)
c.Find(bson.M{"age": bson.M{"$lt":32}}).All(&users)
>=($gte)[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#gte)
c.Find(bson.M{"age": bson.M{"$gte":33}}).All(&users)
<=($lte)[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#lte)
c.Find(bson.M{"age": bson.M{"$lte":31}}).All(&users)
in($in)[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#in-in)
c.Find(bson.M{"name": bson.M{"$in":[]string{"Jimmy Kuu","Tracy Yu"}}}).All(&users)
###### 多条件查询[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#id10)
and($and)[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#and-and)
c.Find(bson.M{"name":"Jimmy Kuu","age":33}).All(&users)
or($or)[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#or-or)
c.Find(bson.M{"$or":[]bson.M{bson.M{"name":"Jimmy Kuu"}, bson.M{"age":31}}}).All(&users)
#### 修改[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#id11)
通过func (*Collection) Update来进行修改操作。
func (c *Collection)Update(selector interface{}, change interface{}) error
注意修改单个或多个字段需要通过$set操作符号，否则集合会被替换。
##### 修改字段的值($set)[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#set)
c.Update(bson.M{"_id": bson.ObjectIdHex("5204af979955496907000001")},
    bson.M{"$set": bson.M{"name":"Jimmy Gu","age":34,}})
##### inc($inc)[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#inc-inc)
字段增加值
c.Update(bson.M{"_id": bson.ObjectIdHex("5204af979955496907000001")},
    bson.M{"$inc": bson.M{"age":-1,}})
##### push($push)[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#push-push)
从数组中增加一个元素
c.Update(bson.M{"_id": bson.ObjectIdHex("5204af979955496907000001")},
    bson.M{"$push": bson.M{"interests":"Golang",}})
##### pull($pull)[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#pull-pull)
从数组中删除一个元素
c.Update(bson.M{"_id": bson.ObjectIdHex("5204af979955496907000001")},
    bson.M{"$pull": bson.M{"interests":"Golang",}})
#### 删除[](http://jimmykuu.sinaapp.com/static/books/mgo_guide/index.html#id12)
c.Remove(bson.M{"name":"Jimmy Kuu"})
注解
这里也支持多条件，参考多条件查询。

其余示例一：
github:  [https://github.com/ZhangzheBJUT/blog/blob/master/mgo.md](https://github.com/ZhangzheBJUT/blog/blob/master/mgo.md)
**注**：现在MongoDB官方还没有推出关于官方支持的golang的driver,推荐使用的是mgo， mgo的详细文档说明：http://godoc.org/labix.org/v2/mgo
下面是开发中自己写的一个用mgo连接MongoDB数据库的使用实例。
```
package main
import (
    "fmt"
    "labix.org/v2/mgo"
    "labix.org/v2/mgo/bson"
)
type Person struct {
    NAME  string
    PHONE string
}
type Men struct {
    Persons []Person
}
const = (
    URL = "192.168.2.175:27017"
)
func main() {
    session, err := mgo.Dial(URL)  //连接数据库
    if err != nil {
        panic(err)
    }
    defer session.Close()
    session.SetMode(mgo.Monotonic, true)
    db := session.DB("mydb")     //数据库名称
    collection := db.C("person") //如果该集合已经存在的话，则直接返回
    //*****集合中元素数目********
    countNum, err := collection.Count()
    if err != nil {
        panic(err)
    }
    fmt.Println("Things objects count: ", countNum)
    //*******插入元素*******
    temp := &Person{
        PHONE: "18811577546",
        NAME:  "zhangzheHero"
    }
        //一次可以插入多个对象 插入两个Person对象
    err = collection.Insert(&Person{"Ale", "+55 53 8116 9639"}, temp)
    if err != nil {
        panic(err)
    }
    //*****查询单条数据*******
    result := Person{}
    err = collection.Find(bson.M{"phone": "456"}).One(&result)
    fmt.Println("Phone:", result.NAME, result.PHONE)
**    //*****查询多条数据*******    var personAll Men  //存放结果    iter := collection.Find(nil).Iter()    for iter.Next(&result) {        fmt.Printf("Result: %v\n", result.NAME)        personAll.Persons = append(personAll.Persons, result)    }**
    //*******更新数据**********
    err = collection.Update(bson.M{"name": "ccc"}, bson.M{"$set": bson.M{"name": "ddd"}})
    err = collection.Update(bson.M{"name": "ddd"}, bson.M{"$set": bson.M{"phone": "12345678"}})
    err = collection.Update(bson.M{"name": "aaa"}, bson.M{"phone": "1245", "name": "bbb"})
    //******删除数据************
    _, err = collection.RemoveAll(bson.M{"name": "Ale”})
}
```
其余示例二： 
https://docs.compose.io/languages/golang.html
https://gist.github.com/border/3489566
