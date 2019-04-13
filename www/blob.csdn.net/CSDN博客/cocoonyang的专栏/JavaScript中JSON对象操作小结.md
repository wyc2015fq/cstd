
# JavaScript中JSON对象操作小结 - cocoonyang的专栏 - CSDN博客


2017年06月05日 10:11:06[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：1813


**0. 简介**
**JSON**(JavaScript Object Notation) 是一种轻量的数据交换格式[1]. JSON有四种数据类型和两种数据集合类型，分别为：
数据类型: string, number, Boolean( true, false), null,
数据集合类型: object, array,
**string**是一列包含在两个双引号之间，有0个或多个Unicode 字符的字符串。string使用反斜杠("\")做转义字符[1]。
**number**与C语言中的数的格式相同，只是没有8进制数和16进制数。
array是一组有序的值的集合。array由 [ 起始，以 ] 结束。array中的值由逗号分隔[1]。
object是一个无序集合，object由 { 起始，以 } 结束。集合中的元素为 名称/值(name/value) 组成的数据对。名称与其相应的值之间由冒号 (":")分隔，对元素之间由逗号分隔。object的语法如下图所示：
![json-object](https://img-blog.csdn.net/20170620113807667?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY29jb29ueWFuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Figure JSON Object[1]
由上图可以看出，object中数据对中名称的数据类型只能是字符串(string)，而数据对中的value(值)可以为由双引号界定的字符串，或者是数(number)，或者是bool型数据，或者是null数据，也可以是object型数据.


## 1. JSON构造方法
常用的JSON对象构造方法有四种：直接构造，使用面向对象方法构造，使用字符串构造以及通过读取文件构造。
### 1.1 直接构造
```python
var myJson ={ 'key':'value', 
              'key1':[1,2,3,4], 
               4.1e-4 :'hello',
              [ 4.1e-4  + 9.0 ]:'hello',
              [ '4.1e-4' + 9.0 ]:'hello',
               true :'hello',
             };
```
As show in Figure JSON Object, the data type of name is string. Besides string, number, one element array, bool can also set as name key,  and JSON would transform these keys as string.  As show in the above codes, array which contains only one element would be transformed into string successfully. If the array contains more than one element,  an exception would be invoked when it is set as a key of JSON object.
上述例子中的JSON对象也可使用下列代码构造实现：
```python
var myJson ={}
myJson["key"] = "value";
myJson["key1"] = [1,2,3,4];
myJson[4.1e-4] = 'hello';
myJson[ 4.1e-4  + 9.0 ] = 'hello',
myJson[ '4.1e-4' + 9.0 ] = 'hello';
myJson[true] = 'hello';
```


### 1.2 使用面向对象方法构造
```python
function MyClass(){
    this.key = 'one';
    this.key2 = {
       'key3': 'another'
    };
}
var myJson = new MyClass();
```

### 1.3 由String构造
```python
var jsonStr = "{\"key\":\"value\"}";
var jsonObj = JSON.parse(jsonStr);
console.log(jsonObj);
console.log(jsonObj.key);
```


### 1.4 读取json文件构造
#### 1.4.1  With require Method
```python
var jsonObj = require("./path/to/myjsonfile.json");
```
#### 1.4.2 Using readFile Method
```python
var theConfig  = fs.readFileSync( thePath );
   var jsonObj = JSON.parse( theConfig );
```

## 2. Element Operations
### 2.1  Add New Elements
```python
var myJson = {'key':'value'};
// 
myJson.key2 = 'value2';
//or
myJson['key3'] = 'value3';
console.log( myJson )
```

### 2.2  Delete Elements
```python
var myJson = {'key':'value'};
delete myJson['key'];
```

### 2.3  Adding Comment
JSON语法中没有注释这种数据类型。如果想对JSON对象中的数据添加注释，可以增加一个保存注释的数据项，例如"comment":"xxx"

### 2.4  Iterate
```python
var myJson = {'key1':'value1', 'key2':'value2'};
for(var theKey in myJson) {
   console.log("key:"+theKey+", value:"+myJson[theKey]);
}
```

### 2.5  Deep Iterate
```python
function iterateJSON( theJSON, variableName )
{
     for( var id in theJSON )
     { 
         if (typeof theJSON[id] === 'object')
         {
               iterateJSON( theJSON[id], variableName )
               if( id == variableName )
               {
                     // do something
               }
          }else{
               if( id == variableName )
               {
                     // do something 
               }
          }
     }
}
```

### 2.6  Check An Element
```python
var myJson = {'key1':'value1', 'key2':'value2'};
if(myJson.hasOwnProperty('key1')){
     //do something  
}
```
这个方法只能查询JSON对象的下一层子数据，如果要查询深层的数据内容(孙子数据，重孙数据等：），则需要使用deep iterate方法。

## 3. Outputing Operations
### 3.1 To String
```python
console.log(JSON.stringify(instance, null, " "));
```
“The JSON.stringify() method converts a JavaScript value to a JSON string, optionally replacing values if a replacer function is specified, or optionally including only the specified properties if a replacer array is specified”[3]
Syntax of stringify method[3]
```python
JSON.stringify(value[, replacer[, space]])
value：
    The value to convert to a JSON string.
replacer：  
    A function that alters the behavior of the stringification process, 
    or an array of String and Number objects that serve as a whitelist 
    for selecting/filtering the properties of the value object to be 
    included in the JSON string. If this value is null or not provided, 
    all properties of the object are included in the resulting JSON string.
space： 
    A String or Number object that's used to insert white space into the 
    output JSON string for readability purposes. If this is a Number, it 
    indicates the number of space characters to use as white space; this 
    number is capped at 10 (if it is greater, the value is just 10). 
    Values less than 1 indicate that no space should be used. If this is 
    a String, the string (or the first 10 characters of the string, if 
    it's longer than that) is used as white space. If this parameter is not
    provided (or is null), no white space is used.
```
Examples replacer parameter [3]

```python
var foo = {foundation: 'Mozilla', model: 'box', week: 45, transport: 'car', month: 7};
// Example with a function 
function replacer(key, value) {
  // Filtering out properties
  if (typeof value === 'string') {
    return undefined;
  }
  return value;
}
JSON.stringify(foo, replacer);
// '{"week":45,"month":7}'
// Example with an array
JSON.stringify(foo, ['week', 'month']);  
// '{"week":45,"month":7}', only keep "week" and "month" properties
```

### 3.2 To Tree View[4] Html
The underlying of JSON object is tree data structure, and it is convenient to show a JSON object in tree view. [4] provides a simple and efficient facilities of tree view. The following codes covert a JSON object into a HTML file abiding the format of [4].

## 4. 解析
## 4.1 使用V8 引擎解析JSON对象

```python
void json(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	Isolate* isolate = args.GetIsolate();
	v8::Handle<v8::Object> theArg  = args[0]->ToObject();
	if (theArg.IsEmpty() || theArg->IsNull()
			|| theArg->IsUndefined() || theArg->IsFunction())
	{
		printf("Invalid argument.");
		return;
	}
	v8::Handle<v8::Value> JSON_parse_value = theArg->Get(v8::String::NewFromUtf8(isolate,  "Something"));
	//获取所有的key
	v8::Handle<v8::Object> theObj = JSON_parse_value->ToObject();
	v8::Handle<v8::Array> keys = theObj->GetOwnPropertyNames();
	//获取object内元素的个数
	int len = keys->Length();
	for (int i = 0; i < len; i++)
	{
		v8::Handle<v8::Value> theKey = keys->Get(i);
		v8::Handle<v8::Value> theValue = theObj->Get(theKey);
		v8::String::Utf8Value theStr(theValue->ToString());
		v8::String::Utf8Value keyStr(theKey->ToString());
		printf("%s: %s \n", *keyStr, *theStr);
	}
	return;
}
```
测试脚本代码
```python
var JSONParser = require( 'jsonparser')
var parser = new JSONParser();
var jsonObj = {}; 
jsonObj["Something"] = {"mass": "10", "disp": "20" };
 
parser.json( jsonObj );
```


## 5. JSON vs 容器类
计算机的存储、操作空间是一个一维拓扑空间，它最小单元是位。然而现实世界是一个多维度拓扑空间(从物理角度看，可以简单看作三维空间+一维时间)。 计算机语言，数据结构，算法都是将现实世界多维度拓扑空间中的元素及其关系映射到计算机一维拓扑空间中。JSON就很好的体现体现了这种多维到一维的映射关系。
### 相同点：
JSON和C++程序设计语言中的容器类都是用于保存一个数据单元集合的数据结构。
### 不同点：
C++程序设计语言中的容器类提供 对容器中数据单元操作接口，如遍历，添加，删除，提取，插入，更新等。JSON只提供了数据结构，不提供操作接口。

# 参考文献
[1]   http://json.org/
[2]   http://codesamplez.com/programming/using-json-in-node-js-javascript
[3]   https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/JSON/stringify
[4]   http://www.codestrive.com/examples/treeview.zip



