# 高逼格saas服务的文档展示 - 三少GG - CSDN博客
2015年12月08日 20:07:44[三少GG](https://me.csdn.net/scut1135)阅读数：1291
***********************
gitbook
readthedocs
[http://wenchao-img.qiniudn.com/markdown.html#slide1](http://wenchao-img.qiniudn.com/markdown.html#slide1)
***********************
1. **mkdocs  http://www.mkdocs.org/**
**theme主题： https://github.com/chrissimpkins/cinder   **
**效果： http://sourcefoundry.org/cinder/**
### Install the Cinder Theme
Download the Cinder theme archive.
[](https://github.com/chrissimpkins/cinder/releases/download/v0.9.3/cinder.zip) Download Cinder v0.9.3
Unpack the contents of the archive into a directory named `cinder` at
 the top level of your MkDocs project directory.
Your project directory should now look like this:
```
.
├── mkdocs.yml
├── cinder
│     ├── css
│     ├── img
│     ├── js
│     ├── base.html
│     ├── content.html
│     ├── 404.html
│     ├── nav-sub.html
│     ├── nav.html
│     └── toc.html
└── docs
      └── index.md
```
### Complete Your Settings File
MkDocs projects use a YAML settings file called `mkdocs.yml`. This
 is located in the root of your project directory after you use the 
```
mkdocs
 new
```
 command. Open the file in a text editor and modify it to include the `theme_dir`setting
 as follows:
```
site_name: [YOURPROJECT]
theme_dir: cinder
pages:
- Home: index.md
```
**docker部署：https://github.com/andrglo/docker-mkdocs.  暂时不可以用daocloud管理，直接docker run.**
Create a new project
```
$ docker run --rm -v ~/local_workspace:/workspace andrglo/mkdocs new lf-project
```
Webserver for development
`$ docker run -d -p 8010:8000 -v ~/local_workspace/lf-project:/workspace andrglo/mkdocs`
2.[justinwalsh/ daux.io](https://github.com/justinwalsh/daux.io)
大全： https://github.com/PharkMillups/beautiful-docs
国内： 
**阿里 http://rap.taobao.org/org/index.do**
http://www.kancloud.cn  
http://apizza.cc/
小幺鸡
补充：
特别棒的js文档  
# [https://beecloud.cn/doc/#1-2-3](https://beecloud.cn/doc/#1-2-3)[http://docs.daocloud.io/api/#介绍](http://docs.daocloud.io/api/#%E4%BB%8B%E7%BB%8D)
其他：
[https://getgrav.org/](https://getgrav.org/)
http://www.sphinx-doc.org/en/stable/theming.html
