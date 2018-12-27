#ifndef CAFFE_UTIL_DB_HPP
#define CAFFE_UTIL_DB_HPP

#include <string>
#include <sys/stat.h>
#include "std/fileio_c.h"
#include "types.h"
using namespace std;

enum Mode { READ, WRITE, NEW };

class Cursor
{
public:
  Cursor() { }
  virtual ~Cursor() { }
  virtual void SeekToFirst() = 0;
  virtual void Next() = 0;
  virtual string key() = 0;
  virtual string value() = 0;
  virtual bool valid() = 0;
};

class Transaction
{
public:
  Transaction() { }
  virtual ~Transaction() { }
  virtual void Put(const string & key, const string & value) = 0;
  virtual void Commit() = 0;
};

class DB
{
public:
  DB() { }
  virtual ~DB() { }
  virtual void Open(const char* source, Mode mode) = 0;
  virtual void Close() = 0;
  virtual Cursor* NewCursor() = 0;
  virtual Transaction* NewTransaction() = 0;
};

#ifdef _WIN32
#define USE_LMDB
#endif
#ifdef USE_LMDB
#include "lmdb/lmdb.h"

inline void MDB_CHECK(int mdb_status)
{
  CHECK_EQ(mdb_status, MDB_SUCCESS) << mdb_strerror(mdb_status);
}

class LMDBCursor : public Cursor
{
public:
  explicit LMDBCursor(MDB_txn* mdb_txn, MDB_cursor* mdb_cursor)
    : mdb_txn_(mdb_txn), mdb_cursor_(mdb_cursor), valid_(false) {
    SeekToFirst();
  }
  virtual ~LMDBCursor() {
    mdb_cursor_close(mdb_cursor_);
    mdb_txn_abort(mdb_txn_);
  }
  virtual void SeekToFirst() { Seek(MDB_FIRST); }
  virtual void Next() { Seek(MDB_NEXT); }
  virtual string key() {
    return string(static_cast<const char*>(mdb_key_.mv_data), mdb_key_.mv_size);
  }
  virtual string value() {
    return string(static_cast<const char*>(mdb_value_.mv_data),
      mdb_value_.mv_size);
  }
  virtual bool valid() { return valid_; }

private:
  void Seek(MDB_cursor_op op) {
    int mdb_status = mdb_cursor_get(mdb_cursor_, &mdb_key_, &mdb_value_, op);
    if (mdb_status == MDB_NOTFOUND) {
      valid_ = false;
    }
    else {
      MDB_CHECK(mdb_status);
      valid_ = true;
    }
  }

  MDB_txn* mdb_txn_;
  MDB_cursor* mdb_cursor_;
  MDB_val mdb_key_, mdb_value_;
  bool valid_;
};

class LMDBTransaction : public Transaction
{
public:
  explicit LMDBTransaction(MDB_env* mdb_env)
    : mdb_env_(mdb_env) { }
  virtual void Put(const string& key, const string & value)
  {
    keys.push_back(key);
    values.push_back(value);
  }
  virtual void Commit()
  {
    MDB_dbi mdb_dbi;
    MDB_val mdb_key, mdb_data;
    MDB_txn* mdb_txn;
    // Initialize MDB variables
    MDB_CHECK(mdb_txn_begin(mdb_env_, NULL, 0, &mdb_txn));
    MDB_CHECK(mdb_dbi_open(mdb_txn, NULL, 0, &mdb_dbi));
    for (int i = 0; i < keys.size(); i++) {
      mdb_key.mv_size = keys[i].size();
      mdb_key.mv_data = const_cast<char*>(keys[i].data());
      mdb_data.mv_size = values[i].size();
      mdb_data.mv_data = const_cast<char*>(values[i].data());
      // Add data to the transaction
      int put_rc = mdb_put(mdb_txn, mdb_dbi, &mdb_key, &mdb_data, 0);
      if (put_rc == MDB_MAP_FULL) {
        // Out of memory - double the map size and retry
        mdb_txn_abort(mdb_txn);
        mdb_dbi_close(mdb_env_, mdb_dbi);
        DoubleMapSize();
        Commit();
        return;
      }
      // May have failed for some other reason
      MDB_CHECK(put_rc);
    }
    // Commit the transaction
    int commit_rc = mdb_txn_commit(mdb_txn);
    if (commit_rc == MDB_MAP_FULL) {
      // Out of memory - double the map size and retry
      mdb_dbi_close(mdb_env_, mdb_dbi);
      DoubleMapSize();
      Commit();
      return;
    }
    // May have failed for some other reason
    MDB_CHECK(commit_rc);
    // Cleanup after successful commit
    mdb_dbi_close(mdb_env_, mdb_dbi);
    keys.clear();
    values.clear();
  }

private:
  MDB_env* mdb_env_;
  vector<string> keys, values;

  void DoubleMapSize()
  {
    struct MDB_envinfo current_info;
    MDB_CHECK(mdb_env_info(mdb_env_, &current_info));
    size_t new_size = current_info.me_mapsize * 2;
    DLOG(INFO) << "Doubling LMDB map size to " << (int)(new_size >> 20) << "MB ...";
    MDB_CHECK(mdb_env_set_mapsize(mdb_env_, new_size));
  }
};

class LMDB : public DB
{
public:
  LMDB() : mdb_env_(NULL) { }
  virtual ~LMDB() { Close(); }
  virtual void Open(const char* source, Mode mode)
  {
    MDB_CHECK(mdb_env_create(&mdb_env_));
    if (mode == NEW) {
      CHECK_EQ(_mkdir(source), 0) << "mkdir " << source << "failed";
    }
    int flags = 0;
    if (mode == READ) {
      flags = MDB_RDONLY | MDB_NOTLS;
    }
    int rc = mdb_env_open(mdb_env_, source, flags, 0664);
#ifndef ALLOW_LMDB_NOLOCK
    MDB_CHECK(rc);
#else
    if (rc == EACCES) {
      LOG(WARNING) << "Permission denied. Trying with MDB_NOLOCK ...";
      // Close and re-open environment handle
      mdb_env_close(mdb_env_);
      MDB_CHECK(mdb_env_create(&mdb_env_));
      // Try again with MDB_NOLOCK
      flags |= MDB_NOLOCK;
      MDB_CHECK(mdb_env_open(mdb_env_, source.c_str(), flags, 0664));
    }
    else {
      MDB_CHECK(rc);
    }
#endif
    LOG(INFO) << "Opened lmdb " << source;
  }
  virtual void Close() {
    if (mdb_env_ != NULL) {
      mdb_dbi_close(mdb_env_, mdb_dbi_);
      mdb_env_close(mdb_env_);
      mdb_env_ = NULL;
    }
  }
  virtual Cursor* NewCursor()
  {
    MDB_txn* mdb_txn;
    MDB_cursor* mdb_cursor;
    MDB_CHECK(mdb_txn_begin(mdb_env_, NULL, MDB_RDONLY, &mdb_txn));
    MDB_CHECK(mdb_dbi_open(mdb_txn, NULL, 0, &mdb_dbi_));
    MDB_CHECK(mdb_cursor_open(mdb_txn, mdb_dbi_, &mdb_cursor));
    return new LMDBCursor(mdb_txn, mdb_cursor);
  }
  virtual Transaction* NewTransaction()
  {
    return new LMDBTransaction(mdb_env_);
  }

private:
  MDB_env* mdb_env_;
  MDB_dbi mdb_dbi_;
};



#endif

DB* GetDB(DBMethod backend)
{
#ifdef USE_LEVELDB
  if (backend == DBMethod_LEVELDB)) {
  return new LevelDB();
  }
#endif  // USE_LEVELDB
#ifdef USE_LMDB
  if (backend == DBMethod_LMDB) {
    return new LMDB();
  }
#endif  // USE_LMDB
  LOG(FATAL) << "Unknown database backend";
  return NULL;
}

DB* GetDB(const char* backend)
{
#ifdef USE_LEVELDB
  if (0==stricmp(backend, "LEVELDB")) {
    return new LevelDB();
  }
#endif  // USE_LEVELDB
#ifdef USE_LMDB
  if (0 == stricmp(backend, "LMDB")) {
    return new LMDB();
  }
#endif  // USE_LMDB
  LOG(FATAL) << "Unknown database backend";
  return NULL;
}


#endif  // CAFFE_UTIL_DB_HPP
