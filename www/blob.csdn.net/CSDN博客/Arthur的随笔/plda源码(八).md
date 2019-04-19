# plda源码(八) - Arthur的随笔 - CSDN博客
2019年01月09日 17:53:32[largetalk](https://me.csdn.net/largetalk)阅读数：87
## plda源码(八)
sampler.h
终于来到最关键的地方，lda不能直接算出word和document的topic分布，只能不停的随机对应的topic分布，然后选择接受与否，即Gibbs采样
```
// LDASampler trains LDA models and computes statistics about documents in
// LDA models.
class LDASampler {
 public:
  // alpha and beta are the Gibbs sampling symmetric hyperparameters.
  // model is the model to use.
  LDASampler(double alpha, double beta, LDAModel* model,
             LDAAccumulativeModel* accum_model);
  // Performs one round of Gibbs sampling on documents in the corpus
  // by invoking SampleNewTopicsForDocument(...).  If we are to train
  // a model given training data, we should set train_model to true,
  // and the algorithm updates model_ during Gibbs sampling.
  // Otherwise, if we are to sample the latent topics of a query
  // document, we should set train_model to false.  If train_model is
  // true, burn_in indicates should we accumulate the current estimate
  // to accum_model_.  For the first certain number of iterations,
  // where the algorithm has not converged yet, you should set burn_in
  // to false.  After that, we should set burn_in to true.
  void DoIteration(
      Random* random, LDACorpus* corpus, bool train_model, bool burn_in);//一轮Gibbs采样
  // Performs one round of Gibbs sampling on a document.  Updates
  // document's topic assignments.  For learning, update_model_=true,
  // for sampling topics of a query, update_model_==false.
  void SampleNewTopicsForDocument(
      Random* random, LDADocument* document, bool update_model);
  // Computes the log likelihood of a corpus.
  double LogLikelihood(LDACorpus* corpus) const;
 private:
  // The core of the Gibbs sampling process.  Compute the full conditional
  // posterior distribution of topic assignments to the indicated word.
  //
  // That is, holding all word-topic assignments constant, except for the
  // indicated one, compute a non-normalized probability distribution over
  // topics for the indicated word occurrence.
  void GenerateTopicDistributionForWord(
      const LDADocument& document, int word, int cur_topic,
      bool train_model, vector<double>* distribution) const;
  // Returns a sample selected from a non-normalized probability distribution.
  int GetAccumulativeSample(
      Random* random, const vector<double>& distribution) const;
  // Core function implementing fast sampling.
  // NOTE:
  // 1. The topic distribution of the document, represented by
  // mutable_doc_distribution, will be changed herein.
  // 2. The sum_a2_ of the document, represented by
  // mutable_sum_a2, will be changed herein.
  int UpdateTopicForWord(
      Random* random, int16* mutable_doc_distribution, double* mutable_sum_a2,
      int word, int cur_topic, bool update_model) ;
  // Computes the log likelihood of a document.
  double LogLikelihood(LDADocument* document) const;
  const double alpha_;
  const double beta_;
  LDAModel* model_;
  LDAAccumulativeModel* accum_model_;
  vector<double> sum_b2_;               // $\Sigma_k {nw[w][k] + beta}^2$ 
  vector<uint16_t> nwsum_idx_;          // Sorted index for global_distribution
  vector<uint16_t> nwsum_rev_;          // Reverse index for nwsum_idx_
  vector<uint16_t> nw_idx_;             // Sorted index for word_distribution
};
```
note:
`$\Sigma_k {nw[w][k] + beta}^2$`
$\Sigma_k {nw[w][k] + beta}^2$
[sampler.cc](http://sampler.cc)
```
LDASampler::LDASampler(double alpha, double beta, LDAModel* model,
                       LDAAccumulativeModel* accum_model)
    : alpha_(alpha), beta_(beta),
      model_(model), accum_model_(accum_model) {
}
void LDASampler::InitModelGivenTopics(const LDACorpus& corpus) {//初始化model
  for (LDACorpus::const_iterator iter = corpus.begin();
       iter != corpus.end(); ++iter) {
    LDADocument* document = *iter;
    for (LDADocument::WordOccurrenceIterator iter2(document);
         !iter2.Done(); iter2.Next()) {
      model_->IncrementTopic(iter2.Word(), iter2.Topic(), 1);
    }
  }
}
//DoIteration, SampleNewTopicsForDocument,GenerateTopicDistributionForWord 这几个是普通Gibbs采样
void LDASampler::DoIteration(
    Random* random, LDACorpus* corpus, bool train_model, bool burn_in) {
    //一次Gibbs采样
  for (LDACorpus::iterator iter = corpus->begin();
       iter != corpus->end(); ++iter) {//iter文档
    SampleNewTopicsForDocument(random, *iter, train_model);
  }
  if (accum_model_ != NULL && train_model && !burn_in) {
    accum_model_->AccumulateModel(*model_);//累加
  }
}
void LDASampler::SampleNewTopicsForDocument(
    Random* random, LDADocument* document, bool update_model) {
  for (LDADocument::WordOccurrenceIterator iterator(document);
       !iterator.Done(); iterator.Next()) {//在word的所有topic上迭代
    // This is a (non-normalized) probability distribution from which we will
    // select the new topic for the current word occurrence.
    vector<double> new_topic_distribution;
    //计算当前word，topic-1后的topic分布
    GenerateTopicDistributionForWord(
        *document, iterator.Word(), iterator.Topic(),
        update_model, &new_topic_distribution);
    //在新的topic分布上采样（多项分布）
    int new_topic = GetAccumulativeSample(random, new_topic_distribution);
    // Update document and model parameters with the new topic.
    if (update_model) {
    //更新model
      model_->ReassignTopic(iterator.Word(), iterator.Topic(), new_topic, 1);
    }
    iterator.SetTopic(new_topic);//文档和word设置成新的topic
  }
}
void LDASampler::GenerateTopicDistributionForWord(
    const LDADocument& document, int word, int cur_topic,
    bool train_model, vector<double>* distribution) const {
    //生成该文档该词的采样分布
  int num_topics = model_->num_topics();
  int vocab_size = model_->vocab_size();
  distribution->clear();
  distribution->reserve(num_topics);
  const TopicDistribution<int32>& word_distribution =
    model_->GetWordTopicDistribution(word);
  for (int k = 0; k < num_topics; ++k) {
    // We will need to temporarily unassign the word from its old topic, which
    // we accomplish by decrementing the appropriate counts by 1.
    int current_topic_adjustment =
      (train_model && k == cur_topic) ? -1 : 0;
    double topic_word_factor = word_distribution[k] + current_topic_adjustment;
    double global_topic_factor =
      model_->GetGlobalTopicDistribution()[k] + current_topic_adjustment;
    current_topic_adjustment = (k == cur_topic) ? -1 : 0;
    double document_topic_factor =
        document.topic_distribution()[k] + current_topic_adjustment;
//（词word在主题k上的分布 + beta） * (文档document在主题k上的分布 + alpha)/(全部词在主题k上的分布 + 词数*beta)
//采样公式见下图1,2
    distribution->push_back(
        (topic_word_factor + beta_) *
        (document_topic_factor + alpha_) /
        (global_topic_factor + vocab_size * beta_));
  }
}
int LDASampler::GetAccumulativeSample(
    Random* random, const vector<double>& distribution) const {
    //多项分布采样选择对应topic
  double distribution_sum = 0.0;
  for (int i = 0; i < distribution.size(); ++i) {
    distribution_sum += distribution[i];
  }
  double choice = random->RandDouble() * distribution_sum;
  double sum_so_far = 0.0;
  for (int i = 0; i < distribution.size(); ++i) {
    sum_so_far += distribution[i];
    if (sum_so_far >= choice) {
      return i;
    }
  }
  LOG(FATAL) << "Failed to choose element from distribution of size "
             << distribution.size() << " and sum " << distribution_sum;
  return -1;
}
```
下面实现的是Fast Gibbs采样
```
void LDASampler::InitFastData(bool update_model) {//初始化
  int num_topics = model_->num_topics();
  int vocab_size = model_->vocab_size();
  // Note: infer is not active in refreshing model
  if (update_model || sum_b2_.empty()) {
    // Give values for $\Sigma_k {b_k}$ each time
    sum_b2_.resize(vocab_size);
    for (int w = 0; w < vocab_size; ++w) {
      const TopicDistribution<int32>& word_topic_cooccurrences =
        model_->GetWordTopicDistribution(w);
      double s2 = 0;
      //计算（单词w在主题k上分布+beta）^ 2, 然后累加
      for (int k = 0; k < num_topics; ++k) {
        double tm = word_topic_cooccurrences[k] + beta_;
        s2 += tm * tm;
      }
      sum_b2_[w] = s2;
    }
    const TopicDistribution<int32>& global_topic_occurrences =
      model_->GetGlobalTopicDistribution();
    nwsum_idx_.resize(num_topics);
    nwsum_rev_.resize(num_topics);
    // Begin sorting elements DECREMENT:
    //nwsum_idx_存储降序排列的全局（所有单词累加）主题分布的下标
    sort_index_descend(&global_topic_occurrences[0],
                       &nwsum_idx_[0], num_topics);
    //nwsum_rev_存储全局主题分布中对应下标主题的rank
    index_to_rank(&nwsum_idx_[0], &nwsum_rev_[0], num_topics);
    //举例如下：
    //global_topic_occurrences：9 1 6 7 4 
    //则nwsum_idx_：            0 3 2 4 1 
    //nwsum_rev_：              0 4 2 1 3 
    nw_idx_.resize(vocab_size * num_topics);
    for (int w = 0; w < vocab_size; ++w) {
      const TopicDistribution<int32>& word_topic_cooccurrences =
        model_->GetWordTopicDistribution(w);
      uint16_t *idx = &nw_idx_[w * num_topics];
      //每个单词也计算其按降序排列的主题分布下标
      sort_index_descend(&word_topic_cooccurrences[0], idx, num_topics);
    }
  }
}
void LDASampler::DoFastIteration(
    Random* random, LDACorpus* corpus, bool train_model, bool burn_in) {
  // A pre-step for sampling:
  InitFastData(train_model);
  for (LDACorpus::iterator iter = corpus->begin();
       iter != corpus->end(); ++iter) {
    FastSampleNewTopicsForDocument(random, *iter, train_model);
  }
  if (accum_model_ != NULL && train_model && !burn_in) {
    accum_model_->AccumulateModel(*model_);
  }
}
void LDASampler::FastSampleNewTopicsForDocument(
    Random* random, LDADocument* document, bool update_model) {
  int num_topics = model_->num_topics();
  int16* mutable_doc_distribution = document->mutable_topic_distribution();
  double* mutable_sum_a2 = document->mutable_sum_a2();
  if (*mutable_sum_a2 <= 0) {
    *mutable_sum_a2 = 0;
    //计算（文档在主题k上的分布+alpha）^2,然后累加
    for (int k = 0; k < num_topics; ++k) {
      double tm = mutable_doc_distribution[k] + alpha_;
      *mutable_sum_a2 += tm * tm;
    }
  }
  for (LDADocument::WordOccurrenceIterator iterator(document);
     !iterator.Done();
     iterator.Next()) {
    int new_topic = UpdateTopicForWord(//生成新topic
        random, mutable_doc_distribution, mutable_sum_a2,
        iterator.Word(), iterator.Topic(), update_model);
    // The topic distribution has been changed in UpdateTopicForWord(),
    // so we need only to alter the topic assignment of the current word.
    iterator.SetTopicAssignment(new_topic);
  }
}
// This thread safety requirement comes from the inference scenario.
int LDASampler::UpdateTopicForWord(
    Random* random, int16* mutable_doc_distribution, double* mutable_sum_a2,
    int word, int cur_topic, bool update_model) {
  const TopicDistribution<int32>& word_distribution =
    model_->GetWordTopicDistribution(word);
  const TopicDistribution<int32>& global_distribution =
    model_->GetGlobalTopicDistribution();
  int num_topics = model_->num_topics();
  int vocab_size = model_->vocab_size();
  // locate the rank for fast-resorting
  //得到cur_topic的rank
  const uint16_t *index = &nw_idx_[word * num_topics];
  int current_rank = 0;
  if (update_model) {
    for (; current_rank < num_topics; ++current_rank) {
      if (index[current_rank] == cur_topic) break;
    }
  }
  // (1) Remove z_i from the count variables
  --mutable_doc_distribution[cur_topic];//文档doc的主题k数量-1
  *mutable_sum_a2 -= 2 * (mutable_doc_distribution[cur_topic] + alpha_) + 1;//(x+1+a)^2 = (x+a)^2 + 2(x+a) + 1
  if (update_model) {
    model_->IncrementTopic(word, cur_topic, -1);
    sum_b2_[word] -= 2 * (word_distribution[cur_topic] + beta_) + 1;//同上
    //重新排序
    resort_after_decreased(num_topics, &word_distribution[0],
        &nw_idx_[word * num_topics], current_rank);
    resort_after_decreased(num_topics, &global_distribution[0],
        &nwsum_idx_[0], &nwsum_rev_[0], cur_topic);
  }
  // (2) Do multinomial sampling via fast Gibbs-sampling:
  //TODO:
  double a2 = *mutable_sum_a2;
  double b2 = sum_b2_[word];
  double z_k = 0;
  const double wbeta = vocab_size * beta_;
  uint64_t nwsum_min = global_distribution[nwsum_idx_[num_topics-1]];
  double fac = 1 / (nwsum_min + wbeta);
  vector<double> pk(num_topics);
  double *p = &pk[0];
  double u = random->RandDouble();
  int new_topic = cur_topic;
  int new_rank = current_rank;
  for (int k = 0; k < num_topics; ++k) {
    int t = index[k];
    double a = mutable_doc_distribution[t] + alpha_;
    double b = word_distribution[t] + beta_;
    p[k] = (k > 0 ? p[k-1] : 0);
    p[k] += a * b / (global_distribution[t] + wbeta);
    double z_old = z_k;
    a2 -= a * a;
    b2 -= b * b;
    z_k = p[k];
    if (a2 > 0 && b2 > 0) {
      z_k += sqrt(a2 * b2) * fac; // |a|*|b|*|c|
    }
    // Case A: not found, loop back
    if (u*z_k > p[k]) continue;
    // Case B: it's this topic, done
    if (k == 0 || u*z_k > p[k-1]) {
      new_rank = k;
      new_topic = t;
      break;
    }
    // Case C: it's a previous topic
    u = (u*z_old - p[k-1]) * z_k / (z_old - z_k); // renew threshold
    for (int i = 0; i < k; ++i) {
      if (p[i] >= u) {
        new_rank = i;
        new_topic = index[i];
        break;
      }
    }
    break; // Quit, got the right one
  }
  // (3) Add newly estimated z_i to count variables
  ++mutable_doc_distribution[new_topic];
  *mutable_sum_a2 += 2 * (mutable_doc_distribution[new_topic] + alpha_) - 1;
  if (update_model) {
    model_->IncrementTopic(word, new_topic, +1);
    sum_b2_[word] += 2 * (word_distribution[new_topic] + beta_) - 1;
    resort_after_increased(num_topics, &word_distribution[0],
        &nw_idx_[word * num_topics], new_rank);
    resort_after_increased(num_topics, &global_distribution[0],
        &nwsum_idx_[0], &nwsum_rev_[0], new_topic);
  }
  return new_topic;
}
double LDASampler::LogLikelihood(LDACorpus* corpus) const {
//所有文档的log likelihood累加
  double log_likelihood = 0;
  for (LDACorpus::const_iterator iterator = corpus->begin();
       iterator != corpus->end(); ++iterator) {
    log_likelihood += LogLikelihood(*iterator);
  }
  return log_likelihood;
}
// Compute log P(d) = sum_w log P(w), where P(w) = sum_z P(w|z)P(z|d).
double LDASampler::LogLikelihood(LDADocument* document) const {
  const int num_topics(model_->num_topics());
  // Compute P(z|d) for the given document and all topics.
  const int16* document_topic_cooccurrences(document->topic_distribution());
  int64 document_length = document->topics().wordtopics_.size();
  vector<double> prob_topic_given_document(num_topics);
  //p(z|d)=(文档在主题t上的分布+alpha）/（文档长度(文档在所有主题上分布累加)+alpha*主题数）
  for (int t = 0; t < num_topics; ++t) {
    prob_topic_given_document[t] =
        (document_topic_cooccurrences[t] + alpha_) /
        (document_length + alpha_ * num_topics);
  }
  // Get global topic occurrences, which will be used compute P(w|z).
  TopicDistribution<int32> global_topic_occurrences =
    model_->GetGlobalTopicDistribution();
  double log_likelihood = 0.0;
  // A document's likelihood is the product of its words' likelihoods.  Compute
  // the likelihood for every word and sum the logs.
  for (LDADocument::WordOccurrenceIterator iterator(document);
       !iterator.Done(); iterator.Next()) {
    // Get topic_count_distribution of the current word, which will be
    // used to Compute P(w|z).
    TopicDistribution<int32> word_topic_cooccurrences =
      model_->GetWordTopicDistribution(iterator.Word());
    // Comput P(w|z).
    // (单词在主题t上分布+beta)/（所有单词在主题t上分布 + 词数*beta)
    vector<double> prob_word_given_topic(num_topics);
    for (int t = 0; t < num_topics; ++t) {
      prob_word_given_topic[t] =
          (word_topic_cooccurrences[t] + beta_) /
          (global_topic_occurrences[t] + model_->vocab_size() * beta_);
    }
    // Compute P(w) = sum_z P(w|z)P(z|d)
    double prob_word = 0.0;
    for (int t = 0; t < num_topics; ++t) {
      prob_word += prob_word_given_topic[t] * prob_topic_given_document[t];
    }
    log_likelihood += log(prob_word);
  }
  return log_likelihood;
}
```
$P(d) = sum_w log P(w), where P(w) = sum_z P(w|z)P(z|d).$
