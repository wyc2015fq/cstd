# Jenkins-API使用（python） - zhusongziye的博客 - CSDN博客





2019年01月12日 14:19:51[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：88







- 
**环境配置**


jenkinsapi、python-jenkins、pbr、multi-key-dict
- 
**API****参考**


**https://blog.csdn.net/seeeees**

**https://python-jenkins.readthedocs.io/en/latest/api.html**



**class jenkins.Jenkins(*****server_url,username,password*****)**
|server_url|jenkins服务器地址|
|----|----|
|password|jenkins password|
|timeout|超时时间可选|



**get****_job_info(*****name, depth=0, fetch_all_builds=False*****) #****获取****job****的信息**
|name|job name|
|----|----|
|depth||
|fetch_all_builds||



**get_job_name(*****name*****) #****验证项目是否存在**
|存在|返回job名称|
|----|----|
|不存在|None|



**debug_job_info() #****测试job信息**



**get_build_info(*****name, number, depth=0*****)  #****获取构建信息**
|name|job_name|
|----|----|
|number|build numberget_job_info(job_name)['lastBuild']['number']|
|depth||





**get_whoami()    #****当前用户**

**get_version()   #****当前版本**

**get_jobs()    #****返回所有job**



**copy_job(fromname,toname)   #****复制job**

**rename_job(fromname,toname)    #****重命名job**

**delete_job(name)    #****删除job**

**enable_job(name)    #****启动job**

**disable_job(name)    #****关闭job**

**jobs_count()    #jenkins job count**

**jobs_exists(name)   #job ****是否存在 存在true 不存在none**

**assert_job_exist(name)    #job****不存在 异常**

**create_job(name,config_xml)**
|name|jobname|
|----|----|
|config_xml|jenkins.EMPTY_CONFIG_XML|

**get_job_config(name)    #****获取job配置文件**

**reconfig_job(name)    #****重新配置job配置文件**

**build_job_url(name,parameters=None, token=None)  #****获取job构建url**
|name|job_name|
|----|----|
|parameters|参数|
|token|user token|



**build_job(name,parameters=None,token=None)  #****构建job**
|name|job_name|
|----|----|
|parameters|参数|
|token|user token|



 server.get_build_info(job_name,last_build_number)

key_list
|[u'mavenVersionUsed', u'number', u'actions', u'result', u'duration', u'id', u'builtOn', u'artifacts', u'mavenArtifacts', u'keepLog', u'_class', u'description', u'changeSet', u'timestamp', u'culprits', u'estimatedDuration', u'building', u'queueId', u'displayName', u'url', u'executor', u'fullDisplayName']|
|----|



