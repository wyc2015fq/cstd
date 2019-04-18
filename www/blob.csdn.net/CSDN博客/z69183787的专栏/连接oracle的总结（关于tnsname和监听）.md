# 连接oracle的总结（关于tnsname和监听） - z69183787的专栏 - CSDN博客
2013年10月14日 21:49:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7831
该文是我连接oracle的总结，特别适合于程序开发人员与oracle菜鸟 
如何配置才能使客户端连到数据库： 
要使一个客户端机器能连接oracle数据库，需要在客户端机器上安装oracle的客户端软件，唯一的例外就是java连接数据库的时候，可以用jdbc thin模式，不用装oracle的客户端软件。加入你在机器上装了oracle数据库，就不需要在单独在该机器上安装oracle客户端了，因为装oracle数据库的时候会自动安装oracle客户端。 
用过sql server数据库然后又用oracle的新手可能会有这样的疑问：问什么我用sql server的时候不用装sql server的客户端呢？原因很简单，sql server也是microsoft的，它在操作系统中集成了sql server客户端，如果microsoft与oracle有协议，将oracle客户端也集成到操作系统中，那我们也就不用在客户端机器装oracle客户端软机就可访问数据库了，不过，这好像是不可能实现的事情。 
也有的人会问：为什么在sql server中没有侦听端口一说，而在oracle中要配置侦听端口？其实sql server中也有侦听端口，只不过microsoft将侦听端口固定为1433，不允许你随便改动，这样给你一个错觉感觉sql server中没有侦听端口，咳，microsoft把太多的东西都封装到黑盒子里，方便使用的同时也带来的需要副作用。而oracle中的侦听端口直接在配置文件中，允许随便改动，只不过无论怎样改动，要与oracle服务器端设置的侦听端口一致。 
好，言归正传，我们如何做才能使客户端机器连接到oracle数据库呢？ 
A． 安装相关软件 
B． 进行适当的配置 
A．在适当的位置安装适当的软件： 
在客户端机器： 
1．在客户端机器上安装ORACLE的Oracle Net通讯软件，它包含在oracle的客户端软件中。 
2．正确配置了sqlnet.ora文件：  
NAMES.DIRECTORY_PATH = (TNSNAMES, ….)  
NAMES.DEFAULT_DOMAIN=DB_DOMAIN  
一般情况下我们不用NAMES.DEFAULT_DOMAIN参数。如果想不用该参数用#注释掉或将该参数删除即可，对于NAMES.DIRECTORY_PATH参数采用缺省值即可，对于NAMES.DEFAULT_DOMAIN参数有时需要注释掉，在下面有详细解释。 
3．正确配置了tnsname.ora文件 
在服务器端机器： 
1．保证listener已经启动 
2．保证数据库已经启动。 
如果数据库没有启动，用： 
Oracle 9i: 
dos>sqlplus “/ as sysdba” 
sqlplus> startup 
Oracle 8i: 
dos>svrmgrl 
svrmgrl>connect internal 
svrmgrl>startup 
命令启动数据库 
如果listener没有启动，用： 
lsnrctl start [listener name] 
lsnrctl status [listener name] 
命令启动listener 
B．进行适当的配置 
如何正确配置tnsname.ora文件： 
可以在客户端机器上使用oracle Net Configuration Assistant或oracle Net Manager图形配置工具对客户端进行配置，该配置工具实际上修改tnsnames.ora文件。所以我们可以直接修改tnsnames.ora文件，下面以直接修改tnsnames.ora文件为例： 
该文件的位置为： …/network/admin/tnsnames.ora (for windows) 
…/network/admin/tnsnames.ora (for unix) 
此处，假设服务器名为testserver，服务名为orcl.testserver.com，使用的侦听端口为1521，则tnsnams.ora文件中的一个test网络服务名（数据库别名）为： 
test = 
(DESCRIPTION= 
(ADDRESS_LIST= 
(ADDRESS=(PROTOCOL=TCP)(HOST=testserver)(PORT=1521)) 
) 
(CONNECT_DATA=(SERVICE_NAME=orcl.testserver.com) 
) 
) 
红色的内容为需要根据实际情况修改的内容，现解释如下： 
PROTOCOL：客户端与服务器端通讯的协议，一般为TCP，该内容一般不用改。 
HOST：数据库侦听所在的机器的机器名或IP地址，数据库侦听一般与数据库在同一个机器上，所以当我说数据库侦听所在的机器一般也是指数据库所在的机器。在UNIX或WINDOWS下，可以通过在数据库侦听所在的机器的命令提示符下使用hostname命令得到机器名，或通过ipconfig(for WINDOWS) or ifconfig（for UNIX）命令得到IP地址。需要注意的是，不管用机器名或IP地址，在客户端一定要用ping命令ping通数据库侦听所在的机器的机器名，否则需要在hosts文件中加入数据库侦听所在的机器的机器名的解析。 
PORT：数据库侦听正在侦听的端口，可以察看服务器端的listener.ora文件或在数据库侦听所在的机器的命令提示符下通过lnsrctl status [listener name]命令察看。此处Port的值一定要与数据库侦听正在侦听的端口一样。 
SERVICE_NAME：在服务器端，用system用户登陆后，sqlplus> show parameter service_name命令察看。 
如何利用配置的网络服务名连接到数据库： 
用sqlplus程序通过test网络服务名进行测试，如sqlplus system/manager@test。如果不能连接到数据库，则在tnsname.ora文件中的test网络服务名（net service）后面加上Oracle数据库的DB_Domain参数值，通过用sqlplus> show parameter db_domain命令察看。此处db_domain参数值为testserver.com，将其加到网络服务名后面，修改后的tnsname.ora中关于该网络服务名的内容为： 
test.testserver.com = 
(DESCRIPTION= 
(ADDRESS_LIST= 
(ADDRESS=(PROTOCOL=TCP)(HOST=testserver)(PORT=1521)) 
) 
(CONNECT_DATA=(SERVICE_NAME=orcl.testserver.com) 
) 
) 
用sqlplus程序通过test.testserver.com网络服务名测试，如sqlplus system/manager@test.testserver.com。 
关于为什们在网络服务名后面加db_domain参数，需要了解sql*plus连接数据库的原理，我在后面解决12154常见故障中给出了详细的说明。 
如果上面的招数还不奏效的话，只好用一下乾坤大挪移了。 
将客户端的网络服务名部分 
test.testserver.com = 
(DESCRIPTION= 
(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST=testserver)(PORT=1521)) 
) 
(CONNECT_DATA=(SERVICE_NAME=orcl.testserver.com) 
) 
) 
拷贝到服务器的tnsnames.ora文件中。然后再服务器端用sqlplus system/manager@test.testserver.com连接到数据库。 
如果能连接成功，说明你的客户端与服务器端的网络有问题。 
如果连接不成功，用前面的部分检查网络服务名部分部分是否正确，如果确信网络服务名部分正确而且所有的客户端都连不上数据库则可能为系统TCP/IP或Oracle系统有问题，建议重新安装数据库。 
常见故障解决办法： 
TNS-12154 (ORA-12154)：TNS:could not resolve service name 
该错误表示用于连接的网络服务名在tnsnames.ora文件中不存在，如上面的tnsnames.ora中的网络服务名只有test，假如用户在连接时用sqlplus system/manager@test1则就会给出TNS-12154错误。 
要注意的是，有时即使在tnsnames.ora文件中有相应的网络服务名，可是用该网络服务名连接时还会出错，出现这种情况的典型配置如下（在客户端的机器上）： 
sqlnet.ora文件： 
NAMES.DIRECTORY_PATH = (TNSNAMES, ….) 
NAMES.DEFAULT_DOMAIN = server.com 
tnsnames.ora文件: 
test = 
(DESCRIPTION= 
(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST=testserver)(PORT=1521)) 
) 
(CONNECT_DATA=(SERVICE_NAME=orcl.testserver.com) 
) 
) 
sql*plus运行基本机理： 
在用户输入sqlplus system/manager@test后，sqlplus程序会自动到sqlnet.ora文件中找NAMES.DEFAULT_DOMAIN参数，假如该参数存在，则将该参数中的值取出，加到网络服务名的后面，即此例中你的输入由sqlplus system/manager@test自动变为sqlplus system/manager@test.server.com
 ，然后再到tnsnames.ora文件中找test.server.com网络服务名，这当然找不到了，因为该文件中只有test网络服务名，所以报错。解决的办法就是将sqlnet.ora文件中的NAMES.DEFAULT_DOMAIN参数注释掉即可，如#NAMES.DEFAULT_DOMAIN = server.com。假如NAMES.DEFAULT_DOMAIN参数不存在，则sqlplus程序会直接到tnsnames.ora文件中找test网络服务名，然后取出其中的host，port，tcp，service_name，利用这些信息将连接请求发送到正确的数据库服务器上。 
另外原则上tnsnames.ora中的配置不区分大小写，但是我的确遇到区分大小写的情况，所以最好将使用的网络服务与tnsnames.ora中配置的完全一样。 
ORA-12514: TNS:listener could not resolve SERVICE_NAME given in connect Descriptor. 
该错误表示能在tnsnames.ora中找到网络服务名，但是在tnsnames.ora中指定的SERVICE_NAME与服务器端的SERVICE_NAME不一致。解决的办法是修改tnsnames.ora中的SERVICE_NAME。 
易混淆术语介绍： 
Db_name：对一个数据库（Oracle database）的唯一标识，该数据库为第一章讲到的Oracle database。这种表示对于单个数据库是足够的，但是随着由多个数据库构成的分布式数据库的普及，这种命令数据库的方法给数据库的管理造成一定的负担，因为各个数据库的名字可能一样，造成管理上的混乱。为了解决这种情况，引入了Db_domain参数，这样在数据库的标识是由Db_name和Db_domain两个参数共同决定的，避免了因为数据库重名而造成管理上的混乱。这类似于互连网上的机器名的管理。我们将Db_name和Db_domain两个参数用’.’连接起来，表示一个数据库，并将该数据库的名称称为Global_name，即它扩展了Db_name。Db_name参数只能由字母、数字、’_’、’#’、’$’组成，而且最多8个字符。 
Db_domain：定义一个数据库所在的域，该域的命名同互联网的’域’没有任何关系，只是数据库管理员为了更好的管理分布式数据库而根据实际情况决定的。当然为了管理方便，可以将其等于互联网的域。 
Global_name：对一个数据库（Oracle database）的唯一标识，oracle建议用此种方法命令数据库。该值是在创建数据库是决定的，缺省值为Db_name. Db_domain。在以后对参数文件中Db_name与Db_domain参数的任何修改不影响Global_name的值，如果要修改Global_name，只能用ALTER DATABASE RENAME
 GLOBAL_NAME TO <db_name.db_domain>命令进行修改，然后修改相应参数。 
Service_name：该参数是oracle8i新引进的。在8i以前，我们用SID来表示标识数据库的一个实例，但是在Oracle的并行环境中，一个数据库对应多个实例，这样就需要多个网络服务名，设置繁琐。为了方便并行环境中的设置，引进了Service_name参数，该参数对应一个数据库，而不是一个实例，而且该参数有许多其它的好处。该参数的缺省值为Db_name. Db_domain，即等于Global_name。一个数据库可以对应多个Service_name，以便实现更灵活的配置。该参数与SID没有直接关系，即不必Service
 name 必须与SID一样。 
Net service name：网络服务名，又可以称为数据库别名（database alias）。是客户端程序访问数据库时所需要，屏蔽了客户端如何连接到服务器端的细节，实现了数据库的位置透明的特性。
