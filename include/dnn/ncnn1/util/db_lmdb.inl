

#include "lmdb/lmdb.inl"
enum DBMode { DB_READ, DB_WRITE, DB_NEW };
enum DBSEEK { DB_FIRST, DB_NEXT, DB_PREV, DB_LAST };

struct Cursor {
  int (*Seek)(struct Cursor* s, enum DBSEEK);
  void (*Free)(struct Cursor* s);
  buf_t key[1];
  buf_t value[1];
  int x[32];
};

#define db_Close(db)  db->Close(db)
#define db_Commit(db, keys, values, len)  db->Commit(db, keys, values, len)
#define db_initCursor(db, cursor)  db->initCursor(db, cursor)
#define db_initTransaction(db, txn)  db->initTransaction(db, txn)

struct DB {
  void (*Close)(struct DB* db);
  int (*Commit)(struct DB* db, const buf_t* keys, const buf_t* values, int len);
  int (*initCursor)(struct DB* db, struct Cursor* cursor);
  int x[32];
};

inline void MDB_CHECK(int mdb_status) {
  if (mdb_status != MDB_SUCCESS) { printf(mdb_strerror(mdb_status)); }
}

struct LMDBCursor  {
  MDB_txn* mdb_txn_;
  MDB_cursor* mdb_cursor_;
};

static enum MDB_cursor_op to_MDB_cursor_op(enum DBSEEK op) {
  switch (op) {
#define CURSOR_OP_DEF(NAME)  case DB_ ## NAME: return MDB_ ## NAME;
  CURSOR_OP_DEF(FIRST)
  CURSOR_OP_DEF(NEXT)
  CURSOR_OP_DEF(PREV)
  CURSOR_OP_DEF(LAST)
#undef CURSOR_OP_DEF
  }
  return MDB_FIRST;
}
static int LMDBCursor_Seek(struct Cursor* s, enum DBSEEK op) {
  struct LMDBCursor* x = (struct LMDBCursor*)s->x;
  MDB_val mdb_key_, mdb_value_;
  int mdb_status = mdb_cursor_get(x->mdb_cursor_, &mdb_key_, &mdb_value_, to_MDB_cursor_op(op));
  s->key->data = (uchar*)mdb_key_.mv_data;
  s->key->len = mdb_key_.mv_size;
  s->value->data = (uchar*)mdb_value_.mv_data;
  s->value->len = mdb_value_.mv_size;
  return (mdb_status != MDB_NOTFOUND);
}
static void LMDBCursor_Free(struct Cursor* s) {
  struct LMDBCursor* x = (struct LMDBCursor*)s->x;
  mdb_cursor_close(x->mdb_cursor_);
  mdb_txn_abort(x->mdb_txn_);
}

struct LMDB {
  MDB_env* mdb_env_;
  MDB_dbi mdb_dbi_;
};

void LMDB_DoubleMapSize(struct DB* db) {
  struct LMDB* x = (struct LMDB*)db->x;
  struct MDB_envinfo current_info;
  MDB_CHECK(mdb_env_info(x->mdb_env_, &current_info));
  size_t new_size = current_info.me_mapsize * 2;
  printf("Doubling LMDB map size to %d MB ...", (new_size>>20));
  MDB_CHECK(mdb_env_set_mapsize(x->mdb_env_, new_size));
}

int LMDB_Commit(struct DB* db, const buf_t* keys, const buf_t* values, int len) {
  struct LMDB* x = (struct LMDB*)db->x;
  MDB_dbi mdb_dbi;
  MDB_val mdb_key, mdb_data;
  MDB_txn *mdb_txn;

  // Initialize MDB variables
  MDB_CHECK(mdb_txn_begin(x->mdb_env_, NULL, 0, &mdb_txn));
  MDB_CHECK(mdb_dbi_open(mdb_txn, NULL, 0, &mdb_dbi));

  for (int i = 0; i < len; i++) {
    mdb_key.mv_size = keys[i].len;
    mdb_key.mv_data = (char*)(keys[i].data);
    mdb_data.mv_size = values[i].len;
    mdb_data.mv_data = (char*)(values[i].data);

    // Add data to the transaction
    int put_rc = mdb_put(mdb_txn, mdb_dbi, &mdb_key, &mdb_data, 0);
    if (put_rc == MDB_MAP_FULL) {
      // Out of memory - double the map size and retry
      mdb_txn_abort(mdb_txn);
      mdb_dbi_close(x->mdb_env_, mdb_dbi);
      LMDB_DoubleMapSize(db);
      return LMDB_Commit(db, keys, values, len);
    }
    // May have failed for some other reason
    MDB_CHECK(put_rc);
  }

  // Commit the transaction
  int commit_rc = mdb_txn_commit(mdb_txn);
  if (commit_rc == MDB_MAP_FULL) {
    // Out of memory - double the map size and retry
    mdb_dbi_close(x->mdb_env_, mdb_dbi);
    LMDB_DoubleMapSize(db);
    return LMDB_Commit(db, keys, values, len);
  }
  // May have failed for some other reason
  MDB_CHECK(commit_rc);

  // Cleanup after successful commit
  mdb_dbi_close(x->mdb_env_, mdb_dbi);
  return true;
}

void LMDB_Close(struct DB* db) {
  struct LMDB* x = (struct LMDB*)db->x;
  if (x->mdb_env_ != NULL) {
    mdb_dbi_close(x->mdb_env_, x->mdb_dbi_);
    mdb_env_close(x->mdb_env_);
    x->mdb_env_ = NULL;
  }
}

int LMDB_initCursor(struct DB* db, struct Cursor* cursor) {
  struct LMDB* x = (struct LMDB*)db->x;
  struct LMDBCursor* cursor_x = (struct LMDBCursor*)cursor->x;
  MDB_txn* mdb_txn;
  MDB_cursor* mdb_cursor;
  MDB_CHECK(mdb_txn_begin(x->mdb_env_, NULL, MDB_RDONLY, &mdb_txn));
  MDB_CHECK(mdb_dbi_open(mdb_txn, NULL, 0, &x->mdb_dbi_));
  MDB_CHECK(mdb_cursor_open(mdb_txn, x->mdb_dbi_, &mdb_cursor));

  cursor_x->mdb_txn_ = (mdb_txn);
  cursor_x->mdb_cursor_ = (mdb_cursor);
  cursor->Seek = LMDBCursor_Seek;
  cursor->Free = LMDBCursor_Free;
  return LMDBCursor_Seek(cursor, DB_FIRST);
}

int LMDB_Open(struct DB* db, const char* source, enum DBMode mode) {
  struct LMDB* x = (struct LMDB*)db->x;
  db->Close = LMDB_Close;
  db->initCursor = LMDB_initCursor;
  db->Commit = LMDB_Commit;
  MDB_CHECK(mdb_env_create(&x->mdb_env_));
  if (mode == DB_NEW) {
    sys_mkdir(source);
  }
  int flags = 0;
  if (mode == DB_READ) {
    flags = MDB_RDONLY | MDB_NOTLS;
  }
  int rc = mdb_env_open(x->mdb_env_, source, flags, 0664);
#ifndef ALLOW_LMDB_NOLOCK
  MDB_CHECK(rc);
#else
  if (rc == EACCES) {
    LOG(WARNING) << "Permission denied. Trying with MDB_NOLOCK ...";
    // Close and re-open environment handle
    mdb_env_close(x->mdb_env_);
    MDB_CHECK(mdb_env_create(&x->mdb_env_));
    // Try again with MDB_NOLOCK
    flags |= MDB_NOLOCK;
    MDB_CHECK(mdb_env_open(x->mdb_env_, source, flags, 0664));
  } else {
    MDB_CHECK(rc);
  }
#endif
  return true;
}
