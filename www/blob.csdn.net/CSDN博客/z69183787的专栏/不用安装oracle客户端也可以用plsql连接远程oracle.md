# 不用安装oracle客户端也可以用plsql连接远程oracle - z69183787的专栏 - CSDN博客
2013年12月21日 12:34:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3218
不用安装oracle客户端也可以用plsql连接远程oracle
关键字: plsql连接远程oracle 
    每次问人家，plsql 可不可以直接远程连接服务器，他们都说要安装客户端，记得以前用过个什么sql deveplore 什么的工具，不用安装客户端的，而且是绿色软件来的，我就想，一定就办法，不用这么麻烦安装客户端的。上网找了找，下面把我的实践详细过程记录下来，希望对大家有帮助，有什么不对的，请大家多多评论，指出来。 
1、在安装ORACLE服务器的机器上搜索下列文件， 
oci.dll 
ocijdbc10.dll 
ociw32.dll 
orannzsbb10.dll 
oraocci10.dll 
oraociei10.dll 
sqlnet.ora 
tnsnames.ora 
classes12.jar 
ojdbc14.jar 
放到一个文件夹,如 oraclient 
2、配置tnsnames.ora，如果本机上没有安装oracle， 
可以从安装了oracle的机上拷贝一个(tnsnames.ora文件在%ORACLE_HOME%/network/admin下)放在上面的目录D:/oraclient下。 
oracledata = 
(DESCRIPTION = 
    (ADDRESS_LIST = 
      (ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.0.58)(PORT = 1521)) 
    ) 
    (CONNECT_DATA = 
      (SERVICE_NAME = oracledata) 
    ) 
) 
3、添加一个环境变量，名为TNS_ADMIN，值为tnsnames.ora文件所在路径（如：D:/oraclient，特别是重装后或其它操作，忘了TNS_ADMIN变量，plsql登陆就会报无法解析指定的连接标识符)，这是为了能够找到上面说的tnsnames.ora。如果本机上安装了ORACLE，并且设置了ORACLE_HOME环境变量，那么会自动在%ORACLE_HOME%/network/admin/位置查找tnsnames.ora文件。 
4、设置ORACLE的语言，添加环境变量： 
NLS_LANG =SIMPLIFIED CHINESE_CHINA.ZHS16GBK，(AMERICAN_AMERICA.US7ASCII 是ASCII编码类型，其它类型可自己到服务器看一下或网上查找一下)
