
# online游戏服务器架构—用户登录数据组织 . - 高科的专栏 - CSDN博客

2012年11月24日 00:33:12[高科](https://me.csdn.net/pbymw8iwm)阅读数：5959


sprite_t类型的数据结构是核心数据结构，每一个登录用户对应一个，它的初始化在用户登录的时候，此后一直到用户退出或者离线一直保存在系统内存当中，在此过程中该sprite_t数据结构被保存在两个哈希表当中，一个是以用户的id为索引，这个是逻辑相关的，另一个是以此连接的套结字描述符为索引，这个是逻辑无关的：
int parse_protocol(uint8_t *data, int rcvlen, fdsession_t* fdsess)
{
protocol_t pkg;
sprite_t *p, tmp; //tmp是个局部变量，分配于栈上，由于此后的执行续是串行的，也就是说在动态分配sprite_t数据结构于堆上之前并不清除此函数的调用栈帧，因此这里使用局部变量很安全。
int i;
i = 0;
//此处用UNPKG_XX系列解析pkg.len, pkg.ver, pkg.cmd, pkg.id, pkg.ret
…
p = get_sprite_by_fd(fdsess->fd); //以套结字描述符查找sprite_t数据结构，如果该用户已经登录，那么一定能查找到的，因为用户和online的交互是长连接，如果是登录包，那么肯定查不到，因为这是第一个包
if ((pkg.cmd != PROTO_LOGIN && !p) || (pkg.cmd == PROTO_LOGIN && p) …){
ERROR_RETURN(("pkg error”);
}
…
if (pkg.cmd == PROTO_LOGIN) {
sprite_t* old = get_sprite(pkg.id); //以id为索引查找该用户是否已经登录
if (old) notify_user_exit(old, -ERR_multi_login, 1); //如果已经登录，那么踢出已经登录的用户
p = &tmp;
memset(p, 0, sizeof(*p));
p->id = pkg.id; //设置用户的id，登录期间一定唯一
p->item_cnt = 0;
p->fd = fdsess->fd; //设置fd，套结字描述符，一定是唯一的
p->fdsess = fdsess;
}
return dispatch_protocol(p, pkg.cmd, data + sizeof (pkg), pkg.len - sizeof (pkg));
}
由此可见，一个sprite_t数据结构连接在两个哈希列表中，一个是套结字描述符为索引的，另一个是用户id为索引的，注意这两个索引都是唯一的索引。每每分配一个sprite_t数据结构都要将之插入到两个哈希表当中，以套结字描述符为索引的查找函数如下：
sprite_t* get_sprite_by_fd(int fd)
{
sprite_t* p = g_hash_table_lookup(all_sprites, &fd);
if ( !p || IS_NPC_ID(p->id) )
return 0;
return p;
}
以用户id为索引的查找函数如下：
static inline sprite_t *get_sprite (uint32_t id)
{
sprite_t *p;
list_for_each_entry (p, &idslots[id % HASH_SLOT_NUM], hash_list)
if (p->id == id)
return p;
return NULL;
}
对于登录包，最终dispatch_protocol会进入到auth_cmd，该函数对用户的一些信息进行一些如MD5之类的验证，然后进入到do_auth函数：
static inline int
do_auth(sprite_t* v)
{
sprite_t* p = add_sprite(v); //该函数将新分配的sprite_t数据结构插入到两个哈希链表当中
notify_user_login(p, 1);
ADD_TIMER_EVENT(p, long_time_min45_in_game, 0, now.tv_sec + 45*60);
ADD_TIMER_EVENT(p, long_time_min10_in_game, 0, now.tv_sec + 10*60);
if (IS_GUEST_ID(p->id)) { //如果是访客的话进入下面流程
enter_map(p, 1, 0); //为访客直接设置地图
rsp_proto_login(p);
return 0;
} else {
return db_get_sprite_with_mail(p);
}
}
sprite_t* add_sprite(sprite_t* v)
{
sprite_t* p = alloc_sprite(v->fd);
*p = *v;
p->stamp = now.tv_sec;
INIT_LIST_HEAD(&p->hash_list);
INIT_LIST_HEAD(&p->map_list);
INIT_LIST_HEAD(&p->timer_list);
…
list_add_tail(&p->hash_list, &idslots[p->id % HASH_SLOT_NUM]); //插入到以用户id为索引值的哈希链表
return p;
}
static inline sprite_t* alloc_sprite(int fd)
{
sprite_t* p = g_slice_alloc(SPRITE_STRUCT_LEN); //分配一个sprite_t数据结构
p->fd = fd; //初始化一个套结字索引值
g_hash_table_insert(all_sprites, &(p->fd), p); //以套结字描述符为索引插入到全局的哈希链表
++sprites_count;
return p;
}
enter_map是一个很重要的函数，它设置了玩家的地图信息，每一个地图都是一个数据结构map_t，里面包含一个list_head类型的数据结构sprite_list_head，而每一个sprite_t数据结构都有一个list_head类型的map_list，每次初始化完了一个sprite_t之后最终都要调用一个list_add_tail (&p->map_list, &tile->sprite_list_head)将该sprite_t加入到一个map的list当中，这里的list_head就是linux内核中的最常见的list_head数据结构


