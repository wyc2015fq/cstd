#ifndef CAFFE_DATA_READER_HPP_
#define CAFFE_DATA_READER_HPP_

#include <map>
#include <string>
#include <vector>

#include "common.hpp"
#include "internal_thread.hpp"
#include "blocking_queue.hpp"
#include "db.hpp"

namespace caffe
{

  /**
   * @brief Reads data from a source to queues available to data layers.
   * A single reading thread is created per source, even if multiple solvers
   * are running in parallel, e.g. for multi-GPU training. This makes sure
   * databases are read sequentially, and that each solver accesses a different
   * subset of the database. Data is distributed to solvers in a round-robin
   * way to keep parallel training deterministic.
   */
  class DataReader
  {
  public:
    static std::mutex bodies_mutex_;

    explicit DataReader(CJSON* param)
      : queue_pair_(new QueuePair(cJSON_GetObjectNumber(param, "prefetch", 4) * cJSON_GetObjectNumber(param, "batch_size", 64)))
    {
      // Get or create a body
      std::lock_guard<std::mutex> lock(bodies_mutex_);
      string key = source_key(param);
      WEAK_PTR(Body) & weak = bodies_[key];
      body_ = weak.lock();
      if (!body_) {
        body_.reset(new Body(param));
        bodies_[key] = WEAK_PTR(Body)(body_);
      }
      body_->new_queue_pairs_.push(queue_pair_);
    }

    ~DataReader()
    {
      string key = source_key(body_->param_);
      body_.reset();
      std::lock_guard<std::mutex> lock(bodies_mutex_);
      if (bodies_[key].expired()) {
        bodies_.erase(key);
      }
    }

    inline const BlockingQueue<Datum*> & free() const {
      return queue_pair_->free_;
    }
    inline const BlockingQueue<Datum*> & full() const {
      return queue_pair_->full_;
    }

  protected:
    // Queue pairs are shared between a body and its readers
    class QueuePair
    {
    public:
      explicit QueuePair(int size)
      {
        // Initialize the free queue with requested number of datums
        for (int i = 0; i < size; ++i) {
          free_.push(new Datum());
        }
      }
      ~QueuePair()
      {
        Datum* datum;
        while (free_.try_pop(&datum)) {
          delete datum;
        }
        while (full_.try_pop(&datum)) {
          delete datum;
        }
      }

      BlockingQueue<Datum*> free_;
      BlockingQueue<Datum*> full_;
    };

    // A single body is created per source
    class Body : public InternalThread
    {
    public:
      explicit Body(CJSON* param)
        : param_(param),
        new_queue_pairs_()
      {
        StartInternalThread();
      }

      virtual ~Body()
      {
        StopInternalThread();
      }

      void InternalThreadEntry()
      {
        SHARED_PTR(db::DB) db(db::GetDB(cJSON_GetObjectString(param_, "backend", "lmdb")));
        db->Open(cJSON_GetObjectString(param_, "source", ""), db::READ);
        SHARED_PTR(db::Cursor) cursor(db->NewCursor());
        vector<SHARED_PTR(QueuePair) > qps;
        try {
          int solver_count = 0 == strcmp(cJSON_GetObjectString(param_, "phase", "TEST"), "TRAIN") ? solver_count() : 1;
          // To ensure deterministic runs, only start running once all solvers
          // are ready. But solvers need to peek on one item during initialization,
          // so read one item, then wait for the next solver.
          for (int i = 0; i < solver_count; ++i) {
            SHARED_PTR(QueuePair) qp(new_queue_pairs_.pop());
            read_one(cursor.get(), qp.get());
            qps.push_back(qp);
          }
          // Main loop
          while (!must_stop()) {
            for (int i = 0; i < solver_count; ++i) {
              read_one(cursor.get(), qps[i].get());
            }
            // Check no additional readers have been created. This can happen if
            // more than one net is trained at a time per process, whether single
            // or multi solver. It might also happen if two data layers have same
            // name and same source.
            CHECK_EQ(new_queue_pairs_.size(), 0);
          }
        }
        catch (std::exception const&) {
          // Interrupted exception is expected on shutdown
        }
      }
      void read_one(db::Cursor* cursor, QueuePair* qp)
      {
        Datum* datum = qp->free_.pop();
        // TODO deserialize in-place instead of copy?
        int i = 0;
        DatumParseFromString(cursor->value(), i, *datum);
        qp->full_.push(datum);
        // go to the next iter
        cursor->Next();
        if (!cursor->valid()) {
          DLOG(INFO) << "Restarting data prefetching from start.";
          cursor->SeekToFirst();
        }
      }

      CJSON* param_;
      BlockingQueue<SHARED_PTR(QueuePair)> new_queue_pairs_;
      friend class DataReader;
    };

    // A source is uniquely identified by its layer name + path, in case
    // the same database is read from two different locations in the net.
    static inline string source_key(CJSON* param) {
      return string(cJSON_GetObjectString(param, "name", "_")) + ":" + cJSON_GetObjectString(param, "source", "_");
    }

    const SHARED_PTR(QueuePair) queue_pair_;
    SHARED_PTR(Body) body_;

    static map<const string, WEAK_PTR(DataReader::Body) > bodies_;

    DISABLE_COPY_AND_ASSIGN(DataReader);
  };

}  // namespace caffe

#endif  // CAFFE_DATA_READER_HPP_
