# 利用reflect设置struct的字段 - 三少GG - CSDN博客
2015年12月29日 00:00:27[三少GG](https://me.csdn.net/scut1135)阅读数：2615
**极为重要的两篇文章**
**1.** http://speakmy.name/2014/09/14/modifying-interfaced-go-struct/
Combining it all together, this is the final function:
```
func Destroy(subj interface{}) {
	stype := reflect.ValueOf(subj).Elem()
	field := stype.FieldByName("Status")
	if field.IsValid() {
		field.SetString("Destroyed")
	}
}
```
```
func TestDestroy(t *testing.T) {
	// Initialize data
	jaeger := Jaeger{Name: "Cherno Alpha", Country: "RU", Status: "Active"}
	kaiju := Kaiju{Alias: "Scissure", Origin: "Sydney", Status: "Unknown"}
	shatterdome := Shatterdome{Location: "Lima"}
	// Destroy everything
	Destroy(&jaeger)
	Destroy(&kaiju)
	Destroy(&shatterdome)
	// Check the result
	if jaeger.Status != "Destroy" {
		t.Error("jaeger was not destroyed")
	}
	if kaiju.Status != "Destroy" {
		t.Error("kaiju was not destroyed")
	}
}
```
**2.**
参考golang反射三大规则
reflect.Value区分CanSet和Can not Set的, 所以, 必须要返回成Can set的reflect.Value
如:
```
s := reflect.ValueOf(&t).Elem()
```
然后就可以happy的设值了, 可是不能随便设值的, 一个通用的方法就是使用Set(v Value)方法, 所以之前必须将值转成reflect.Value类型, 下面的这段代码就是转成Value类型
```
sliceValue := reflect.ValueOf([]int{1, 2, 3}) // 这里将slice转成reflect.Value类型
```
完整参考:
```cpp
type T struct {
    Age int
    Name string
    Children []int
}
t := T{12, "someone-life", nil}
s := reflect.ValueOf(&t).Elem()
s.Field(0).SetInt(123) // 内置常用类型的设值方法
sliceValue := reflect.ValueOf([]int{1, 2, 3}) // 这里将slice转成reflect.Value类型
s.FieldByName("Children").Set(sliceValue)
```
# ###############
# How to use Reflect to set a struct field
http://samwize.com/2015/03/20/how-to-use-reflect-to-set-a-struct-field/
In short, if you want to set a struct `foo` object when you know the field
 name to set, this is the code:
funcsetFoo(foo*Foo,fieldstring,valuestring){
v:=reflect.ValueOf(foo).Elem().FieldByName(field)
ifv.IsValid(){
v.SetString(value)
}
}
If you want to print all the fields of `foo`, this is the code:
s:=reflect.ValueOf(foo).Elem()
typeOfT:=s.Type()
fori:=;i<s.NumField();i++{
f:=s.Field(i)
fmt.Printf("%d: %s %s = %v\n",i,
typeOfT.Field(i).Name,f.Type(),f.Interface())
}
