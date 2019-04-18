# 以太坊RLP使用例子 - weixin_33985507的博客 - CSDN博客
2018年06月13日 17:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
## RLP
Recursive Length Prefix，是以太坊中数据序列化和反序列化的主要方法，区块、交易等数据结构会先经过RLP编码再存入数据库。
下面代码是RLP序列化和反序列化的例子。
```
package main
import (
    "fmt"
    "github.com/ethereum/go-ethereum/rlp"
)
// 结构体定义时，字段名需要定义成大些的，否则解码时无法解码出来，官方demo中A、B就是小写的
// 因此解码不出来
type MyCoolType2 struct {
    Name string
    A, B uint
}
func ExampleEncoder() {
    var t *MyCoolType2 // t is nil pointer to MyCoolType
    // 编码的例子
    bytes_, _ := rlp.EncodeToBytes(t)
    fmt.Printf("%v → %X\n", t, bytes_)
    t = &MyCoolType2{Name: "foobar", A: 10, B: 6}
    bytes_, _ = rlp.EncodeToBytes(t)
    fmt.Printf("%v → %X\n", t, bytes_)
    // 解码的例子
    var val MyCoolType2
    err := rlp.DecodeBytes(bytes_, &val)
    fmt.Printf("MyCoolType2:err=%+v,val=%+v\n", err, val)
}
func main() {
    ExampleEncoder()
}
```
结果：
```
<nil> → C0
&{foobar 10 6} → C986666F6F6261720A06
MyCoolType2:err=<nil>,val={Name:foobar A:10 B:6}
```
参考文献：
[ethereum/wiki/RLP](https://github.com/ethereum/wiki/wiki/RLP)
