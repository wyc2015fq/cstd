# rails初体验 - weixin_33985507的博客 - CSDN博客
2015年12月06日 19:54:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
很久没有来更新了，因为最近自己在新浪云上面搭建了个wordpress玩，但是还没有弄上markdown的插件，所以还是现在这里写，那边再复制过去。
- <a href="http://cword.sinaapp.com">我的wordprss地址</a>
# 以下是正题
### 为什么会有rails
- 其实一开始是前端入门的，后来写着写着，刚好学校网络中心需要PHP开发，然后就开始入了后端的坑，然后不知道为什么写着写着对PHP并不是特别喜欢，一个是现在的基础教程真的不友好，随手一搜都是PHP4的教程，现在都出PHP7了，还有一个就是很混乱，面向对象一种方式，面向过程又一种方式，一上来就迷迷糊糊的，然后想着有没有其他后台语言可以学
- 然后在udacity看jquery教程的时候，看到他们在ajax做交互的时候使用python写的，然后我又知道原来有django的web开发框架，然后又知道railis，对于rails的快速原型开发很有好感，现在打算用rails写一个电商网站
### rails初体验
- 服务器部署略过，由于是学生党没钱换mac，所以是在linux下部署的ruby和rails的开发环境，网上教程很多，我是参考ruby china 的wiki部署的
- 先到图书馆借了本rails学习手册，虽然里面的代码很久了，但是多多少少也有启发作用，等自己做到一定的成果以后，再上网入手经典rails开发书，继续开发
### rails上线
###### 其实rubychina上面的会更全，我之所以写来是为了帮自己理清思路和加深印象
- $ rails new  blog
> - 这个命令会在文件夹 blog 中新建一个 Rails 程序，然后执行 bundle install命令安装 Gemfile 中列出的 gem。
- 执行 rails new -h
可以查看新程序生成器的所有命令行选项。
- $ cd blog
> 
blog 文件夹中有很多自动生成的文件和文件夹，组成一个 Rails 程序。本文大部分时间都花在 app文件夹上。下面简单介绍默认生成的文件和文件夹的作用：
|文件/文件夹|作用|
|----|----|
|app/|存放程序的控制器、模型、视图、帮助方法、邮件和静态资源文件。本文主要关注的是这个文件夹。|
|bin/|存放运行程序的 rails脚本，以及其他用来部署或运行程序的脚本。|
|config/|设置程序的路由，数据库等。详情参阅“[设置 Rails 程序](https://link.jianshu.com?t=http://guides.ruby-china.org/configuring.html)”一文。|
|config.ru|基于 Rack 服务器的程序设置，用来启动程序。|
|db/|存放当前数据库的模式，以及数据库迁移文件。|
|Gemfile, Gemfile.lock|这两个文件用来指定程序所需的 gem 依赖件，用于 Bundler gem。关于 Bundler 的详细介绍，请访问 [Bundler 官网](https://link.jianshu.com?t=http://bundler.io/)。|
|lib/|程序的扩展模块。|
|log/|程序的日志文件。|
|public/|唯一对外开放的文件夹，存放静态文件和编译后的资源文件。|
|Rakefile|保存并加载可在命令行中执行的任务。任务在 Rails 的各组件中定义。如果想添加自己的任务，不要修改这个文件，把任务保存在 lib/tasks文件夹中。|
|README.rdoc|程序的简单说明。你应该修改这个文件，告诉其他人这个程序的作用，如何安装等。|
|test/|单元测试，固件等测试用文件。详情参阅“[测试 Rails 程序](https://link.jianshu.com?t=http://guides.ruby-china.org/testing.html)”一文。|
|tmp/|临时文件，例如缓存，PID，会话文件。|
|vendor/|存放第三方代码。经常用来放第三方 gem。|
- $ rails server
> 
上述命令会启动 WEBrick，这是 Ruby 内置的服务器。要查看程序，请打开一个浏览器窗口，访问[http://localhost:3000](https://link.jianshu.com?t=http://localhost:3000/)。应该会看到默认的 Rails 信息页面
### 显示“Hello, Rails!”
> - 要在 Rails 中显示“Hello, Rails!”，需要新建一个控制器和视图。
- 
控制器用来接受向程序发起的请求。路由决定哪个控制器会接受到这个请求。一般情况下，每个控制器都有多个路由，对应不同的动作。动作用来提供视图中需要的数据。
- 
视图的作用是，以人类能看懂的格式显示数据。有一点要特别注意，数据是在控制器中获取的，而不是在视图中。视图只是把数据显示出来。默认情况下，视图使用 eRuby（嵌入式 Ruby）语言编写，经由 Rails 解析后，再发送给用户。
- 
控制器可用控制器生成器创建，你要告诉生成器，我想要个名为“welcome”的控制器和一个名为“index”的动作，如下所示：
- 
rails generate controller welcome index
> 
运行上述命令后，Rails 会生成很多文件，以及一个路由。
create  app/controllers/welcome_controller.rb
route  get 'welcome/index'
invoke  erb
create    app/views/welcome
create    app/views/welcome/index.html.erb
invoke  test_unit
create    test/controllers/welcome_controller_test.rb
invoke  helper
create    app/helpers/welcome_helper.rb
invoke  assets
invoke    coffee
create      app/assets/javascripts/welcome.js.coffee
invoke    scss
create      app/assets/stylesheets/welcome.css.scss
- 在这些文件中，最重要的当然是控制器app/controllers/welcome_controller.rb
- 以及视图，位于 app/views/welcome/index.html.erb。
- 使用文本编辑器打开 app/views/welcome/index.html.erb
文件，删除全部内容，写入下面这行代码：
`<h1>Hello, Rails!</h1>`
我们已经创建了控制器和视图，现在要告诉 Rails 在哪个地址上显示“Hello, Rails!”。这里，我们希望访问根地址 [http://localhost:3000](https://link.jianshu.com?t=http://localhost:3000/) 时显示。但是现在显示的还是欢迎页面。
我们要告诉 Rails 真正的首页是什么。在编辑器中打开 config/routes.rb 文件。
```
Rails.application.routes.draw do 
get 'welcome/index'
# The priority is based upon order of creation:
# first created -> highest priority.
# You can have the root of your site routed with "root"
# root 'welcome#index'
#
# ...
```
这是程序的路由文件，使用特殊的 DSL（domain-specific language，领域专属语言）编写，告知 Rails 请求应该发往哪个控制器和动作。文件中有很多注释，举例说明如何定义路由。其中有一行说明了如何指定控制器和动作设置网站的根路由。找到以 root
开头的代码行，去掉注释，变成这样：
```
root
'welcome#index'
root 'welcome#index'
```
- 告知 Rails，访问程序的根路径时，交给 welcome控制器中的 index 动作处理。get 'welcome/index'告知 Rails，访问 [http://localhost:3000/welcome/index](https://link.jianshu.com?t=http://localhost:3000/welcome/index) 时，交给 welcome 控制器中的 index动作处理。get 'welcome/index'是运行 rails generate controller welcome index时生成的。
- 如果生成控制器时停止了服务器，请再次启动（rails server），然后在浏览器中访问[http://localhost:3000](https://link.jianshu.com?t=http://localhost:3000/)。你会看到之前写入 app/views/welcome/index.html.erb文件的“Hello, Rails!”，说明新定义的路由把根目录交给 WelcomeController的 index动作处理了，而且也正确的渲染了视图。
