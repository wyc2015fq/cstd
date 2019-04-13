
# docker基础：私库系列：再探Harbor：(7) 使用restapi对项目进行增删改查 - 知行合一 止于至善 - CSDN博客

2018年11月01日 20:53:46[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：1852所属专栏：[DevOps系列之：镜像私库](https://blog.csdn.net/column/details/26478.html)



在上篇文章中介绍了Harbor的RestApi，这篇具体以项目的增删改查作为例子来进行说明。
# 前提假定
假定Harbor运行于本机localhost的32031端口
# 项目查询
## 查询全部项目
> curl -X GET “
> [http://localhost:32031/api/projects](http://localhost:32031/api/projects)
> ”  -uadmin:用户密码

## 按项目名称查询
查询项目名称为library的项目信息
> curl -X GET “
> [http://localhost:32031/api/projects?name=library](http://localhost:32031/api/projects?name=library)
> ”  -uadmin:用户密码

# 项目增加
添加如下项目名称为testproject1的项目，类型为public
> curl -X POST “
> [http://localhost:32031/api/projects](http://localhost:32031/api/projects)
> ” -H “accept: application/json” -H “Content-Type: application/json” -d@testproject1.json -uadmin:用户密码
用户json信息
`[root@liumiaocn ~]# cat testproject1.json 
{
  "project_name": "testproject1",
  "metadata": {
    "public": "true"
  }
}
[root@liumiaocn ~]#`创建项目信息查询
`[root@liumiaocn ~]# curl -X GET "http://localhost:32031/api/projects?name=testproject1"  -uadmin:用户密码
[
  {
    "project_id": 10,
    "owner_id": 1,
    "name": "testproject1",
    "creation_time": "2018-11-01T12:38:49Z",
    "update_time": "2018-11-01T12:38:49Z",
    "deleted": 0,
    "owner_name": "",
    "togglable": true,
    "current_user_role_id": 1,
    "repo_count": 0,
    "metadata": {
      "public": "true"
    }
  }
][root@liumiaocn ~]#`
# 项目信息更新
将刚刚创建的项目从public的状态改为private的，相关的json信息如下
`[root@liumiaocn ~]# cat testproject1.json 
{
  "project_name": "testproject1",
  "metadata": {
    "public": "false"
  }
}
[root@liumiaocn ~]#`使用put方式更新项目信息
`[root@liumiaocn ~]# curl -X PUT "http://localhost:32031/api/projects/10" -H "accept: application/json" -H "Content-Type: application/json" -d@testproject1.json -uadmin:用户密码
[root@mail ~]# echo $?
0
[root@liumiaocn ~]#`因为项目变成了private，所以不指定用户已经无法查询出结果，指定admin账户后可以看到项目的public已经被设定称了false
`[root@liumiaocn ~]# curl -X GET "http://localhost:32031/api/projects?name=testproject1"  
null[root@liumiaocn ~]# curl -X GET "http://localhost:32031/api/projects?name=testproject1"  -uadmin:用户密码
[
  {
    "project_id": 10,
    "owner_id": 1,
    "name": "testproject1",
    "creation_time": "2018-11-01T12:38:49Z",
    "update_time": "2018-11-01T12:38:49Z",
    "deleted": 0,
    "owner_name": "",
    "togglable": true,
    "current_user_role_id": 1,
    "repo_count": 0,
    "metadata": {
      "public": "false"
    }
  }
][root@liumiaocn ~]#`
# 项目删除
使用DELETE方法可以对刚创建的项目进行删除
> [root@liumiaocn ~]\# curl -X DELETE “
> [http://localhost:32031/api/projects/10](http://localhost:32031/api/projects/10)
> ”  -uadmin:用户密码

> [root@liumiaocn ~]\# echo $?

> 0

> [root@liumiaocn ~]\#


