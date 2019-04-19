# InnoDB数据页结构 - LC900730的博客 - CSDN博客
2017年05月22日 12:01:36[lc900730](https://me.csdn.net/LC900730)阅读数：746
页是InnoDB存储引擎管理数据库的最小磁盘单位； 
页类型为B-tree node的页，存放的即是表中行的实际数据。
## InnoDB数据页
- File Header(文件头)
- Page Header(页头)
- Infimum +Supermum Records
- User Records(用户记录，即行记录)
- Free Space(空闲空间)
- Page Directory(页目录)
- File Trailer(文件结尾信息)
其中File Header 、Page Header、File Trailer的大小是固定的，用来表示该页的一些信息，如Checksum、数据所在的索引层。
### File Header
由8个部分组成，共38个字节
|名称|大小|
|----|----|
|FIL_PAGE_SPACE_OR_CHECKSUM|4|
|FIL_PAGE_OFFSET|4|
|FIL_PAGE_PREV|4|
|FIL_PAGE_NEXT|4|
|FIL_PAGE_LSN|8|
|FIL_PAGE_TYPE|2|
|FIL_PAGE_FILE_FLUSH_LSN|8|
|FIL_PAGE_ARCH_LOG_NO_SPACE_ID|4|
- FIL_PAGE_SPACE_OR_CHECKSUM
代表了页的checksum值； 
- FIL_PAGE_OFFSET
表空间中页的偏移值； 
- FIL_PAGE_PREV
当前页的上一个页； 
- FIL_PAGE_NEXT
当前页的下一个页； 
B+Tree特性决定叶子节点必须是双向列表 
- FIL_PAGE_LSN
该值代表了该页最后被修改的日志序列位置LSN(Log Sequence Number) 
- FIL_PAGE_TYPE
页的类型：
|名称|十六进制|解释|
|----|----|----|
|FIL_PAGE_INDEX|0x45BF|B+树叶子节点|
|FIL_PAGE_UNDO_LOG|0x0002|Undo Log页|
|FIL_PAGE_INODE|0x0003|索引节点|
|FIL_PAGE_IBUF_FREE_LIST|0x0004|Insert Buffer空闲列表|
|FIL_PAGE_TYPE_ALLOCATED|0x0000|该页为最新分配|
|FIL_PAGE_IBUF_BITMAP|0x0005|Insert Buffer位图|
|FIL_PAGE_TYPE_SYS|0x0006|系统页|
|FIL_PAGE_TYPE_TRX_SYS|0x0007|事务系统数据|
|FIL_PAGE_FSP_HDR|0x0008|File Space Header|
|FIL_PAGE_TYPE_XDES|0x0009|扩展描述符|
|FIL_PAGE_TYPE_BLOB|0x000A|BLOB页|
- FIL_PAGE_FILE_FLUSH_LSN
该值仅在数据文件中的一个页中定义，代表文件至少被更新到了该LSN值 
- FIL_PAGE_ARCH_LOG_NO_OR_SPACE_ID
该值仅在数据文件中的一个页中定义，代表文件至少被更新到了该LSN值;
### Page Header
记录数据页的状态信息，由14个部分组成，共56个字节
|名称|大小|
|----|----|
|PAGE_N_DIR_SLOTS|2|
|PAGE_HEAP_TOP|2|
|PAGE_N_HEAP|2|
|PAGE_FREE|2|
|PAGE_GARBAGE|2|
|PAGE_LAST_INSERT|2|
|PAGE_DIRECTION|2|
|PAGE_N_RECS|2|
|PAGE_MAX_TRX_ID|8|
|PAGE_LEVEL|2|
|PAGE_INDEX_ID|8|
|PAGE_BTR_SEG_LEAF|10|
|PAGE_BTR_SEG_TOP|10|
### Page Directory
页目录存放了记录的相对位置(存放的是页的相对位置，而不是偏移量)，有些时候这些记录指针称为slot(槽)或者目录槽(Directory Slots)。InnoDB存储引擎的槽是一个稀疏目录，即一个槽中可能属于多个记录，最少属于4条，最多属于8条记录。 
Slot中记录按照键的顺序存放，这样可以利用二叉查找迅速找到记录的指针。 
B+树本身不能找到具体的一条记录，B+树索引能够找到只是该记录所在的页。数据库把页载入到内存，然后通过Page Diectory再进行二叉查找。
### File Trailer
为了保证页能完整写入磁盘，InnoDB存储引擎页中设置了File Trailer部分。File Trailer只有一个FIL_PAGE_END_LSN部分，占用8个字节。前4个字节代表该页的checksum值，最后4个字节和File Header中的FIL_PAGE_LSN相同。通过这两个值来和File Header中的FIL_PAGE_SPACE_OR_CHKSUM和
