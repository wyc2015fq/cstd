# Mac Security工具使用总结find-identity - starRTC免费im直播会议一对一视频 - CSDN博客
2017年06月29日 11:26:20[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：536
Security是Mac系统中钥匙串和安全模块的命令行管理工具，（图形化工具为Keychain Access.app）。钥匙串（Keychain）实质上就是一个用于存放证书、密钥、密码等安全认证实体的仓库，在计算机中保存为一个.keychain的文件
使用手册:
命令行执行 man  security 
**Security命令**
语法如下：
**security [-hilqv] [-p prompt] [command] [command options] [command args]**
- [subcommand] -h    帮助，无命令参数时查看所有命令，带命令参数时显示该命令的使用方法
- -i    交互模式
- -p <prompt>    进入交互模式，使用自定义提示符
- -q      减少信息输出
- -v      增加信息输出
- -l      security退出前，调用/usr/bin/leaks -nocontext检查执行的命令是否有泄漏
子命令列表如下
- **create-keychain**    创建钥匙串并加入搜索列表
- list-keychains   显示或设置钥匙串搜索列表
- default-keychain   显示或设置默认的钥匙串 
- login-keychain     显示或设置登录钥匙串
- delete-keychain    删除钥匙串并从搜索列表移除
- **lock-keychain**    锁定制定的钥匙串
- **unlock-keychain**    解锁制定的钥匙串
- set-keychain-settings    设置钥匙串配置
- set-keychain-password   设置钥匙串密码
- dump-keychain    显示一个或多个钥匙串的内容
- create-keypair     创建非对称密钥对
- add-generic-password    向钥匙串中添加通用密码项
- find-generic-password    查找通用密码项
- delete-generic-password   删除通用密码项
- add-internet-password   向钥匙串中添加网络密码项
- find-internet-password   查找网络密码项
- delete-internet-password   删除网络密码项
- **add-certificates**     向钥匙串种添加证书
- **find-certificate**    查找证书
- **delete-certificate**   从钥匙串种删除证书
- **find-identity**     查找认证实体（证书+私钥）
- set-identity-preference  
- get-identity-preference
- create-db
- export
- import
- **cms**     编码或解码CMS信息（PKCS#7）
- install-mds    安装/重装MDS 数据库
- add-trusted-cert    添加可信证书（只包含公钥，无私钥）
- remove-trusted-cert  删除可信证书
- dump-trusted-setting   显示信任配置
- user-trust-settings-enable    显示或管理用户级别的信任配置
- trust-settings-export   导出信任配置
- trust-settings-import   导入信任配置
- verify-cert    验证证书
- authorize    授权操作
- authorizationdb   变更授权策略数据库
- execute-with-privileges   带特权执行工具
- leaks     在本进程中运行/usr/bin/leaks
- error     显示指定错误码的描述信息
- create-filevaultmaster-keychain   创建一个带密钥对的钥匙串，用于FileVault恢复（FileVault是苹果系统里的一项保密机制，会自动透明地对主目录的内容进行实时加密和解密） 
**创建钥匙串**
create-keychain [-P] [-p <password>]   [keychainFiles...]
- -p <password>      设置新创建的钥匙串的密码
- -P    调用SecurityAgent图形化程序设置密码

**向钥匙串中添加钥匙（认证实体）**
import <inputfile>  [-k <keychain>] [-t <type>]  [-f format] [-w]  [-P passphrase] [options...]
- **-k <keychain>**    指定要导入项目到哪个钥匙串中
- **-t  <type>**     指定要导入的项目类型，可取值为：** pub | priv | session | cert | agg**
- **-f  <format>**    指定导入项目的格式，可取值为：openssl | openssl1 | openssl2 | bsafe | raw | pkcs7 | pkcs8
 | pkcs12 | netscape | pemseq
- -w     标明包装了私钥，导入时要解开
- -x     标明导入后，私钥无法提取私钥
- **-P  <password>**    直接输入导入项目密码，默认会使用GUI输入密码
- -a  指定键值对属性，可以重复出现多次
- **-A**    所有程序可以使用导入的项目 
- **-T**  指定可以使用导入项目的程序，可以重复出现多次
security create-keychain -p
123456 certsOfAppBuild.keychainsecurity
 unlock-keychain -p 123456 certsOfAppBuild.keychain
**解锁钥匙串**security -k certsOfAppBuild.keychain
 import xxx.p12 -P 111111 -T /usr/bin/codesign
...sign operation...
security delete-keychain certsOfAppBuild.keychian
**删除证书**
delete-certificate [-c name] [-Z hash] [-t]  [keychain...]
- -c  <name>   按common name删除证书
- -Z  <hash>    按SHA1哈希值删除证书
- -t                  同时删除该证书的用户信任设置
- 要删除的证书必须能够唯一确定，如果未指定钥匙串则是用默认搜索列表中的所有钥匙串
**查找认证实体（证书＋私钥）**
find-identity [-p policy] [-s string] [-v] [keychain...]
- -p    按指定用途策略查找，可重复出现以指定多项，可用值有： basic, ssl-client, ssl-server, smime, eap, ipsec, ichat, **codesigning**, sys-default, sys-kerberos-kdc
 ,macappstore, appleID 
- -s   为-p指定的策略提供参数
- **-v**  只显示有效的，默认显示所有。
- 如果未指定keychain，则使用默认的搜索列表
security
find-identity -v -p codesigning #显示默认搜索列表中有效地认证实体
问题：
security find-identity 0 valid identities found
不能用root运行！

**codesign使用keychain中的Identity(证书+私钥)进行签名**
指定Identity的选项为 -s  <identity>
- 如果identiy正好是40位16进制数，则按证书的**SHA-1指纹**查找
- 如果identity为单个横线 "-"，则执行ad-hoc签名，不会用到identity。
- 首先查找keychain identity preference的full name
- 在keychain中查找Subject的common name，找到多个则失败。大小写敏感，完整匹配比部分匹配更好。多个keychain中包含同一个证书也是可以的。
 关于codesign的更多用法可以查看帮助文档（命令行执行：man codesign）。
示例
codesign -s
'iPhone Developer: Thomas Kollbach (7TPNXN7G6K)'
 Example.appcodesign -s 9c8b212f6a2c2382847b104e387a01b246d4ce42 Example.app

