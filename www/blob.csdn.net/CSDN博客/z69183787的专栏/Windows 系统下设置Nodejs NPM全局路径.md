# Windows 系统下设置Nodejs NPM全局路径 - z69183787的专栏 - CSDN博客
2015年08月25日 17:02:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4247
Windows下的Nodejs npm路径是appdata，很不爽，想改回来，但是在cmd下执行以下命令也无效
npm config set cache "D:\nodejs\node_cache"
npm config set prefix "D:\nodejs\node_global"
最后在nodejs的安装目录中找到node_modules\npm\.npmrc文件
修改如下即可：
prefix = D:\nodejs\node_global
cache = D:\nodejs\node_global
