# 用capistrano写一个简单的deploy脚本 - Arthur的随笔 - CSDN博客
2011年09月02日 15:04:44[largetalk](https://me.csdn.net/largetalk)阅读数：6105
                
capistrano是什么我就不说了， 自己google去
setp 1: gem install capistrano
           gem install capistrano-ext
setp 2:  在项目根目录执行 "capify ." 这将在根目录创建Capfile 和 config目录下创建deploy.rb 文件
setp 3: 编辑deploy.rb
```
require 'bundler/capistrano'     #添加之后部署时会调用bundle install， 如果不需要就可以注释掉
require "capistrano/ext/multistage"     #多stage部署所需
set :stages, %w(development production)
set :default_stage, "development"
set :application, "crm_app_end"   #应用名称
set :repository,  "https://chang.abc.com/svn/engineering/vwaccount/mydeploy/trunk"   
set :keep_releases, 5          #只保留5个备份
set :deploy_to, "/var/www/#{application}"  #部署到远程机器的路径
set :user, "user1"              #登录部署机器的用户名
set :password, "user1"      #登录部署机器的密码， 如果不设部署时需要输入密码
default_run_options[:pty] = true          #pty: 伪登录设备
#default_run_options[:shell] = false     #Disable sh wrapping
set :use_sudo, true                            #执行的命令中含有sudo， 如果设为false， 用户所有操作都有权限
set :runner, "user2"                          #以user2用户启动服务
set :svn_username, "xxxx"     
set :scm, :subversion                        #
# Or: `accurev`, `bzr`, `cvs`, `darcs`, `git`, `mercurial`, `perforce`, `subversion` or `none`                   
#set :deploy_via, :copy                     #如果SCM设为空， 也可通过直接copy本地repo部署
#set :domain, "crm.abc.com"    #custom define
role :web, "192.168.0.13", "192.168.0.117"                          # Your HTTP server, Apache/etc
role :app, "192.168.0.13", "192.168.0.117"                          # This may be the same as your `Web` server
role :db,  "192.168.0.13", :primary => true # This is where Rails migrations will run
#role :db,  "your slave db-server here"
#
      
namespace :deploy do
    desc "remove and destory this app"
    task :destory, :roles => :app do
        run "cd #{deploy_to}/../ && #{try_sudo} mv #{application} /tmp/#{application}_#{Time.now.strftime('%Y%d%m%H%M%S')}"      #try_sudo 以sudo权限执行命令
    end
    after "deploy:update", "deploy:shared:setup"              #after， before 表示在特定操作之后或之前执行其他任务
    namespace :shared do
        desc "setup shared folder symblink"
        task :setup do
            run "cd #{deploy_to}/current; rm -rf shared; ln -s #{shared_path} ."     
        end
    end
    after "deploy:setup", "deploy:setup_chown"
    desc "change owner from root to user1"
    task :setup_chown do
        run "cd #{deploy_to}/../ && #{try_sudo} chown -R #{user}:#{user} #{application}"
    end
    task :start do
       run "cd #{deploy_to}/current && ./crmd.sh start"
       #try_sudo "cd #{deploy_to}/current && ./restart.sh"
    end
    task :stop do
       run "cd #{deploy_to}/current && ./crmd.sh stop"
    end
    task :restart do
       run "cd #{deploy_to}/current && ./crmd.sh restart"
    end
    
end
```
setup 4: 在项目根目录执行  
             cap deploy:setup #建立部署路径
             cap deploy:update #部署
             cap deploy:start    #启动服务
             cap deploy:stop   #停止服务
             cap deploy:restart #重启服务
setup 5: 如果有多个stage要部署，则在config下创建deploy文件夹， 在该文件夹下有各stages文件, 文件名和 set :stages, %w(development production) 对应， 如development.rb production.rb，在各文件中设置相应变量即可， 然后可用 cap production deploy:... 来执行对应production的操作
其实capistrano最终都是转变成shell命令来完成任务的， 所以纯粹用shell脚本也可以完成相应功能， 但相对于晦涩的shell命令， capistrano明显更好懂， 并且它有很多默认操作非常好用， 比如setup， update， 如果自己用shell来实现非常麻烦， capistrano允许在脚本中嵌入shell， 比如上面的start， restart等， 这非常方便， 很多capistrano做不到的就可以用shell去搞定了
这事最近又有了新进展，我在一个django项目中用capistrano来发布，虽然python下也有一个叫fabric的东西，但那个还不熟，先用这个，有时间学习一下fabric再说
```
require 'bundler/capistrano'     #添加之后部署时会调用bundle install， 如果不需要就可以注释掉  
#require "capistrano/ext/multistage"     #多stage部署所需  
  
set :application, "app"   #应用名称  
set :scm, :subversion                        
set :repository,  "svn://0.0.0.0/trunk/src"         
set :keep_releases, 5          #只保留5个备份  
  
set :deploy_to, "/var/www/#{application}"  #部署到远程机器的路径  
set :user, "xxx"              #登录部署机器的用户名  
set :password, "xxxxx"      #登录部署机器的密码， 如果不设部署时需要输入密码  
  
default_run_options[:pty] = true          #pty: 伪登录设备  
#default_run_options[:shell] = false     #Disable sh wrapping  
  
#set :use_sudo, true                            #执行的命令中含有sudo， 如果设为false， 用户所有操作都有权限  
set :runner, "xxxx"                          #以用户启动服务  
                     
#set :deploy_via, :copy                     #如果SCM设为空， 也可通过直接copy本地repo部署  
 
role :web, "172.16.120.222"#, "192.168.0.117"                          # Your HTTP server, Apache/etc  
role :app, "172.16.120.222"#, "192.168.0.117"                          # This may be the same as your `Web` server  
role :db,  "172.16.120.222", :primary => true # This is where Rails migrations will run  
#role :db,  "your slave db-server here"  
#  
        
namespace :deploy do  
  
  
    desc "remove and destory this app"  
    task :destory, :roles => :app do  
        run "cd #{deploy_to}/../ && #{try_sudo} mv #{application} /tmp/#{application}_#{Time.now.strftime('%Y%d%m%H%M%S')}"      #try_sudo 以sudo权限执行命令  
    end  
    
#    after "deploy:setup", "deploy:setting_link"
#    desc "symlink for settings.py"
#    task :setting_link do
#        run "cd #{deploy_to}; #{try_sudo} touch current; #{try_sudo} ln -s current releases/#{application}"
#    end
  
    after "deploy:update", "deploy:shared:setup"              #after， before 表示在特定操作之后或之前执行其他任务  
  
    namespace :shared do  
        desc "setup shared folder symblink"  
        task :setup do
            run "cd #{deploy_to}/current; ln -s #{shared_path} x"       
            run "cd #{deploy_to}/current; rm -rf log; ln -s #{shared_path}/log log"       
            run "cd #{deploy_to}/releases; rm -f #{application} &&  __realversion__=`realpath ../current` && ln -s $__realversion__ #{application}"
        end  
    end  
  
    after "deploy:setup", "deploy:setup_chown"  
    desc "change owner from root to user1"  
    task :setup_chown do  
        run "cd #{deploy_to}/../ && #{try_sudo} chown -R #{user}:#{user} #{application}"  
    end  
 
    task :default do 
        transaction do 
            update_code
            symlink
        end
    end
   task :update_code, :except => { :no_release => true } do 
        on_rollback { run "rm -rf #{release_path}; true" } 
        strategy.deploy! 
    end
   before "deploy:rollback", "deploy:clean_adaptive"
   task :clean_adaptive do
        run "cd #{deploy_to}/releases; rm -f #{application}"
   end
       
   
   after "deploy:rollback", "deploy:rollback_set"
   task :rollback_set do
        run "cd #{deploy_to}/releases; rm -f #{application} &&  __realversion__=`realpath ../current` && ln -s $__realversion__ #{application}"
   end
  
    task :start do  
       run "cd #{deploy_to}/current && ./adaptive_pool.sh start"  
       run "cd #{deploy_to}/current && ./adaptive_exam.sh start"   
    end  
  
  
    task :stop do  
       run "cd #{deploy_to}/current && ./adaptive_pool.sh stop"  
       run "cd #{deploy_to}/current && ./adaptive_exam.sh stop"  
    end  
  
  
    task :restart do  
       run "cd #{deploy_to}/current && ./adaptive_pool.sh restart"  
       run "cd #{deploy_to}/current && ./adaptive_exam.sh restart"  
    end  
      
end
```
