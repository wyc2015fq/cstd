# 在linux系统中安装VSCode(Visual Studio Code) - DoubleLi - 博客园






在linux系统中安装VSCode(Visual Studio Code)

## 1.从官网下载压缩包(话说下载下来解压就直接可以运行了咧,都不需要make)

访问Visual Studio Code官网 [https://code.visualstudio.com/docs?dv=linux64](https://code.visualstudio.com/docs?dv=linux64)

我是64位的:

wget https://az764295.vo.msecnd.net/stable/7ba55c5860b152d999dda59393ca3ebeb1b5c85f/code-stable-code_1.7.2-1479766213_amd64.tar.gz

## 2.解压 ,如果文件名不对,可能解压不出来的(扩展名:tar.gz)

tar jxcv code-stable-code_1.7.2-1479766213_amd64.tar.gz

## 3.然后移动到 /usr/local/ 目录

mv VSCode-linux-x64 /usr/local/

## 4.可能还需要给可执行的权限, 然后就已经可以运行了

chmod +x /usr/local/VSCode-linux-x64/code

## 5.复制一个VScode图标文件到 /usr/share/icons/ 目录(后面会有用)

cp /usr/local/VSCode-linux-x64/resources/app/resources/linux/code.png /usr/share/icons/

## 6.创建启动器, 在/usr/share/applications/ 目录, 也可以将它复制到桌面目录

直接在中断 使用 命令: 

vim /usr/share/applications/VSCode.desktop

然后输入以下文本:

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

[Desktop Entry]
Name=Visual Studio Code
Comment=Multi-platform code editor for Linux
Exec=/usr/local/VSCode-linux-x64/code
Icon=/usr/share/icons/code.png
Type=Application
StartupNotify=true
Categories=TextEditor;Development;Utility;
MimeType=text/plain;

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

保存后退出, 然后可以复制到桌面:

cp /usr/share/applications/VSCode.desktop ~/桌面/

之后 就会发现 桌面和 应用程序菜单都有了 VSCode的快捷方式了

## 8.打开VSCode, 加载插件: cpptools | vscode-icons

![](https://images2015.cnblogs.com/blog/391071/201612/391071-20161208164235038-70377102.png)

OK, 其他插件自己看着装吧,^,^

 不过,在虚拟机中 VSCode 启动有点慢哟~



*参考:http://os.51cto.com/art/201509/490032.htm*











