# MySQL排序内部原理探秘 - Spark高级玩法 - CSDN博客
2018年12月31日 00:07:18[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：97
> 
**来源：沃趣科技**
**目录：**
一、我们要解决什么问题
二、排序，排序，排序
三、索引优化排序
四、排序模式
    4.1实际trace结果
    4.2排序模式概览
        4.2.1回表排序模式
        4.2.2不回表排序模式
        4.2.3打包数据排序模式
        4.2.4三种模式比较
五、外部排序
    5.1普通外部排序
        5.1.1两路外部排序
        5.1.2多路外部排序
    5.2MySQL外部排序
        5.2.1MySQL外部排序算法
        5.2.2sort_merge_passes
六、trace 结果解释
    6.1     是否存在磁盘外部排序
    6.2     是否存在优先队列优化排序
七、MySQL其他相关排序参数
    7.1max_sort_length
    7.2innodb_disable_sort_file_cache
    7.3innodb_sort_buffer_size
八、MySQL排序优化总结
九、参考文献
警示：不好意思，我们太想把排序的前前后后说清楚了，导致这是一篇长文！
**一、我们要解决什么问题**
MySQL排序其实是一个老生长谈的问题了，但是我们这次想由浅入深详细的说说MySQL排序模式，怎么影响MySQL选择不同的排序模式和怎么优化排序。
同时也希望通过这篇文章解决大家的以下疑问：
- 
MySQL在哪些地方会使用排序，怎么判断MySQL使用了排序
- 
MySQL有几种排序模式，我们可以通过什么方法让MySQL选择不同的排序模式
- 
MySQL排序跟read_rnd_buffer_size 有啥关系，在哪些情况下增加read_rnd_buffer_size能优化排序
- 
怎么判断MySQL使用到了磁盘来排序，怎么避免或者优化磁盘排序
- 
排序时变长字段（varchar）数据在内存是怎么存储的，5.7有哪些改进
- 
在limit情况下，排序模式有哪些改进
- 
sort_merge_pass到底是什么鬼，该状态值过大说明了什么问题，可以通过什么方法解决
- 
最后MySQL使用到了排序的话，依次可以通过什么办法分析和优化让排序更快
**二、排序，排序，排序**
我们通过explain查看MySQL执行计划的时候，经常会看到在Extra列中显示Using filesort。
其实这种情况就说明MySQL就使用了排序。
Using filesort经常出现在order by、group by、distinct、join等情况下。
**三、索引优化排序**
看到排序，我们的DBA首先想到的肯定是，是否可以利用索引来优化。
INNODB默认采用的是B tree索引，B tree索引本身就是有序的，如果有一个查询如下
> 
select * from film where actor_name='苍老师' order by prod_time;
那么只需要加一个(actor_name,prod_time)的索引就能够利用B tree的特性来避免额外排序。
如下图所示：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmBU8XAzuic4KwdYAUYcjvVDsNMBcDVXib4wicM5FKeiaQvVE6cEl2rv0slBwZIG2S12EsGhLsAf8pPzQ/640?wx_fmt=png)
通过B-tree查找到actor_name=’苍老师’演员为苍老师的数据以后，只需要按序往右查找就可以了，不需要额外排序操作
对应的哪些可以利用索引优化排序的列举如下：
> 
SELECT * FROM t1
  ORDER BY key_part1,key_part2,... ;
SELECT * FROM t1
  WHERE key_part1 = constant
  ORDER BY key_part2;
SELECT * FROM t1
  ORDER BY key_part1 DESC, key_part2 DESC;
SELECT * FROM t1
  WHERE key_part1 = 1
  ORDER BY key_part1 DESC, key_part2 DESC;
SELECT * FROM t1
  WHERE key_part1 > constant
  ORDER BY key_part1 ASC;
SELECT * FROM t1
  WHERE key_part1 < constant
  ORDER BY key_part1 DESC;
SELECT * FROM t1
  WHERE key_part1 = constant1 AND key_part2 > constant2
  ORDER BY key_part2;
从以上例子里面我们也可以看到，如果要让MySQL使用索引优化排序应该怎么建组合索引。
**四、排序模式**
**4.1 实际trace结果**
但是还是有非常多的SQL没法使用索引进行排序，例如
> 
select * from film where Producer like '东京热%'  and prod_time>'2015-12-01' order by actor_age;
我们想查询’东京热’出品的，从去年12月1号以来，并且按照演员的年龄排序的电影信息。
（好吧，假设我这里有一个每一位男DBA都想维护的数据库:）
这种情况下，使用索引已经无法避免排序了，那MySQL排序到底会怎么做列。
笼统的来说，它会按照：
- 
依据“Producer like ‘东京热%’  and prod_time>’2015-12-01’  ”过滤数据，查找需要的数据；
- 
对查找到的数据按照“order by actor_age”进行排序,并 按照“select *”将必要的数据按照actor_age依序返回给客户端。
空口无凭，我们可以利用MySQL的optimize trace来查看是否如上所述。
如果通过optimize trace看到更详细的MySQL优化器trace信息，可以查看阿里印风的博客初识5.6的optimizer trace
trace结果如下：
- 
依据“Producer like ‘东京热%’  and prod_time>’2015-12-01’  ”过滤数据，查找需要的数据
> 
              "attaching_conditions_to_tables": {
              "original_condition": "((`film`.`Producer` like '东京热%') and (`film`.`prod_time` > '2015-12-01'))",
              "attached_conditions_computation": [
              ],
              "attached_conditions_summary": [
                {
                  "table": "`film`",
                  "attached": "((`film`.`Producer` like '东京热%') and (`film`.`prod_time` > '2015-12-01'))"
                }
              ]
            }
- 
对查找到的数据按照“order by actor_age”进行排序,并 按照“select *”将必要的数据按照actor_age依序返回给客户端
> 
      "join_execution": {
        "select#": 1,
        "steps": [
          {
            "filesort_information": [
              {
                "direction": "asc",
                "table": "`film`",
                "field": "actor_age"
              }
            ],
            "filesort_priority_queue_optimization": {
              "usable": false,
              "cause": "not applicable (no LIMIT)"
            },
            "filesort_execution": [
            ],
            "filesort_summary": {
              "rows": 1,
              "examined_rows": 5,
              "number_of_tmp_files": 0,
              "sort_buffer_size": 261872,
              "sort_mode": "<sort_key, packed_additional_fields>"
            }
          }
        ]
      }
这里，我们可以明显看到，MySQL在执行这个select的时候执行了针对film表.actor_age字段的asc排序操作。
> 
"filesort_information": [
              {
                "direction": "asc",
                "table": "`film`",
                "field": "actor_age"
              }
**4.2 排序模式概览**
我们这里主要关心MySQL到底是怎么排序的，采用了什么排序算法。
请关注这里
> 
"sort_mode": "<sort_key, packed_additional_fields>"
MySQL的sort_mode有三种。
摘录5.7.13中sql/filesort.cc源码如下：
> 
  Opt_trace_object(trace, "filesort_summary")
    .add("rows", num_rows)
    .add("examined_rows", param.examined_rows)
    .add("number_of_tmp_files", num_chunks)
    .add("sort_buffer_size", table_sort.sort_buffer_size())
    .add_alnum("sort_mode",
               param.using_packed_addons() ?
               "<sort_key, packed_additional_fields>" :
               param.using_addon_fields() ?
               "<sort_key, additional_fields>" : "<sort_key, rowid>");
“< sort_key, rowid >”和“< sort_key, additional_fields >” 看过其他介绍介绍MySQL排序文章的同学应该比较清楚，“< sort_key, packed_additional_fields >” 相对较新。
- 
< sort_key, rowid >对应的是MySQL 4.1之前的“原始排序模式”
- 
< sort_key, additional_fields >对应的是MySQL 4.1以后引入的“修改后排序模式”
- 
< sort_key, packed_additional_fields >是MySQL 5.7.3以后引入的进一步优化的"打包数据排序模式”
下面我们来一一介绍这三个模式：
**4.2.1  回表排序模式**
- 
根据索引或者全表扫描，按照过滤条件获得需要查询的排序字段值和row ID；
- 
将要排序字段值和row ID组成键值对，存入sort buffer中；
- 
如果sort buffer内存大于这些键值对的内存，就不需要创建临时文件了。否则，每次sort buffer填满以后，需要直接用qsort(快速排序算法)在内存中排好序，并写到临时文件中；
- 
重复上述步骤，直到所有的行数据都正常读取了完成；
- 
用到了临时文件的，需要利用磁盘外部排序，将row id写入到结果文件中；
- 
根据结果文件中的row ID按序读取用户需要返回的数据。由于row ID不是顺序的，导致回表时是随机IO，为了进一步优化性能（变成顺序IO），MySQL会读一批row ID，并将读到的数据按排序字段顺序插入缓存区中(内存大小read_rnd_buffer_size)。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmBU8XAzuic4KwdYAUYcjvVDgk80k1kibgdOsoic0xcNrpztLX5ZPFHnz0TfHOKdYxIE2XMZTbUT1RDg/640?wx_fmt=png)
**4.2.2 不回表排序模式**
- 
根据索引或者全表扫描，按照过滤条件获得需要查询的数据；
- 
将要排序的列值和 用户需要返回的字段 组成键值对，存入sort buffer中；
- 
如果sort buffer内存大于这些键值对的内存，就不需要创建临时文件了。否则，每次sort buffer填满以后，需要直接用qsort(快速排序算法)在内存中排好序，并写到临时文件中；
- 
重复上述步骤，直到所有的行数据都正常读取了完成；
- 
用到了临时文件的，需要利用磁盘外部排序，将排序后的数据写入到结果文件中；
- 
直接从结果文件中返回用户需要的字段数据，而不是根据row ID再次回表查询。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmBU8XAzuic4KwdYAUYcjvVDGxmibq4AnKficwjJWmTfyBCOfGrBWjJN2Szw9w0apiaSTpP9vzEuTCMpg/640?wx_fmt=png)
**4.2.3打包数据排序模式**
第三种排序模式的改进仅仅在于将char和varchar字段存到sort buffer中时，更加紧缩。
在之前的两种模式中，存储了”yes”3个字符的定义为VARCHAR(255)的列会在内存中申请255个字符内存空间，但是5.7.3改进后，只需要存储2个字节的字段长度和3个字符内存空间（用于保存”yes”这三个字符）就够了，内存空间整整压缩了50多倍,可以让更多的键值对保存在sort buffer中。
**4.2.4三种模式比较**
第二种模式是第一种模式的改进，避免了二次回表，采用的是用空间换时间的方法。
但是由于sort buffer就那么大，如果用户要查询的数据非常大的话，很多时间浪费在多次磁盘外部排序，导致更多的IO操作，效率可能还不如第一种方式。
所以，MySQL给用户提供了一个max_length_for_sort_data的参数。当“排序的键值对大小” > max_length_for_sort_data时，MySQL认为磁盘外部排序的IO效率不如回表的效率，会选择第一种排序模式；反之，会选择第二种不回表的模式。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmBU8XAzuic4KwdYAUYcjvVDNBcelWpJiakVPwcvArdcgWfzBdDCibCMVVLmfZlaPibIpJibZibibNQrxnDg/640?wx_fmt=png)
第三种模式主要是解决变长字符数据存储空间浪费的问题，对于实际数据不多，字段定义较长的改进效果会更加明显。
> 
能看到这里的同学绝逼是真爱，但是还没完，后面的东西可能会更烧脑…
> 
建议大家喝杯咖啡再继续看。
很多文章写到这里可能就差不多了，但是大家忘记关注一个问题了：“如果排序的数据不能完全放在sort buffer内存里面，是怎么通过外部排序完成整个排序过程的呢？”
要解决这个问题，我们首先需要简单查看一下外部排序到底是怎么做的。
**五、外部排序**
**5.1 普通外部排序**
**5.1.1 两路外部排序**
我们先来看一下最简单，最普遍的两路外部排序算法。
假设内存只有100M，但是排序的数据有900M，那么对应的外部排序算法如下：
- 
从要排序的900M数据中读取100MB数据到内存中，并按照传统的内部排序算法（快速排序）进行排序；
- 
将排序好的数据写入磁盘；
- 
重复1，2两步，直到每个100MB chunk大小排序好的数据都被写入磁盘；
- 
每次读取排序好的chunk中前10MB（= 100MB / (9 chunks + 1)）数据，一共9个chunk需要90MB，剩下的10MB作为输出缓存；
- 
对这些数据进行一个“9路归并”，并将结果写入输出缓存。如果输出缓存满了，则直接写入最终排序结果文件并清空输出缓存；如果9个10MB的输入缓存空了，从对应的文件再读10MB的数据，直到读完整个文件。最终输出的排序结果文件就是900MB排好序的数据了。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmBU8XAzuic4KwdYAUYcjvVDVbfve04n4gUMlYOoFu6um4g7pvny6srRVuUXsu4wibx2tmCWOeYxFew/640?wx_fmt=png)
**5.1.2 多路外部排序**
上述排序算法是一个两路排序算法（先排序，后归并）。但是这种算法有一个问题，假设要排序的数据是50GB而内存只有100MB，那么每次从500个排序好的分片中取200KB（100MB / 501 约等于200KB）就是很多个随机IO。效率非常慢，对应可以这样来改进：
- 
从要排序的50GB数据中读取100MB数据到内存中，并按照传统的内部排序算法（快速排序）进行排序；
- 
将排序好的数据写入磁盘；
- 
重复1，2两步，直到每个100MB chunk大小排序好的数据都被写入磁盘；
- 
每次取25个分片进行归并排序，这样就形成了20个（500/25=20）更大的2.5GB有序的文件；
- 
对这20个2.5GB的有序文件进行归并排序，形成最终排序结果文件。
对应的数据量更大的情况可以进行更多次归并。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmBU8XAzuic4KwdYAUYcjvVD8ibtJtFguyvw51Wz4PewDAibicA4ePZjiaz70b0kSRFNDY6IcUSVgZ3FGQ/640?wx_fmt=png)
**5.2 MySQL外部排序**
**5.2.1 MySQL外部排序算法**
那MySQL使用的外部排序是怎么样的列，我们以回表排序模式为例：
- 
根据索引或者全表扫描，按照过滤条件获得需要查询的数据；
- 
将要排序的列值和row ID组成键值对，存入sort buffer中；
- 
如果sort buffer内存大于这些键值对的内存，就不需要创建临时文件了。否则，每次sort buffer填满以后，需要直接用qsort(快速排序模式)在内存中排好序，作为一个block写到临时文件中。跟正常的外部排序写到多个文件中不一样，MySQL只会写到一个临时文件中，并通过保存文件偏移量的方式来模拟多个文件归并排序；
- 
重复上述步骤，直到所有的行数据都正常读取了完成；
- 
每MERGEBUFF (7) 个block抽取一批数据进行排序，归并排序到另外一个临时文件中，直到所有的数据都排序好到新的临时文件中；
- 
重复以上归并排序过程，直到剩下不到MERGEBUFF2 (15)个block。
通俗一点解释：
第一次循环中，一个block对应一个sort buffer（大小为sort_buffer_size）排序好的数据；每7个做一个归并。
第二次循环中，一个block对应MERGEBUFF (7) 个sort buffer的数据，每7个做一个归并。
…
直到所有的block数量小于MERGEBUFF2 (15)。
- 
最后一轮循环，仅将row ID写入到结果文件中；
- 
根据结果文件中的row ID按序读取用户需要返回的数据。为了进一步优化性能，MySQL会读一批row ID，并将读到的数据按排序字段要求插入缓存区中(内存大小read_rnd_buffer_size)。
这里我们需要注意的是：
- 
MySQL把外部排序好的分片写入同一个文件中，通过保存文件偏移量的方式来区别各个分片位置；
- 
MySQL每MERGEBUFF (7)个分片做一个归并，最终分片数达到MERGEBUFF2 (15)时，做最后一次归并。这两个值都写死在代码中了…
**5.2.2 sort_merge_passes**
MySQL手册中对Sort_merge_passes的描述只有一句话
> 
Sort_merge_passes
The number of merge passes that the sort algorithm has had to do. If this value is large, you should consider increasing the value of the sort_buffer_size system variable.
这段话并没有把sort_merge_passes到底是什么，该值比较大时说明了什么，通过什么方式可以缓解这个问题。
我们把上面MySQL的外部排序算法搞清楚了，这个问题就清楚了。
其实sort_merge_passes对应的就是MySQL做归并排序的次数，也就是说，如果sort_merge_passes值比较大，说明sort_buffer和要排序的数据差距越大，我们可以通过增大sort_buffer_size或者让填入sort_buffer_size的键值对更小来缓解sort_merge_passes归并排序的次数。
对应的，我们可以在源码中看到证据。
上述MySQL外部排序的算法中第5到第7步，是通过sql/filesort.cc文件中merge_many_buff()函数来实现，第5步单次归并使用merge_buffers()实现，源码摘录如下：
> 
int merge_many_buff(Sort_param *param, Sort_buffer sort_buffer,
                    Merge_chunk_array chunk_array,
                    size_t *p_num_chunks, IO_CACHE *t_file)
{
...
    for (i=0 ; i < num_chunks - MERGEBUFF * 3 / 2 ; i+= MERGEBUFF)
    {
      if (merge_buffers(param,                  // param
                        from_file,              // from_file
                        to_file,                // to_file
                        sort_buffer,            // sort_buffer
                        last_chunk++,           // last_chunk [out]
                        Merge_chunk_array(&chunk_array[i], MERGEBUFF),
                        0))                     // flag
      goto cleanup;
    }
    if (merge_buffers(param,
                      from_file,
                      to_file,
                      sort_buffer,
                      last_chunk++,
                      Merge_chunk_array(&chunk_array[i], num_chunks - i),
                      0))
      break;                                    /* purecov: inspected */
...
}
截取部分merge_buffers()的代码如下，
> 
int merge_buffers(Sort_param *param, IO_CACHE *from_file,
                  IO_CACHE *to_file, Sort_buffer sort_buffer,
                  Merge_chunk *last_chunk,
                  Merge_chunk_array chunk_array,
                  int flag)
{
...
  current_thd->inc_status_sort_merge_passes();
...
}
可以看到：每个merge_buffers()都会增加sort_merge_passes，也就是说每一次对MERGEBUFF (7) 个block归并排序都会让sort_merge_passes加一，sort_merge_passes越多表示排序的数据太多，需要多次merge pass。解决的方案无非就是缩减要排序数据的大小或者增加sort_buffer_size。
打个小广告，在我们的qmonitor中就有sort_merge_pass的性能指标和参数值过大的报警设置。
**六、trace 结果解释**
说明白了三种排序模式和外部排序的方法，我们回过头来看一下trace的结果。
**6.1 是否存在磁盘外部排序**
> 
"number_of_tmp_files": 0,
number_of_tmp_files表示有多少个分片，如果number_of_tmp_files不等于0，表示一个sort_buffer_size大小的内存无法保存所有的键值对，也就是说，MySQL在排序中使用到了磁盘来排序。
**6.2 是否存在优先队列优化排序**
由于我们的这个SQL里面没有对数据进行分页限制，所以filesort_priority_queue_optimization并没有启用
> 
"filesort_priority_queue_optimization": {
              "usable": false,
              "cause": "not applicable (no LIMIT)"
            },
而正常情况下，使用了Limit会启用优先队列的优化。优先队列类似于FIFO先进先出队列。
算法稍微有点改变，以回表排序模式为例。
- 
sort_buffer_size足够大
如果Limit 限制返回N条数据，并且N条数据比sort_buffer_size小，那么MySQL会把sort buffer作为priority queue，在第二步插入priority queue时会按序插入队列；在第三步，队列满了以后，并不会写入外部磁盘文件，而是直接淘汰最尾端的一条数据，直到所有的数据都正常读取完成。
算法如下：
- 
根据索引或者全表扫描，按照过滤条件获得需要查询的数据
- 
将要排序的列值和row ID组成键值对，按序存入中priority queue中
- 
如果priority queue满了，直接淘汰最尾端记录。
- 
重复上述步骤，直到所有的行数据都正常读取了完成
- 
最后一轮循环，仅将row ID写入到结果文件中
- 
根据结果文件中的row ID按序读取用户需要返回的数据。为了进一步优化性能，MySQL会读一批row ID，并将读到的数据按排序字段要求插入缓存区中(内存大小read_rnd_buffer_size)。
- 
sort_buffer_size不够大
否则，N条数据比sort_buffer_size大的情况下，MySQL无法直接利用sort buffer作为priority queue，正常的文件外部排序还是一样的，只是在最后返回结果时，只根据N个row ID将数据返回出来。具体的算法我们就不列举了。
这里MySQL到底是否选择priority queue是在sql/filesort.cc的check_if_pq_applicable()函数中确定的，具体的代码细节这里就不展开了。
另外，我们也没有讨论limit m,n的情况，如果是Limit m,n， 上面对应的“N个row ID”就是“M+N个row ID”了，MySQL的limit m,n 其实是取m+n行数据，最后把M条数据丢掉。
从上面我们也可以看到sort_buffer_size足够大对limit数据比较小的情况，优化效果是很明显的。
**七、MySQL其他相关排序参数**
**7.1 max_sort_length**
这里需要区别max_sort_length 和max_length_for_sort_data。
max_length_for_sort_data是为了让MySQL选择”< sort_key, rowid >”还是”< sort_key, additional_fields >”的模式。
而max_sort_length是键值对的大小无法确定时（比如用户要查询的数据包含了 SUBSTRING_INDEX(col1, ‘.’,2)）MySQL会对每个键值对分配max_sort_length个字节的内存，这样导致内存空间浪费，磁盘外部排序次数过多。
**7.2 innodb_disable_sort_file_cache**
innodb_disable_sort_file_cache设置为ON的话，表示在排序中生成的临时文件不会用到文件系统的缓存，类似于O_DIRECT打开文件。
**7.3 innodb_sort_buffer_size**
这个参数其实跟我们这里讨论的SQL排序没有什么关系。innodb_sort_buffer_size设置的是在创建InnoDB 索引时，使用到的sort buffer的大小。
以前写死为1M，现在开放出来，允许用户自定义设置这个参数了。
**八、MySQL排序优化总结**
最后整理一下优化MySQL排序的手段
- 
排序和查询的字段尽量少。只查询你用到的字段，不要使用select * ；使用limit查询必要的行数据；
- 
要排序或者查询的字段，尽量不要用不确定字符函数，避免MySQL直接分配max_sort_length，导致sort buffer空间不足；
- 
使用索引来优化或者避免排序；
- 
增加sort_buffer_size大小，避免磁盘排序；
- 
不得不使用original 排序算法时，增加read_rnd_buffer_size；
- 
字段长度定义合适就好（避免过长）；
- 
tmpdir建议独立存放，放在高速存储设备上。
写到这里，大家可以回顾一下文章开头的那八个问题，如果回答不了这些问题，说明其实你没有真正的理解透MySQL的排序，或者说我们的这篇文章写的太烂了==
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/icNyEYk3VqGmBU8XAzuic4KwdYAUYcjvVDicbrDYrkiaf5MtCvia2tibhVXqZQicDDqJJCvX4fIa7ekSOibibdt8ibpu7oXA/640?wx_fmt=png)
是不是感觉累觉不爱了，我们的文章也终于要结束了，引用一个图片作为文章的结尾，向Christophe致敬。当下次其他同学问你，MySQL的排序到底是怎么做的时候，你可以告诉他：
This is
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/icNyEYk3VqGmBU8XAzuic4KwdYAUYcjvVD2hhs2H0cLvfSySQxcmTVRsOx2UD67pfGaaqXUhfl5jou4BPkVicgoxQ/640?wx_fmt=gif)
**九、参考文献**
https://dev.mysql.com/doc/refman/5.7/en/order-by-optimization.html
http://coding-geek.com/how-databases-work/
版权申明：内容来源网络，版权归原创者所有。除非无法确认，我们都会标明作者及出处，如有侵权烦请告知，我们会立即删除并表示歉意。谢谢。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
