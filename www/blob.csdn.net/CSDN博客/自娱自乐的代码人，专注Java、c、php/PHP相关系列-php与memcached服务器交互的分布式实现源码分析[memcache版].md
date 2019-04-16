# PHP相关系列 - php与memcached服务器交互的分布式实现源码分析[memcache版] - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年01月11日 20:43:19[initphp](https://me.csdn.net/initphp)阅读数：1032









前段时间，因为一个项目的关系，研究了php通过调用[memcache](http://php.net/manual/en/book.memcache.php)和[memcached](http://www.php.net/manual/en/book.memcached.php) PECL扩展库的接口存储到分布式缓存服务器的机制，在此做我根据他们各自的源码进行分析，希望能对这方面感兴趣的人有些帮助。
本篇文章我会针对php和memcache扩展库的交互根据源码展开分析。
PHP调用memcache的接口通常会是如下过程：

- <?php  
- $mmc = new Memcache();  
- $mmc->addServer('node1', 11211);  
- $mmc->addServer('node2', 11211, MemcacheConfig::MEMCACHE_PERSISTENT, 2);  
- $mmc->set('key', 'value');  
- echo$mmc->get('key');  
- $mmc->delete('key');  


短短几行代码，一个缓存key的生命周期就已经完整层现。从Memcache的初始化，到addServer添加两个服务器节点，接着set一个key到服务器上，然后get到这个key输出，最后delete这个key。在这个生命周期里，Memcache在底层究竟做了哪些事情，保证了数据存储服务器的均匀分布，数据的完整性？
接下来，我会根据上述生命周期的顺序，循序渐进的分析（由于主题是分布式算法的分析，所以接下来不相干的代码我会略去，很多分析我会直接备注在源码上）。


**1. Memcache的初始化**
对应PHP的代码：

- $mmc = new Memcache();  


对应C的代码：// Memcache类对应的方法名已经实际在c中实现过程的函数名，在接下来的分析中会用到。忽略不会分析到的方法。

- static zend_function_entry php_memcache_class_functions[] = {  
- PHP_FALIAS(addserver, memcache_add_server, NULL)  
- PHP_FALIAS(set, memcache_set, NULL)  
- PHP_FALIAS(get, memcache_get, NULL)  
- PHP_FALIAS(delete, memcache_delete, NULL)  
- ......  
- };  
- PHP_MINIT_FUNCTION(memcache)  
- {  
- // 初始化Memcache类实体，给类定在php空间中的调用名称以及类所拥有的方法
- zend_class_entry memcache_class_entry;  
- INIT_CLASS_ENTRY(memcache_class_entry, "Memcache", php_memcache_class_functions);  
- memcache_class_entry_ptr = zend_register_internal_class(&memcache_class_entry TSRMLS_CC);  
- ......  
- }  



以上过程是在Module Initialization的环节已经做好，在new的过程中，并无其余处理。
**2. 添加缓存服务器，使之成为分布式存储**

对应PHP的代码：

- $mmc->addServer('node1', 11211);  
- $mmc->addServer('node2', 11211, MemcacheConfig::MEMCACHE_PERSISTENT, 2);  




由上面的php_memcache_class_functions结构可以看出，addServer方法对应的是memcache_add_server函数，因此对应C的代码：

- PHP_FUNCTION(memcache_add_server)  
- {  
- zval **connection, *mmc_object = getThis(), *failure_callback = NULL;  
- // 整个Memcache中最重要的一个结构mmc_pool_t
- mmc_pool_t *pool;  
- // 当前新添服务器的结构变量
- mmc_t *mmc;  
- ......  
- // 如果pool之前没有初始化过，则初始化
- if (zend_hash_find(Z_OBJPROP_P(mmc_object), "connection", sizeof("connection"), (void **) &connection) == FAILURE) {  
- // 调用mmp_pool_new完成初始化
- pool = mmc_pool_new(TSRMLS_C);  
- ......  
- }  
- else {  
- ......  
- }  
- //将新增服务器添加到pool中
- mmc_pool_add(pool, mmc, weight);  
- RETURN_TRUE;  
- }  


来看下mmc_pool_t结构的定义：



- typedefstruct mmc_pool {  
- mmc_t **servers; // 所有服务器的状态
- int num_servers; // 服务器数量
- mmc_t **requests; // 根据get的array key请求顺序返回的服务器数组状态
- int compress_threshold; // 待存储的数据压缩的下限值
- double min_compress_savings; // 待存储的数据最小的压缩百分比
- zend_bool in_free; // 标记该pool是否被释放
- mmc_hash_t *hash; // hash策略容器
- void *hash_state; // hash函数
- } mmc_pool_t;  


然后我们看下mmc_hash_t的结构，再接下去的分析中会用到：// 结构定义中包含了四种抽象函数，作为基本结构，用于定义子结构

- typedefstruct mmc_hash {  
- mmc_hash_create_state create_state; // 创建hash策略状态，主要是接纳了hash函数算法
- mmc_hash_free_state free_state; // 释放hash策略状态
- mmc_hash_find_server find_server; // 根据key和分布式算法定位到某台服务器
- mmc_hash_add_server add_server; // 根据hash策略、算法以及权重值添加服务器资源
- } mmc_hash_t;  




接着我们追踪memcache_add_server函数中的mmc_pool_new函数调用方法：

- typedefstruct mmc_hash {  
- mmc_hash_create_state create_state; // 创建hash策略状态，主要是接纳了hash函数算法
- mmc_hash_free_state free_state; // 释放hash策略状态
- mmc_hash_find_server find_server; // 根据key和分布式算法定位到某台服务器
- mmc_hash_add_server add_server; // 根据hash策略、算法以及权重值添加服务器资源
- } mmc_hash_t;  




现在初始化hash算法已经逐渐显露，继续追踪mmc_pool_init_hash函数:



- staticvoid mmc_pool_init_hash(mmc_pool_t *pool TSRMLS_DC) /* {{{ */
- {  
- mmc_hash_function hash;// 初始化hash函数
- // 根据php.ini中的memcache.hash_strategy配置选择hash存储策略，默认为标准hash存储策略
- switch (MEMCACHE_G(hash_strategy)) {  
- case MMC_CONSISTENT_HASH:  
- pool->hash = &mmc_consistent_hash;// 采用持久化hash存储策略
- break;  
- default:  
- pool->hash = &mmc_standard_hash;// 采用标准hash存储策略
- }  




// 根据php.ini中的memcache.hash_function配置选择hash函数，默认为crc32算法

- switch (MEMCACHE_G(hash_function)) {  
- case MMC_HASH_FNV1A:  
- hash = &mmc_hash_fnv1a; // 采用fnv1a算法
- break;  
- default:  
- hash = &mmc_hash_crc32; // 采用crc32算法
- }  
- // hash策略中根据选择的hash函数创建对应的状态
- pool->hash_state = pool->hash->create_state(hash);  
- }  



根据上面的两个switch可以知道，在create_state的时候，是有两种策略选择的可能性，接着传入的hash参数也存在两种可能性，这里我先分析标准hash存储策略，以及对应的两种hash算法，然后再分析持久化hash策略。
先看下mmc_consistent_hash结构：// 根据mmc_hash_t的定义包含了四种具体函数实现

- mmc_hash_t mmc_standard_hash = {  
- mmc_standard_create_state,  
- mmc_standard_free_state,  
- mmc_standard_find_server,  
- mmc_standard_add_server  
- };  




由上可知，pool->hash->create_state的函数调用实际是对mmc_standard_create_state的函数调用，继续看mmc_standard_create_state函数代码的实现：



- // hash策略状态
- typedefstruct mmc_standard_state {  
- int num_servers; // 服务器数量
- mmc_t **buckets; // 哈希桶，和权重值相关
- int num_buckets; // 哈系桶的数量
- mmc_hash_function hash; // hash算法
- } mmc_standard_state_t;  
- 
- void *mmc_standard_create_state(mmc_hash_function hash) /* {{{ */
- {  
- // 初始化状态
- mmc_standard_state_t *state = emalloc(sizeof(mmc_standard_state_t));  
- memset(state, 0, sizeof(mmc_standard_state_t));  
- // 选择的hash函数赋给hash属性
- state->hash = hash;  
- return state;  
- }  




crc的算法实现：



- static unsigned int mmc_hash_crc32(constchar *key, int key_len) /* CRC32 hash {{{ */
- {  
- unsigned int crc = ~0;  
- int z;  
- 
- for (z=0; z<key_len; z++) {  
- CRC32(crc, key[z]);  
- }  
- 
- return ~crc;  
- }  






有关CRC32再深入的实现可以参考[Cyclic redundancy check](http://en.wikipedia.org/wiki/Cyclic_redundancy_check)

然后来看看fnv算法实现：



- /* 32 bit magic FNV-1a prime and init */
- #define FNV_32_PRIME 0x01000193
- #define FNV_32_INIT 0x811c9dc5
- static unsigned int mmc_hash_fnv1a(constchar *key, int key_len) /* FNV-1a hash {{{ */
- {  
- unsigned int hval = FNV_32_INIT;  
- int z;  
- 
- for (z=0; z<key_len; z++) {  
- hval ^= (unsigned int)key[z];  
- hval *= FNV_32_PRIME;  
- }  
- 
- return hval;  
- }  




具体fnv算法的深入实现可以参考[Fowler–Noll–Vo hash function](http://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function)

最后我们看看mmc_consistent_hash结构：



- mmc_hash_t mmc_consistent_hash = {  
- mmc_consistent_create_state,  
- mmc_consistent_free_state,  
- mmc_consistent_find_server,  
- mmc_consistent_add_server  
- };  




一样是四个函数，看下对应的create_state中的mmc_consistent_create_state的实现：



- /* number of precomputed buckets, should be power of 2 */
- #define MMC_CONSISTENT_BUCKETS 1024
- 
- typedefstruct mmc_consistent_point {  
- mmc_t *server; // 服务器状态
- unsigned int point; // 对应的指针
- } mmc_consistent_point_t;  
- 
- typedefstruct mmc_consistent_state {  
- int num_servers; // 服务器数量
- mmc_consistent_point_t *points; // 持久化服务器指针
- int num_points; // 指针数量
- mmc_t *buckets[MMC_CONSISTENT_BUCKETS]; // 哈希桶
- int buckets_populated; //标记哈希桶是否计算过
- mmc_hash_function hash; // hash函数
- } mmc_consistent_state_t;  
- 
- void *mmc_consistent_create_state(mmc_hash_function hash) /* {{{ */
- {  
- // 初始化state
- mmc_consistent_state_t *state = emalloc(sizeof(mmc_consistent_state_t));  
- memset(state, 0, sizeof(mmc_consistent_state_t));  
- // 将hash函数赋值给hash属性
- state->hash = hash;  
- return state;  
- }  




至此，memcache_add_server中mmc_pool_new函数流程结束，接着来看mmc_pool_add函数：



- void mmc_pool_add(mmc_pool_t *pool, mmc_t *mmc, unsigned int weight) /* {{{ */
- {  
- /* add server and a preallocated request pointer */
- if (pool->num_servers) {  
- pool->servers = erealloc(pool->servers, sizeof(mmc_t *) * (pool->num_servers + 1));  
- pool->requests = erealloc(pool->requests, sizeof(mmc_t *) * (pool->num_servers + 1));  
- }  
- else {  
- pool->servers = emalloc(sizeof(mmc_t *));  
- pool->requests = emalloc(sizeof(mmc_t *));  
- }  
- 
- pool->servers[pool->num_servers] = mmc;  
- pool->num_servers++;  
- // 根据pool状态，当前要添加的服务器状态和权重调用add_server函数
- pool->hash->add_server(pool->hash_state, mmc, weight);  
- }  




由上面的说明可知add_server在标准hash模式下对应mmc_standard_add_server函数：



- void mmc_standard_add_server(void *s, mmc_t *mmc, unsigned int weight) /* {{{ */
- {  
- mmc_standard_state_t *state = s;  
- int i;  
- 
- // 哈希桶初始化或重新分配相应的权重数值对应的空间
- if (state->num_buckets) {  
- state->buckets = erealloc(state->buckets, sizeof(mmc_t *) * (state->num_buckets + weight));  
- }  
- else {  
- state->buckets = emalloc(sizeof(mmc_t *) * (weight));  
- }  
- // 在某个区间内为哈希桶赋予服务器状态
- for (i=0; i<weight; i++) {  
- buckets[state->num_buckets + i] = mmc;  
- }  
- 
- state->num_buckets += weight;  
- state->num_servers++;  
- }  




在持久化hash模式下，对应的是mmc_consistent_add_server函数：



- #define MMC_CONSISTENT_POINTS 160 /* points per server */
- 
- void mmc_consistent_add_server(void *s, mmc_t *mmc, unsigned int weight) /* {{{ */
- {  
- mmc_consistent_state_t *state = s;  
- int i, key_len, points = weight * MMC_CONSISTENT_POINTS;  
- 
- /* buffer for "host:port-i\0" */
- char *key = emalloc(strlen(mmc->host) + MAX_LENGTH_OF_LONG * 2 + 3);  
- 
- /* add weight * MMC_CONSISTENT_POINTS number of points for this server */
- state->points = erealloc(state->points, sizeof(mmc_consistent_point_t) * (state->num_points + points));  
- 
- // 将区块内的server赋予当前服务器状态，point赋予hash函数处理后的值
- for (i=0; i<points; i++) {  
- key_len = sprintf(key, "%s:%d-%d", mmc->host, mmc->port, i);  
- state->points[state->num_points + i].server = mmc;  
- state->points[state->num_points + i].point = state->hash(key, key_len);  
- MMC_DEBUG(("mmc_consistent_add_server: key %s, point %lu", key, state->points[state->num_points + i].point));  
- }  
- 
- state->num_points += points;  
- state->num_servers++;  
- 
- // 新增加服务器后需重新计算buckets顺序
- state->buckets_populated = 0;  
- 
- efree(key);  
- }  









以上代码有持久化hash算法的赋值实现，具体深入的了解请看[Consistent hashing](http://en.wikipedia.org/wiki/Consistent_hashing)和国内大侠charlee翻译的小日本的文章[memcached全面剖析–PDF总结篇](http://tech.idv2.com/2008/08/17/memcached-pdf/)。
Consistent hashing 算法最大的特点是当你的缓存服务器数量变更的时候，它能够最大化的保留原有的缓存不变，而不需要重新分布原有缓存的服务器位置。
至此，整个memcache_add_server流程结束。
**3. 向缓存服务器保存数据**

对应PHP的代码：



- $mmc->set('key', 'value');  




由上面的分析可知，set方法对应的是memcache_set函数：



- /* {{{ proto bool memcache_set( object memcache, string key, mixed var [, int flag [, int expire ] ] )
- Sets the value of an item. Item may exist or not */
- PHP_FUNCTION(memcache_set)  
- {  
- // Memcache对象中的add，set和replace皆会走该函数
- php_mmc_store(INTERNAL_FUNCTION_PARAM_PASSTHRU, "set", sizeof("set") - 1);  
- }  






看php_mmc_store函数：



- staticvoid php_mmc_store(INTERNAL_FUNCTION_PARAMETERS, char *command, int command_len) /* {{{ */
- {  
- mmc_pool_t *pool;  
- ......  
- // 获得pool
- if (!mmc_get_pool(mmc_object, &pool TSRMLS_CC) || !pool->num_servers) {  
- RETURN_FALSE;  
- }  
- // 对不同的存储的值类型进行不同的处理
- switch (Z_TYPE_P(value)) {  
- // 字符串类型
- case IS_STRING:  
- result = mmc_pool_store(  
- pool, command, command_len, key_tmp, key_tmp_len, flags, expire,   
- Z_STRVAL_P(value), Z_STRLEN_P(value) TSRMLS_CC);  
- break;  
- // 长整型，浮点型，布尔型
- case IS_LONG:  
- case IS_DOUBLE:  
- case IS_BOOL: {  
- ......  
- result = mmc_pool_store(  
- pool, command, command_len, key_tmp, key_tmp_len, flags, expire,   
- Z_STRVAL(value_copy), Z_STRLEN(value_copy) TSRMLS_CC);  
- 
- zval_dtor(&value_copy);  
- break;  
- }  
- // 默认为数组类型
- default: {  
- ......  
- result = mmc_pool_store(  
- pool, command, command_len, key_tmp, key_tmp_len, flags, expire,   
- buf.c, buf.len TSRMLS_CC);  
- }  
- }  
- ......  
- }  




由上代码可以看出，存储数据主要是交由mmc_pool_store处理：



- int mmc_pool_store(mmc_pool_t *pool, constchar *command, int command_len, constchar *key, int key_len, int flags, int expire, constchar *value, int value_len TSRMLS_DC) /* {{{ */
- {  
- /* 该省略过程处理数据压缩，处理待发送的请求数据 */
- ......  
- 
- // 通过key确定待保存的服务器
- while (result < 0 && (mmc = mmc_pool_find(pool, key, key_len TSRMLS_CC)) != NULL) {  
- // 向缓存服务器发送请求，保存数据
- if ((result = mmc_server_store(mmc, request, request_len TSRMLS_CC)) < 0) {  
- mmc_server_failure(mmc TSRMLS_CC);  
- }  
- }  
- 
- if (key_copy != NULL) {  
- efree(key_copy);  
- }  
- if (data != NULL) {  
- efree(data);  
- }  
- efree(request);  
- return result;  
- }  




接着我们看下mmc_pool_find是处理的



- #define mmc_pool_find(pool, key, key_len) \
- pool->hash->find_server(pool->hash_state, key, key_len)  




原来是再次多态调用了find_server函数，由之前的分析可以得知find_server在标准hash模式中的函数为mmc_standard_find_server，在持久化hash模式中的函数为mmc_consistent_find_server，一样先看



- mmc_standard_find_servermmc_t *mmc_standard_find_server(void *s, constchar *key, int key_len TSRMLS_DC) /* {{{ */
- {  
- mmc_standard_state_t *state = s;  
- mmc_t *mmc;  
- 
- if (state->num_servers > 1) {  
- // 用设定的hash函数算法，找到对应的服务器
- unsigned int hash = mmc_hash(state, key, key_len), i;  
- mmc = state->buckets[hash % state->num_buckets];  
- 
- // 如果获取到的服务器状态有问题，则重新hash遍历寻找到可用的缓存服务器为止 
- for (i=0; !mmc_open(mmc, 0, NULL, NULL TSRMLS_CC) && MEMCACHE_G(allow_failover) && i<MEMCACHE_G(max_failover_attempts); i++) {  
- char *next_key = emalloc(key_len + MAX_LENGTH_OF_LONG + 1);  
- int next_len = sprintf(next_key, "%d%s", i+1, key);  
- MMC_DEBUG(("mmc_standard_find_server: failed to connect to server '%s:%d' status %d, trying next", mmc->host, mmc->port, mmc->status));  
- 
- hash += mmc_hash(state, next_key, next_len);  
- mmc = state->buckets[hash % state->num_buckets];  
- 
- efree(next_key);  
- }  
- }  
- else {  
- mmc = state->buckets[0];  
- mmc_open(mmc, 0, NULL, NULL TSRMLS_CC);  
- }  
- 
- return mmc->status != MMC_STATUS_FAILED ? mmc : NULL;  
- }  






再看



- mmc_consistent_find_servermmc_t *mmc_consistent_find_server(void *s, constchar *key, int key_len TSRMLS_DC) /* {{{ */
- {  
- mmc_consistent_state_t *state = s;  
- mmc_t *mmc;  
- 
- if (state->num_servers > 1) {  
- unsigned int i, hash = state->hash(key, key_len);  
- // 如果哈希桶没有进行过排序，则进行圆环排序操作
- if (!state->buckets_populated) {  
- mmc_consistent_populate_buckets(state);  
- }  
- mmc = state->buckets[hash % MMC_CONSISTENT_BUCKETS];  
- 
- // 如果获取到的服务器状态有问题，则重新hash遍历寻找到可用的缓存服务器为止 
- for (i=0; !mmc_open(mmc, 0, NULL, NULL TSRMLS_CC) && MEMCACHE_G(allow_failover) && i<MEMCACHE_G(max_failover_attempts); i++) {  
- char *next_key = emalloc(key_len + MAX_LENGTH_OF_LONG + 1);  
- int next_len = sprintf(next_key, "%s-%d", key, i);  
- MMC_DEBUG(("mmc_consistent_find_server: failed to connect to server '%s:%d' status %d, trying next", mmc->host, mmc->port, mmc->status));  
- 
- hash = state->hash(next_key, next_len);  
- mmc = state->buckets[hash % MMC_CONSISTENT_BUCKETS];  
- 
- efree(next_key);  
- }  
- }  
- else {  
- mmc = state->points[0].server;  
- mmc_open(mmc, 0, NULL, NULL TSRMLS_CC);  
- }  
- 
- return mmc->status != MMC_STATUS_FAILED ? mmc : NULL;  
- }  
- // 持久化哈希算法的核心部分
- staticvoid mmc_consistent_populate_buckets(mmc_consistent_state_t *state) /* {{{ */
- {  
- unsigned int z, step = 0xffffffff / MMC_CONSISTENT_BUCKETS;  
- 
- qsort((void *)state->points, state->num_points, sizeof(mmc_consistent_point_t), mmc_consistent_compare);  
- for (z=0; z<MMC_CONSISTENT_BUCKETS; z++) {  
- state->buckets[z] = mmc_consistent_find(state, step * z);  
- }  
- 
- state->buckets_populated = 1;  
- }  
- staticint mmc_consistent_compare(constvoid *a, constvoid *b) /* {{{ */
- {  
- if (((mmc_consistent_point_t *)a)->point < ((mmc_consistent_point_t *)b)->point) {  
- return -1;  
- }  
- if (((mmc_consistent_point_t *)a)->point > ((mmc_consistent_point_t *)b)->point) {  
- return 1;  
- }  
- return 0;  
- }  
- static mmc_t *mmc_consistent_find(mmc_consistent_state_t *state, unsigned int point) /* {{{ */
- {  
- int lo = 0, hi = state->num_points - 1, mid;  
- 
- while (1) {  
- /* point is outside interval or lo >= hi, wrap-around */
- if (point <= state->points[lo].point || point > state->points[hi].point) {  
- return state->points[lo].server;  
- }  
- 
- /* test middle point */
- mid = lo + (hi - lo) / 2;  
- MMC_DEBUG(("mmc_consistent_find: lo %d, hi %d, mid %d, point %u, midpoint %u", lo, hi, mid, point, state->points[mid].point));  
- 
- /* perfect match */
- if (point <= state->points[mid].point && point > (mid ? state->points[mid-1].point : 0)) {  
- return state->points[mid].server;  
- }  
- 
- /* too low, go up */
- if (state->points[mid].point < point) {  
- lo = mid + 1;  
- }  
- else {  
- hi = mid - 1;  
- }  
- }  
- }  






至此，memcache_set过程结束。



**4. 向缓存服务器获得已保存的数据**

对应PHP的代码：



- echo$mmc->get('key');  




由上面的分析可知，get方法对应的是memcache_get函数：



- PHP_FUNCTION(memcache_get)  
- {  
- ......  
- // 获得pool
- if (!mmc_get_pool(mmc_object, &pool TSRMLS_CC) || !pool->num_servers) {  
- RETURN_FALSE;  
- }  
- // 当key不为数组的情况下处理
- if (Z_TYPE_P(zkey) != IS_ARRAY) {  
- // 检查key的合法性
- if (mmc_prepare_key(zkey, key, &key_len TSRMLS_CC) == MMC_OK) {  
- // 获取key获取value
- if (mmc_exec_retrieval_cmd(pool, key, key_len, &return_value, flags TSRMLS_CC) < 0) {  
- zval_dtor(return_value);  
- RETVAL_FALSE;  
- }  
- }  
- else {  
- RETVAL_FALSE;  
- }  
- // 为数组的情况下处理
- } elseif (zend_hash_num_elements(Z_ARRVAL_P(zkey))){  
- //根据数据key获取数组值
- if (mmc_exec_retrieval_cmd_multi(pool, zkey, &return_value, flags TSRMLS_CC) < 0) {  
- zval_dtor(return_value);  
- RETVAL_FALSE;  
- }  
- } else {  
- RETVAL_FALSE;  
- }  
- }  






接着看mmc_exec_retrieval_cmd和mmc_exec_retrieval_cmd_multi函数：



- int mmc_exec_retrieval_cmd(mmc_pool_t *pool, constchar *key, int key_len, zval **return_value, zval *return_flags TSRMLS_DC) /* {{{ */
- {  
- mmc_t *mmc;  
- char *command, *value;  
- int result = -1, command_len, response_len, value_len, flags = 0;  
- 
- MMC_DEBUG(("mmc_exec_retrieval_cmd: key '%s'", key));  
- 
- command_len = spprintf(&command, 0, "get %s", key);  
- // 遍历寻找到key对应的value值
- while (result < 0 && (mmc = mmc_pool_find(pool, key, key_len TSRMLS_CC)) != NULL) {  
- ......  
- }  
- 
- if (return_flags != NULL) {  
- zval_dtor(return_flags);  
- ZVAL_LONG(return_flags, flags);  
- }  
- 
- efree(command);  
- return result;  
- }  
- staticint mmc_exec_retrieval_cmd_multi(mmc_pool_t *pool, zval *keys, zval **return_value, zval *return_flags TSRMLS_DC) /* {{{ */
- {  
- ......  
- do {  
- result_status = num_requests = 0;  
- zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(keys), &pos);  
- 
- // 遍历key得到所有key对应的服务器资源存入pool->requests中
- while (zend_hash_get_current_data_ex(Z_ARRVAL_P(keys), (void **)&zkey, &pos) == SUCCESS) {  
- if (mmc_prepare_key(*zkey, key, &key_len TSRMLS_CC) == MMC_OK) {  
- /* schedule key if first round or if missing from result */
- if ((!i || !zend_hash_exists(Z_ARRVAL_PP(return_value), key, key_len)) &&  
- // 根据key寻找到服务器
- (mmc = mmc_pool_find(pool, key, key_len TSRMLS_CC)) != NULL) {  
- if (!(mmc->outbuf.len)) {  
- smart_str_appendl(&(mmc->outbuf), "get", sizeof("get")-1);  
- pool->requests[num_requests++] = mmc;  
- }  
- 
- smart_str_appendl(&(mmc->outbuf), " ", 1);  
- smart_str_appendl(&(mmc->outbuf), key, key_len);  
- MMC_DEBUG(("mmc_exec_retrieval_cmd_multi: scheduled key '%s' for '%s:%d' request length '%d'", key, mmc->host, mmc->port, mmc->outbuf.len));  
- }  
- }  
- 
- zend_hash_move_forward_ex(Z_ARRVAL_P(keys), &pos);  
- }  
- 
- ......  
- 
- } while (result_status < 0 && MEMCACHE_G(allow_failover) && i++ < MEMCACHE_G(max_failover_attempts));  
- 
- ......  
- 
- return result_status;  
- }  




由上可见分布式hash的核心函数皆为mmc_pool_find，首先找到key对应的服务器资源，然后根据服务器资源请求数据。
至此，memcache_get的过程结束。
**5.向缓存服务器删除已保存的数据**
对应的php代码：

- $mmc->delete('key');  


由之前的分析可知，delete对应的为



- - memcache_delete：/* {{{ proto bool memcache_delete( object memcache, string key [, int expire ])
- Deletes existing item */
- PHP_FUNCTION(memcache_delete)  
- {  
- mmc_t *mmc;  
- mmc_pool_t *pool;  
- int result = -1, key_len;  
- zval *mmc_object = getThis();  
- char *key;  
- long time = 0;  
- char key_tmp[MMC_KEY_MAX_SIZE];  
- unsigned int key_tmp_len;  
- 
- if (mmc_object == NULL) {  
- if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Os|l", &mmc_object, memcache_class_entry_ptr, &key, &key_len, &time) == FAILURE) {  
- return;  
- }  
- }  
- else {  
- if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|l", &key, &key_len, &time) == FAILURE) {  
- return;  
- }  
- }  
- 
- if (!mmc_get_pool(mmc_object, &pool TSRMLS_CC) || !pool->num_servers) {  
- RETURN_FALSE;  
- }  
- 
- if (mmc_prepare_key_ex(key, key_len, key_tmp, &key_tmp_len TSRMLS_CC) != MMC_OK) {  
- RETURN_FALSE;  
- }  
- 
- // 先获得服务器资源
- while (result < 0 && (mmc = mmc_pool_find(pool, key_tmp, key_tmp_len TSRMLS_CC)) != NULL) {  
- // 根据资源向缓存服务器发送请求删除存储的数据 
- if ((result = mmc_delete(mmc, key_tmp, key_tmp_len, time TSRMLS_CC)) < 0) {  
- mmc_server_failure(mmc TSRMLS_CC);  
- }  
- }  
- 
- if (result > 0) {  
- RETURN_TRUE;  
- }  
- RETURN_FALSE;  
- }  
- /* }}} */








