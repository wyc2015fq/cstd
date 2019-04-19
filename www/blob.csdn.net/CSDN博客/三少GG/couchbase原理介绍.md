# couchbase原理介绍 - 三少GG - CSDN博客
2014年08月01日 17:09:49[三少GG](https://me.csdn.net/scut1135)阅读数：2446
http://www.couchbase.com/wiki/display/couchbase/Couchbase+View+Engine+Internals
# [Couchbase View Engine Internals](http://www.couchbase.com/wiki/display/couchbase/Couchbase+View+Engine+Internals)
[](http://www.couchbase.com/wiki/display/couchbase/Couchbase+View+Engine+Internals#page-metadata-end)
- 
- [Attachments:6](http://www.couchbase.com/wiki/pages/viewpageattachments.action?pageId=14516232&metadataLink=true)
- 
Added by [Sarath Lakshman](http://www.couchbase.com/wiki/display/~sarath), last edited by [Sarath
 Lakshman](http://www.couchbase.com/wiki/display/~sarath) on Mar 07, 2014  ([view
 change](http://www.couchbase.com/wiki/pages/diffpages.action?pageId=14516232&originalId=14516243))
[](http://www.couchbase.com/wiki/display/couchbase/Couchbase+View+Engine+Internals#page-metadata-start)
## Overview
This document describes view index engine internals. The focus is to provide a brief description of various components inside the view engine and their interactions. The view engine is co-located with KV server in all the nodes. The cluster manager notifies
 view engine about relevent state changes happening within the cluster and view engine responds accordingly. Each view engine instance is responsible for indexing of hash partitioned data that belongs to each of the nodes.
![](http://www.couchbase.com/wiki/download/attachments/14516232/view-engine.png?version=3&modificationDate=1394184693409)
The component of view engine responsible for an individual bucket data is called set view server. It can hold many design documents and each design document can contain many views. Each design document is considered as a related set of views. Each set view
 server owns multiple group servers (one corresponding to each design document). Group server owns all the operations of indexes belonging to a design document. The set view server delegates all the incoming requests to right group server. All the views related
 to a single design document is stored in a single index file (one per design document and per type of index - active/replica).
The configurations for view indexes can be setup per design document level.
In a high level overview, the three major tasks that view index engine is carried out are index updation, query handling and consistent index housekeeping. The indexes are updated per design document level and handled by its group server. ns server is responsible
 for triggering updates by time duration or number of changes threshold. The query happens in a distributed manner. Query request can be issued to any node in the cluster and the node that receives the query initiates internal sub query across all the nodes
 in the cluster, streams the results from all the nodes and merge the results on-the-fly to produce sorted result. The query initiator node streams the result as they arrive from internal query requests. The load balancing should be performed by the query client
 by randomly hitting nodes in the cluster. The index ondisk structures used are append only format and hence needs frequent housekeeping to reduce the fragmentation. The ns server is responsible for triggering compaction jobs on index structures by fragmentation
 threshold. When rebalance occur (any vbucket movements in the cluster nodes), we need to remove the corresponding data belonging to the vbuckets which got moved out of the current node from the index structures. We have cleaner jobs which operates on the index
 structures to remove data belonging to cleanup vbuckets.
## Details on index structures
The design document definitions are store in a metadata file default/master.couch.1. The index files are stored in @indexes/default (default is the bucket name) directory. One file per design document and index type is used. Files are
 named similar to the following examples:
main_da1eaf6fac28abafd16daa38c3bbbfd7.view.1
replica_da1eaf6fac28abafd16daa38c3bbbfd7.view.1 
The main_ prefix signifies that it is an active index and replica_ signifies that it is a replica index for a design document. The hex digest followed by the index type is called index signature. It is calculated by applying md5sum on design document definition.
 If multiple design documents are defined with same definition, it will create only one index file (since they have same signature). The view engine also facilities to have multiple design document definitions to be present at a time for query request handling.
 For example, if a ddoc definition is changed while a query is under processing, the query can operate on older file (different signature) and new index file can be created independently without affecting the running query request.
Each design document’s index file contains a back index btree and one or more view index btrees. The couch btree used for index is an append only btree and it is creates a snapshot on each btree insert operation (one snapshot per bulk insert). Latest snapshot
 information can be obtained by reading from the end of the index file backwards by locating a valid header information. Header information is block aligned and it has its first byte non-zero. While all other blocks written in the index file has first byte
 as zero.
The header contains necessary information related to the view btrees and id btree stored in the index file as well as few other meta data information as follows:
ddoc signature, number of views, number of vbuckets, active bitmask, passive bitmask, cleanup bitmask, id btree root file offset, view btrees root file offset and vbucket seqence numbers.
In order to retrieve data from a view, we should first locate a valid header. From the header, we can locate the the file offset position from which we can read the root node for a view btree. Views are addressed by number rather than its name (starting from
 0). Once you read the root node from the corresponding file offset position, we can read its child nodes by offset information of child nodes retrieved from the root node. The same procedure can be applied to locate any node that particular btree. 
The couch btree is a COW btree structure. Every time when you mutate any node in the tree, we need to append that node in the file as well as all its parent nodes up to the root node. Hence the same file may contain many instances of btree point in time. By
 obtaining different versions of root offset position, you can read all the snapshots of the index point in time.
### Map and Reduce functions
The view btree’s key is the same as the key emitted by the map javascript function. Map is applied and its results are used to insert into the btrees. Each btree node has a reduce value field which stores reduce value obtained by applying reduce function in
 its subtree nodes.
The reduce function is applied at the time of btree writes into the file. We have reduce and rereduce javascript functions. The reduce function is used to obtain a reduce value from key value pairs obtained by applying map function. It is used to obtain reduce
 value of a KV node.
The rereduce function is used to combine existing reduce values to obtain combined reduce value. In the case of btree, rereduce is used to calculate reduce value in a KP node. 
To obtain reduce value of a view, you just need to read the root node and get its reduce field.
### vBucket bitmasks (Partition bitmasks)
Each node in a cluster is mapped with many vbuckets and view engine is responsible for indexing all the data belonging to the vbuckets for that node. Even though data is partitioned into multiple vbuckets, we do not use separate btree per vbucket. Instead data
 belonging to all the vbuckets for that node is inserted into same btree index. But we tag each item inserted into the btree with that vbucket-id. Since vbuckets are moved in and out between the nodes by the cluster manager wrt rebalance and failover scenarios,
 we need a flexible scheme to remove and add data belonging to new vbuckets to the index. We use a vbucket bitmask per node in the btree which contains 1024 bits. The bits in the bitarray is set according to whether a vbucket’s data is present in the subtree
 of that node. For KP-node, vbucket bitmask = union of all vbucket bitmasks of its child nodes. For KV-node, its the union of all vbuckets of data present in that node.
This bitmask is very helpful in determining if a node’s subtree contains a given vbucket’s data or not. During rebalance, when few vbuckets are moved out from the current node the view engine can easily remove the data belonging to the vbuckets from the btree
 by conditionally traversing down the subtrees. If this bitmask was not present, it requires to rebuild the btree by filtering out the entries belonging to given vbuckets. The vbucket bitmask is calculated while btree nodes are being written to the disk.
## Index updater operation
The index updater is a component within the view engine which can be called on demand to update the indexes up to date with the KV server. Updater is part of design doc group server and one updater instance per group server. The updater is triggered by stale=false
 query or the periodic trigger update daemon by the ns server. The updater loads a batch of mutations that happened between last indexed sequence and latest mutation sequence and updates the view btrees.
![](http://www.couchbase.com/wiki/download/attachments/14516232/view-engine+updater.png?version=1&modificationDate=1394184452358)
The updater has mainly three components Loader, Mapper and Writer. They operate in a pipeline fashion. The loader is responsible for collecting a batch of changes from last indexed sequence of mutation to the latest mutation sequence and load it into a queue.
 The updater stores the highest sequence number for every partition it has already indexed . This way it's easy to find out whether an update it needed or not. The updater loops through all partitions and requests all the updates that happened since the last
 time the updater ran.
The mapper process dequeues the mutations in sequence, applies map function and enqueues into the writer queue. The writer queue contains one item per updated document (if there were no errors). The value of each item is a list of result from every map function
 of the design document.
The map function for all the views within the design document is applied and the result is an array of map results for each views.
The btree updates are efficient for bulk updates since it will result only in writing a single snapshot. If we were to do use single btree operation per item, it will result in that many snapshot being created and lot of storage space being consumed due to
 append only design. To apply bulk inserts, the keys should be in sorted order. We create a sort records temporary file and perform sorting before doing the actual btree updates.
### Back index
In view index, we keep a back index btree per design document. The purpose of back index is to identify what was the previous key generated from a given document. Since we need to insert to the view index btree, before that we need to remove previous entry
 of indexed key from the view btree if a modification occurs to the document.
For eg,
We have a view index which maps city as the key.
Say, following mutations occur for a document doc1.
Mutation 1:
doc1 => {city:Bangalore, id:100}
Now we will add an item with key [Bangalore, doc1] to the view index.
Mutation 2:
doc1 => {city:Hyderabad, id:100}
Now we need to update the view index, by removing earlier key [Bangalore, doc1] and add new item with key [Hyderabad, doc1].
How do we know what was its previous key ?
Use a separate btree which stores docId => key mapping. This is called back index.
So every time a mutation arrives, we can lookup the previous key by using its documentId from the back index and update back index with new key for the document. The back index btree contains key value in the following format:
documentId => [view0-key, view1-key, view2-key …]
To apply bulk updates, we have operation records temporary file per view and one for back index btree. 
The writer process reads a batch of items (buffer size) from the writer queue and performs the following set of operations.
1. Lookup in the back index and get the previous key if any
2. For each view and back index, place the btree operation entry in the corresponding sort file.
The sort record file will look something as follows:
view btree sort record file:
remove key:Bangalore
insert key:Hyderabad, val: Something
insert key:Mumbai, val: SomethingElse
back index sort record file:
remove key:doc1
insert key:doc1 val:Hyderabad
remove key:doc2
insert key:doc2, val:Mumbai
Once these sort files are created for the given batch, it is processed by an external sorting component, which sorts these files with 64MB inmemory buffer. Sorted records will be bulk inserted into the corresponding view btree in the index file and appends
 a new index header with new root node information for all btrees.
### Initial index builder and incremental updates
Within the updater we have two types of update. Initial index build and incremental update. If an index for a design document is build for the first time, it is called initial index build. In the case of initial index build, the writer do not batch the incoming
 mutations, all mutations written into the sort record files and is used to build the initial view btrees. The btree is build in bottom up fashion and is very efficient (Refer: [http://stackoverflow.com/a/15996616](http://stackoverflow.com/a/15996616))
The further changes that happens after first time index build are called incremental updates. For incremental updates, we batch our changes in size of 1MB and applies to the index. The 1MB size batch is used so that the queries will see updates applied to btree
 at shortest intervals. The batch updates within the incremental update phase is called checkpointing.
Each time when a update to the index is made, the index header is updated with the latest mutation sequence numbers for all the vbuckets. This sequence set is useful for identifying from which sequences we need to start reading mutations for the next incremental
 update.
## View query operation
The query for view engine operates in a distributed manner. The node that is receiving the query request acts as distributed query coordinator and broadcasts the query requests to all the other nodes. A k-way merge queue is used to enqueue streaming results
 from the nodes. Each node gets a request handler process which reads the query rows on arrival from http response and enqueues to the k-way merge queue. The k-way merge queue blocks the enqueue operation by each process based on k-way mergesort property. The
 results coming out of k-way merge queue is sort ordered and they are streamed to query response as they arrive.
![](http://www.couchbase.com/wiki/download/attachments/14516232/query.png?version=3&modificationDate=1394185086783)
![](http://www.couchbase.com/wiki/download/attachments/14516232/query2.png?version=1&modificationDate=1394185092088)
The query server in each node is responsible for retrieving data from the view btrees. When a query request arrives, the query server will request for the latest view group information from the view group server corresponding to its design document. The group
 info request is a blocking request. When stale = false, it will block until current vbuckets seq num <= indexed seqs num by triggering an updater in the background. 
The view group information contains details about the view index file path, file offset location for view btree root node, etc. The query server walks down that btree on disk to obtain results based on query parameter like ranges, limit, etc.
### Rebalance and consistent views
A rebalance is needed if the topology of the cluster changes, by adding/removing/exchanging nodes. Rebalance is the process of distributing the partitions evenly across the cluster after a topology change. When vbuckets are redistributed across the cluster,
 the view engine also has to perform rebalancing of data stored in the indexes. During the rebalance, the query results in the cluster are always consistent. Let’s take a look at how view engine achieves consistent views.
### Partitions in view engine
Couchbase splits the key space into a fixed amount of partitions(vBuckets), usually 1024. That is, keys are deterministically assigned to a partition, and partitions are assigned to nodes to balance load across the cluster.
### Partition states
![](http://www.couchbase.com/wiki/download/attachments/14516232/partition_state.png?version=1&modificationDate=1394184575367)
A partition within the view engine can be in either of the following states.
active:  Partition is indexed and the result will be returned on query time
passive: Partition is indexed, but will not be returned on query time
cleanup: Partition will be removed during the next cleanup cycle, hence they won't be returned on query time.
not-present: After cleanup a partition will not be part of the index any more, thus that partition will not appear in the query result.
The cleanup is a job that is run periodically to remove data belonging to the partitions which got moved out of current index.
### Queries during rebalance
Currently the indexers operated locally on every node in the cluster. If partitions are moved between nodes, the indexers need to be made aware of that change and either index that partition or remove it from the index.
The cluster manager handles the details on which partition to move/add/delete and the view engine just performs such requests. The concept of indexable and unindexable partitions is needed here. It's independent of the state the partition is in:
indexable: That's the default state, the partition acts as described above.
unindexable: When set to unindexable, the indexing is stopped. A view query will return the results that are currently indexed.
With the possibility to mark partitions unindexable it's possible to have consistent views during rebalance. When a partition is handed over to a new server, the partition is marked as passive (and indexable) on the new server. It means that it will start indexing
 right away, but a query won't take it into account. On the old server the partition will be set to unindexable, that it won't receive any updates, only the partition on the new server will. Once the migration of the partition is complete and it is fully indexed,
 the partition is changed to active on the new server and marked for cleanup on the old one.
No matter when a query happens, it will always return the correct result, either from the old node, or after the migration from the new node.
This diagram shows the migration of one partition from one node to another during rebalance. Only states that change are listed.
|Events|Partition state on Node A|Partition state on Node B|
|----|----|----|
|Initial state|active, indexable|not-present|
|Start migration of partition from Node A to Node B|active, indexable|passive, indexable|
|Destination starts indexing newly added partition|active, unindexable|passive, indexable|
|Destination completed indexing the newly added partition|cleanup|active, indexable|
### Replica partitions
The view engine supports optional replica partitions similar to replicas supported by the KV server. When a node which holds the active partition goes down, the cluster manager needs to promote the available replica partitions to active partitions. This process
 is called failover.
When you failover a node, the partitions from that node are not accessible anymore. The corresponding replicas that are split across the cluster will become active and serve the missing data. The end user will still get all the data back he expects.
This failover does not only work for your data partitions, but there are also so-called "index replicas". That means that your indexed data also has replicas throughout the cluster that can be activated once a node is failed over. The view engine has no knowledge
 on what should happen in case of a failover, it just follows the instructions it gets from the cluster manager.
The replica partitions have the same semantics as the main partitions, hence everything written above also applies to the replica partitions.
### Queries during failover
Let's look at a single server, where some of the replica partitions should be used because of a failover.
Just as during rebalance, the cluster manager will tell the view engine to use those replica partitions from now on when a query was made. This means that all queries will still contain all the data the end user would expect to be returned.
Internally (not visible for the cluster manager) the view engine will now dynamically query those now active replica partitions in addition to the active main partitions. As querying two indexes and merging the results dynamically naturally results in a slowdown,
 the view engine transfers those now active replica partitions into the main index (replica design document index file to active design document file). The transfer works like this: The partition is added to the main index as passive partition. Once the indexing
 is completed it will be switched to active and removed from the list of replicas on transfer.
|Events|Partition state on Node A|Partition state on Node B|
|----|----|----|
|Initial state|main index: active, indexablereplica index: not-present|main index: not-presentreplica index: passive|
|Failover initiated|not available|main index: not-presentreplica index: active, unindexable|
|Started indexing partition on main index.|not available|main index: passive, indexablereplica index: active, unindexable|
|Indexing on main index completed|not available|main index: active, indexablereplica index: not-present|
## Index compaction in view engine
The view engine performs frequent updates to the index structures to keep upto date with the KV node data. As time passes, the fragmentation in the index file increases and it is required to compact the index files. The couch btrees follows an append only design
 and hence every update to the btree results in copy-on-write(COW) of one or nodes. All the btrees stores a meta data information about what is the total data size of the tree. To find out the fragmentation ratio, we can add up all the actual data sizes reported
 by all the btrees stored in an index file and compare it with the file size of that index file. If the fragmentation ratio exceeds the threshold value, view engine performs compaction of the index file. ![](http://www.couchbase.com/wiki/download/attachments/14516232/compactor.png?version=1&modificationDate=1394184545629)
The idea behind view index compaction is simple. Create a new empty file. For each btree from the current index file, iterate over all nodes and build a new btree in the new file. Once all btrees are rewritten, the group process for the corresponding design
 document will switch to the new index file and previous one is removed. Earlier in this document, we mentioned that the index file names look something like main_da1eaf6fac28abafd16daa38c3bbbfd7.view.1. The suffix “.1” signifies that it is the first version
 of the file. When the file is compacted and newone is created, it will increment that suffix number to become main_da1eaf6fac28abafd16daa38c3bbbfd7.view.2.
If the view engine sits idle, we can just compact the index file and switch to the new one. But view engine always server requests and updates can happen to the index file anytime. We cannot stop the view engine updates from happening. While doing the compaction,
 new changes would be happening to the current index file. Since we read the index header at an earlier point, we saw an earlier snapshot of all btrees. Unless newly created btrees are up to date with the current index, we cannot switch to the newly created
 compacted index file. 
As mentioned in the earlier part of this document, the updater uses a sort record file for performing bulk updates to the btrees. The sort record file is essentially a list of operations to be applied to the btrees. To solve the view compactor problem of source
 index file being updated while compaction is running, updater is made aware of compactor execution. If a compactor process exists, the updater will not remove the sort record files after applying changes to the current index, it keeps those temporary files
 for compactor to use. Updater sends a message to the compactor with the list of sort record files. Immediately after view compactor rewrites the current btrees to a new file, it will start performing delta changes application to the new index file. It reads
 the message sent by the updater and updates the new index files with those changes. Compactor repeats this process until the updater completes all batch of updates. Immediately after that, compactor instructs design doc group server to switch its index file
 to new compacted index file.
[view](http://www.couchbase.com/wiki/label/couchbase/view)
[engine](http://www.couchbase.com/wiki/label/couchbase/engine)
