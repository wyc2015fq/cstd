
# linux 更新yum源 改成阿里云源 - 致林 - 博客园






# [linux 更新yum源 改成阿里云源](https://www.cnblogs.com/bincoding/p/7892762.html)

### 1、备份
`mv /etc/yum.repos.d/CentOS-Base.repo /etc/yum.repos.d/CentOS-Base.repo.backup`
### 2、下载新的CentOS-Base.repo 到/etc/yum.repos.d/

#### CentOS 5
`wget -O /etc/yum.repos.d/CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-5.repo`
`或者`
`curl -o /etc/yum.repos.d/CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-5.repo`

#### CentOS 6
`wget -O /etc/yum.repos.d/CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-6.repo`
`或者`
`curl -o /etc/yum.repos.d/CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-6.repo`

#### CentOS 7
`wget -O /etc/yum.repos.d/CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-7.repo`
`或者`
`curl -o /etc/yum.repos.d/CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-7.repo`

### 3、之后运行yum makecache生成缓存





