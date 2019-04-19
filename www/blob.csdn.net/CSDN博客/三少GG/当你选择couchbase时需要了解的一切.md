# 当你选择couchbase时需要了解的一切 - 三少GG - CSDN博客
2014年07月31日 11:01:45[三少GG](https://me.csdn.net/scut1135)阅读数：3158
**优质架构师网站：**
**http://architects.dzone.com/**
[http://architects.dzone.com/articles/couchbase-architecture-deep](http://architects.dzone.com/articles/couchbase-architecture-deep)
# Everything You Need To Know About Couchbase Architecture
07.06.2012
|11875
 views |
in**Share**15
- 
- 
- 
- 
- 15
in**Share**
- 
- ![submit to reddit](http://www.reddit.com/static/spreddit7.gif)
After receiving a lot of good feedback and comment on [my
 last blog on MongoDb](http://horicky.blogspot.com/2012/04/mongodb-architecture.html), I was encouraged to do another deep dive on another popular document oriented db; Couchbase.
I have been a long-time fan CouchDb and has [wrote
 a blog on it many years ago](http://horicky.blogspot.com/2008/10/couchdb-implementation.html).  After it merges with Membase, I am very excited to take a deep look into it again.
Couchbase is the merge of two popular NOSQL technologies:  
- 
Membase, which provides persistence, replication, sharding to the high performance memcached technology
- 
CouchDB, which pioneers the document oriented model based on JSON
Like other NOSQL technologies, both Membase and CouchDB are built from the ground up on a highly distributed architecture, with data shard across machines in a cluster.  Built around the Memcached protocol, Membase provides an easy migration to existing Memcached
 users who want to add persistence, sharding and fault resilience on their familiar Memcached model.  On the other hand, CouchDB provides first class support for storing JSON documents as well as a simple RESTful API to access them.  Underneath, CouchDB also
 has a highly tuned storage engine that is optimized for both update transaction as well as query processing.  Taking the best of both technologies, Membase is well-positioned in the NOSQL marketplace.
### Programming model
Couchbase provides client libraries for different programming languages such as Java / .NET / PHP / Ruby / C / Python / Node.js
For read, Couchbase provides a key-based lookup mechanism where the client is expected to provide the key, and only the server hosting the data (with that key) will be contacted.
Couchbase also provides a query mechanism to retrieve data where the client provides a query (for example, range based on some  secondary key) as well as the view (basically the index).  The query will be broadcasted to all servers in the cluster and the result
 will be merged and sent back to the client.
For write, Couchbase provides a key-based update mechanism where the client sends in an updated document with the key (as doc id).  When handling write request, the server will return to client’s write request as soon as the data is stored in RAM on the active
 server, which offers the lowest latency for write requests. 
Following is the core API that Couchbase offers.  (in an abstract sense)
[view
 source](http://architects.dzone.com/articles/couchbase-architecture-deep#viewSource)
[print](http://architects.dzone.com/articles/couchbase-architecture-deep#printSource)[?](http://architects.dzone.com/articles/couchbase-architecture-deep#about)
`01.`
```
#
 Get a document by key
```
`02.`
`03.`
```
doc
 = get(key)
```
`04.`
`05.`
```
#
 Modify a document, notice the whole document
```
`06.`
```
#  
 need to be passed in
```
`07.`
`08.`
```
set(key,
 doc)
```
`09.`
`10.`
```
#
 Modify a document when no one has modified it
```
`11.`
```
# 
 since my last read
```
`12.`
`13.`
```
casVersion
 = doc.getCas()
```
`14.`
```
cas(key,
 casVersion, changedDoc)
```
`15.`
`16.`
```
#
 Create a new document, with an expiration time
```
`17.`
```
#  
 after which the document will be deleted
```
`18.`
`19.`
```
addIfNotExist(key,
 doc, timeToLive)
```
`20.`
`21.`
```
#
 Delete a document
```
`22.`
`23.``delete``(key)`
`24.`
`25.`
```
#
 When the value is an integer, increment the integer
```
`26.`
`27.``increment(key)`
`28.`
`29.`
```
#
 When the value is an integer, decrement the integer
```
`30.`
`31.``decrement(key)`
`32.`
`33.`
```
#
 When the value is an opaque byte array, append more
```
`34.`
```
# 
 data into existing value
```
`35.`
`36.`
```
append(key,
 newData)
```
`37.`
`38.`
```
#
 Query the data
```
`39.`
`40.`
```
results
 = query(viewName, queryParameters)
```
In Couchbase, document is the unit of manipulation.  Currently Couchbase doesn't support server-side execution of custom logic.  Couchbase server is basically a passive store and unlike other document oriented DB, Couchbase doesn't support field-level modification. 
 In case of modifying documents, client need to retrieve documents by its key, do the modification locally and then send back the whole (modified) document back to the server.  This design tradeoff network bandwidth (since more data will be transferred across
 the network) for CPU (now CPU load shift to client).
Couchbase currently doesn't support bulk modification based on a condition matching.  Modification happens only in a per document basis.  (client will save the modified document one at a time).
### Transaction Model
Similar to many NOSQL databases, Couchbase’s transaction model is primitive as compared to RDBMS.  Atomicity is guaranteed at a single document and transactions that span update of multiple documents are unsupported.  To provide necessary isolation for concurrent
 access, Couchbase provides a CAS (compare and swap) mechanism which works as follows …
- 
When the client retrieves a document, a CAS ID (equivalent to a revision number) is attached to it.
- 
While the client is manipulating the retrieved document locally, another client may modify this document.  When this happens, the CAS ID of the document at the server will be incremented.
- 
Now, when the original client submits its modification to the server, it can attach the original  CAS ID in its request.  The server will verify this ID with the actual ID in the server.  If they differ, the document has been updated in between and the server
 will not apply the update.
- 
The original client will re-read the document (which now has a newer ID) and re-submit its modification. 
Couchbase also provides a locking mechanism for clients to coordinate their access to documents.  Clients can request a LOCK on the document it intends to modify, update the documents and then releases the LOCK.  To prevent a deadlock situation, each LOCK grant
 has a timeout so it will automatically be released after a period of time.
### Deployment Architecture
 In a typical setting, a Couchbase DB resides in a server clusters involving multiple machines.  Client library will connect to the appropriate servers to access the data.  Each machine contains a number of daemon processes which provides data access as well
 as management functions.
![](http://4.bp.blogspot.com/-Tjau6V_HEkg/T-1HnK4bfxI/AAAAAAAAAww/CQGwo4OG8yw/s400/p1.png)
The data server, written in C/C++, is responsible to handle get/set/delete request from client.  The Management server, written in Erlang, is responsible to handle the query traffic from client, as well as manage the configuration and communicate with other
 member nodes in the cluster.
### Virtual Buckets
The basic unit of data storage in Couchbase DB is a JSON document (or primitive data type such as int and byte array) which is associated with a key.  The overall key space is partitioned into 1024 logical storage unit called "virtual buckets" (or vBucket). 
 vBucket are distributed across machines within the cluster via a map that is shared among servers in the cluster as well as the client library.
![](http://2.bp.blogspot.com/-E5EyhY6_1cI/T-1Lxefs7zI/AAAAAAAAAw8/nouSETW7-k8/s400/p2.png)
High availability is achieved through data replication at the vBucket level.  Currently Couchbase supports one active vBucket zero or more standby replicas hosted in other machines.  Curremtly the standby server are idle and not serving any client request. 
 In future version of Couchbase, the standby replica will be able to serve read request.
Load balancing in Couchbase is achieved as follows:
- 
Keys are uniformly distributed based on the hash function
- 
When machines are added and removed in the cluster.  The administrator can request a redistribution of vBucket so that data are evenly spread across physical machines.
### Management Server
Management server performs the management function and co-ordinate the other nodes within the cluster.  It includes the following monitoring and administration functions
Heartbeat: A watchdog process periodically communicates with all member nodes within the same cluster to provide Couchbase Server health updates.
Process monitor: This subsystem monitors execution of the local data manager, restarting failed processes as required and provide status information
 to the heartbeat module.
Configuration manager: Each Couchbase Server node shares a cluster-wide configuration which contains the member nodes within the cluster, a vBucket
 map.  The configuration manager pull this config from other member nodes at bootup time.
Within a cluster, one node’s Management Server will be elected as the leader which performs the following cluster-wide management function
- 
Controls the distribution of vBuckets among other nodes and initiate vBucket migration
- 
Orchestrates the failover and update the configuration manager of member nodes
If the leader node crashes, a new leader will be elected from surviving members in the cluster.
When a machine in the cluster has crashed, the leader will detect that and notify member machines in the cluster that all vBuckets hosted in the crashed machine is dead.  After getting this signal, machines hosting the corresponding vBucket replica will set
 the vBucket status as “active”.  The vBucket/server map is updated and eventually propagated to the client lib.  Notice that at this moment, the replication level of the vBucket will be reduced.  Couchbase doesn’t automatically re-create new replicas which
 will cause data copying traffic.  Administrator can issue a command to explicitly initiate a data rebalancing.  The crashed machine, after reboot can rejoin the cluster.  At this moment, all the data it stores previously will be completely discard and the
 machine will be treated as a brand new empty machine.
As more machines are put into the cluster (for scaling out), vBucket should be redistributed to achieve a load balance.  This is currently triggered by an explicit command from the administrator.  Once receive the “rebalance” command, the leader will compute
 the new provisional map which has the balanced distribution of vBuckets and send this provisional map to all members of the cluster.
To compute the vBucket map and migration plan, the leader attempts the following objectives:
- 
Evenly distribute the number of active vBuckets and replica vBuckets among member nodes.
- 
Place the active copy and each replicas in physically separated nodes.
- 
Spread the replica vBucket as wide as possible among other member nodes.
- 
Minimize the amount of data migration
- 
Orchestrate the steps of replica redistribution so no node or network will be overwhelmed by the replica migration.
Once the vBucket maps is determined, the leader will pass the redistribution map to each member in the cluster and coordinate the steps of vBucket migration.  The actual data transfer happens directly between the origination node to the destination node.
Notice that since we have generally more vBuckets than machines.  The workload of migration will be evenly distributed automatically.  For example, when new machines are added into the clusters, all existing machines will migrate some portion of its vBucket
 to the new machines.  There is no single bottleneck in the cluster.
Throughput the migration and redistribution of vBucket among servers, the life cycle of a vBucket in a server will be in one of the following states
- 
“Active”:  means the server is hosting the vBucket is ready to handle both read and write request
- 
“Replica”: means the server is hosting the a copy of the vBucket that may be slightly out of date but can take read request that can tolerate some degree of outdate.
- 
“Pending”: means the server is hosting a copy that is in a critical transitional state.  The server cannot take either read or write request at this moment.
- 
“Dead”: means the server is no longer responsible for the vBucket and will not take either read or write request anymore.
### Data Server
Data server implements the memcached APIs such as get, set, delete, append, prepend, etc. It contains the following key datastructure:
- 
One in-memory hashtable (key by doc id) for the corresponding vBucket hosted.  The hashtable acts as both a metadata for all documents as well as a cache for the document content.  Maintain the entry gives a quick way to detect whether the document exists on
 disk.
- 
To support async write, there is a checkpoint linkedlist per vBucket holding the doc id of modified documents that hasn't been flushed to disk or replicated to the replica.
![](http://2.bp.blogspot.com/-ItznjiYqb_I/T_Xlpc70WqI/AAAAAAAAAxU/SkI9dC4g6-o/s400/p1.png)
To handle a "GET" request
- 
Data server routes the request to the corresponding ep-engine responsible for the vBucket.
- 
The ep-engine will lookup the document id from the in-memory hastable.  If the document content is found in cache (stored in the value of the hashtable), it will be returned.  Otherwise, a background disk fetch task will be created and queued into the RO dispatcher
 queue.
- 
The RO dispatcher then reads the value from the underlying storage engine and populates the corresponding entry in the vbucket hash table.
- 
Finally, the notification thread notifies the disk fetch completion to the memcached pending connection, so that the memcached worker thread can revisit the engine to process a get request.
To handle a "SET" request,  a success response will be returned to the calling client once the updated document has been put into the in-memory hashtable with a write request put into the checkpoint buffer.  Later on the Flusher thread will pickup the outstanding
 write request from each checkpoint buffer, lookup the corresponding document content from the hashtable and write it out to the storage engine.
Of course, data can be lost if the server crashes before the data has been replicated to another server and/or persisted.  If the client requires a high data availability across different crashes, it can issue a subsequent observe() call which blocks on the
 condition  that the server persist data on disk, or the server has replicated the data to another server (and get its ACK).  Overall speaking, the client has various options to tradeoff data integrity with throughput.
Hashtable Management
To synchronize accesses to a vbucket hash table, each incoming thread needs to acquire a lock before accessing a key region of the hash table. There are multiple locks per vbucket hash table, each of which is responsible for controlling exclusive accesses to
 a certain ket region on that hash table. The number of regions of a hash table can grow dynamically as more documents are inserted into the hash table.
To control the memory size of the hashtable, Item pager thread will monitor the memory utilization of the hashtable.  Once a high watermark is reached, it will initiate an eviction process to remove certain document content from the hashtable.  Only entries
 that is not referenced by entries in the checkpoint buffer can be evicted because otherwise the outstanding update (which only exists in hashtable but not persisted) will be lost. 
After eviction, the entry of the document still remains in the hashtable; only the document content of the document will be removed from memory but the metadata is still there.  The eviction process stops after reaching the low watermark.  The high / low water
 mark is determined by the bucket memory quota. By default, the high water mark is set to 75% of bucket quota, while the low water mark is set to 60% of bucket quota. These water marks can be configurable at runtime.
In CouchDb, every document is associated with an expiration time and will be deleted once it is expired.  Expiry pager is responsible for tracking and removing expired document from both the hashtable as well as the storage engine (by scheduling a delete operation). 
Checkpoint Manager 
Checkpoint manager is responsible to recycle the checkpoint buffer, which holds the outstanding update request, consumed by the two downstream processes, Flusher and TAP replicator.  When all the request in the checkpoint buffer has been  processed, the checkpoint
 buffer will be deleted and a new one will be created.
TAP Replicator
TAP replicator is responsible to handle vBucket migration as well as vBucket replication from active server to replica server.  It does this by propagating the latest modified document to the corresponding replica server.
At the time a replica vBucket is established, the entire vBucket need to be copied from the active server to the empty destination replica server as follows
- 
The in-memory hashtable at the active server will be transferred to the replica server.  Notice that during this period, some data may be updated and therefore the data set transfered to the replica can be inconsistent (some are the latest and some are outdated).
- 
Nevertheless, all updates happen after the start of transfer is tracked in the checkpoint buffer.
- 
Therefore, after the in-memory hashtable transferred is completed, the TAP replicator can pickup those updates from the checkpoint buffer.  This ensures the latest versioned of changed documents are sent to the replica, and hence fix the inconsistency.
- 
However the hashtable cache doesn’t contain all the document content.  Data also need to be read from the vBucket file and send to the replica.  Notice that during this period, update of vBucket will happen in active server.  However, since the file is appended
 only, subsequent data update won’t interfere the vBucket copying process.
After the replica server has caught up, subsequent update at the active server will be available at its checkpoint buffer which will be pickup by the TAP replicator and send to the replica server.
### CouchDB Storage Structure
Data server defines an interface where different storage structure can be plugged-in.  Currently it supports both a SQLite DB as well as CouchDB.  Here we describe the details of CouchDb, which provides a super high performance storage mechanism underneath
 the Couchbase technology.
Under the CouchDB structure, there will be one file per vBucket.  Data are written to this file in an append-only manner, which enables Couchbase to do mostly sequential writes for update, and provide the most optimized access patterns for disk I/O.  This unique
 storage structure attributes to Couchbase’s fast on-disk performance for write-intensive applications.
The following diagram illustrate the storage model and how it is modified by 3 batch updates (notice that since updates are asynchronous, it is perform by "Flusher" thread in batches).
![](http://2.bp.blogspot.com/-2CZM0d76Dgw/T_Xl008qbRI/AAAAAAAAAxc/au7g8bDDe28/s400/p2.png)
The Flusher thread works as follows:
1) Pick up all pending write request from the dirty queue and de-duplicate multiple update request to the same document.
2) Sort each request (by key) into corresponding vBucket and open the corresponding file
3) Append the following into the vBucket file (in the following contiguous sequence)
- 
All document contents in such write request batch.  Each document will be written as [length, crc, content] one after one sequentially.
- 
The index that stores the mapping from document id to the document’s position on disk (called the BTree by-id)
- 
The index that stores the mapping from update sequence number to the document’s position on disk.  (called the BTree by-seq)
The by-id index plays an important role for looking up the document by its id.  It is organized as a B-Tree where each node contains a key range.  To lookup a document by id, we just need to start from the header (which is the end of the file), transfer to
 the root BTree node of the by-id index, and then further traverse to the leaf BTree node that contains the pointer to the actual document position on disk.
During the write, the similar mechanism is used to trace back to the corresponding BTree node that contains the id of the modified documents.  Notice that in the append-only model, update is not happening in-place, instead we located the existing location and
 copy it over by appending.  In other words, the modified BTree node will be need to be copied over and modified and finally paste to the end of file, and then its parent need to be modified to point to the new location, which triggers the parents to be copied
 over and paste to the end of file.  Same happens to its parents’ parent and eventually all the way to the root node of the BTree.  The disk seek can be at the O(logN) complexity.
The by-seq index is used to keep track of the update sequence of lived documents and is used for asynchronous catchup purposes.  When a document is created, modified or deleted, a sequence number is added to the by-seq btree and the previous seq node will be
 deleted.  Therefore, for cross-site replication, view index update and compaction, we can quickly locate all the lived documents in the order of their update sequence.   When a vBucket replicator asks for the list of update since a particular time, it provides
 the last sequence number in previous update, the system will then scan through the by-seq BTree node to locate all the document that has sequence number larger than that, which effectively includes all the document that has been modified since the last replication.
As time goes by, certain data becomes garbage (see the grey-out region above) and become unreachable in the file.  Therefore, we need a garbage collection mechanism to clean up the garbage.  To trigger this process, the by-id and by-seq B-Tree node will keep
 track of the data size of lived documents (those that is not garbage) under its substree.  Therefore, by examining the root BTree node, we can determine the size of all lived documents within the vBucket.  When the ratio of actual size and vBucket file size
 fall below a certain threshold, a compaction process will be triggered  whose job is to open the vBucket file and copy the survived data to another file.
Technically, the compaction process opens the file and read the by-seq BTree at the end of the file.  It traces the Btree all the way to the leaf node and copy the corresponding document content to the new file.  The compaction process happens while the vBucket
 is being updated.  However, since the file is appended only, new changes are recorded after the BTree root that the compaction has opened, so subsequent data update won’t interfere with the compaction process.  When the compaction is completed, the system
 need to copy over the data that was appended since the beginning of the compaction to the new file.
### View Index Structure
Unlike most indexing structure which provide a pointer from the search attribute back to the document.  The CouchDb index (called View Index) is better perceived as a denormalized table with arbitrary keys and values loosely associated to the document.
Such denormalized table is defined by a user-provided map() and reduce() function.
[view
 source](http://architects.dzone.com/articles/couchbase-architecture-deep#viewSource)
[print](http://architects.dzone.com/articles/couchbase-architecture-deep#printSource)[?](http://architects.dzone.com/articles/couchbase-architecture-deep#about)
`01.`
```
map
 =
```
`function`
```
(doc)
 {
```
`02.``…`
`03.`
```
emit(k1,
 v1)
```
`04.``…`
`05.`
```
emit(k2,
 v2)
```
`06.``…`
`07.``}`
`08.`
`09.`
```
reduce
 =
```
`function`
```
(keys,
 values, isRereduce) {
```
`10.``if`
```
(isRereduce)
 {
```
`11.`
```
//
 Do the re-reduce only on values (keys will be null)
```
`12.``} ``else``{`
`13.`
```
//
 Do the reduce on keys and values
```
`14.``}`
`15.`
```
//
 result must be ready for input values to re-reduce
```
`16.`
`17.``return``result`
`18.``}`
Whenever a document is created, updated, deleted, the corresponding map(doc) function will be invoked (in an asynchronous manner) to generate a set of key/value pairs.  Such key/value will be stored in a B-Tree structure.  All the key/values pairs of each B-Tree
 node will be passed into the reduce() function, which compute an aggregated value within that B-Tree node.  Re-reduce also happens in non-leaf B-Tree nodes which further aggregate the aggregated value of child B-Tree nodes.
The management server maintains the view index and persisted it to a separate file.
Create a view index is perform by broadcast the index creation request to all machines in the cluster.  The management process of each machine will read its active vBucket file and feed each surviving document to the Map function.  The key/value pairs emitted
 by the Map function will be stored in a separated BTree index file.  When writing out the BTree node, the reduce() function will be called with the list of all values in the tree node.  Its return result represent a partially reduced value is attached to the
 BTree node.
The view index will be updated incrementally as documents are subsequently getting into the system.  Periodically, the management process will open the vBucket file and scan all documents since the last sequence number.  For each changed document since the
 last sync, it invokes the corresponding map function to determine the corresponding key/value into the BTree node.  The BTree node will be split if appropriate.
Underlying, Couchbase use a back index to keep track of the document with the keys that it previously emitted.  Later when the document is deleted, it can look up the back index to determine what those key are and remove them.  In case the document is updated,
 the back index can also be examined; semantically a modification is equivalent to a delete followed by an insert.
The following diagram illustrates how the view index file will be incrementally updated via the append-only mechanism.
![](http://2.bp.blogspot.com/-8M0EpXZF3Ms/T_YxdY4M5aI/AAAAAAAAAxo/LKkgChgfR_g/s400/p1.png)
### Query Processing
Query in Couchbase is made against the view index.  A query is composed of the view name, a start key and end key.  If the reduce() function isn’t defined, the query result will be the list of values sorted by the keys within the key range.  In case the reduce()
 function is defined, the query result will be a single aggregated value of all keys within the key range.
![](http://4.bp.blogspot.com/-7ECdkkBEs24/T_Y5w4JxIZI/AAAAAAAAAx0/94K6hwrjqSQ/s400/p1.png)
If the view has no reduce() function defined, the query processing proceeds as follows:
- 
Client issue a query (with view, start/end key) to the management process of any server (unlike a key based lookup, there is no need to locate a specific server).
- 
The management process will broadcast the request to other management process on all servers (include itself) within the cluster.
- 
Each management process (after receiving the broadcast request) do a local search for value within the key range by traversing the BTree node of its view file, and start sending back the result (automatically sorted by the key) to the initial server.
- 
The initial server will merge the sorted result and stream them back to the client.
 However, if the view has reduce() function defined, the query processing will involve computing a single aggregated value as follows:
- 
Client issue a query (with view, start/end key) to the management process of any server (unlike a key based lookup, there is no need to locate a specific server).
- 
The management process will broadcast the request to other management process on all servers (include itself) within the cluster.
- 
Each management process do a local reduce for value within the key range by traversing the BTree node of its view file to compute the reduce value of the key range.  If the key range span across a BTree node, the pre-computed of the sub-range can be used. 
 This way, the reduce function can reuse a lot of partially reduced values and doesn’t need to recomputed every value of the key range from scratch.
- 
The original server will do a final re-reduce() in all the return value from each other servers, and then passed back the final reduced value to the client.
To illustrate the re-reduce concept, lets say the query has its key range from A to F.
![](http://1.bp.blogspot.com/-YXqRMwbgemU/T_Y6I_h9RhI/AAAAAAAAAx8/vntg8o9-oP0/s400/p2.png)
Instead of calling reduce([A,B,C,D,E,F]), the system recognize the BTree node that contains [B,C,D] has been pre-reduced and the result P is stored in the BTree node, so it only need to call reduce(A,P,E,F). 
Update View Index as vBucket migrates
Since the view index is synchronized with the vBuckets in the same server, when the vBucket has migrated to a different server, the view index is no longer correct; those key/value that belong to a migrated vBucket should be discarded and the reduce value cannot
 be used anymore.
To keep track of the vBucket and key in the view index, each bTree node has a 1024-bitmask indicating all the vBuckets that is covered in the subtree (ie: it contains a key emitted from a document belonging to the vBucket).  Such bit-mask is maintained whenever
 the bTree node is updated.
At the server-level, a global bitmask is used to indicate all the vBuckets that this server is responsible for.
In processing the query of the map-only view, before the key/value pair is returned, an extra check will be perform for each key/value pair to make sure its associated vBucket is what this server is responsible for.
When processing the query of a view that has a reduce() function, we cannot use the pre-computed reduce value if the bTree node contains a vBucket that the server is not responsible for.  In this case, the bTree node’s bit mask is compared with the global bit
 mask.  In case if they are not aligned, then the reduce value need to be recomputed.
Here is an example to illustrate this process
![](http://1.bp.blogspot.com/-x5ECjyCJ50Y/T_Y8xU0visI/AAAAAAAAAyI/pVR7Mz0uJ-s/s400/p1.png)
Couchbase is one of the popular NOSQL technology built on a solid technology foundation designed for high performance.  In this post, we have examined a number of such key features:
- 
Load balancing between servers inside a cluster that can grow and shrink according to workload conditions.  Data migration can be used to re-achieve workload balance.
- 
Asynchronous write provides lowest possible latency to client as it returns once the data is store in memory.
- 
Append-only update model pushes most update transaction into sequential disk access, hence provide extremely high throughput for write intensive applications.
- 
Automatic compaction ensures the data lay out on disk are kept optimized all the time.
- 
Map function can be used to pre-compute view index to enable query access.  Summary data can be pre-aggregated using the reduce function.  Overall, this cut down the workload of query processing dramatically.
For a review on NOSQL architecture in general and some theoretical foundation, I have wrote a[NOSQL
 design pattern](http://horicky.blogspot.com/2009/11/nosql-patterns.html) blog, as well as [some
 fundamental difference between SQL and NOSQL.](http://horicky.blogspot.com/2010/05/nosql-debate.html)
For other NOSQL technologies, please read my other blog on [MongoDb](http://horicky.blogspot.com/2012/04/mongodb-architecture.html), [Cassandra
 and HBase](http://horicky.blogspot.com/2010/10/bigtable-model-with-cassandra-and-hbase.html),[Memcached](http://horicky.blogspot.com/2009/10/notes-on-memcached.html)
Special thanks to Damien Katz and Frank Weigel from Couchbase team who provide a lot of implementation details of Couchbase.
Published at DZone with permission of [Ricky
 Ho](http://architects.dzone.com/users/riho), author and DZone MVB. ([source](http://horicky.blogspot.com/2012/07/couchbase-architecture.html))
(Note: Opinions expressed in this article and its replies are the opinions of their respective authors and not those of DZone, Inc.)
Tags: 
- [Couchbase](http://architects.dzone.com/category/tags/couchbase)
- [Tutorial](http://architects.dzone.com/category/content-form/tutorials)
- [NoSQL](http://architects.dzone.com/category/dzone-taxonomy/architecture/nosql)
