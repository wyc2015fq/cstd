# ubuntu12.04部署ROR - gauss的专栏 - CSDN博客
2014年09月13日 07:36:19[gauss](https://me.csdn.net/mathlmx)阅读数：272
个人分类：[web](https://blog.csdn.net/mathlmx/article/category/2558531)
刚开始部署这玩意是很折磨的，折腾吧。
确保使用最新版本的linux，否则成功率会大大降低，因为ruby开源社区对于ubuntu的支持总是不兼容旧版本的。笔者部署时的版本是ubuntu12.04
每次都是缺少这库又需要那库的，所以这里让大家首先安装各种库
1.首先安装yaml 库，避免以后的麻烦。 不装此库会让你安装不上rubygems
wget -c http://pyyaml.org/download/libyaml/yaml-0.1.4.tar.gz
tar xzvf yaml-0.1.4.tar.gz
cd yaml-0.1.4
./configure --prefix=/usr/local #注意此处请勿修改路径，否则库文件无法写入正确目录
make && make install
2.安装zlib1g  和 zlib1g-dev，防止以后运行gem 出现 no such file to load zlib(LoadError)
sudo apt-get install zlib1g zlib1g-dev
装好以后，如果是已经装了Ruby[注意：这里是已经装了ruby] , 那么就找到ruby的安装的源文件夹下的/ext/zlib/  运行
ruby extconf.rb
make && make install
还有openssl这个要装，不然在rails一个demo时会失败
sudo apt-get install libopenssl-ruby
sudo apt-get install openssl
sudo apt-get install libssl-dev
sudo apt-get install libssl0.9.8
【注意：这里也是已经装了ruby的，没装ruby直接下一步】在进入源码下的/ext/openssl
ruby extconf.rb
make && make install
3.安装rvm
apt-get install curl
curl -L get.rvm.io | bash -s stable
如果你是root用户登录，则此时rvm -v可以看到版本号了，如果不是，你需要做下面这个【】步骤
【sudo gedit ~/.bashrc
添加export PATH=/usr/local/rvm/bin:$PATH
source ~/.bashrc
source ~/.bash_profile
】
sed -i -e 's/ftp\.ruby-lang\.org\/pub\/ruby/ruby\.taobao\.org\/mirrors\/ruby/g' /usr/local/rvm/config/db
（修改rvm的ruby安装源到淘宝镜像，原因是国内网络你懂的）
4.安装Ruby 
列出已知ruby版本
rvm list known
安装一个ruby版本
rvm install ruby-2.0.0-p247
确保bash已登陆
/bin/bash --login
使用一个ruby版本
rvm use 2.0.0-p247
如果想设置为默认版本，可以这样
rvm use 2.0.0 --default
卸载一个已安装版本
rvm remove 1.9.2
5.安装gem
wget -c http://production.cf.rubygems.org/rubygems/rubygems-1.8.24.tgz
tar xzvf rubygems-1.8.24.tgz
cd rubygems-1.8.24
ruby setup.rb　　
6.安装rails
由于国内网络原因（你懂的），导致rubygems.org存放在Amazon S3上面的资源间歇连接失败，所以你会遇到gem install rails 半天没反应的情况。
所以，淘宝网做了rubygems 镜像。
首先替换sources
gem sources --remove http://rubygems.org/
gem sources -a http://ruby.taobao.org/
gem sources -l       #用来查看source list
请确保只有http://ruby.taobao.org/ 一个
然后就可以gem install rails --no-ri --no-rdoc
为什么要用--no-ri --no-rdoc 参数，因为加上去安装快，而且这两个也用不着
安装完毕后，使用rails -v 查看版本，如果你看到正确显示版本，恭喜你，可以下一步了。
7.开始一个demo
当然不可能就让你这么愉快的开始demo
sudo apt-get install libsqlite3-dev               //虽然你不用这sqlite,但是还得装，不然就无法bundle install ，虽然我也不知道这个bundle是个啥
sudo apt-get install nodejs        
   //Nodejs就不用解释了，不知道为啥要安装这个，不过不安装的话当你rake db:create 的时候会提示你could not find Javascript Runtime
至此，就可以在一个目录下运行
rails new demo --skip-bundle　　#直接rails new会自动运行bundle install，而bundle install会自动去rubygems.org查找是否有更新，少则五分钟，多则几十分钟
另外，最新版bundler速度提高不少，建议使用，也能提速
gem install bundler --pre
cd demo
bundle install --local
这个命令如果失败，则少了什么装什么，比如提示Could not find gem 'sdoc (>= 0) ruby' in the gems available on this machine.那就gem install sdoc
还有可能缺少的比如  gem install coffee-rails
gem install sqlite3
如果嫌麻烦还可以先 gem install all一下，再一个个加剩余的。
直到 bundle install --local 能够成功为止。
rake db:create
此时若出现rake aborted!
cannot load such file -- sqlite3/sqlite3_native
请手动setup一下sqlite3
cd   /usr/local/rvm/gems/ruby-2.0.0-p247/gems/sqlite3-1.3.8/lib/
ruby setup.rb
rake db:create
若报错：rake aborted!
Could not find a JavaScript runtime.
gedit Gemfile
添加一行
`gem 'therubyracer'`
保存，再bundle install，gem会帮你安装javascript
再一次rake db:create，我成功了，若还有其他报错，请检查数据库密码，改./config/database.yml将密码打上双引号可能可以解决。
rails s
看到WEBrick启动后，就可以打开浏览器，输入http://localhost:3000 访问demo了，是一个博客：） 
