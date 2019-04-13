
# go 开发环境安装教程 windows - 小小情意 - 博客园






# [go 开发环境安装教程 windows](https://www.cnblogs.com/xiaoxiaoqingyi/p/7040761.html)
首先进入go 语言官网下载最新安装包，我目前安装的版本是 1.8.3版本：go1.8.3.windows-amd64.msi 如果下载慢，这个是百度云地址：[https://pan.baidu.com/s/1c10YXf6](https://pan.baidu.com/s/1c10YXf6)
点击msi文件直接安装，根据安装提示直接安装，默认安装路径为 C:\Go.
安装完毕后，配置系统环境变量，在Path变量中 添加 “;C:\Go\bin” 记得路径前 加 ; 分号，作为路径分隔。
![](https://images2015.cnblogs.com/blog/804587/201706/804587-20170617170318509-1996583126.png)

配置完后，检验是否配置成功，你进入 运行 => cmd , 输入 go -help 得到如下图：
![](https://images2015.cnblogs.com/blog/804587/201706/804587-20170617170350446-1127529901.png)
说明你配置成功了。
下面新建一个简单的Go项目，hello word!
我创建了 F:\GoProject\hello 目录（你可以根据你的情况，创建你自己的工作目录），然后在该目录下创建一个 hello.go 文件，文件内容为：
package main
import "fmt"
func main() {
    fmt.Printf("hello, world\n")
}
然后你进入 cmd 命令行执行这个文件：
C:\Users\Administrator>f:
F:\>cd F:\GoProject\hello
F:\GoProject\hello>go build
![](https://images2015.cnblogs.com/blog/804587/201706/804587-20170617170515353-1076202514.png)
完成后，在hello.go 旁边出现一个 hello,exe 运行文件，记住不是双击执行它，而是
输入 hello 得到如下： hello world!
![](https://images2015.cnblogs.com/blog/804587/201706/804587-20170617170643196-2049887979.png)
那就大功告成了！
更多信息请参考go 语言官网：[https://golang.org/](https://golang.org/)







