# go json解析 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年02月07日 11:53:13[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：218
JSON转换库为 encoding/json
把对象转换为JSON的方法（函数）为 json.Marshal()，其函数原型如下
　　　　func Marshal(v
interface{}) ([]byte, error)
　　　　也就是说，这个函数接收任意类型的数据 v，并转换为字节数组类型，返回值就是我们想要的JSON数据和一个错误代码。当转换成功的时候，这个错误代码为nil

把 JSON 转换回对象的方法（函数）为 json.Unmarshal()，其函数原型如下
　　　　func Unmarshal(data [] byte, v
interface{}) error
　　　　这个函数会把传入的 data 作为一个JSON来进行解析，解析后的数据存储在参数 v 中。这个参数 v 也是任意类型的参数（但一定是一个类型的指针）
b, err := json.Marshal(st)
if err != nil {
fmt.Println("json err:", err)
}
fmt.Println(string(b))
输出字段名的首字母都是大写的，如果你想用小写的首字母怎么办呢？
必须通过struct tag定义来实现：
type Server struct {
ServerName string `json:"serverName"`
ServerIP string `json:"serverIP"`
}
type Serverslice struct {
Servers []Server `json:"servers"`
}
通过修改上面的结构体定义，输出的JSON串就和我们最开始定义的JSON串保持一致了。

json转成对象
stb := &Student{}
err = json.Unmarshal([]byte(strData), &stb)
[https://www.cnblogs.com/fengbohello/p/4665883.html](https://www.cnblogs.com/fengbohello/p/4665883.html)
请问这个短横线是什么关键字？
 Classes    []string
占位符，表示忽略下标
[https://www.cnblogs.com/fengxm/p/5997265.html](https://www.cnblogs.com/fengxm/p/5997265.html)
json_map := make(map[string]interface{})
err = json.Unmarshal(body, &json_map)
JSON是Javascript的一个子集
 JSON 文本（实际上是一个 []byte）：

定义了与json数据对应的结构体
type Server struct {
ServerName string
ServerIP string
}
type Serverslice struct {
Servers []Server
}
func main() {
var s Serverslice
str := `{"servers":[{"serverName":"Shanghai_VPN","serverIP":"127.0.0.1"},{"serverName":"Beijing_VPN","serverIP":"127.0.0.2"}]}`
json.Unmarshal([]byte(str), &s)
fmt.Println(s)
}
struct字段(首字母大写)
上面那种解析方式是在我们知晓被解析的JSON数据的结构的前提下采取的方案，如果我们不知道被解析的数据的格式，又应该如何来解析呢？
我们知道interface{}可以用来存储任意数据类型的对象，这种数据结构正好用于存储解析的未知结构的json数据的结果。JSON包中采用map[string]interface{}和[]interface{}结构来存储任意的JSON对象和数组。
现在我们假设有如下的JSON数据
b := []byte(`{"Name":"Wednesday","Age":6,"Parents":["Gomez","Morticia"]}`)
如果在我们不知道他的结构的情况下，我们把他解析到interface{}里面
var f interface{}
err := json.Unmarshal(b, &f)
这个时候f里面存储了一个map类型，他们的key是string，值存储在空的interface{}里
f = map[string]interface{}{
"Name": "Wednesday",
"Age": 6,
"Parents": []interface{}{
"Gomez",
"Morticia",
},
}
如何来访问这些数据呢？通过断言的方式：
m := f.(map[string]interface{})
通过断言之后，你就可以通过如下方式来访问里面的数据了
for k, v := range m {
switch vv := v.(type) {
case string:
fmt.Println(k, "is string", vv)
case int:
fmt.Println(k, "is int", vv)
case float64:
fmt.Println(k,"is float64",vv)
case []interface{}:
fmt.Println(k, "is an array:")
for i, u := range vv {
fmt.Println(i, u)
}
default:
fmt.Println(k, "is of a type I don't know how to handle")
}
}
JSON对象只支持string作为key，所以要编码一个map，那么必须是map[string]T这种类型(T是Go语言中任意的类型)
[https://github.com/astaxie/build-web-application-with-golang/blob/master/zh/07.2.md](https://github.com/astaxie/build-web-application-with-golang/blob/master/zh/07.2.md)
