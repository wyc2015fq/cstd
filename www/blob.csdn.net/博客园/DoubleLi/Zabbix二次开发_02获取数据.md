# Zabbix二次开发_02获取数据 - DoubleLi - 博客园






最近准备写一个zabbix二次页面的呈现。打算调用zabbix api接口来进行展示。

具体流程以及获取的数据。


    1、  获得认证密钥
    2、  获取zabbix所有的主机组
    3、  获取单个组下的所有主机
    4、  获取某个主机下的所有监控项
    5、  获取某个监控项的历史数据
    6、  获取某个监控项的最新数据




# 认证获取


**user.login方法获取zabbix server的认证结果****官方地址：**[https://www.zabbix.com/documentation/2.2/manual/api/reference/user/login](https://www.zabbix.com/documentation/2.2/manual/api/reference/user/login)
**python****脚本：**
|1234567891011121314151617181920212223242526272829303132|[iyunv@yang python]# cat auth.py#!/usr/bin/env python2.7#coding=utf-8import jsonimport urllib2# based url and required headerurl = "http://1.1.1.1/zabbix/api_jsonrpc.php"header = {"Content-Type":"application/json"}# auth user and passworddata = json.dumps({   "jsonrpc": "2.0",   "method": "user.login",   "params": {   "user": "Admin",   "password": "zabbix"},"id": 0})# create request objectrequest = urllib2.Request(url,data)for key in header:   request.add_header(key,header[key])# auth and get authidtry:   result = urllib2.urlopen(request)except URLError as e:   print "Auth Failed, Please Check Your Name AndPassword:",e.codeelse:   response = json.loads(result.read())   result.close()print"Auth Successful. The Auth ID Is:",response['result']|
|----|----|



**python****脚本运行结果：**
|12|[iyunv@yang python]# python auth.pyAuth Successful. The Auth ID Is: a0b82aae0842c2041386a61945af1180|
|----|----|



**curl****命令：**
|123|curl -i -X POST -H 'Content-Type:application/json' -d '{"jsonrpc":"2.0","method":"user.login","params":{"user":"admin","password":"zabbix"},"auth":null,"id":0}' [http://1.1.1.1/zabbix/api_jsonrpc.php](http://1.1.1.1/zabbix/api_jsonrpc.php)|
|----|----|



**curl****命令运行结果：**
|1|{"jsonrpc":"2.0","result":"b895ce91ba84fe247e444817c6773cc3","id":0}|
|----|----|



# 主机ID获取


**2.     hostgroup.get方法获取所有主机组ID**把认证密钥放到脚本中，每次获取数据时都需要认证。此处是获取zabbix server上的所有主机组名称与ID号。
**官方地址：**[https://www.zabbix.com/documentation/2.2/manual/api/reference/hostgroup/get](https://www.zabbix.com/documentation/2.2/manual/api/reference/hostgroup/get)
**python****脚本：**
|12345678910111213141516171819202122232425262728293031323334353637383940|[iyunv@yang python]# catget_hostgroup_list.py#!/usr/bin/env python2.7#coding=utf-8import jsonimport urllib2# based url and required headerurl = "http://1.1.1.1/zabbix/api_jsonrpc.php"header = {"Content-Type":"application/json"}# request jsondata = json.dumps({   "jsonrpc":"2.0",   "method":"hostgroup.get",   "params":{       "output":["groupid","name"],   },   "auth":"3c0e88885a8cf8af9502b5c850b992bd", # theauth id is what auth script returns, remeber it is string   "id":1,})# create request objectrequest = urllib2.Request(url,data)for key in header:   request.add_header(key,header[key])# get host listtry:   result = urllib2.urlopen(request)except URLError as e:   if hasattr(e, 'reason'):       print 'We failed to reach a server.'       print 'Reason: ', e.reason   elif hasattr(e, 'code'):       print 'The server could not fulfill the request.'       print 'Error code: ', e.codeelse:   response = json.loads(result.read())   result.close()   print "Number Of Hosts: ", len(response['result'])   #print response   for group in response['result']:       print "Group ID:",group['groupid'],"\tGroupName:",group['name']|
|----|----|



**python****脚本执行结果：**
|1234567891011121314|[iyunv@yang python]# pythonget_hostgroup_list.pyNumber Of Hosts:  12Group ID: 11    Group Name: DB ScheduleGroup ID: 14    Group Name: DG-WY-KD-ServerGroup ID: 5     Group Name: Discovered hostsGroup ID: 7     Group Name: HypervisorsGroup ID: 2     Group Name: Linux serversGroup ID: 8     Group Name: monitored_linuxGroup ID: 9     Group Name: qsmindGroup ID: 12    Group Name: qssecGroup ID: 13    Group Name: switchGroup ID: 1     Group Name: TemplatesGroup ID: 6     Group Name: Virtual machinesGroup ID: 4     Group Name: Zabbix servers|
|----|----|



**curl****命令：**
|1|curl -i -X POST -H 'Content-Type:application/json' -d '{"jsonrpc": "2.0","method":"hostgroup.get","params":{"output":["groupid","name"]},"auth":"11d2b45415d5de6770ce196879dbfcf1","id": 0}' [http://1.1.1.1/zabbix/api_jsonrpc.php](http://1.1.1.1/zabbix/api_jsonrpc.php)|
|----|----|



**curl****执行结果：**
|1|{"jsonrpc":"2.0","result":[{"groupid":"11","name":"DBSchedule"},{"groupid":"14","name":"DG-WY-KD-Server"},{"groupid":"5","name":"Discoveredhosts"},{"groupid":"7","name":"Hypervisors"},{"groupid":"2","name":"Linuxservers"},{"groupid":"8","name":"monitored_linux"},{"groupid":"9","name":"qsmind"},{"groupid":"12","name":"qssec"},{"groupid":"13","name":"switch"},{"groupid":"1","name":"Templates"},{"groupid":"6","name":"Virtualmachines"},{"groupid":"4","name":"Zabbixservers"}],"id":0}|
|----|----|




**host.get方法获取单个主机组下所有的主机ID。**根据标题2中获取到的主机组id，把主机组id填入到下边脚本中，就可以获得该主机组下所有的主机id。
**官方地址：**[https://www.zabbix.com/documentation/2.2/manual/api/reference/host/get](https://www.zabbix.com/documentation/2.2/manual/api/reference/host/get)
**python****脚本：**
|12345678910111213141516171819202122232425262728293031323334353637383940|[iyunv@yang python]# cat get_group_one.py#!/usr/bin/env python2.7#coding=utf-8import jsonimport urllib2# based url and required headerurl = "http://1.1.1.1/zabbix/api_jsonrpc.php"header = {"Content-Type":"application/json"}# request jsondata = json.dumps({   "jsonrpc":"2.0",   "method":"host.get",   "params":{       "output":["hostid","name"],       "groupids":"14",   },   "auth":"3c0e88885a8cf8af9502b5c850b992bd", # theauth id is what auth script returns, remeber it is string   "id":1,})# create request objectrequest = urllib2.Request(url,data)for key in header:   request.add_header(key,header[key])# get host listtry:   result = urllib2.urlopen(request)except URLError as e:   if hasattr(e, 'reason'):       print 'We failed to reach a server.'       print 'Reason: ', e.reason   elif hasattr(e, 'code'):       print 'The server could not fulfill the request.'       print 'Error code: ', e.codeelse:   response = json.loads(result.read())   result.close()   print "Number Of Hosts: ", len(response['result'])   for host in response['result']:       print "Host ID:",host['hostid'],"HostName:",host['name']|
|----|----|



**python****脚本执行结果：**
|123456|[iyunv@yang python]# pythonget_group_one.py  Number Of Hosts:  4Host ID: 10146 Host Name: DG-WY-KD-3F3B-00Host ID: 10147 Host Name: DG-WY-KD-3F3B-01Host ID: 10148 Host Name: DG-WY-KD-3F3B-02Host ID: 10149 Host Name: DG-WY-KD-3F3B-03|
|----|----|



**curl****命令：**
|12|curl -i -X POST -H'Content-Type: application/json' -d '{"jsonrpc":"2.0","method":"host.get","params":{"output":["hostid","name"],"groupids":"14"},"auth":"11d2b45415d5de6770ce196879dbfcf1","id": 0}' [http://1.1.1.1/zabbix/api_jsonrpc.php](http://1.1.1.1/zabbix/api_jsonrpc.php)|
|----|----|



**curl****命令执行结果：**
|1|{"jsonrpc":"2.0","result":[{"hostid":"10146","name":"DG-WY-KD-3F3B-00"},{"hostid":"10147","name":"DG-WY-KD-3F3B-01"},{"hostid":"10148","name":"DG-WY-KD-3F3B-02"},{"hostid":"10149","name":"DG-WY-KD-3F3B-03"}],"id":0}|
|----|----|



# 获取主机数据

** itemsid.get方法获取单个主机下所有的监控项ID**根据标题3中获取到的所有主机id与名称，找到你想要获取的主机id，获取它下面的所有items。
**官方地址：**[https://www.zabbix.com/documentation/2.2/manual/api/reference/item](https://www.zabbix.com/documentation/2.2/manual/api/reference/item)
**python****脚本：**
|1234567891011121314151617181920212223242526272829303132333435363738394041|[iyunv@yang python]# cat get_items.py#!/usr/bin/env python2.7#coding=utf-8import jsonimport urllib2# based url and required headerurl = "http://1.1.1.1/zabbix/api_jsonrpc.php"header = {"Content-Type":"application/json"}# request jsondata = json.dumps({   "jsonrpc":"2.0",   "method":"item.get",   "params":{       "output":["itemids","key_"],       "hostids":"10146",   },   "auth":"3c0e88885a8cf8af9502b5c850b992bd", # theauth id is what auth script returns, remeber it is string   "id":1,})# create request objectrequest = urllib2.Request(url,data)for key in header:   request.add_header(key,header[key])# get host listtry:   result = urllib2.urlopen(request)except URLError as e:   if hasattr(e, 'reason'):       print 'We failed to reach a server.'       print 'Reason: ', e.reason   elif hasattr(e, 'code'):       print 'The server could not fulfill the request.'       print 'Error code: ', e.codeelse:   response = json.loads(result.read())   result.close()   print "Number Of Hosts: ", len(response['result'])   for host in response['result']:       print host       #print "Host ID:",host['hostid'],"HostName:",host['name']|
|----|----|



**python****脚本运行结果：**
|12345678910|[iyunv@yang python]# python get_items.pyNumber Of Hosts:  54{u'itemid': u'24986', u'key_':u'agent.hostname'}{u'itemid': u'24987', u'key_':u'agent.ping'}{u'itemid': u'24988', u'key_':u'agent.version'}{u'itemid': u'24989', u'key_':u'kernel.maxfiles'}{u'itemid': u'24990', u'key_':u'kernel.maxproc'}{u'itemid': u'25157', u'key_':u'net.if.in[eth0]'}{u'itemid': u'25158', u'key_':u'net.if.in[eth1]'}… …|
|----|----|



**curl****命令：**
|12|curl -i -X POST -H 'Content-Type:application/json' -d '{"jsonrpc":"2.0","method":"item.get","params":{"output":"itemids","hostids":"10146","search":{"key_":"net.if.out[eth2]"}},"auth":"11d2b45415d5de6770ce196879dbfcf1","id": 0}' [http://1.1.1.1/zabbix/api_jsonrpc.php](http://1.1.1.1/zabbix/api_jsonrpc.php)#此处加上了单个key的名称|
|----|----|



**curl****命令执行结果：**
|1|{"jsonrpc":"2.0","result":[{"itemid":"25154"}],"id":0}|
|----|----|



**history.get方法获取单个监控项的历史数据**根据第4项的获取到的所有items id的值，找到想要监控的那项，获取它的历史数据。
**官方地址：**[https://www.zabbix.com/documentation/2.2/manual/api/reference/history/get](https://www.zabbix.com/documentation/2.2/manual/api/reference/history/get)
**python****脚本：**
|12345678910111213141516171819202122232425262728293031323334353637383940414243|[iyunv@yang python]# catget_items_history.py#!/usr/bin/env python2.7#coding=utf-8import jsonimport urllib2# based url and required headerurl = "http://1.1.1.1/zabbix/api_jsonrpc.php"header = {"Content-Type":"application/json"}# request jsondata = json.dumps({   "jsonrpc":"2.0",   "method":"history.get",   "params":{       "output":"extend",       "history":3,       "itemids":"25159",       "limit":10   },   "auth":"3c0e88885a8cf8af9502b5c850b992bd", # theauth id is what auth script returns, remeber it is string   "id":1,})# create request objectrequest = urllib2.Request(url,data)for key in header:   request.add_header(key,header[key])# get host listtry:   result = urllib2.urlopen(request)except URLError as e:   if hasattr(e, 'reason'):       print 'We failed to reach a server.'       print 'Reason: ', e.reason   elif hasattr(e, 'code'):       print 'The server could not fulfill the request.'       print 'Error code: ', e.codeelse:   response = json.loads(result.read())   result.close()   print "Number Of Hosts: ", len(response['result'])   for host in response['result']:       print host       #print "Host ID:",host['hostid'],"HostName:",host['name']|
|----|----|



**python****脚本执行结果：**
|123456789101112|[iyunv@yang python]# pythonget_items_history.pyNumber Of Hosts:  10{u'itemid': u'25159', u'ns': u'420722133',u'value': u'3008', u'clock': u'1410744079'}{u'itemid': u'25159', u'ns': u'480606614',u'value': u'5720', u'clock': u'1410744139'}{u'itemid': u'25159', u'ns': u'40905600',u'value': u'6144', u'clock': u'1410744200'}{u'itemid': u'25159', u'ns': u'175337062',u'value': u'2960', u'clock': u'1410744259'}{u'itemid': u'25159', u'ns': u'202705084',u'value': u'3032', u'clock': u'1410744319'}{u'itemid': u'25159', u'ns': u'263158421',u'value': u'2864', u'clock': u'1410744379'}{u'itemid': u'25159', u'ns': u'702285081',u'value': u'7600', u'clock': u'1410744439'}{u'itemid': u'25159', u'ns': u'231191890',u'value': u'3864', u'clock': u'1410744499'}{u'itemid': u'25159', u'ns': u'468566742',u'value': u'3112', u'clock': u'1410744559'}{u'itemid': u'25159', u'ns': u'421679098',u'value': u'2952', u'clock': u'1410744619'}|
|----|----|



**curl****命令：**
|1|curl -i -X POST -H 'Content-Type:application/json' -d '{"jsonrpc":"2.0","method":"history.get","params":{"history":3,"itemids":"25154","output":"extend","limit":10},"auth":"11d2b45415d5de6770ce196879dbfcf1","id": 0}' [http://1.1.1.1/zabbix/api_jsonrpc.php](http://1.1.1.1/zabbix/api_jsonrpc.php)|
|----|----|



**curl****命令运行结果：**
|1|{"jsonrpc":"2.0","result":[{"itemid":"25154","clock":"1410744134","value":"4840","ns":"375754276"},{"itemid":"25154","clock":"1410744314","value":"5408","ns":"839852515"},{"itemid":"25154","clock":"1410744374","value":"7040","ns":"964558609"},{"itemid":"25154","clock":"1410744554","value":"4072","ns":"943177771"},{"itemid":"25154","clock":"1410744614","value":"8696","ns":"995289716"},{"itemid":"25154","clock":"1410744674","value":"6144","ns":"992462863"},{"itemid":"25154","clock":"1410744734","value":"6472","ns":"152634327"},{"itemid":"25154","clock":"1410744794","value":"4312","ns":"479599424"},{"itemid":"25154","clock":"1410744854","value":"4456","ns":"263314898"},{"itemid":"25154","clock":"1410744914","value":"8656","ns":"840460009"}],"id":0}|
|----|----|




**history.get方法获取单个监控项最后的值**只需把上个脚本中或curl中的limit参数改为1就可。


这样的话需要监控的数据就拿到了，下一步的就是前端页面的展示了。



from:https://www.cnblogs.com/yangliheng/p/6394676.html









