# golang.org/x/*无法访问的解决方案 - walkabc - 博客园
# [golang.org/x/*无法访问的解决方案](https://www.cnblogs.com/hitfire/articles/6410486.html)
　　因为天朝防火墙的缘故，不连接VPN时，golang.org这个域名在国内是访问不到的。
　　导致我们使用golang时，有些依赖包下载不了，好在现在，golang把这些官方依赖包都放在了github上面。
　　对应的github地址是:https://github.com/golang
　　解决方法就是把github上这些项目都一个个git clone 下来，然后放在$GOPATH/golang/x下面，这样就可以直接使用了。
　　我现在使用govendor来管理项目依赖库关系。
　　govendor add golang.org/x/^ 使用这个命令就是可以把golang.org/x中的所有代码都加到项目本身的vendor文件夹下面。
　　这里加一个小技巧,govendor fetch时，有些包也是依赖golang.org的，只要把对应的包从github上下载下来放进$GOPATH下面即可。

