# MongoDB C#驱动 - weixin_33985507的博客 - CSDN博客
2013年11月13日 11:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6

# [烟波钓徒](http://www.cnblogs.com/kingdomheart/)
## [MongoDB C#驱动](http://www.cnblogs.com/kingdomheart/archive/2012/12/28/2837983.html)
[http://www.mongodb.org/display/DOCS/CSharp+Driver+Tutorial](http://www.mongodb.org/display/DOCS/CSharp+Driver+Tutorial)
笔记
首先下载驱动。驱动有两个文件
- MongoDB.Bson.dll
- MongoDB.Driver.dll
可以直接下载这两个驱动，或者按照下载源码进行编译生成。下载的源码可以看些test例子。
在新建的c#工程中添加这两个dll文件，并且使用如下命名空间
至少要引用如下命名空间
```
using MongoDB.Bson; 
using MongoDB.Driver;
```
`另外使用比较多的命名空间是`
```
using MongoDB.Driver.Builders;
using MongoDB.Driver.GridFS;
using MongoDB.Driver.Linq;
```
另外有些可能会用得到的命名空间
```
using MongoDB.Bson.IO;
using MongoDB.Bson.Serialization;
using MongoDB.Bson.Serialization.Attributes;
using MongoDB.Bson.Serialization.Conventions;
using MongoDB.Bson.Serialization.IdGenerators;
using MongoDB.Bson.Serialization.Options;
using MongoDB.Bson.Serialization.Serializers;
using MongoDB.Driver.Wrappers;
```
`BSON类库``BSON是类似JSON的一种二进制形式的存储格式，简称Binary JSON，它和JSON一样，支持内嵌的文档对象和数组对象，但是BSON有JSON没有的一些数据类型，如Date和BinData类型。它也是MongoDB文档数据库内部的数据存储方式。``BsonType`` `
```
public enum BsonType {
    Double = 0x01,
    String = 0x02,
    Document = 0x03,
    Array = 0x04,
    Binary = 0x05,
    Undefined = 0x06,
    ObjectId = 0x07,
    Boolean = 0x08,
    DateTime = 0x09,
    Null = 0x0a,
    RegularExpression = 0x0b,
    JavaScript = 0x0d,
    Symbol = 0x0e,
    JavaScriptWithScope = 0x0f,
    Int32 = 0x10,
    Timestamp = 0x11,
    Int64 = 0x12,
    MinKey = 0xff,
    MaxKey = 0x7f
}
```
BsonValue和子类
BsonValue是一种代表BsonType的虚拟类。它是BsonType枚举类的凝聚子类。
·可以使用public构造函数生成BsonValue子类
·使用静态create函数生成
·Use a static property of a subclass of BsonValue（静态的子类属性？）
·隐式转换成BsonValue
BsonType的类型
可以用下面的例子代码确认BsonValue的属性
```
BsonValue value;
if (value.BsonType == BsonType.Int32) {
    // we know value is an instance of BsonInt32
}
if (value is BsonInt32) {
    // another way to tell that value is a BsonInt32
}
if (value.IsInt32) {
    // the easiest way to tell that value is a BsonInt32
}
```
As[Type] Properties
BsonValue有一系列转换方式将它的类型cast（抛）（而不是conversion）成与.NET相匹配的数据类型。如果他不是一个.NET相对应的数据属性，它将会抛出一个InvalidCastException 异常。下面是一些将数据转变的方式。
```
BsonDocument document;
string name = document["name"].AsString;//As方式，类似转变
int age = document["age"].AsInt32;
BsonDocument address = document["address"].AsBsonDocument;
string zip = address["zip"].AsString;
```
Is[Type] Properties
使用下面例子测试BsonValues是什么类型
```
BsonDocument document;
int age = -1;
if (document.Contains["age"] && document["age"].IsInt32) {//Is 是否为Int32类型
    age = document["age"].AsInt32;
}
To[Type] 转变方法
```
`与As不同，To是用于可以转变类型之间的转类型。比如int和double之间。``比如ToBoolen方法永远不会失败。它是按照javascript里面定义的。false, 0, 0.0, NaN, BsonNull, BsonUndefined 以及"" 是false，其他所有都是true。`
```
if (employee["ismanager"].ToBoolean()) {
    // we know the employee is a manager
    // works with many ways of recording boolean values
}
ToDouble、ToInt32、以及ToInt64在数字之间的转变都不会失败。即使数字长度不匹配被缩短了都不会照成函数错误。string类型可以转成数字类型。但是如果string类型不能转成相应的数字的时候，会抛出异常。
```
`隐式的转化``下面的数据类型可以直接转化`- bool
- byte[]
- DateTime
- double
- Enum
- Guid
- int
- long
- ObjectId
- Regex
- string
比如下面
```
BsonValue b = true; // b is an instance of BsonBoolean
BsonValue d = 3.14159; // d is an instance of BsonDouble
BsonValue i = 1; // i is an instance of BsonInt32
BsonValue s = "Hello"; // s is an instance of BsonString
```
BsonMaxKey, BsonMinKey, BsonNull and BsonUndefined
这些数据类型是单个的类，要用到这些数据，需要使用各自的类来生成
```
document["status"] = BsonNull.Value;
document["priority"] = BsonMaxKey.Value;
注意，这个c#的null和BsonNull是两个完全不同的东西。BsonNull是一个C#类，它的Value属性是null。所以他们在函数构造不同。
```
` ``ObjectId and BsonObjectId``一些常用的创建ObjectId 值的方式`
```
var id1 = new ObjectId(); // same as ObjectId.Empty
var id2 = ObjectId.Empty; // all zeroes
var id3 = ObjectId.GenerateNewId(); // generates new unique Id
var id4 = ObjectId.Parse("4dad901291c2949e7a5b6aa8"); // parses a 24 hex digit string
```
在C#里面，刚创建的值默认都是零的。但是在javascript里面会创建一个唯一的值。
BsonElement
`（Bson元素）``Bson元素是一个name/value的键值对。`
```
document.Add(new BsonElement("age", 21)); // OK, but next line is shorter
document.Add("age", 21); // creates BsonElement automatically
```
``
BsonDocument
` ``BsonDocument是name/value键值对的集合。``BsonDocument构造函数`- BsonDocument()
- BsonDocument(string name, BsonValue value)
上面是用的比较多
- BsonDocument(BsonElement element)
- BsonDocument(Dictionary<string, object> dictionary)
- BsonDocument(Dictionary<string, object> dictionary, IEnumerable<string> keys)
- BsonDocument(IDictionary dictionary)
- BsonDocument(IDictionary dictionary, IEnumerable<string> keys)
- BsonDocument(IDictionary<string, object> dictionary)
- BsonDocument(IDictionary<string, object> dictionary, IEnumerable<string> keys)
- BsonDocument(IEnumerabe<BsonElement> elements)
- BsonDocument(params BsonElement[] elements)
- BsonDocument(bool allowDuplicateNames)
创建一个新的document并且调用Add和Set函数
```
BsonDocument book = new BsonDocument();
book.Add("author", "Ernest Hemingway");
book.Add("title", "For Whom the Bell Tolls");
```
`另外一种方式`
```
BsonDocument book = new BsonDocument()
    .Add("author", "Ernest Hemingway")
    .Add("title", "For Whom the Bell Tolls");
```
`使用c#的collection初始化语法（推荐）`
```
BsonDocument book = new BsonDocument {
    { "author", "Ernest Hemingway" },
    { "title", "For Whom the Bell Tolls" }
};
编译器会将它翻译成调用Add函数的方式
```
```
BsonDocument book = new BsonDocument();
    book.Add("author", "Ernest Hemingway");
    book.Add("title", "For Whom the Bell Tolls");
不要缺少里面的大括号，不然就会像下面一样
```
```
BsonDocument bad = new BsonDocument {
    "author", "Ernest Hemingway"
};
编译成：
```
```
BsonDocument bad = new BsonDocument();
bad.Add("author");
bad.Add("Ernest Hemingway");
```
`创建嵌套的BSON documents`
```
BsonDocument nested = new BsonDocument {
    { "name", "John Doe" },
    { "address", new BsonDocument {
        { "street", "123 Main St." },
        { "city", "Centerville" },
        { "state", "PA" },
        { "zip", 12345}
    }}
};
”address”是一个嵌套的document
```
` `
ADD函数
- Add(BsonElement element)
- Add(Dictionary<string, object> dictionary)
- Add(Dictionary<string, object> dictionary, IEnumerable<string> keys)
- Add(IDictionary dictionary)
- Add(IDictionary dictionary, IEnumerable<string> keys)
- Add(IDictionary<string, object> dictionary)
- Add(IDictionary<string, object> dictionary, IEnumerable<string> keys)
- Add(IEnumerable<BsonElement> elements)
- Add(string name, BsonValue value)
- Add(string name, BsonValue value, bool condition)
如果里面的属性值是null的话，add函数将不把数据加入到document中
```
BsonDocument document = new BsonDocument {
    { "name", name },
    { "city", city }, // not added if city is null
    { "dob", dob, dobAvailable } // not added if dobAvailable is false
};
可以翻译成如下：
```
```
BsonDocument document = new BsonDocument();
document.Add("name", name);
if (city != null) {
    document.Add("city", city);
}
if (dobAvailable) {
    document.Add("dob", dob);
}
如果想add一个BsonNull值，可以使用C#中的null联合
```
```
BsonDocument = new BsonDocument {
    { "city", city ?? BsonConstants.Null }
};
```
`获取BsonDocument的元素elements`- BsonValue this[int index]
- BsonValue this[string name]
- BsonValue this[string name, BsonValue defaultValue]
返回的是BsonValue值，例子如下
```
BsonDocument book;
string author = book["author"].AsString;
DateTime publicationDate = book["publicationDate"].AsDateTime;
int pages = book["pages", -1].AsInt32; // default value is -1
```
``
BsonArray
这个类是用来表示BSON 数组。
构造函数
- BsonArray()
- BsonArray(IEnumerable<bool> values)
- BsonArray(IEnumerable<BsonValue> values)
- BsonArray(IEnumerable<DateTime> values)
- BsonArray(IEnumerable<double> values)
- BsonArray(IEnumerable<int> values)
- BsonArray(IEnumerable<long> values)
- BsonArray(IEnumerable<ObjectId> values)
- BsonArray(IEnumerable<string> values)
- BsonArray(IEnumerable values)
Add 和 AddRange 函数
- BsonArray Add(BsonValue value)
- BsonArray AddRange(IEnumerable<bool> values)
- BsonArray AddRange(IEnumerable<BsonValue> values)
- BsonArray AddRange(IEnumerable<DateTime> values)
- BsonArray AddRange(IEnumerable<double> values)
- BsonArray AddRange(IEnumerable<int> values)
- BsonArray AddRange(IEnumerable<long> values)
- BsonArray AddRange(IEnumerable<ObjectId> values)
- BsonArray AddRange(IEnumerable<string> values)
- BsonArray AddRange(IEnumerable values)
```
// traditional approach
BsonArray a1 = new BsonArray();
a1.Add(1);
a2.Add(2);
// fluent interface
BsonArray a2 = new BsonArray().Add(1).Add(2);
// values argument
int[] values = new int[] { 1, 2 };
BsonArray a3 = new BsonArray(values);
// collection initializer syntax
BsonArray a4 = new BsonArray { 1, 2 };
```
访问
```
BsonArray array = new BsonArray { "Tom", 39 };
string name = array[0].AsString;
int age = array[1].AsInt32;
```
