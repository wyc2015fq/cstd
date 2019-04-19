# mysql+memcached【续】 - 三少GG - CSDN博客
2013年07月28日 14:59:54[三少GG](https://me.csdn.net/scut1135)阅读数：1823
个人分类：[Web网站设计](https://blog.csdn.net/scut1135/article/category/762263)
#### 15.6.3.3. Using `libmemcached` with C and C++
The `libmemcached` library provides both
 C and C++ interfaces to memcached and
 is also the basis for a number of different additional API implementations, including Perl, Python and Ruby. Understanding the core`libmemcached` functions
 can help when using these other interfaces.
The C library is the most comprehensive interface library for memcached and
 provides functions and operational systems not always exposed in interfaces not based on the `libmemcached` library.
The different functions can be divided up according to their basic operation. In addition to functions that interface to the core API, a number of utility functions provide extended functionality, such as appending and prepending data.
To build and install `libmemcached`, download
 the `libmemcached` package, run configure,
 and then build and install:
shell> tar xjf libmemcached-0.21.tar.gz
shell> cd libmemcached-0.21
shell> ./configure
shell> make
shell> make install
On many Linux operating systems, you can install the corresponding `libmemcached` package
 through the usualyum, apt-get,
 or similar commands.
To build an application that uses the library, first set the list of servers. Either directly manipulate the servers configured within the main `memcached_st` structure,
 or separately populate a list of servers, and then add this list to the `memcached_st` structure.
 The latter method is used in the following example. Once the server list has been set, you can call the functions to store or retrieve data. A simple application for setting a preset value to`localhost` is
 provided here:
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libmemcached/memcached.h>
int main(int argc, char *argv[])
{
  memcached_server_st *servers = NULL;
  memcached_st *memc;
  memcached_return rc;
  char *key= "keystring";
  char *value= "keyvalue";
  memcached_server_st *memcached_servers_parse (char *server_strings);
  memc= memcached_create(NULL);
  servers= memcached_server_list_append(servers, "localhost", 11211, &rc);
  rc= memcached_server_push(memc, servers);
  if (rc == MEMCACHED_SUCCESS)
    fprintf(stderr,"Added server successfully\n");
  else
    fprintf(stderr,"Couldn't add server: %s\n",memcached_strerror(memc, rc));
  rc= memcached_set(memc, key, strlen(key), value, strlen(value), (time_t)0, (uint32_t)0);
  if (rc == MEMCACHED_SUCCESS)
    fprintf(stderr,"Key stored successfully\n");
  else
    fprintf(stderr,"Couldn't store key: %s\n",memcached_strerror(memc, rc));
  return 0;
}
To test the success of an operation, use the return value, or populated result code, for a given function. The value is always set to `MEMCACHED_SUCCESS` if
 the operation succeeded. In the event of a failure, use the`memcached_strerror()` function
 to translate the result code into a printable string.
To build the application, specify the `memcached` library:
shell> gcc -o memc_basic memc_basic.c -lmemcached
Running the above sample application, after starting a memcached server,
 should return a success message:
shell> memc_basic
Added server successfully
Key stored successfully
##### 15.6.3.3.1. `libmemcached` Base Functions
The base `libmemcached` functions let you
 create, destroy and clone the main `memcached_st` structure
 that is used to interface with the `memcached` servers.
 The main functions are defined below:
memcached_st *memcached_create (memcached_st *ptr);
Creates a new `memcached_st` structure for
 use with the other `libmemcached` API functions.
 You can supply an existing, static, `memcached_st` structure,
 or `NULL` to have a new structured allocated.
 Returns a pointer to the created structure, or `NULL` on
 failure.
void memcached_free (memcached_st *ptr);
Frees the structure and memory allocated to a previously created `memcached_st` structure.
memcached_st *memcached_clone(memcached_st *clone, memcached_st *source);
Clones an existing `memcached` structure
 from the specified `source`, copying the
 defaults and list of servers defined in the structure.
##### 15.6.3.3.2. `libmemcached` Server Functions
The `libmemcached` API uses a list of servers,
 stored within the `memcached_server_st` structure,
 to act as the list of servers used by the rest of the functions. To use `memcached`,
 you first create the server list, and then apply the list of servers to a valid `libmemcached` object.
Because the list of servers, and the list of servers within an active `libmemcached` object
 can be manipulated separately, you can update and manage server lists while an active `libmemcached` interface
 is running.
The functions for manipulating the list of servers within a `memcached_st` structure
 are:
memcached_return
   memcached_server_add (memcached_st *ptr,
                         char *hostname,
                         unsigned int port);
Adds a server, using the given `hostname` and `port` into
 the `memcached_st` structure given in `ptr`.
memcached_return
   memcached_server_add_unix_socket (memcached_st *ptr,
                                     char *socket);
Adds a Unix socket to the list of servers configured in the `memcached_st` structure.
unsigned int memcached_server_count (memcached_st *ptr);
Returns a count of the number of configured servers within the `memcached_st` structure.
memcached_server_st *
   memcached_server_list (memcached_st *ptr);
Returns an array of all the defined hosts within a `memcached_st` structure.
memcached_return
   memcached_server_push (memcached_st *ptr,
                          memcached_server_st *list);
Pushes an existing list of servers onto list of servers configured for a current `memcached_st` structure.
 This adds servers to the end of the existing list, and duplicates are not checked.
The `memcached_server_st` structure can be
 used to create a list of `memcached` servers
 which can then be applied individually to `memcached_st` structures.
memcached_server_st *
   memcached_server_list_append (memcached_server_st *ptr,
                                 char *hostname,
                                 unsigned int port,
                                 memcached_return *error);
Adds a server, with `hostname` and `port`,
 to the server list in `ptr`. The result
 code is handled by the `error`argument,
 which should point to an existing `memcached_return` variable.
 The function returns a pointer to the returned list.
unsigned int memcached_server_list_count (memcached_server_st *ptr);
Returns the number of the servers in the server list.
void memcached_server_list_free (memcached_server_st *ptr);
Frees the memory associated with a server list.
memcached_server_st *memcached_servers_parse (char *server_strings);
Parses a string containing a list of servers, where individual servers are separated by a comma, space, or both, and where individual servers are of the form ``server`[:`port`]`.
 The return value is a server list structure.
##### 15.6.3.3.3. `libmemcached` Set Functions
The set-related functions within `libmemcached` provide
 the same functionality as the core functions supported by the `memcached` protocol.
 The full definition for the different functions is the same for all the base functions (`add`,`replace`, `prepend`, `append`).
 For example, the function definition for `memcached_set()` is:
memcached_return
   memcached_set (memcached_st *ptr,
                  const char *key,
                  size_t key_length,
                  const char *value,
                  size_t value_length,
                  time_t expiration,
                  uint32_t flags);
The `ptr` is the `memcached_st` structure.
 The `key` and `key_length` define
 the key name and length, and `value`and `value_length` the
 corresponding value and length. You can also set the expiration and optional flags. For more information, see [Section 15.6.3.3.5,
 “Controlling `libmemcached` Behaviors”](http://dev.mysql.com/doc/refman/5.0/en/ha-memcached-interfaces-libmemcached.html#ha-memcached-interfaces-libmemcached-behaviors).
This table outlines the remainder of the set-related `libmemcached` functions
 and the equivalent core functions supported by the memcached protocol.
|`libmemcached` Function|Equivalent Core Function|
|----|----|
|```memcached_set(memc, key, key_length, value, value_length, expiration, flags)```|Generic `set()` operation.|
|```memcached_add(memc, key, key_length, value, value_length, expiration, flags)```|Generic `add()` function.|
|```memcached_replace(memc, key, key_length, value, value_length, expiration, flags)```|Generic `replace()`.|
|```memcached_prepend(memc, key, key_length, value, value_length, expiration, flags)```|Prepends the specified `value` before the current value of the specified `key`.|
|```memcached_append(memc, key, key_length, value, value_length, expiration, flags)```|Appends the specified `value` after the current value of the specified `key`.|
|```memcached_cas(memc, key, key_length, value, value_length, expiration, flags, cas)```|Overwrites the data for a given key as long as the corresponding `cas` value is still the same within the server.|
|```memcached_set_by_key(memc, master_key, master_key_length, key, key_length, value, value_length, expiration, flags)```|Similar to the generic `set()`, but has the option of an additional master key that can be used to identify an individual server.|
|```memcached_add_by_key(memc, master_key, master_key_length, key, key_length, value, value_length, expiration, flags)```|Similar to the generic `add()`, but has the option of an additional master key that can be used to identify an individual server.|
|```memcached_replace_by_key(memc, master_key, master_key_length, key, key_length, value, value_length, expiration, flags)```|Similar to the generic `replace()`, but has the option of an additional master key that can be used to identify an individual server.|
|```memcached_prepend_by_key(memc, master_key, master_key_length, key, key_length, value, value_length, expiration, flags)```|Similar to the `memcached_prepend()`, but has the option of an additional master key that can be used to identify an individual server.|
|```memcached_append_by_key(memc, master_key, master_key_length, key, key_length, value, value_length, expiration, flags)```|Similar to the `memcached_append()`, but has the option of an additional master key that can be used to identify an individual server.|
|```memcached_cas_by_key(memc, master_key, master_key_length, key, key_length, value, value_length, expiration, flags)```|Similar to the `memcached_cas()`, but has the option of an additional master key that can be used to identify an individual server.|
The `by_key` methods add two further arguments
 that define the master key, to be used and applied during the hashing stage for selecting the servers. You can see this in the following definition:
memcached_return
   memcached_set_by_key(memcached_st *ptr,
                        const char *master_key,
                        size_t master_key_length,
                        const char *key,
                        size_t key_length,
                        const char *value,
                        size_t value_length,
                        time_t expiration,
                        uint32_t flags);
All the functions return a value of type `memcached_return`,
 which you can compare against the`MEMCACHED_SUCCESS` constant.
##### 15.6.3.3.4. `libmemcached` Get Functions
The `libmemcached` functions provide both
 direct access to a single item, and a multiple-key request mechanism that provides much faster responses when fetching a large number of keys simultaneously.
The main get-style function, which is equivalent to the generic `get()` is `memcached_get()`.
 This function returns a string pointer, pointing to the value associated with the specified key.
char *memcached_get (memcached_st *ptr,
                     const char *key, size_t key_length,
                     size_t *value_length,
                     uint32_t *flags,
                     memcached_return *error);
A multi-key get, `memcached_mget()`, is also
 available. Using a multiple key get operation is much quicker to do in one block than retrieving the key values with individual calls to `memcached_get()`.
 To start the multi-key get, call`memcached_mget()`:
memcached_return
    memcached_mget (memcached_st *ptr,
                    char **keys, size_t *key_length,
                    unsigned int number_of_keys);
The return value is the success of the operation. The `keys` parameter
 should be an array of strings containing the keys, and `key_length` an
 array containing the length of each corresponding key. `number_of_keys` is
 the number of keys supplied in the array.
To fetch the individual values, use `memcached_fetch()` to
 get each corresponding value.
char *memcached_fetch (memcached_st *ptr,
                       const char *key, size_t *key_length,
                       size_t *value_length,
                       uint32_t *flags,
                       memcached_return *error);
The function returns the key value, with the `key`, `key_length` and `value_length` parameters
 being populated with the corresponding key and length information. The function returns `NULL` when
 there are no more values to be returned. A full example, including the populating of the key data and the return of the information is provided here.
#include <stdio.h>
#include <sstring.h>
#include <unistd.h>
#include <libmemcached/memcached.h>
int main(int argc, char *argv[])
{
  memcached_server_st *servers = NULL;
  memcached_st *memc;
  memcached_return rc;
  char *keys[]= {"huey", "dewey", "louie"};
  size_t key_length[3];
  char *values[]= {"red", "blue", "green"};
  size_t value_length[3];
  unsigned int x;
  uint32_t flags;
  char return_key[MEMCACHED_MAX_KEY];
  size_t return_key_length;
  char *return_value;
  size_t return_value_length;
  memc= memcached_create(NULL);
  servers= memcached_server_list_append(servers, "localhost", 11211, &rc);
  rc= memcached_server_push(memc, servers);
  if (rc == MEMCACHED_SUCCESS)
    fprintf(stderr,"Added server successfully\n");
  else
    fprintf(stderr,"Couldn't add server: %s\n",memcached_strerror(memc, rc));
  for(x= 0; x < 3; x++)
    {
      key_length[x] = strlen(keys[x]);
      value_length[x] = strlen(values[x]);
      rc= memcached_set(memc, keys[x], key_length[x], values[x],
                        value_length[x], (time_t)0, (uint32_t)0);
      if (rc == MEMCACHED_SUCCESS)
        fprintf(stderr,"Key %s stored successfully\n",keys[x]);
      else
        fprintf(stderr,"Couldn't store key: %s\n",memcached_strerror(memc, rc));
    }
  rc= memcached_mget(memc, keys, key_length, 3);
  if (rc == MEMCACHED_SUCCESS)
    {
      while ((return_value= memcached_fetch(memc, return_key, &return_key_length,
                                            &return_value_length, &flags, &rc)) != NULL)
        {
          if (rc == MEMCACHED_SUCCESS)
            {
              fprintf(stderr,"Key %s returned %s\n",return_key, return_value);
            }
        }
    }
  return 0;
}
Running the above application produces the following output:
shell> memc_multi_fetch
Added server successfully
Key huey stored successfully
Key dewey stored successfully
Key louie stored successfully
Key huey returned red
Key dewey returned blue
Key louie returned green
##### 15.6.3.3.5. Controlling `libmemcached` Behaviors
The behavior of `libmemcached` can be modified
 by setting one or more behavior flags. These can either be set globally, or they can be applied during the call to individual functions. Some behaviors also accept an additional setting, such as the hashing mechanism used when selecting servers.
To set global behaviors:
memcached_return
   memcached_behavior_set (memcached_st *ptr,
                           memcached_behavior flag,
                           uint64_t data);
To get the current behavior setting:
uint64_t
   memcached_behavior_get (memcached_st *ptr,
                           memcached_behavior flag);
The following table describes `libmemcached` behavior
 flags.
|Behavior|Description|
|----|----|
|`MEMCACHED_BEHAVIOR_NO_BLOCK`|Caused `libmemcached` to use asynchronous I/O.|
|`MEMCACHED_BEHAVIOR_TCP_NODELAY`|Turns on no-delay for network sockets.|
|`MEMCACHED_BEHAVIOR_HASH`|Without a value, sets the default hashing algorithm for keys to use MD5. Other valid values include`MEMCACHED_HASH_DEFAULT`, `MEMCACHED_HASH_MD5`,`MEMCACHED_HASH_CRC`, `MEMCACHED_HASH_FNV1_64`,`MEMCACHED_HASH_FNV1A_64`,`MEMCACHED_HASH_FNV1_32`, and`MEMCACHED_HASH_FNV1A_32`.|
|`MEMCACHED_BEHAVIOR_DISTRIBUTION`|Changes the method of selecting the server used to store a given value. The default method is`MEMCACHED_DISTRIBUTION_MODULA`. You can enable consistent hashing by setting`MEMCACHED_DISTRIBUTION_CONSISTENT`.`MEMCACHED_DISTRIBUTION_CONSISTENT` is an alias for the value`MEMCACHED_DISTRIBUTION_CONSISTENT_KETAMA`.|
|`MEMCACHED_BEHAVIOR_CACHE_LOOKUPS`|Cache the lookups made to the DNS service. This can improve the performance if you are using names instead of IP addresses for individual hosts.|
|`MEMCACHED_BEHAVIOR_SUPPORT_CAS`|Support CAS operations. By default, this is disabled because it imposes a performance penalty.|
|`MEMCACHED_BEHAVIOR_KETAMA`|Sets the default distribution to`MEMCACHED_DISTRIBUTION_CONSISTENT_KETAMA` and the hash to `MEMCACHED_HASH_MD5`.|
|`MEMCACHED_BEHAVIOR_POLL_TIMEOUT`|Modify the timeout value used by `poll()`. Supply a ```signed int``` pointer for the timeout value.|
|`MEMCACHED_BEHAVIOR_BUFFER_REQUESTS`|Buffers IO requests instead of them being sent. A get operation, or closing the connection causes the data to be flushed.|
|`MEMCACHED_BEHAVIOR_VERIFY_KEY`|Forces `libmemcached` to verify that a specified key is valid.|
|`MEMCACHED_BEHAVIOR_SORT_HOSTS`|If set, hosts added to the list of configured hosts for a`memcached_st` structure are placed into the host list in sorted order. This breaks consistent hashing if that behavior has been enabled.|
|`MEMCACHED_BEHAVIOR_CONNECT_TIMEOUT`|In nonblocking mode this changes the value of the timeout during socket connection.|
##### 15.6.3.3.6. libmemcached Command-Line Utilities
In addition to the main C library interface, `libmemcached` also
 includes a number of command-line utilities that can be useful when working with and debugging memcached applications.
All of the command-line tools accept a number of arguments, the most critical of which is `servers`,
 which specifies the list of servers to connect to when returning information.
The main tools are:
- 
memcat:
 Display the value for each ID given on the command line:
shell> memcat --servers=localhost hwkey
Hello world
- 
memcp:
 Copy the contents of a file into the cache, using the file name as the key:
shell> echo "Hello World" > hwkey
shell> memcp --servers=localhost hwkey
shell> memcat --servers=localhost hwkey
Hello world
- 
memrm:
 Remove an item from the cache:
shell> memcat --servers=localhost hwkey
Hello world
shell> memrm --servers=localhost hwkey
shell> memcat --servers=localhost hwkey
- 
memslap:
 Test the load on one or more memcached servers,
 simulating get/set and multiple client operations. For example, you can simulate the load of 100 clients performing get operations:
shell> memslap --servers=localhost --concurrency=100 --flush --test=get
memslap --servers=localhost --concurrency=100 --flush --test=get	Threads connecting to servers 100
	Took 13.571 seconds to read data
- 
memflush:
 Flush (empty) the contents of the memcached cache.
shell> memflush --servers=localhost

---------------------------------------------------
Grant推荐：
[nosql小组](http://zhan.renren.com/nosqlgroup?from=template)
-----------------------------------------------------
## [memcahe整合mysql](http://zhan.renren.com/nosqlgroup?gid=3602888497994954107&checked=true)
memcache整合mysql
对memcache的操作一般都是放程序里面去操作的，新增，更新，删除什么的。如果能利用mysql来对memcache进行操作，那就更好，代码端就会简单一点。但是利用mysql来操作memcache，比较适合实现简单的方式。下面说一下安装的过程和遇到的问题，在看安装过程的之前，我觉得应当先看一下，我安装时候所遇到的问题，这样你可以避免掉，少走一点弯路。
一，安装所要的软件
mysql5.1以上版本：http://downloads.mysql.com/archives/
libevent下载：wget  http://www.monkey.org/~provos/libevent-1.2.tar.gz
libmemcached下载：http://download.tangent.org/
memcached下载：http://www.danga.com/memcached/
memcached_functions_mysql下载：http://download.tangent.org/
二，mysql的安装
mysql的安装方式有很多，你可以用系统的软件管理包来安装，不同的linux版本，软件管理工具是不一样的。
redhat,centos 有yum，ubuntu 有apt-get ，arch有pacman等。用系统自带的管理工具安装比较简单。如果是自己下载怎么安装的呢，mysql的官方网站提供了三种mysql的安装源码，一种是.rpm的，一种是二进制的，一种是要自己编译的。
1，用rpm来按装
    rpm -i MySQL-server-VERSION.i386.rpm  
    rpm -i MySQL-client-VERSION.i386.rpm  
2，二进制包进行安装
二进制包安装有一个缺点，就是要安装到什么地方，都是死的。安装过程中有问题的，查看一下是不是装了glibc,以及版本是不是太低了。
    groupadd mysql  
    useradd -g mysql mysql  
    tar zxvf /path/to/mysql-VERSION-OS.tar.gz　-C /usr/local  
    cd　/usr/local  
    mv mysql-VERSION-OS ./mysql  
    cd /usr/local/mysql  
    scripts/mysql_install_db        --user=mysql  
    chown  -R mysql:mysql /usr/local/mysql  
    bin/mysqld_safe --user=mysql        &  
３，源码自己编译
    groupadd mysql  
    useradd -g mysql mysql  
    tar zxvf /path/to/mysql-VERSION-OS.tar.gz  
    cd　/mysql-VERSION-OS  
    ./configure --prefix=/usr/local/mysql　　//路径可自定义   
    make && make install  
    cp support-files/my-medium.cnf /etc/my.cnf  
    cd /usr/local/mysql  
    bin/mysql_install_db --user=mysql  
    chown  -R mysql:mysql /usr/local/mysql  
    bin/mysqld_safe --user=mysql &  
安装遇难问题：安装mysql至少要５.1版本以上的，服务器端，还是客户端都要，装完memcached_functions_mysql后，调用libmemcached模块时会报错的。
[root@BlackGhost sql]#  /usr/local/mysql/bin/mysql <install_functions.sql
ERROR 1126 (HY000) at line 1: Can't open shared library 'libmemcached_functions_mysql.so' (errno: 22 /usr/local/mysql/lib/mysql/plugin/libmemcached_functions_mysql.so: undefined symbol: memcached_string_append)
上面是用mysqld_safe来启动mysql的，也可以用mysql.server来启动，它在/usr/local/mysql/share/mysql 下面，你也可以把mysql.server考到开机启动的目录下面，并且重命名为mysqld
[root@BlackGhost mysql]# ./mysql.server stop
Shutting down MySQL.. SUCCESS!
[root@BlackGhost mysql]# ./mysql.server start
Starting MySQL. SUCCESS!
二，关于libevent和memcached的安装
1.deb
Debian 系直接 $  sudo apt-get install memcached   都搞定，不然还需要分别安装libevent与memcached
2.源码安装
wget  http://www.monkey.org/~provos/libevent-1.2.tar.gz   #下载libevent
1 .查看是否已经安装了libevent，一般情况下，装系统的时候libevent就会装进去的。
ls -al /usr/lib | grep libevent
[root@BlackGhost etc]# ls -al /usr/lib | grep libevent
lrwxrwxrwx  1 mysql mysql       21 Mar 25  2009 libevent-1.2.so.1 -> libevent-1.2.so.1.0.3
-rwxr-xr-x  1 mysql mysql   208300 Mar 25  2009 libevent-1.2.so.1.0.3
-rw-r--r--  1 mysql mysql   268992 Mar 25  2009 libevent.a
-rwxr-xr-x  1 mysql mysql      820 Mar 25  2009 libevent.la
lrwxrwxrwx  1 mysql mysql       21 Mar 25  2009 libevent.so -> libevent-1.2.so.1.0.3
如果显示上面内容说明你已安装了，就可以跳过第二步。
2 . 先安装libevent：
第一种方法
tar zxvf libevent-1.2.tar.gz
cd libevent-1.2
./configure --prefix=/usr
make
make install
第二种方法
用系统自命的软件管理工具，archlinux用pacman，centos用yum ，大便用apt-get等
[root@BlackGhost etc]# pacman -Ss libevent
core/libevent 1.4.11-1
An event notification library
安装完后在进行第一步，进行测试
3 . 安装memcached服务器端：
tar zxvf memcached-1.2.0.tar.gz
cd memcached-1.2.0
./configure  --with-libevent=/usr    --prefix=/usr/local/memcahced
make
make install
如果有错，请确认一下，local下面有memcahced这个文件夹，以及libevent的安装路径是否正确
4 .启动memcached服务
/usr/local/bin/memcached -d -m 20 -u root -p 11211 -P ./memcached.pid
-d #作为守护进程运行
-m#分配20M的内存
-u ＃用户是zhangy
-p ＃监听端口是12000
－P＃进程PID存放的位置
[zhangy@BlackGhost ~]$ ps -e|grep memcache
17904 ?        00:00:00 memcached
到此memcached的服务器端已经安装。
三，libmemcached的安装
wget  http://download.tangent.org/libmemcached-0.34.tar.gz
tar xvzf  libmemcached-0.34.tar.gz 
cd libmemcached-0.34
./configure --prefix=/usr/local/libmemcached34 --with-memcached=/usr/local/memcached/bin/memcached
sudo make
sudo make install
sudo chmod 777 /etc/ld.so.conf
echo "/usr/local/libmemcached34" >> /etc/ld.so.conf
ldconfig
注意路径不要错了。install_functions.sql定义了一些memcache的操作函数：如下到这儿安装基本上结束，下面我们来测试一下
1.当时我下载的是libmemcached-0.42.tar.gz，安装memcached_functions_mysql过程中遇到这样一个问题
servers.c:263:28: error: 'memcached_st' has no member named 'hosts'
servers.c:264:28: error: 'memcached_st' has no member named 'hosts'
后来我在网上查一下，libmemcached-0.34没有这个问题
2. libmemcached-0.37会 遇到  ERROR 1126 (HY000) at line 38: Can't open shared library 'libmemcached_functions_mysql.so' (errno: 0 /usr/local/mysql/lib/plugin/libmemcached_functions_mysql.so: undefined symbol: memcached_string_append)
所以推荐安装 ibmemcached-0.34。
3.在安装libmemcached的时候，如果不是默认安装，需要指定memcached安装路径。
四，安装memcached_functions_mysql
    wget http://download.tangent.org/memcached_functions_mysql-0.9.tar.gz
    tar xzf memcached_functions_mysql-0.9.tar.gz  
    cd memcached_functions_mysql-0.9  
    ./configure --prefix=/usr/local/memcache_mysql --with-mysql=/data/mysql/bin/mysql_config --with-libmemcached=/usr/local/libmemcached34  
    make && make install  
    cp /usr/local/memcache_mysql/lib/libmemcached_functions*   /data/lib/mysql/plugin
别忘了加上--with-libmemcached=/usr/local/libmemcached34 不然会报以下错误
checking for mysql_config... /usr/bin/mysql_config
checking for libmemcached >= 0.17... configure: error: libmemcached not found
如果是mysql 5.5 mysql插件的目录是/usr/local/mysql/lib/plugin
创建mysql的memcache操作函数
mysql -uroot -proot < sql/install_functions.sql
mysql如果不是默认安装，需要指定路径。
也可以在mysql管理里面运行install_functions.sql的sql脚本。
mysql>  source sql/install_functions.sql
这样我们就可以使用mysql memcached UDF 了，我们可以通过下面语句查看是否已经正常安装
    mysql>  select * from mysql.func;
    +------------------------------+-----+---------------------------------+----------+
    | name | ret | dl | type |
    +------------------------------+-----+---------------------------------+----------+
    | memc_add | 2 | libmemcached_functions_mysql.so | function |
    | memc_add_by_key | 2 | libmemcached_functions_mysql.so | function |
    | memc_servers_set | 2 | libmemcached_functions_mysql.so | function |
    32 rows in set (0.00 sec)
    添加trigger ，就是向memcache内insert，update，deletel等，参照：
    1）memcached_functions_mysql-0.9/sql 目录下的trigger_fun.sql
    2）或者文档：http://dev.mysql.com/doc/refman/5.1/en/ha-memcached-interfaces-mysqludf.html
    如果懒得看就看我后边的例子
五，测试
例子1：
mysql> select memc_servers_set('127.0.0.1:11211')
注意：如果mysql restart，需要重新运行这句以建立与memcached之间的关系
mysql>  select memc_servers_behavior_set('MEMCACHED_BEHAVIOR_NO_BLOCK','1');
+--------------------------------------------------------------+
| memc_servers_behavior_set('MEMCACHED_BEHAVIOR_NO_BLOCK','1') |
+--------------------------------------------------------------+
| 0 |
+--------------------------------------------------------------+
1 row in set (0.00 sec)
mysql>  select memc_servers_behavior_set('MEMCACHED_BEHAVIOR_TCP_NODELAY','1');
+-----------------------------------------------------------------+
| memc_servers_behavior_set('MEMCACHED_BEHAVIOR_TCP_NODELAY','1') |
+-----------------------------------------------------------------+
| 0 |
+-----------------------------------------------------------------+
1 row in set (0.00 sec)
注意：设置MEMCACHED_BEHAVIOR_NO_BLOCK为打开状态，这样在memcached出现问题时（不能连接时）
数据继续插入到mysql中，报错提示，如果不设置此值，如果memcached失败，mysql需要等到timeout
才可以插入到表中。
mysql> use test;
Database changed
mysql> create table xxd (id int, value varchar(100));    
mysql> create trigger xxdmmci after insert on xxd for each row set @tmp = memc_set(NEW.id, NEW.value);
mysql> create trigger xxdmmcu after update on xxd for each row set @tmp = memc_set(NEW.id, NEW.value);
mysql> create trigger xxdmmcd before delete on xxd for each row set @tmp = memc_delete(OLD.id);
mysql> insert into xxd values(1, 'xxd'),(2,'xxd79'),(3, 'buro79xxd');         
Query OK, 3 rows affected (0.00 sec)
Records: 3  Duplicates: 0  Warnings: 0
mysql> select memc_get('2');
+----------------------------+
| memc_get('2')              |
+----------------------------+
| xxd79 |
+----------------------------+
1 row in set (0.00 sec)
mysql> update xxd set value='xxd_new' where id=1;
Query OK, 1 row affected (0.00 sec)
Rows matched: 1  Changed: 1  Warnings: 0
mysql> select memc_get('1');
+----------------------------+
| memc_get('1')              |
+----------------------------+
| xxd_new |
+----------------------------+
1 row in set (0.00 sec)
mysql> delete from xxd where id=1;
Query OK, 1 row affected (0.00 sec)
mysql> select memc_get('1');
+----------------------------+
| memc_get('1')              |
+----------------------------+
| NULL |
+----------------------------+
1 row in set (0.00 sec)
mysql> exit
$  telnet 127.0.0.1 11211
Trying 127.0.0.1...
Connected to 127.0.0.1.
Escape character is '^]'.
get 1
END
get 2
VALUE 2 0 5
xxd79
END
get 3
VALUE 3 0 9
buro79xxd
END
quit
Connection closed by foreign host.
例子2：
    //创建一个测试有   
    drop table if  exists urls;  
    create table urls (  
     id int(3) not null,  
     url varchar(64) not null default   '' ,  
     primary key (id)  
     );  
    //连接memcched，根启动memcahed的端口要一样   
    select memc_servers_set('127.0.0.1:12000,127.0.0.1:13000' );  
    //设置一个开始序列   
    select memc_set('urls:sequence' , 0);  
    //创建插入memcached触发器   
    DELIMITER |  
    DROP TRIGGER IF EXISTS url_mem_insert |  
    CREATE TRIGGER url_mem_insert  
    BEFORE INSERT ON urls  
    FOR EACH ROW BEGIN  
     SET NEW.id= memc_increment('urls:sequence' );  
     SET @mm= memc_set(concat('urls:' ,NEW.id), NEW.url);  
    END  |  
    //创建更新memcached触发器   
    DROP TRIGGER IF EXISTS url_mem_update |  
    CREATE TRIGGER url_mem_update  
    BEFORE UPDATE ON urls  
    FOR EACH ROW BEGIN  
     SET @mm= memc_replace(concat('urls:' ,OLD.id), NEW.url);  
    END  |  
    //创建删除memcached触发器   
    DROP TRIGGER IF EXISTS url_mem_delete |  
    CREATE TRIGGER url_mem_delete  
    BEFORE DELETE  ON urls  
    FOR EACH ROW BEGIN  
     SET @mm= memc_delete(concat('urls:' ,OLD.id));  
    END  |  
    DELIMITER ;  //写触发器的时候，我们会用；mysql执行分割符也是；所以我们在写触发器或者是存储过程的时候都会改变一下，例如：DELIMITER |   
    //插入一些测试数据   
    insert into urls (url) values ('http://google.com' );  
    insert into urls (url) values ('http://baidu.com/' );  
    insert into urls (url) values ('http://www.51yip.com/' );  
    insert into urls (url) values ('http://blog.51yip.com/' );  
    insert into urls (url) values ('http://51yip.com' );  
    insert into urls (url) values ('http://mysql.com' );  
    select * from urls;  
    //将插入的6条数据显示出来,下面的显示和删除也是一样的不多说了。   
    select memc_get('urls:1' );  
    select memc_get('urls:2' );  
    select memc_get('urls:3' );  
    select memc_get('urls:4' );  
    select memc_get('urls:5' );  
    select memc_get('urls:6' );  
    update urls set url= 'http://mysql.com/sun'  where url =  'http://51yip.com' ;  
    select url from urls where url = 'http://51yip.com/manual' ;  
    select memc_get('urls:6' );  
    delete  from urls where url =  'http://blog.51yip.com/' ;  
    select * from urls where url='http://blog.51yip.com/' ;  
    select memc_get('urls:4' );  
我以前写过一篇：关于memcache的key的管理，徘徊中 里面提到对单表进行缓存 ，可以完全配合这篇文章所说的东西。如果有多表联合查询的话，在用触器就比较麻烦了，还不如放到程序里去执行emcached37  

# Memcached缓存瓶颈分析
Posted on [2013 年 5 月 25 日](http://www.zrwm.com/?p=7335)by [Jose](http://www.zrwm.com/?author=1)
#### Memcached缓存瓶颈分析
- 
##### 获取Memcached的统计信息
Shell:
# echo "stats" | nc 127.0.0.1 11211
PHP:
$mc = new Memcached();
$mc->addServer('127.0.0.1',11211);
$stats = $mc->getStats();
- 
##### Memcached缓存瓶颈分析的一些指标
![memcached_bandwidth](http://www.zrwm.com/wp-content/uploads/2013/05/memcached_bandwidth.png)
![memcache_user_time](http://www.zrwm.com/wp-content/uploads/2013/05/memcache_user_time.png)
![memcached_system_time](http://www.zrwm.com/wp-content/uploads/2013/05/memcached_system_time.png)
![memcache_cache_miss](http://www.zrwm.com/wp-content/uploads/2013/05/memcache_cache_miss.png)
Posted in [Memcached](http://www.zrwm.com/?cat=54), [Performance
 analysis & tuning](http://www.zrwm.com/?cat=25).
# mysql memcached UDF安装使用
在Centos5.6下通过验证！
官方网站：[http://dev.mysql.com/doc/refman/5.1/en/ha-memcached-interfaces-mysqludf.html](http://dev.mysql.com/doc/refman/5.1/en/ha-memcached-interfaces-mysqludf.html)
很早之前，就看到了通过mysql UDF 更新memcached ，原来也研究过一段时间，只是没有来得及写个文档，导致后来工作中，经常要google，搜索其安装，使用的方法，刹时麻烦，今天总结一下：
1：mysql memcached UD介绍
mysql memcached UDF 其实就是通过libmemcached来使用memcache的一系列函数，通过这些函数，你能 对memcache进行get, set, cas, append, prepend, delete, increment, decrement objects操作，如果我们通过mysql trigger来使用这些函数，那么就能通过mysql更好的，更自动的管理memcache!下载地址：[https://launchpad.net/memcached-udfs/+download](https://launchpad.net/memcached-udfs/+download)
2:安装方法：
1）安装memcache和memcached
参考：[http://blog.haohtml.com/archives/395](http://blog.haohtml.com/archives/395)
2）安装libmemcached（[https://launchpad.net/libmemcached/+download](https://launchpad.net/libmemcached/+download)）
> 
$ wget http://download.tangent.org/libmemcached-0.31.tar.gz
$ tar -xzvf libmemcached-0.31.tar.gz
$ cd libmemcached-0.31
$ ./configure
$ make
安装的时候，发现新版本的都提示错误的
> 
wget http://launchpad.net/libmemcached/1.0/0.43/+download/libmemcached-0.43.tar.gz
tar zxvf libmemcached-0.43.tar.gz
cd libmemcached-0.43
./configure --with-memcached=/usr/local/bin/memcached
make && make install
echo "/usr/local/lib" >> /etc/ld.so.conf
ldconfig
安装完成后，libmemcached 的文件包括：
/usr/local/bin/ 目录下
memcat memcp memdump memerror memflush memrm memslap memstat
都是可执行文件，是一些命令行工具，具体使用，可参考官方文档，或帮助。
/usr/local/include/libmemcached 目录下是该函数库的一些头文件
/usr/local/lib 目录下
libmemcached* 等文件，都是库文件。
/usr/local/share/man1 目录下，有 memcat 等命令行工具的 man 帮助文件。
/usr/local/share/man3 目录下，是函数库的一些帮助文件。
命令行工具中，memstat 可在命令行查看 memcached 服务器的情况，比如：
$ memcat --servers=127.0.0.1:11211
输出的为 memcached 服务器的一些统计数据等。
3）安装memcached_functions_mysql
To install the MySQL memcached UDFs, download the UDF package from [http://libmemcached.org/](http://libmemcached.org/).
 Unpack the package and run configureto configure the build process. When running configure,
 use the `--with-mysql`option and specify the location of
 the [mysql_config](http://dev.mysql.com/doc/refman/5.1/en/mysql-config.html) command.安装教程请参考：[http://dev.mysql.com/doc/refman/5.1/en/ha-memcached-interfaces-mysqludf.html](http://dev.mysql.com/doc/refman/5.1/en/ha-memcached-interfaces-mysqludf.html)
> 
tar zxvf memcached_functions_mysql-0.9.tar.gz
cd memcached_functions_mysql-0.9
./configure --with-mysql=/usr/local/mysql51/bin/mysql_config
make && make install
4）拷贝lib文件到mysql的plugin下面
> 
shell> cp -R /usr/local/lib/libmemcached_functions_mysql.* /usr/local/mysql51/lib/mysql/plugin/
5）添加memcache UDF 函数
在mysql里执行(要sql目录里)
> 
source install_functions.sql
这样我们就可以使用mysql memcached UDF 了，我们可以通过下面语句查看是否已经正常安装
1)查看mysql.func,有很多函数
> 
mysql> select * from mysql.func;
+------------------------------+-----+---------------------------------+----------+
| name                         | ret | dl                              | type     |
+------------------------------+-----+---------------------------------+----------+
| memc_add                     |   2 | libmemcached_functions_mysql.so | function |
| memc_add_by_key              |   2 | libmemcached_functions_mysql.so | function |
| memc_servers_set             |   2 | libmemcached_functions_mysql.so | function |
2）添加trigger，看是否向memcache里insert、update等
对于验证方法请参考：[http://dev.mysql.com/doc/refman/5.1/en/ha-memcached-interfaces-mysqludf.html](http://dev.mysql.com/doc/refman/5.1/en/ha-memcached-interfaces-mysqludf.html)
> 
mysql> select memc_servers_set('127.0.0.1');
<8 new client connection
<8 version
>8 VERSION 1.2.0
+-------------------------------+
| memc_servers_set('127.0.0.1') |
+-------------------------------+
| 0 |
+-------------------------------+
1 row in set (0.03 sec)
mysql> select memc_servers_set('127.0.0.1');
<8 version
>8 VERSION 1.2.0
<9 new client connection
<9 version
>9 VERSION 1.2.0
+-------------------------------+
| memc_servers_set('127.0.0.1') |
+-------------------------------+
| 0 |
+-------------------------------+
1 row in set (0.00 sec)
mysql> select memc_servers_set('127.0.0.1');
<8 version
>8 VERSION 1.2.0
<9 version
>9 VERSION 1.2.0
<10 new client connection
<10 version
>10 VERSION 1.2.0
+-------------------------------+
| memc_servers_set('127.0.0.1') |
+-------------------------------+
| 0 |
+-------------------------------+
1 row in set (0.00 sec)
mysql> select memc_set('myid','atest');
<11 new client connection
<11 set myid 0 0 5
>11 STORED
<11 quit
<11 connection closed.
+--------------------------+
| memc_set('myid','atest') |
+--------------------------+
| 1 |
+--------------------------+
1 row in set (0.00 sec)
mysql> select memc_get('myid');
<11 new client connection
<11 get myid
>11 sending key myid
>11 END
<11 quit
<11 connection closed.
+------------------+
| memc_get('myid') |
+------------------+
| atest |
+------------------+
1 row in set (0.01 sec)
mysql>
总结：由下面的信息可能看出，每次调用udf的时候，都会连接和关闭memcached．这里还是比较的消费资源的．后面还有注意事项！
具体的语句，我们可以参照：
1）memcached_functions_mysql-0.9/sql 目录下的trigger_fun.sql
2）使用参照文档：[http://dev.mysql.com/doc/refman/5.1/en/ha-memcached-interfaces-mysqludf.html](http://dev.mysql.com/doc/refman/5.1/en/ha-memcached-interfaces-mysqludf.html)
我们还必须注意以下几点：
1）mysql 编译时一定不要带'--with-mysqld-ldflags=-all-static' 这个参数，因为这样就限制了mysql 的动态安装功能了
2）使用时，要观察mysql.err日志，不知道是有意还是无意，udf更新memcache都会记录在err日志里，注意清理该日志，否则一下就爆满了
3）mysql 官网有这样一句话：
The list of servers used by the memcached UDFs is not persistent over restarts of the MySQL server. If the MySQL server fails,
 then you must re-set the list of memcachedservers.
所以，当我们重启mysql，我们必须通过select  memc_servers_set('192.168.0.1:11211,192.168.0.2:11211');语句重新注册memcache服务器！
### 相关文章
- [让memcached和mysql更好的工作](http://blog.haohtml.com/archives/386)
- [memcached vs MySQL Memory
 engine table 速度比较](http://blog.haohtml.com/archives/380)
- [多memcached 和 mysql 主从
 环境下PHP开发: 代码详解（转）](http://blog.haohtml.com/archives/383)
- [memcached配置](http://blog.haohtml.com/archives/364)
- [MySQL集群技术–转载](http://blog.haohtml.com/archives/915)
# 让memcached和mysql更好的工作
这次是[Fotolog](http://www.fotolog.com/)的经验，传说中比Flickr更大的网站，Fotolog在21台服务器上部署了51个memcached实例，总计有254G缓存空间可用，缓存了多达175G的内容，这个数量比很多网站的数据库都要大的多，原文是[A
 Bunch of Great Strategies for Using Memcached and MySQL Better Together](http://highscalability.com/bunch-great-strategies-using-memcached-and-mysql-better-together)，我这里还是选择性的翻译以及按照我的理解补充，感谢[Todd
 Hoff](http://highscalability.com/)，总能给我们一些学习的案例，从这里也能看出国外技术的开放态度，不似我们，其实就那么点小九九还藏着掖着，好了，进入正题。
一、关于[memcached](http://www.ourmysql.com/archives/tag/memcached)
　　还不知道这个？那你去面试的时候要吃亏了，赶紧去官方网站看一下[http://www.danga.com/memcached/](http://www.danga.com/memcached/)，另外google一下用法，硬盘总是太慢，把数据存在内存里面吧，如果你只有一台服务器，推荐用一下[APC](http://www.ourmysql.com/wp-admin/www.php.net/apc)(Facebook在用)或者[eaccelerator](http://eaccelerator.net/)或者[Xcache](http://xcache.lighttpd.net/)(国人开发的)，这些产品单机效果更好，如果你需要分布式的缓存方案，那么用memcached吧。
二、memcached如何与mysql并肩作战？
- 通过数据库分片来解决数据库写扩展的问题把数据库分片，部署到不同的服务器上，免得只有一个主服务器，写操作成为瓶颈以及可能有的“单点故障”，一般的数据库分片主要是按照业务来分，尽可能的拆分业务，不相干的都独立起来做成服务也好
- 前端mysql和一堆memcached服务器来应付读的问题应用程序首先从memcached中获取数据，获取不到再从数据库中获得并保存在memcached中，以前看过一篇文章说好的应用95％的数据从memcache的中获得，3％的数据从mysql的query cache中获得，剩下2％才去查表，对比一下你的应用，差距有多远？
- 通过mysql复制（master-slave）来解决读的问题
首先mysql数据库通过master-slave读写分离，多个slave来应对应用程序读的操作。
三、为什么不用mysql的query cache？
　　我们都知道mysql有个query cache，可以缓存上次查询的结果，可实际上帮不上太多的忙，下面是mysql quety cache的不足：
- 只能有一个实例
意味着你能存储内容的上限就是你服务器的可用内存，一台服务器能有多少内存？你又能存多少呢？
- 只要有写操作，mysql的query cache就失效
只要数据库内容稍有改变，那怕改变的是其他行，mysql的query cache也会失效
- mysql的query cache只能缓存数据库数据行
意味着其他内容都不行，比如数组，比如对象，而memcached理论上可以缓存任何内容，甚至文件^_^
四、Fotolog的缓存技术
- 非确定性缓存你不确定你要的数据缓存中有没有，你也不知道是不是过期了，于是你就试探性的问memcached，我要的什么什么数据你那有吗？我可不要过期的数据啊，memcached告诉你说有并且给你，你就开心了，如果没有呢，你就要从数据库或者别的地方去获取了，这是memcached典型的应用。主要应用在：1.复杂的数据需要多次读取，你的数据库做了分片处理，从多个数据库中获取数据并组合起来是一个非常大的开销，你大可以把这些数据取出来之后存到memcached中
2.mysql query cache的一个好的替代方案，这样数据库其他部门改变了，只要自己没改变就没问题（注意数据库更新的问题，后面会提到）
3.把关系或者列表缓存起来，比如某个栏目下的多篇文章列表
4.被多个页面调用并且获取起来很慢的数据，或者是更新很慢的数据，比如文章浏览排行榜
5.如果cache的开销超过重新获取的开销，那么不要缓存它吧
6.标签云和自动建议(类似google sugest)
例如：当一个用户上传一个图片，这个用户的好友页面上都要列出这张图片来，那么把它缓存起来吧。
潜在问题：
memcached消耗的主要是服务器内存，对CPU消耗很小，所以Fotolog把memcached部署在他们的应用服务器上（貌似我们也是这样），他们遇到了CPU搞到90％的使用率（怎么会那么高？哪出问题了吧）、内存回收（这是个大问题）等等问题。
- 状态缓存把应用服务的当前状态存在memcached中主要应用在：1.“昂贵”的操作，开销大的操作
2.sessions会话，Flickr把session存在数据库中，个人感觉还是存memcached比较“便宜”些，如果memecached服务器down掉了，那么重新登录吧。
3.记录用户在线信息(我们也是这样做的)
- 确定性缓存对于某些特定数据库的全部内容，都缓存到memcached，有一个专门的应用服务来保障你要的数据都在memcached中，其他应用服务直接从memcached中获取数据而不去取数据库，因为数据库已经全部保存到memcached中并保持同步。主要应用在：1.读取伸展，所有的读取都从memcached中获得，数据库没有负载
2.”永不过期“(相对的)的数据，比如行政规划数据，变动很小吧
3.经常调用的内容
4.用户的认证信息
5.用户的概要信息
6.用户的参数设置
7.用户当前常用的媒体文件列表，比如用户的图片
8.用户登录，不走数据库，只走memcached（个人觉得这个不太好，登录信息还是需要持久化的，用类似BDB这样效果也不错）
使用方式：
1.多个专门的缓存池而不是一个大的缓存服务器，多个缓存池保障了高可用性，一个缓存实例挂掉了走其他的缓存实例，所有的缓存实例挂掉了，走数据库（估计数据库抗不住^_^）
2.所有的缓存池都用程序来维护，比如数据库有更新时，程序自动把更新后的内容同步到多个缓存实例中
3.服务器重启之后，缓存要比网站先启动，这就意味着当网站已经启动了，所有的缓存都可用
4.读取的请求可以负载均衡到多个缓存实例中去，高性能，高可靠性
潜在的问题：
1.你需要足够多的内存来存储那么多的数据
2.数据以行记录数据，而memcached以对象来存储数据，你的逻辑要把行列的数据转换成缓存对象
3.要维护多个缓存实例非常麻烦,Fotolog用Java/Hibernate，他们自己写了个客户端来轮询
4.管理多个缓存实例会增加应用程序的许多开销，但这些开销相对于多个缓存得到的好处来说算不了什么
- 主动缓存数据魔法般的出现在缓存中，当数据库中有更新的时候，缓存立马填充，更新的数据被调用的可能性更高（比如一篇新文章，看的的人当然多），是非确定性缓存的一种变形(原文是It’s non-deterministic caching with a twist.我觉得这样翻译怪怪的)。主要应用在：1.预填充缓存：让memcached尽可能的少调用mysql如果内容不展现的话。
2.“预热”缓存：当你需要跨数据中心复制的时候
使用步骤：
1.解析数据库更新的二进制日志，发现数据库更新时对memcached也进行同样的更新
2.执行用户自定义函数，设置触发器调用UDF更新，具体参考[http://tangent.org/586/Memcached_Functions_for_MySQL.html](http://tangent.org/586/Memcached_Functions_for_MySQL.html)
3.使用[BLACKHOLE](http://dev.mysql.com/doc/refman/5.0/en/blackhole-storage-engine.html)策略，传说中Facebook也用mysql的Blackhole存储引擎来填充缓存，写到Blackhole的数据复制到缓存中，Facebook用这来设置数据作废以及跨国界的复制，好处是数据库的复制不走mysql，这就意味着没有二进制日志以及对CPU使用不那么多（啊？难道通过memcached存储二进制日志，然后复制到不同的数据库？有经验的同志在这个话题上可以补充。）
- 文件系统缓存把文件直接缓存在memcached中，哇，够BT的，减轻NFS的负担，估计只缓存那些过于热门的图片吧。
- 部分页面内容缓存如果页面的某些部分获取起来非常费劲，以其缓存页面的原始数据还不如把页面的部分内容直接缓存起来直接调用
- 应用程序级别的复制通过API来更新缓存，API的执行细节如下：1.一个应用把数据写到某个缓存实例，这个缓存实例把内容复制到其他缓存实例（memcached同步）2.自动获得缓存池地址以及实例个数
3.同时对多个缓存实例更新
4.如果某个缓存实例down掉了，跳到下一个实例，直到更新成功
整个过程非常高效以及低开销
- 其他技巧1.多节点以应对”单点故障”2.使用热备技术，当某个节点down掉了，另外一台服务自动替换成它的IP，这样客户端不用更新memcached的IP地址3.memcached可以通过TCP/UDP访问，持续连接可以减轻负载，系统设计成可同时承受1000个连接
4.不同的应用服务，不同的缓存服务器群
5.检查一下你的数据大小是否匹配你分配的缓存，更多请参考[http://download.tangent.org/talks/Memcached%20Study.pdf](http://download.tangent.org/talks/Memcached%20Study.pdf)
6.不要考虑数据行缓存，缓存复杂的对象
7.不要在你的数据库服务器上跑memcached，两个都是吃内存的怪兽
8.不要被TCP延迟困扰，本地的TCP/IP对内存复制是做了优化的
9.尽可能的并行处理数据
10.并不是所有的memcached的客户端都是一样的，仔细研究你用的语言所对应的（好像php和memcached配合的不错）
11.尽可能的是数据过期而不是使数据无效，memcached可以设定过期时间
12.选择一个好的缓存标识key，比如更新的时候加上版本号
13.把版本号存储在memcached中
　　作者最后的感言我就不翻译了，貌似mysql proxy正在做一个项目，自动同步mysql以及memcached，更多参考[http://jan.kneschke.de/2008/5/18/mysql-proxy-replicating-into-memcache](http://jan.kneschke.de/2008/5/18/mysql-proxy-replicating-into-memcache)
