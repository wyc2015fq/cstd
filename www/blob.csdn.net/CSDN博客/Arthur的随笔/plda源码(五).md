# plda源码(五) - Arthur的随笔 - CSDN博客
2019年01月07日 12:52:54[largetalk](https://me.csdn.net/largetalk)阅读数：90标签：[plda](https://so.csdn.net/so/search/s.do?q=plda&t=blog)
个人分类：[plda](https://blog.csdn.net/largetalk/article/category/8532621)
## plda源码(五)
model_base.h
存储所有word的topic分布
```
// The ModelBase class stores topic-word co-occurrence count vectors as
// well as a vector of global topic occurrence counts.  The global vector is
// the sum of the other vectors.  These vectors are precisely the components of
// an LDA model (as document-topic associations are not true model parameters).
//
// This class supports common operations on this sort of model, primarily in
// the form of assigning new topic occurrences to words, and in reassigning
// word occurrences from one topic to another.
//
// This class is not thread-safe.  Do not share an object of this
// class by multiple threads.
template <class T>
class ModelBase {
 public:
  // An iterator over a LDA model.  Returns distributions in an arbitrary
  // order.  Altering the parent LDA model in any way invalidates the
  // iterator, although this is not currently enforced.
  class Iterator {//返回词的topic分布的迭代器
   public:
    // Initializes the iterator for the model specified by parent.  parent must
    // exist and must not be modified for the lifetime of the iterator.
    explicit Iterator(const ModelBase<T>* parent)
        : parent_(parent),
          iterator_(0) { }
    ~Iterator() { }
    // Advances to the next word.
    void Next() {
      CHECK(!Done());
      ++iterator_;
    }
    // Returns true if we have finished iterating over the model.
    bool Done() const {
      return iterator_ == parent_->topic_distributions_.size();
    }
    // Returns the current word.
    int Word() const {
      CHECK(!Done());
      return iterator_;
    }
    // Returns the current word's distribution.
    const TopicDistribution<T>& Distribution() const {
      CHECK(!Done());
      return parent_->GetWordTopicDistribution(iterator_);
    }
   private:
    const ModelBase<T>* parent_;
    int iterator_;
  };
  friend class Iterator;
  ModelBase(int num_topics, const map<string, int>& word_index_map) {//知道topic数量和word_index_map, 初始分布为0
    int vocab_size = word_index_map.size();
    n_kw_vec.resize(vocab_size);
    memory_alloc_.resize((int64)num_topics * ((int64)vocab_size + 1), 0);
    // topic_distribution and global_distribution are just accessor pointers
    // and are not responsible for allocating/deleting memory.
    topic_distributions_.resize(vocab_size);
    global_distribution_.Reset(
        &memory_alloc_[0] + (int64)vocab_size * num_topics, num_topics);
    for (int i = 0; i < vocab_size; ++i) {
      topic_distributions_[i] =
        TopicDistribution<T>(
            &memory_alloc_[0] + (int64)num_topics * i, num_topics);
    }
    word_index_map_ = word_index_map;
  }
  // Read word topic distribution and global distribution from iframe.
  // Return a map from word string to index. Intenally we use int to represent
  // each word.
  ModelBase(std::istream& in, map<string, int>* word_index_map) {
  //从流中读取topic分布,自己构建word_index_map
    word_index_map_.clear();
    memory_alloc_.clear();
    string line;
    while (getline(in, line)) {  // Each line is a training document.
      if (line.size() > 0 &&     // Skip empty lines.
          line[0] != '\r' &&     // Skip empty lines.
          line[0] != '\n' &&     // Skip empty lines.
          line[0] != '#') {      // Skip comment lines.
        std::istringstream ss(line);
        string word;
        double count_float;
        CHECK(ss >> word);
        while (ss >> count_float) {
          memory_alloc_.push_back((T)count_float);
        }
        int size = word_index_map_.size();
        word_index_map_[word] = size;
      }
    }
    SetDistributionData();
    *word_index_map = word_index_map_;
  }
  // Read word topic distribution and global distribution from iframe.
  // Only words in the word_index_map are kept.
  // Intenally we use int to represent each word.
  ModelBase(std::istream& in, const map<string, int>& word_index_map) {//从流中读取topic分布，使用指定word_inde_map
    memory_alloc_.clear();
    word_index_map_ = word_index_map;
    int num_topics = 0;
    string line;
    while (getline(in, line)) {  // Each line is a training document.
      if (line.size() > 0 &&     // Skip empty lines.
          line[0] != '\r' &&     // Skip empty lines.
          line[0] != '\n' &&     // Skip empty lines.
          line[0] != '#') {      // Skip comment lines.
        std::istringstream ss(line);
        string word;
        double count_float;
        CHECK(ss >> word);
        if (memory_alloc_.empty()) {
          vector<T> temp;
          while (ss >> count_float) {
            temp.push_back((T)count_float);
          }
          num_topics = temp.size();
          memory_alloc_.resize(
            num_topics * (word_index_map_.size()), (T)0);
          if (word_index_map_.find(word) != word_index_map_.end()) {
            int index = word_index_map_[word];
            typename vector<T>::iterator cpit = memory_alloc_.begin();
            //advance的英文含义是增加、向前推。顾名思义就是讲迭代器向前推n个位置
            advance(cpit, index * num_topics);
            copy(temp.begin(), temp.end(), cpit);
          }
          continue;
        }
        if (word_index_map_.find(word) == word_index_map_.end())
          continue;
        int index = word_index_map_[word] * num_topics;
        while (ss >> count_float) {
          memory_alloc_[index++] = (T)count_float;
        }
      }
    }
    SetDistributionData();
  }
    // Read word topic distribution and global distribution from iframe,extend topic to num_topics.
    // Only words in the word_index_map are kept.
    // Intenally we use int to represent each word.
    ModelBase(std::istream& in, const map<string, int>& word_index_map, int num_topics) {
        memory_alloc_.clear();
        word_index_map_ = word_index_map;
        memory_alloc_.resize(num_topics * (word_index_map_.size()),
                                                 (T) 0);
        n_kw_vec.resize(word_index_map_.size());
        string line;
        while (getline(in, line)) {  // Each line is a training document.
            if (line.size() > 0 &&     // Skip empty lines.
                    line[0] != '\r' &&     // Skip empty lines.
                    line[0] != '\n' &&     // Skip empty lines.
                    line[0] != '#') {      // Skip comment lines.
                std::istringstream ss(line);
                string word;
                double count_float;
                CHECK(ss >> word);
                if (word_index_map_.find(word) == word_index_map_.end())
                    continue;
                int word_id = word_index_map_[word];
                int index = word_id * num_topics;
                int topic_id = 0;
                std::unordered_map<int, int> topic_map;
                while (ss >> count_float) {
                    memory_alloc_[index++] = (T) count_float;
                    if((int)count_float >= 1) {
                        topic_map[topic_id] = (int)count_float;
                    }
                    topic_id++;
                }
                n_kw_vec[word_id] = topic_map;
            }
        }
        SetDistributionData();
    }
  ~ModelBase() {}
  // Returns the topic distribution for word.
  const TopicDistribution<T>& GetWordTopicDistribution(int word) const {
    return topic_distributions_[word];
  }
  // Returns the global topic distribution.
  const TopicDistribution<T>& GetGlobalTopicDistribution() const {
    return global_distribution_;
  }
  // Appends more topics.
  void AppendTopics(int num_new_topics) {//新增topic，num_new_topics > 0
    int num_cur_topics = global_distribution_.size();
    int num_topics = num_cur_topics + num_new_topics;
    int vocab_size = word_index_map_.size();
    memory_alloc_.resize((int64)num_topics * (vocab_size + 1), 0);
    global_distribution_.Reset(
        &memory_alloc_[0] + (int64)vocab_size * num_topics, num_topics);
    for (int i = 0; i < vocab_size; ++i) {
      topic_distributions_[i].Reset(
          &memory_alloc_[0] + (int64)i * num_topics, num_topics);
    }
    typename vector<T>::iterator base_iter_src =
      memory_alloc_.begin() + (int64)vocab_size * num_cur_topics;
    typename vector<T>::iterator base_iter_dst =
      memory_alloc_.begin() + (int64)vocab_size * num_topics;
    fill_n(base_iter_dst + num_cur_topics, num_new_topics, 0);
    copy_backward(base_iter_src, base_iter_src + num_cur_topics,
                  base_iter_dst + num_cur_topics);
    for (int i = vocab_size - 1; i >= 0; --i) {
      base_iter_src = memory_alloc_.begin() + (int64)i * num_cur_topics;
      base_iter_dst = memory_alloc_.begin() + (int64)i * num_topics;
      fill_n(base_iter_dst + num_cur_topics, num_new_topics, 0);
      copy_backward(base_iter_src, base_iter_src + num_cur_topics,
                    base_iter_dst + num_cur_topics);
    }
  }
  // Returns the number of topics in the model.
  int num_topics() const { return global_distribution_.size(); }
  // Returns the number of words in the model (not including the global word).
  int vocab_size() const { return topic_distributions_.size(); }
  // Returns the model memory.
  const vector<T>& model_memory() const { return memory_alloc_; }
  // Output topic_distributions_ into human readable format.
  void AppendAsString(std::ostream& out) const {//输出topic分布
    vector<string> index_word_map(word_index_map_.size());
    for (map<string, int>::const_iterator iter = word_index_map_.begin();
         iter != word_index_map_.end(); ++iter) {
      index_word_map[iter->second] = iter->first;
    }
    for (Iterator iter(this); !iter.Done(); iter.Next()) {
      out << index_word_map[iter.Word()] << "\t";
      for (int topic = 0; topic < num_topics(); ++topic) {
        out << iter.Distribution()[topic]
            << ((topic < num_topics() - 1) ? " " : "\n");
      }
    }
  }
 protected:
  // The dataset which keep all the model memory.
  vector<T> memory_alloc_;
  // topic_distributions_["word"][k] counts the number of times that
  // word "word" and assigned topic k by a Gibbs sampling iteration.
  vector<TopicDistribution<T> > topic_distributions_;
  // global_distribution_[k] is the number of words in the training
  // corpus that are assigned by topic k.
  TopicDistribution<T> global_distribution_;
  std::vector<std::unordered_map<int, int> > n_kw_vec;
 private:
  void SetDistributionData() {//设置global_distribution_
    int vocab_size = word_index_map_.size();
    int num_topics = memory_alloc_.size() / vocab_size;
    CheckNumTopics(num_topics, true, true);
    memory_alloc_.resize((int64)num_topics * ((int64)vocab_size + 1), 0);
    // topic_distribution and global_distribution are just accessor pointers
    // and are not responsible for allocating/deleting memory.
    topic_distributions_.resize(vocab_size);
    global_distribution_.Reset(
      &memory_alloc_[0] + (int64)vocab_size * num_topics, num_topics);
    for (int i = 0; i < vocab_size; ++i) {
      topic_distributions_[i] =
        TopicDistribution<T>(&memory_alloc_[0] + num_topics * i, num_topics);
    }
    for (int i = 0; i < vocab_size; ++i) {
      for (int j = 0; j < num_topics; ++j) {
        global_distribution_[j] += topic_distributions_[i][j];
      }
    }
  }
  map<string, int> word_index_map_;
};
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190107150013933.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhcmdldGFsaw==,size_16,color_FFFFFF,t_70)
