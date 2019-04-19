# redmine安装 - Arthur的随笔 - CSDN博客
2012年06月16日 00:40:54[largetalk](https://me.csdn.net/largetalk)阅读数：2449
折腾了一下redmine安装， 发现ruby很多东西都不记得了，留个笔记，备忘。
安装redmine之前先要安装 ruby， 我是用rvm安装得， rvm是个很好得东西
按照 [https://rvm.io/rvm/install/](https://rvm.io/rvm/install/) 安装rvm， 我安装得是single user，因为就我一个人用。
然后 
```
source ~/.rvm/script/rvm
rvm install ruby 1.9.2
```
把淘宝gem源加进去，加快后面gem安装速度
gem sources -a http://ruby.taobao.org/
下面根据[http://www.redmine.org/projects/redmine/wiki/RedmineInstall](http://www.redmine.org/projects/redmine/wiki/RedmineInstall) 这个网址一步步安装redmine
svn co http://redmine.rubyforge.org/svn/branches/2.0-stable redmine-2.0
获得redmine源码后，cd到这个目录下，执行
`gem install bundler``bundle install --without development test rmagick sqlite`
中间可能会有出错，比如mysql2安装出错，就sudo apt-get install mysql-server libmysql-ruby libmysqlclient-dev
创建数据库:
create database redmine character set utf8;grant all privileges on redmine.* to 'redmine'@'localhost' identified by 'my_password';
好了之后
cp config/database.yml.example config/database.yml
production:
  adapter: mysql2
  database: redmine
  host: localhost
  username: redmine
  password: my_passwordrake generate_secret_token`RAILS_ENV=production rake db:migrate``RAILS_ENV=production rake redmine:load_default_data`mkdir tmp public/plugin_assets
sudo chown -R redmine:redmine files log tmp public/plugin_assets
sudo chmod -R 755 files log tmp public/plugin_assets
ps. 如果是在用户自己目录， chown那步就无所谓了
现在 rails -e production 就可以把服务起起来了，在3000端口就可以看到redmine了。
需要一个在有bug更新时有邮件发送得服务， 配置下邮件服务，
cp config/configuration.yml.example config/configuration.yml
然后进去配置下，ok。
webrick实在太慢了， 又不愿用nginx， apache来配置，只想自己监听个端口就行， 本来想用mongrel来起，但怎么搞都出错， 好像mongrel和ruby1.9有冲突， 
后来在[http://unicorn.bogomips.org/](http://unicorn.bogomips.org/) 发现unicorn, python下得gunicorn非常好用，这个也非常简单
gem install unicornunicorn_rails -p 3000 -E production -D
基本上就这样了
