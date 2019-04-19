# python虚拟环境迁移 - _天枢 - 博客园
## [python虚拟环境迁移](https://www.cnblogs.com/yhleng/p/9921641.html)
2018-11-07 11:41 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9921641)
python虚拟环境迁移：
注意事项：直接将虚拟环境复制到另一台机器，直接执行是会有问题的。
那么可以采用以下办法：
思路：将机器1虚拟环境下的包信息打包，之后到机器2上进行安装；
（有两种情况要考虑，机器2无网络和有网络情况(无网，证明无法在线安装包，那么就需要在机器1将包，整体打包成.whl,然后到机器2安装；有网情况，只需将导信息导出，然后到机器2上在线安装)）
机器1：
1.首先在cmd命令行下，激活虚拟环境
![](https://img2018.cnblogs.com/blog/1149221/201811/1149221-20181107114137723-689396587.png)
2.在项目目录下新建一个whls文件夹(用来存储我们依赖包)然后在虚拟环境cmd命令下切换到whls目录
执行 pip freeze --all > requirements.txt 命 令 将安装包版本信息导入到requireents.txt文件中(该文件位置在执行cmd命令当前目录下，也就是whls下); 注意--all参数，加上此参数会将setuptools、urllib3包进行打包；如果不加，这两个不会打包。
![](https://img2018.cnblogs.com/blog/1149221/201811/1149221-20181107114157488-275878289.png)
3.机器2无网的情况下，先在机器1上将包下载到whls目录下－－(有网也可以用此方法，这样可以减少在机器2下载安装包时间)
whls目录下执行虚拟环境cmd命令pip install --download . -r requirements.txt （注意--download后边的点，代表是下载到当前目录，并且requirements.txt存在于当前目录中，否则会报错）
![](https://img2018.cnblogs.com/blog/1149221/201811/1149221-20181107114217127-570117651.png)
机器2：
1.新建虚拟环境，并将项目目录放入该虚拟环境中
2.激活虚拟环境
![](https://img2018.cnblogs.com/blog/1149221/201811/1149221-20181107114242989-1708921221.png)
3.虚拟环境cmd命令进入机器1复制过来的项目下whls目录
4.执行命令安装包
前提是依赖第3步
1)有网情况通过requirements.txt包信息安装包命令如下：
pip install -r requirements.txt #安装依赖包
2)通过机器1下载的包安装；命令如下：
pip install --no-index --find-index= . -r requirements.txt 
注意命令中的.点 代表是当前目录也就是whls目录；是下载的包和
requirements.txt所在目录 
以上步骤完成后，就可以正常在机器2使用虚拟环境了。
