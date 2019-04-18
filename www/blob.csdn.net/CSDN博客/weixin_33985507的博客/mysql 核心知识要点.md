# mysql 核心知识要点 - weixin_33985507的博客 - CSDN博客
2015年03月25日 11:09:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
**整体知识介绍**：mysql基本操作和使用，mysql优化（索引，分表等），mysql部署（读写分离，负载均衡等）
**数据库基本介绍**：数据库概念，常用数据库，web应用三大软件分工，PHP动态语言特点（处理数据），数据的存放（脚本运行时和执行完毕），数据库系统
**关系型数据库**：概念（关系+二维表），教学系统实例，常见关系型数据库，关系型数据库特点（所有数据相同结构，没有数据也分配存储空间）
**非关系型数据库**：键值对型，常见非关系型数据库，非关系型数据库特点（内存运行，硬盘同步）
**关系型数据库典型概念**：行row（记录record），列col（字段field），SQL（概念，DDL，DML（DML+DQL），DCL）
**Mysql基本介绍**：mysql软件介绍（c/s结构），mysql操作步骤，客户端的工作，mysql单独安装，mysql目录及文件介绍，mysql服务端管理（服务和mysqld.exe），客户端连接服务端（连接认证，执行sql，退出）
**Mysql****数据对象**：mysql服务器à数据库à数据表à字段
**基本SQL****操作**：库操作（create[if not exists][charset,collation]，库文件夹介绍，数据库命名规则，特殊名字处理；show[like pattern]，通配符，查看库创建语句；drop；alter），表操作（表与字段的依赖性；create [engine,charset]，表文件分析，表与库的关系，保留字冲突，从已有表创建[like]；show[like pattern]，查看表字段[desc/describe/show columns from tb_name]，查看表创建语句；drop；alter [操作add/drop/modify/change][位置first after]，rename to重命名；数据操作（insert into省略字段和选择字段;select from[where条件]，=比较；delete from[where]；update set[where]；删除和更新不可逆）
**字符集**：快速解决问题（set names），字符集，mysql字符集（数据存储，数据传输），数据存储字符集（服务器端，库，表，字段，图解），字符集优先级（字段à表à库à服务器端），数据传输过程字符集（图解（客户端clientà连接层connectionà服务器à客户端result），character_set变量，set names分析
**校对集**：字符比较（ASCII比较），校对集工作（查看show collation），校对集设置（表选项中collate=校对集），校对集命名规则（_bin二进制，_ci[case insensitive]，_cs[case sensitive]），utf8&gbk（utf8不支持中文比较，gbk支持拼音）
**存储引擎**：概览mysql存储引擎，主要引擎[InnoDB和MyISAM]，引擎的未来发展，引擎区别（MyISAM[查询和新增，全文索引]，InnoDB[更新和删除，事务，5.5之后全文索引]，文件区别）
**乱码问题解决**：（浏览器—PHP--mysql）
**字段（列）类型**：
　　mysql数据类型（数值型，字符串型，日期时间型），
　　数值类型（整型[tinyint，smallint，mediumint，int，bigint]，显示宽度，0填充，无符号；
　　浮点型[float，double]，语法float(M,D)，浮点支持科学计数法；定点数[decimal(M,D)]，定点与浮点的存储方式区别），
　　字符串类型（char(L)；varchar(L)；text；char与varchar比较表，mysql记录总长限制，文本不受限制(10字节)；枚举enum[值存储，2字节，单选，方便插入[数值或者　　　　字符串]，从1开始]；集合set[存储方式二进制，多选，8字节]；二进制文本），
　　日期和时间类型（datetime[格式，8字节存储，时间跨度9999]；date[]；time[时间间隔]；year[1字节存储，跨度1900-2155或70-69]；timestamp[标准整型存储，表现与datetime一致]）
**列属性（约束）**：NULL（概念[与PHP不同]，语法NULL/NOT NULL），默认值（default，用法，插入数据时使用default关键字），键（主键[primary key，唯一标识，设计表图解，主键优势，主键语法包括建表和修改，删除主键]；唯一键[unique key，优点，增加包含建表时和修改表结构，删除唯一键平[只能通过删除索引方式]]），自动增长（auto_increment，常用主键ID），注释（comment，SQL通用注释语法[--空格，#，/*块注释*/]）
**关系**：关系分类（1：N[学生与班级对应关系，设计]；M：N[教师与班级对应关系，设计]；1：1[学生与学生关系]）
**外键（约束）**：外键（foreign key，学生与班级关系，外键功能，外键定义[外键（逻辑上），外键约束（语法上）]，增加外键[创建表时和修改表结构]，约束作用，删除约束[约束名]，增加约束名[constraint]，约束对应的索引），外键约束（主表和从表，约束操作[严格控制restrict，置空set null，级联cascade]，约束范围[删除和更新]，外键支持[InnoDB，类型严格一致，置空操作字段必须可以为空]）
**范式**：范式定义（Normal Format），范式等级（1到6），数据库范式（满足到第三等级），1NF（原子性），2NF（部分依赖，复合主键），3NF（传递依赖），范式有点（通用，减低数据冗余），逆规范化（设置必要数据冗余，效率与磁盘空间的对抗）
**高级数据操作**：
　　插入数据（选择插入[insert into tb_name select子句]，主键冲突[insert into … on duplicate key update]，替换插入[replace into]）
　　删除数据（搭配orderby和limit，清空表[truncate]，delete与truncate区别），修改数据（搭配orderby和limit）
　　查询数据（
　　　　select选项[all和distinct]，字段列表[表达式，字段别名]，from子句[多表，表别名，虚拟表dual]，
　　　　五子句[where，group by，having，order by，limit]），
　　　　where子句（操作流程，mysql布尔类型和值，表达式[关系运算符 =，in和not in，between and和not between and，is null和is not null；逻辑运算符；like运算符，通配符；运算符优先级]），group by子句（分组统计，合计函数[count(对非空字段统计)，max()，min()，sum()，avg()]，grou_concat(字段)[分组专用字段连接]，
　　group by[排序asc|desc]，多字段分组，回溯统计[with rollup]），
　　having子句（与where功能相似，可以使用字段别名，可以使用合计函数，与where操作数据不同），
　　order by子句（排序，多字段排序），limit子句（限制记录数，指定索引位置，分页）
**联合查询**：联合查询（union，结果间联合，条件简化，union选项[all和distinct]，字段数量一致类型无所谓[原因，var_dump看结果]，union的select子句分别排序[子句需要括号，需要配合limit]）
**连接查询**：概念，连接分类（内连接[inner join [on]，参与连接数据都必须存在，where替代on，默认连接方式，连接原理，where与on的区别，using去重]，外连接[left/right join on，参与连接的数据有对应不上[左外保留左表数据，右外保留右表数据]，full全外mysql不支持，on和using使用]，交叉连接[内连接的无条件模式]，自然连接[natural [left/right] join，相当与其他连接使用using，不能使用on]）
**子查询**：子查询概念，功能，子查询分类（位置分类[where，from，exists]，返回形式分类[标量子查询[单一值，用于比较]，列子查询[配合in和not in，any，all和some]，行子查询[使用括号语法构建行]，表子查询[典型from子查询]]）
**视图**：概念（虚拟表），视图作用（查询数据，提供接口，隐藏真实数据），视图管理（创建[create view as select子句]，删除视图[drop view]，修改视图[alter view v_name as select子句]，查看视图[show create view v_name，查看视图字段，查看视图创建语句]，视图算法[概念，合并算法merge，临时表temptable，未定义undefined，视图指定算法[create algorithm=temptable view]]）
**数据备份还原**：导出（select into outfile ‘路径’，导出文件不能重名，字段分隔符[fields terminated by，enclosed by，escaped by]，记录分隔符[lines terminated by/starting by]，导出文件编码），导入（load data infile ‘路径’ into table tb_name 选项）
**数据备份**：MyISAM备份，mysqldump（mysqldump客户端，语法，表备份[库 表 表/ -b 库]，数据库备份），还原（source，mysql库名 表名 < sql文件）
**事务**：事务目的，事务操作（开启[start transaction]，提交[commit]，回滚[rollback]，回滚点[savepoint name，rollback to name]），事务基本原理（mysql执行SQL方式[执行+提交]，默认自动提交autocommit），事务安全日志（保存事务执行中间结果ib_logfile，InnoDB支持事务），事务特点ACID（原子性[Atomicity，事务是整体]，一致性[consistency，事务前后数据完整性一致]，持久性[事务一旦提交，数据改变永久]，隔离性[isolation，多用户并发互不干扰，相互隔离]）
**触发器**：概念，语法（创建[create trigger t_name 事件时机 事件类型 on 表 for each row]，事件时机[before，after]，事件类型[insert，delete，update]，mysql不支持在同一事件上创建多个出发程序；删除[drop trigger t_name]；获取触发程序相关记录[old，new]，分支结构[if 条件 then 代码 end if]，多语句[begin，end]，语句结束符）
**变量**：系统变量，自定义变量（set，使用变量，select into，变量赋值符[:=]），变量有效期，变量作用域
**SQL****函数**：内置函数（substring，char_length，length，instr，lpad，insert,strcmp），自定义函数（概念，函数要素[函数名，参数列表，函数体，返回值]，语法[创建create function f_name(参数列表) returns 返回值类型]，调用函数[select f_name(参数)]，分支结构，循环结构[标签]，内部定义变量[全局变量@var，局部变量declare var]，中断（leave，iterate））
**存储过程**：概念，与函数的区别，语法（创建[create procedure pr_name(过程参数in out inout)]，过程参数类型，过程调用call）
**Mysql****用户管理**：
　　root密码忘记（重装，mysql管理用户的表user），
　　重置root密码（带参数重启服务[--skip-grant-tables]，无密码登录，修改用户表root密码[update set]，加密算法[sha1，md5，password()，重启]），
　　普通用户管理（创建用户[create user user_name[ identified by ‘密码’]，用户名形式[username@host]]，删除用户[drop user username@host]），
　　设置用户权限（添加[grant 权限列表 on 表名 to 用户]，权限列表，表名表示方法，特殊权限[all privileges]，刷新缓存[flush privileges，flush tables]，
　　删除用户权限[revoke 权限列表 on 表名 from 用户]）
**索引**：概念，mysql索引类型[主键索引，唯一索引，全文索引，普通索引，复合索引] 
**PHP****操作mysql**：
　　PHP变身客户端，开启该扩展，典型扩展（mysql，mysqli，pdo-mysql），libmysql.dll（连接服务提供者），查看mysql是否加载（PHPinfo），
　　PHP操作mysql（连接认证mysql_connect()，
　　准备和发送sql语句mysql_query，
　　处理执行结果mysql_fetch_assoc()，释放资源[结果集]，关闭连接mysql_close()），
　　mysql_query（mysql_query返回值[布尔型，结果集]，有返回值的语句[select,show,desc]，没有返回值的语句[insert,update,delete ,DDL]），
　　其他常用函数(mysql_errno()，mysql_error()，常见处理方式，mysql_fetch_array()，mysql_fetch_row()的区别，mysql_data_seek()，Mysql_affected_rows，Mysql_insert_id)
