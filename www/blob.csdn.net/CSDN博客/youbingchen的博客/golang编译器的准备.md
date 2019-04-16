# golang编译器的准备 - youbingchen的博客 - CSDN博客





2016年08月01日 13:41:40[youbingchen](https://me.csdn.net/youbingchen)阅读数：586








> 
最近编程喜欢上用sublime text3,因为简洁加上语法高亮，所以很适合我个人风格，今天我就针对sublime来配置写go语言的环境


# 安装go环境
- 1.去[go官方](https://golang.org/dl/)下载适合你电脑环境版本
- 2：新建文件项目文件夹存放(比如:E:/goProject) 

  分别建立 bin src pkg 子文件夹
- 3:配置环境变量 

 GOROOT（go安装的路径） 

 GOPATH （上面第二步建立的文件夹 ）
# 安装sublime Text
- - 去[Sublime Text官网](http://www.sublimetext.com/)下载sublime text

- - 安装package-ctrl 

在view->show console下输入

` import urllib.request,os; pf = 'Package Control.sublime-package'; ipp = sublime.installed_packages_path(); urllib.request.install_opener( urllib.request.build_opener( urllib.request.ProxyHandler()) ); open(os.path.join(ipp, pf), 'wb').write(urllib.request.urlopen( 'http://sublime.wbond.net/' + pf.replace(' ','%20')).read())`
回车，重启sublime text 

+ 3.安装GoSublime、SidebarEnhancements

> 
```
按Ctrl +shift+P 打开Package Ctrl 输入install按回车，会进入安装页面。输入GoSublime 找到后回车 安装。(有的速度慢,要等到弹出这个输入框再搜索)
```


# 安装gocode

> 
 这个需要git环境的支持，必须先安装git环境

  打开cmd，输入以下内容


```
go get -u github.com/nsf/gocode

go install github.com/nsf/gocode
```

# 自动运行go

Tools->Bulid  System->New Bulid System 

修改文本 

{ 

    “shell_cmd”: “D:\programfiles\Go\bin run $file”, 

    “encoding”:”utf-8” 

} 

保存为 GoBuild.submlime-build 

然后 Tools->Build System 选中GoBuild
> 
测试安装是否成功


如果你想使用vim编辑的话：可以采用这篇博客进行配置[Golang开发环境搭建-Vim篇](http://tonybai.com/2014/11/07/golang-development-environment-for-vim/)

c





