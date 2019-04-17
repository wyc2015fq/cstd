# 【空间释放】解决Linux下rm操作不释放空间的问题 - CD's Coding - CSDN博客





2015年11月30日 12:50:13[糖果天王](https://me.csdn.net/okcd00)阅读数：1624








最近在开发的过程中，不小心把屏幕回显的输出定向到nohup.out里去了，导致周末过完回来一看，我的天被打满了……

然后赶紧删掉这104个G的文件，看看空间还给我没——

啊咧？



```
$ df -h
Filesystem            Size  Used Avail Use% Mounted on
/dev/vda1              20G  3.4G   16G  18% /
/dev/vdb              155G  148G     0 100% /home
```
然后我赶紧看看这个文件夹大小——

啊咧？



```
du -h
176K    ./goose/python-goose/dist
96K     ./goose/python-goose/goose/resources/text
8.0K    ./goose/python-goose/goose/resources/images
108K    ./goose/python-goose/goose/resources
24K     ./goose/python-goose/goose/utils
84K     ./goose/python-goose/goose/extractors
304K    ./goose/python-goose/goose
36K     ./goose/python-goose/goose_extractor.egg-info
4.0K    ./goose/python-goose/build/bdist.linux-x86_64
96K     ./goose/python-goose/build/lib/goose/resources/text
8.0K    ./goose/python-goose/build/lib/goose/resources/images
108K    ./goose/python-goose/build/lib/goose/resources
24K     ./goose/python-goose/build/lib/goose/utils
84K     ./goose/python-goose/build/lib/goose/extractors
300K    ./goose/python-goose/build/lib/goose
68K     ./goose/python-goose/build/lib/tests/extractors
96K     ./goose/python-goose/build/lib/tests
400K    ./goose/python-goose/build/lib
408K    ./goose/python-goose/build
44K     ./goose/python-goose/.git/hooks
8.0K    ./goose/python-goose/.git/logs/refs/remotes/origin
12K     ./goose/python-goose/.git/logs/refs/remotes
8.0K    ./goose/python-goose/.git/logs/refs/heads
24K     ./goose/python-goose/.git/logs/refs
32K     ./goose/python-goose/.git/logs
8.0K    ./goose/python-goose/.git/info
2.0M    ./goose/python-goose/.git/objects/pack
4.0K    ./goose/python-goose/.git/objects/info
2.0M    ./goose/python-goose/.git/objects
4.0K    ./goose/python-goose/.git/refs/tags
8.0K    ./goose/python-goose/.git/refs/remotes/origin
12K     ./goose/python-goose/.git/refs/remotes
8.0K    ./goose/python-goose/.git/refs/heads
28K     ./goose/python-goose/.git/refs
4.0K    ./goose/python-goose/.git/branches
2.2M    ./goose/python-goose/.git
68K     ./goose/python-goose/tests/extractors
8.0K    ./goose/python-goose/tests/data/parser
292K    ./goose/python-goose/tests/data/extractors/tags
12K     ./goose/python-goose/tests/data/extractors/tweets
4.0M    ./goose/python-goose/tests/data/extractors/content
12K     ./goose/python-goose/tests/data/extractors/opengraph
32K     ./goose/python-goose/tests/data/extractors/videos
112K    ./goose/python-goose/tests/data/extractors/images/test_basic_image
12K     ./goose/python-goose/tests/data/extractors/images/test_known_image_name_parent
12K     ./goose/python-goose/tests/data/extractors/images/test_known_image_css_parent_class
12K     ./goose/python-goose/tests/data/extractors/images/test_known_image_css_parent_id
12K     ./goose/python-goose/tests/data/extractors/images/test_opengraph_tag
12K     ./goose/python-goose/tests/data/extractors/images/test_known_image_empty_src
12K     ./goose/python-goose/tests/data/extractors/images/test_known_image_css_id
12K     ./goose/python-goose/tests/data/extractors/images/test_known_image_css_class
200K    ./goose/python-goose/tests/data/extractors/images
36K     ./goose/python-goose/tests/data/extractors/publishdate
12K     ./goose/python-goose/tests/data/extractors/links
20K     ./goose/python-goose/tests/data/extractors/title
12K     ./goose/python-goose/tests/data/extractors/authors
4.6M    ./goose/python-goose/tests/data/extractors
4.6M    ./goose/python-goose/tests/data
4.7M    ./goose/python-goose/tests
7.8M    ./goose/python-goose
7.8M    ./goose
12K     ./nginx-push-stream-module/nginx-1.2.0/man
24K     ./nginx-push-stream-module/nginx-1.2.0/contrib/unicode2nginx
36K     ./nginx-push-stream-module/nginx-1.2.0/contrib
4.0K    ./nginx-push-stream-module/nginx-1.2.0/objs/src/mail
4.0K    ./nginx-push-stream-module/nginx-1.2.0/objs/src/http/modules/perl
2.7M    ./nginx-push-stream-module/nginx-1.2.0/objs/src/http/modules
4.4M    ./nginx-push-stream-module/nginx-1.2.0/objs/src/http
1000K   ./nginx-push-stream-module/nginx-1.2.0/objs/src/core
484K    ./nginx-push-stream-module/nginx-1.2.0/objs/src/os/unix
4.0K    ./nginx-push-stream-module/nginx-1.2.0/objs/src/os/win32
492K    ./nginx-push-stream-module/nginx-1.2.0/objs/src/os
48K     ./nginx-push-stream-module/nginx-1.2.0/objs/src/event/modules
288K    ./nginx-push-stream-module/nginx-1.2.0/objs/src/event
4.0K    ./nginx-push-stream-module/nginx-1.2.0/objs/src/misc
6.1M    ./nginx-push-stream-module/nginx-1.2.0/objs/src
4.0K    ./nginx-push-stream-module/nginx-1.2.0/objs/addon/src
8.0K    ./nginx-push-stream-module/nginx-1.2.0/objs/addon
6.2M    ./nginx-push-stream-module/nginx-1.2.0/objs
28K     ./nginx-push-stream-module/nginx-1.2.0/auto/os
44K     ./nginx-push-stream-module/nginx-1.2.0/auto/cc
12K     ./nginx-push-stream-module/nginx-1.2.0/auto/lib/libatomic
8.0K    ./nginx-push-stream-module/nginx-1.2.0/auto/lib/libxslt
24K     ./nginx-push-stream-module/nginx-1.2.0/auto/lib/sha1
12K     ./nginx-push-stream-module/nginx-1.2.0/auto/lib/perl
8.0K    ./nginx-push-stream-module/nginx-1.2.0/auto/lib/libgd
8.0K    ./nginx-push-stream-module/nginx-1.2.0/auto/lib/geoip
28K     ./nginx-push-stream-module/nginx-1.2.0/auto/lib/zlib
28K     ./nginx-push-stream-module/nginx-1.2.0/auto/lib/pcre
8.0K    ./nginx-push-stream-module/nginx-1.2.0/auto/lib/google-perftools
20K     ./nginx-push-stream-module/nginx-1.2.0/auto/lib/openssl
24K     ./nginx-push-stream-module/nginx-1.2.0/auto/lib/md5
196K    ./nginx-push-stream-module/nginx-1.2.0/auto/lib
20K     ./nginx-push-stream-module/nginx-1.2.0/auto/types
432K    ./nginx-push-stream-module/nginx-1.2.0/auto
40K     ./nginx-push-stream-module/nginx-1.2.0/conf
280K    ./nginx-push-stream-module/nginx-1.2.0/src/mail
64K     ./nginx-push-stream-module/nginx-1.2.0/src/http/modules/perl
1.3M    ./nginx-push-stream-module/nginx-1.2.0/src/http/modules
2.1M    ./nginx-push-stream-module/nginx-1.2.0/src/http
668K    ./nginx-push-stream-module/nginx-1.2.0/src/core
420K    ./nginx-push-stream-module/nginx-1.2.0/src/os/unix
424K    ./nginx-push-stream-module/nginx-1.2.0/src/os
140K    ./nginx-push-stream-module/nginx-1.2.0/src/event/modules
348K    ./nginx-push-stream-module/nginx-1.2.0/src/event
12K     ./nginx-push-stream-module/nginx-1.2.0/src/misc
3.8M    ./nginx-push-stream-module/nginx-1.2.0/src
12K     ./nginx-push-stream-module/nginx-1.2.0/html
11M     ./nginx-push-stream-module/nginx-1.2.0
272K    ./nginx-push-stream-module/nginx-push-stream-module/src
92K     ./nginx-push-stream-module/nginx-push-stream-module/include
44K     ./nginx-push-stream-module/nginx-push-stream-module/.git/hooks
8.0K    ./nginx-push-stream-module/nginx-push-stream-module/.git/logs/refs/remotes/origin
12K     ./nginx-push-stream-module/nginx-push-stream-module/.git/logs/refs/remotes
8.0K    ./nginx-push-stream-module/nginx-push-stream-module/.git/logs/refs/heads
24K     ./nginx-push-stream-module/nginx-push-stream-module/.git/logs/refs
32K     ./nginx-push-stream-module/nginx-push-stream-module/.git/logs
8.0K    ./nginx-push-stream-module/nginx-push-stream-module/.git/info
2.1M    ./nginx-push-stream-module/nginx-push-stream-module/.git/objects/pack
4.0K    ./nginx-push-stream-module/nginx-push-stream-module/.git/objects/info
2.1M    ./nginx-push-stream-module/nginx-push-stream-module/.git/objects
4.0K    ./nginx-push-stream-module/nginx-push-stream-module/.git/refs/tags
8.0K    ./nginx-push-stream-module/nginx-push-stream-module/.git/refs/remotes/origin
12K     ./nginx-push-stream-module/nginx-push-stream-module/.git/refs/remotes
8.0K    ./nginx-push-stream-module/nginx-push-stream-module/.git/refs/heads
28K     ./nginx-push-stream-module/nginx-push-stream-module/.git/refs
4.0K    ./nginx-push-stream-module/nginx-push-stream-module/.git/branches
2.3M    ./nginx-push-stream-module/nginx-push-stream-module/.git
60K     ./nginx-push-stream-module/nginx-push-stream-module/docs/examples
36K     ./nginx-push-stream-module/nginx-push-stream-module/docs/directives
112K    ./nginx-push-stream-module/nginx-push-stream-module/docs
20K     ./nginx-push-stream-module/nginx-push-stream-module/misc/examples
116K    ./nginx-push-stream-module/nginx-push-stream-module/misc/js
48K     ./nginx-push-stream-module/nginx-push-stream-module/misc/tools
72K     ./nginx-push-stream-module/nginx-push-stream-module/misc/spec/publisher
168K    ./nginx-push-stream-module/nginx-push-stream-module/misc/spec/subscriber
8.0K    ./nginx-push-stream-module/nginx-push-stream-module/misc/spec/javascripts/helpers
12K     ./nginx-push-stream-module/nginx-push-stream-module/misc/spec/javascripts/support
72K     ./nginx-push-stream-module/nginx-push-stream-module/misc/spec/javascripts
124K    ./nginx-push-stream-module/nginx-push-stream-module/misc/spec/mix
456K    ./nginx-push-stream-module/nginx-push-stream-module/misc/spec
676K    ./nginx-push-stream-module/nginx-push-stream-module/misc
3.5M    ./nginx-push-stream-module/nginx-push-stream-module
16M     ./nginx-push-stream-module
44K     ./word2vec/.git/hooks
8.0K    ./word2vec/.git/logs/refs/remotes/origin
12K     ./word2vec/.git/logs/refs/remotes
8.0K    ./word2vec/.git/logs/refs/heads
24K     ./word2vec/.git/logs/refs
32K     ./word2vec/.git/logs
8.0K    ./word2vec/.git/info
132K    ./word2vec/.git/objects/pack
4.0K    ./word2vec/.git/objects/info
140K    ./word2vec/.git/objects
4.0K    ./word2vec/.git/refs/tags
8.0K    ./word2vec/.git/refs/remotes/origin
12K     ./word2vec/.git/refs/remotes
8.0K    ./word2vec/.git/refs/heads
28K     ./word2vec/.git/refs
276K    ./word2vec/.git
8.9G    ./word2vec
8.9G    .
```


没错啊，总计8.9G，那100多G的文件确实我已经删了啊……再看一下磁盘空间：

100%……

这这这……我已经删了啊

赶紧查一查解决方法——

好多地方都说是杀进程，于是top，kill了一下……居然真的就把空间还给我了……



```
df -h
Filesystem            Size  Used Avail Use% Mounted on
/dev/vda1              20G  3.4G   16G  18% /
/dev/vdb              155G   49G   99G  33% /home
```



关于网上的解决方案和解释，大概如下——







因为被删除的文件在删除的时侯还是进程在操作(打开、访问等)的缘故，rm只完成了在磁盘上文件实体的释放，而类似free list结构中相应的文件系统因进程的操作相应的inode并未释放。


解决的方法：这样的问题解决起来也很简单，找到操作的进程，kill掉就可以了，可是找到操作的进程恰恰是本问题的难点和关键。这样的问题也可以通过重启机器和nmount/mount文件系统这样的方式解决，但这样的方法我是不提倡的，小小的问题就重启机器，小题大做。


linux及solaris可以这样做: 

a、下载一个lsof软件装上，google上可以搜到 

b、找到正在用被删文件的进程 
lsof | grep deleted 
c、kill掉相应的进程空间就释放了





