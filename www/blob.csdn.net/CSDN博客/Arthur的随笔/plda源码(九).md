# plda源码(九) - Arthur的随笔 - CSDN博客
2019年01月10日 16:49:34[largetalk](https://me.csdn.net/largetalk)阅读数：101
## plda源码(九)
BaseSampler是把Sampler抽象一下，添加词相识度和为新采样方法提供接口
```
class BaseSampler {
    public:
        BaseSampler(double alpha, double beta,
                LDAModel* model,
                LDAAccumulativeModel* accum_model);
        virtual void InitModelGivenTopics(const LDACorpus& corpus);
        virtual void SpecificInit(LDAModel*) {
        }
        virtual void DoIteration(Random* random, LDACorpus* corpus, bool train_model, bool burn_in);
        virtual void SampleNewTopicsForDocument(Random* random, LDADocument* document,
                bool update_model) = 0;
        virtual double LogLikelihood(LDADocument* document) const;
        virtual double LogLikelihood(LDACorpus* document) const;
    protected:
        inline double GetFactorInModel(int w, int k, int ajustment) const {//w相似的wod并在主题k上有分布的相似度乘积
            ElemIter iter = matrix->GetRowWithIterUnsorted(w);
            double res = 1.0;
            while (iter.HasNext()) {
                double sim;
                int word;
                iter.GetNext(sim, word);
                const TopicDistribution<int32>& word_distribution = model_->GetWordTopicDistribution(word);
                if (word_distribution[k] > 0) {
                    res *= sim;
                    if (use_model_weight) {
                        res *= GetModelWeight(word, k, ajustment);
                    }
                }
            }
            return res;
        }
        inline double GetFactorInDoc(int word, int k, const LDADocument* document) const {
        //word的相似词并在文档document中有主题k分布的相似度乘积
            ElemIter iter = matrix->GetRowWithIterUnsorted(word);
            double res = 1.0;
            while (iter.HasNext()) {
                double sim;
                int word;
                iter.GetNext(sim, word);
                if (HasSameTopic(word, k, document)) {
                    res *= sim;
                }
            }
            return res;
        }
        inline bool HasSameTopic(int word, int k, const LDADocument* document) const {
        //word在document中是否有topic k分布
            ......
            return true;
        }
        inline double GetModelWeight(int word, int k, int ajustment) const {
        //exp(word在主题k上分布/(所有单词在主题k上分布+adjust))
            const TopicDistribution<int32>& word_distribution;
            const TopicDistribution<int32>& global_distribution;
            double relative_weight = 1.0;
            if (global_distribution[k] + ajustment > 0) {
                relative_weight = ((double) word_distribution[k]) / (global_distribution[k] + ajustment);
            }
            double result = fast_exp(relative_weight);//exp
            return result < 1.0 ? 1.0 : result;
        }
    const double alpha_;
    const double beta_;
    LDAModel* model_;
    LDAAccumulativeModel* accum_model_;
    WordMatrix* matrix;
    int mode;
    bool use_model_weight;
    const int DOC_MODE = 1;
    const int MODEL_MODE = 2;
};
```
NaiveSampler
在Gibbs基础上添加了词相似度的采样
```
class NaiveSampler : public BaseSampler
void NaiveSampler::GenerateTopicDistributionForWord(
  ...
    double origin_gibbs_score = (topic_word_factor + beta_) *
        (document_topic_factor + alpha_) /
        (global_topic_factor + vocab_size * beta_);
    double ajust_prob = ComputeAjustProb(word, k, origin_gibbs_score, &document, current_topic_adjustment);
    distribution->push_back(ajust_prob);
  }
}
double NaiveSampler::ComputeAjustProb(int word, int iter_topic, double origin_gibbs_score, const LDADocument* document,
        int current_topic_adjustment) const {
    if (mode == DOC_MODE) {
        return origin_gibbs_score * GetFactorInDoc(word, iter_topic, document);//对同一文档，词语相似落在同一topic上的有加成
    } else if (mode == MODEL_MODE) {
        return origin_gibbs_score * GetFactorInModel(word, iter_topic, current_topic_adjustment);//全局模式，词语相似落在同一topic上的有加成
    }
    return origin_gibbs_score;
}
```
