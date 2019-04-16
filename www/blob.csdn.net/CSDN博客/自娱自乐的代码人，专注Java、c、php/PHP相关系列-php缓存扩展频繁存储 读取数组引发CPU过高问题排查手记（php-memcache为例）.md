# PHP相关系列 - php缓存扩展频繁存储/读取数组引发CPU过高问题排查手记（php-memcache为例） - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年11月01日 18:14:21[initphp](https://me.csdn.net/initphp)阅读数：1953
个人分类：[PHP相关系列](https://blog.csdn.net/initphp/article/category/2609095)











来源：[http://www.iirr.info/blog/?p=155](http://www.iirr.info/blog/?p=155)

最近进行性能排查的时候发现一个怪事：用php-memcache，缓存命中率越高CPU反而占用越大。

联想起之前用Xcache进行载入速度排除测试时也出现此问题，不禁疑惑了：不是说缓存命中率越高越好么？怎么变成烧CPU了？

今天周六总算空闲，决定硬着头皮去粗略浏览相关扩展源代码。现在写下来当手记。

[![](http://www.iirr.info/blog/wp-content/uploads/2011/03/HIGH_CPU_IN_X.jpg)](http://www.iirr.info/blog/wp-content/uploads/2011/03/HIGH_CPU_IN_X.jpg)



### **（1）php缓存扩展共有的特性**

得益于php的弱语言特性和统一的变量存储结构-zval[2]，绝大多数php缓存扩展并不要求你存储的内容一定要是什么类型，它会自行进行处理。

```php
cache_set('key', 'string_value', 500);  //字符串、数字肯定OK
cache_set('key', array(), 500);  //数组当然OK
cache_set('key', $anObject, 500);   //对象也OK
```

所以，问题的关键，也许就是php缓存扩展在读取缓存或者存储缓存的时候，如何处理这些不同的类型数据，以及会带来什么样的性能问题。

顺着这条思路，一开始，以为只要浏览缓存读取的相关代码就知道是怎么一回事了，没想到绕了大半圈子之后才发现，从缓存存储相关代码读取，才是正道。555…



### （2）php-memcache[1]源代码浏览简析

php-memcache的代码其实尚算简单，其中存储部分重点落在函数php_mmc_store和mmc_pool_store中。

摘要如下（memcache-2.2.6，memcache.c）：
static void php_mmc_store(INTERNAL_FUNCTION_PARAMETERS, char *command, int command_len) /* {{{ */
{

	//前面代码略
	//读取参数，value即为我们要存储的缓存内容
	if (mmc_object == NULL) {
		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Osz|ll", &mmc_object, memcache_class_entry_ptr, &key, &key_len, &value, &flags, &expire) == FAILURE) {
			return;
		}
	}
	else {
		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz|ll", &key, &key_len, &value, &flags, &expire) == FAILURE) {
			return;
		}
	}

	//判断memcache状态，代码略
	//重点，判断value类型，采取不同的策略
	switch (Z_TYPE_P(value)) {
		case IS_STRING:
			result = mmc_pool_store(
				pool, command, command_len, key_tmp, key_tmp_len, flags, expire,
				Z_STRVAL_P(value), Z_STRLEN_P(value) TSRMLS_CC);	//字符串，直接调用mmc_pool_store存储
			break;

		case IS_LONG:
		case IS_DOUBLE:
		case IS_BOOL: {
			zval value_copy;

			/* FIXME: we should be using 'Z' instead of this, but unfortunately it's PHP5-only */
			value_copy = *value;
			zval_copy_ctor(&value_copy);
			convert_to_string(&value_copy);	//数字和布尔值，需要转换为字符串再存储

			result = mmc_pool_store(
				pool, command, command_len, key_tmp, key_tmp_len, flags, expire,
				Z_STRVAL(value_copy), Z_STRLEN(value_copy) TSRMLS_CC);	//调用mmc_pool_store存储

			zval_dtor(&value_copy);
			break;
		}

		default: {
			zval value_copy, *value_copy_ptr;

			/* FIXME: we should be using 'Z' instead of this, but unfortunately it's PHP5-only */
			value_copy = *value;
			zval_copy_ctor(&value_copy);
			value_copy_ptr = &value_copy;

			//重点：数组、对象和其它类型的，一律序列化成字符串再存储
			PHP_VAR_SERIALIZE_INIT(value_hash);
			php_var_serialize(&buf, &value_copy_ptr, &value_hash TSRMLS_CC);
			PHP_VAR_SERIALIZE_DESTROY(value_hash);

			if (!buf.c) {
				/* something went really wrong */
				zval_dtor(&value_copy);
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "Failed to serialize value");
				RETURN_FALSE;
			}

			flags |= MMC_SERIALIZED;	//重点：按位或运算flag，表明是序列化后的内容。MMC_SERIALIZED为1
			zval_dtor(&value_copy);

			result = mmc_pool_store(
				pool, command, command_len, key_tmp, key_tmp_len, flags, expire,
				buf.c, buf.len TSRMLS_CC);	//序列化后的字符串，调用mmc_pool_store存储
		}
	}

	if (flags & MMC_SERIALIZED) {
		smart_str_free(&buf);
	}

	if (result > 0) {
		RETURN_TRUE;
	}

	RETURN_FALSE;
}
/* }}} */

int mmc_pool_store(mmc_pool_t *pool, const char *command, int command_len, const char *key, int key_len, int flags, int expire, const char *value, int value_len TSRMLS_DC) /* {{{ */
{

	//前面代码略
	/* autocompress large values *///此处的有关内容，请参考Memcache::setCompressThreshold[4]
	if (pool->compress_threshold && value_len >= pool->compress_threshold) {
		flags |= MMC_COMPRESSED;
	}

	//检测flag是否指定要进行压缩存储，是则压缩。MMC_COMPRESSED为2（即二进制10）
	if (flags & MMC_COMPRESSED) {
		unsigned long data_len;

		if (!mmc_compress(&data, &data_len, value, value_len TSRMLS_CC)) {
			/* mmc_server_seterror(mmc, "Failed to compress data", 0); */
			return -1;
		}

		//检测是否达到压缩比，否就丢弃此次压缩。默认压缩比为0.2，即如果10K不能压缩到8K以下就丢弃
		/* was enough space saved to motivate uncompress processing on get */
		if (data_len < value_len * (1 - pool->min_compress_savings)) {
			value = data;
			value_len = data_len;
		}
		else {
			flags &= ~MMC_COMPRESSED;
			efree(data);
			data = NULL;
		}
	}

	//下面有关发送命令到memcache的代码略
}
/* }}} */
从上面可以看到，php-memcache对数组和对象等，采取了php默认的serialize方法变成字符串；发送到memcached服务器前，再进行压缩（如果指定了压缩或者指定了压缩比的话）。其中flags值至关重要，保存着是否为序列化数据和压缩数据的双重任务。换句话讲，序列化和压缩都是在web服务器运行的。

那么反推，解压和反序列化也是在web服务器运行的：php-memcache从memcached服务器中获取时，会先根据flags值判断是否进行解压，有的话就解压缩（话说回来，Memcache::get中传递的flags似乎在源代码中没什么作用，奇怪了）；然后再根据flags值判断是否需要反序列化，有则反序列化。相关函数如下，篇幅关系不贴源代码了：
//类比php_mmc_store，Memcache::get的主要实现代码，缓存读取和返回调度代码
int mmc_exec_retrieval_cmd(mmc_pool_t *pool, const char *key, int key_len, zval **return_value, zval *return_flags TSRMLS_DC)
//类比mmc_pool_store，从服务器中读取缓存内容并解压缩
static int mmc_read_value(mmc_t *mmc, char **key, int *key_len, char **value, int *value_len, int *flags TSRMLS_DC)
//类比php_mmc_store中的数组和对象等资源序列化片段，这段代码是反序列化
static int mmc_postprocess_value(zval **return_value, char *value, int value_len TSRMLS_DC)


### （3）验证：serialize是否导致php-memcache CPU占用异常

众所周知，serialize占用的资源是挺大的[5]，而项目中的缓存基本就是数组。它会不会就是造成php-memcache CPU占用异常的原因呢？

为此进行进行测试，分压缩和不压缩、序列化和不序列化4种可能性进行缓存读取相交测试（代码和下载请看最后）。结果如下：
**A）php-memcache存储数组后进行5000次读取（相当于unserialize 5000次）+ MEMCACHE_COMPRESSED**

PROCESS COUNT:5000. PORCESS ALL TIME:10.8274896145s. EACH TIME:0.0021654979229

PROCESS COUNT:5000. PORCESS ALL TIME:10.6685016155s. EACH TIME:0.0021337003231

[![](http://www.iirr.info/blog/wp-content/uploads/2011/03/0_MC_COMPRESS_ARRAY.jpg)](http://www.iirr.info/blog/wp-content/uploads/2011/03/0_MC_COMPRESS_ARRAY.jpg)



**B）php-memcache存储数组后进行5000次读取（相当于unserialize 5000次）+ 非COMPRESSED**

PROCESS COUNT:5000. PORCESS ALL TIME:9.95206928253s. EACH TIME:0.00199041385651

PROCESS COUNT:5000. PORCESS ALL TIME:8.75112223625s. EACH TIME:0.00175022444725

[![](http://www.iirr.info/blog/wp-content/uploads/2011/03/1_MC_NO_COMPRESS_ARRAY.jpg)](http://www.iirr.info/blog/wp-content/uploads/2011/03/1_MC_NO_COMPRESS_ARRAY.jpg)



**C）php-memcache存储字符串（var_export(数组, true)而来）后进行5000次读取（没有unserialize）+ MEMCACHE_COMPRESSED**

PROCESS COUNT:5000. PORCESS ALL TIME:5.49585962296s. EACH TIME:0.00109917192459

PROCESS COUNT:5000. PORCESS ALL TIME:4.89061260223s. EACH TIME:0.000978122520447

[![](http://www.iirr.info/blog/wp-content/uploads/2011/03/2_MC_COMPRESS_STRING.jpg)](http://www.iirr.info/blog/wp-content/uploads/2011/03/2_MC_COMPRESS_STRING.jpg)



**D）php-memcache存储字符串（var_export(数组, true)而来）后进行5000次读取（没有unserialize）+ 非COMPRESSED**

PROCESS COUNT:5000. PORCESS ALL TIME:3.33534455299s. EACH TIME:0.000667068910599

PROCESS COUNT:5000. PORCESS ALL TIME:5.05854201317s. EACH TIME:0.00101170840263

[![](http://www.iirr.info/blog/wp-content/uploads/2011/03/3_MC_NO_COMPRESS_STRING.jpg)](http://www.iirr.info/blog/wp-content/uploads/2011/03/3_MC_NO_COMPRESS_STRING.jpg)



**从上面数据和图可以看到几个现象：**

a）serialize确实引发了php-memcache占用资源异常的问题：对比B）和D）结果，无论从运行时间还是CPU占用都显著增加不少

b）compress也会导致php-memcache占用资源异常：对比C）和D）结果，主要在于显著的CPU占用率升高

c）不启用compress，对I/O和memcached服务端的要求比较高：对比C）和D）结果，php的I/O成倍升高，memcached服务端的CPU占用也较高
**从上述现象可以总结：**

a）导致php-memcache CPU占用资源异常有两个影响因素：数组序列化/反序列化，压缩。其中序列化/反序列化引发的CPU占用率问题相对较高。

b）如何才能正确使用缓存？理想的情况下，不序列化不压缩，缓存存储和读取对资源的消耗似乎是最低的，但会引发I/O、流量以及memcached服务端资源消耗增大的问题；那么相对平衡的做法，在开启压缩的情况下尽量存储字符串内容，也应该可以显著的降低web服务器系统响应时间。也就是说无论如何，“存储渲染好的片段html而非渲染前的原始数组”，之前记得有前辈分享过，只是一时忘了出处了。

### （4）其它缓存系统的简略浏览分析

其它立足于本地缓存的php扩展（APC、XCache、WinCache等），除了wincache对对象的处理也是进行了序列化/反序列化的方式外，其它对数组的的处理方式，基本没有看到序列化/反序列化的身影。限于水平所限，无法完全弄懂，只觉得好像是对数组变量本身的zval体进行分块拆分/拼装处理。
但是这样的处理方式，在面对大数组似乎也是无能为力。以下这是APC的测试结果：

APC循环5000次读取数组：

PROCESS COUNT:5000. PORCESS ALL TIME:5.98700547218s. EACH TIME:0.00119740109444

[![](http://www.iirr.info/blog/wp-content/uploads/2011/03/APC_ARRAY.jpg)](http://www.iirr.info/blog/wp-content/uploads/2011/03/APC_ARRAY.jpg)

APC循环5000次读取字符串（var_export(数组, true)而来）：

PROCESS COUNT:5000. PORCESS ALL TIME:0.0747337341309s. EACH TIME:1.49467468262E-5

[![](http://www.iirr.info/blog/wp-content/uploads/2011/03/APC_STRING.jpg)](http://www.iirr.info/blog/wp-content/uploads/2011/03/APC_STRING.jpg)



这个结果……唉,不多说了……

=====================================================

限于水平所限，本文肯定错误多多，希望有大牛指点一下APC、XCache、WinCache对数组究竟是如何处理的。

最后说一句，wincache的代码中发现许多goto，让人犯晕。不得不吐槽一句：分多几个函数，不会增加你工作量啊……

[![](http://www.iirr.info/blog/wp-content/uploads/2011/03/goto_in_wincache.jpg)](http://www.iirr.info/blog/wp-content/uploads/2011/03/goto_in_wincache.jpg)



### 代码下载：

[cache_array_string_test_code](http://www.iirr.info/blog/wp-content/uploads/2011/03/cache_array_string_test_code.zip)

### 备注：

[1]php-memcache代码：[http://pecl.php.net/package/memcache](http://pecl.php.net/package/memcache)

[2]laruence. 深入理解PHP原理之变量(Variables inside PHP)：[http://www.laruence.com/2008/08/22/412.html](http://www.laruence.com/2008/08/22/412.html)

[3]laruence. php的hash算法：[http://www.laruence.com/2009/07/23/994.html](http://www.laruence.com/2009/07/23/994.html)

[4]Volcano. 启用memcached压缩注意事项：[http://www.ooso.net/archives/475](http://www.ooso.net/archives/475)

[5]Volcano. 关于“facebook的memcached实战”小记：[http://www.ooso.net/archives/558](http://www.ooso.net/archives/558)





