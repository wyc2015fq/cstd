
# k8s 权威教程第一章demo（Bug） - Wesley's Blog - CSDN博客


2018年08月02日 09:30:58[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：573


# Bug 1
## 问题描述
`做Tomcat的例子，结果get pods一直显示没有资源？`应用配置代码：
`apiVersion : v1
kind : ReplicationController
metadata : 
    name : mysql
spec : 
  replicas : 2
  selector : 
    app : mysql
  template : 
    metadata : 
      labels : 
        app : mysql
    spec : 
      containers : 
      - name : mysql
        image : mysql
        ports : 
        - containerPort : 3306
        env : 
        - name : MYSQL_ROOT_PASSWORD
          value : "123456"`创建rc
`# kubectl create -f mysql-rc.yaml 
replicationcontroller "mysql" created
# kubectl get rc
NAME      DESIRED   CURRENT   READY     AGE
mysql     2         0         0         11s
myweb     2         0         0         5s
# kubectl get pods
No resources found.
# docker images
REPOSITORY                                            TAG                 IMAGE ID            CREATED             SIZE`
## 分析
创建了rc之后,虽然命令行提示成功了，但是pods一直没有创建，也检查了docker，mysql的镜像没有拉取。又仔细看了一边rc的结果，current和ready列的值是0，应该先解决current为０的问题。
## 解决方法
executed command :
openssl genrsa -out /tmp/serviceaccount.key 2048
modified the /etc/kubernetes/apiserver file to add following :
KUBE_API_ARGS="--service_account_key_file=/tmp/serviceaccount.key"
modified the /etc/kubernetes/controller-manager and add following:
KUBE_CONTROLLER_MANAGER_ARGS="--service_account_private_key_file=/tmp/serviceaccount.key"
restarted the kube-apiserver and kube-controller-manager services to restart the services
service kube-apiserver restart
service kube-controller-manager restart
# Bug 2
## 问题描述
通过`kubectl describe pod`可以查看报错：
`Error:com.mysql.jdbc.exceptions.jdbc4.CommunicationsException: Communications link failure The last packet sent successfully to the server was 0 milliseconds ago. The driver has not received any packets from the server.`相关资源rc、pod、service、ep都创建成功,但是**myweb的pods无法访问到mysql提供的数据库服务**。
`# kubectl get all
NAME       DESIRED   CURRENT   READY     AGE
rc/mysql   1         1         1         4h
rc/myweb   1         1         1         2h
NAME             CLUSTER-IP      EXTERNAL-IP   PORT(S)          AGE
svc/kubernetes   10.254.0.1      <none>        443/TCP          5d
svc/mysql        10.254.67.31    <none>        3306/TCP         4h
svc/myweb        10.254.62.177   <nodes>       8080:30001/TCP   2h
NAME             READY     STATUS    RESTARTS   AGE
po/mysql-vc9x6   1/1       Running   0          4h
po/myweb-6k7s3   1/1       Running   0          2h
# kubectl get ep
NAME         ENDPOINTS            AGE
kubernetes   192.168.1.171:6443   5d
mysql        172.17.0.4:3306      4h
myweb        172.17.0.2:8080      2h`
## 分析
查看源代码
既然无法建立连接，那先看下是如何建立连接的。登录到myweb的docker容器里面，查看index.jsp文件，主要内容如下：
`java.sql.Connection conn=null;
java.lang.String strConn;
java.sql.Statement stmt=null;
java.sql.ResultSet rs=null;
Class.forName("com.mysql.jdbc.Driver").newInstance();
try{
      Class.forName("com.mysql.jdbc.Driver");
       String ip=System.getenv("MYSQL_SERVICE_HOST");
       String port=System.getenv("MYSQL_SERVICE_PORT");
       ip=(ip==null)?"localhost":ip;
       port=(port==null)?"3306":port;
      System.out.println("Connecting to database...");
      System.out.println("jdbc:mysql://"+ip+":"+port+"?useUnicode=true&characterEncoding=UTF-8");
      conn = java.sql.DriverManager.getConnection("jdbc:mysql://"+ip+":"+port+"?useUnicode=true&characterEncoding=UTF-8", "root","123456");
      stmt = conn.createStatement();
}catch(Exception ex){
  ...
}`就是用jsp创建了一个连接，连接的地址通过ENV方式注入。即在myweb-rc.yaml中配置的`MYSQL_SERVICE_HOST`和`MYSQL_SERVICE_PORT`环境变量指定。
登陆myweb应用查看这两个环境变量是否有问题：
`[root@localhost ~]# kubectl exec -ti myweb-cp4gm -- /bin/bash
root@myweb-cp4gm:/usr/local/tomcat# echo $MYSQL_SERVICE_HOST
10.254.15.45
root@myweb-cp4gm:/usr/local/tomcat# echo $MYSQL_SERVICE_PORT
3306`可以看出环境变量也是没有问题的。
## 解决方法
考虑到可能是mysql版本的问题，修改mysql版本为5.7，并且设置来直接使用本地镜像。
`apiVersion : v1
kind : ReplicationController
metadata : 
    name : mysql
spec : 
  replicas : 1
  selector : 
    app : mysql
  template : 
    metadata : 
      labels : 
        app : mysql
    spec : 
      containers : 
      - name : mysql
        image : mysql：5.7
        imagePullPolicy: IfNotPresent 
        ports : 
        - containerPort : 3306
        env : 
        - name : MYSQL_ROOT_PASSWORD
          value : "123456"`
# 参考博客
[https://www.cnblogs.com/jason0529/p/9122694.html](https://www.cnblogs.com/jason0529/p/9122694.html)

