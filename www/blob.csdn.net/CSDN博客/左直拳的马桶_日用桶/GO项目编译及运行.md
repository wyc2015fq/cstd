# GO项目编译及运行 - 左直拳的马桶_日用桶 - CSDN博客
2018年11月20日 18:58:42[左直拳](https://me.csdn.net/leftfist)阅读数：1223
近期下载了一个名为[蓝眼云盘](https://github.com/eyebluecn/tank)的项目作为学习研究用途。这是一个服务器端用go语言开发的开源项目。
我从未用过GO语言，所以会遇到一些低级的问题要加以解决。总计解决了以下4个问题：
1、配置GOPATH
2、GOPATH分为全局GOPATH和项目GOPATH
3、下载依赖包
4、设置项目输出文件夹
我安装了GO语言环境，使用jetbrains GoLand开发工具打开项目，然后“Run”，报错了。错误定位在主函数main.go里，这一句：
```
import (
	"fmt"
	_ "github.com/go-sql-driver/mysql" <!--------------------------这里
	"log"
	"net/http"
	"tank/rest"  <!--------------------------这里
)
```
就说找不到相关的包。
我不太清楚确切的因由，像“[github.com/go-sql-driver/mysql”很明显是个包，找不到很正常，但“tank/rest”却是有的，怎么找不到呢？](http://github.com/go-sql-driver/mysql%E2%80%9D%E5%BE%88%E6%98%8E%E6%98%BE%E6%98%AF%E4%B8%AA%E5%8C%85%EF%BC%8C%E6%89%BE%E4%B8%8D%E5%88%B0%E5%BE%88%E6%AD%A3%E5%B8%B8%EF%BC%8C%E4%BD%86%E2%80%9Ctank/rest%E2%80%9D%E5%8D%B4%E6%98%AF%E6%9C%89%E7%9A%84%EF%BC%8C%E6%80%8E%E4%B9%88%E6%89%BE%E4%B8%8D%E5%88%B0%E5%91%A2%EF%BC%9F)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120181635820.png)
于是按照网上的资料，一点点设置。
**1、设置GOPATH**
这是一个系统环境变量。
当初装GO环境的时候，好像没提到这个，但似乎不妨碍，我可以正常输出“Hello World”。
hello.go
```
package main
import "fmt"
func main() {
    fmt.Println("Hello World!\r\n你好，leftfist！");
	fmt.Println("Hello Human!");
	if(3 > 4){
			fmt.Println("3 > 4");
	} else {
		fmt.Println("3 <= 4");
	}
}
```
这个GOPATH类似于Eclipse的WorkSpace，就是指向源代码所在爷爷文件夹的。为什么说是爷爷文件夹呢，是因为源代码要放在：`%GOPATH%/src/`下。比如这个蓝眼云盘项目放在一个叫“tank”的文件夹里，则它所在的位置是：`%GOPATH%/src/tank`。为啥一定要放在“src”里呢，可能这就是GO的规矩吧，所谓约定大于配置。
**2、GOPATH分为全局GOPATH和项目GOPATH**
上面说到，GOPATH指向源代码存放文件夹，但将所有的GO项目都集中于一处，好像不合适。其实，GOPATH可以分为全局GOPATH和项目GOPATH，可以分别指向不同的文件夹。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120183724362.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
这样子的话，一些公用包可以放在全局GOPATH，而私货只存放于项目GOPATH。
**3、下载依赖包**
我找了下，在goland里没有找到下载依赖包的命令。只能手动下载了。在%GOPATH%/src下，进入cmd窗口，敲入类似如下命令：
```
go get github.com/go-sql-driver/mysql
```
其实这个也可以直接在GoLand的命令行窗口里完成
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181121090736932.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181121090748126.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
**4、设置项目输出文件夹**
下载了依赖包以后，能编译了，但不能正确运行。原因是，项目需要读取一个配置文件。这是这个项目本身的逻辑，与GO环境无关。这个配置文件放在一个[/build/conf]的文件夹里，看上去，项目运行之后，根目录找不到它，就报错了。
通过设置goland的项目配置解决。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018112018534674.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181120185505962.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
将Run kind类型改为package，设置output directory到 build，可以run了。
以上都是一些操作细节。连技术细节都不算，只是操作细节，唯一的价值，在于避免遗忘。
