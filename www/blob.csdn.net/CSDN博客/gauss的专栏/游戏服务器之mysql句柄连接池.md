# 游戏服务器之mysql句柄连接池 - gauss的专栏 - CSDN博客
2014年10月19日 16:43:02[gauss](https://me.csdn.net/mathlmx)阅读数：645
mysql连接使用封装原理是建立mysql连接句柄池。mysql连接句柄池在初始化时会根据放入的连接地址读取所有的表(建立表对象及其相关字段)和初始化一个mysql连接句柄。mysql连接句柄池可根据哈希和使用标识获取没有标识使用的句柄。
设计上：
（1）封装mysql连接到mysql连接句柄。mysql连接句柄由句柄管理器管理(加入和读取要加读写锁)。
（2）加载连接的表及其表的字段到表对象。表对象由表结构管理器管理（加入和读取要加读写锁）。实现orm的检查功能。
（3）封装条件查询。实现简化和安全使用字段和条件组成sql。
（4）封装结果集。实现简化获取执行sql结果。
数据结构：
（1）表结构管理器 
std::vector<mysql_table_manager *> tm;  //数据库表管理器 数组
获取表如
mysql_table* table  = this->pool->tm[this->_hashcode]->getTableByName(tablename);//pool 为句柄连接池
（2）句柄管理器
std::vector<mysql_handle_manager *> mhm;//mysql句柄数组，由哈希获取句柄管理者，再遍历获取没有标识使用的句柄。根据实际应用情况分散hashcode 的桶上的连接对象 可以有效减少多线程使用mysql句柄的竞争。
目录：
1、mysql句柄池
（1）句柄池初始化mysql句柄池
（2）加载连接地址
（2-1）加载表格
（3）获取句柄
2、条件查询
(1)条件查询封装
(2)执行sql
(3)查询使用实例
内容：
1、mysql句柄池
每个mysql句柄就是个mysql连接。句柄池会用哈希结构存储mysql句柄，按哈希结构存储mysql连接上的所有表结构。
（1）句柄池初始化mysql句柄池
/**
 * \description构造函数
 * \parammax_hash 支持最大的hash个数
 * \parammax_handle 每个hash桶中支持的最多handle个数
 */
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/417135/fork)
- mysql_handle_pool::mysql_handle_pool(int max_hash,int max_handle)//int max_hash = 3,int max_handle = 64
- {  
- for(int i = 0 ; i < max_hash ; i ++)  
-     {  
-         mhm.push_back(new mysql_handle_manager);  
- tm.push_back(new mysql_table_manager);  
-     }  
-     _max_hash = max_hash;  
-     _max_handle = max_handle;  
-     g_log->info("Version of the mysql libs is %s" ,mysql_get_client_info());  
- if(!mysql_thread_safe())  
-     {         
-         g_log->warn("The mysql libs is not thread safe...");  
-     }    
- }  
（2）加载连接地址
生成连接句柄。加载连接地址上的所有表结构。
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/417135/fork)
- bool mysql_handle_pool::putUrl(unsigned int hashcode,constchar *url)  
- {  
-     mysql_url *mu = new mysql_url(hashcode,url);//生成连接句柄
- if(!mu)  
-     {  
- returnfalse;  
-     }  
- if(!mum.addMysqlUrl(mu))  
-     {  
-         SAFE_DELETE(mu);  
- returnfalse;  
-     }  
-     mysql_handle *handle = new mysql_handle(mu,this,hashcode%_max_hash);  
- if(!handle)  
-     {  
- returnfalse;  
-     }  
- if(!handle->initHandle())//初始化mysql连接
-     {  
-         SAFE_DELETE(handle);  
- returnfalse;  
-     }  
- if(!mhm[hashcode]->addMysqlHandle(handle))  
-     {  
- returnfalse;  
-     }  
- tm[hashcode]->reloadAllTables(handle->getMysql());//加载连接上的所有表格
- returntrue;  
- }  
使用实例
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/417135/fork)
- mysqlPool = new mysql_handle_pool;  
- if(mysqlPool == NULL ||!mysqlPool->putUrl(0,g_xml_config.get_string("Global.MYSQL.Config")))  
- {  
-     g_log->error("连接数据库失败");  
- returnfalse;  
- }  
连接配置如： "mysql://root:123456@192.168.188.87:3306/mu"
（2-1）加载表格
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/417135/fork)
- bool mysql_table_manager::reloadAllTables(MYSQL *mysql_conn)  
- {  
-     MYSQL_RES* table_res = NULL;  
- 
- if((table_res = mysql_list_tables(mysql_conn, NULL)) == NULL)//加载所有表格
-     {  
-         g_log->error("%s:mysql_list_tables fail.", __FUNCTION__);  
-         g_log->error("%s:reason: %s", __FUNCTION__,mysql_error(mysql_conn));  
- returnfalse;  
-     }             
-     MYSQL_ROW row;  
- 
-     delete_all();  
- 
- while((row=mysql_fetch_row(table_res)))  
-     {  
- this->addNewTable(mysql_conn, row[0]);  
-     }  
- 
-     mysql_free_result(table_res);  
- returntrue;  
- }  
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/417135/fork)
- bool mysql_table_manager::addNewTable(MYSQL *mysql_conn,constchar *tablename)  
- {  
-     mysql_table *table = new mysql_table;  
- if(!table)  
-     {  
- returnfalse;  
-     }  
-     strncpy(table->name,tablename,sizeof(table->name));  
-     table->reloadAllFields(mysql_conn , tablename);  
- return addTable(table);  
- }  
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/417135/fork)
- bool addTable(mysql_table *entry)  
- {  
- int len = strlen(entry->name);  
- for (int i = 0; i < len; i++)  
-     {  
-         entry->name[i] = ::toupper(entry->name[i]);  
-     }  
- 
- bool bret = false;  
-     rwlock.wrlock();  
-     bret = add_object(entry);  
-     rwlock.unlock();  
- return bret;  
- }  
加载字段
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/417135/fork)
- bool mysql_table::reloadAllFields(MYSQL *mysql_conn,constchar *tablename)  
- {  
-     MYSQL_RES* field_res = NULL;  
-     field_res = mysql_list_fields(mysql_conn, tablename, NULL);  
- if(!field_res)  
-     {  
- returnfalse;  
-     }  
-     unsigned int num_fields = mysql_num_fields(field_res);  
-     MYSQL_FIELD* mysql_fields = NULL;  
-     mysql_fields = mysql_fetch_fields(field_res);  
- 
- for(unsigned int i=0; i<num_fields; i++)  
-     {  
- if(!fs.addNewField(mysql_fields[i].name, mysql_fields[i].type))  
-         {  
-             mysql_free_result(field_res);  
- returnfalse;  
-         }  
-     }  
- 
-     mysql_free_result(field_res);  
- returntrue;  
- }  
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/417135/fork)
- bool mysql_record::addNewField(constchar *fieldname, int fieldType, uint32 mask)  
- {  
-     mysql_field *entry = new mysql_field;  
- if(!entry)  
-     {  
- returnfalse;  
-     }  
-     strncpy(entry->name,fieldname,sizeof(entry->name));  
-     entry->type = fieldType;  
-     entry->mask = mask;  
- return addField(entry);  
- }  
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/417135/fork)
- bool mysql_record::addField(mysql_field *entry)  
- {  
- for(unsigned int i = 0;entry->name[i]&&i < sizeof(entry->name);++i)  
-         entry->name[i] = (char)::toupper(entry->name[i]);  
-         g_log->trace("%s,%u, %s", __PRETTY_FUNCTION__, __LINE__, entry->name.c_str());  
- bool bret = false;  
-     rwlock.wrlock();  
-     bret = add_object(entry);  
-     rwlock.unlock();  
- return bret;  
- }  
（3）获取句柄
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/417135/fork)
- mysql_handle *mysql_handle_pool::getHandle(uint32 hashcode)  
- {  
- struct GetHandleExec :public callback<mysql_handle>  
-     {  
-         GetHandleExec():_handle(NULL)  
-         {  
-         }  
-         mysql_handle *_handle;  
- bool invoke(mysql_handle *entry)  
-         {  
- switch(entry->state)  
-             {  
- case HandleState_Valid:  
- case HandleState_Invalid:  
-                     {  
- if(entry->setHandle())//设置句柄
-                         {  
-                             _handle = entry;  
- returnfalse;  
-                         }  
-                     }  
- break;  
- case HandleState_Used:  
-                     {  
-                         entry->checkUseTime();//检查使用句柄时间
-                     }  
- break;  
-             }  
- returntrue;  
-         }  
-     };  
-     GetHandleExec exec;  
-         _mutex.lock();  
- while(true)  
-     {  
-         mhm[hashcode%_max_hash]->traverseMysqlHandle(exec);  
- if(exec._handle)  
-         {  
-                         _mutex.unlock();  
- return exec._handle;  
-         }  
- if(mhm[hashcode%_max_hash]->getSize() < _max_handle)  
-         {  
-             mysql_url *mu = mum.getMysqlUrlByID(hashcode);  
- if(mu)  
-             {  
-                 mysql_handle *handle = new mysql_handle(mu,this,hashcode%_max_hash);  
- if(!handle)  
-                 {  
-                                         _mutex.unlock();  
- return NULL;  
-                 }  
- if(!handle->initHandle())  
-                 {  
-                     SAFE_DELETE(handle);  
-                                         _mutex.unlock();  
- return NULL;  
-                 }  
- if(!mhm[hashcode]->addMysqlHandle(handle))  
-                 {  
-                                          _mutex.unlock();  
- return NULL;  
-                 }  
- tm[hashcode]->reloadAllTables(handle->getMysql());  
-                                 _mutex.unlock();  
- return handle;  
-             }  
-         }  
-         thread_base::msleep(50);  
-     }  
-         _mutex.unlock();  
- return exec._handle;  
- }  
2、条件查询
(1)条件查询封装
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/417135/fork)
- mysql_record_set* mysql_handle::exeSelect(constchar *tablename, mysql_record* column, mysql_record* where, mysql_record* order,   
-                 unsigned int limit_max,unsigned int limit_min,mysql_record* groupby, mysql_record* having)  
- {  
-     mysql_table* table  = this->pool->tm[this->_hashcode]->getTableByName(tablename);//获取表
- if (NULL == _mysql)  
-     {  
-         error_log("执行%s时_mysql指针为空",__FUNCTION__);  
- return NULL;  
-     }  
- if (NULL == table)  
-     {  
-         error_log("执行%s时table指针为空,找不到%s表",__FUNCTION__,tablename);  
- return NULL;  
-     }  
- 
-     _select_time.now();//记录查询时间
- 
-     std::ostringstream query_string;  
- 
-     query_string << "SELECT ";  
- //所有字段名
- if(column && !column->isEmpty())  
-     {  
- struct SqlFieldExec : public callback<mysql_field>  
-         {  
- bool _first;  
-             mysql_table* _table;  
-             std::ostringstream &query_string;  
-             SqlFieldExec(mysql_table* table, std::ostringstream &query_string)  
-                 : _first(true), _table(table), query_string(query_string)   
-                 {  
-                 }  
- bool invoke(mysql_field *entry)  
-             {  
- if( entry->name == "*" || _table->fs.get(entry->name))//获取字段
-                 {  
- if(_first)  
-                     {  
-                         _first=false;  
-                     }  
- else
-                     {  
-                         query_string <<  " , ";  
-                     }  
- 
-                     query_string << "`" << entry->name << "`";//字段名
-                 }  
- returntrue;  
-             }  
-         } sfe(table, query_string);  
- 
-         column->traverseField(sfe);  
-     }  
- else
-     {  
-         query_string << " * ";  
-     }  
- 
-     query_string << "  FROM " << " `" << table->name << "` ";  
-     getWhere(table, query_string, where);//where 条件
-     getGroupBy(table, query_string, groupby);//group 
-     getHaving(table, query_string, having);//having 条件
-     getOrder(table, query_string, order);  
- 
- if(limit_max)  
-     {  
-         query_string << " LIMIT " << limit_min << " , " << limit_max;  
-     }  
- 
- if(execSql(query_string.str().c_str(), query_string.str().size())) //执行sql
-     {  
-         error_log("%s", mysql_error(_mysql));  
- return NULL;  
-     }   
- 
-     unsigned int retCount=0;  
-     MYSQL_RES *result=NULL;  
- 
-     result=mysql_store_result(_mysql);//一次性传送结果
- if(result==NULL)  
-     {  
-         error_log("%s", mysql_error(_mysql));  
- return NULL;  
-     }  
-     retCount =mysql_num_rows(result);//获取行数
- 
- if(retCount==0)  
-     {  
-         mysql_free_result(result);  
- return NULL;  
-     }  
- 
- 
-     MYSQL_ROW row;  
-     unsigned int num_field = mysql_num_fields(result);//获取结果集中的字段数
-     MYSQL_FIELD* mysql_fields = NULL;  
-     mysql_fields = mysql_fetch_fields(result);//获取结果字段
- 
- 
-     mysql_record_set* ret_set = NULL;  
-     ret_set = new mysql_record_set(retCount);  
- 
- if(ret_set)  
-     {  
-         unsigned int j = 0;  
- while((row = mysql_fetch_row(result)))//获取一行结果
-         {  
-             unsigned long *lengths= mysql_fetch_lengths(result);//获取行长度
-             mysql_record* rec = ret_set->get(j++);  
- if(rec)  
-             {  
- for(unsigned int i=0; i<num_field; i++)  
-                 {  
- if(row[i] != NULL)  
-                     {  
- //g_log->debug("%s,%s,%lu", mysql_fields[i].name, row[i], lengths[i]);
-                         rec->put(mysql_fields[i].name, row[i], lengths[i]);//放入结果（字段名、结果、长度）
-                     }  
-                 }  
-             }  
-         }  
-     }  
- 
-     mysql_free_result(result);  
- 
- if(ret_set && ret_set->size() == 0)  
-     {  
-         SAFE_DELETE(ret_set);  
-     }  
- 
- 
- if(_select_time.elapse(realtime()) >= 3 * 1000L)//检查查询事件
-     {  
-         g_log->warn("超时%llu毫秒sql:%s",_select_time.elapse(realtime()),query_string.str().c_str());  
-     }  
- return ret_set;  
- }  
(2)执行sql
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/417135/fork)
- int mysql_handle::execSql(constchar *sql,unsigned int sqllen,bool need_errlog)  
- {  
- if (NULL == _mysql)  
-     {  
-         error_log("执行%s时_mysql指针为空,sql语句为%s",__FUNCTION__,sql);  
- return -1;  
-     }  
- if (NULL == sql)  
-     {  
-         error_log("执行%s时传入sql语句为空",__FUNCTION__);  
- return -1;  
-     }  
- if (0 == sqllen)  
-     {  
-         error_log("执行%s时sql语句%s长度为0",__FUNCTION__,sql);  
- return -1;  
-     }  
- 
-     lastSql=sql;  
- int ret=mysql_real_query(_mysql,sql,sqllen);  
- if(ret && need_errlog)  
-     {     
-         error_log("%s", mysql_error(_mysql));  
-         error_log("%s", sql);  
-     }  
- return ret;  
- }  
(3)查询使用实例
查询服务器列表
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/36698011#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/417135/fork)
- mysql_record_set *recordset = NULL;  
- std::ostringstream oss;  
- mysql_record column,where;  
- mysql_handle *handle = mysqlPool->getHandle();//从句柄池获取句柄
- if (!handle)  
- {  
-     g_log->error("不能从数据库连接池获取连接句柄");  
- returnfalse;  
- }  
- oss << "type=" << CENTERSERVER;  
- where.put("type",oss.str());//从服务器表格中查询类型为中心服务器的服务器配置
- recordset = handle->exeSelect("SERVERLIST",NULL,&where);  
- if(recordset && recordset->size() == 1)//有结果且结果只有一条
- {  
- //只有一条满足条件的记录
- if (strcmp(pstrIP, recordset->get(0)->getvalue("ip")) == 0)//从结果集的第一条结果中获取字段为“ip”的值,且比较ip跟本服务器ip一致
-     {  
-         wdServerID = recordset->get(0)->getvalue("id");//获取服务器id、名字、端口
- this->name = (constchar* )(recordset->get(0)->getvalue("name"));  
-         wdPort = recordset->get(0)->getvalue("port");  
-         wdExtPort = recordset->get(0)->getvalue("extport");  
-     }  
- else
-     {  
- char temp[MAX_NAME_LEN];//结果不一致，输出日志
-         strcpy(temp, recordset->get(0)->getvalue("name"));  
-         g_log->error("数据库中的记录不符合：%s, %s, %s", pstrIP, (constchar *)recordset->get(0)->getvalue("ip"),(constchar *)recordset->get(0)->getvalue("name"));  
-         SAFE_DELETE(recordset);  
-         mysqlPool->putHandle(handle);  
- returnfalse;  
-     }  
- }  
