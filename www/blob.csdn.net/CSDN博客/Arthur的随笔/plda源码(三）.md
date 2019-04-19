# plda源码(三） - Arthur的随笔 - CSDN博客
2018年12月18日 16:59:37[largetalk](https://me.csdn.net/largetalk)阅读数：101
document.h
```
class DocumentWordTopicsPB;
// Stores a document as a bag of words and provides methods for interacting
// with Gibbs LDA models.
class LDADocument { //存储一个文档topic分布的类
 public:
  // An iterator over all of the word occurrences in a document.
  class WordOccurrenceIterator { //在词上迭代的类
   public:
    // Intialize the WordOccurrenceIterator for a document.
    explicit WordOccurrenceIterator(LDADocument* parent);
    ~WordOccurrenceIterator();
    // Returns true if we are done iterating.
    bool Done();
    // Advances to the next word occurrence.
    void Next();
    // Returns the topic of the current occurrence.
    int Topic();
    // Changes the topic of the current occurrence.
    void SetTopic(int new_topic);
    // Changes the topic assignment of the current word.
    void SetTopicAssignment(int new_topic);
    // Returns the word of the current occurrence.
    int Word();
   private:
    // If the current word has no occurrences, advance until reaching a word
    // that does have occurrences or the end of the document.
    void SkipWordsWithoutOccurrences();
    void UpdateNMKMap(int new_topic);
    LDADocument* parent_;
    int word_index_;
    int word_topic_index_;
  };
  friend class WordOccurrenceIterator;
  // Initializes a document from a DocumentWordTopicsPB. Usually, this
  // constructor is used in training an LDA model, because the
  // initialization phase creates the model whose vocabulary covers
  // all words appear in the training data.
  LDADocument(const DocumentWordTopicsPB& topics, int num_topics, int16* base);
  virtual ~LDADocument();
  // Returns the document's topic associations.
  DocumentWordTopicsPB& topics() const {
    return *topic_assignments_;
  }
  // Returns the document's topic occurrence counts.
  const int16* topic_distribution() const {
    return topic_distribution_;
  }
  // Returns the mutable document's topic occurrence counts.
  int16* mutable_topic_distribution() {
    return topic_distribution_;
  }
  // Returns the number of topics.
  int num_topics() const { return num_topics_; }
  double* mutable_sum_a2() { return &sum_a2_; }
  void AppendTopics(int num_new_topics, int16* new_base);
  string DebugString();
  const std::unordered_map<int, int>& GetTopicDist() const {
      return n_mk_map;
  }
  int GetDocumentLength() {return document_length;}
 protected:
  DocumentWordTopicsPB*  topic_assignments_;//一个文档所有word和对应的topic
  // Number of topics.
  int num_topics_;//总topic数
  int16* topic_distribution_;//文档在每个topic上的计数
  double sum_a2_;//samper会用到
  int document_length;//文档长度
  // Count topic occurrences in topic_assignments_ and stores the
  // result in topic_distribution_.
  void CountTopicDistribution();
  std::unordered_map<int, int> n_mk_map;//和topic_distribution_一个意思，for spare
};
```
[document.cc](http://document.cc)
```
LDADocument::WordOccurrenceIterator::WordOccurrenceIterator(
    LDADocument* parent) {
  parent_ = parent;
  word_index_ = 0;
  word_topic_index_ = 0;
  SkipWordsWithoutOccurrences();
}
LDADocument::WordOccurrenceIterator::~WordOccurrenceIterator() { }
// Have we advanced beyond the last word?
bool LDADocument::WordOccurrenceIterator::Done() {
  CHECK_GE(parent_->topic_assignments_->words_size(), word_index_);
  return word_index_ == parent_->topic_assignments_->words_size();
}
// We iterate over all the occurrences of each word.  If we have finished with
// the current word, we advance to the 0th ocurrence of the next word that has
// occurrences.
void LDADocument::WordOccurrenceIterator::Next() {
  CHECK(!Done());
  ++word_topic_index_;
  if (word_topic_index_ >
      parent_->topic_assignments_->word_last_topic_index(word_index_)) {
    ++word_index_;
    SkipWordsWithoutOccurrences();
  }
}
int LDADocument::WordOccurrenceIterator::Topic() {
  CHECK(!Done());
  return parent_->topic_assignments_->wordtopics(word_topic_index_);
}
// Exchange the topic.  Be sure to keep the topic count distribution up to
// date.
void LDADocument::WordOccurrenceIterator::SetTopic(int new_topic) {
  CHECK(!Done());
  CHECK_LE(0, new_topic);
  CHECK_GT(parent_->num_topics(), new_topic);
  // Adjust the topic counts before we set the new topic and forget the old
  // one.
  parent_->topic_distribution_[Topic()] -= 1;
  CHECK_LE(0, parent_->topic_distribution_[Topic()]);
  parent_->topic_distribution_[new_topic] += 1;
  CHECK_LE(0, parent_->topic_distribution_[new_topic]);
  UpdateNMKMap(new_topic);
  *(parent_->topic_assignments_->mutable_wordtopics(word_topic_index_)) = new_topic;
}
void LDADocument::WordOccurrenceIterator::SetTopicAssignment(int new_topic) {
  CHECK(!Done());
  CHECK_LE(0, new_topic);
  CHECK_GT(parent_->num_topics(), new_topic);
  UpdateNMKMap(new_topic);
  *(parent_->topic_assignments_->mutable_wordtopics(word_topic_index_)) = new_topic;
}
void LDADocument::WordOccurrenceIterator::UpdateNMKMap(int new_topic) {
  parent_->n_mk_map[Topic()] -= 1;
  CHECK_LE(0, parent_->n_mk_map[Topic()]);
  parent_->n_mk_map[new_topic] += 1;
  CHECK_LE(1, parent_->n_mk_map[new_topic]);
  if (parent_->n_mk_map[Topic()] == 0) {
      parent_->n_mk_map.erase(Topic());
  }
}
int LDADocument::WordOccurrenceIterator::Word() {
  CHECK(!Done());
  return parent_->topic_assignments_->word(word_index_);
}
void LDADocument::WordOccurrenceIterator::SkipWordsWithoutOccurrences() {
  // The second part of the condition means "while the current word has no
  // occurrences" (and thus no topic assignments).
  while (
      !Done() &&
      parent_->topic_assignments_->wordtopics_count(word_index_) == 0) {
    ++word_index_;
  }
}
void LDADocument::CountTopicDistribution() {
  for (int i = 0; i < num_topics_; ++i) {
    topic_distribution_[i] = 0;
  }
  for (WordOccurrenceIterator iter(this); !iter.Done(); iter.Next()) {
    document_length += 1;
    topic_distribution_[iter.Topic()] += 1;
    n_mk_map[iter.Topic()] += 1;
  }
}
LDADocument::LDADocument(const DocumentWordTopicsPB& topics,
                         int num_topics, int16* base) {
  topic_assignments_ = new DocumentWordTopicsPB;
  topic_assignments_->CopyFrom(topics);
  num_topics_ = num_topics;
  topic_distribution_ = base;
  document_length = 0;
  CountTopicDistribution();
  sum_a2_ = 0;
}
void LDADocument::AppendTopics(int num_new_topics, int16* new_base) {
  num_topics_ += num_new_topics;
  topic_distribution_ = new_base;
  sum_a2_ = 0;
}
```
