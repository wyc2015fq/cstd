# plda源码（一） - Arthur的随笔 - CSDN博客
2018年12月14日 15:37:27[largetalk](https://me.csdn.net/largetalk)阅读数：95
common.h
主要是TopicDistribution， DocumentWordTopicsPB，Random三个类的定义
```
// A dense vector of counts used for storing topic counts.
// No memory allocation here, just keep pointers.
template <class T>
class TopicDistribution { //保存一个文档或词对应的topic的分布的类
 public:
  TopicDistribution()
      : distribution_(NULL), size_(0) {
  }
  TopicDistribution(T* distribution, int size)
      : distribution_(distribution), size_(size) {
  }
  void Reset(T* distribution, int size) {
    distribution_ = distribution;
    size_ = size;
  }
  int size() const { return size_; }
  inline T& operator[](int index) const { return distribution_[index]; }
  void clear() { memset(distribution_, 0, sizeof(*distribution_) * size_); }
 private:
  T* distribution_;
  int size_;
};
// A dense vector of probability values representing a discrete
// probability distribution, e.g., the topic distribution of a word.
typedef vector<double> TopicProbDistribution;
// The structure representing a document as a bag of words and the
// topic assigned to each occurrence of a word.  In term of Bayesian
// learning and LDA, the bag of words are ``observable'' data; the
// topic assignments are ``hidden'' data.
struct DocumentWordTopicsPB {//存放一篇文章的词和对应topic的类
  // The document unique words list.
  vector<int> words_; //文章中唯一词列表
  std::unordered_map<int, int> word_to_word_index; //词到词在words_中index的映射
  // Each word occurrance's topic.
  //  wordtopics_.size() = num_words_in_document.
  //  words_.size() = num_unique_words_in_document.
  vector<int16> wordtopics_;//所有词的topic列表
  // A map from words_ to wordtopics_.
  // For example:
  // The document: WORDS1:4  WORD2:2 WORD3:1
  // words_:                     WORD1 WORD2  WORD3
  // wordtopics_start_index_:     |       \      |
  // wordtopics_:                 0 3 4 0  0 3   1
  vector<int16> wordtopics_start_index_;
  DocumentWordTopicsPB() { wordtopics_start_index_.push_back(0); }
  int words_size() const { return words_.size(); }
  int16 wordtopics_count(int word_index) const {
    return wordtopics_start_index_[word_index + 1] - wordtopics_start_index_[word_index];
  }
  int16 word_last_topic_index(int word_index) const {
    return wordtopics_start_index_[word_index + 1] - 1;
  }
  int word(int word_index) const { return words_[word_index]; }
  int16 wordtopics(int index) const { return wordtopics_[index]; }
  int16* mutable_wordtopics(int index) { return &wordtopics_[index]; }
  int word_index(int word) const {
      auto search = word_to_word_index.find(word);
      if (search != word_to_word_index.end()) {
          return search->second;
      }
      return -1;
  }
  void add_wordtopics(int word, const vector<int16>& topics) {
    words_.push_back(word);//在列表尾部追加词
    word_to_word_index[word] = words_.size() - 1;//建立词到index的映射
    wordtopics_start_index_.pop_back();
    wordtopics_start_index_.push_back((int16)wordtopics_.size());//词的topic在wordtopics_中起始位置
    for (size_t i = 0; i < topics.size(); ++i) {
      wordtopics_.push_back(topics[i]);//词对应的topic
    }
    wordtopics_start_index_.push_back((int16)wordtopics_.size());//词的topic在wordtopics_中结束位置
  }
  void CopyFrom(const DocumentWordTopicsPB& instance) { *this = instance; }
};
// Random number generator.
class Random {//随机数生成器
 public:
  Random(bool fixed_seed = false) {
    ResetSeed(fixed_seed);
  }
  void ResetSeed(bool fixed_seed = false) {
    long int seedval;
    if (fixed_seed) {
      // An arbitrary value for previous Xi
      seedval = 123456;
    } else {
      seedval = time(NULL);
    }
    srand48_r(seedval, &buffer_);
  }
  // Generate a random float value in the range of [0,1) from the
  // uniform distribution.
  inline double RandDouble() {
    double result;
    drand48_r(&buffer_, &result);
    return result;
  }
  // Generate a random integer value in the range of [0,bound) from the
  // uniform distribution.
  inline int RandInt(int bound) {
    // NOTE: Do NOT use lrand48_r() % bound, which does not approximate a
    // discrete uniform distribution well.
    return static_cast<int>(RandDouble() * bound);
  }
 private:
  struct drand48_data buffer_;
};
```
[common.cc](http://common.cc)
```
bool CheckNumTopics(int num_topics, bool logs, bool exits_on_error) {//检查topic是否超过short最大值
  bool is_valid = true;
  int64 limit = numeric_limits<short>::max();
  if (num_topics > limit) {
    is_valid = false;
    if (logs) {
      printf("[ERROR] Number of topics > 0x%llX: %d\n", limit, num_topics);
    }
    if (exits_on_error) {
      exit(1);
    }
  }
  return is_valid;
}
bool CheckDocumentSize(int document_size, const char* corpus_file,
                       int line, bool logs, bool exits_on_error) {//检查文档是否超过short最大值
  bool is_valid = true;
  int64 limit = numeric_limits<short>::max();
  if (document_size > limit) {
    is_valid = false;
    if (logs) {
      printf("[WARNING] Document size in L%d of %s > 0x%llX: %d "
             "=> file truncated\n", line, corpus_file, limit, document_size);
    }
    if (exits_on_error) {
      exit(1);
    }
  }
  return is_valid;
}
bool CheckCorpusSize(int64 corpus_size, bool logs, bool exits_on_error) {//检测corpus是否超过int最大值
  bool is_valid = true;
  int64 limit = numeric_limits<int>::max();
  if (corpus_size > limit) {
    is_valid = false;
    if (logs) {
      printf("[ERROR] Corpus size > 0x%llX: %lld\n", limit, corpus_size);
    }
    if (exits_on_error) {
      exit(1);
    }
  }
  return is_valid;
}
void PrintTimeSpan(const char* tag, time_t start_time, int myid) {//打印使用时间
  if (myid != 0) {
    return;
  }
  printf("[TIME] <%s> ", tag);
  time_t end_time = time(NULL);
  double seconds = end_time - start_time;
  if (seconds < 60) {
    printf("%.2fs\n", seconds);
  } else {
    double minutes = seconds / 60;
    if (minutes < 60) {
      printf("%.2fm\n", minutes);
    } else {
      double hours = minutes / 60;
      printf("%.2fh\n", hours);
    }
  }
}
double GetNormalizedLikelihood(double loglikelihood, int64 corpus_size) {
  double normalized = (corpus_size == 0) ? 0 :
      loglikelihood / corpus_size;
  return normalized;
}
void GenerateFileList(const char* input_path, const char* output_path,
                      vector<string>* input_list,
                      vector<string>* output_list) {//准备输入输出文件列表
  input_list->clear();
  output_list->clear();
  struct stat file_stat;
  if (stat(input_path, &file_stat) == 0) {
    if (S_ISREG(file_stat.st_mode)) {
      input_list->push_back(input_path);
      if (output_path != NULL) {
        output_list->push_back(output_path);
      }
    } else if (S_ISDIR(file_stat.st_mode)) {
      string input_dir = input_path;
      if (input_dir[input_dir.length() - 1] != '/') {
        input_dir += '/';
      }
      string output_dir;
      if (output_path != NULL) {
        output_dir = output_path;
        if (output_dir[output_dir.length() - 1] != '/') {
          output_dir += '/';
        }
      }
      DIR* dir = opendir(input_path);
      struct dirent* ent;
      while ((ent = readdir(dir)) != NULL) {
        if (*ent->d_name == '.') {
          continue;
        }
        string input_file = input_dir + ent->d_name;
        if (stat(input_file.c_str(), &file_stat) == 0 &&
            S_ISREG(file_stat.st_mode)) {
          input_list->push_back(input_file);
          if (output_path != NULL) {
            output_list->push_back(output_dir + ent->d_name);
          }
        }
      }
      closedir(dir);
      sort(input_list->begin(), input_list->end());
      sort(output_list->begin(), output_list->end());
    }
  }
}
void TryFilesCreation(const vector<string>& output_list) {//尝试新建文件
  for (vector<string>::const_iterator iter = output_list.begin();
       iter != output_list.end(); ++iter) {
    FILE* file = fopen(iter->c_str(), "wt");
    if (file == NULL) {
      std::cout << "File creation of \"" << *iter << "\" failed!\n";
      exit(-1);
    }
    fclose(file);
  }
}
```
