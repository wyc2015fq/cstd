# plda源码(十二) - Arthur的随笔 - CSDN博客
2019年02月01日 15:56:48[largetalk](https://me.csdn.net/largetalk)阅读数：106
## plda源码(十二)
### LightLDA
原始 Gibbs Sampling 采样函数如下：
$p(z_{di}=k | rest) ∝ \frac{(n^{−di}_{kd}+\alpha_k)(n^{−di}_{kw}+\beta_w)}{n^{−di}_k+\overline{\beta}}$
AliasLDA
$p(z_{di}=k | rest) ∝ \frac{n^{−di}_{kd}(n^{−di}_{kw}+\beta_w)}{n^{−di}_k+\overline{\beta}} +  \frac{\alpha_k(n^{−di}_{kw}+\beta_w)}{n^{−di}_k+\overline{\beta}}$
第二项可以看做“topic-word”桶,与文档无关。这一项可以通过Alias Table和 Metropolis-Hastings（一种蒙特卡洛采样方法） 进行O(1) 时间复杂度采样。Alias Table在上一篇文章有介绍。
LightLDA
$p(z_{di}=k | rest) ∝ (n^{−di}_{kd}+\alpha_k) * \frac{(n^{−di}_{kw}+\beta_w)}{n^{−di}_k+\overline{\beta}}$
$q(z_{di}=k | rest) \propto (n_{kd} + \alpha_{k}) * \frac{n_{kw} + \beta_w}{n_k + \overline{\beta}}$
第一项为doc-proposal，第二项为word-proposal。
同样退化成MH采样
$min\{ 1, \frac{p(t)q(t \rightarrow s)}{p(s)q(s\rightarrow t)} \}$
doc-proposal
$q = p_d(k)  \propto n_{kd}+\alpha_k$
接受率
$\pi_d = \frac{ (n^{−di}_{td}+\alpha_t)(n^{−di}_{tw}+\beta_w)(n^{−di}_s+\overline{\beta})(n_{sd}+\alpha_s)}{ (n^{−di}_{sd}+\alpha_s)(n^{−di}_{sw}+\beta_w)(n^{−di}_t+\overline{\beta})(n_{td}+\alpha_t)}$
```
int K = model_->num_topics();
        double sumPd = document->GetDocumentLength() + Kalpha;
        for (...) {
            int w = iterator.Word();
            int topic = iterator.Topic();
            int new_topic;
            int old_topic = topic;
            
                {
                    // Draw a topic from doc-proposal
                    double u = random->RandDouble() * sumPd;
                    if (u < document->GetDocumentLength()) {
                        // draw from doc-topic distribution skipping n
                        unsigned pos = (unsigned) (u);
                        new_topic = document->topics().wordtopics(pos);
                    } else {
                        // draw uniformly
                        u -= document->GetDocumentLength();
                        u = u / alpha_;
                        new_topic = (unsigned short) (u); // pick_a_number(0,trngdata->docs[m]->length-1); (int)(utils::unif01()*ptrndata->docs[m]->length);
                    }
                    if (topic != new_topic) {
                        //2. Find acceptance probability
                        int ajustment_old = topic == old_topic? -1 : 0;
                        int ajustment_new = new_topic == old_topic? -1 : 0;
                        double temp_old = ComputeProbForK(document, w, topic, ajustment_old);
                        double temp_new = ComputeProbForK(document, w, new_topic, ajustment_new);
                        double prop_old = (N_DK(document, topic) + alpha_);
                        double prop_new =  (N_DK(document, new_topic) + alpha_);
                        double acceptance = (temp_new * prop_old) / (temp_old * prop_new);
                        //3. Compare against uniform[0,1]
                        if (random->RandDouble() < acceptance) {
                            topic = new_topic;
                        }
                    }
```
其中的ComputeProbForK是
```
double ComputeProbForK(LDADocument* document, int w, int topic,
            int ajustment) {
      return  (N_DK(document, topic) + alpha_ + ajustment)
                * (N_WK(w, topic) + beta_ + ajustment)
                / (N_K(topic) + Vbeta + ajustment);
  }
```
word-proposal
$q = p_d(k)  \propto \frac{n_{kw} + \beta_w}{n_k + \overline{\beta}}$
接受率
$\pi_w = \frac{ (n^{−di}_{td}+\alpha_t)(n^{−di}_{tw}+\beta_w)(n^{−di}_s+\overline{\beta})(n_{sw} + \beta_w)(n_t + \overline{\beta})}{ (n^{−di}_{sd}+\alpha_s)(n^{−di}_{sw}+\beta_w)(n^{−di}_t+\overline{\beta})(n_{tw} + \beta_w)(n_s + \overline{\beta})}$
```
{
                    // Draw a topic from word-proposal
                    q[w].noSamples++;
                    if (q[w].noSamples > qtable_construct_frequency) {
                        GenerateQTable(w);
                    }
                    new_topic = q[w].sample(random->RandInt(K), random->RandDouble());
                    if (topic != new_topic) {
                        //2. Find acceptance probability
                        int ajustment_old = topic == old_topic? -1 : 0;
                        int ajustment_new = new_topic == old_topic? -1 : 0;
                        double temp_old = ComputeProbForK(document, w, topic, ajustment_old);
                        double temp_new = ComputeProbForK(document, w, new_topic, ajustment_new);
                        double acceptance = (temp_new * q[w].w[topic]) / (temp_old * q[w].w[new_topic]);
                        //3. Compare against uniform[0,1]
                        if (random->RandDouble() < acceptance) {
                            topic = new_topic;
                        }
                    }
                }
```
其中GenerateQTable如下
```
void GenerateQTable(unsigned int w) {
        int num_topics = model_->num_topics();
        q[w].wsum = 0.0;
        const TopicDistribution<int32>& word_distribution = model_->GetWordTopicDistribution(w);
        const TopicDistribution<int32>& n_k = model_->GetGlobalTopicDistribution();
        for (int k = 0; k < num_topics; ++k) {
            q[w].w[k] =   (word_distribution[k] + beta_) / (n_k[k] + Vbeta);
            q[w].wsum += q[w].w[k];
        }
        q[w].constructTable();
    }
```
