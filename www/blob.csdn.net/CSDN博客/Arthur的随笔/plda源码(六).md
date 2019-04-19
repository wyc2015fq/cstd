# plda源码(六) - Arthur的随笔 - CSDN博客
2019年01月07日 15:21:55[largetalk](https://me.csdn.net/largetalk)阅读数：99
个人分类：[plda](https://blog.csdn.net/largetalk/article/category/8532621)
## plda源码(六)
LDAModel
只增加了IncrementTopic和ReassignTopic函数
```
class LDAModel : public ModelBase<int32> 
void LDAModel::IncrementTopic(int word, int topic, int32 count) {
  CHECK_GT(num_topics(), topic);
  CHECK_GT(vocab_size(), word);
  topic_distributions_[word][topic] += count;
  global_distribution_[topic] += count;
  n_kw_vec[word][topic] += count;
  CHECK_LE(0, n_kw_vec[word][topic]);
  if (n_kw_vec[word][topic] == 0) {
      n_kw_vec[word].erase(topic);
  }
  CHECK_LE(0, topic_distributions_[word][topic]);
}
void LDAModel::ReassignTopic(int word, int old_topic,
                             int new_topic, int32 count) {
  IncrementTopic(word, old_topic, -count);
  IncrementTopic(word, new_topic, count);
}
```
LDAAccumulativeModel
把多次迭代的LDAModel累加和平均
```
class LDAAccumulativeModel : public ModelBase<double> 
void LDAAccumulativeModel::AccumulateModel(const LDAModel& source_model) {//当前model+source_model
  CHECK_EQ(num_topics(), source_model.num_topics());
  for (LDAModel::Iterator iter(&source_model); !iter.Done(); iter.Next()) {
    const TopicDistribution<int32>& source_dist = iter.Distribution();
    TopicDistribution<double>* dest_dist = &(topic_distributions_[iter.Word()]);
    CHECK_EQ(num_topics(), source_dist.size());
    for (int k = 0; k < num_topics(); ++k) {
      (*dest_dist)[k] += static_cast<double>(source_dist[k]);
    }
  }
  for (int k = 0; k < num_topics(); ++k) {
    global_distribution_[k] +=
        static_cast<double>(source_model.GetGlobalTopicDistribution()[k]);
  }
}
void LDAAccumulativeModel::AverageModel(int num_accumulations) {//求平均
  for (vector<TopicDistribution<double> >::iterator iter =
         topic_distributions_.begin();
       iter != topic_distributions_.end(); ++iter) {
    TopicDistribution<double>& dist = *iter;
    for (int k = 0; k < num_topics(); ++k) {
      dist[k] /= num_accumulations;
    }
  }
  for (int k = 0; k < num_topics(); ++k) {
    global_distribution_[k] /= num_accumulations;
  }
}
```
