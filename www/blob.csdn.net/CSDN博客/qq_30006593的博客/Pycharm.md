# Pycharm - qq_30006593的博客 - CSDN博客





2017年10月18日 15:11:11[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：610







- can’t get remote credentials for deployment server:是因为deploy只对该项目可见。
- 使用deployment程序跑的慢（也不慢），反向更新不易。图片得配置环境变量DISPLAY=localhost:15.0。
- 直接远程打开pycharm慢， windows挺快的。。。。。
- teamviewer+github，提交麻烦

# Basic

## project files

A project directory is marked with /help/img/idea/2017.2/project_icon.png icon. 

Such project directory contains the .idea directory, with the following files:
- *.iml file that describes the project structure
- workspace.xml file that contains your workspace preferences.
- A number of .xml files. Each .xml file is responsible for its own set of settings

All the settings files in the .idea directory should be put under version control except the workspace.xml, which stores your local preferences. The workspace.xml file should be marked as ignored by VCS.

## content root

The folder which is the highest in this hierarchy is called content root folder or content root (shown as /help/img/idea/2017.2/folder.png) for short. A project has at least one content root folder, by default it is the project folder itself.

#### Creating a content root
- Open the Project Structure settings.
- In the Projects pane of the Project Structure page, click the project you want to configure content roots for.
- In the Content roots of the Project Structure page, click the Add Content Root button /help/img/idea/2017.2/addRoot.png.
- In the dialog that opens, locate the desired directory and click OK.

# Deployment

## basic
- An in-place server is a server whose document root is the parent of the project root, either immediate or not. In other words, the Web server is running on your computer, your project is under its document root, and you do your development directly on the server.
- A local server is a server that is running in a local or a mounted folder and whose document root is NOT the parent of the project root.
- A remote server is a server on another computer (remote host).

## [deployment in pycharm](https://www.jetbrains.com/help/pycharm/deployment-in-pycharm.html?search=deploy)



