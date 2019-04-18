# 游戏服务器之内存数据库redis客户端应用 - gauss的专栏 - CSDN博客
2014年10月17日 20:10:48[gauss](https://me.csdn.net/mathlmx)阅读数：935
本文主要介绍游戏服务器的对redis的应用。介绍下redis c++客户端的一些使用。
存储结构设计：
（1）装备道具的redis存储结构为例（Hashes存储类型）
存储结构为：    key : EQUIPMENTBAG角色id  frield: 装备位置 value:装备信息
存储一个装备道具到redis(使用hset 命令)
一次存储玩家的装备背包里的所有道具(使用命令hmset)
一次获取一个玩家的装备包裹的所有道具（一次获取键的所有field和value（使用命令hgetall））
（2）角色基础属性的redis存储结构为例(字符串存储类型)
存储结构：key：BASE角色id ，value： 角色基础信息 
获取一个角色基础属性 （使用命令 get）
存储一个角色基础属性（使用命令set）
（3）过期时间
设置过期时间 30天.访问时需要判断key 是否还存在。
本文目录：
1、redis命令介绍
（1）基本命令
（2）应用介绍
2、redis存储结构之应用解析
3、写入redis的应用
(1)存储一个装备道具到redis(使用hset 命令)
存储结构key : EQUIPMENTBAG角色id  frield: 装备位置 value:装备信息
key : EQUIPMENTBAGplayerId  frield: pos value:CBagItem
(2)一次存储玩家的装备背包里的所有道具(使用命令hmset)
(3)存储一个角色的基础信息（使用命令set）
存储结构：key BASE角色id ，value 角色基础信息 
4、读取redis的应用
（1）一次获取一个玩家的装备包裹的所有道具（一次获取键的所有field和value（使用命令hgetall））
（2）读取角色的基础信息（使用命令get）
存储结构：key BASE角色id，value 角色基础信息
5、redis客户端池
6、第三方的库接口
(1)redis客户端对象
(2)接口函数
(3)定义的异常
本文内容：
1、redis命令介绍
介绍一下redis客户端的接口对应使用到的redis的原生命令。
（1）基本命令
命令类型有：
(1)String
常用命令：
set,get,decr,incr,mget 等。
(2)Hash
常用命令：
hget,hset,hgetall 等。
(3)List
常用命令：
lpush,rpush,lpop,rpop,lrange等。
(4)Set
常用命令：
sadd,spop,smembers,sunion 等。
(5)Sorted set
常用命令：
zadd,zrange,zrem,zcard等
参考：[http://blog.csdn.net/chenjiayi_yun/article/details/18887757](http://blog.csdn.net/chenjiayi_yun/article/details/18887757)
（2）应用介绍
常用命令：
hget,hset,hgetall、hmset  、mset 等。
应用场景：
我们简单举个实例来描述下Hash的应用场景，比如我们要存储一个用户信息对象数据，包含以下信息：
           用户ID，为查找的key，
           存储的value用户对象包含姓名name，年龄age，生日birthday 等信息，
   如果用普通的key/value结构来存储，主要有以下2种存储方式：
(1)   第一种方式将用户ID作为查找key,把其他信息封装成一个对象以序列化的方式存储，需要一次次地发送和返回。
           如：set u001 "李三,18,20010101"
           这种方式的缺点是，增加了序列化/反序列化的开销，并且在需要修改其中一项信息时，需要把整个对象取回，并且修改操作需要对并发进行保护，引入CAS等复杂问题。
(2)   第二种方法是这个用户信息对象有多少成员就存成多少个key-value对儿，用用户ID+对应属性的名称作为唯一标识来取得对应属性的值，不需要一次次地设置，可以一次设置多个，但命令信息有些冗余。
           如：mset user:001:name "李三 "user:001:age18user:001:birthday "20010101"
           虽然省去了序列化开销和并发问题，但是用户ID为重复存储，如果存在大量这样的数据，内存浪费还是非常可观的。
（3）第三个，那么Redis提供的Hash很好的解决了这个问题，Redis的Hash实际是内部存储的Value为一个HashMap，并提供了直接存取这个Map成员的接口，
           如：hmset user:001 name "李三" age 18 birthday "20010101"      
   也就是说，Key仍然是用户ID, value是一个Map，这个Map的key是成员的属性名，value是属性值，这样对数据的修改和存取都可以直接通过其内部Map的Key(Redis里称内部Map的key为field), 也就是通过 key(用户ID) + field(属性标签) 就可以操作对应属性数据了，既不需要重复存储数据，也不会带来序列化和并发修改控制的问题。很好的解决了问题。
这里同时需要注意，Redis提供了接口(hgetall)可以直接取到全部的属性数据,但是如果内部Map的成员很多，那么涉及到遍历整个内部Map的操作，由于Redis单线程模型的缘故，这个遍历操作可能会比较耗时，而另其它客户端的请求完全不响应，这点需要格外注意。
参考：[http://www.cnblogs.com/stephen-liu74/archive/2012/04/16/2370212.html](http://www.cnblogs.com/stephen-liu74/archive/2012/04/16/2370212.html)
2、redis存储结构之应用解析
如果是一个键对应一个值并且多个field和多个value的值，如整个背包的道具（键是角色id），可使用hash存储结构。
如果内容是一个键对应一个value，如角色属性，可考虑使用string存储结构。
hash存储结构（应用于装备背包道具）
|**HSET** key field value|O(1)|为指定的Key设定Field/Value对，如果Key不存在，该命令将创建新Key以参数中的Field/Value对，如果参数中的Field在该Key中已经存在，则用新值覆盖其原有值。|1表示新的Field被设置了新值，0表示Field已经存在，用新值覆盖原有值。|
|----|----|----|----|
|**HGET** key field|O(1)|返回指定Key中指定Field的关联值。|返回参数中Field的关联值，如果参数中的Key或Field不存，返回nil。|
|**HEXISTS**key field|O(1)|判断指定Key中的指定Field是否存在。|1表示存在，0表示参数中的Field或Key不存在。|
|**HLEN** key|O(1)|获取该Key所包含的Field的数量。|返回Key包含的Field数量，如果Key不存在，返回0。|
|**HGETALL**key|O(N)|时间复杂度中的N表示Key包含的Field数量。获取该键包含的所有Field/Value。其返回格式为一个Field、一个Value，并以此类推。|Field/Value的列表。|
|**HMSET** key field value [field value ...]|O(N)|时间复杂度中的N表示被设置的Field数量。逐对依次设置参数中给出的Field/Value对。如果其中某个Field已经存在，则用新值覆盖原有值。如果Key不存在，则创建新Key，同时设定参数中的Field/Value。|
参考：[http://www.cnblogs.com/stephen-liu74/archive/2012/02/15/2352932.html](http://www.cnblogs.com/stephen-liu74/archive/2012/02/15/2352932.html)
string存储结构（应用于角色属性）
|**GET** key|O(1)|获取指定Key的Value。如果与该Key关联的Value不是string类型，Redis将返回错误信息，因为GET命令只能用于获取string Value。|与该Key相关的Value，如果该Key不存在，返回nil。|
|----|----|----|----|
|**SET** key value|O(1)|设定该Key持有指定的字符串Value，如果该Key已经存在，则覆盖其原有值。|总是返回"OK"。|
参考：[http://www.cnblogs.com/stephen-liu74/archive/2012/02/13/2349815.html](http://www.cnblogs.com/stephen-liu74/archive/2012/02/13/2349815.html)
3、写入redis的客户端应用
装备道具结构：
**[cpp]**[view
 plain](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/396377/fork)
- struct CBagItem  
- {  
- int m_nID;  //物品ID
- int m_nCount; //物品个数 默认一
- //物品的可变属性
- int m_nUsedHole;  //能用孔的个数
- int m_GremId[MAXGREMNUM];  //孔里面放置的宝石id 目前武器最多有5个孔
- int m_nDeadTime;  //使用的结束时间
- bool m_IsDead;    //是否到期
- bool m_IsBand;//是否帮定
- int m_augmentLevel; //强化等级
- int m_score; //装备评分
- public:  
- strengthen m_nStreng;//装备属性
- int m_nCurNum;//当前已有进度
- int m_nUpNum; //下一等级升级所需数量
- };  
(1)存储一个装备道具到redis(使用hset 命令)
**[cpp]**[view
 plain](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/396377/fork)
- void CRWRedisClientOperator::add_equip_to_redis(CGamePlayer* player,int pos,CBagItem* bagItem)  
- {  
- if(NULL == player||NULL == bagItem))  
- {  
- return;  
- }  
- int playerId = player->get_player_base()->m_player_id;  
- //实际是以玩家id和装备关键字来作为key。例如  ：  EQUIPMENTBAG%d
- char tmpBuf[256];  
- memset(tmpBuf,0,sizeof(tmpBuf));  
- sprintf(tmpBuf,"EQUIPMENTBAG%d",playerId);  
- string key(tmpBuf);  
- 
- memset(tmpBuf,0,sizeof(tmpBuf));  
- sprintf(tmpBuf,"%d",pos);  
- string frield(tmpBuf);  
- memset(tmpBuf,0,sizeof(tmpBuf));  
- //这里可以直接用道具bagItem的地址是因为CBagItem类型里面的成员都是原子类型的，如果有容器（stl或其他的容器），则需要自己提供一个序列化的函数。
- memcpy(tmpBuf,(void*)bagItem,sizeof(CBagItem));  
- string value;  
- value.assign(tmpBuf,sizeof(CBagItem));//这里是把一个道具的内存存到一个字符串类型（string）的内存里，最大的长度是256字节，不可以超过这个长度。
- 
- CRWRedisClient redisClient;  
- redis::client* tmpRedisClient = redisClient.get_redis_client();//这里是redis客户端的一个实例池，做了个简单的封装
- if(NULL == tmpRedisClient)  
- {  
- return;  
- }  
- try
- {  
- tmpRedisClient->hset(key,frield,value);//key : EQUIPMENTBAGplayerId  frield: pos value:CBagItem
- //设置过期时间 30天，需要设置redis内存数据的期限，鉴于内存受限
- tmpRedisClient->expire(key,3600*24*30);  
- }  
- catch (redis::redis_error & e)  
- {  
- cerr << "got exception: " << e.what() << endl << "FAIL" << endl;  
- return;  
- }  
- }  
(2)一次存储一个玩家的装备背包里的所有道具(使用命令hmset)
存储结构：
key : EQUIPMENTBAGplayerId  frield: pos value:CBagItem
**[cpp]**[view
 plain](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/396377/fork)
- void CRWRedisClientOperator::player_equipbag_insert_db_redis(CGamePlayer* player)  
- {  
- if(NULL == player)  
-     {  
- return;  
-     }  
-     CPlayerBag* bag = player->get_player_bag();  
- if(NULL == bag)  
-     {  
- return;  
-     }  
- //玩家装备道具的map存储到strPairVec，然后调用接口void hmset( const string_type & key, const string_pair_vector & field_value_pairs )发送到redis 服务器
-     map<int,CBagItem*>* tmpMap = bag->get_equip_map();  
-     map<int,CBagItem*>::iterator iter = tmpMap->begin();  
- 
- //以hash格式插入redis的元素
-     vector<pair<string,string> > strPairVec;  
- for(;iter != tmpMap->end();iter++)  
-     {  
-         CBagItem* equipItem = iter->second;  
- if(NULL == equipItem)  
-         {  
- return;  
-         }  
- //设置frield值，道具id
- char frieldBuf[64];  
-         memset(frieldBuf, 0, sizeof(frieldBuf));  
-         sprintf(frieldBuf,"%d",iter->first);  
-         string frield(frieldBuf);  
- 
- //设置value，道具二进制信息
- char valueBuf[256];  
-         memset(valueBuf, 0, sizeof(valueBuf));  
-         memcpy(valueBuf,equipItem,sizeof(CBagItem));  
-         string value;  
-         value.assign(valueBuf,sizeof(CBagItem));  
-         strPairVec.push_back(pair<string,string>(frield,value));  
-     }  
- //通过playerId得到key值 EQUIPMENTBAGplayerId
- int playerId = player->get_player_base()->m_player_id;  
- if(0 == playerId)  
-     {  
- return;  
-     }  
- char tmpBuf[64];  
-     memset(tmpBuf,0,64);  
-     sprintf(tmpBuf,"EQUIPMENTBAG%d",playerId);  
-     string key(tmpBuf);  
- 
-     CRWRedisClient redisClient;  
-     redis::client* tmpRedisClient = redisClient.get_redis_client();  
- if(NULL == tmpRedisClient)  
-     {  
- return ;  
-     }  
- try
-     {  
- //使用命令hmset 设置 key : EQUIPMENTBAGplayerId  frield: pos value:CBagItem
-         tmpRedisClient->hmset(key,strPairVec);  
- //设置过期时间 30天
-         tmpRedisClient->expire(key,60*60*24*30);  
-     }  
- catch (redis::redis_error & e)  
-     {  
-         cerr << "got exception: " << e.what() << endl << "FAIL" << endl;  
- return ;  
-     }  
- }  
(3)存储一个角色的基础信息（使用命令set）
存储结构：
key：BASE角色id ，value 角色基础信息 
**[cpp]**[view
 plain](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/396377/fork)
- int playerId = player->get_player_base()->m_player_id;  
- char tmpBuf[64];  
- memset(tmpBuf,0,64);  
- sprintf(tmpBuf,"BASE%d",playerId);  
- string key(tmpBuf);  
- CRWRedisClient redisClient;  
- redis::client* tmpRedisClient = redisClient.get_redis_client();  
- if(NULL == tmpRedisClient)  
- {  
- return ;  
- }  
- try
- {  
-     tmpRedisClient->set(key, value);  
- }  
- catch (redis::redis_error & e)  
- {  
-     cerr << "got exception: " << e.what() << endl << "FAIL" << endl;  
- return ;  
- }  
4、读取redis的客户端应用
（1）一次获取一个玩家的装备包裹的所有道具（使用命令hgetall）
存储结构：
key : EQUIPMENTBAGplayerId  frield: pos value:CBagItem
**[cpp]**[view
 plain](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/396377/fork)
- bool CRWRedisClientOperator::load_player_equipbag_from_redis(CGamePlayer* player)  
- {  
- if(NULL == player)  
-     {  
- returnfalse;  
-     }  
- //通过playerId得到key值
- int playerId = player->get_player_base()->m_player_id;  
- if(0 == playerId)  
-     {  
- returnfalse;  
-     }  
- 
- char tmpBuf[64];  
-     memset(tmpBuf,0,64);  
- //根据键（EQUIPMENTBAG%d",playerId），获取该玩家 所有装备道具的数据到vector<pair<string,string> > strPairVec
- //使用到接口void hgetall( const string_type & key, string_pair_vector & out )
-     sprintf(tmpBuf,"EQUIPMENTBAG%d",playerId);  
-     string key(tmpBuf);  
- if(false == CRWRedisClientOperator::instance()->is_key_exist_in_redis(key))  
-     {  
- returnfalse;  
-     }  
-     vector<pair<string,string> > strPairVec;  
- //通过key值从redis取数据
-     CRWRedisClient redisClient;  
-     redis::client* tmpRedisClient = redisClient.get_redis_client();  
- if(NULL == tmpRedisClient)  
-     {  
- returnfalse;  
-     }  
- try
-     {  
-         tmpRedisClient->hgetall(key,strPairVec);//获取一个玩家的所有装备背包道具
-     }  
- catch (redis::redis_error & e)  
-     {  
-         cerr << "got exception: " << e.what() << endl << "FAIL" << endl;  
- returnfalse;  
-     }  
- //得到背包
-     CPlayerBag* tmpBag = player->get_player_bag();  
- if(NULL == tmpBag)  
-     {  
- returnfalse;  
-     }  
-     vector<pair<string,string> >::iterator iter = strPairVec.begin();  
- for(;iter != strPairVec.end();iter++)  
-     {  
-         string frield = iter->first;  
- int pos = ACE_OS::atoi(frield.c_str());  
-         string value = iter->second;  
- if(value.length() != sizeof(CBagItem))  
-         {  
- returnfalse;  
-         }  
-         CBagItem* equipItem = new CBagItem();  
- if(NULL == equipItem)  
-         {  
- returnfalse;  
-         }  
-         memcpy(equipItem,value.c_str(),value.length());  
- if(!tmpBag->insert_bagitem_equip(pos,equipItem))  
-         {  
- returnfalse;  
-         }  
-     }  
- returntrue;  
- }  
（2）读取角色的基础信息（使用命令get）
存储结构：
key BASE角色id，value 角色基础信息
**[cpp]**[view
 plain](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/396377/fork)
- char tmpBuf[64];  
- memset(tmpBuf,0,64);  
- sprintf(tmpBuf,"BASE%d",player->get_player_base()->m_player_id);  
- string key(tmpBuf);  
- 
- struct CPlayerBase playerMsg;  
- memset(&playerMsg, 0, sizeof(CPlayerBase));  
- string getValue;  
- CRWRedisClient redisClient;  
- redis::client* tmpRedisClient = redisClient.get_redis_client();  
- 
- if(NULL == tmpRedisClient)  
- {  
- returnfalse;  
- }  
- try
- {  
- if (false == tmpRedisClient->exists(key))  
-     {  
-         cout <<key <<" not exists !!!" << endl;  
- returnfalse;  
-     }  
-     getValue = tmpRedisClient->get(key);  
- }  
- catch (redis::redis_error & e)//对于会抛出异常的接口，需要捕捉异常
- {  
-     cerr << "got exception: " << e.what() << endl << "FAIL" << endl;  
- returnfalse;  
- }  
- 
- if(getValue.length() > sizeof(CPlayerBase))//如果比需要的长度要大，则是不合法的
- {  
- returnfalse;  
- }  
- memcpy(&playerMsg, getValue.c_str(), getValue.length());//直接在redis,copy到player中
- //开始拷贝数据到角色指针的数据里
- player->get_player_base()->m_player_id = playerMsg.m_player_id;  
- ......  
5、redis客户端池
客户端池的初始化
**[cpp]**[view
 plain](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/396377/fork)
- bool CRedisClientPool::init_redis_client_pool()  
- {  
-     CRedisServer tmpRedisServer = CConfigManager::instance()->get_srv_config().get_redis_server_conf();  
-     string redisIp = tmpRedisServer.get_ip();  
- int port = tmpRedisServer.get_port();  
- for(int i = 0; i < 5; i++)  
-     {  
- if(false == init_redis_client(redisIp,port))  
-         {  
- returnfalse;  
-         }  
-     }  
- returntrue;  
- }  
客户端连接初始化
**[cpp]**[view
 plain](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/396377/fork)
- bool CRedisClientPool::init_redis_client(string redisIp, int port)  
- {  
- try
-     {  
-         redis::client* m_redis_client = new redis::client(redisIp,port,"");  
- if(NULL != m_redis_client)  
-         {  
-             push_redis_client(m_redis_client);//压到redis客户端池列表
-         }  
-     }  
- catch (redis::redis_error & e)  
-     {  
-         cerr << "got exception: " << e.what() << endl << "FAIL" << endl;  
- returnfalse;  
-     }  
- returntrue;  
- }  
添加客户端连接到客户端池
**[cpp]**[view
 plain](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/396377/fork)
- void CRedisClientPool::push_redis_client(redis::client* redisClient)  
- {  
- if(NULL == redisClient)  
-     {  
- return ;  
-     }  
-     m_q_mutex.acquire();  
-     m_redis_client_list.push_back(redisClient);  
-     m_q_mutex.release();  
- }  
6、第三方的库接口
(1)redis客户端对象
**[cpp]**[view
 plain](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/396377/fork)
- typedef base_client<default_hasher> client;  
- struct default_hasher  
-   {  
- inlinesize_t operator()(const std::string & key, const std::vector<connection_data> & connections)  
-     {  
- return boost::hash<std::string>()(key) % connections.size();//每次操作是根据键哈希获取连接列表里的一个，这样每个键可以尽量使用不同的连接（可能是为了某些多线程场景的减少锁竞争）
-     }  
-   };  
- 
- template<typename CONSISTENT_HASHER>  
- class base_client  
-   {  
- private:  
- void init(connection_data & con)  
-     {  
- char err[ANET_ERR_LEN];  
-       con.socket = anetTcpConnect(err, const_cast<char*>(con.host.c_str()), con.port);//使用anet库做网络通信客户端接口
- if (con.socket == ANET_ERR)  
-       {  
-         std::ostringstream os;  
-         os << err << " (redis://" << con.host << ':' << con.port << ")";  
- throw connection_error( os.str() );  
-       }  
-       anetTcpNoDelay(NULL, con.socket);  
-       select(con.dbindex, con);  
-     }  
- 
- 
- public:  
-    ...  
- explicit base_client(const string_type & host = "localhost",  
-                     uint16_t port = 6379,const string_type &pwd ="",int_type dbindex = 0)  
-     {  
-       connection_data con;  
-       con.host = host;  
-       con.port = port;  
-       con.dbindex = dbindex;  
-       con.pwd = pwd;  
-       init(con);  
-       connections_.push_back(con);//初始化连接后放到连接列表里
-     }  
（2）接口函数
获取redis的哈希表的值
**[cpp]**[view
 plain](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/396377/fork)
- void hgetall( const string_type & key, string_pair_vector & out )  
-     {  
- int socket = get_socket(key);  
-       send_(socket, makecmd("HGETALL") << key);//命令
-       string_vector s;  
-       recv_multi_bulk_reply_(socket, s);//获取多个返回的回应消息作为HGETALL 的结果
- for(size_t i = 0; i < s.size(); i+=2)  
-         out.push_back( make_pair(s[i], s[i+1]) );  
-     }  
插入到redis的哈希表用到的
**[cpp]**[view
 plain](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/396377/fork)
- void hmset( const string_type & key, const string_pair_vector & field_value_pairs )  
-     {  
- int socket = get_socket(key);  
-       makecmd m("HMSET");//命令
-       m << key;  
- for(size_t i=0; i < field_value_pairs.size(); i++)  
-         m << field_value_pairs[i].first << field_value_pairs[i].second;//把一个vector的键值发送过去设置
-       send_(socket, m);  
-       recv_ok_reply_(socket);//接收应答结果
-     }  
get命令
**[cpp]**[view
 plain](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/396377/fork)
- string_type get(const string_type & key)  
-     {  
- int socket = get_socket(key);  
-       send_(socket, makecmd("GET") << key);  
- return recv_bulk_reply_(socket);  
-     }  
读取网络数据
**[cpp]**[view
 plain](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/396377/fork)
- std::string recv_bulk_reply_(int socket)  
-     {  
-       int_type length = recv_bulk_reply_(socket, REDIS_PREFIX_SINGLE_BULK_REPLY );  
- 
- if (length == -1)  
- return missing_value();  
- 
-       int_type real_length = length + 2;    // CRLF
- 
-       std::string data = read_n(socket, real_length);  
- 
- #ifndef NDEBUG
- //output_proto_debug(data.substr(0, data.length()-2));
- #endif
- 
- if (data.empty())  
- throw protocol_error("invalid bulk reply data; empty");  
- 
- if (data.length() != static_cast<std::string::size_type>(real_length))  
- throw protocol_error("invalid bulk reply data; data of unexpected length");  
- 
-       data.erase(data.size() - 2);  
- 
- return data;  
-     }  
（3）定义的异常
定义的redis异常的基础类
**[cpp]**[view
 plain](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/18887647#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/396377/fork)
- class redis_error : public std::exception  
-   {  
- public:  
-     redis_error(const std::string & err) : err_(err) {}  
- virtual ~redis_error() throw () {}  
-     operator const std::string () const { return err_; }  
- virtualconstchar* what() constthrow ()  
-     {  
- return err_.c_str();  
-     }  
- 
- 
- private:  
-     std::string err_;  
-   };  
- 


