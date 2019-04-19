# 开源客服系统 ppmessage - 三少GG - CSDN博客
2016年10月03日 14:38:02[三少GG](https://me.csdn.net/scut1135)阅读数：5338

**不用docker部署方案，官方做的太烂！！！**
**直接在服务器端本地安装**
在`Linux(Debian/Ubuntu)`下, 执行
`bash set-up-ppmessage-on-debian-or-ubuntu.sh`
确保每一项都成功安装。任何错误提示都需要解决掉
########################
For Debian and Ubuntu, the following command will ensure that the required dependencies are installed:
$ sudo apt-get install build-essential libssl-dev libffi-dev python-dev            sudo pip install cryptography
`sudo apt-get install libncurses5-dev`sudo pip install readline
#### 一。下载 [PPMessage](https://ppmessage.gitbooks.io/ppbook/content/GLOSSARY.html#ppmessage)镜像
安装好`Docker`之后，下一步是下载`PPMessage`镜像。镜像托管在[Docker Hub](https://hub.docker.com/r/ppmessage/ppmessage/)上，你可以搜索`ppmessage`找到它。要下载`PPMessage`镜像，打开`Terminal`，输入命令：
```
docker pull ppmessage/ppmessage
```
下载过程可能比较漫长，等待过程中你可继续执行下面的步骤。
#### 二。下载 [PPMessage](https://ppmessage.gitbooks.io/ppbook/content/GLOSSARY.html#ppmessage) 源码
启动 [PPMessage](https://ppmessage.gitbooks.io/ppbook/content/GLOSSARY.html#ppmessage) 之前，首先需要获取[PPMessage](https://ppmessage.gitbooks.io/ppbook/content/GLOSSARY.html#ppmessage)的源码。源码托管在 [github](https://github.com/PPMESSAGE/ppmessage) 上。
假设你将[PPMessage](https://ppmessage.gitbooks.io/ppbook/content/GLOSSARY.html#ppmessage)克隆到目录`~/Documents/ppmessage`。
`git clone git@github.com:PPMESSAGE/ppmessage.git`
#### 下载bower, npm依赖
首先需要安装`nodejs`，在`Mac`上通过以下命令安装
```
brew install nodejs
```
在`Debian\Ubuntu`上，通过以下命令安装`nodejs 6.x`
```
curl -sL https://deb.nodesource.com/setup_6.x | sudo -E bash -
sudo apt-get install -y nodejs
```
然后，用`npm`安装`cnpm, bower, gulp`
```
sudo npm install -g cnpm bower gulp
```
最后，下载`PPCom, PPKefu, PPConsole`的`bower，npm`依赖。进入`~/Documents/ppmessage`目录，执行
```
**./dist.sh bower （务必保证 Jquery安装成功，否则ppcom会弹出错误）**
./dist.sh npm
```
在国内，从`npm`官方镜像源下载`npm`包速度比较慢，且会出现资源被屏蔽的现象。此时可以用`./dist.sh cnpm`代替上面的`./dist.sh npm`, 这会从淘宝`npm`镜像源下载`npm`依赖。
三。配置 [PPMessage](https://ppmessage.gitbooks.io/ppbook/content/GLOSSARY.html#ppmessage)
#### 生成配置文件
> 
启动 [ppmessage](https://ppmessage.gitbooks.io/ppbook/content/GLOSSARY.html#ppmessage).py，如果是首次使用那么使用浏览器访问 [http://127.0.0.1:8945](http://127.0.0.1:8945/) 就会进入配置界面。判断是否配置过的唯一标准是`ppmessage/bootstrap/config.json`这个文件是否存在且其中的
```
configed:
 true
```
。如果想重新配置系统哦，只需要将这个json文件删除，再次启动 [ppmessage](https://ppmessage.gitbooks.io/ppbook/content/GLOSSARY.html#ppmessage).py。
1.  安装用 sudo
2. 通过 pip list检查是否已安装。
（如果没安装pip:   sudo
 apt-get install python-pip )
sudo pip install PyMySQL 
sudo pip install SQLAlchemy
For apt (ubuntu, debian...):
```
sudo apt-get install python-dev  # for python2.x installs
sudo apt-get install python3-dev  # for python3.x installs
```
`sudo apt-get install libjpeg-dev zlib1g-dev``sudo pip install pillow `
(http://stackoverflow.com/questions/3544155/about-the-pil-error-ioerror-decoder-zip-not-available)
~~http://www.pythonware.com/products/**pil**/ 下载对应版本的PIL我下载的是：Imaging-1.1.7.tar.gz ~~
~~sudo wget http://effbot.org/downloads/Imaging-1.1.7.tar.gz~~
~~  tar zxvf Imaging-1.1.7.tar.gz    cd Imaging-1.1.7   cat READMEpython setup.py install然后运行脚本即正常~~
`sudo pip install tornado```sudo pip install pypinyin`sudo pip install redis`sudo pip install StringGenerator
```
sudo pip install python-gcm
```
`sudo pip install paho-mqtt`
**配置redis服务： 默认是本地，利用docker安装到此服务器即可**
**用sudo执行：**
**nohup sudo ./ppmessage.py > console.log 2>&1 &**
**tail -f console.log**

配置时，优先选择账号名、团队名 都先**输入英文**，后续再改
#### 访问 [PPMessage](https://ppmessage.gitbooks.io/ppbook/content/GLOSSARY.html#ppmessage)
在用`Docker`启动`PPMessage`之后，我们还需要生成`PPCom, PPKefu, PPConsole`运行时所需要的`js，css`文件，才能使用这三个模块。
进入`~/Documents/ppmessage`目录，执行
```
./dist.sh gulp
```
以后，当你更新`PPMessage`源码后，都要重复这一操作。
现在，你可以访问`PPMessage`的各模块
/ppmessage/ppmessage/ppkefu/gulp 路径下执行  npm install gulp-sass --save
