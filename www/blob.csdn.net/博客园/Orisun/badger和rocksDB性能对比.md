# badger和rocksDB性能对比 - Orisun - 博客园







# [badger和rocksDB性能对比](https://www.cnblogs.com/zhangchaoyang/articles/9427675.html)





![](https://images2018.cnblogs.com/blog/103496/201808/103496-20180808133618694-47462893.png)

结论：
- 从最后一个表格来看，ssd只对batch_read和batch-write操作有优势，而且在多协程的情况下，这个优势也丢失了。
- 从第二和第三个表格来看，badger的write操作比rocksDB慢了一个数量级，而batch_write操作badger又非常快。所以如果你用的是go语言，如果write不是你的主要操作，推荐用badger。



数据集不同、参数不同、测试方法不同都会导致结论不同，以下是我的测试代码。



storage.go


package storage

import (
	"fmt"
)

var storageOpenFunction = map[string]func(path string) (Storage, error){
	"badger":  OpenBadger,
	"rocksdb": OpenRocksdb,
}

type Storage interface {
	Set(k, v []byte, expireAt int64) error
	BatchSet(keys, values [][]byte, expireAts []int64) error
	Get(k []byte) ([]byte, error)
	BatchGet(keys [][]byte) ([][]byte, error)
	Delete(k []byte) error
	BatchDelete(keys [][]byte) error
	Has(k []byte) bool
	IterDB(fn func(k, v []byte) error) int64
	IterKey(fn func(k []byte) error) int64
	Close() error
}

func OpenStorage(storeName string, path string) (Storage, error) {
	if fc, exists := storageOpenFunction[storeName]; exists {
		return fc(path)
	} else {
		return nil, fmt.Errorf("unsupported storage engine: %v", storeName)
	}
}


rocks.go



package storage

import (
	"github.com/tecbot/gorocksdb"
	"os"
	"path"
	"sync/atomic"
)

var (
	rocksOptions = gorocksdb.NewDefaultOptions()
	readOptions  = gorocksdb.NewDefaultReadOptions()
	writeOptions = gorocksdb.NewDefaultWriteOptions()
)

type Rocksdb struct {
	db *gorocksdb.DB
}

func OpenRocksdb(dbPath string) (Storage, error) {
	if err := os.MkdirAll(path.Dir(dbPath), os.ModePerm); err != nil { //如果dbPath对应的文件夹已存在则什么都不做，如果dbPath对应的文件已存在则返回错误
		return nil, err
	}

	rocksOptions.SetCreateIfMissing(true)
	rocksOptions.SetCompression(gorocksdb.NoCompression)
	rocksOptions.SetWriteBufferSize(1000000)

	db, err := gorocksdb.OpenDb(rocksOptions, dbPath)
	if err != nil {
		panic(err)
	}

	return &Rocksdb{db: db}, err
}

func (s *Rocksdb) Set(k, v []byte, expireAt int64) error {
	return s.db.Put(writeOptions, k, v)
}

func (s *Rocksdb) BatchSet(keys, values [][]byte, expireAts []int64) error {
	wb := gorocksdb.NewWriteBatch()
	defer wb.Destroy()
	for i, key := range keys {
		value := values[i]
		wb.Put(key, value)
	}
	s.db.Write(writeOptions, wb)
	return nil
}

func (s *Rocksdb) Get(k []byte) ([]byte, error) {
	return s.db.GetBytes(readOptions, k)
}

func (s *Rocksdb) BatchGet(keys [][]byte) ([][]byte, error) {
	var slices gorocksdb.Slices
	var err error
	slices, err = s.db.MultiGet(readOptions, keys...)
	if err == nil {
		values := make([][]byte, 0, len(slices))
		for _, slice := range slices {
			values = append(values, slice.Data())
		}
		return values, nil
	}
	return nil, err
}

func (s *Rocksdb) Delete(k []byte) error {
	return s.db.Delete(writeOptions, k)
}

func (s *Rocksdb) BatchDelete(keys [][]byte) error {
	wb := gorocksdb.NewWriteBatch()
	defer wb.Destroy()
	for _, key := range keys {
		wb.Delete(key)
	}
	s.db.Write(writeOptions, wb)
	return nil
}

//Has
func (s *Rocksdb) Has(k []byte) bool {
	values, err := s.db.GetBytes(readOptions, k)
	if err == nil && len(values) > 0 {
		return true
	}
	return false
}

func (s *Rocksdb) IterDB(fn func(k, v []byte) error) int64 {
	var total int64
	iter := s.db.NewIterator(readOptions)
	defer iter.Close()
	for iter.SeekToFirst(); iter.Valid(); iter.Next() {
		//k := make([]byte, 4)
		//copy(k, iter.Key().Data())
		//value := iter.Value().Data()
		//v := make([]byte, len(value))
		//copy(v, value)
		//fn(k, v)
		if err := fn(iter.Key().Data(), iter.Value().Data()); err == nil {
			atomic.AddInt64(&total, 1)
		}
	}
	return atomic.LoadInt64(&total)
}

func (s *Rocksdb) IterKey(fn func(k []byte) error) int64 {
	var total int64
	iter := s.db.NewIterator(readOptions)
	defer iter.Close()
	for iter.SeekToFirst(); iter.Valid(); iter.Next() {
		//k := make([]byte, 4)
		//copy(k, iter.Key().Data())
		//fn(k)
		if err := fn(iter.Key().Data()); err == nil {
			atomic.AddInt64(&total, 1)
		}
	}
	return atomic.LoadInt64(&total)
}

func (s *Rocksdb) Close() error {
	s.db.Close()
	return nil
}


badger.go



package storage

import (
	"github.com/dgraph-io/badger"
	"os"
	"path"
	"time"
	"github.com/pkg/errors"
	"fmt"
	"sync/atomic"
	"github.com/dgraph-io/badger/options"
)

type Badger struct {
	db *badger.DB
}

var badgerOptions = badger.Options{
	DoNotCompact:        false,    //LSM tree最主要的性能消耗在于 compaction 过程：多个文件需要读进内存，排序，然后再写回磁盘
	LevelOneSize:        64 << 20, //第一层大小
	LevelSizeMultiplier: 10,       //下一层是上一层的多少倍
	MaxLevels:           7,        //LSM tree最多几层
	//key存在内存中，values(实际上value指针)存在磁盘中--称为vlog file
	TableLoadingMode:        options.MemoryMap, //LSM tree完全载入内存
	ValueLogLoadingMode:     options.FileIO,    //使用FileIO而非MemoryMap可以节省大量内存
	MaxTableSize:            4 << 20,           //4M
	NumCompactors:           8,                 //compaction线程数
	NumLevelZeroTables:      4,
	NumLevelZeroTablesStall: 10,
	NumMemtables:            4,     //写操作立即反应在MemTable上，当MemTable达到一定的大小时，它被刷新到磁盘，作为一个不可变的SSTable
	SyncWrites:              false, //异步写磁盘。即实时地去写内存中的LSM tree，当数据量达到MaxTableSize时，才对数据进行compaction然后写入磁盘。当调用Close时也会把内存中的数据flush到磁盘
	NumVersionsToKeep:       1,
	ValueLogFileSize:        64 << 20, //单位：字节。vlog文件超过这么大时就分裂文件。64M
	ValueLogMaxEntries:      100000,
	ValueThreshold:          32,
	Truncate:                false,
}

//var badgerOptions = badger.DefaultOptions

func OpenBadger(dbPath string) (Storage, error) {
	if err := os.MkdirAll(path.Dir(dbPath), os.ModePerm); err != nil { //如果dbPath对应的文件夹已存在则什么都不做，如果dbPath对应的文件已存在则返回错误
		return nil, err
	}

	badgerOptions.Dir = dbPath
	badgerOptions.ValueDir = dbPath
	db, err := badger.Open(badgerOptions) //文件只能被一个进程使用，如果不调用Close则下次无法Open。手动释放锁的办法：把LOCK文件删掉
	if err != nil {
		panic(err)
	}

	return &Badger{db}, err
}

func (s *Badger) CheckAndGC() {
	lsmSize1, vlogSize1 := s.db.Size()
	for {
		if err := s.db.RunValueLogGC(0.5); err == badger.ErrNoRewrite || err == badger.ErrRejected {
			break
		}
	}
	lsmSize2, vlogSize2 := s.db.Size()
	if vlogSize2 < vlogSize1 {
		fmt.Printf("badger before GC, LSM %d, vlog %d. after GC, LSM %d, vlog %d\n", lsmSize1, vlogSize1, lsmSize2, vlogSize2)
	} else {
		fmt.Println("collect zero garbage")
	}
}

//Set 为单个写操作开一个事务
func (s *Badger) Set(k, v []byte, expireAt int64) error {
	err := s.db.Update(func(txn *badger.Txn) error { //db.Update相当于打开了一个读写事务:db.NewTransaction(true)。用db.Update的好处在于不用显式调用Txn.Commit()了
		duration := time.Duration(expireAt-time.Now().Unix()) * time.Second
		return txn.SetWithTTL(k, v, duration) //duration是能存活的时长
	})
	return err
}

//BatchSet 多个写操作使用一个事务
func (s *Badger) BatchSet(keys, values [][]byte, expireAts []int64) error {
	if len(keys) != len(values) {
		return errors.New("key value not the same length")
	}
	var err error
	txn := s.db.NewTransaction(true)
	for i, key := range keys {
		value := values[i]
		duration := time.Duration(expireAts[i]-time.Now().Unix()) * time.Second
		//fmt.Println("duration",duration)
		if err = txn.SetWithTTL(key, value, duration); err != nil {
			_ = txn.Commit(nil) //发生异常时就提交老事务，然后开一个新事务，重试set
			txn = s.db.NewTransaction(true)
			_ = txn.SetWithTTL(key, value, duration)
		}
	}
	txn.Commit(nil)
	return err
}

//Get 如果key不存在会返回error:Key not found
func (s *Badger) Get(k []byte) ([]byte, error) {
	var ival []byte
	err := s.db.View(func(txn *badger.Txn) error { //db.View相当于打开了一个读写事务:db.NewTransaction(true)。用db.Update的好处在于不用显式调用Txn.Discard()了
		item, err := txn.Get(k)
		if err != nil {
			return err
		}
		//buffer := make([]byte, badgerOptions.ValueLogMaxEntries)
		//ival, err = item.ValueCopy(buffer) //item只能在事务内部使用，如果要在事务外部使用需要通过ValueCopy
		ival, err = item.Value()
		return err
	})
	return ival, err
}

//BatchGet 返回的values与传入的keys顺序保持一致。如果key不存在或读取失败则对应的value是空数组
func (s *Badger) BatchGet(keys [][]byte) ([][]byte, error) {
	var err error
	txn := s.db.NewTransaction(false) //只读事务
	values := make([][]byte, len(keys))
	for i, key := range keys {
		var item *badger.Item
		item, err = txn.Get(key)
		if err == nil {
			//buffer := make([]byte, badgerOptions.ValueLogMaxEntries)
			var ival []byte
			//ival, err = item.ValueCopy(buffer)
			ival, err = item.Value()
			if err == nil {
				values[i] = ival
			} else { //拷贝失败
				values[i] = []byte{} //拷贝失败就把value设为空数组
			}
		} else { //读取失败
			values[i] = []byte{} //读取失败就把value设为空数组
			if err != badger.ErrKeyNotFound { //如果真的发生异常，则开一个新事务继续读后面的key
				txn.Discard()
				txn = s.db.NewTransaction(false)
			}
		}
	}
	txn.Discard() //只读事务调Discard就可以了，不需要调Commit。Commit内部也会调Discard
	return values, err
}

//Delete
func (s *Badger) Delete(k []byte) error {
	err := s.db.Update(func(txn *badger.Txn) error {
		return txn.Delete(k)
	})
	return err
}

//BatchDelete
func (s *Badger) BatchDelete(keys [][]byte) error {
	var err error
	txn := s.db.NewTransaction(true)
	for _, key := range keys {
		if err = txn.Delete(key); err != nil {
			_ = txn.Commit(nil) //发生异常时就提交老事务，然后开一个新事务，重试delete
			txn = s.db.NewTransaction(true)
			_ = txn.Delete(key)
		}
	}
	txn.Commit(nil)
	return err
}

//Has 判断某个key是否存在
func (s *Badger) Has(k []byte) bool {
	var exists bool = false
	s.db.View(func(txn *badger.Txn) error { //db.View相当于打开了一个读写事务:db.NewTransaction(true)。用db.Update的好处在于不用显式调用Txn.Discard()了
		_, err := txn.Get(k)
		if err != nil {
			return err
		} else {
			exists = true //没有任何异常发生，则认为k存在。如果k不存在会发生ErrKeyNotFound
		}
		return err
	})
	return exists
}

//IterDB 遍历整个DB
func (s *Badger) IterDB(fn func(k, v []byte) error) int64 {
	var total int64
	s.db.View(func(txn *badger.Txn) error {
		opts := badger.DefaultIteratorOptions
		it := txn.NewIterator(opts)
		defer it.Close()
		for it.Rewind(); it.Valid(); it.Next() {
			item := it.Item()
			key := item.Key()
			val, err := item.Value()
			if err != nil {
				continue
			}
			if err := fn(key, val); err == nil {
				atomic.AddInt64(&total, 1)
			}
		}
		return nil
	})
	return atomic.LoadInt64(&total)
}

//IterKey 只遍历key。key是全部存在LSM tree上的，只需要读内存，所以很快
func (s *Badger) IterKey(fn func(k []byte) error) int64 {
	var total int64
	s.db.View(func(txn *badger.Txn) error {
		opts := badger.DefaultIteratorOptions
		opts.PrefetchValues = false //只需要读key，所以把PrefetchValues设为false
		it := txn.NewIterator(opts)
		defer it.Close()
		for it.Rewind(); it.Valid(); it.Next() {
			item := it.Item()
			k := item.Key()
			if err := fn(k); err == nil {
				atomic.AddInt64(&total, 1)
			}
		}
		return nil
	})
	return atomic.LoadInt64(&total)
}

func (s *Badger) Size() (int64, int64) {
	return s.db.Size()
}

//Close 把内存中的数据flush到磁盘，同时释放文件锁
func (s *Badger) Close() error {
	return s.db.Close()
}


compare.go

package main

import (
	"crypto/md5"
	"encoding/hex"
	"math/rand"
	"pkg/radic/storage"
	"time"
	"fmt"
	"sync"
	"sync/atomic"
)

const (
	KEY_LEN   = 30
	VALUE_LEN = 1000
)

func checksum(data []byte) string {
	checksum := md5.Sum(data)
	return hex.EncodeToString(checksum[:])
}

func Bytes(n int) []byte {
	d := make([]byte, n)
	rand.Read(d)

	return d
}

type src struct {
	Data     []byte
	Checksum string
}

func prepareData(n int) src {
	data := Bytes(n)
	checksum := md5.Sum(data)
	return src{Data: data, Checksum: hex.EncodeToString(checksum[:])}
}

func TestWriteAndGet(db storage.Storage, parallel int) {
	var writeTime int64
	var readTime int64
	var writeCount int64
	var readCount int64
	wg := sync.WaitGroup{}
	wg.Add(parallel)
	for r := 0; r < parallel; r++ {
		go func() {
			defer wg.Done()
			EXPIRE_AT := time.Now().Add(100 * time.Minute).Unix()
			keys := [][]byte{}
			values := [][]byte{}
			validations := []string{}
			const loop = 100000
			for i := 0; i < loop; i++ {
				key := prepareData(KEY_LEN).Data
				keys = append(keys, key)
				value := prepareData(VALUE_LEN)
				values = append(values, value.Data)
				validations = append(validations, value.Checksum)
			}
			begin := time.Now()
			for i, key := range keys {
				value := values[i]
				db.Set(key, value, EXPIRE_AT)
			}
			atomic.AddInt64(&writeTime, time.Since(begin).Nanoseconds())
			atomic.AddInt64(&writeCount, int64(len(keys)))

			begin = time.Now()
			for _, key := range keys {
				db.Get(key)
			}
			atomic.AddInt64(&readTime, time.Since(begin).Nanoseconds())
			atomic.AddInt64(&readCount, int64(len(keys)))
		}()
	}
	wg.Wait()

	fmt.Printf("write %d op/ns, read %d op/ns\n", atomic.LoadInt64(&writeTime)/atomic.LoadInt64(&writeCount), atomic.LoadInt64(&readTime)/atomic.LoadInt64(&readCount))
}

func TestBatchWriteAndGet(db storage.Storage, parallel int) {
	var writeTime int64
	var readTime int64
	var writeCount int64
	var readCount int64

	loop := 100
	wg := sync.WaitGroup{}
	wg.Add(parallel)
	for r := 0; r < parallel; r++ {
		go func() {
			defer wg.Done()
			for i := 0; i < loop; i++ {
				EXPIRE_AT := time.Now().Add(100 * time.Minute).Unix()
				keys := [][]byte{}
				values := [][]byte{}
				expire_ats := []int64{}
				for j := 0; j < 1000; j++ {
					key := prepareData(KEY_LEN).Data
					keys = append(keys, key)
					value := prepareData(VALUE_LEN).Data
					values = append(values, value)
					expire_ats = append(expire_ats, EXPIRE_AT)
				}
				begin := time.Now()
				db.BatchSet(keys, values, expire_ats)
				atomic.AddInt64(&writeTime, time.Since(begin).Nanoseconds())
				atomic.AddInt64(&writeCount, 1)

				begin = time.Now()
				db.BatchGet(keys)
				atomic.AddInt64(&readTime, time.Since(begin).Nanoseconds())
				atomic.AddInt64(&readCount, 1)
			}
		}()
	}
	wg.Wait()

	fmt.Printf("batch write %d op/ns, batch read %d op/ns\n", atomic.LoadInt64(&writeTime)/atomic.LoadInt64(&writeCount), atomic.LoadInt64(&readTime)/atomic.LoadInt64(&readCount))
}

func main() {
	badger, _ := storage.OpenStorage("badger", "badgerdb")
	rocks, _ := storage.OpenStorage("rocksdb", "rocksdb")
	TestWriteAndGet(badger, 1)
	TestWriteAndGet(rocks, 1)
	TestBatchWriteAndGet(badger, 1)
	TestBatchWriteAndGet(rocks, 1)
	fmt.Println("parallel test")
	TestWriteAndGet(badger, 10)
	TestWriteAndGet(rocks, 10)
	TestBatchWriteAndGet(badger, 10)
	TestBatchWriteAndGet(rocks, 10)
	fmt.Println("please watch the memory")
	fmt.Println("rocksdb......")
	rocks.IterDB(func(k, v []byte) error {
		return nil
	})
	fmt.Println("badger......")
	badger.IterDB(func(k, v []byte) error {
		return nil
	})
}


















