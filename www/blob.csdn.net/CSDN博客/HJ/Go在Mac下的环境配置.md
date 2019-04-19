# Go在Mac下的环境配置 - HJ - CSDN博客
2018年10月25日 22:47:24[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：71
Go语言环境搭建
1、更新brew  输入
```
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```
输入 brew help 进行验证–>  出现正常信息
2、更新下载go  输入 brew install go   下载安装过程中按任意键继续
3、配置GOPATH
（1）在fishshell设置GOPATH 输入命令行 set -gx GOPATH /usr/local/Cellar/go/1.7.6
（2）因为需要修改.bash_profile文件 而该文件的权限范围是 rw-r—r—，因此需要切换管理员用户，进行vim修改，添加如下内容
```
export GOPATH=/usr/local/Cellar/go/1.7.6
export GOBIN=$GOPATH/bin
export PATH=$PATH:$GOBIN
```
修改后输入source .bash_profile立即生效    输入 go version 进行验证 —>  出现版本信息
