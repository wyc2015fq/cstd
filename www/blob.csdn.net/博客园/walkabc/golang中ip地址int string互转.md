# golang中ip地址int string互转 - walkabc - 博客园
# [golang中ip地址int string互转](https://www.cnblogs.com/hitfire/articles/6597654.html)
　　ip4的地址格式为255.255.255.255，很显然最大值255可以使用一个字节来保存，总共使用4个字节就可以保存，所以使用一个32位的int整型来保存ip地址。
　　之后从int整形转为ip字符串时，分别对32位的每8位进行处理即可，均可以通过简单的位运算获得
　　废话不多说直接上代码
```go;gutter
package iphelper
import (
	"strings"
	"strconv"
	"bytes"
)
func StringIpToInt(ipstring string) int {
	ipSegs := strings.Split(ipstring, ".")
	var ipInt int = 0
	var pos uint = 24
	for _, ipSeg := range ipSegs {
		tempInt, _ := strconv.Atoi(ipSeg)
		tempInt = tempInt << pos
		ipInt = ipInt | tempInt
		pos -= 8
	}
	return ipInt
}
func IpIntToString(ipInt int) string {
	ipSegs := make([]string, 4)
	var len int = len(ipSegs)
	buffer := bytes.NewBufferString("")
	for i := 0; i < len; i++ {
		tempInt := ipInt & 0xFF
		ipSegs[len-i-1] = strconv.Itoa(tempInt)
		ipInt = ipInt >> 8
	}
	for i := 0; i < len; i++ {
		buffer.WriteString(ipSegs[i])
		if i < len-1 {
			buffer.WriteString(".")
		}
	}
	return buffer.String()
}
```
