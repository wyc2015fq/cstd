# idea go run 报错: undefined: xxxx - weixin_33985507的博客 - CSDN博客
2017年06月27日 11:28:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
一如既往的喜欢用idea的 "Run", 但是在某个项目中竟然跑不起来了.
```
# command-line-arguments
.\main.go:253: undefined: service
```
然后我又在命令行了试了一下 go build, 发现是正常的, 奇怪了, 虽然能build在执行, 但是没idea中run来得方便啊, 不能忍, 所以开始研究之旅 .
首先 我的项目结构是这样的:
![3447621-e30efda5ac5d960e.png](https://upload-images.jianshu.io/upload_images/3447621-e30efda5ac5d960e.png)
Paste_Image.png
## 看 go help
第一怀疑的是自己用法不对, 敲 go help run
```
go help run
usage: go run [build flags] [-exec xprog] gofiles... [arguments...]
...
For more about build flags, see 'go help build'.
```
首先想到的是 build flags参数, 是不是有什么其他的参数没配置对, 先看看参数的使用吧.
```
go help build
The build flags are shared by the build, clean, get, install, list, run,
and test commands:
         -n
                print the commands but do not run them.
...(省略1w字)
```
好像参数都和那undefined无关, 没办法了, -n看看run和build的commands有什么不同, 导致了一个正确一个错误.
**go build -n**(只截取了重要部分)
```
"D:\\dev\\Go\\pkg\\tool\\windows_amd64\\compile.exe" -o "$WORK\\room.a" -trimpath $WORK -p main -complete -buildid 9818a29629bab0107dd87d443f6410ca199b11b0 -importmap github.com/Shopify/sarama=vendor/github.com/Shopify/sarama -im
portmap github.com/Sirupsen/logrus=vendor/github.com/Sirupsen/logrus -importmap github.com/doublemo/koala/cluster=vendor/github.com/doublemo/koala/cluster -importmap github.com/doublemo/koala/db/mongodb=vendor/github.com/doublemo
/koala/db/mongodb -importmap github.com/doublemo/koala/helper=vendor/github.com/doublemo/koala/helper -importmap github.com/doublemo/koala/kafka=vendor/github.com/doublemo/koala/kafka -importmap github.com/doublemo/koala/kafka/ho
ok=vendor/github.com/doublemo/koala/kafka/hook -importmap github.com/doublemo/koala/os/stats=vendor/github.com/doublemo/koala/os/stats -importmap github.com/doublemo/koala/svc=vendor/github.com/doublemo/koala/svc -importmap golan
g.org/x/net/context=vendor/golang.org/x/net/context -importmap google.golang.org/grpc=vendor/google.golang.org/grpc -importmap gopkg.in/urfave/cli.v2=vendor/gopkg.in/urfave/cli.v2 -importmap gopkg.in/urfave/cli.v2/altsrc=vendor/g
opkg.in/urfave/cli.v2/altsrc -D _/E_/go_project/koal/src/room -I $WORK -I "E:\\go_project\\koal\\pkg\\windows_amd64" -pack "E:\\go_project\\koal\\src\\room\\main.go" "E:\\go_project\\koal\\src\\room\\service.go"
```
**go run -n main.go**
```
"D:\\dev\\Go\\pkg\\tool\\windows_amd64\\compile.exe" -o "$WORK\\command-line-arguments.a" -trimpath $WORK -p main -complete -buildid 2c633397042ad253e002d08263af62f3d95c0754 -importmap github.com/Shopify/sarama=vendor/github.com/
Shopify/sarama -importmap github.com/Sirupsen/logrus=vendor/github.com/Sirupsen/logrus -importmap github.com/doublemo/koala/cluster=vendor/github.com/doublemo/koala/cluster -importmap github.com/doublemo/koala/db/mongodb=vendor/g
ithub.com/doublemo/koala/db/mongodb -importmap github.com/doublemo/koala/kafka=vendor/github.com/doublemo/koala/kafka -importmap github.com/doublemo/koala/kafka/hook=vendor/github.com/doublemo/koala/kafka/hook -importmap github.c
om/doublemo/koala/os/stats=vendor/github.com/doublemo/koala/os/stats -importmap github.com/doublemo/koala/svc=vendor/github.com/doublemo/koala/svc -importmap google.golang.org/grpc=vendor/google.golang.org/grpc -importmap gopkg.i
n/urfave/cli.v2=vendor/gopkg.in/urfave/cli.v2 -importmap gopkg.in/urfave/cli.v2/altsrc=vendor/gopkg.in/urfave/cli.v2/altsrc -D _/E_/go_project/koal/src/room -I $WORK -I "E:\\go_project\\koal\\pkg\\windows_amd64" -pack "E:\\go_pro
ject\\koal\\src\\room\\main.go"
```
我细(cou)心(qiao)的发现了最后一句话的不同. build -pack了两个文件, run 只-pack了一个文件, 而报错的地方正是run没有-pack进来的文件, 那么真相只有一个 : 那就是run没有-pack进来package(main) 的所有文件, 导致直接go run main.go, 找不到其他文件下的东西. 问题找到了, 怎么解决呢?
## 再看go help
那么 怎么能让go run包含多个文件进来呢?
```
go help run
usage: go run [build flags] [-exec xprog] gofiles... [arguments...]
```
看到gofiles...了吗, 看到熟悉的...了吗?  是的, go run可以传多个文件 如: go run a.go b.go c.go. 打开命令行, 一试 好了 ! 好像又离解决问题近了一步.
> 
小插曲是我天真的试了下 `go run *.go` .
## 修改idea run配置
我是相信idea的强大的, run多个文件的问题它肯定能解决;
首先看看run配置
![3447621-021c2ee90d670799.png](https://upload-images.jianshu.io/upload_images/3447621-021c2ee90d670799.png)
go run main.go
这里要是能配置多个文件应该就可以, 然后并不能.
那算了, 删了再来, 这一次我们build and run
![3447621-dae6f40764686f91.png](https://upload-images.jianshu.io/upload_images/3447621-dae6f40764686f91.png)
build and run
然后点击
![3447621-54a9feb979352ed2.png](https://upload-images.jianshu.io/upload_images/3447621-54a9feb979352ed2.png)
edit
![3447621-f5d816efba8a59ff.png](https://upload-images.jianshu.io/upload_images/3447621-f5d816efba8a59ff.png)
Paste_Image.png
依然有一个file选项, 依然是不能多选的...
不过上面还有一个Run kind, 点一下出现了package: 运行整个包,而不是单一个文件?  好像有希望, 输入room, 试一试, 果然跑起来了 .
