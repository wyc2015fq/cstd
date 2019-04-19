# leveldb原理探究与代码分析 - fanyun的博客 - CSDN博客
2018年06月23日 22:47:40[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：105
# 1. 概述
Level DB（[http://code.google.com/p/leveldb/](http://code.google.com/p/leveldb/)）是google开源的Key/Value存储系统，它的committer阵容相当强大，基本上是bigtable的原班人马，包括像jeff dean这样的大牛，它的代码合设计非常具有借鉴意义，是一种典型的LSM Tree的KV引擎的实现，从它的数据结构来看，基本就是sstable的开源实现，而且针对各种平台作了port，目前被用在chrome等项目中。
# 2. LSM Tree
Level DB是典型的Log-Structured-Merge Tree的实现，它通过延迟写入以及Write Log Ahead技术来加速数据的写入并保障数据的安全。LevelDB的每个数据文件(sstable)中的记录都是按照Key的顺序进行排序的，但是随机写入时，key的到来是无序的，因此难以将记录插入到其排序位置。于是需要它采取一种延迟写入的方式，批量攒集一定量的数据，将它们在内存中排好序，一次性写入到磁盘中。但是这期间一旦系统断电或其他异常，则可能导致数据丢失，因此需要将数据先写入到log的文件中，这样便将随机写转化为追加写入，对于磁盘性能会有很大提升，如果进程发生中断，重启后可以根据log恢复之前写入的数据。
## 2.1 Write Batch
Level DB只支持两种更新操作:
1. 插入一条记录 
2. 删除一条记录
代码如下：
[cpp][view plain](http://blog.csdn.net/icefireelf/article/details/7515816#)[copy](http://blog.csdn.net/icefireelf/article/details/7515816#)
- std::string key1,key2,value;    
- leveldb::Status s;  
- s = db->Put(leveldb::WriteOptions(), key1, value);    
- s = db->Delete(leveldb::WriteOptions(), key2);    
同时还支持以一种批量的方式写入数据：
[cpp][view plain](http://blog.csdn.net/icefireelf/article/details/7515816#)[copy](http://blog.csdn.net/icefireelf/article/details/7515816#)
- std::string key1,key2,value;   
- leveldb::WriteBatch batch;   
- batch.Delete(key1);   
- batch.Put(key2, value);   
- leveldb::status s = db->Write(leveldb::WriteOptions(), &batch);  
![](http://blog.csdn.net/icefireelf/article/details/7515816)
其实，在Level DB内部，单独更新与批量更新的调用的接口是相同的，单独更新也会被组织成为包含一条记录的Batch，然后写入数据库中。Write Batch的组织形式如下：
![](https://img-my.csdn.net/uploads/201204/26/1335447663_7043.png)
## 2.1 Log Format
每次更新操作都被组织成这样一个数据包，并作为一条日志写入到log文件中，同时也会被解析为一条条内存记录，按照key排序后插入到内存表中的相应位置。LevelDB使用Memory Mapping的方式对log数据进行访问：如果前一次映射的空间已写满，则先将文件扩展一定的长度（每次扩展的长度按64KB,128KB,...的顺序逐次翻倍，最大到1MB），然后映射到内存，对映射的内存再以32KB的Page进行切分，每次写入的日志填充到Page中，攒积一定量后Sync到磁盘上（也可以设置WriteOptions，每写一条日志就Sync一次，但是这样效率很低），内存映射文件的代码如下：
[cpp][view plain](http://blog.csdn.net/icefireelf/article/details/7515816#)[copy](http://blog.csdn.net/icefireelf/article/details/7515816#)
- class PosixMmapFile : public WritableFile   
- {  
- private:  
-   std::string filename_;  // 文件名称
- int fd_;                // 文件句柄
- size_t page_size_;      // 
- size_t map_size_;       // 内存映射的区域大小
- char* base_;            // 内存映射区域的起始地址
- char* limit_;           // 内存映射区域的结束地址
- char* dst_;             // 最后一次占用的内存的结束地址
- char* last_sync_;       // 最后一次同步到磁盘的结束地址
-   uint64_t file_offset_;  // 当前文件的偏移值
- bool pending_sync_;     // 延迟同步的标志
- 
- public:  
-   PosixMmapFile(const std::string& fname, int fd, size_t page_size)  
-       : filename_(fname),  
-         fd_(fd),  
-         page_size_(page_size),  
-         map_size_(Roundup(65536, page_size)),  
-         base_(NULL),  
-         limit_(NULL),  
-         dst_(NULL),  
-         last_sync_(NULL),  
-         file_offset_(0),  
-         pending_sync_(false) {  
-     assert((page_size & (page_size - 1)) == 0);  
-   }  
- 
-   ~PosixMmapFile() {  
- if (fd_ >= 0) {  
-       PosixMmapFile::Close();  
-     }  
-   }  
- 
-   Status Append(const Slice& data) {  
- constchar* src = data.data();  
- size_t left = data.size();  
- while (left > 0) {  
- // 计算上次最后一次申请的区域的剩余容量，如果已完全耗尽，
- // 则卸载当前区域，申请一个新的区域
- size_t avail = limit_ - dst_;  
- if (avail == 0) {  
- if (!UnmapCurrentRegion() ||  
-             !MapNewRegion()) {  
- return IOError(filename_, errno);  
-         }  
-       }  
- // 填充当前区域的剩余容量
- size_t n = (left <= avail) ? left : avail;  
-       memcpy(dst_, src, n);  
-       dst_ += n;  
-       src += n;  
-       left -= n;  
-     }  
- return Status::OK();  
-   }  
- 
-   Status PosixMmapFile::Close() {  
-     Status s;  
- size_t unused = limit_ - dst_;  
- if (!UnmapCurrentRegion()) {  
-       s = IOError(filename_, errno);  
-     } elseif (unused > 0) {  
- // 关闭时将文件没有使用用的空间truncate掉
- if (ftruncate(fd_, file_offset_ - unused) < 0) {  
-         s = IOError(filename_, errno);  
-       }  
-     }  
- 
- if (close(fd_) < 0) {  
- if (s.ok()) {  
-         s = IOError(filename_, errno);  
-       }  
-     }  
- 
-     fd_ = -1;  
-     base_ = NULL;  
-     limit_ = NULL;  
- return s;  
-   }  
- 
- virtual Status Sync() {  
-     Status s;  
- if (pending_sync_) {  
- // 上个区域也有数据未同步，则先同步数据
-       pending_sync_ = false;  
- if (fdatasync(fd_) < 0) {  
-         s = IOError(filename_, errno);  
-       }  
-     }  
- 
- if (dst_ > last_sync_) {  
- // 计算未同步数据的起始与结束地址，同步时，起始地址按page_size_向下取整，
- // 结束地址向上取整，保证每次同步都是同步一个或多个page
- size_t p1 = TruncateToPageBoundary(last_sync_ - base_);  
- size_t p2 = TruncateToPageBoundary(dst_ - base_ - 1);   
- // 如果刚好为整数个page_size_，由于下面同步时必然会加一个page_size_，所以这里可以减去1
-       last_sync_ = dst_;  
- if (msync(base_ + p1, p2 - p1 + page_size_, MS_SYNC) < 0) {  
-         s = IOError(filename_, errno);  
-       }  
-     }  
- return s;  
-   }  
- private:  
- // 将x按y向上对齐   
- staticsize_t Roundup(size_t x, size_t y) {  
- return ((x + y - 1) / y) * y;  
-   }  
- // 将s按page_size_向下对齐
- size_t TruncateToPageBoundary(size_t s) {  
-     s -= (s & (page_size_ - 1));  
-     assert((s % page_size_) == 0);  
- return s;  
-   }   
- 
- // 卸载当前映射的内存区域  
- bool UnmapCurrentRegion() {      
- bool result = true;  
- if (base_ != NULL) {  
- if (last_sync_ < limit_) {  
- // 如果当前页没有完全被同步，则标明本文件需要被同步，下次调用Sync()方法时会将本页中未同步的数据同步到磁盘
-         pending_sync_ = true;  
-       }  
- if (munmap(base_, limit_ - base_) != 0) {  
-         result = false;  
-       }  
-       file_offset_ += limit_ - base_;  
-       base_ = NULL;  
-       limit_ = NULL;  
-       last_sync_ = NULL;  
-       dst_ = NULL;      // 使用翻倍的策略增加下次申请区域的大小，最大到1MB
- if (map_size_ < (1<<20)) {  
-         map_size_ *= 2;  
-       }  
-     }  
- return result;  
-   }    
- 
- bool MapNewRegion() {  
-     assert(base_ == NULL); // 申请一个新的区域时，上一个申请的区域必须已经卸载 
- // 先将文件扩大    
- if (ftruncate(fd_, file_offset_ + map_size_) < 0) {  
- returnfalse;  
-     }  
- // 将新区域映射到文件
- void* ptr = mmap(NULL, map_size_, PROT_READ | PROT_WRITE, MAP_SHARED,  
-                      fd_, file_offset_);  
- if (ptr == MAP_FAILED) {  
- returnfalse;  
-     }  
-     base_ = reinterpret_cast<char*>(ptr);  
-     limit_ = base_ + map_size_;  
-     dst_ = base_;  
-     last_sync_ = base_;  
- returntrue;  
-   }  
- };  
但是，一个Batch的数据按上面的方式组织后，如果做为一条日志写入Log，则很可能需要跨两个或更多个Page；为了更好地管理日志以及保障数据安全，LevelDB对日志记录进行了更细的切分，如果一个Batch对应的数据需要跨页，则会将其切分为多条Entry，然后写入到不同Page中，Entry不会跨越Page，我们通过对多个Entry进行解包，可以还原出的Batch数据。最终，LevelDB的log文件被组织为下面的形式：
![](https://img-my.csdn.net/uploads/201204/26/1335447972_2102.png)
这里，我们可以看一下log_writer的代码：
[cpp][view plain](http://blog.csdn.net/icefireelf/article/details/7515816#)[copy](http://blog.csdn.net/icefireelf/article/details/7515816#)
- Status Writer::AddRecord(const Slice& slice) {  
- constchar* ptr = slice.data();  
- size_t left = slice.size();  
- 
-   Status s;  
- bool begin = true;  
- do {  
- constint leftover = kBlockSize - block_offset_;  
-     assert(leftover >= 0);  
- if (leftover < kHeaderSize) {  
- // 如果当前page的剩余长度小于7字节且大于0，则都填充'\0'，并新起一个page
- if (leftover > 0) {  
-         assert(kHeaderSize == 7);  
-         dest_->Append(Slice("\x00\x00\x00\x00\x00\x00", leftover));  
-       }  
-       block_offset_ = 0;  
-     }  
- 
- // 计算page能否容纳整体日志，如果不能，则将日志切分为多条entry，插入不同的page中，type中注明该entry是日志的开头部分，中间部分还是结尾部分。
- constsize_t avail = kBlockSize - block_offset_ - kHeaderSize;  
- constsize_t fragment_length = (left < avail) ? left : avail;  
- 
-     RecordType type;  
- constbool end = (left == fragment_length);  
- if (begin && end) {  
-       type = kFullType;   // 本Entry保存完整的Batch
-     } elseif (begin) {  
-       type = kFirstType;  // 本Entry只保存起始部分
-     } elseif (end) {  
-       type = kLastType;   // 本Entry只保存结束部分
-     } else {  
-       type = kMiddleType; // 本Entry保存Batch的中间部分，不含起始与结尾，有时可能需要保存多个middle
-     }  
- 
-     s = EmitPhysicalRecord(type, ptr, fragment_length);  
-     ptr += fragment_length;  
-     left -= fragment_length;  
-     begin = false;  
-   } while (s.ok() && left > 0);  
- return s;  
- }  
- 
- Status Writer::EmitPhysicalRecord(RecordType t, constchar* ptr, size_t n) {  
-   assert(n <= 0xffff);    
-   assert(block_offset_ + kHeaderSize + n <= kBlockSize);  
- 
- // 填充记录头
- char buf[kHeaderSize];  
-   buf[4] = static_cast<char>(n & 0xff);  
-   buf[5] = static_cast<char>(n >> 8);  
-   buf[6] = static_cast<char>(t);  
- 
- // 计算crc
-   uint32_t crc = crc32c::Extend(type_crc_[t], ptr, n);  
-   crc = crc32c::Mask(crc);   
-   EncodeFixed32(buf, crc);  
- 
- // 填充entry内容
-   Status s = dest_->Append(Slice(buf, kHeaderSize));  
- if (s.ok()) {  
-     s = dest_->Append(Slice(ptr, n));  
- if (s.ok()) {  
-       s = dest_->Flush();  
-     }  
-   }  
-   block_offset_ += kHeaderSize + n;  
- return s;  
- }  
## 2.3 Write Log Ahead
Level DB在更新时，先写log，然后更新memtable，每个memtable会设置一个最大容量，如果超过阈值，则采用双buffer机制，关闭当前log文件并将当前memtable切换未从memtable，然后新建一个log文件以及memtable，将数据写进新的log文件与memtable，并通知后台线程对从memtable进行处理，及时将其dump到磁盘上，或者启动compaction流程。Write的代码分析如下：
[cpp][view plain](http://blog.csdn.net/icefireelf/article/details/7515816#)[copy](http://blog.csdn.net/icefireelf/article/details/7515816#)
- Status DBImpl::Write(const WriteOptions& options, WriteBatch* updates)   
- {  
-   Status status;  
-   MutexLock l(&mutex_);  // 锁定互斥体，同一时间只能有一个线程更新数据
-   LoggerId self;     
- // 获取Logger的使用权,如果有其他线程拥有所有权，则等待至其释放所有权。
-   AcquireLoggingResponsibility(&self);  
-   status = MakeRoomForWrite(false);  // May temporarily release lock and wait
-   uint64_t last_sequence = versions_->LastSequence();  // 获取当前的版本号
- if (status.ok()) {  
- // 将当前版本号加1后作为本次更新的日志的版本，
- // 一次批量更新可能包含多个操作，这些操作都用一个版本有一个好处：
- // 本次更新的所有操作，要么都可见，要么都不可见，不存在一部分可见，另一部分不可见的情况。
-     WriteBatchInternal::SetSequence(updates, last_sequence + 1);  
- // 但是本次更新可能有多个操作，跳过与操作数相等的版本号，保证不被使用
-     last_sequence += WriteBatchInternal::Count(updates);  
- 
- // 将batch写入log，然后应用到memtable中
-     {  
-       assert(logger_ == &self);  
-       mutex_.Unlock();  
- // 这里，可以解锁，因为在AcquireLoggingResponsibility()方法中已经获取了Logger的拥有权，
- // 其他线程即使获得了锁，但是由于&self != logger，其会阻塞在AcquireLoggingResponsibility()方法中。
- // 将更新写入log文件，如果设置了每次写入进行sync，则将其同步到磁盘，这个操作可能比较长，
- // 防止了mutex_对象长期被占用，因为其还负责其他一些资源的同步
-       status = log_->AddRecord(WriteBatchInternal::Contents(updates));  
- if (status.ok() && options.sync) {  
-         status = logfile_->Sync();  
-       }  
- if (status.ok()) {  
- // 成功写入了log后，才写入memtable
-         status = WriteBatchInternal::InsertInto(updates, mem_);  
-       }  
- // 重新锁定mutex_
-       mutex_.Lock();  
-       assert(logger_ == &self);  
-     }  
- // 更新版本号
-     versions_->SetLastSequence(last_sequence);  
-   }  
- // 释放对logger的所有权，并通知等待的线程，然后解锁
-   ReleaseLoggingResponsibility(&self);  
- return status;  
- }  
- 
- // force参数表示强制新起一个memtable
- Status DBImpl::MakeRoomForWrite(bool force) {  
-   mutex_.AssertHeld();  
-   assert(logger_ != NULL);  
- bool allow_delay = !force;  
-   Status s;  
- while (true) {  
- if (!bg_error_.ok()) {  
- // 后台线程存在问题，则返回错误，不接受更新
-       s = bg_error_;  
- break;  
-     } elseif (  
-         allow_delay &&  
-         versions_->NumLevelFiles(0) >= config::kL0_SlowdownWritesTrigger) {  
- // 如果不是强制写入，而且level 0的sstable超过8个，则本次更新阻塞1毫秒，
- // leveldb将sstable分为多个等级，其中level 0中的不同表的key是可能重叠的，
- // 如果l0的sstable过多，会导致查询性能下降，这时需要适当降低更新速度，让
- // 后台线程进行compaction操作，但是设计者不希望让某次写操作等待数秒，
- // 而是让每次更新操作分担延迟，即每次写操作阻塞1毫秒，平衡读写速率；
- // 另外，理论上这也能让compaction线程获得更多的cpu时间（当然，
- // 这是假定compaction与更新操作共享一个CPU时才有意义）
-       mutex_.Unlock();  
-       env_->SleepForMicroseconds(1000);  
-       allow_delay = false;  // 最多延迟一次，下次不延迟
-       mutex_.Lock();  
-     } elseif (!force &&  
-                (mem_->ApproximateMemoryUsage() <= options_.write_buffer_size)) {  
- // 如果当前memtable已使用的空间小于write_buffer_size，则跳出，更新到当前memtable即可。
- // 当force为true时，第一次循环会走后面else逻辑，切换了memtable后force被置为false，
- // 第二次循环时就可以在此跳出了
- break;  
-     } elseif (imm_ != NULL) {  
- // 如果当前memtable已经超过write_buffer_size,且备用的memtable也在被使用，则阻塞更新并等待
-       bg_cv_.Wait();  
-     } elseif (versions_->NumLevelFiles(0) >= config::kL0_StopWritesTrigger) {  
- // 如果当前memtable已使用的空间小于write_buffer_size，但是备用的memtable未被使用，
- // 则检查level 0的sstable个数，如超过12个，则阻塞更新并等待
-       Log(options_.info_log, "waiting...\n");  
-       bg_cv_.Wait();  
-     } else {  
- // 否则，使用新的id新创建一个log文件，并将当前memtable切换为备用的memtable，新建一个
- // memtable，然后将数据写入当前的新memtable，即切换log文件与memtable，并告诉后台线程
- // 可以进行compaction操作了
-       assert(versions_->PrevLogNumber() == 0);  
-       uint64_t new_log_number = versions_->NewFileNumber();  
-       WritableFile* lfile = NULL;  
-       s = env_->NewWritableFile(LogFileName(dbname_, new_log_number), &lfile);  
- if (!s.ok()) {  
- break;  
-       }  
- delete log_;  
- delete logfile_;  
-       logfile_ = lfile;  
-       logfile_number_ = new_log_number;  
-       log_ = new log::Writer(lfile);  
-       imm_ = mem_;  
-       has_imm_.Release_Store(imm_);  
-       mem_ = new MemTable(internal_comparator_);  
-       mem_->Ref();  
-       force = false;   // 下次判断可以不新建memtable了
-       MaybeScheduleCompaction();  
-     }  
-   }  
- return s;  
- }  
- void DBImpl::AcquireLoggingResponsibility(LoggerId* self) {  
- while (logger_ != NULL) {  
-     logger_cv_.Wait();  
-   }  
-   logger_ = self;  
- }  
- 
- void DBImpl::ReleaseLoggingResponsibility(LoggerId* self) {  
-   assert(logger_ == self);  
-   logger_ = NULL;  
-   logger_cv_.SignalAll();  
- }  
## 2.4 Skip List
Level DB内部采用跳表结构来组织Memtable，每插入一条记录，先根据跳表通过多次key的比较，定位到记录应该插入的位置，然后按照一定的概率确定该节点需要建立多少级的索引，跳表结构如下：
![](https://img-my.csdn.net/uploads/201204/27/1335523511_6056.png)
Level DB的SkipList最高12层，最下面一层（level0）的链是全链，即每条记录必须在此链中插入相应的索引节点；从level1到level11则是按概率决定是否需要建索引，概率按照1/4的因子等比递减。下面举个例子，说明一下这个流程：
1. 看上图，假定我们链不存在record3，level0中，record2的下一条记录是record4，level1中，record2的下一条记录是record5。
2. 现在，我们插入一条记录record3，通过key的比较，我们定位到它应该在record2与record4之间。
3. 然后，我们按照下面的代码确定一条记录需要在跳表中建立几重索引：
[cpp][view plain](http://blog.csdn.net/icefireelf/article/details/7515816#)[copy](http://blog.csdn.net/icefireelf/article/details/7515816#)
- template<typename Key, class Comparator>  
- int SkipList<Key,Comparator>::RandomHeight() {  
- // Increase height with probability 1 in kBranching
- staticconst unsigned int kBranching = 4;  
- int height = 1;  
- while (height < kMaxHeight && ((rnd_.Next() % kBranching) == 0)) {  
-     height++;  
-   }  
- return height;  
- }  
按照上面的代码，我们可以得出，建立x级索引的概率是0.25 ^(x - 1) * 0.75，所以，建立1级索引的概率为75%，建立2级索引的概率为25%*75%=18.75%，...（个人感觉，google把分支因子定为4有点高了，这样在绝大多数情况下，跳表的高度都不大于3）。
4.  在level0 ~ level (x-1)中链表的合适位置插入record3，假定根据上面的公式，我们得到需要为record3建立2级索引，即x=2，因此需要在level0与level1中的链中插入record3：在level 0的链中，record3插在record2与record4之间，在level 1的链中，record3插入在record2与record5之间，形成了现在的索引结构，在查询一个记录时，可以从最高一级索引向下查找，节约比较次数。
## 2.5 Record Format
Level DB将用户的每个更新或删除操作组合成一个Record，其格式如下：
![](https://img-my.csdn.net/uploads/201204/27/1335533865_8142.png)
从图中可以看出，每个Record会在原用key的基础上添加版本号以及key的类型（更新 or 删除），组成internal key。插入跳表时，是按照internal key进行排序，而非用户key。这样，我们只可能向跳表中添加节点，而不可能删除和替换节点。
Internal Key在比较时，按照下面的算法：
[cpp][view plain](http://blog.csdn.net/icefireelf/article/details/7515816#)[copy](http://blog.csdn.net/icefireelf/article/details/7515816#)
- int InternalKeyComparator::Compare(const Slice& akey, const Slice& bkey) const {  
- int r = user_comparator_->Compare(ExtractUserKey(akey), ExtractUserKey(bkey));  
- if (r == 0) {  
- // 比较后面8个字节构造的整数，第一个字节的type为Least Significant Byte
- const uint64_t anum = DecodeFixed64(akey.data() + akey.size() - 8);  
- const uint64_t bnum = DecodeFixed64(bkey.data() + bkey.size() - 8);  
- if (anum > bnum)  // 注意：整数大反而key比较小
-     {  
-       r = -1;  
-     } elseif (anum < bnum) {  
-       r = +1;  
-     }  
-   }  
- return r;  
- }  
根据上面的算法，我们可以得知Internal Key的比较顺序：
1. 如果User Key不相等，则User Key比较小的记录的Internal Key也比较小，User Key默认采用字典序（lexicographic）进行比较，可以在建表参数中自定义comparator。
2. 如果type也相同，则比较Sequence Num，Sequence Num大的Internal Key比较小。
3. 如果Sequence Num相等，则比较Type，type为更新（Key Type=1）的记录比的type为删除（Key Type=0）的记录的Internal Key小。
在插入到跳表时，一般不会出现Internal Key相等的情况（除非在一个Batch中操作了同一条记录两次，这里会出现一种bug：在一个Write Batch中，先插入一条记录，然后删除这条记录，最后把这个Batch写入DB，会发现DB中这条记录存在。因此，不推荐在Batch中多次操作相同key的记录），User Key相同的记录插入跳表时，Sequence Num大的记录会排在前面。
设计Internal Key有个以下一些作用：
1. Level DB支持快照查询，即查询时指定快照的版本号，查询出创建快照时某个User Key对应的Value，那么可以组成这样一个Internal Key：Sequence=快照版本号，Type=1，User Key为用户指定Key，然后查询数据文件与内存，找到大于等于此Internal Key且User Key匹配的第一条记录即可（即Sequence Num小于等于快照版本号的第一条记录）。
2.如果查询最新的记录时，将Sequence Num设置为0xFFFFFFFFFFFFFF即可。因为我们更多的是查询最新记录，所以让Sequence Num大的记录排前面，可以在遍历时遇见第一条匹配的记录立即返回，减少往后遍历的次数。
# 3.文件结构
## 3.1 文件组成
Level DB包含一下几种文件：
|文件类型|说明|
|----|----|
|dbname/MANIFEST-[0-9]+|清单文件|
|dbname/[0-9]+.log|db日志文件|
|dbname/[0-9]+.sst|dbtable文件|
|dbname/[0-9]+.dbtmp|db临时文件|
|dbname/CURRENT|记录当前使用的清单文件名|
|dbname/LOCK|DB锁文件|
|dbname/LOG|info log日志文件|
|dbname/LOG.old|旧的info log日志文件|
上面的log文件，sst文件，临时文件，清单文件末尾都带着序列号，序号是单调递增的（随着next_file_number从1开始递增），以保证不会和之前的文件名重复。另外，注意区分db log与info log：前者是为了防止保障数据安全而实现的二进制Log，后者是打印引擎中间运行状态及警告等信息的文本log。
随着更新与Compaction的进行，Level DB会不断生成新文件，有时还会删除老文件，所以需要一个文件来记录文件列表，这个列表就是清单文件的作用，清单会不断变化，DB需要知道最新的清单文件，必须将清单准备好后原子切换，这就是CURRENT文件的作用，Level DB的清单过程更新如下：
1. 递增清单序号，生成一个新的清单文件。
2. 将此清单文件的名称写入到一个临时文件中。
3. 将临时文件rename为CURRENT。
代码如下：
[cpp][view plain](http://blog.csdn.net/icefireelf/article/details/7515816#)[copy](http://blog.csdn.net/icefireelf/article/details/7515816#)
- Status SetCurrentFile(Env* env, const std::string& dbname,  
-                       uint64_t descriptor_number) {  
- // 创建一个新的清单文件名
-   std::string manifest = DescriptorFileName(dbname, descriptor_number);  
-   Slice contents = manifest;  
- // 移除"dbname/"前缀
-   assert(contents.starts_with(dbname + "/"));  
-   contents.remove_prefix(dbname.size() + 1);  
- // 创建一个临时文件
-   std::string tmp = TempFileName(dbname, descriptor_number);  
- // 写入清单文件名
-   Status s = WriteStringToFile(env, contents.ToString() + "\n", tmp);  
- if (s.ok()) {  
- // 将临时文件改名为CURRENT
-     s = env->RenameFile(tmp, CurrentFileName(dbname));  
-   }  
- if (!s.ok()) {  
-     env->DeleteFile(tmp);  
-   }  
- return s;  
- }  
## 3.2 Manifest
在介绍其他文件格式前，先了解清单文件，MANIFEST文件是Level DB的元信息文件，它主要包括下面一些信息：
1. Comparator的名称
2. 
其的格式如下：
![](https://img-my.csdn.net/uploads/201205/01/1335865966_8376.png)
我们可以看看其序列化的代码：
[cpp][view plain](http://blog.csdn.net/icefireelf/article/details/7515816#)[copy](http://blog.csdn.net/icefireelf/article/details/7515816#)
- void VersionEdit::EncodeTo(std::string* dst) const {  
- if (has_comparator_) {  // 记录Comparator名称
-     PutVarint32(dst, kComdparator);  
-     PutLengthPrefixedSlice(dst, comparator_);  
-   }  
- if (has_log_number_) {  // 记录Log Numer
-     PutVarint32(dst, kLogNumber);  
-     PutVarint64(dst, log_number_);  
-   }  
- if (has_prev_log_number_) {  // 记录Prev Log Number，现在已废弃，一般为0
-     PutVarint32(dst, kPrevLogNumber);  
-     PutVarint64(dst, prev_log_number_);  
-   }  
- if (has_next_file_number_) {  // 记录下一个文件序号
-     PutVarint32(dst, kNextFileNumber);  
-     PutVarint64(dst, next_file_number_);  
-   }  
- if (has_last_sequence_) {  // 记录最大的sequence num
-     PutVarint32(dst, kLastSequence);  
-     PutVarint64(dst, last_sequence_);  
-   }  
- // 记录每一级Level下次compaction的起始Key
- for (size_t i = 0; i < compact_pointers_.size(); i++) {  
-     PutVarint32(dst, kCompactPointer);  
-     PutVarint32(dst, compact_pointers_[i].first);  // level
-     PutLengthPrefixedSlice(dst, compact_pointers_[i].second.Encode());  
-   }  
- // 记录每一级需要删除的文件
- for (DeletedFileSet::const_iterator iter = deleted_files_.begin();  
-        iter != deleted_files_.end();  
-        ++iter) {  
-     PutVarint32(dst, kDeletedFile);  
-     PutVarint32(dst, iter->first);   // level
-     PutVarint64(dst, iter->second);  // file number
-   }  
- // 记录每一级需要有效的sst以及其smallest与largest的key
- for (size_t i = 0; i < new_files_.size(); i++) {  
- const FileMetaData& f = new_files_[i].second;  
-     PutVarint32(dst, kNewFile);  
-     PutVarint32(dst, new_files_[i].first);  // level
-     PutVarint64(dst, f.number);  
-     PutVarint64(dst, f.file_size);  
-     PutLengthPrefixedSlice(dst, f.smallest.Encode());  
-     PutLengthPrefixedSlice(dst, f.largest.Encode());  
-   }  
- }  
## 3.3 Sortedtable
Level DB间歇性地将内存中的SkipList对应的数据集合Dump到磁盘上，生成一个sst的文件，这个文件的格式如下：
![](https://img-my.csdn.net/uploads/201204/27/1335525886_5008.png)
按照SSTable的结构，可以正向遍历，也可以逆向遍历，但是逆向遍历的代价要远远高于正向遍历的代价，因为每条record都是变长的，且其没有记录前一条记录的偏移，因此逆向Group遍历时，只能先回到group（代码中称为一个restart，为了便于理解，下面都称为group）开头（一个Data Block的group一般为16条记录，每个Data Block的尾部有group起始位置偏移索引），然后从头开始正向遍历，直至找到其前一条记录，如果当前位置为group的第一条记录，则需要回到上一个group的开头，遍历到其最后一条记录。另外，内存中跳表反向的遍历效率也远远不如正向遍历。
## 3.4 Sparse Index
一个sst文件内部除了Data Block，还有Index Block，Index Block的结构与Data Block一样，只不过每个group只包含一条记录，即Data Block的最大Key与偏移。其实这里说最大Key并不是很准确，理论上，只要保存最大Key就可以实现二分查找，但是Level DB在这里做了个优化，它并保存最大key，而是保存一个能分隔两个Data Block的最短Key，如：假定Data Block1的最后一个Key为“abcdefg”，Data Block2的第一个Key为“abzxcv”，则index可以记录Data Block1的索引key为“abd”；这样的分割串可以有很多，只要保证Data Block1中的所有Key都小于等于此索引，Data Block2中的所有Key都大于此索引即可。这种优化缩减了索引长度，查询时可以有效减小比较次数。我们可以看看默认comparator如何实现这种分割的：
[cpp][view plain](http://blog.csdn.net/icefireelf/article/details/7515816#)[copy](http://blog.csdn.net/icefireelf/article/details/7515816#)
- void BytewiseComparatorImpl::FindShortestSeparator(  
-       std::string* start,  
- const Slice& limit) const {  
- // 先比较获得最大公共前缀
- size_t min_length = std::min(start->size(), limit.size());  
- size_t diff_index = 0;  
- while ((diff_index < min_length) &&  
-            ((*start)[diff_index] == limit[diff_index])) {  
-       diff_index++;  
-     }  
- if (diff_index >= min_length) {  
- // 如果start就是limit的前缀，则只能使用start本身作为分割
-     } else {  
-       uint8_t diff_byte = static_cast<uint8_t>((*start)[diff_index]);  
- // 将第一个不同字符+1，并确保其不会溢出，同时比limit小
- if (diff_byte < static_cast<uint8_t>(0xff) &&  
-           diff_byte + 1 < static_cast<uint8_t>(limit[diff_index])) {  
-         (*start)[diff_index]++;  
-         start->resize(diff_index + 1);  
-         assert(Compare(*start, limit) < 0);  
-       }  
-     }  
-   }  
从上面可以看出，FindShortestSeparator方法并不严格，有些时候没有找出最短分割的key（比如第一个不等的字符已经为0xFF时），它只是一种优化，我们自定义Comparator时，既可以实现，也可以不实现，如果不实现，将始终使用Data Block的最大Key作为索引，并不影响功能正确性。
# 4. Operations
在介绍了数据结构后，我们看看Level DB一些基本操作的实现：
## 4.1 创建一个新表
创建一个新的表大概分为几步，包括建立各类文件以及内存中的数据结构，线程同步对象等，关键代码如下：
[cpp][view plain](http://blog.csdn.net/icefireelf/article/details/7515816#)[copy](http://blog.csdn.net/icefireelf/article/details/7515816#)
- // DBImpl在构造时会初始化互斥体与信号量，创建一个空的memtable，并根据配置设置Comparator及LRU缓冲
- DBImpl::DBImpl(const Options& options, const std::string& dbname)  
-     : env_(options.env),  
-       internal_comparator_(options.comparator), // 初始化Comparator
-       options_(SanitizeOptions(dbname, &internal_comparator_, options)),  // 检查参数是否合法
-       owns_info_log_(options_.info_log != options.info_log),  // 是拥有自己info log，还是使用用户提供的
-       owns_cache_(options_.block_cache != options.block_cache), // 是否拥有自己的LRU缓冲，或者使用用户提供的
-       dbname_(dbname),  // 数据表名称
-       db_lock_(NULL),  // 不创建也不锁定文件锁
-       shutting_down_(NULL),   
-       bg_cv_(&mutex_),  // 用于与后台线程交互的条件信号
-       mem_(new MemTable(internal_comparator_)), // 创建一个新的跳表
-       imm_(NULL),  // 用于双缓冲的缓冲跳表开始时为NULL
-       logfile_(NULL),  // log文件
-       logfile_number_(0), // log文件的序号
-       log_(NULL),  // log writer
-       logger_(NULL),  // 用于在多线程环境中记录Owner logger的一个指针
-       logger_cv_(&mutex_), // 用于与Logger交互的条件信号
-       bg_compaction_scheduled_(false), // 没打开表时不起动后台的compaction线程
-       manual_compaction_(NULL) {  
- // 增加memtable的引用计数
-   mem_->Ref();  
-   has_imm_.Release_Store(NULL);  
- 
- // 根据Option创建一个LRU的缓冲对象，如果options中指定了Cache空间，则使用用户
- // 提供的Cache空间，否则会在内部确实创建8MB的Cache，另外，LRU的Entry数目不能超过max_open_files-10
- constint table_cache_size = options.max_open_files - 10;  
-   table_cache_ = new TableCache(dbname_, &options_, table_cache_size);  
- 
- // 创建一个Version管理器
-   versions_ = new VersionSet(dbname_, &options_, table_cache_,  
-                              &internal_comparator_);  
- }  
- 
- Options SanitizeOptions(const std::string& dbname,  
- const InternalKeyComparator* icmp,  
- const Options& src) {  
-   Options result = src;  
-   result.comparator = icmp;  
-   ClipToRange(&result.max_open_files,           20,     50000);  
-   ClipToRange(&result.write_buffer_size,        64<<10, 1<<30);  
-   ClipToRange(&result.block_size,               1<<10,  4<<20);  
- // 如果用户未指定info log文件（用于打印状态等文本信息的日志文件），则由引擎自己创建一个info log文件。
- if (result.info_log == NULL) {  
- // Open a log file in the same directory as the db
-     src.env->CreateDir(dbname);  // 如果目录不存在则创建
- // 如果已存在以前的info log文件，则将其改名为LOG.old，然后创建新的log文件与日志的writer
-     src.env->RenameFile(InfoLogFileName(dbname), OldInfoLogFileName(dbname));  
-     Status s = src.env->NewLogger(InfoLogFileName(dbname), &result.info_log);  
- if (!s.ok()) {  
-       result.info_log = NULL;  
-     }  
-   }  
- // 如果用户没指定LRU缓冲，则创建8MB的LRU缓冲
- if (result.block_cache == NULL) {  
-     result.block_cache = NewLRUCache(8 << 20);  
-   }  
- return result;  
- }  
- 
- Status DBImpl::NewDB() {  
- // 创建version管理器
-   VersionEdit new_db;  
- // 设置Comparator
-   new_db.SetComparatorName(user_comparator()->Name());  
-   new_db.SetLogNumber(0);  
- // 下一个序号从2开始，1留给清单文件
-   new_db.SetNextFile(2);  
-   new_db.SetLastSequence(0);  
- // 创建一个清单文件，MANIFEST-1
- const std::string manifest = DescriptorFileName(dbname_, 1);  
-   WritableFile* file;  
-   Status s = env_->NewWritableFile(manifest, &file);  
- if (!s.ok()) {  
- return s;  
-   }  
-   {  
- // 写入清单文件头
-     log::Writer log(file);  
-     std::string record;  
-     new_db.EncodeTo(&record);  
-     s = log.AddRecord(record);  
- if (s.ok()) {  
-       s = file->Close();  
-     }  
-   }  
- delete file;  
- if (s.ok()) {  
- // 设置CURRENT文件，使其指向清单文件
-     s = SetCurrentFile(env_, dbname_, 1);  
-   } else {  
-     env_->DeleteFile(manifest);  
-   }  
- return s;  
## 4.2 打开一个已存在的表
上面的步骤中，其实还遗漏了一个的重要流程，那就是DB的Open方法。Level DB无论是创建表，还是打开现有的表，都是使用Open方法。代码如下：
[cpp][view plain](http://blog.csdn.net/icefireelf/article/details/7515816#)[copy](http://blog.csdn.net/icefireelf/article/details/7515816#)
- Status DB::Open(const Options& options, const std::string& dbname,  
-                 DB** dbptr) {  
-   *dbptr = NULL;  
- 
-   DBImpl* impl = new DBImpl(options, dbname);  
-   impl->mutex_.Lock();  
-   VersionEdit edit;  
- // 如果存在表数据，则Load表数据，并对日志进行恢复，否则，创建新表
-   Status s = impl->Recover(&edit);  
- if (s.ok()) {  
- // 从VersionEdit获取一个新的文件序号，所以如果是新建数据表，则第一个LOG的序号为2（1已经被MANIFEST占用）
-     uint64_t new_log_number = impl->versions_->NewFileNumber();  
- // 记录日志文件号，创建新的log文件及Writer对象
-     WritableFile* lfile;  
-     s = options.env->NewWritableFile(LogFileName(dbname, new_log_number),  
-                                      &lfile);  
- if (s.ok()) {  
-       edit.SetLogNumber(new_log_number);  
-       impl->logfile_ = lfile;  
-       impl->logfile_number_ = new_log_number;  
-       impl->log_ = new log::Writer(lfile);  
- // 如果存在原来的log，则回放log
-       s = impl->versions_->LogAndApply(&edit, &impl->mutex_);  
-     }  
- if (s.ok()) {  
- // 删除废弃的文件（如果存在）
-       impl->DeleteObsoleteFiles();  
- // 检查是否需要Compaction，如果需要，则让后台启动Compaction线程
-       impl->MaybeScheduleCompaction();  
-     }  
-   }  
-   impl->mutex_.Unlock();  
- if (s.ok()) {  
-     *dbptr = impl;  
-   } else {  
- delete impl;  
-   }  
- return s;  
- }  
从上面可以看出，其实到底是新建表还是打开表都是取决与DBImpl::Recover()这个方法的行为，它的流程如下：
[cpp][view plain](http://blog.csdn.net/icefireelf/article/details/7515816#)[copy](http://blog.csdn.net/icefireelf/article/details/7515816#)
- Status DBImpl::Recover(VersionEdit* edit) {  
-   mutex_.AssertHeld();  
- 
- // 创建DB目录，不关注错误
-   env_->CreateDir(dbname_);  
- // 在DB目录下打开或创建（如果不存在）LOCK文件并锁定它，防止其他进程打开此表
-   Status s = env_->LockFile(LockFileName(dbname_), &db_lock_);  
- if (!s.ok()) {  
- return s;  
-   }  
- 
- if (!env_->FileExists(CurrentFileName(dbname_))) {  
- // 如果DB目录下不存在CURRENT文件且允许在表不存在时创建表，则新建一个表返回
- if (options_.create_if_missing) {  
-       s = NewDB();  
- if (!s.ok()) {  
- return s;  
-       }  
-     } else {  
- return Status::InvalidArgument(  
-           dbname_, "does not exist (create_if_missing is false)");  
-     }  
-   } else {  
- if (options_.error_if_exists) {  
- return Status::InvalidArgument(  
-           dbname_, "exists (error_if_exists is true)");  
-     }  
-   }  
- // 如果运行到此，表明表已经存在，需要load，第一步是从MANIFEST文件中恢复VersionSet
-   s = versions_->Recover();  
- if (s.ok()) {  
-     SequenceNumber max_sequence(0);  
- // 获取MANIFEST中获取最后一次持久化清单时在使用LOG文件序号，注意：这个LOG当时正在使用，
- // 表明数据还在memtable中，没有形成sst文件，所以数据恢复需要从这个LOG文件开始(包含这个LOG)。
- // 另外，prev_log是早前版本level_db使用的机制，现在以及不再使用，这里只是为了兼容
- const uint64_t min_log = versions_->LogNumber();  
- const uint64_t prev_log = versions_->PrevLogNumber();  
- // 扫描DB目录，记录下所有比MANIFEST中记录的LOG更加新的LOG文件
-     std::vector<std::string> filenames;  
-     s = env_->GetChildren(dbname_, &filenames);  
- if (!s.ok()) {  
- return s;  
-     }  
-     uint64_t number;  
-     FileType type;  
-     std::vector<uint64_t> logs;  
- for (size_t i = 0; i < filenames.size(); i++) {  
- if (ParseFileName(filenames[i], &number, &type)  
-           && type == kLogFile  
-           && ((number >= min_log) || (number == prev_log))) {  
-         logs.push_back(number);  
-       }  
-     }  
- // 将LOG文件安装从小到大排序
-     std::sort(logs.begin(), logs.end());  
- // 逐个LOG文件回放    for (size_t i = 0; i < logs.size(); i++) {
- // 回放LOG时，记录被插入到memtable，如果超过write buffer，则还会dump出level 0的sst文件，
- // 此方法会将日志种每条记录的sequence num与max_sequence进行比较，以记录下最大的sequence num。
-       s = RecoverLogFile(logs[i], edit, &max_sequence);  
- // 更新最大的文件序号，因为MANIFEST文件中没有记录这些LOG文件占用的序号；
- // 当然，也可能LOG的序号小于MANIFEST中记录的最大文件序号，这时不需要更新。
-       versions_->MarkFileNumberUsed(logs[i]);  
-     }  
- if (s.ok()) {  
- // 比较日志回放前后的最大sequence num，如果回放记录中有超过LastSequence()的记录，则替换
- if (versions_->LastSequence() < max_sequence) {  
-         versions_->SetLastSequence(max_sequence);  
-       }  
-     }  
-   }  
- return s;  
- }     
## 4.3 关闭一个已打开的表
Level DB设计成只要删除DB对象就可以关闭表，其关键流程如下：
[cpp][view plain](http://blog.csdn.net/icefireelf/article/details/7515816#)[copy](http://blog.csdn.net/icefireelf/article/details/7515816#)
- DBImpl::~DBImpl() {  
- // 通知后台线程，DB即将关闭
-   mutex_.Lock();  
- // 后台线程会间歇性地检查shutting_down_对象的指针，一旦不为NULL就会退出
-   shutting_down_.Release_Store(this);  
- // 注意:这里必须循环通知，直至compaction线程获得信号并设置了bg_compaction_scheduled_为false  
- while (bg_compaction_scheduled_) {  
-     bg_cv_.Wait();  
-   }  
-   mutex_.Unlock();  
- 
- // 如果锁定了文件锁，则释放文件锁
- if (db_lock_ != NULL) {  
-     env_->UnlockFile(db_lock_);  
-   }  
- 
- delete versions_;  
- // 减去memtable的引用计数
- if (mem_ != NULL) mem_->Unref();  
- if (imm_ != NULL) imm_->Unref();  
- // 销毁db log相关对象以及表缓冲对象
- delete log_;  
- delete logfile_;  
- delete table_cache_;  
- 
- // 如果info log与cache是引擎自己构建，则需要销毁它们
- if (owns_info_log_) {  
- delete options_.info_log;  
-   }  
- if (owns_cache_) {  
- delete options_.block_cache;  
-   }  
- }  
由上可见，delete一个db对象可能会阻塞调用线程一段时间，必须让其完成一些必须完成的工作，才能进一步保障数据的安全。
## 4.4 随机查询
Level DB可能dump多个sst文件，这些文件的key范围可能重叠。按照Level DB的设计，其会将sst分为7个等级，可以视为代龄，其中，只有Level 0中的sst可能存在key的区间重叠的情况，而level1 - level6中，同一level中的sst可以保证不重叠，但不同level之间的sst依然可能key重叠。因此，如果查询一个key，其最多可能在6+n个sst中同时存在，n为level0中sst的个数；同时，由于这些文件的生成有先后关系，查询时还需要注意顺序，Get一个key的流程如下：
![](https://img-my.csdn.net/uploads/201204/30/1335772471_9154.png)
