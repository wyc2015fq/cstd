# FNCOS FM1208-43COS指令集 指令标识 与错误号 - xqhrs232的专栏 - CSDN博客
2018年07月27日 16:18:02[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：218
原文地址::[https://blog.csdn.net/qq_34075348/article/details/77877306](https://blog.csdn.net/qq_34075348/article/details/77877306)
!FM1208
//
//FM1208-43 CPU卡状态字定义 根据FMCOS通用技术手册_仅供参考_1.1_200907.pdf编制
//
//当SW1的高半字节为’9’，且低半字节不为’0’时，其含义依赖于相关应用。
//当SW1的高半字节为’6’，且低半字节不为’0’时，其含义与应用无关。
//
//格式:SW1SW2|意义
9000|正确执行
61xx|还有xx字节可读.
6281|回送的数据可能错误
6283|选择文件无效，文件或密钥校验错误
63Cx|x表示还可再试次数
6400|状态标志未改变
6581|写EEPROM不成功
6700|错误的长度
6900|CLA与线路保护要求不匹配
6901|无效的状态
6981|命令与文件结构不相容
6983|密钥被锁死
6985|使用条件不满足
6987|无安全报文
6988|安全报文数据项不正确
6A80|数据域参数错误
6A81|功能不支持或卡中无MF或卡片已锁定
6A82|文件未找到
6A83|记录未找到
6A84|文件无足够空间
6A86|参数P1P2错误
6A88|密钥未找到
6B00|在达到Le/Lc字节之前文件结束，偏移量错误
6Cxx|Le错误,记录实际长度为xx字节
6E00|无效的CLA
6F00|数据无效
9302|MAC错误
9303|应用已被锁定
9401|金额不足
9403|密钥未找到
9406|所需的MAC不可用
6581|写EEPROM不成功 
6982|权限不满足 
6983|密钥被锁死 
6A88|密钥未找到 
6700|错误的长度 
6300|认证识别码出错 
6D00|INS错误或不支持 
//
//FM1208-43COS指令集
//
0020|			VERIFY,验证口令
00C0|			GET RESPONSE,取响应数据
00D0|			UPDATE BINARY,写二进制文件
04D0|			UPDATE BINARY,写二进制文件，带MAC
00D2|			UPDATE RECORD,写记录文件
04D2|			UPDATE RECORD,写记录文件，带MAC
8416|			CARD BLOCK,卡片锁定，带MAC
8418|			APPLICATION UNBLOCK,应用解锁，带MAC
841E|			APPLICATION BLOCK,应用锁定，带MAC
8424|			PIN UNBLOCK,个人密码解锁 ，带MAC
8024|			PIN UNBLOCK,个人密码解锁 
802C|			UNBLOCK,解锁被锁住的口令 
8050|			INITIALIZE Purchase,初始化交易/建设部
8052|			CREDIT FOR LOAD,圈存
8054|			DEBIT FOR PURCHASE/CASEWITHDRAW/UNLOAD,消费/取现/圈提
8058|			UPDATE OVERDRAW LIMIT,修改透支限额
805A|			GET TRANSCATIONPROVE,取交易认证 
805C|			GET BALANCE,读余额
805E|			RELOAD/CHANGEPIN,重装/修改个人密码
800E|			ERASEDF,擦除DF/专有
8030|			PULL,专用消费/建设部
8032|			CHARGE,专用充值/建设部
8084|			WRITE KEY,增加或修改密钥/专有
80D4|			WRITE KEY,增加或修改密钥/专有
80E0|			CREATE,建立文件/专有
0000|			WRITE EEPROM,写数据EEPROM/生产测试
0004|			READE EPROM,读数据EEPROM/生产测试
0002|			INITIAL EEPROM,初始化EEPROM/生产测试
000C|			READROM,读程序ROM/生产测试
000A|			CALCULATE ROM CRC,计算程序ROMCRC/生产测试
80DC|			UPDATE CAPP DATA CACHE,更新复合应用数据缓存
//
//建设部 PSAM COS指令码
//根据2008年12月建设部PSAM卡参考手册编制
//
8018|	APPLICATION UNBLOCK（应用解锁）./命令成功执行后，MAC的错误计数器将被重置。 
80CA|	GET MESSAGE[LC=0,LE=09]用于获取CPU卡的安全认证码。/AUTHENTICATION MASSAGE[LC=09,LE=不存在],建设部PSAM卡验证安全认证码。这2个指令用于消费/取现交易。芯片安全区域内的安全认证识别码或芯片中的MID ║ UID0UID1UID2UID3 ║ 四字节识别码，将安全认证识别码发送给PSAM卡进行认证。该命令在应在任意目录下都可以执行。  
80FC|	CACULATE KEY(计算密钥)命令 CALCULATE KEY命令是标准的建设部SAM 专用命令，是实现在SAM卡的控制下，使用MIFARE ONE卡作为用户卡，计算逻辑加密卡的扇区密钥。 
8072|	CREDIT_SAM_FOR_PURCHASE（校验 MAC2）命令 CREDIT_SAM_FOR_PURCHASE命令利用INIT_SAM_FOR_PURCHASE命令产生的过程密钥SESPK校验 MAC2。 
80FA|	DES CRYPT 命令利用指定的密钥来进行运算。
0082|	PSAM卡用EXTERNAL AUTHENTICATION命令通过3DES加密算法对卡片外部进行安全认证。终端对卡返回的随机数进行加密来进行认证。 
0084|	终端用GET CHALLENGE命令从PSAM卡中取回一个随机数，用于安全交易过程。 
0088|	通过INTERNAL AUTHENTICATION命令，终端设备可以采用DES加密算法来对PSAM卡片进行认证。对于这个认证，卡片需要对接收到的随机数进行加密，并将加密结果返回到终端设备。 
801A|	INIT_FOR_DESCRYPT命令用来初始化通用密钥计算过程。 PSAM卡将利用卡中指定的密钥进行运算，产生一个临时密钥。运算方式由指定的密钥类型、密钥分散级数和密钥算法标识确定。 
8070|	INIT_SAM_FOR_PURCHASE命令支持多级消费密钥分散机制。 （MAC1 计算初始化）命令 
0022|	在建设部PSAM卡中，MANAGE SECURITY ENVIRONMENT命令在WRITE RSA KEY命令前使用，用于指定修改PSAM卡中的哪个RSA密钥对的公钥。  
00B0|	READ BINARY ,读取二进制文件中内容或部分内容。 被读取的EF文件可以通过SELECT等命令进行显式选择，也可以通过READ BINARY命令的P1字节进行隐式选择。 READ BINARY命令只有在创建EF文件时设置的AC READ条件得到满足后，才能执行。
04B0|	READ BINARY ,读取二进制文件中内容或部分内容。 被读取的EF文件可以通过SELECT等命令进行显式选择，也可以通过READ BINARY命令的P1字节进行隐式选择。 READ BINARY命令只有在创建EF文件时设置的AC READ条件得到满足后，才能执行。
00B2|	READ RECORD,读取非二进制文件中内容。被读取的EF文件可以通过SELECT等命令进行显式选择， 也可以通过READ RECORD命令的P2字节进行隐式选择。 READ RECORD命令只有在创建EF文件时设置的AC READ条件得到满足后，才能执行。
04B2|	READ RECORD,读取非二进制文件中内容，带MAC。
00A4|	SELECT,选定DF或EF，激活一个存在的文件或文件层。 
00D6|	UPDATE BINARY,更新二进制文件的内容(或部分内容)。 被读取的EF文件可以通过SELECT等命令进行显式选择，也可以通过UPDATE BINARY命令的P1字节进行隐式选择。 UPDATE BINARY命令只有在创建EF文件时设置的 AC UPDATE条件得到满足后，才能执行。 
04D6|	UPDATE BINARY,更新二进制文件的内容(或部分内容)，带MAC。 
00DC|	UPDATE RECORD,更新线性定长结构的文件记录。当前指定记录将被新记录覆盖。 该命令以整条记录的形式更新记录，不能只更新记录中的一部分。所更新的EF要求已经被选定或在UPDATE RECORD 的命令参数P2中指定。 只有更新记录的权限满足时，才能执行命令UPDATE RECORD。 
04DC|	UPDATE RECORD,更新线性定长结构的文件记录，带MAC。 
80D2|	WRITE RSA KEY,更新PSAM卡中已经存在的RSA公钥,（写 RSA 密钥）.
80D8|	UPDATE KEY PROTECTED BY RSA,装载DES应用密钥或更新已经存在的密钥，首先使用卡片内RSA密钥对的公钥，对被私钥加密的新DES密钥进行解密计算，然后进行DES密钥的装载或更新。 
//FM1208-PSAM特殊指令集
848A|	CREATE MF=84 8A 00;ERASE MF=84 8A 01.明文+MAC。
808A|	CREATE MF=80 8A 00;ERASE MF=80 8A 01.
8080|	CREATE FILE
8480|	CREATE FILE  明文+MAC方式
841C|   END ISSUE 明文+MAC方式
84D4|	WRITE KEY/CREATE KEY 密文+MAC方式
808C|	SET CARD SN
