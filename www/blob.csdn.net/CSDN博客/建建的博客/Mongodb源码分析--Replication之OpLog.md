# Mongodb源码分析--Replication之OpLog - 建建的博客 - CSDN博客
2017年03月07日 14:42:14[纪建](https://me.csdn.net/u013898698)阅读数：465
  在之前的文章中，介绍了关于master-slave模式下的主从端代码的执行流程，因为当时篇幅所限，未对oplog的数据结构以及mongodb的local数据库作过多阐述，而这可能会让不知道其内容的朋友看代码时云里雾里找不到头绪，今天我专门用一篇文章来大致解释一下（这些内容可能会在后面章节中有所涉及）。
      首先了解一个local数据库:
      在mongod中，出于特殊目的（复制机制），保留性使用了local数据库。当使用认证机制时，对local数据库等同于认证admin数据库。
      当使用复制集(Replica sets)模式时，其会使用下面的local数据库：
```
local.system.replset 用于复制集配置对象存储 (通过shell下的rs.conf()或直接查询)
 local.oplog.rs       一个capped collection集合.可在命令行下使用--oplogSize 选项设置该集合大小尺寸.
 local.replset.minvalid  通常在复制集内使用，用于跟踪同步状态(sync status)
```
      主从复制模式(Master/Slave):
```
* Master
          o local.oplog.$main 存储"oplog"信息
          o local.slaves  存储在master结点上相应slave结点的同步情况（比如syncTo时间戳等）
    * Slave
          o local.sources 从结点所要链接的master结点信息（可通过--source配置参数指定）
    * Other
          o local.me 未知待查:)
          o local.pair.* (replica pairs选项，目前已不推荐使用)
    
    除了了解local之外，还有oplog的数据结构（存储在local.oplog.$main）要解释一下：
     { ts : ..., op: ..., ns: ..., o: ... o2: ...  }
    上面就是一条oplog信息，复制机制就是通过这些信息来进行节点间的数据同步并维护数据一致性的，其中：
   
```
ts：8字节的时间戳，由4字节unix timestamp + 4字节自增计数表示。
        这个值很重要，在选举(如master宕机时)新primary时，会选择ts最大的那个secondary作为新primary。
    op：1字节的操作类型，例如i表示insert，d表示delete。
    ns：操作所在的namespace。
    o：操作所对应的document,即当前操作的内容（比如更新操作时要更新的的字段和值）
    o2: 在执行更新操作时的where条件，仅限于update时才有该属性
```
    其中op，可以是如下几种情形之一：
```
"i"： insert
     "u"： update
     "d"： delete
     "c"： db cmd
     "db"：声明当前数据库 (其中ns 被设置成为=>数据库名称+ '.')
     "n":  no op,即空操作，其会定期执行以确保时效性
```
   了解了这些内容之后，大家通过下面方式来验证一下上面的这些内容：
   1.本地构造复制集
```
D:\mongodb\bin>mongod --replSet myoplogs --dbpath d:\mongodb\db
```
   2.使用mongo连接到上面结点并初始化
```
D:\mongodb\bin>mongo
      MongoDB shell version: ...
      connecting to: test
      > rs.initiate()
```
   3.查看oplog复制集信息
```
> use local
     switched to db local
   > db.oplog.rs.find()
```
{"ts" : { "t" : 1306207268000, "i" : 1 }, "h" : NumberLong(0), "op" : "n", "ns" : "", "o" : { "msg" : "initiating set" } }
    4.开始添加记录 
```
> use test
     switched to db test
   > db.foo.insert({x:1})
   > db.foo.update({x:1}, {set : {y:1}}, true)
   > db.foo.remove({x:1})
```
   5.查看上面操作生成的oplog信息：
```
> use local
     switched to db local
   > db.oplog.rs.find()
```
    { "ts" : { "t" : 1306207268000, "i" : 1 }, "h" : NumberLong(0), "op" : "n", "ns" : "", "o" : { "msg" : "initiating set" } }
    { "ts" : { "t" : 1306207310000, "i" : 1 }, "h" : NumberLong("3138280161636515857"), "op" : "i", "ns" : "test.foo", "o" : { "_id" : ObjectId("4ddb244d2d0d00000000551a"), "x" : 1 } }
    { "ts" : { "t" : 1306207314000, "i" : 1 }, "h" : NumberLong("772196482295043060"), "op" : "u", "ns" : "test.foo", "o2" : { "_id" : ObjectId("4ddb244d2d0d00000000551a") }, "o" : { "main.help() 
 db.printReplicationInfo();
   知道上面这些内容之后，我们来大致看一下mongod在源码层面上是如何存储oplog的，先请看下面文件：
oplog.cpp：顾名思义，它的作用就是存储和读取oplog，其主要包括方法：
 createOplog()：初始化本地local.oplog.$main 集合信息，包括大小尺寸等
_logOpOld():   用于在master/slave模式下向local.oplog.$main 添加oplog信息
_logOpObjRS():    用于在replset模式下向local.oplog.rs模式下添加oplog 信息
append_O_Obj():向已存在的obj对象后追加新的对象元素信息（主要用于更新类型的oplog）
pretouchN()及pretouchOperation():某些情况下MongoDB会锁住数据库。如果此时正有数百个请求，则它们会堆积起来，造成许多问题。这里使用下面的优化方式来避免锁定：每次更新前，先查询记录。查询操作会将对象放入内存，于是更新则会尽可能的迅速。在"主/从"部署方案中，从节点可以使用“-pretouch”参数运行，这也可以得到相同的效果。
       下面就借助源码来进一步深入了解，首先回顾一下之前这篇文章中的所描述的这段代码：
```
//repl.cpp
   void startReplication() {
        /* if we are going to be a replica set, we aren't doing other forms of replication. */
        if( !cmdLine._replSet.empty() ) {
            ......
            //绑定函数，oplog.cpp ->_logOpRS(), 用于处理replset类型的oplog操作
            newRepl();
            return;
        }
        //绑定函数，oplog.cpp ->_logOpOld(), 用于处理master-slave类型的oplog操作
        oldRepl();
     上面的newRepl()及oldRepl()主要是实现函数绑定，它会将oplog.cpp文件中的_logOpRS和_logOpOld方法加以绑定（本人猜测可能因为1.6版之后引入了replset，造成方法名称的重新命名和分配，导致这里用这种方式对老的方法进行“过渡”）。
     接着上面方面会调用oplog.cpp文件的createOplog（）来构造master的“local.oplog.$main”集合,如下：     
    
```
//如果是master，则构造并启动线程方法replMasterThread
     if ( replSettings.master || replPair ) {
            if ( replSettings.master )
                log(1) << "master=true" << endl;
            replSettings.master = true;
            //构造oplog集合"local.oplog.$main"
            createOplog();
            boost::thread t(replMasterThread);
        }
      在上面初始化及函数指针绑定任务完成了，就可以在数据同步时，根据oplog集合信息来进行同步了，如下：
```
//repl.cpp
   void ReplSource::sync_pullOpLog_applyOperation(BSONObj& op, OpTime *localLogTail, bool alreadyLocked) {
     .....
     if( cmdLine.pretouch && !alreadyLocked /*在非写锁下,因为写锁下进行pretouch操作没意义*/) {
            if( cmdLine.pretouch > 1 ) {
                /* note: this is bad - should be put in ReplSource.  but this is first test... */
                static int countdown;
                assert( countdown >= 0 );
                if( countdown > 0 ) {
                    countdown--; // was pretouched on a prev pass
                }
                else {
                    const int m = 4;
                    if( tp.get() == 0 ) {
                        int nthr = min(8, cmdLine.pretouch);
                        nthr = max(nthr, 1);
                        tp.reset( new ThreadPool(nthr) );
                    }
                    vector<BSONObj> v;
                    oplogReader.peek(v, cmdLine.pretouch);
                    unsigned a = 0;
                    while( 1 ) {
                        if( a >= v.size() ) break;
                        unsigned b = a + m - 1; // v[a..b]
                        if( b >= v.size() ) b = v.size() - 1;
                        tp->schedule(pretouchN, v, a, b);
                        DEV cout << "pretouch task: " << a << ".." << b << endl;
                        a += m;
                    }
                    //执行oplog.cpp中的预取操作
                    pretouchOperation(op);
                    tp->join();
                    countdown = v.size();
                }
            }
            else {
                //执行oplog.cpp中的预取操作
                pretouchOperation(op);
            }
        }
        ......
        //使用获取到的oplog操作信息进行同步操作，有关该方法本人已在这个链接中有关介绍
        //Mongodb源码分析--Replication之主从模式--Slave
        applyOperation( op );
        ......
    }
    上面方法中出现了预取操作，该技术主要是出现运行效率的考虑，下面是pretouchOperation方法的具体实现(pretouch方法实现与其大同小异，感兴趣的朋友可自行阅读)：
  
```
//oplog.cpp中的相应方法
    //某些情况下MongoDB会锁住数据库。如果此时正有数百个请求，则它们会堆积起来，造成许多问题。
    //这里使用下面的优化方式来避免锁定：
    //    每次更新前，先查询记录。查询操作会将对象放入内存，于是更新则会尽可能的迅速。
    //在主/从部署方案中，从节点可以使用“-pretouch”参数运行，这也可以得到相同的效果。
    void pretouchOperation(const BSONObj& op) {
        // no point pretouching if write locked.
        //not sure if this will ever fire, but just in case.
        if( dbMutex.isWriteLocked() )//写锁下则不执行pretouch操作
            return;
          
        const char *which = "o";//数据的对象
        const char *opType = op.getStringField("op");
        if ( *opType == 'i' )
            ;
        else if( *opType == 'u' )//如是更新操作时，则执行touch
            which = "o2";//更新的条件pattern
        else
            return;
        /* todo : other operations */
        try {
            BSONObj o = op.getObjectField(which);
            BSONElement _id;
            //获取_id信息,用于下面find查找
            if( o.getObjectID(_id) ) {
                const char *ns = op.getStringField("ns");
                BSONObjBuilder b;
                b.append(_id);
                BSONObj result;
                readlock lk(ns);
                Client::Context ctx( ns );
                //获取要更新的obj
                if( Helpers::findById(cc(), ns, b.done(), result) )
                    _dummy_z += result.objsize(); // touch
            }
        }
        catch( DBException& ) {
            log() << "ignoring assertion in pretouchOperation()" << endl;
        }
    }
    说到这里只是获取oplog的操作，那mongod进行cud操作时，log是如何记录到日志集合中的呢？还记得上面提到的函数绑定吗？我们来看一下oplog.cpp中的下面方法：
  
```
/*@ @param opstr:
          c userCreateNS
          i insert
          n no-op / keepalive
          d delete / remove
          u update
    */
    void logOp(const char *opstr, const char *ns, const BSONObj& obj, BSONObj *patt, bool *b) {
        if ( replSettings.master ) {
            _logOp(opstr, ns, 0, obj, patt, b);//这里将会执行之前函数绑定的方法,比如：_logOpOld
        }
        //用于在shard模式下oplog操作
        logOpForSharding( opstr , ns , obj , patt );
    }
   上面的logOp方法几乎出现在了所有cud操作场景中，比如在update操作时：
```
//update.cpp
   static UpdateResult _updateById(bool isOperatorUpdate, int idIdxNo, ModSet *mods, int profile, NamespaceDetails *d,
                                    NamespaceDetailsTransient *nsdt,
                                    bool god, const char *ns,
                                    const BSONObj& updateobj, BSONObj patternOrig, bool logop, OpDebug& debug) {
              ......
              if ( logop ) {
                 ......
                if( mss->needOpLogRewrite() ) {
                    DEBUGUPDATE( "\t rewrite update: " << mss->getOpLogRewrite() );
                    logOp("u", ns, mss->getOpLogRewrite() , &pattern );//记录日志
                }
                else {
                    logOp("u", ns, updateobj, &pattern );//记录日志
                }
             }
             .....
   }   
   这里以_logOpOld()方法为例，来看一下mongod是如何向local.oplog.$main集合中添加oplog日志的：   
```
/* we write to local.oplog.main".
       bb:
         if not null, specifies a boolean to pass along to the other side as b: param.
         used for "justOne" or "upsert" flags on 'd', 'u'      
       note this is used for single collection logging even when --replSet is enabled.
    */
    static void _logOpOld(const char *opstr, const char *ns, const char *logNS, const BSONObj& obj, BSONObj *o2, bool *bb ) {
        DEV assertInWriteLock();
        static BufBuilder bufbuilder(8*1024);
        //如是master上的local.slaves，则更新该数据库缓存信息
        if ( strncmp(ns, "local.", 6) == 0 ) {
            if ( strncmp(ns, "local.slaves", 12) == 0 ) {
                resetSlaveCache();
            }
            return;
        }
        //获取当前系统时间作为操作时间
        const OpTime ts = OpTime::now();
        Client::Context context;
        /* we jump through a bunch of hoops here to avoid copying the obj buffer twice --
           instead we do a single copy to the destination position in the memory mapped file.
        */
        //生成bson格式的oplog的对象
        bufbuilder.reset();
        BSONObjBuilder b(bufbuilder);
        b.appendTimestamp("ts", ts.asDate());
        b.append("op", opstr);
        b.append("ns", ns);
        if ( bb )
            b.appendBool("b", *bb);
        if ( o2 )//如果不为空，表示执行操作的条件（where）
            b.append("o2", *o2);
        BSONObj partial = b.done(); // partial is everything except the o:... part.
        int po_sz = partial.objsize();
        int len = po_sz + obj.objsize() + 1 + 2 /*o:*/;
        Record *r;
        //为0/null,表示"local.oplog.$main"
        if( logNS == 0 ) {
            logNS = "local.oplog.$main";
            if ( localOplogMainDetails == 0 ) {
                Client::Context ctx( logNS , dbpath, 0, false);
                localDB = ctx.db();
                assert( localDB );
                localOplogMainDetails = nsdetails(logNS);
                assert( localOplogMainDetails );
            }
            Client::Context ctx( logNS , localDB, false );
            //在指定Namespace下的Extent中分配一个record空间，用于下面添加log信息
            r = theDataFileMgr.fast_oplog_insert(localOplogMainDetails, logNS, len);
        }
        else {
            //在指定的logNS中分配一个record空间
            Client::Context ctx( logNS, dbpath, 0, false );
            assert( nsdetails( logNS ) );
            // 如第一次分配first we allocate the space, then we fill it below.
            r = theDataFileMgr.fast_oplog_insert( nsdetails( logNS ), logNS, len);
        }
        //将操作条件(where)追加到已存在对象
        append_O_Obj(r->data, partial, obj);
        //设置最近操作时间
        context.getClient()->setLastOp( ts.asDate() );
        if ( logLevel >= 6 ) {
            BSONObj temp(r);
            log( 6 ) << "logging op:" << temp << endl;
        }
    }
    /** given a BSON object, create a new one at dst which is the existing (partial) object
        with a new object element appended at the end with fieldname "o".
        @param partial already build object with everything except the o member.  e.g. something like:
               { ts:..., ns:..., o2:... }
        @param o a bson object to be added with fieldname "o"
        @dst   where to put the newly built combined object.  e.g. ends up as something like:
               { ts:..., ns:..., o2:..., o:... }
    */
    void append_O_Obj(char *dst, const BSONObj& partial, const BSONObj& o) {
        const int size1 = partial.objsize() - 1;  // less the EOO char
        const int oOfs = size1+3;                 // 3 = byte BSONOBJTYPE + byte 'o' + byte \0
        void *p = getDur().writingPtr(dst, oOfs+o.objsize()+1);
        memcpy(p, partial.objdata(), size1);
        // adjust overall bson object size for the o: field
        *(static_cast<unsigned*>(p)) += o.objsize() + 1/*fieldtype byte*/ + 2/*"o" fieldname*/;
        char *b = static_cast<char *>(p);
        b += size1;
        *b++ = (char) Object;
        *b++ = 'o'; // { o : ... }
        *b++ = 0;   // null terminate "o" fieldname
        memcpy(b, o.objdata(), o.objsize());
        b += o.objsize();
        *b = EOO;
    }
    好了，关于oplog的主要内容介绍的差不多了，感兴趣的朋友可以下载相关源码做进一步的分析。
    
    参考链接：    
    http://www.snailinaturtleneck.com/blog/2010/10/14/getting-to-know-your-oplog/
    http://www.snailinaturtleneck.com/blog/2010/08/02/replica-sets-part-2-what-are-replica-sets/
    
    
    原文链接:[http://www.cnblogs.com/daizhj/archive/2011/06/27/mongodb_sourcecode_oplog.html](http://www.cnblogs.com/daizhj/archive/2011/06/27/mongodb_sourcecode_oplog.html)
    作者: daizhj, 代震军   
    微博: http://t.sina.com.cn/daizhj
    Tags: mongodb,c++,Replica,master-slave,oplog,local database
```
```
```
```
```
```
```
```
