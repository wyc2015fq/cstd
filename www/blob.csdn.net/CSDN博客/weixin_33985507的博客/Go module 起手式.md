# Go module 起手式 - weixin_33985507的博客 - CSDN博客
2018年09月05日 22:28:04[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
- **安装 Golang 到最新版并检查**
```
> //低于下面的版本去 golang.google.cn 升级 Golang
> go version
> go version go1.11 darwin/amd64
```
- **设置环境变量开启功能**
```
> //下面是临时开启，写到 bashrc 或者 zshrc 来永久开启
> export GO111MODULE=on
```
- **初始化项目**
```
>mkdir myproject1 && cd myproject1
>go mod init
```
- **直接写代码**
```
package main //import github.com/airylinus/blank5
import (
    "log"
    "github.com/airylinus/goutils"
)
func main() {
    log.Println(goutils.RandStringBytesMaskImprSrc(16))
}
```
然后编译运行
`go run main.go`
**`注意，我们并没有 go get github.com/airylinus/goutils`**
- 
**后续和扩展**
- 包管理是个比较复杂的问题，有循环依赖等问题
- go mod 的原理有很多资料可以看
- [https://docs.gomods.io/](https://docs.gomods.io/) 是微软开源的项目，解决了 proxy 的问题。
