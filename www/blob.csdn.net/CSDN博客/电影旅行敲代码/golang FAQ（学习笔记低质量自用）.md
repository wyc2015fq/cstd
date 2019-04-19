# golang FAQ（学习笔记低质量自用） - 电影旅行敲代码 - CSDN博客
2019年03月14日 16:11:06[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：44
|序号|问题|解答|
|----|----|----|
|1|为什么要有$GOPATH|done|
|2|go build 和 go install 有什么区别|done|
|3|import语句是什么含义||
|4|go是如何进行编译的||
|5|什么是module||
|6|module是如何实现的||
|7|什么是go-gettable||
|8|为什么要有module||
|9|go module和GOPATH是什么关系（为什么说有了go module就不需要GOPATH了）||
|10|什么是go module Notary||
|11|使用empty interface作为参数类型||
|12|go closure||
## 1. 为什么要有$GOPATH
为什么要有`$GOPATH`，在一个21世纪的语言上设计一个这样老旧的概念，go的设计目标不是fancy，是简单的令人发指的好用。By the way，如果你看过go的实现，你会发现go的实现也是简单到令人发指，AST node能省就省。
> 
The goal of the GOPATH is to centralize all packages into one common workspace. It is not really a new concept by itself (think of the Java Classpath for example), but Go’s use is drastically simpler by not supporting packages versioning.
> 
How you handle different projects is up to you. The Go way is to put every project as a package in the $GOPATH/src directory and do everything from there. As I don’t really like it, I defined my  GOPATH to be $HOME/.go. Then I put each project in a dedicated directory somewhere else (anywhere in my computer), and symlink the project directory into my $GOPATH/src dir.
*注：quote from [Can someone explain why GOPATH is convenient and how it should be used in general?](https://stackoverflow.com/questions/24306183/can-someone-explain-why-gopath-is-convenient-and-how-it-should-be-used-in-genera)*
总而言之，`$GOPATH`就是提供一个统一的**workspace**，让你组织你的package。`$GOPATH`不是孤立存在的，是和import机制相互结合的产物。
## 2. go build和go install有什么区别
首先`go build`和`go install`都会对源码进行编译。
For packages
- `go build`, builds your package then discards the results.
- `go install`, builds then installs the package in your `$GOPATH/pkg` directory.
For commands(package main)
- `go build`  builds the command and leaves the result in the current working directory.
- `go install` builds the command in a temporary directory then moves it to `$GOPATH/bin`.
Whenever the go tool installs a package or binary, it also installs whatever dependencies it has, so running go install will also install packages your program depends on (publicly available, “go gettable” packages), automatically.
*Note: 上面的内容来自于[What does go build build?](https://stackoverflow.com/questions/30612611/what-does-go-build-build)*
## 3. import语句是什么含义
在搞清楚import语句之前，需要搞清楚import后面紧跟的import path字符串是什么意思，**package名字**？**相对路径**？**URL**？
An import path (see ‘go help packages’) denotes a package stored **in the local file system**. In general, an import path denotes either a standard package (such as “unicode/utf8”) or a package found in one of the work spaces (For more details see: ‘go help gopath’).
首先需要明确的是`go import path`表示的是**存储在本地文件系统中的package**，无论这些package是怎么来的，最终go在执行的时候需要确保这些package都必须在本地。import path分为两种，一种是standard package，另外一种是需要在gopath中寻找。
那么这些go package是如何得到的，go build什么的并不关心，who cares，只要这些pacakge在$GOPATH/src下，我能找到就行了。
而这些package的download则是`go get`的任务。
*注：上面的描述来自于官方的[Import path syntax](https://golang.org/cmd/go/#hdr-Import_path_syntax)*
### go get
> 
**When asked to fetch a package, go get does a number of checks. If the target is on a known hosting site, it fetches the data using a method that is hardcoded (git for [github.com](http://github.com), hg for bitbucket, …).**
known hosting site可以在go/src/cmd/go/internal/get/vcs.go找到，恕我直言这是一个和GOPATH同样傻逼的设定，但是简单粗暴又好用。
```
// vcsPaths defines the meaning of import paths referring to
// commonly-used VCS hosting sites (github.com/user/dir)
// and import paths referring to a fully-qualified importPath
// containing a VCS type (foo.com/repo.git/dir)
var vcsPaths = []*vcsPath{
	// Github
	{
		prefix: "github.com/",
		regexp: lazyregexp.New(`^(?P<root>github\.com/[A-Za-z0-9_.\-]+/[A-Za-z0-9_.\-]+)(/[\p{L}0-9_.\-]+)*$`),
		vcs:    "git",
		repo:   "https://{root}",
		check:  noVCSSuffix,
	},
	// Bitbucket
	{
		prefix: "bitbucket.org/",
		regexp: lazyregexp.New(`^(?P<root>bitbucket\.org/(?P<bitname>[A-Za-z0-9_.\-]+/[A-Za-z0-9_.\-]+))(/[A-Za-z0-9_.\-]+)*$`),
		repo:   "https://{root}",
		check:  bitbucketVCS,
	},
	// IBM DevOps Services (JazzHub)
	{
		prefix: "hub.jazz.net/git/",
		regexp: lazyregexp.New(`^(?P<root>hub\.jazz\.net/git/[a-z0-9]+/[A-Za-z0-9_.\-]+)(/[A-Za-z0-9_.\-]+)*$`),
		vcs:    "git",
		repo:   "https://{root}",
		check:  noVCSSuffix,
	},
	// Git at Apache
	{
		prefix: "git.apache.org/",
		regexp: lazyregexp.New(`^(?P<root>git\.apache\.org/[a-z0-9_.\-]+\.git)(/[A-Za-z0-9_.\-]+)*$`),
		vcs:    "git",
		repo:   "https://{root}",
	},
	// Git at OpenStack
	{
		prefix: "git.openstack.org/",
		regexp: lazyregexp.New(`^(?P<root>git\.openstack\.org/[A-Za-z0-9_.\-]+/[A-Za-z0-9_.\-]+)(\.git)?(/[A-Za-z0-9_.\-]+)*$`),
		vcs:    "git",
		repo:   "https://{root}",
	},
	// chiselapp.com for fossil
	{
		prefix: "chiselapp.com/",
		regexp: lazyregexp.New(`^(?P<root>chiselapp\.com/user/[A-Za-z0-9]+/repository/[A-Za-z0-9_.\-]+)$`),
		vcs:    "fossil",
		repo:   "https://{root}",
	},
	// General syntax for any server.
	// Must be last.
	{
		regexp: lazyregexp.New(`(?P<root>(?P<repo>([a-z0-9.\-]+\.)+[a-z0-9.\-]+(:[0-9]+)?(/~?[A-Za-z0-9_.\-]+)+?)\.(?P<vcs>bzr|fossil|git|hg|svn))(/~?[A-Za-z0-9_.\-]+)*$`),
		ping:   true,
	},
}
```
*注：quote from [Hosting Go code on Github with a custom import path](https://jve.linuxwall.info/blog/index.php?post/2015/08/26/Hosting_Go_code_on_Github_with_custom_import_path)*
文章《[Vanity Go Import Paths](https://blog.bramp.net/post/2017/10/02/vanity-go-import-paths/)》中揭示了go get的来龙去脉。官方
> 
For code hosted on other servers, import paths may either be qualified with the version control type, or the go tool can dynamically fetch the import path over https/http and discover where the code resides from a <meta> tag in the HTML. [<remote import path](https://golang.org/cmd/go/#hdr-Remote_import_paths)>
其中go get需要从网上download的package对应的是remote import path，或者从知名的host中下载，例如github，bitbucket。或者通过读取HTML中的<meta> tag来获知从哪里下载这些package。
## 4. go是如何进行编译的
### 编译
### 链接
#### 什么时候进行type check
#### 什么时候进行链接
## 5. 什么是module
关于go module最好的资料是https://github.com/golang/go/wiki/Modules。
## 6. module是如何实现的
require 后面的version号是如何得到的，例如
[https://github.com/golang/lint/commit/d0100b6bd8b389f0385611eb39152c4d7c3a7905](https://github.com/golang/lint/commit/d0100b6bd8b389f0385611eb39152c4d7c3a7905)
### semantic import versioning
## 7. 什么是go-gettable
[https://go.googlesource.com/proposal/+/master/design/25530-notary.md](https://go.googlesource.com/proposal/+/master/design/25530-notary.md)
[https://medium.com/golang-learn/go-project-layout-e5213cdcfaa2](https://medium.com/golang-learn/go-project-layout-e5213cdcfaa2)
## 8. 为什么要有module
### go module的问题
例如https://github.com/golang/lint/issues/436
## 9. go module和$GOPATH是关系
为什么有人说有了go module就需要GOPATH了。
## 10. 什么是go module Notary
## 11. 使用empty interface作为参数类型
## 13. Go closure
[1] [https://blog.bramp.net/post/2017/10/02/vanity-go-import-paths/](https://blog.bramp.net/post/2017/10/02/vanity-go-import-paths/)
[2] [https://golang.org/cmd/go/#hdr-Import_path_syntax](https://golang.org/cmd/go/#hdr-Import_path_syntax)
[3] [https://jve.linuxwall.info/blog/index.php?post/2015/08/26/Hosting_Go_code_on_Github_with_custom_import_path](https://jve.linuxwall.info/blog/index.php?post/2015/08/26/Hosting_Go_code_on_Github_with_custom_import_path)
