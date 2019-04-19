# golang 动态 实例化 结构体 - 三少GG - CSDN博客
2015年12月27日 16:19:17[三少GG](https://me.csdn.net/scut1135)阅读数：4574
真理 http://stackoverflow.com/questions/10210188/instance-new-type-golang
So, if I understand your question correctly, you are asking about how you can create an object when you just have the name of the type as string. So, for example, you might have a string "MyStruct" and you want to create an object of this type.
**Unfortunately, that's not easily possible because Go is a statically typed language and the linker will eliminate dead code (or inline parts of it). So, there is no guarantee, that your final executable will even contain the code of "MyStruct".**
**You can however, maintain a global `map[string]reflect.Type` manually. For example by initializing this map in the `init()` function of your packages which defines such discover-able types. **This will also tell the compiler that you are using the types. Afterwards, you can use this map to look up the [reflect.Type](http://golang.org/pkg/reflect/#Type) of
 the type you want to create and use [reflect.New](http://golang.org/pkg/reflect/#New) to get a pointer to a new object of that
 type (stored as a reflect.Value). You can extract the object into an interface with something like this:
`reflect.New(yourtype).Elem().Interface()`
[Elem()](http://golang.org/pkg/reflect/#Value.Elem) will de-reference the pointer and [Interface()](http://golang.org/pkg/reflect/#Value.Interface) will
 return the reflected value as an `interface{}`.
 See [The Laws of Reflection](http://blog.golang.org/2011/09/laws-of-reflection.html) for further details.
PS: There might be a better way to structure your program which doesn't even require reflection and which let the compiler catch more errors. Have you considered using a [factory
 method](http://en.wikipedia.org/wiki/Factory_method_pattern) for example? An other easy solution might be to maintain a 
```
map[string]func()
 interface{}
```
 of functions which can be invoked to create a new object with that name.
http://mikespook.com/2012/07/%E5%9C%A8-golang-%E4%B8%AD%E7%94%A8%E5%90%8D%E5%AD%97%E8%B0%83%E7%94%A8%E5%87%BD%E6%95%B0/
但这里有一个限制：这个 map 仅仅可以用原型是“func()”的没有输入参数或返回值的函数。
如果想要用这个方法实现调用不同函数原型的函数，需要用到 interface{}。
***********参考*****************
1.  https://www.socketloop.com/tutorials/golang-fix-type-interface-has-no-field-or-no-methods-and-type-assertions-example
# [Golang : Fix type interface{} has no field or no methods and type assertions example](https://www.socketloop.com/tutorials/golang-fix-type-interface-has-no-field-or-no-methods-and-type-assertions-example)
2. [golang用字符串反射实例化结构体](http://www.01happy.com/golang-get-struct-instance-by-reflect/)
```
package main
 
import (
 "fmt"
 "reflect"
)
 
type Foo struct {
}
type Bar struct {
}
 
//用于保存实例化的结构体对象
var regStruct map[string]interface{}
 
func main() {
 str := "Bar"
 if regStruct[str] != nil {
  t := reflect.ValueOf(regStruct[str]).Type()
  v := reflect.New(t).Elem()
  fmt.Println(v)
 }
 
}
 
func init() {
 regStruct = make(map[string]interface{})
 regStruct["Foo"] = Foo{}
 regStruct["Bar"] = Bar{}
}
```
**3. http://stackoverflow.com/questions/17507697/dry-out-my-go-function-with-interfaces**
```
kinds := map[string]func() Entity {
    "user": func() Entity { return &User{} },
    "space": func() Entity { return &Space{} },
    "room": func() Entity { return &Room{} },
}
	
func Create(kind string) {
    instance := kinds[kind]()
    decoder.Decode(instance)
    saveEntity(instance)
}
```
**4.  新手求解 golang 动态new struct **
**http://www.oschina.net/question/1388294_141504?sort=default&p=2#answers**
```
<span style="font-size:14px;">我现在是这样做的
var ModelObj map[string]interface{} = map[string]interface{}{
"Member": new(Member), 
} 
func GetModObj(mod string) interface{} { 
if ModelObj[mod] != nil { 
return ModelObj[mod] 
} else { 
return nil 
} 
} </span>
```
5. http://stackoverflow.com/questions/13856539/how-do-you-get-struct-value-on-unknown-interface
