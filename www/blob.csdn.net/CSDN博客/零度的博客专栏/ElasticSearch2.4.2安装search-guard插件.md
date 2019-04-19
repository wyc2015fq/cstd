# ElasticSearch2.4.2安装search-guard插件 - 零度的博客专栏 - CSDN博客
2018年07月18日 12:00:10[零度anngle](https://me.csdn.net/zmx729618)阅读数：417
(1)环境;
elasticsearch 2.4.2
(2)：github上分支没有2.4.x版本的分支，笔者一度找了好久才找到下面一个版本关系对应表
elasticsearch与search-guard以及search-guard-ssl：
![这里写图片描述](https://img-blog.csdn.net/20170406164034681?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlqaWJvNTAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（3）： 进入elasticsearch的bin目录，执行命令安装search-guard
`./plugin install -b com.floragunn/search-guard-2/2.4.2.10`
![这里写图片描述](https://img-blog.csdn.net/20170406164149261?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlqaWJvNTAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
注：当es的版本大于2.2时，安装过程中可能会有WARNING提示，这个不影响。其它ES版本，请对照表格替换命令行后的版本号即可。
(4):安装 search-guard-ssl
`./plugin install -b com.floragunn/search-guard-ssl/2.4.2.10`
(5)安装完成后，会在plugins目录中生成两个目录
![这里写图片描述](https://img-blog.csdn.net/20170406164342216?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlqaWJvNTAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
(6):下载 searchguard-ssl 的包，里面包含自动创建证书的脚本：
```
wget https://github.com/floragunncom/search-guard-ssl/archive/v2.4.2.19.zip
unzip v2.4.2.19.zip 
cd search-guard-ssl-2.4.2.19/example-pki-scripts/
```
![这里写图片描述](https://img-blog.csdn.net/20170406164459515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlqaWJvNTAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
编辑脚本 vim example.sh
原始example.sh文件内容：
```
set -e
./clean.sh
./gen_root_ca.sh capass changeit
./gen_node_cert.sh 0 changeit capass && ./gen_node_cert.sh 1 changeit capass &&  ./gen_node_cert.sh 2 changeit capass
./gen_client_node_cert.sh spock changeit capass
./gen_client_node_cert.sh kirk changeit capass
```
修改后example.sh中文件内容
```
set -e
./clean.sh
./gen_root_ca.sh elastic elastic
./gen_node_cert.sh 181 elastic elastic
./gen_node_cert.sh 182 elastic elastic
./gen_node_cert.sh 183 elastic elastic
./gen_client_node_cert.sh admin elastic elastic
cp truststore.jks node-181-keystore.jks /a/apps/elasticsearch-2.4.2/config/
cp truststore.jks admin-keystore.jks /a/apps/elasticsearch-2.4.2/plugins/search-guard-2/sgconfig/
```
运行 ./example.sh命令，会在当前目录生成多个证书文件
![这里写图片描述](https://img-blog.csdn.net/20170406164709394?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlqaWJvNTAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
(7):编辑elasticsearch-2.4.2/config/elasticsearch.yml配置文件 添加如下内容
```
#-------------------search guard config--------------------------
security.manager.enabled: false
searchguard.authcz.admin_dn:
   - "CN=admin, OU=client, O=client, L=Test, C=DE"
#上面的admin是要和example.sh中的对应上
#-------------------search guard ssl----------------------------------------
#------------------------transport layer SSL------------------------------------
searchguard.ssl.transport.enabled: true
searchguard.ssl.transport.keystore_filepath: node-181-keystore.jks
searchguard.ssl.transport.keystore_password: elastic
searchguard.ssl.transport.truststore_filepath: truststore.jks
searchguard.ssl.transport.truststore_password: elastic
searchguard.ssl.transport.enforce_hostname_verification: false
searchguard.ssl.transport.resolve_hostname: false
searchguard.ssl.http.enabled: true       #设置成true浏览器也无法访问，测试请改为false
searchguard.ssl.http.keystore_filepath: node-181-keystore.jks
searchguard.ssl.http.keystore_password: elastic
searchguard.ssl.http.truststore_filepath: truststore.jks
searchguard.ssl.http.truststore_password: elastic
searchguard.allow_all_from_loopback: true
```
(8):在elasticsearch根目录 执行命令 将配置插入
./plugins/search-guard-2/tools/sgadmin.sh -cn 集群名称 -h hostname -cd plugins/search-guard-2/sgconfig -ks plugins/search-guard-2/sgconfig/admin-keystore.jks -kspass elastic -ts plugins/search-guard-2/sgconfig/truststore.jks -tspass elastic -nhnv
```
./plugins/search-guard-2/tools/sgadmin.sh -cn admin -h 172.100.50.181-cd plugins/search-guard-2/sgconfig -ks plugins/search-guard-2/sgconfig/admin-keystore.jks -kspass elastic -ts plugins/search-guard-2/sgconfig/truststore.jks -tspass elastic -nhnv
```
![这里写图片描述](https://img-blog.csdn.net/20170406164822489?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlqaWJvNTAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
需要注意：
如果提示没有操作权限，则必须先把hash.sh文件的权限开放
`chmod -R 777 ./plugins/search-guard-2/tools/sgadmin.sh`
如成功写入配置，则会显示以下信息：
![这里写图片描述](https://img-blog.csdn.net/20170406164927874?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlqaWJvNTAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
9):search guard配置文件配置
search-guard中的用户权限管理
相关配置文件的介绍
searchguard 主要有5个配置文件在plugins/search-guard-2/sgconfig 下：
1、sg_config.yml：主配置文件不需要做改动。
2、sg_internal_users.yml：本地用户文件，定义用户密码以及对应的权限。
3、sg_roles.yml：权限配置文件
4、sg_roles_mapping.yml:定义用户的映射关系
5、sg_action_groups.yml：定义权限
修改内置用户密码，然后再运行一次search guard 配置写入命令。
1.则先用plugins/search-guard-2/tools/hash.sh生成hash字符串，生成密码：
![这里写图片描述](https://img-blog.csdn.net/20170406165019952?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlqaWJvNTAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
先修改权限。再生成密码。
2：将字符串复制到sg_internal_users.yml文件的对应用户密码位置，在密码下面记得写入原密码的提示，难保你那天忘记了。
`vim plugins/search-guard-2/sgconfig/sg_internal_users.yml`
![这里写图片描述](https://img-blog.csdn.net/20170406165137703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlqaWJvNTAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3：添加用户权限
vim sg_roles_mapping.yml
![这里写图片描述](https://img-blog.csdn.net/20170406165221181?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlqaWJvNTAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4:重新写入配置文件
先回到elasticsearch的安装文件夹
执行命令
```
./plugins/search-guard-2/tools/sgadmin.sh -cn admin -h 172.100.50.181-cd plugins/search-guard-2/sgconfig -ks plugins/search-guard-2/sgconfig/admin-keystore.jks -kspass elastic -ts plugins/search-guard-2/sgconfig/truststore.jks -tspass elastic -nhnv
```
5:测试：
curl -XGET “http://elastic:123456@172.100.50.181:9200”
![这里写图片描述](https://img-blog.csdn.net/20170406165328886?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlqaWJvNTAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
现在每次想访问你网站的9200端口都必须要有搜索认证的保护了。
![这里写图片描述](https://img-blog.csdn.net/20170406165409782?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlqaWJvNTAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
注：admin 为集群名称。
番外篇：
集群部署：
配置好一台机器后，需要把这台机器中的plugins文件夹中的下面两个文件夹cp到其它节点
![这里写图片描述](https://img-blog.csdn.net/20170406165539685?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlqaWJvNTAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后把
![这里写图片描述](https://img-blog.csdn.net/20170406165610283?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlqaWJvNTAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这两个文件cp到对应机器的elasticsearch安装目录的config文件夹下。
修改其它节点的elasticsearch.yml配置文件，参考本机器。至此，集群部署成功。
测试问题：
(1):集群名称不能包含“-”符号，如es-kz，这个集群名称无法生成searchguard索引。
(2):测试时，把searchguard.ssl.http.enabled: true设置成false,设置成true浏览器也无法访问
(3):如果elasticsearch添加了searchguard权限控制，则kibana，logstash等就不能再连接上es了，请参考下两篇文章解决该问题
