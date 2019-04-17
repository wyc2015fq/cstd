# Shell实现Docker环境检测和安装 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年10月11日 13:27:26[boonya](https://me.csdn.net/boonya)阅读数：369








shell脚本执行的是bash语法，这里定一个docker_install函数:

```bash
function docker_install()
{
	echo "检查Docker......"
	docker -v
    if [ $? -eq  0 ]; then
        echo "检查到Docker已安装!"
    else
    	echo "安装docker环境..."
        curl -sSL https://get.daocloud.io/docker | sh
        echo "安装docker环境...安装完成!"
    fi
    # 创建公用网络==bridge模式
    #docker network create share_network
}
```

编写标准的bash shell脚本：

```bash
#!/bin/bash
#file:docker_install.sh

function docker_install()
{
	echo "检查Docker......"
	docker -v
    if [ $? -eq  0 ]; then
        echo "检查到Docker已安装!"
    else
    	echo "安装docker环境..."
        curl -sSL https://get.daocloud.io/docker | sh
        echo "安装docker环境...安装完成!"
    fi
    # 创建公用网络==bridge模式
    #docker network create share_network
}

# 执行函数
docker_install
```

 这样很简单就实现了docker的环境安装。



