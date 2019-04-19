# 发布自己的pods到CocoaPods trunk 及问题记录 - 月若无涯的专栏 - CSDN博客
2015年07月08日 16:07:12[月若无涯](https://me.csdn.net/u010124617)阅读数：2401

这两天准备把之前写的一些小玩意添加到pods库中去，参考了一些资料后进行操作，实际中也遇到了一些问题，记录下来，问题及解决方式在后面。
参考内容转载如下：
首先更新了用trunk之后，CocoaPods需要0.33版本以上的，用pod
 --version查看，如果版本低，需要更新，之前有介绍更新方法。
下一步注册trunk
pod trunk register orta@cocoapods.org 'Orta Therox'
 --description='macbook air'
你注册的时候需要替换邮箱和名字，加上--verbose可以看到详细信息。
然后顺利的话你会收到一份邮件，需要点击验证。
pod trunk me//查看自己的注册信息
当然，如果你的pod是由多人维护的，你也可以添加其他维护者
pod trunk add-owner ARAnalytics kyle@cocoapods.org
创建podspec
执行命令:
pod spec create HZWebViewController
会在当前目录下生成HZWebViewController.podspec文件，然后我们编辑这个文件。
podspec文件里面有很多注释。我们看个例子
一眼看去就很明白了，不用一一解释了，当然，这里的配置项远远多于这些。
ok这里配置完成之后，需要把你的源码push到github上，tag一个版本号并且发布一个release版本，这样podspec文件中的s.source的值才能是准确的。
这些操作也不属于本文的所研究的范畴。
提交
上面的工作完成之后，我们就可以开始trunk push了
pod trunk push命令会首先验证你本地的podspec文件(是否有错误)，之后会上传spec文件到trunk，最后会将你上传的podspec文件转换为需要的json文件
第一步验证podspec文件也可以自己去做pod spec lint Peanut.podspec
成功部署之后，CocoaPods会在Twitter上@你
我们可以看看我们提交的名字为:HZWebViewController的pod
pod search HZWebViewController
我们也可以在本地的~/.cocoapods路径下看到,转换之后的json文件
实际操作中的问题：
1.pod验证本地podspec文件速度很快，使用pod trunk me查看自己的信息也能很快响应，但是当使用
pod trunk push命令时，需要非常的有耐心，因为这步的操作实在是耗时太久。
2.push成功之后，我尝试使用pod search命令查找自己的控件，发现无法搜索到，后来发现提交之后，需要使用pod
 setup命令进行库更新，实际更新时又是卡了很久，最后还是通过翻墙VPN操作成功的。期间还遇到了CocoaPods was not able to update the`master`这样的错误，最后通过删除之前的缓存rm
 -fr ~/.cocoapods/repos/master
再使用pod setup操作成功。
最后总结下操作步骤：
1.你的开源库发布之后，需要打上版本号tag，否则后续不能完成
2.注册pod trunk
pod trunk register xxx@xx'Your name' --description='macbook air'
2.cd到项目目录下，创建podspec文件
pod spec create YourPodProjectName
3.修改podspec文件中的相关信息，这很重要，校验，不能有任何警告或错误
pod spec lint YourPodProjectName.podspec
4.发布到pod trunk
pod trunk push YourPodProjectName.podspec
5.更新pod库
pod setup
之后就可以搜索到并使用了。
