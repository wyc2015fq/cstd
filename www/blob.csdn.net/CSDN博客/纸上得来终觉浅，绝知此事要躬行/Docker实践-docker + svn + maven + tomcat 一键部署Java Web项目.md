# Docker实践-docker + svn + maven + tomcat 一键部署Java Web项目 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年05月24日 16:53:04[boonya](https://me.csdn.net/boonya)阅读数：2455








自动化部署时Docker的特性之一，这篇文章主要是通过docker + svn + maven + tomcat 一键部署Java Web项目。


# 要准备的环境

> - docker
- maven
- jdk
- svn
- docker官方的tomcat镜像


### 有关环境的安装和配置可参考

> 
[linux下安装JDK](http://blog.csdn.net/massivestars/article/details/54346829)

[linux系统安装maven](http://blog.csdn.net/massivestars/article/details/54348242)

[安装Docker并在容器里运行tomcat](http://blog.csdn.net/massivestars/article/details/54352484)

*centos下安装svn可以用yum快捷安装yum install svn*


要实现一键部署其实就是要写一个脚本帮助我们完成部署任务。下面我们用一个脚本完成项目自动部署至[Docker](http://lib.csdn.net/base/docker)，由于步骤比较多，我们将步骤分解出来执行以便有更清晰的认识。
- 从SVN拉取或更新项目
- 使用MAVEN编译打包
- 将可执行的web应用移至发布的目录
- 使用Docker运行tomcat镜像,将步骤3的web应用目录挂载至Docker容器

步骤如上所示，下面开始动手。

# 从SVN中checkout项目

```
mkdir -p /usr/local/svn-projects
svn checkout --username=massive --password=massive svn://192.168.1.108/test/basic-web
```

> 
如果项目已经checkout出来，使用svn update更新项目


# 使用MAVEN编译打包项目
`mvn package -Dmaven.test.skip=true -f /usr/local/svn-projects/basic-web`
# 将可执行的web应用移至发布的目录
`cp /usr/local/svn-projects/basic-web/target/basic-web /app/deploy/basic-web`
# 使用docker运行tomcat镜像
`docker run --privileged=true -v /app/deploy/basic-web:/usr/local/tomcat/webapps/basic-web -p 8081:8080 tomcat:7`
> 
将 /app/deploy/basic-web 的资源挂载至容器的 /usr/local/tomcat/webapps/basic-web 目录


# 把上述步骤编写成一个脚本执行

步骤分析完毕，现在要做的事就是把上述的步骤放在一个脚本里执行，达到我们一建部署的目的。

```
mkdir -p /usr/local/script
touch /usr/local/script/docker-deploy
chmod +x /usr/local/script/docker-deploy
ln -s /usr/local/bin/docker-deploy /usr/local/script/docker-deploy
```

脚本的内容如下：

```bash
#!/bin/sh
# deploy options

project=basic-web

# 要部署的目录
deploy_dir=/app/deploy/$project


# svn options
project_svn_uri=svn://192.168.1.108/test/basic-web
svn_base_dir=/usr/local/svn-projects/
svn_user=massive
svn_password=massive



# docker options
# 接收 -p 参数，若没传此参数则取默认端口8080
port=8080
while getopts ":p:" opt; do
  case $opt in
    p)
      port=$OPTARG   #get the value
      ;;
    ?)
      echo "How to use: $0 [-p port]" >&2
      exit 1
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      exit 1
      ;;
  esac
done


# 取svn地址最后一个`/`字符后面的名称作为checkout出来的目录
svn_full_dir=$svn_base_dir${project_svn_uri##*/}


echo '----------------------------------------------'
echo "checkout project    :    ${project_svn_uri}"
echo "checkout diretory   :    ${svn_full_dir}"
echo "deploy project      :    ${deploy_dir}"
echo "deploy port         :    ${port}"
echo '----------------------------------------------'


# ------------------------------------
# checkout or update from svn
# ------------------------------------
cd $svn_full_dir

echo "try to get svn info..."
svn info
# 如果svn info命令执行成功，证明项目已checkout，否则就未checkout
# 若未checkout则checkout项目，否则update项目至最新版本
if [ $? -eq 0 ];
then
    echo "update svn ..."
    svn update
else
    echo 'checkout from svn...'
    echo "svn checkout --username=${svn-user} --password=${svn_password} ${project_svn_uri}"
    svn checkout --username=$svn_user --password=$svn_password $project_svn_uri
fi


# ------------------------------------
# install maven project
# ------------------------------------
mvn package -Dmaven.test.skip=true -f $svn_full_dir
# ------------------------------------

echo "copying project to ${deploy_dir}..."
mkdir -p $deploy_dir
cp $svn_full_dir/target/$project $deploy_dir -r
echo "copy project end"

echo "docker run --privileged=true -v ${deploy_dir}:/usr/local/tomcat/webapps/${project} -p ${port}:8080 tomcat:7"
docker run --privileged=true -v $deploy_dir:/usr/local/tomcat/webapps/$project -p $port:8080 tomcat:7
```

编写脚本完成后，我们可以就可以愉快的使用部署命令了
`docker-deploy`
若要指定容器的映射端口可以加上 *-p* 参数
`docker-deploy -p 8081`
至此，我们已经实现了一个命令实现应用的部署，而且容器不用关心环境变量，不得不感慨Docker对于DevOps带来的便利。这个脚本执行的任务和Jenkins有点类似，但脚本可以控制的粒度更细，可扩展性更高。

> 
若有其它需要，可以扩展脚本以实现更多功能， 
  比如svn的版本回滚至3000再运行： docker-deploy -b 3000 -p 8081 
  比如svn的版本不更新直接运行：   docker-deploy -U -p 8081  (可设置成-U 不更新svn，-u更新svn)




