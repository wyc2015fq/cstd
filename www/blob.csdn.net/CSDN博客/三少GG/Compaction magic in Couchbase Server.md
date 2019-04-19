# Compaction magic in Couchbase Server - 三少GG - CSDN博客
2014年09月16日 21:54:08[三少GG](https://me.csdn.net/scut1135)阅读数：952
个人分类：[云计算/大数据](https://blog.csdn.net/scut1135/article/category/1449467)

[http://blog.couchbase.com/compaction-magic-couchbase-server-20](http://blog.couchbase.com/compaction-magic-couchbase-server-20)
# Compaction magic in Couchbase Server 2.0

With Couchbase’s [append-only
 storage design](http://damienkatz.net/2012/05/stabilizing_couchbase_server_2.html), it’s impossible to corrupt data and index files as updates go only to the end of the file. There are no in-place file updates and the files are never in an inconsistent state. But writing to
 an ever-expanding file will eventually eat up all your diskspace. Therefore, Couchbase server has a process called compaction. Compaction cleans up the disk space by removing stale data and index values so that the data and index files don’t unnecessarily
 eat up your disk space.  If your app’s use-case is mostly-reads, this maybe OK but if you have write-heavy workloads, you may want to learn about how auto-compaction works in Couchbase Server. 
By design, documents in Couchbase Server are partitioned into vBuckets (or partitions). There are multiple files used for storage – a data file per partition (the “data files”), multiple index-files (active,
 replica and temp) per design document and a master file that has metadata related to the design documents and view definitions. For example on Mac OSX (as shown below), the sample ‘gamesim’ bucket has [64
 individual data files](http://www.couchbase.com/docs/couchbase-manual-2.0/couchbase-getting-started-prepare-platforms.html), one per partition (0.couch.1 to 63.couch.1), and a master file that has design documents and other view metadata (master.couch.1)
![](http://blog.couchbase.com/sites/default/files/uploads/all/images/Screen%20Shot%202013-02-18%20at%2012.28.24%20PM.png)
**Couchbase Data and Master File**
The index files are in the @indexes folder and consist of the active index file starting with main_, the replica index file (if index replication is enabled) starting with replica_ and a temporary file that is used
 while building and updating the index starting with tmp_.
![](http://blog.couchbase.com/sites/default/files/uploads/all/images/Screen%20Shot%202013-02-18%20at%2012.28.37%20PM.png)
**Index Files in Couchbase Server**
Data and index files in Couchbase Server are organized as b-trees.  The root nodes (shown in red) contains pointers to the intermediate nodes, which contain pointers to the leaf nodes (shown in blue). In the case
 of data files, the root and intermediate nodes track the sizes of documents under their sub-tree.  The leaf nodes store the document id, document metadata and pointers to the document content. For index files, the root and intermediate nodes track the outputted
 map-function result and the reduce values under their subtree.
![](http://blog.couchbase.com/sites/default/files/uploads/all/images/Screen%20Shot%202013-02-18%20at%2012.28.49%20PM.png)
**B-tree storage for data files (shown on the left) and index files (shown on the right)**
All mutations including inserts, updates and deletes are written to the end of the file leaving old data in place. Add a new document? The b-tree grows at the end. Delete a document? That gets recorded at the end of the b-tree. For example, as shown in the
 Figure below, document A is mutated followed by a mutation to document B and then a new document D is added followed by another mutation to document A. Old data is shown by the red crossed-out nodes in the Figure below.
![](http://blog.couchbase.com/sites/default/files/uploads/all/images/Screen%20Shot%202013-02-18%20at%2012.28.59%20PM.png)
**Logical data layout for data and index files**
By examining the size of the documents tracked by the root node in the b-tree file structure, the ratio of the actual size to the current size of the file is calculated. If this ratio hits a certain threshold
 that is configurable as shown in the Figure below, an online compaction process is triggered. Compaction scans through the current data and index files and creates new data and index files, without the items marked for cleanup. During compaction, the b-trees
 are balanced and the reduce values are re-computed for the new tree. Additionally, data that does not belong to a particular node is also cleaned up. 
Finally to catch-up with the active workload that might have changed the old partition data file during compaction, Couchbase copies over the data that was appended since the start of the compaction process
 to the new partition data file so that it is up-to date. The new index file is also updated in this manner. The old partition data and index file are then deleted and the new data and index files are used. 
 Normally, compaction is an all-or-nothing operation but since compaction in Couchbase is on a per partition (vbucket) basis, the dataset can be compacted
 incrementally without losing any changes it has made when aborted.
![](http://blog.couchbase.com/sites/default/files/uploads/all/images/Screen%20Shot%202013-02-18%20at%2012.29.09%20PM.png)
**Configuring the compaction thresholds in the settings UI tab for data  and index files **
Compaction in Couchbase Server is an online operation. By default, auto-compaction kicks in when the fragmentation threshold reaches 30%, but you
 should test what settings works well for your workload and tune this setting accordingly.
![](http://blog.couchbase.com/sites/default/files/uploads/all/images/Screen%20Shot%202013-02-18%20at%2012.29.18%20PM.png)
Because compaction is a resource intensive you can also schedule it during off-peak hours. To prevent auto compaction from taking place when your database is in heavy use, you can configure an off-peak
 time period during which compaction is allowed using the UI shown above.  For example, here I’ve set compaction to run between 12am and 1am server time every day. If the compaction operation does not complete in this time period, it will continue, but you
 can check the box to have it aborted. 
Compaction can also be triggered manually per bucket or per design document as shown in the Figures below.
![](http://blog.couchbase.com/sites/default/files/uploads/all/images/Screen%20Shot%202013-02-18%20at%2012.29.25%20PM.png)
**Manually compacting a data bucket in Couchbase**
![](http://blog.couchbase.com/sites/default/files/uploads/all/images/Screen%20Shot%202013-02-18%20at%2012.29.38%20PM.png)
**Manually compacting a design document in Couchbase**
Compaction performance in Couchbase Server depends on IO capacity and [proper
 cluster sizing](http://blog.couchbase.com/how-many-nodes-part-1-introduction-sizing-couchbase-server-20-cluster). Your cluster must be properly sized so that there is enough capacity in all the various areas to support everything else the system is doing to maintain the required level of performance. So,
 how do you tune compaction in Couchbase Server?
There is no magic bullet here ... Depending on your application’s IOPS requirement, you need to size your cluster properly and might want to test your workload across a variety of different storage hardware.
 If your app is write heavy, SSD’s might be the best option but for heavy read ratios, EBS might be a good solution at a low cost. 
By default, if both data and view indexes are configured for auto-compaction, compaction operates sequentially, first on the database and then on the views.  By enabling parallel compaction, both the databases
 and views can be compacted at the same time. This requires more CPU and disk I/O, but if the database and view indexes are stored on different physical disk devices ([as
 is our best practice anyway](http://www.couchbase.com/docs/couchbase-manual-2.0/best-practice-guide.html)), the two can complete in parallel so that the index and data files does not grow extremely large.
Conclusion
At the end of the day, every database needs regular maintenance. Online compaction is a huge plus but you have to test your system and configure your compaction settings appropriately so that it does not affect
 your system load. 

