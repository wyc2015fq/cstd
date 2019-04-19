# plda源码(十) - Arthur的随笔 - CSDN博客
2019年01月15日 16:39:03[largetalk](https://me.csdn.net/largetalk)阅读数：90
## plda源码(十)
Sparse LDA
StandardGibbs采样公式如下
$\begin{aligned} q(z) &= \frac{n^{t}_{k,\neg i} + \beta}{n_{k,\neg i} + \beta V}(n^{k}_{m,\neg i} + \alpha_k) \end{aligned}$
$n^{t}_{k}$ 是单词t在主题k上的分布，  $n^{k}_{m}$是文档m在主题k上的分布， $n_{k}$是所有单词在主题k上的分布
每个采样迭代复杂度是$O(M\overline{N_m}K)$, ( $\overline{N_m}$ 表示训练文档的平均长度）；内存消耗主要在  $n^{k}_{m}$ 和 $n^{t}_{k}$，假设都采用Dense存储，内存复杂度是 $O\left(K(M+V)\right)$
### sparseLDA
因为$n^{k}_{m}$ 和 $n^{t}_{k}$一般很稀疏，SparseLDA 将 $Q$变形
$Q = \sum_k \frac{n^{t}_{k,\neg i} + \beta}{n_{k,\neg i} + \beta V}\left(n^{k}_{m,\neg i} + \alpha_k\right) \\= \sum_k \left(\frac{n^{t}_{k,\neg i}\left(n^{k}_{m,\neg i} + \alpha_k\right)}{n_{k,\neg i} + \beta V} + \frac{\beta n^{k}_{m,\neg i}}{n_{k,\neg i} + \beta V} + \frac{\beta \alpha_k}{n_{k,\neg i} + \beta V}\right) \\= \underbrace{\sum_k \frac{n^{t}_{k,\neg i}\left(n^{k}_{m,\neg i} + \alpha_k\right)}{n_{k,\neg i} + \beta V}}_E + \underbrace{\sum_k \frac{\beta n^{k}_{m,\neg i}}{n_{k,\neg i} + \beta V}}_F + \underbrace{\sum_k \frac{\beta \alpha_k}{n_{k,\neg i} + \beta V}}_G$
令 $E=\sum_k e(k)$、$F=\sum_k f(k)$ 和 $G=\sum_k g(k)$。其中 $E$ 包含 $|Nonzero(n^{t}_{k,\neg i})|$项，称为“topic word”桶；$F$ 包含 $|Nonzero(n^{k}_{m,\neg i})|$项，称为“document topic” 桶；$G$ 包含 $K$ 项，称为“smoothing only”桶。
$c(z=k) = \frac{n^{k}_{m,\neg i} + \alpha_k}{n_{k,\neg i} + \beta V}$
$e(z=k) = n^{t}_{k,\neg i}c(k)$
$f(z=k) = \frac{\beta n^{k}_{m,\neg i}}{n_{k,\neg i} + \beta V}$
$g(z=k) = \frac{\beta \alpha_k}{n_{k,\neg i} + \beta V}$
采样文档中词的主题时，首先计算 $Q=E+F+G$，同时生成一个随机变量$U∼Uniform(0,Q)$，然后在三个桶里进行具体的主题采样：
若 $U<E$，主题落在“topic word” 桶；
若 $U<E+F$，主题落在“document topic”桶；
否则，主题落在“smoothing only”桶。
```
class SparseLDASampler : public BaseSampler {
 private:
  vector<double> e_;
  vector<double> f_;
  vector<double> g_;
  double G;
  vector<double> c_;
  map<int64, int32> n_mk;
  map<int32, int64> n_kw;
};
```
```
e_.resize(model_->num_topics());
        f_.resize(model_->num_topics());
        g_.resize(model_->num_topics());
        c_.resize(model_->num_topics());
    
    //初始化
        G = 0;
        double betaV = vocab_size * beta_;
        for (int i = 0; i < num_topics; i++) {
            c_[i] = alpha_ /
                    (model_->GetGlobalTopicDistribution()[i] + betaV);
            g_[i] = (alpha_ * beta_) / (model_->GetGlobalTopicDistribution()[i] + betaV);
            G += g_[i];
        }
    
  //一个文档的一次采样
        double F = 0;
        int vocab_size;//词典大小
        double betaV = vocab_size * beta_;
        unordered_map<int, int> n_mk_map = document->GetTopicDist();//文档的topic->count,稀疏
        for (iter = n_mk_map.begin();iter != n_mk_map.end(); iter++) {
               int topic = iter->first;
                c_[topic] = (document->topic_distribution()[topic] + alpha_) /
                        (model_->GlobalTopicDistribution()[topic] + betaV);//该文档没有出现的topic的c函数为alpha/(global[topic] +betaV)
                f_[topic] = (beta_ * document->topic_distribution()[topic]) /
                        (model_->GetGlobalTopicDistribution()[topic] + betaV);//未出现的topic的f函数为0
                F += f_[topic];
        }
        for (iterator(document);
                !iterator.Done(); iterator.Next()) {//在文档word上的迭代
            int current_topic = iterator.Topic();
            int old_topic = current_topic;
            double numer_mk = document->topic_distribution()[old_topic] - 1;
            double denom = model_->GetGlobalTopicDistribution()[old_topic] - 1 + betaV;
            // update all the statistic associate with old_topic
            c_[old_topic] = (numer_mk + alpha_) / denom;
            double f_update = beta_ * numer_mk / denom - beta_ * (numer_mk + 1) / (denom + 1);
            f_[old_topic] += f_update;
            F = F + f_update;
            double g_update = beta_ * alpha_ * (1 / denom - 1/ (denom + 1));
            g_[old_topic] += g_update;
            G += g_update;
            double E = 0;
            unordered_map<int, int> n_kw_map = model_->GetTopicDistByWord(iterator.Word());//word的topic->count,稀疏
            for (kw_iter = n_kw_map.begin();kw_iter != n_kw_map.end(); kw_iter++) {
                int topic_tmp = kw_iter->first;
                int n_kw_factor = kw_iter->second;
                if (topic_tmp == old_topic) {
                    n_kw_factor -= 1;
                }
                e_[topic_tmp] = n_kw_factor * c_[topic_tmp];
                E += e_[topic_tmp];
            }
            double total = E + F + G;
            n_mk_map = document->GetTopicDist();
            double choice = random->RandDouble() * total;
            int new_topic = -1;
            if (choice < E) {
                new_topic = SampleInBucketWithMap(n_kw_map, e_, choice);
            } else if (choice < E + F) {
                new_topic = SampleInBucketWithMap(n_mk_map, f_, choice - E);
            } else {
                new_topic = SampleInSmoothBucket(g_, choice - E - F);
            }
            if (update_model) {
                model_->ReassignTopic(iterator.Word(), iterator.Topic(), new_topic, 1);
            }
            iterator.SetTopic(new_topic);
            // update all the statistic associate with new_topic
            numer_mk = document->topic_distribution()[new_topic];
            denom = model_->GetGlobalTopicDistribution()[new_topic] + betaV;
            c_[new_topic] = (numer_mk + alpha_) / denom;
            f_update = beta_ * numer_mk / denom - beta_ * (numer_mk - 1) / (denom - 1);
            f_[new_topic] += f_update;
            F = F + f_update;
            g_update = beta_ * alpha_ * (1 / denom - 1/ (denom - 1));
            g_[new_topic] += g_update;
            G += g_update;
        }
        n_mk_map = document->GetTopicDist();
        iter = n_mk_map.begin();
        for (;iter != n_mk_map.end(); iter++) {
               int topic = iter->first;
               c_[topic] = alpha_ / (model_->GetGlobalTopicDistribution()[topic] + betaV);
        }
    
    int SparseLDASampler::SampleInBucketWithMap(unordered_map<int, int>& map,
            vector<double>& vec_, double choice) const {
        double sum_so_far = 0.0;
        for (const auto &kv : map) {
            sum_so_far += vec_[kv.first];
            if (sum_so_far >= choice) {
                return kv.first;
            }
        }
    }
    int SparseLDASampler::SampleInSmoothBucket(
            const vector<double>& distribution, double choice) const {
        double sum_so_far = 0.0;
        for (int i = 0; i < distribution.size(); ++i) {
            sum_so_far += distribution[i];
            if (sum_so_far >= choice) {
                return i;
            }
        }
    }
```
