# Metric Learning度量学习：**矩阵学习和图学习 - wishchinYang的专栏 - CSDN博客
2016年05月18日 19:23:38[wishchin](https://me.csdn.net/wishchin)阅读数：4357
**DML学习**原文链接：[http://blog.csdn.net/lzt1983/article/details/7884553](http://blog.csdn.net/lzt1983/article/details/7884553)
一篇metric learning（DML）的综述文章，对DML的意义、方法论和经典论文做一个介绍，同时对我的研究经历和思考做一个总结。可惜一直没有把握自己能够写好，因此拖到现在。
先列举一些DML的参考资源，以后有时间再详细谈谈。
1. [Wikipedia](http://en.wikipedia.org/wiki/Metric_%28mathematics%29)
2. CMU的Liu Yang总结的关于DML的[综述页面](http://www.cs.cmu.edu/~liuy/distlearn.htm)。对DML的经典算法进行了分类总结，其中她总结的[论文](http://www.cs.cmu.edu/~liuy/frame_survey_v2.pdf)非常有价值，也是我的入门读物。
3. ECCV 2010的[turorial](http://www.cs.huji.ac.il/~ofirpele/DFML_ECCV2010_tutorial/)。
4. Weinberger的[页面](http://www.cse.wustl.edu/~kilian/page9/page15/page15.html)，上面有LMNN（Distance Metric Learning for Large Margin Nearest Neighbor Classification）的论文、[sclides](http://www.cse.wustl.edu/~kilian/talks/lmnn4.pdf)和代码。
5. [ITML](http://www.cs.utexas.edu/~pjain/itml/)(Information Throretic Metric Learning)。ITML是DML的经典算法，获得了ICML 2007的best paper award。[sclides](http://bengio.abracadoudou.com/lce/slides/davis.pdf)。
**顶级会议上矩阵学习的paper清单**：[http://blog.csdn.net/lzt1983/article/details/7831524](http://blog.csdn.net/lzt1983/article/details/7831524)
近2年顶级会议上度量学习相关的论文，数量之多，颇受震动。这其中怕是不乏灌水炒作新概念的文章，看来DML大有前几年sparse coding的势头啊。
**[ICML 2012](http://icml.cc/2012/papers/)**
[Maximum Margin Output Coding](http://icml.cc/2012/papers/778.pdf)
[Information-theoretic Semi-supervised Metric Learning via Entropy Regularization](http://icml.cc/2012/papers/74.pdf)
[A Hybrid Algorithm for Convex Semidefinite Optimization](http://icml.cc/2012/papers/109.pdf)
[Information-Theoretical Learning of Discriminative Clusters for Unsupervised Domain Adaptation](http://icml.cc/2012/papers/566.pdf)
[Similarity Learning for Provably Accurate Sparse Linear Classification](http://icml.cc/2012/papers/919.pdf)
**[ICML 2011](http://www.icml-2011.org/papers.php)**
[Learning Discriminative Fisher Kernels](http://www.icml-2011.org/papers/178_icmlpaper.pdf)
[Learning Multi-View Neighborhood Preserving Projections](http://www.icml-2011.org/papers/304_icmlpaper.pdf)
**[CVPR 2012](http://www.cvpr2012.org/program-details/papers)**
Order Determination and Sparsity-Regularized Metric Learning for Adaptive Visual Tracking
Non-sparse Linear Representations for Visual Tracking with Online Reservoir Metric Learning
[Unsupervised Metric Fusion by Cross Diffusion](http://www.loni.ucla.edu/~ztu/publication/cvpr12_CrDP.pdf)
[Learning Hierarchical Similarity Metrics](http://cseweb.ucsd.edu/~naverma/papers/hier_embd.pdf)
Large Scale Metric Learning from Equivalence Constraints
Neighborhood Repulsed Metric Learning for Kinship Verification
Learning Robust and Discriminative Multi-Instance Distance for Cost Effective Video Classification
[PCCA: a new approach for distance learning from sparse pairwise constraints](https://users.info.unicaen.fr/~jurie/papers/12_cvpr_ldca.pdf)
[Group Action Induced Distances for Averaging and Clustering Linear Dynamical Systems with Applications
 to the Analysis of Dynamic Visual Scenes](http://www.cis.jhu.edu/~rizwanch/papers/AfsariCVPR12.pdf)
**[CVPR 2011](http://www.cvpapers.com/cvpr2011.html)**
[A Scalable Dual Approach to Semidefinite Metric Learning](http://www.nicta.com.au/pub?doc=4745)
[AdaBoost
 on Low-Rank PSD Matrices for Metric Learning with Applications in Computer Aided Diagnosis](https://6274085717921901633-a-1802744773732722657-s-sites.googlegroups.com/site/tabatabaye/MetricBoost_IEEE3_final.pdf?attachauth=ANoY7cp_grw7MgEcmJugx8Lh5ioCAhdANw7xfTt9oz-JZueLuWs_0kq0fcbA7m-m_4mD4xRWDiong7X4UI2DqhcHmzRsM6-iJQhu6wfUIN8iVsokD9gXEqc6n1VJjlSAwX4U5OembQQ4SZeBJ71UepnKIIeP6LXGGFfQKRlxJrBZPn4tH3BzuMgWUF942T77IKGRIQrbDf_nvgeUYjtMS5iVc_ZwseOl0A%3D%3D&attredirects=0)
Adaptive Metric Differential Tracking (HUST)
Tracking Low Resolution Objects by Metric Preservation (HUST)
**[ACM MM 2012](http://www.acmmm12.org/accepted-papers/)**
Optimal Semi-Supervised Metric Learning for Image Retrieval
Low Rank Metric Learning for Social Image Retrieval
Activity-Based Person Identification Using Sparse Coding and Discriminative Metric Learning
Deep Nonlinear Metric Learning with Independent Subspace Analysis for Face Verification
**ACM MM 2011**
Biased Metric Learning for Person-Independent Head Pose Estimation
**[ICCV 2011](http://www.iccv2011.org/authors/accepted-papers)**
Learning Mixtures of Sparse Distance Metrics for Classification and Dimensionality Reduction
Unsupervised Metric Learning for Face Identification in TV Video
Random Ensemble Metrics for Object Recognition
Learning Nonlinear Distance Functions using Neural Network for Regression with Application to Robust Human Age Estimation
Learning parameterized histogram kernels on the simplex manifold for image and action classification
**[ECCV 2012](http://eccv2012.unifi.it/program/main-conference/)**
Metric Learning for Large Scale Image Classification: Generalizing to New Classes at Near-Zero Cost
Dual-force Metric Learning for Robust Distractor Resistant Tracker
Learning to Match Appearances by Correlations in a Covariance Metric Space
Image Annotation Using Metric Learning in Semantic Neighbourhoods
Measuring Image Distances via Embedding in a Semantic Manifold
Supervised Earth Mover’s Distance Learning and Its Computer Vision Applications
Learning Class-to-Image Distance via Large Margin and L1-norm Regularization
Labeling Images by Integrating Sparse Multiple Distance Learning and Semantic Context Modeling
**[IJCAI 2011](http://ijcai-11.iiia.csic.es/program/accepted_papers/)**
Distance Metric Learning Under Covariate Shift
Learning a Distance Metric by Empirical Loss Minimization
**[AAAI 2011](https://docs.google.com/viewer?url=http%3A%2F%2Fwww.aaai.org%2FConferences%2FAAAI%2F2011%2Faaai11accepts.pdf)**
Efficiently Learning a Distance Metric for Large Margin Nearest Neighbor Classification
[**NIPS 2011**](http://nips.cc/Conferences/2011/Program/accepted-papers.php)
[Learning a Distance Metric from a Network](http://books.nips.cc/papers/files/nips24/NIPS2011_1070.pdf)
[Learning a Tree of Metrics with Disjoint Visual Features](http://blog.csdn.net/lzt1983/article/details/www-bcf.usc.edu/~feisha/pubs/tom.pdf)
[Metric Learning with Multiple Kernels](http://books.nips.cc/papers/files/nips24/NIPS2011_0683.pdf)
**[KDD 2012](http://kdd2012.sigkdd.org/papers.shtml)**
[Random Forests for Metric Learning with Implicit Pairwise Position Dependence](http://arxiv.org/pdf/1201.0610.pdf)
**WSDM 2011**
[Mining Social Images with Distance Metric Learning for Automated
 Image Tagging](http://www.ntu.edu.sg/home/yhe/papers/wsdm.pdf)
