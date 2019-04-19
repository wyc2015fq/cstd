# Golang 环境配置建议（Atom） - =朝晖= - 博客园
# [Golang 环境配置建议（Atom）](https://www.cnblogs.com/dhcn/p/7121494.html)
http://www.philo.top/2015/02/06/golang-%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE%E5%BB%BA%E8%AE%AE/
## 开发环境的必要特征
- 项目管理
- 快速文件跳转
- 自动语法检查
- 自动补全
- 查找定义
- 启动速度快
- 如果自己有需求的话插件可以随便写
- 灵活的运行配置
### (project,lint,hint,autocomplete)
要实现上面几点我们来一步一步的配置。
## 我的大环境
- OSX
- GO 1.4
- HomeBrew
## go 环境安装
这一部分是最重要的，如果没有它，每次build的时候出现 too many errors 心里真的是非常难过的。
- 
环境配置：（golint,gooracle,mercurial）
- 安装mercurial: brew install mercurial > 这个东西是用来做版本管理的，也是下载代码的工具类似git，貌似google的项目用的挺多的。
- 安装golint： `shell $ go get github.com/golang/lint $ go install github.com/golang/lint`
- 安装gooracle `shell go get code.google.com/p/go.tools/cmd/oracle`
- 安装goimport `shell go get golang.org/x/tools/cmd/goimports`
- 安装gocode `shell go get -u github.com/nsf/gocode`
- 安装 godef `shell go get -v code.google.com/p/rog-go/exp/cmd/godef go install -v code.google.com/p/rog-go/exp/cmd/godef`
- 
安装环境的时候经常会出现下载不下来的问题，大概是我网络不好吧。连接google经常出现问题。 > 解决方案： > golang中国的下载频道中有一个第三方包的下载工具，只要输入地址之后人家会给你提供下载tar包的。 > 放到gopath中就ok了。 > 此步骤只能替代[Go](http://lib.csdn.net/base/go) get的步骤，最后还是需要[go](http://lib.csdn.net/base/go) install
- 
go install 之后会在 `$GOPATH/bin/` 中出现各种工具文件
- gocode 提供代码补全
- godef 代码跳转
- gofmt 自动代码整理
- golint 代码语法检查
- goimports 自动整理imports
- oracle 代码callgraph查询（plugin中还在todolist中，但是不配置一直报错。实在烦。）
> 
最后不要忘记复制上面的命令到 $GOROOT/bin/下面
- 最后的配置结果(安装完之后 Package->Go Plus->Display Go Information) `Cover Tool: /usr/local/go/pkg/tool/darwin_amd64/cover Vet Tool: /usr/local/go/pkg/tool/darwin_amd64/vet Format Tool: /Users/Li-jianying/git/bin/goimports Lint Tool: /usr/local/go/bin/golint Gocode Tool: /Users/Li-jianying/git/bin/gocode Gocode Status: Enabled Oracle Tool: /Users/Li-jianying/git/bin/oracle Git: /usr/bin/git Mercurial: /usr/local/Cellar/mercurial/3.2.1/bin/hg PATH: /usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/go/bin `没有红色的行就没有问题了。
## 项目管理，自动文件跳转
- 插件：project-manager >这个插件是非常hot的，在atom.io的首页上排名还是非常靠前的。
- 使用细节：- 配合tree view `（ cmd - \）` 打开关闭project file
- 搜索项目文件 `（ cmd -t ）` > 输入的时候按照顺序输入关键字 > 比如说 user模块的controll中的go源代码 > 搜索的时候可以使用多个快捷键 =》 user controll go > 就可以准确的找到源代码了
- 打开项目列表 `（ ctrl-cmd-p ）` > 然后就会显示出来一个项目列表回车就可以打开项目。
- 
总结： > 感觉这个插件的treeview用途的确不是很大，因为正常项目的项目文件会很多。 > 不过是快速打开项目（文件夹）是非常方便的，配合快速打开文件还是相当爽的。
- 
按快捷键之后的效果展示： ![](http://7viiaq.com1.z0.glb.clouddn.com/pm.png)
## 自动语法检，自动补全
- 
插件go-plus,autocomplete-plus(dependance),gocode 使用 apm install 安装就可以了。速度还可以。
- 
使用细节 在保存的时候
- 会自动补上imports
- 会自动整理代码 > 比如说 a:=5 会变成 a := 5 两个空行自动变成一个空格
- lint自动检查语法错误（go build 的错误都会显示出来）
- vet会自动检查语法拼写建议 > 函数，结构体 会提示补上注释。 比如说会提示你函数前面需要加入godoc类型的注释： // functionname 注释的第一行需要使用双斜杠注释+空格+函数名+空格+简短的函数功能说明。
- 
使用细节： 在编辑的时候
- go-plus会配合autocomplete-plus会自动补全包名字（tab补全，回车不行，这个真心舒服）
- 包名.之后输入方法名开头几个字母会给出补全建议。
- 
安装之后的演示（类似go-plus官方演示） ![](https://camo.githubusercontent.com/dfd447388f9d6506dcdd19e6c5c431bf4211e673/687474703a2f2f636c2e6c792f696d6167652f3339327a324c3066304534312f676f2d706c75732d6578616d706c652e676966)
- 
gocode 插件自动补全演示
- 这个插件继承自autocomplete,因为年久失修。api用的是老版本，所以一直都会出现api警告。（我是实用主义，视而不见）
- gocode如果找不到就进入代码找到文件autocomplete-view.coffee 中105行写成绝对路径就肯定没有问题了。
- 绑定快捷键，因为这个插件没有绑定快捷键的文件（keymap）所以需要在keymap.cson下自己手动绑定配置如下：`json '.active.pane': 'ctrl-;': 'gocode:toggle'`
- 
效果演示![](http://7viiaq.com1.z0.glb.clouddn.com/QQ20150208-1@2x.png)
## 查找定义
- 
插件 godef
- 
使用细节: 光标在目标代码上的时候，使用快捷键`ctrl-k`跳转到目标代码的定义代码上。
## 灵活的运行
- 安装插件atom-runner
- 修改源代码中的go run运行为`go: 'sh [绝对路径到你的全局run脚上] '+atom.project.rootDirectories[0].path`
- 脚本的 $1 会是你项目运行的路径。然后你的所有项目都可以添加一个run.sh 来指定一个构建运行的方案。
- 做完之后不管当前在查看什么文件都可以运行整个项目。
- 其他语言也可以这样做。毕竟单个文件的项目不是很常见（开发的时候）
## 总结
- 我这里只是总结了我用的所有的golang相关的atom插件。
- 要相信golang使用编辑器就够用的事实了。因为 `go tool` 实在是太好用了。
### 我的key map
- `cmd-d` duplicate line
- `cmd-w` 关闭标签
- `cmd-r` 文件内的标签列表
- `cmd-e` 使用选中的内容做搜索的pattern
- `shift-cmd-f` project内部搜索，这个非常实用。command 配置(keymap.cson) >`json '.platform-darwin atom-text-editor': 'shift-cmd-D': 'find-and-replace:select-next'`
## 感谢golang
- golang能有如此的开发体验，以及在如此低的版本中表现的这么好。实在是大牛赐予我们最好的礼物了。
- golang的有如此好的开发工具加上各种扩展性比较强的编辑器（vim,emacs,sublime,atom等）开发体验的确是非常好的。
- 

