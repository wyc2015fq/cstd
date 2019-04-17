# 一些支持向量机（SVM）的开源代码库的链接及其简介 - xiahouzuoxin - CSDN博客





2013年03月02日 18:10:11[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：2897








**转载出处：http://blog.csdn.net/carson2005/article/details/8586201**



**（1）LIBSVM:**http://www.csie.ntu.edu.tw/~cjlin/libsvm/



**LIBSVM **is an integrated software for support vector classification, (C-SVC, [nu-SVC](http://www.csie.ntu.edu.tw/~cjlin/libsvm/#nuandone)), regression (epsilon-SVR, [nu-SVR](http://www.csie.ntu.edu.tw/~cjlin/libsvm/#nuandone))
 and distribution estimation ([one-class SVM](http://www.csie.ntu.edu.tw/~cjlin/libsvm/#nuandone)). It supports multi-class classification.


Since version 2.8, it implements an SMO-type algorithm proposed in this paper:

R.-E. Fan, P.-H. Chen, and C.-J. Lin. [Working set selection using second order information for training SVM](http://www.csie.ntu.edu.tw/~cjlin/papers/quadworkset.pdf). Journal of Machine Learning
 Research 6, 1889-1918, 2005. You can also find a pseudo code there. ([how to cite LIBSVM](http://www.csie.ntu.edu.tw/~cjlin/libsvm/faq.html#f203))


Our goal is to help users from other fields to easily use SVM as a tool. **LIBSVM **provides a simple interface where users can easily link it with their own programs. Main features of **LIBSVM** include
- Different SVM formulations
- Efficient multi-class classification
- Cross validation for model selection
- Probability estimates
- Various kernels (including precomputed kernel matrix)
- Weighted SVM for unbalanced data
- Both C++ and [Java](http://www.csie.ntu.edu.tw/~cjlin/libsvm/#java) sources
- [GUI](http://www.csie.ntu.edu.tw/~cjlin/libsvm/#GUI) demonstrating SVM classification and regression
- [Python](http://www.csie.ntu.edu.tw/~cjlin/libsvm/#python), [R](http://www.csie.ntu.edu.tw/~cjlin/libsvm/#R), [MATLAB](http://www.csie.ntu.edu.tw/~cjlin/libsvm/#matlab), [Perl](http://www.csie.ntu.edu.tw/~cjlin/libsvm/#perl), [Ruby](http://www.csie.ntu.edu.tw/~cjlin/libsvm/#ruby), [Weka](http://www.csie.ntu.edu.tw/~cjlin/libsvm/#weka), [Common
 LISP](http://www.csie.ntu.edu.tw/~cjlin/libsvm/#lisp), [CLISP](http://www.csie.ntu.edu.tw/~cjlin/libsvm/#clisp), [Haskell](http://www.csie.ntu.edu.tw/~cjlin/libsvm/#haskell), [LabVIEW](http://www.csie.ntu.edu.tw/~cjlin/libsvm/#labview),
 and [PHP](http://www.csie.ntu.edu.tw/~cjlin/libsvm/#PHP) interfaces. [C# .NET](http://www.csie.ntu.edu.tw/~cjlin/libsvm/#csharp) code
 and [CUDA](http://www.csie.ntu.edu.tw/~cjlin/libsvm/#cuda) extension is available. 

It's also included in some data mining environments: [RapidMiner](http://rapid-i.com/), [PCP](http://pcp.sourceforge.net/), and [LIONsolver](http://www.lionsolver.com/).
- Automatic model selection which can generate contour of cross valiation accuracy.




**(2)LIBLINEAR**http://www.csie.ntu.edu.tw/~cjlin/liblinear/


**LIBLINEAR **is a **linear** classifier for data with **millions** of instances and features. It supports
- L2-regularized classifiers 

L2-loss linear SVM, L1-loss linear SVM, and logistic regression (LR)
- L1-regularized classifiers (after version 1.4) 

L2-loss linear SVM and logistic regression (LR)
- L2-regularized support vector regression (after version 1.9) 

L2-loss linear SVR and L1-loss linear SVR.

Main features of **LIBLINEAR** include
- Same data format as [**LIBSVM**](http://www.csie.ntu.edu.tw/~cjlin/libsvm), our general-purpose SVM solver, and also similar usage
- Multi-class classification: 1) one-vs-the rest, 2) Crammer & Singer
- Cross validation for model selection
- Probability estimates (logistic regression only)
- Weights for unbalanced data
- MATLAB/Octave, Java, Python, Ruby interfaces



**(3)SVMlight**   http://www.cs.cornell.edu/People/tj/svm_light/


SVM*light* is an implementation of Vapnik's Support Vector Machine [[Vapnik, 1995](http://www.cs.cornell.edu/People/tj/svm_light/#References)]
 for the problem of pattern recognition, for the problem of regression, and for the problem of learning a ranking function. The optimization algorithms used in SVM*light* are described in [[Joachims,
 2002a](http://www.cs.cornell.edu/People/tj/svm_light/#References) ]. [[Joachims, 1999a](http://www.cs.cornell.edu/People/tj/svm_light/#References)]. The algorithm has scalable memory requirements and can handle problems with many thousands of support
 vectors efficiently.

The software also provides methods for assessing the generalization performance efficiently. It includes two efficient estimation methods for both error rate and precision/recall. XiAlpha-estimates [[Joachims,
 2002a](http://www.cs.cornell.edu/People/tj/svm_light/#References), [Joachims, 2000b](http://www.cs.cornell.edu/People/tj/svm_light/#References)] can be computed at essentially no computational expense, but they are conservatively biased. Almost unbiased
 estimates provides leave-one-out testing. SVM*light* exploits that the results of most leave-one-outs (often more than 99%) are predetermined and need not be computed [[Joachims,
 2002a](http://www.cs.cornell.edu/People/tj/svm_light/#References)].

New in this version is an algorithm for learning ranking functions [[Joachims, 2002c](http://www.cs.cornell.edu/People/tj/svm_light/#References)]. The goal is to
 learn a function from preference examples, so that it orders a new set of objects as accurately as possible. Such ranking problems naturally occur in applications like search engines and recommender systems.

Futhermore, this version includes an algorithm for training large-scale transductive SVMs. The algorithm proceeds by solving a sequence of optimization problems lower-bounding the solution using a form of local search. A detailed
 description of the algorithm can be found in [[Joachims, 1999c](http://www.cs.cornell.edu/People/tj/svm_light/#References)]. A similar transductive learner, which can be thought of as a transductive
 version of k-Nearest Neighbor is the [Spectral Graph Transducer](http://sgt.joachims.org/).

SVM*light* can also train SVMs with cost models (see [[Morik et al., 1999](http://www.cs.cornell.edu/People/tj/svm_light/#References)]).

The code has been used on a large range of problems, including text classification [[Joachims, 1999c](http://www.cs.cornell.edu/People/tj/svm_light/#References)][[Joachims,
 1998a](http://www.cs.cornell.edu/People/tj/svm_light/#References)], image recognition tasks, bioinformatics and medical applications. Many tasks have the property of sparse instance vectors. This implementation makes use of this property which leads to a very compact and efficient representation.



**(4)SVM struct**    http://www.cs.cornell.edu/People/tj/svm_light/svm_struct.html


*SVMstruct* is a Support Vector Machine (SVM) algorithm for predicting multivariate or structured outputs. It performs supervised learning by approximating a mapping
*h: X --> Y*
using labeled training examples (x1,y1), ..., (xn,yn). Unlike regular SVMs, however, which consider only univariate predictions like in classification and regression,*SVMstruct* can
 predict complex objects y like trees, sequences, or sets. Examples of problems with complex outputs are natural language parsing, sequence alignment in protein homology detection, and markov models for part-of-speech tagging. The *SVMstruct *algorithm
 can also be used for linear-time training of binary and multi-class SVMs under the linear kernel [4].

The 1-slack cutting-plane algorithm implemented in *SVMstruct* V3.10 uses a new but equivalent formulation of the structural SVM quadratic program and is several orders of magnitude faster than prior methods.
 The algorithm is described in [5]. The n-slack algorithm of *SVMstruct* V2.50 is described in [1][2]. The *SVMstruct *implementation is based on the [*SVMlight*](http://www.cs.cornell.edu/People/tj/svm_light/index.html) quadratic
 optimizer 





**(5)BSVM** http://www.csie.ntu.edu.tw/~cjlin/bsvm/

BSVM solves support vector machines (SVM) for the solution of large classification and regression problems. It includes the following methods- One vs. One multi-class classification using a bound-constrained formulation
- Multi-class classification by solving a single optimization problem (again, a bounded formulation). See Section 3 of our comparison paper.
- Multi-class classification using [Crammer and Singer's formulation](http://www.csie.ntu.edu.tw/~cjlin/bsvm/#cands). See Section 4 of our comparison paper.
- Regression using a bound-constrained formulation
- Multi-class classification using Crammer and Singer's formulation with squared hinge (L2) loss

The current implementation borrows the structure of [**libsvm**](http://www.csie.ntu.edu.tw/~cjlin/libsvm). Similar
 options are also adopted. For the bound-constrained formulation for classification and regression, **BSVM **uses a decomposition method. **BSVM **uses a simple working set selection which leads to faster convergences for difficult
 cases. The use of a special implementation of the opmization solver [**TRON**](http://www.mcs.anl.gov/~more/tron)allows **BSVM **to stably identify bounded variables.



**(6)M-SVM**   http://www.loria.fr/~guermeur/

multi class SVM implementation in c by Guermeur




**(7)MATLAB SVM toolbox**    http://www.isis.ecs.soton.ac.uk/resources/svminfo/

The toolbox provides routines for support vector classification and support vector regression. A GUI is included which allows the visualisation of simple classification
 and regression problems. (The MATLAB optimisation toolbox, or an alternative quadratic programming routine is required.)





**(8)TinySVM**    http://chasen.org/~taku/software/TinySVM/

TinySVM is an implementation of Support Vector Machines (SVMs) [[Vapnik
 95]](http://chasen.org/~taku/software/TinySVM/#vapnik95), [[Vapnik 98]](http://chasen.org/~taku/software/TinySVM/#vapnik98) for the problem of pattern
 recognition. Support Vector Machines is a new generation learning algorithms based on recent advances in statistical learning theory, and applied to large number of real-world applications, such as text categorization, hand-written character recognition.





**(9)GPDT**   http://dm.unife.it/gpdt/



GPDT is a C++ software designed to train large-scale Support Vector Machines (SVMs) for binary classification in both scalar and distributed memory parallel environments. It uses a popular problem decomposition technique [[1](http://dm.unife.it/gpdt/#joachims), [2](http://dm.unife.it/gpdt/#parco03), [4](http://dm.unife.it/gpdt/#oms05), [6](http://dm.unife.it/gpdt/#cms2005), [7](http://dm.unife.it/gpdt/#rep2006)]
 to split the SVM quadratic programming (QP) problem into a sequence of smaller QP subproblems, each one being solved by a suitable gradient projection method (GPM). The currently implemented GPMs are the Generalized Variable Projection Method (GVPM) [[3](http://dm.unife.it/gpdt/#oms04)]
 and the Dai-Fletcher method (DFGPM) [[5](http://dm.unife.it/gpdt/#dai-fletcher)].


A few minor bugs fixed (see more details in the [CHANGES](http://dm.unife.it/gpdt/CHANGES) file, also packaged with the sources distribution) 
[Last updated: Fabruary 7, 2007.]



**(10)Spider**   http://people.kyb.tuebingen.mpg.de/spider/

The spider is intended to be a complete object orientated environment for machine learning in Matlab. Aside from easy use of base learning algorithms, algorithms can be plugged
 together and can be compared with, e.g model selection, statistical tests and visual plots. This gives all the power of objects (reusability, plug together, share code) but also all the power of Matlab for machine learning research.





**(11)HeroSVM**    http://www.cenparmi.concordia.ca/~jdong/HeroSvm.html

Support Vector Machine (SVM)  represents the state-of-the-art classification technique. However, training SVM on a large training set becomes a bottleneck. HeroSvm
 is a high-performance library for training SVM for classification to solve this problem. It  has been implemented based on our proposed method [1][5][22].  In order to facilitate the software portability and maintenance, an object-oriented method has been
 applied to design the package. Error handling is supported and HeroSvm is exception-safe. HeroSvm is written in C++. In the current version, a dynamic link library in windows or a shared library in linux is provided to train SVM on a large-scale learning problem
 efficiently for research purpose in PC platform. We expect that HeroSVM can facilitate the training of support vector machine and solve some real-world problems in various engineering fields.





**(12)LS-SVMlab**   http://www.esat.kuleuven.be/sista/lssvmlab/

Support Vector Machines is a powerful methodology for solving problems in nonlinear classification, function estimation and density estimation which has also led
 to many other recent developments in kernel based methods in general. Originally, it has been introduced within the context of statistical learning theory and structural risk minimization. In the methods one solves convex optimization problems, typically quadratic
 programs. Least Squares Support Vector Machines (LS-SVM) are reformulations to the standard SVMs which lead to solving linear KKT systems. LS-SVMs are closely related to regularization networks and Gaussian processes but additionally emphasize and exploit
 primal-dual interpretations. Links between kernel versions of classical pattern recognition algorithms such as kernel Fisher discriminant analysis and extensions to unsupervised learning, recurrent networks and control are available. Robustness, sparseness
 and weightings can be incorporated into LS-SVMs where needed and a Bayesian framework with three levels of inference has been developed. LS-SVM based primal-dual formulations have been given to kernel PCA, kernel CCA and kernel PLS. Recent developments are
 in kernel spectral clustering, data visualization and dimensionality reduction, and survival analysis. For very large scale problems a method of Fixed Size LS-SVM is proposed. The present LS-SVMlab toolbox contains Matlab/C implementations for a number of
 LS-SVM algorithms.





**(13)LSVM**   http://research.cs.wisc.edu/dmi/lsvm/


LSVM is a fast technique for training support vector machines (SVMs), based on a simple iterative approach. For example, it has been used to classify a dataset with 2 million points and 10 features in only 34 minutes on a 400 Mhz Pentium II. For more information,
 see our paper [Lagrangian Support Vector Machines](ftp://ftp.cs.wisc.edu/pub/dmi/tech-reports/00-06.ps).

SVMs are optimization based tools for solving machine learning problems. For an introduction to SVMs, you may want to look at this [tutorial](http://www.kernel-machines.org/papers/Burges98.ps.gz).

The software is free for academic and research use. For commercial use, please contact [Olvi Mangasarian](mailto:olvi@cs.wisc.edu) or [Dave
 Musicant](mailto:musicant@cs.wisc.edu).

[Click here to download the software,](http://research.cs.wisc.edu/dmi/lsvm/license.html) which consists of [MATLAB](http://www.mathworks.com/) m-files.

If you publish any work based on LSVM, please cite both the software and the paper on which it is based. Here are recommended LaTeX bibliography entries:

@misc{lsvm,

author = "O.L. Mangasarian and D. R. Musicant",

title = {{LSVM Software:} Active Set Support Vector Machine Classification Software},

year = 2000,

institution = {Computer Sciences Department, University of Wisconsin, Madison},

note = { www.cs.wisc.edu/$\sim$musicant/lsvm/.}}

@techreport{mm:00,

author = "O. L. Mangasarian and David R. Musicant",

title = "Lagrangian Support Vector Machine Classification",

institution = "Data Mining Institute, Computer Sciences Department, University of Wisconsin",

month = {June},

year = 2000,

number = {00-06},

address = "Madison, Wisconsin",

note={ftp://ftp.cs.wisc.edu/pub/dmi/tech-reports/00-06.ps}}

For more information, contact:

Olvi L. Mangasarian
[olvi@cs.wisc.edu](mailto:olvi@cs.wisc.edu)

David R. Musicant
[dmusican@carleton.edu](mailto:dmusican@carleton.edu)




**(14)ASVM**    http://research.cs.wisc.edu/dmi/asvm/



ASVM is a fast technique for training linear support vector machines (SVMs), based on an active set approach which results in very fast running times. For example, it has been used to classify a dataset with 4 million points and 32 features in only 38 minutes
 on a 400 Mhz Pentium II. For more information, see our paper [Active Support Vector Machines](ftp://ftp.cs.wisc.edu/pub/dmi/tech-reports/00-04.ps).

SVMs are an optimization based approach for solving machine learning problems. For an introduction to SVMs, you may want to look at this [tutorial](http://www.kernel-machines.org/papers/Burges98.ps.gz).

The software is free for academic use. For commercial use, please contact [Dave Musicant](mailto:dmusican@carleton.edu).

[Click here to download the software.](http://research.cs.wisc.edu/dmi/asvm/license.html) The software consists of:
- A stand-alone executable to do training
- A stand-alone executable to do testing
- A mexfile for use in the [MATLAB](http://www.mathworks.com/) environment

No additional software whatsoever is required to use these tools.

If you publish any work based on ASVM, please cite both the software and the paper on which it is based. Here are recommended LaTeX bibliography entries:

@misc{asvm,

author = "D. R. Musicant",

title = {{ASVM Software:} Active Set Support Vector Machine Classification Software},

year = 2000,

institution = {Computer Sciences Department, University of Wisconsin, Madison},

note = { www.cs.wisc.edu/$\sim$musicant/asvm/.}}

@techreport{mm:00,

author = "O. L. Mangasarian and David R. Musicant",

title = "Active Support Vector Machine Classification",

institution = "Data Mining Institute, Computer Sciences Department, University of Wisconsin",

month = {April},

year = 2000,

number = {00-04},

address = "Madison, Wisconsin",

note={ftp://ftp.cs.wisc.edu/pub/dmi/tech-reports/00-04.ps}}

For more information, contact:

David R. Musicant
[dmusican@carleton.edu](mailto:dmusican@carleton.edu)
**(15)psvm **   http://research.cs.wisc.edu/dmi/svm/psvm/


Iinstead of a standard support vector machine that classifies points by assigning them to one of two disjoint half-spaces, PSVM classifies points by assigning them to the closest of two parallel planes. For more
 information, see our paper [Proximal Support Vector Machines](ftp://ftp.cs.wisc.edu/pub/dmi/tech-reports/01-02.ps).

SVMs are an optimization based approach for solving machine learning problems. For an introduction to SVMs, you may want to look at this [tutorial](http://www.kernel-machines.org/papers/Burges98.ps.gz).

The software is free for academic use. For commercial use, please contact [Olvi Mangasarian](mailto:olvi@cs.wisc.edu).

[Click here to download the software.](http://research.cs.wisc.edu/dmi/svm/psvm/license.html) The software consists of:
- A linear version of the PSVM
- A nonlinear version of the PSVM

The only software needed to run these programs is MATLAB [www.mathworks.com](http://www.mathworks.com/).

**(16)Linear SVM**   http://linearsvm.com/

Linear SVM is the newest extremely fast machine learning (data mining) algorithm for solving multiclassclassification problems from ultra
 large data sets that implements an original proprietary version of acutting plane algorithm for designing a linear support vector machine. LinearSVM is a linearly scalable routine meaning
 that it creates an SVM model in a CPU time which scales linearly with the size of the training data set. Our comparisons with other known SVM models clearly show its superior performance when high accuracy is required. We would highly
 appreciate if you may share LinearSVM performance on your data sets with us.




