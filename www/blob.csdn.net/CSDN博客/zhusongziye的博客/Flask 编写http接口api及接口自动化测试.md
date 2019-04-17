# Flask 编写http接口api及接口自动化测试 - zhusongziye的博客 - CSDN博客





2018年03月28日 21:03:43[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：643








## 片言

此文中代码都是笔者工作中源码，所以不会很完整，主要摘常见场景的api片段用以举例说明

另:此文主要针对自动化测试人员，尤其有python基础阅读更佳。

笔者使用 python3.6 + postgresql10 + flask 0.12 的环境

### 场景介绍 

我们要实现后台管理中的用户管理功能，实现对用户的增、修改、删除、查询操作。

文中场景省去登录和用户权限部分，但我会在接口代码片段中进行注释说明

##### 接口：新增用户

（定义访问的http链接地址，以及请求使用的 method）

    http://127.0.0.1:7070/guardian/users


@app.route('/guardian/users', methods=['POST'])# 使用 POST 方法

@require_role('guardian_admin')# 登录用户的角色权限必须是guardian_admin 此处是自定义的更角色管理中的设置管理员角色一致即可。不需要角色权限也可以注释此行即可

@login_required    # 验证用户登录 flask_login插件

defcreate_user():'''
    下面json串是新增时候前台表单提交的内容以及格式，也就是我们接口测试时候需要添加的测试数据
    {
            "user": {
                "domain_id": "default",
                "enabled": True,
                "name": "tsbc1",
                "password": "111111",
                "realname": "Ray",
                "email": "tsbc@vip.qq.com",
                "tel": "+86-029-88880000",
                "phone": "13800138000",
                "org_id": "7d22d25a-b648-4ad3-a834-2398eb989a9e",
                "org_array":[],
                "comments": "备注信息"
            }
        }
    '''try:
        domain_id = None
        user_name = None
        user_passwd = None
        project_id = None
        real_name = None
        email = None
        phone = None
        org_id = None
        org_array = None
        comments = None        try:# 接收前端请求数据，并使用json.loads 把数据转换成Python可以读格式
            resq_obj = request.data
            resq_json = None            if type(resq_obj)is bytes:
                resq_json = resq_obj.decode()else:
                resq_json = resq_obj
            user_obj = jsonpickle.loads(resq_json)# 此处使用了jsonpickle的lib，使用json lib也是一样的
            logging.debug('create user {0} '.format(user_obj))if'default_project_id'in user_obj['user']:
                project_id = user_obj['user']['default_project_id']

            domain_id = user_obj['user']['domain_id']
            user_name = user_obj['user']['name']
            user_passwd = user_obj['user']['password']except Exception as ee:
            abort(400)#raise eeif domain_id isnot None and user_name isnot None and user_passwd isnot None:# 笔者使用 keystone 进行用户管理 此处在keystone创建帐号，然后获取id,把id及其它信息会存放在自定义的数据库表中
            resp = usermgt.create_user(domain_id, user_name, user_passwd, project_id)# resp返回值是 dict 格式if'id'in resp['user']:try:# 把前端POST的表单值进行变量赋值 传给后台进行存库
                    user_id = resp['user']['id']if'realname'in user_obj['user']:
                        real_name = user_obj['user']['realname']if'email'in user_obj['user']:
                        email = user_obj['user']['email']if'tel'in user_obj['user']:
                        tel = user_obj['user']['tel']if'phone'in user_obj['user']:
                        phone = user_obj['user']['phone']if'org_id'in user_obj['user']:
                        org_id = user_obj['user']['org_id']if'org_array'in user_obj['user']:
                        org_array = user_obj['user']['org_array']if'comments'in user_obj['user']:
                        comments = user_obj['user']['comments']except Exception as e:# raise e
                    abort(400)if user_obj isnot None:try:# 调用方法进行后台存库
                        GuardianModelDao._save_update_users_info(user_id, user_name, real_name, email, tel, phone, org_id, org_array, comments)return wrap_response(SUCCESS_MSG,200)# 执行成功 返回操作成功信息及状态码 200except Exception as e:
                        LOG.debug(e)
                        abort(500)# 否则 后台500 错误else:
                abort(500)



##### 后台函数

用于实现对接口中传递数据进行处理入库、修改、查询等等。
@classmethoddef_save_update_users_info(cls, user_id, username, 
    real_name, email, 
    tel, phone, org_id, 
    org_array, comments):
    LOG.debug('start create users info...')try:with TxConn()as conn:# 连接数据库with conn.cursor()as cursor:# save user data
                now_time = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())# 接收变量赋值 user_data
                users_data ={"user_id":user_id,"username":username,"realname":real_name,"email":email,"tel":tel,"phone":phone,"org":org_id,"org_array":org_array,"comments":comments,"now_time":now_time                }
                cursor.execute(cls.insert_users_info, users_data)# 插入数据
            conn.commit()except Exception as e:
        LOG.exception('error saving users info', e)raise e
##### 测试脚本（新增用户）

python unittest做单元测试



import unittest, requests

classGuardianTestCase(unittest.TestCase):
    guardian_base ='http://127.0.0.1:7070'
    user_token = keystone.user_token('default','tsbc','123456')# 调用keystone生成登录用户的tokendeftest_create_user_api(self):# 添加对应headers 及 tonken 用于数据传参和登录认证使用
            myheaders ={'Content-Type':'application/json'}
            myheaders['X-Subject-Token']= self.user_token            # 接口测试数据
            user ={"user":{"domain_id":"default","enabled":True,"name":"tsbc1","password":"123456","realname":"Ray","email":"tsbc@123.com","tel":"+86-029-88880000","phone":"13800138000","org_id":"7d22d25a-b648-4ad3-a834-2398eb989a9e","org_array":None,"comments":"备注信息"}}# 使用requests的post方法进行请求路由
            _resp = requests.post(self.guardian_base +'/guardian/users', json=user, headers=myheaders, verify=True)
            resp_json = _resp.content            # 对返回结果及状态进行校验if _resp.ok:
                self.assertIsNotNone(resp_json)
                LOG.info('create users, response: {0}'.format(resp_json))else:
                LOG.error('error create users, response: {0}, status: {1}'.format(resp_json, _resp.status_code))
##### 接口：修改用户

修改用户和新增用户传递的参数差不多，使用的是http的patch请求，route要添加user_id的参数 ，其他实现和新增差不多代码不重复贴了
@app.route('/guardian/users/<string:user_id>', methods=['PATCH'])@require_role('guardian_admin')@login_requireddefupdate_user(user_id):"""
    此处省略接收请求数据代码
    """try:
        resp = usermgt.update_user(domain_id, user_id, username, user_passwd, project_id, enabled)# 更新keystone 用户密码、状态if resp isnot None:# 更新用户基本信息与新增是同一个接口，执行SQL时使用PostGresql特性，相同user_id数据只更新不会再插入
            GuardianModelDao._save_update_users_info(user_id, username, real_name, email, tel, phone, org_id, org_array, comments)return wrap_response(SUCCESS_MSG,200)# 更新成功返回 200except Exception as e:
        LOG.exception('error modify user', e)
        abort(500)# 更新失败500
##### 测试脚本(修改用户)
deftest_update_user_api(self):

    myheaders ={'Content-Type':'application/json'}
    myheaders['X-Subject-Token']= self.user_token

    user ={"user":{"domain_id":"default","enabled":True,"name":"tsbc1","password":"111111","realname":"xioohua","email":"haoydun@123.com","tel":"+86-029-88880000","phone":"13800138000","org_id":"7d22d25a-b648-4ad3-a834-2398eb989a9e","comments":"备注信息"}}

    _resp = requests.patch(self.guardian_base +'/guardian/users/0d26176579e94d558c4c2d864b003241',# 路由中添加要修改的用户ID requests使用 patch方法
                            json=user, headers=myheaders, verify=True)
    resp_json = _resp.content    # 返回值进行校验if _resp.ok:
        self.assertIsNotNone(resp_json)
        LOG.info('update users, response: {0}'.format(resp_json))else:
        LOG.error('error update users, response: {0}, status: {1}'.format(resp_json, _resp.status_code))
##### 接口：查询用户

根据用户名查询用户，使用http的get请求



@app.route('/guardian/users/<string:user_name>', methods=['GET'])# 查询使用 GET 方法@require_role('guardian_admin')@login_requireddefquery_user_by_username(user_name):try:
        user_info = GuardianModelDao._query_users_info_by_name(user_name)# 调用后台查询函数if len(user_info):return wrap_response(jsonpickle.dumps(user_info),200)# 成功返回200 并把返回内容转换成 json格式else:return wrap_response("[]",200)# 否则返回空except Exception as e:
        LOG.exception('error query user by name', e)
        abort(500)
##### 后台函数



@classmethoddef_query_users_info_by_id(cls, user_id):
    LOG.debug('start list users info...')
    users =[]with TxConn()as conn:try:with conn.cursor()as cursor:
                data =(user_id,)
                cursor.execute(cls.query_users_info_by_id, data)# 查询结构如果是行数据，放进list中进行返回for(user_id, real_name, email, tel, phone, org_id, org_array, comments, create_time, update_time)in cursor:
                    user = Users(user_id, real_name, email, tel, phone, org_id, org_array, comments, create_time, update_time)
                    users.append(user)except Exception as e:
            LOG.exception('error query users !')raise e    return users
##### 测试脚本(查询用户)
deftest_query_user_api(self):

    myheaders ={'Content-Type':'application/json'}
    myheaders['X-Subject-Token']= self.user_token

    _resp = requests.get(self.guardian_base +'/guardian/users/tsbc', headers=myheaders, verify=True)# 链接中加入用户名
    resp_json = _resp.text    # 验证判断if _resp.ok:
        self.assertIsNotNone(resp_json)
        LOG.info('query users, response: {0}'.format(resp_json))else:
        LOG.error('error query users, response: {0}, status: {1}'.format(resp_json, _resp.status_code))


