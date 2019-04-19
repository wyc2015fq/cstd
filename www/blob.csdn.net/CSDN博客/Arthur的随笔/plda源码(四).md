# plda源码(四) - Arthur的随笔 - CSDN博客
2018年12月19日 00:10:01[largetalk](https://me.csdn.net/largetalk)阅读数：85
corpus.h
```
typedef std::list<LDADocument*> LDACorpus;
// Stores multiple documents and manages the memory pool of
// the topic distributions.
class LDACorpusManager {
 public:
  LDACorpusManager(int num_topics, int held_out_base,
                   const vector<string>& input_list,
                   const vector<string>& output_list,
                   Random* random,
                   bool useVocabMap = false,
                   map<string, int> * word_index_map = nullptr,
                   int myid = 0,
                   int pnum = 1
                   );
 private:
  int num_topics_;//topic数
  int held_out_base_;//分离train和test，held_out_base_个train一个test
  int num_my_docs_;//文档数
  LDACorpus corpus_;//trian文档list
  LDACorpus test_corpus_;//test文档list
  int corpus_size_;
  int test_corpus_size_;
  bool distributed_;//true
  Random* random_;//随机数生成器
  int myid_;//0
  int pnum_;//1
  vector<string> input_list_;//输入文件名列表
  vector<string> output_list_;//输出文件名列表
  vector<int> file_size_list_;//输入文件的有效行（文档）数列表
  map<string, int> word_index_map_;//word到index的映射，来自vocabulary
  vector<int16> topic_distribution_pool_;//所有文档的topic分布，长度是num_my_docs_*num_topics_
  bool useVocabMap_;//是否使用vocabulary映射
};
```
[corpus.cc](http://corpus.cc)
```
LDACorpusManager::LDACorpusManager(int num_topics, int held_out_base,
                                   const vector<string>& input_list,
                                   const vector<string>& output_list,
                                   Random* random,
                                   bool useVocabMap,
                                   map<string, int> * word_index_map,
                                   int myid,
                                   int pnum
                                   )
    : num_topics_(num_topics),
      held_out_base_(held_out_base),
      distributed_(true),
      random_(random),
      myid_(myid),
      pnum_(pnum),
      input_list_(input_list),
      output_list_(output_list),
      useVocabMap_(useVocabMap) {
    if (useVocabMap_) {
        word_index_map_ = (*word_index_map);
    }
}
//从input_list读取数据,PrepareLoading是遍历一遍文件，得到文档数量，corpus大小和词到index得索引
void LDACorpusManager::PrepareLoading() {
  time_t start_time = time(NULL);
  int task_index = 0;
  if(!useVocabMap_){
    word_index_map_.clear();
  }
  int map_size = 0;
  int64 corpus_size = 0;
  for (vector<string>::const_iterator iter = input_list_.begin();
       iter != input_list_.end(); ++iter) {
    PrepareFileLoading(iter->c_str(), corpus_size, map_size, task_index);
  }
  AllocateTopicPool();
}
//从input_list读取数据,LoadAndInitTrainingCorpus是遍历一遍文件,初始化LDADocument，train corpus, test corpus
void LDACorpusManager::LoadAndInitTrainingCorpus() {
  time_t start_time = time(NULL);
  int train_test_index = 0;
  int doc_index = 0;
  int task_index = 0;
  for (vector<string>::const_iterator iter = input_list_.begin();
       iter != input_list_.end(); ++iter) {
      file_size_list_.push_back(
          LoadDataFile(iter->c_str(), train_test_index, doc_index, task_index));
  }
  corpus_size_ = GetCorpusSize(corpus_);
  test_corpus_size_ = GetCorpusSize(test_corpus_);
  PrintTimeSpan("load", start_time, myid_);
}
void LDACorpusManager::OutputTopic() {//输出topic
  LDACorpus::const_iterator train_iter = corpus_.begin();
  LDACorpus::const_iterator test_iter = test_corpus_.begin();
  vector<string>::const_iterator file_path_iter = output_list_.begin();
  vector<int>::const_iterator file_size_iter = file_size_list_.begin();
  FILE* topic_file = NULL;
  int cur_file_size = 0;
  int cur_file_index = 0;
  int doc_index = 0;
  while (true) {
    if (cur_file_index == 0) {
      cur_file_size = 0;
      while (file_size_iter != file_size_list_.end()) {
        topic_file = fopen(file_path_iter->c_str(), "wt");
        cur_file_size = *file_size_iter;
        if (cur_file_size > 0) {
          break;
        }
        ++file_path_iter;
        ++file_size_iter;
        fclose(topic_file);
      }
      if (cur_file_size == 0) {
        break;
      }
    }
    // Determines the document container.
    LDACorpus::const_iterator* corpus_iter;
    if (held_out_base_ == 1) {
      corpus_iter = &train_iter;
    } else {
      ++doc_index;
      if (doc_index < held_out_base_) {
        corpus_iter = &train_iter;
      } else {
        corpus_iter = &test_iter;
        doc_index = 0;
      }
    }
    LDADocument* document = **corpus_iter;
    for (LDADocument::WordOccurrenceIterator doc_iter(document);
         !doc_iter.Done(); doc_iter.Next()) {
      fprintf(topic_file, "%d ", doc_iter.Topic());
    }
    fprintf(topic_file, "\n");
    ++cur_file_index;
    if (cur_file_index == cur_file_size) {
      ++file_path_iter;
      ++file_size_iter;
      fclose(topic_file);
      cur_file_index = 0;
    }
    ++(*corpus_iter);
  }
}
//增加topic
void LDACorpusManager::AppendTopics(int num_new_topics) {
  int num_cur_topics = num_topics_;
  num_topics_ += num_new_topics;
  topic_distribution_pool_.resize((int64)num_my_docs_ * num_topics_);
  LDACorpus::reverse_iterator corpus_iter = corpus_.rbegin();
  LDACorpus::reverse_iterator test_corpus_iter = test_corpus_.rbegin();
  LDACorpus::reverse_iterator* iter;
  int train_test_index = (num_my_docs_ - 1) % held_out_base_ + 1;
  for (int i = num_my_docs_ - 1; i >= 0; --i) {
    if (held_out_base_ == 1 || train_test_index < held_out_base_) {
      iter = &corpus_iter;
    } else {
      iter = &test_corpus_iter;
    }
    (*(*iter))->AppendTopics(
        num_new_topics, &topic_distribution_pool_[(int64)i * num_topics_]);
    ++(*iter);
    vector<int16>::iterator base_iter_src =
      topic_distribution_pool_.begin() + (int64)i * num_cur_topics;
    vector<int16>::iterator base_iter_dst =
      topic_distribution_pool_.begin() + (int64)i * num_topics_;
    fill_n(base_iter_dst + num_cur_topics, num_new_topics, 0);
    copy_backward(base_iter_src, base_iter_src + num_cur_topics,
                  base_iter_dst + num_cur_topics);
    --train_test_index;
    if (train_test_index == 0) {
      train_test_index = held_out_base_;
    }
  }
}
void LDACorpusManager::PrepareFileLoading(
    const char* corpus_file, int64& corpus_size,
    int& map_size, int& task_index) {
  ifstream fin(corpus_file);
  string line;
  int line_index = 0;
  while (getline(fin, line)) {  // Each line is a training document.
    if (line.size() == 0 ||     // Skip empty lines.
        line[0] == '\r' ||      // Skip empty lines.
        line[0] == '\n' ||      // Skip empty lines.
        line[0] == '#') {       // Skip comment lines.
      ++line_index;
      continue;
    }
    if (task_index == myid_) {
      ++num_my_docs_;
    }
    istringstream ss(line);
    string word;
    int count;
    int document_size = 0;
    while (ss >> word >> count) {
      map<string, int>::const_iterator iter = word_index_map_.find(word);
      if (useVocabMap_ && iter == word_index_map_.end()) {
          std::cout << "word is not in voc:" << word << std::endl;
          continue;
      }
      document_size += count;
      /*indicate now useVocabMap is false*/
      if (iter == word_index_map_.end()) {
         word_index_map_[word] = map_size++;
      }
    }
    CheckDocumentSize(document_size, corpus_file, line_index, true, false);
    corpus_size += document_size;
    ++task_index;
    if (task_index == pnum_) {
      task_index = 0;
    }
    ++line_index;
  }
}
void LDACorpusManager::AllocateTopicPool() {
  topic_distribution_pool_.resize((int64)num_my_docs_ * num_topics_);
  fill(topic_distribution_pool_.begin(), topic_distribution_pool_.end(), 0);
}
int LDACorpusManager::LoadDataFile(const char* corpus_file,
                                   int& train_test_index, int& doc_index,
                                   int& task_index) {
  ifstream fin(corpus_file);
  string line;
  int num_lines = 0;
  int line_index = 0;
  while (getline(fin, line)) {  // Each line is a training document.
    if (line.size() == 0 ||     // Skip empty lines.
        line[0] == '\r' ||      // Skip empty lines.
        line[0] == '\n' ||      // Skip empty lines.
        line[0] == '#') {       // Skip comment lines.
      ++line_index;
      continue;
    }
    istringstream ss(line);
    string word;
    int count;
    if (task_index == myid_) {
      // This is a document that I need to store in local memory.
      DocumentWordTopicsPB document;
      int document_size = 0;
      while (ss >> word >> count) {  // Load and init a document.
        map<string, int>::const_iterator iter = word_index_map_.find(word);
        if (iter == word_index_map_.end()) continue;
        document_size += count;
        bool size_is_valid = CheckDocumentSize(document_size, corpus_file,
                                               line_index, false, false);
        if (size_is_valid) {
          vector<int16> topics;
          for (int i = 0; i < count; ++i) {
            topics.push_back((int16)random_->RandInt(num_topics_));
          }
          int word_index = iter->second;
          document.add_wordtopics(word_index, topics);
        }
      }
      LDADocument* lda_document = new LDADocument(
          document, num_topics_,
          &topic_distribution_pool_[0] + (int64)doc_index * num_topics_);
      ++doc_index;
      LDACorpus* container = NULL;
      if (held_out_base_ == 1) {
        container = &corpus_;
      } else {
        ++train_test_index;
        if (train_test_index < held_out_base_) {
          container = &corpus_;
        } else {
          container = &test_corpus_;
          train_test_index = 0;
        }
      }
      container->push_back(lda_document);
      ++num_lines;
    } else {
      // This is a document that should be stored by other processors.
      // Just bypassed it.
      while (ss >> word >> count) {
      }
    }
    ++task_index;
    if (task_index == pnum_) {
      task_index = 0;
    }
    ++line_index;
  }
  return num_lines;
}
int64 LDACorpusManager::GetCorpusSize(const LDACorpus& corpus) {
  int64 corpus_size = 0;
  for (LDACorpus::const_iterator iterator = corpus.begin();
       iterator != corpus.end(); ++iterator) {
    corpus_size += (*iterator)->topics().wordtopics_.size();
  }
  return corpus_size;
}
```
