# gometalinter-golang代码质量检查分析工具 - weixin_33985507的博客 - CSDN博客
2018年03月06日 11:22:49[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
## 前言
最近在研究代码质量检测和分析这一个环节，当然代码质量分析是devops中持续集成部分非常重要的一个环节。涉及到团队协作的时候，很多公司会有自己的一套规则，最熟悉的是阿里巴巴的java代码参考手册，专家总结，大家按照规则去写代码。但是对于devops，有了规则远远不够，还需要提高代码检查的自动化程度，以及与其他的环节合作衔接上。
那么如果没有代码质量把控这个环节，那么可能带来以下问题：
- 抽象不够，大量重复的代码。
- 变量命名各种风格。驼峰派，下划线派等。
- if else 嵌套太深。
- 不写注释，或是注释写的不合格。
- 函数里代码太长
- 潜在的bug，这就是各种语言的一些语法上的检查了。
最终导致维护困难。这就是程序员经常所说的前任留下的坑，往往结果就是新的接任者选择重构。其实对单位也是一直财力和人力的浪费。另外，更无法满足devops的快速上线，快速诊断，快速迭代的目标。
## SonarQube 介绍
SonarQube 是一个开源的代码分析平台, 用来持续分析和评测项目源代码的质量。 通过SonarQube我们可以检测出项目中重复代码， 潜在bug， 代码风格问题，缺乏单元测试等问题， 并通过一个web ui展示出来。
![图片描述](https://image-static.segmentfault.com/191/421/1914216671-5a9e42e62c58b_articlex)
SonarQube在devops领域很容易与各种ci各种集成。不过这是我们以后会谈到的问题。
支持不少主流语言，但是没有golang。所以引入了下面的话题。
## golang代码质量检查分析工具
### gometalinter 简介
该工具基本上集成了目前市场上所有的检测工具，然后可以并发的帮你静态分析你的代码
```
deadcode
  dupl
  errcheck
  gas
  goconst
  gocyclo
  goimports
  golint
  gosimple
  gotype
  gotypex
  ineffassign
  interfacer
  lll
  maligned
  megacheck
  misspell
  nakedret
  safesql
  staticcheck
  structcheck
  unconvert
  unparam
  unused
  varcheck
  vet
```
### gometalinter安装
```
go get github.com/alecthomas/gometalinter
gometalinter --install --update
```
执行上面的两个命令即可。安装非常简单。
### gometalinter 的使用
cd 到go项目下，执行 gometalinter ./...
即检查所有目录的go文件，此时vendor目录下的也会检测
如果是想指定指定目录，执行gometalinter + 文件夹名。
```
bogon:telegraf gaohj$ gometalinter web
web/status.go:165::warning: Errors unhandled.,LOW,HIGH (gas)
web/status.go:165:10:warning: error return value not checked (w.Write([]byte("welcome telegraf for rc"))) (errcheck)
web/status.go:212:19:warning: w can be io.Writer (interfacer)
web/status.go:205:25:warning: do not pass a nil Context, even if a function permits it; pass context.TODO if you are unsure about which Context to use (SA1012) (megacheck)
web/status.go:205:2:warning: 'if err != nil { return err }; return nil' can be simplified to 'return err' (S1013) (megacheck)
```
### vscode集成gometalinter
vscode 默认使用的是golint，如果想用gometalinter替换golint，直接打开
设置项，
在用户设置里添加"go.lintTool": "gometalinter"即可。
![图片描述](https://image-static.segmentfault.com/170/735/1707350997-5a9e4790d8ff9_articlex)
## 总结
其实接下来会做：
- gometalinter与SonarQube的配合,可以参考该[sonar-golang](https://github.com/uartois/sonar-golang)项目。
- SonarQube与CI工具的配合，从而构建devops的整个流程。
