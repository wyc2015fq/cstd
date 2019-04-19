# Weka and Hadoop - 三少GG - CSDN博客
2013年12月23日 00:06:46[三少GG](https://me.csdn.net/scut1135)阅读数：4434
# Mark Hall on Data Mining & Weka
### [Weka and Hadoop Part 3](http://markahall.blogspot.com/2013/10/weka-and-hadoop-part-3.html)
This is the third of three posts covering some new functionality for distributed processing in Weka. The [first](http://markahall.blogspot.co.nz/2013/10/weka-and-hadoop-part-1.html) and [second](http://markahall.blogspot.co.nz/2013/10/weka-and-hadoop-part-2.html) installments
 covered base functionality and some of the Hadoop-specific wrappers. In this post we'll look at the remaining functionality in version 1.0 of the[distributedWekaHadoop](http://weka.sourceforge.net/packageMetaData/distributedWekaHadoop/index.html) package.
### Weka classifier evaluation job
This job builds on the classifier training job covered in [installment 2](http://markahall.blogspot.co.nz/2013/10/weka-and-hadoop-part-2.html) and provides
 map-reduce tasks to evaluate classifiers via the training data, a separate test set or cross-validation. Apart from ARFF header creation and the optional randomisation/stratification phase (both of which are re-usable once run initially), the evaluation job
 involves two passes over the data. The first builds the model and the second performs the evaluation.
In the case of a *k*-fold cross-validation, each mapper for the model building job divides its dataset up into *k* folds and builds *k* models in one hit. The reduce phase for the model building job can use up to *k * reducers, with
 a reduce operation aggregating all the models for one fold of the cross-validation. The input to the evaluation pass over the data is then the aggregated model (*k* aggregated models in the case of cross-validation), pushed out to the nodes via the
 distributed cache, and either the input data (in the case of test on training or cross-validation) or a separate test set. In the case where the models are batch trained, the data at each map is randomly shuffled and then divided into stratified folds. In
 the case where the models are incrementally trained, the cross-validation folds are created and processed in a streaming fashion by extracting the instances for a given fold using a modulus operation. The same random seed is used in both the model building
 and evaluation job in order to keep the folds consistent.
![](http://1.bp.blogspot.com/-5lV7oDQ0bAA/UlM5luIkJaI/AAAAAAAAAKU/Zm-LMNLCbEU/s640/EvalJob1.png)
The evaluation job adds only a few options over and above those in the classifier job. You can specify the number of nodes in your cluster so that the job can specify up to *k* reducers for a cross-validation.
 Weka's evaluation module computes just about all of its metrics incrementally in an additive fashion (perfectly suited to aggregation). The only exceptions are area under the ROC curve and area under the precision recall curve. These require predictions to
 be retained. By default, the evaluation job does not compute these two statistics. They can be computed by providing a number for the "sampleFractionForAUC" option. This allows the user to specify some percentage of the total number of predictions generated
 to be retained (via uniform random sampling) for computing these two statistics. In the above screenshot, we've set this to 0.5 - i.e. 50% of all the predictions generated in all the map tasks will be retained.
In the earlier discussion of the classifier training job we used it to build a model on all the data. It can also be used to train a model on a specific fold of a cross-validation by setting the "foldNumber" and "totalNumFolds"
 options. When the evaluation job uses the classifier job to perform cross-validation it sets the "foldNumber" option automatically in order to learn models for each of the folds. All we have to do when configuring the evaluation job is to set the "totalNumFolds"
 parameter.
The output of the evaluation job is the standard Weka evaluation results text (like when the Explorer or command line interface to Weka is used normally) and the metrics stored in a single line CSV and ARFF file. All
 of these files are written out to the "eval" subdirectory of the output directory in HDFS for the job.
![](http://1.bp.blogspot.com/-nZ_bEOSjMbI/UlM9drfZZ8I/AAAAAAAAAKg/dVj6XhTJ5bM/s640/EvalJob2.png)
### Scoring job
The last Hadoop job in the version 1.0 release of the package is one to perform scoring (prediction) using a trained model. This job actually handles scoring using clusterers as well as classifiers, even though there aren't any clustering tasks/jobs in
 version 1.0 (stuff to do for version 1.1...).
![](http://3.bp.blogspot.com/-a4QpaQUGK64/UlNDxA27LNI/AAAAAAAAAKw/E8ePIc1_0tg/s640/ScoringJob1.png)
The job doesn't require a reduce phase, so there will be as many output files in the output directory as there are map tasks run for the dataset being scored. Again the distributed cache is used to place the model on
 the local file system of each node. The model to be used can be initially on the local file system or in HDFS - the job looks in both places.
The map tasks build a mapping between the incoming data fields and what the model is expecting. Missing data fields, nominal values that haven't been seen during training and type mismatches between what the model is
 expecting and what is in the current input row are replaced with missing values. During the setup phase, when the mapping is being built, the job will fail if there are fewer than 50% of the attributes that the model is expecting to see present in the incoming
 data.
The map tasks output CSV data in the same format as the input data but with the predicted probability distribution (comma-separated label:probability pairs) appended to the end of each row. The user can opt to output
 fewer than all the input columns by setting the "columnsToOutputInScoredData" option.
### Orchestrating jobs
The Hadoop jobs can be chained together using the sequential execution facility in the Knowledge Flow and/or new "success" and "failure" event types. The following screenshot shows a flow that:
- Transfers the hypothyroid data into HDFS
- Runs the correlation matrix + PCA job (which also executes the ARFF header creation job first)
- Re-uses the ARFF header and PCA filter created in step 2 to learn a filtered bagging model
- Extracts the learned model from HDFS and saves it to the local file system
![](http://4.bp.blogspot.com/-WoJnf7RXhwo/UlNQegDz9oI/AAAAAAAAALA/sRGpLjA8E4g/s640/TwoJobs.png)
As mentioned in the first installment of this series, all the jobs have an extensive command-line interface to facilitate scripting.
### A note for Windows users
If you are running the Weka jobs from Windows and your Hadoop cluster is running on *nix machines then you will run into an issue with the classpath for the map and reduce tasks on the *nix side of things. It turns out that setting the classpath for a
 Hadoop job programatically uses the path separator character of the client system (naturally I guess). So under Windows the ";" character is used to separate entries in the classpath that is set in the Configuration object for the job. This will result in
 ClassNotFound exceptions when the job is actually executed on the *nix cluster. To get around this the Weka jobs will postprocess the classpath entry in the Configuration to replace ";"s with ":"s, but only if you tell it that you're running a Windows client
 against a *nix Hadoop cluster. To do this you just need to set the environment variable HADOOP_ON_LINUX=true. This is pretty hacky and if anyone knows of a more elegant solution to this please let me know.
### Benchmarking on the KDD99 data
I ran a quick test on the KDD99 data set (just under 5 million instances, 42 attributes and 23 classes) on Waikato's [Symphony](http://wiki.symphony.waikato.ac.nz/index.html) torque
 cluster (quad core i7 processors at 2793 MHz). I set up a 10 node Hadoop cluster and ran a 10-fold cross-validation of a random forest consisting of 200 trees. The job involved creating the ARFF header, creating 15 randomly shuffled input chunks and then the
 evaluation itself. This took just under 5 minutes to run. Subsequent runs of 10-fold cross-validation using the already created input chunks took about 3 and a half minutes.
java weka.distributed.hadoop.WekaClassifierEvaluationHadoopJob \
-hdfs-host 192.168.22.240 -hdfs-port 9000 \
-jobtracker-host 192.168.22.240 -jobtracker-port 9001 \
-input-paths /users/mhall/input/kdd99 \
-output-path /users/mhall/output \
-header-file-name kdd99.arff -max-split-size 50000000 \
-randomized-chunks -num-chunks 15 \
-W weka.classifiers.meta.Bagging -total-folds 10 \
-num-nodes 10 -logging-interval 5 \
-user-prop mapred.child.java.opts=-Xmx1200m \
-- -W weka.classifiers.trees.RandomTree -I 200 \
-- -depth 3 -K 3
![](http://4.bp.blogspot.com/-Rg7Vub3aCW4/Ulx4XHhK7fI/AAAAAAAAALo/Nl9w1xZc6XQ/s1600/Symphony2.png)
Next I doubled the size of the input data (just by duplicating the kdd 99 data), to give just under 10 million instances, and launched a 15 node Hadoop cluster. I ran the same job as before but increased the number
 of randomly shuffled data chunks from 15 to 30 (in order to keep the amount of data entering each map the same as before). This time the job ran in 4 minutes and 23 seconds (the average over several repetitions was about 4 minutes). Although each map is processing
 the same amount of data, the faster run time is explained by greater parallelism - each map in the model building process now only has to build half as many trees as it did in the first job in order to generate a forest of 200 trees.
![](http://3.bp.blogspot.com/-OzjPM838UWE/Ulx927WJPrI/AAAAAAAAAL4/3MHtkmmEozI/s1600/Symphony3.png)
### Future stuff
There is a bunch of stuff that could go into future releases of the distributed packages. Some things I'd like to add for the next release include:
- Clustering. *k*-means first probably.
- More text mining stuff. [SGDText](http://wiki.pentaho.com/display/DATAMINING/SGDText) and [NaiveBayesMultinomialText](http://wiki.pentaho.com/display/DATAMINING/NaiveBayesMultinomialText) can
 already be used in version 1.0 of the distributed packages. Weka's StringToWordVector filter really needs an option to allow a dictionary to be supplied by the user. Once this is done, we could have a job to create a dictionary (and IDF counts) - basically
 just a modification of the classic word count MR job - and then use the [StringToWordVector](http://wiki.pentaho.com/display/DATAMINING/StringToWordVector) filter
 as normal.
- The SubstringLabeler and SubstringReplacer Knowledge Flow steps need to become filters so that they can be used for pre-processing in the classifier training job. This would allow the [twitter
 sentiment analysis example](http://markahall.blogspot.co.nz/2012/03/sentiment-analysis-with-weka.html) (which involves automatic creation of labelled training data) to be implemented as a map-reduce job.
- Allow ensembles of heterogeneous classifiers to be learned with the classifier job. At present, only a voted ensemble of classifiers of the same type can be learned. The job could be extended to allow the user
 to specify a set of base classifiers and then the map tasks could use their task number as a basis for choosing which classifier to build from the set.
- Oversampling in the randomly shuffled chunk creation task. This job already makes sure that minority classes have at least one instance in all data chunks but it could be extended to bias the final distribution
 of classes in each chunk towards a uniform distribution.
- Possibly the execution of a Knowledge Flow process in a map or reduce task.
Posted by [Mark
 Hall](http://www.blogger.com/profile/11041720517232023634)at [18:42](http://markahall.blogspot.com/2013/10/weka-and-hadoop-part-3.html)[No
 comments:](http://markahall.blogspot.com/2013/10/weka-and-hadoop-part-3.html#comment-form)
[Email
 This](http://www.blogger.com/share-post.g?blogID=7927943585854248111&postID=1183685400391474119&target=email)[BlogThis!](http://www.blogger.com/share-post.g?blogID=7927943585854248111&postID=1183685400391474119&target=blog)[Share
 to Twitter](http://www.blogger.com/share-post.g?blogID=7927943585854248111&postID=1183685400391474119&target=twitter)[Share
 to Facebook](http://www.blogger.com/share-post.g?blogID=7927943585854248111&postID=1183685400391474119&target=facebook)
### [Weka and Hadoop Part 2](http://markahall.blogspot.com/2013/10/weka-and-hadoop-part-2.html)
In the [first](http://markahall.blogspot.co.nz/2013/10/weka-and-hadoop-part-1.html) instalment of this series, we outlined what was available in version 1.0 of  new
 general distributed learning package for Weka called [distributedWekaBase](http://weka.sourceforge.net/packageMetaData/distributedWekaBase/index.html). We also
 started to look at some Hadoop-specific wrappers for the base tasks provided in a second new package called[distributedWekaHadoop](http://weka.sourceforge.net/packageMetaData/distributedWekaHadoop/index.html).
In this instalment we'll look at some more of the Hadoop-specific jobs.
### Correlation/covariance matrix creation job
This Hadoop job produces either a correlation or covariance matrix (in a format that Weka's Matrix class can parse). It can handle numeric or nominal data (or a mixture of both) but the correlation matrix is only computed from the numeric fields in the
 data. Like the other remaining Hadoop jobs in the distributedWekaHadoop package, it requires that the ARFF header job be run first, and will run it automatically for you unless the -use-existing-header option is specified. The correlation matrix job relies
 on the summary meta data attributes computed by the ARFF header job so that it can compute the matrix in one pass over the data.
As explained in the first instalment, the map tasks compute a partial matrix of covariance sums for all rows in the matrix using their chunk of the data. The reducer phase aggregates on the basis of individual rows in the matrix, so the maps output rows
 of the full, but partially computed, matrix one at a time. This allows plenty of parallelism in the reduce phase, but does create lots of final output files (one for each reducer) that contain some of the rows of the final matrix. The job automatically tidies
 this up by reading all the part-r-xxxxx files and writing back to HDFS the final matrix in Weka's textual format. The job has an option to specify how many nodes are in the user's cluster, and then sets the number of reducers to min(num nodes * max reducers
 to run in parallel per node, num rows in the matrix).
![](http://3.bp.blogspot.com/-DqPEdZY-MdM/UlHh03nYrJI/AAAAAAAAAIk/p6wrIhqI3Hc/s640/CorrMatrixJob1.png)
The job also has options to specify a class attribute (which, if it happens to be a numeric field, is not part of the correlation analysis unless "keepClassAttributeIfSet" is selected) and run a principal components analysis in a post-processing phase
 after the job completes. The later does not distribute the computation of PCA - it runs locally on the client machine and the results are written back into the HDFS output directory. As PCA has a runtime that is at best quadratic in the number of input fields,
 this phase of the job is suitable for datasets that don't have tons of attributes. The PCA phase also creates a special serialised Weka filter that can be used for pre-processing in the classifier job.
![](http://4.bp.blogspot.com/-y9O3SLU3tY0/UlHvi2vQSTI/AAAAAAAAAJM/9WudM9YIfoo/s640/CorrMatrixJob3.png)
![](http://3.bp.blogspot.com/-7yTGeYLxFzw/UlHvNCCniXI/AAAAAAAAAJE/x0F6pqCmdT4/s640/CorrMatrixJob2.png)
### Weka classifier builder job
This job uses map-reduce to build classifier models in Hadoop and is one of the most complicated due to the number of options it provides. It will run up to three distinct jobs types depending on options:
- ARFF header creation (can be omitted if this has already run previously)
- Optional creation of randomly shuffled (and stratified) input data chunks from the original data
- Training of a Weka model (can involve multiple passes/jobs over the entire data set in the case of iterative incremental algorithms like SGD)
 The optional randomised chunk creation phase gives greater control (compared to using the mapredMaxSplitSize option) over the number of maps actually run in the model learning phase. This is because 1) at least one map is used to process each distinct input
 file, and 2) the job provides options to either specify how many chunks to produce or how many instances should be in each chunk. This phase will also *stratify* the chunks, if the class is nominal, to ensure that each has approximately the same distribution
 of class values as the original dataset. The funky Hadoop MultipleOutputs class is used to write to multiple files from the reducer.
Note that separate runs of the randomised chunk creation phase may not be deterministic (even with the same random seed) due to the fact that keys (there is one per chunk) output from the map tasks are not guaranteed to arrive at the reducer in the same order
 from run to run, combined with the way the reducer "deals" instances out to the output files. However, once run, the randomly shuffled chunks may be re-used in subsequent model building and evaluation tasks.
![](http://3.bp.blogspot.com/-2pkxt5lui9w/UlIo58-XFyI/AAAAAAAAAJ0/NPBY6Rk1EFQ/s640/ClassifierJob3.png)
The model learning phase handles a number of different training scenarios:
- *Aggregateable* classifiers (produce one final model of the same type as the individual models)
- Incremental aggregateable classifiers - e.g. naive Bayes, naive Bayes multinomial and SGD
- Batch aggregateable classifiers - e.g. Bagging
- Non-aggregateable classifiers
- Incremental ones - e.g. Hoeffding trees, raced incremental logit boost
- Batch learners - the majority of the classifiers and regressors in Weka
In the case of non-aggregateable classifiers, the final model produced in the reduce phase is a voted ensemble of the models learned by the mappers. For the technically oriented, this is essentially a "Dagging" model. In all cases, the final serialised
 model is deposited in a "model" subdirectory of the output path of the job in HDFS, along with a copy of the ARFF header (sans summary attributes). The header can be prepended to new data sets and makes the model ready for deployment wherever required.
**A note on Bagging:** The job makes a special check for Bagging (actually for any method that extends weka.classifiers.IteratedSingleClassifierEnhancer) and will divide the total requested number of base models by the number of map tasks that
 will be run. Thus Bagging runs in each mapper in order to produce some of the total number of base models requested by the user. The random forest algorithm can be implemented by setting the base learner to RandomTree in Bagging. I guess the final model produced
 by using Bagging in Hadoop in this fashion is actually a Dagging one again, with the small difference that the base models trained by each map will have training datasets created by bootstrap sampling on the data chunk that enters the map.
The classifier job has a number of options to fine tune the creation of the final model:
- If **not** using the option to create randomly shuffled data chunks, and instead the mapredMaxSplitSize option is used to control the number of maps, then minTrainingFraction can be used to prune away
 a model created on a data split that contains less data that the others
- numIterations controls how many times the model learning phase is invoked. This option only makes sense for iterative incremental classifiers such as SGD. Each iteration is a separate complete pass over the data.
 Hadoop's **distributed cache** is used to distribute the intermediate model learned at iteration *i* out to the nodes for training to continue in the mappers at iteration *i + *1
- pathToPreconstructedFilter allows the PCA filter optionally created by the correlation matrix job to be used to transform the data coming into each map
- filtersToUse can be used instead of (or in conjunction with) pathToPreconstructedFilter in order to use standard Weka filters to pre-process data entering a map. The job automatically determines whether a given
 filter can be used with the base classifier and wraps the base classifier in one of several special subclasses of Weka's [FilteredClassifier](http://wiki.pentaho.com/display/DATAMINING/FilteredClassifier).
 For example, in order to maintain the ability to aggregate an *Aggregateable* classifier after filtering the input data, it is necessary that all filters used with it are *Streamable - *i.e., they can determine their output format from only header
 information (so that the output format produced by the filter is the same in each map) and don't buffer input data
- forceBatchLearningForUpdateableClassifiers will, as the name suggests, force an incremental classifier to be trained in a batch fashion. This essentially makes no difference for naive Bayes but does for SGD, which
 will perform a number of epochs over the training data entering a given map when trained in a batch fashion.
- useReservoirSamplingWhenBatchLearning results in the data streamed into each map getting passed into a reservoir sampling filter. This is useful to control the total number of instances processed in batch learning
 when the user is too lazy to tune the number of maps carefully via the mapredMaxSplitSize option or the option to create randomly shuffled data chunks.
![](http://1.bp.blogspot.com/-dCtrYYDHr98/UlIoVJZJjcI/AAAAAAAAAJo/TOEE15gJ5Bc/s640/ClassifierJob2.png)
![](http://1.bp.blogspot.com/-E0utg1m3U_o/UlIoj_AkxCI/AAAAAAAAAJs/M5wI6kNxVG4/s640/ClassifierJob4.png)
There are a couple of Hadoop configuration properties that can be useful when running the Weka classifier training job, particularly when running batch learners. The first is  mapred.child.java.opts. This
 allows you to specify arguments to the JVM that the mappers run in, and is particularly useful for increasing the amount of heap space. The default is 200Mb, which is not a lot. The heap space can be increased by supplying the -Xmx argument. Another useful
 property to be aware of is mapred.task.timeout. If the task tracker does not hear anything from a mapper/reducer for 10 minutes
 it will kill the task. When the Weka classifier map tasks are training batch classifiers all the action happens in the cleanup() method of the mapper - so there is no feedback to the task tracker (such as writing some stuff to the output for each input key/value)
 until the very end. If batch training takes too long the task tracker will kill the model building job! Increasing the timeout to something longer than the default 10 minutes can definitely help for more powerful (slower) batch learners. These Hadoop-specific
 configuration properties can be supplied on the command-line via the -user-prop option, or in the "User defined properties" table of the "Hadoop configuration" tab in the Knowledge Flow step dialogs.
Hmm. I thought I'd be able to cover everything in two instalments of this series of blog postings. Looks like I'll need a third. [To
 be continued...](http://markahall.blogspot.co.nz/2013/10/weka-and-hadoop-part-3.html)
Posted by [Mark
 Hall](http://www.blogger.com/profile/11041720517232023634)at [18:42](http://markahall.blogspot.com/2013/10/weka-and-hadoop-part-2.html)[No
 comments:](http://markahall.blogspot.com/2013/10/weka-and-hadoop-part-2.html#comment-form)
[Email
 This](http://www.blogger.com/share-post.g?blogID=7927943585854248111&postID=3204896304001388184&target=email)[BlogThis!](http://www.blogger.com/share-post.g?blogID=7927943585854248111&postID=3204896304001388184&target=blog)[Share
 to Twitter](http://www.blogger.com/share-post.g?blogID=7927943585854248111&postID=3204896304001388184&target=twitter)[Share
 to Facebook](http://www.blogger.com/share-post.g?blogID=7927943585854248111&postID=3204896304001388184&target=facebook)
### [Weka and Hadoop Part 1](http://markahall.blogspot.com/2013/10/weka-and-hadoop-part-1.html)
How to handle large datasets with Weka is a question that crops up frequently on the Weka mailing list and forums. This post is the first of three that outlines what's available, in terms of distributed processing functionality, in several new packages for
 Weka 3.7. This series of posts is continued in [part 2](http://markahall.blogspot.co.nz/2013/10/weka-and-hadoop-part-2.html) and [part
 3](http://markahall.blogspot.co.nz/2013/10/weka-and-hadoop-part-3.html).
The first new package is called [distributedWekaBase](http://weka.sourceforge.net/packageMetaData/distributedWekaBase/index.html). It provides base "map" and "reduce"
 tasks that are not tied to any specific distributed platform. The second, called [distributedWekaHadoop](http://weka.sourceforge.net/packageMetaData/distributedWekaHadoop/index.html),
 provides [Hadoop](http://hadoop.apache.org/)-specific wrappers and jobs for these base tasks. In the future there could be other wrappers - one based on the [Spark](http://spark.incubator.apache.org/) platform
 would be cool.
### Base map and reduce tasks
[distributedWekaBase](http://weka.sourceforge.net/packageMetaData/distributedWekaBase/index.html) version 1.0 provides tasks for:
- **Determining a unified ARFF header from separate data chunks in CSV format**. This is particularly important because, as Weka users know, Weka is quite particular about metadata - especially when
 it comes to nominal attributes. At the same time this task computes some handy summary statistics (that are stored as additional "meta attributes" in the header), such as count, sum, sum squared, min, max, num missing, mean, standard deviation and frequency
 counts for nominal values.  These summary statistics come in useful for some of the other tasks listed below.
- **Computing a correlation or covariance matrix**. Once the ARFF header job has been run, then computing a correlation matrix can be completed in just one pass over the data given our handy summary
 stats. The matrix produced by this job can be read by Weka's Matrix class. Map tasks compute a partial matrix of covariance sums. The reduce tasks aggregates individual rows of the matrix in order to produce the final matrix. This means that parallelism can
 be exploited in the reduce phase by using as many reducers as there are rows in the matrix.
- **Training a Weka classifier (or regressor)**. The map portion of this task can train any Weka classifier (batch or incremental) on a given data chunk and then the reduce portion will aggregate the
 individual models in various ways, depending on the type of classifier. Recently, a number of classifiers in Weka 3.7 have become *Aggregateable*. Such classifiers allow one final model, of the same type, to be produced from several separate models.
 Examples include: naive Bayes, naive Bayes multinomial, various linear regression models (learned by SGD) and Bagging. Other, non-Aggregateable, classifiers can be combined by forming a voted ensemble using Weka's Vote meta classifier. The classifier task
 also has various handy options such as allowing reservoir sampling to be used with batch learners (so that a maximum number of instances processed by the learning algorithm in a given map can be enforced), normal Weka filters to be used for pre-processing
 in each map (the task takes care of using various special subclasses of FilteredClassifier for wrapping the base classifier and filters depending on whether the base learner is Aggregateable and/or incremental), forcing batch learning for incremental learners
 (if desired), and for using a special "pre-constructed" filter (see below).
- **Evaluating a classifier or regressor**. This task handles evaluating a classifier using either the training data, a separate test set or cross-validation. Because Weka's Evaluation module is *Aggregateable*,
 and computes statistics incrementally, this is fairly straightforward. The process makes use of the classifier training task to learn an aggregated classifier in one pass over the data and then evaluation proceeds in a second pass. In the case of cross-validation,
 the classifiers for all folds are learned in one go (i.e. one aggregated classifier per fold) and then evaluated. In this case, the learning phase can make use of up to *k* reducers (one per fold). In the batch learning case, the normal process of creating
 folds (using Instances.train/testCV()) is used and the order of the instances in each map gets randomised first. In the case of incremental learning, instances are processed in a streaming fashion and a modulus operation is used to pull out the training/test
 instances corresponding to a given fold of the cross-validation.
- **Scoring using a trained classifier or regressor**. This is fairly simple and just takes uses a trained model to make predictions. No reducer is needed in this case. The task outputs input instances
 with predicted probability distributions appended. The user can specify which of the input attribute values to output along with the predictions. It also builds a mapping between the attributes in the incoming instances and those that the model is expecting,
 with missing attributes or type mismatches replaced with missing values.
- **PreconstructedPCA**. This is not a distributed task as such; instead it is a filter that can accept a correlation matrix or covariance matrix (as produced by the correlation matrix task) and produces
 a principal components analysis. The filter produces the same textual analysis output as Weka's standard PCA (in the attribute selection package) and also encapsulates the transformation for data filtering purposes. Once constructed, it can be used with the
 classifier building task.
### Hadoop wrappers and jobs
[distributedWekaHadoop](http://weka.sourceforge.net/packageMetaData/distributedWekaHadoop/index.html) version 1.0 provides a number of utilities for configuration/HDFS,
 mappers and reducers that wrap the base tasks, and jobs to orchestrate everything against Apache Hadoop 1.x (in particular, it has been developed and tested against Hadoop 1.1.2 and 1.2.1).
#### Getting datasets in and out of HDFS
The first thing this package provides is a "Loader" and "Saver" for HDFS. These can batch transfer or stream data in and out of HDFS using any base Loader or Saver - so any data format that Weka already supports can be written or read to/from HDFS. Because
 the package uses Hadoop's TextInputFormat for delivering data to mappers, we work solely with CSV files that have no header row. The CSVSaver in Weka 3.7.10 has a new option to omit the header row when writing a CSV file. The new HDFSSaver and HDFSLoader can
 be used from the command line or the Knowledge Flow GUI:
![](http://3.bp.blogspot.com/-BqUh8WUKBLk/UlItx6rNg3I/AAAAAAAAAKA/GvMypPIqI8Y/s400/HDFSSaver1.png)
#### ARFF header creation job
The first job that the distributedWekaHadoop package provides is one to create a unified ARFF header + summary statistics from the input data. All Weka Hadoop jobs have an extensive command line interface (to facilitate scripting etc.) and a corresponding
 step in the Knowledge Flow GUI. The jobs also take care of making sure that all Weka classes (and dependencies) are available to map and reduce tasks executing in Hadoop. It does this by installing the Weka jar file (and other dependencies) in HDFS and then
 adding them to the distributed cache and classpath for the job.
java weka.Run ArffHeaderHadoopJob \
-hdfs-host palladium.local -hdfs-port 9000 \
-jobtracker-host palladium.local -jobtracker-port 9001 \
-input-paths /users/mhall/input/classification \
-output-path /users/mhall/output \
-names-file $HOME/hypothyroid.names -max-split-size 100000 \
-logging-interval 5 \
-user-prop mapred.child.java.opts=-Xmx500m
![](http://3.bp.blogspot.com/-Nxt31mdQTxQ/Uk3oE-L202I/AAAAAAAAAHQ/d0yqRUSIwE0/s320/ARFFJob1.png)
![](http://1.bp.blogspot.com/-8X8Bpc_XSHM/Uk3pbi38eDI/AAAAAAAAAHg/Px2cacOce1k/s400/ARFFJob2.png)
![](http://4.bp.blogspot.com/-bZQzQ0FDqso/Uk3p-HjgHtI/AAAAAAAAAHo/YnvA-y6swJ4/s640/ARFFJob3.png)
![](http://3.bp.blogspot.com/-3y0jtHZWBlM/Uk4BifQc_kI/AAAAAAAAAIE/4U410Pd1Jc0/s640/ARFFJob4.png)
The job has options for specifying Hadoop connection details and input/output paths. It also allows control over the number of map tasks that actually get executed via the max-split-size option (this sets dfs.block.size)
 as Hadoop's default of 64Mb may not be appropriate for batch learning tasks, depending on data characteristics. The classifier job, covered in the [next
 instalment](http://markahall.blogspot.co.nz/2013/10/weka-and-hadoop-part-2.html)of this series, has a pre-processing option to create a set of randomly shuffled input data chunks, which gives greater control over the number and size of the data sets processed by the mappers. The ARFF header job also has a set of options
 for controlling how the CSV input file gets parsed and processed. It is possible to specify attribute (column) names directly or have them read from a "names" file (one attribute name per line; not to be confused with the C4.5 ".names" file format) stored
 on the local file system or in HDFS. 
As other Weka Hadoop jobs use the ARFF job internally, and it is not necessary to repeat it for subsequent jobs that process the same data set, it is possible to prevent the job from executing by providing a path to
 an existing ARFF header (in or out of HDFS) to use. 
The image below shows what the job produces for the UCI hypothyroid dataset. Given the configuration for this job shown above, the header gets stored as /users/mhall/output/arff/hypothyroid.arff in HDFS. It also gets displayed by the TextViewer in the Knowledge
 Flow. "Class" is the last of the actual data attributes and the ones that occur after that are the summary meta attributes that correspond to each of the nominal or numeric attributes in the data.
![](http://4.bp.blogspot.com/-O-du1ez7szM/Uk4ChWLydfI/AAAAAAAAAIQ/9FbIrRXKBzQ/s640/ARFFJob5.png)
This ends the first part of our coverage of the new distributed Weka functionality. In [part two](http://markahall.blogspot.co.nz/2013/10/weka-and-hadoop-part-2.html) I'll
 cover the remaining Hadoop jobs for learning and evaluating classifiers and performing a correlation analysis.
### [Learning linear models in Hadoop with Weka and Pentaho](http://markahall.blogspot.com/2013/03/learning-linear-models-in-hadoop-with.html)
The ever increasing size of recorded data and the recent rise in popularity of distributed processing systems such as Hadoop fuel the need for machine learning algorithms that
 can take advantage of the later in order to tackle the former. In many cases it is possible to carefully extract a subsample from a large data set and process it using a sequential algorithm on a single machine. However, there is evidence that, in some cases,
 even highly biased methods (such as linear regression) benefit from seeing all the data, and significantly outperform training on a carefully chosen subsample [1].
While certain supervised learning algorithms lend themselves naturally to parallel implementations - for example, ensemble methods such as bagging and random forests - others
 remain open research questions. A recent paper on the parallel learning of linear models [2] looked interesting because 1. the basic idea is super simple, 2. it uses stochastic gradient descent (SGD) and requires no communication between machines until the
 end, which is suited to Hadoop's single-pass map-reduce framework, and 3. just about all the necessary bits and pieces to implement it are already available in Weka and Pentaho. Weka already has an implementation of stochastic gradient descent for learning
 least squares linear models, so there was no need to consider distributed matrix-based solutions (furthermore, matrix-based solutions don't scale in the number of features, and large feature spaces is one situation where seeing all the data outperforms subsampling).
 The basic idea is:
- Split the data up into chunks
- Apply SGD to learn a linear model for each chunk (one pass)
- Take the weights (and bias) from each individual model and simply average them into an aggregated model
- Take the aggregated model from step 3 and use it as the starting point for step 2
- Repeat steps 1 - 4 until convergence (or happy with the error)
Bits that were already available in Weka and Pentaho:
- SGD and SGD for text classification implemented in Weka
- Pentaho Map Reduce for graphically designing map and reduce processes as Pentaho Data Integration (PDI) ETL processes
- Weka plugins for PDI:
- Knowledge Flow plugin for executing a data mining process
Bits that were missing:
- The ability for the PDI Knowledge Flow plugin to listen for a "classifier" event and output a serialized Weka classifier in an outgoing PDI data field
- A way of aggregating Weka SGD models
The first was easily rectified with a few extra lines of code in the Knowledge Flow plugin step; the second required a new "Weka Model Aggregator" step for PDI.
#### Test scenario
I quickly transfered my trusty sentiment analysis Twitter data set into HDFS as the basis for testing because no data preprocessing was needed and everything was good to go with the standard Hadoop TextFileInputFormat. The following diagram shows the basic
 map-reduce setup:
![](http://3.bp.blogspot.com/-dz5HBr6Ek4I/UTL4k4BM7fI/AAAAAAAAADo/C6Jn0-MjwzY/s640/MR.png)
To facilitate multiple iterations over the data, Hadoop's efficient distributed cache mechanism is used copy the aggregated model from iteration *i* out to the local filesystem of each node - ready for the mappers at iteration *i* +
 1 to load and continue training. Because stuff in the distributed cache is stored on the local filesystem of each node it meant that Weka's Knowledge Flow did not need to be made aware of HDFS.
#### Implementation
To learn the SGD logistic regression models at each mapper I essentially used the same Knowledge Flow layout as from the sentiment analysis with Weka post - the main difference is that the ArffLoader step has been replaced with the plugin
 "KettleInject" step that allows rows from Kettle (PDI) to be converted into Weka Instances.
![](http://3.bp.blogspot.com/-wSmKbXn5-YY/UTMA4klfQCI/AAAAAAAAAD4/ijExGDtBpII/s640/MR2.png)
![](http://4.bp.blogspot.com/-swTjfJ8dE80/UTMCb6dfAyI/AAAAAAAAAEE/GAYtDjDjDLg/s640/MR3.png)
The PDI transformation used to implement the mappers makes use of the above Knowledge Flow layout and looks like:
![](http://2.bp.blogspot.com/-76t7GpcUe6w/UTOyqXWhfVI/AAAAAAAAAEY/nMZFNhklCyw/s640/MR4.png)
The "Add constants" step is used to produce a constant key value. This ensures that all models produced by the mappers go to the same reducer instance.
The transformation used to implement the reducer uses the new "Weka Model Aggregator" PDI step and looks like:
![](http://2.bp.blogspot.com/-kCdm39UvaJI/UTO0PwRAF8I/AAAAAAAAAEg/CzTu222kc58/s640/MR5.png)
Finally, we have a PDI job that ties it all together using the "Pentaho MapReduce" job entry:
![](http://1.bp.blogspot.com/-dorQyRMG6iw/UTO12lQXwOI/AAAAAAAAAEo/lHVKvHlkNkg/s640/MR6.png)
There are two paths that are executed in the job graph. The first (failure path) gets executed on the first iteration only. This path is triggered by the failure of the "Hadoop Copy Files" step to copy the aggregated model file from a "staging" directory to
 a "deploy" directory in HDFS. There is no aggregated model file for the first iteration of training so we don't want to use the distributed cache mechanism in this case (and, in fact, it packs a sad if you specify a non-existent file via the "mapred.cache.files"
 property). The second (non-failure path) is executed for subsequent iterations, and makes sure that the aggregated model file is pushed out to nodes via the distributed cache. The whole job is repeated for as many iterations as desired.
So, that's about it. It doesn't quite implement Algorithm 2 from the paper as the data is not randomly shuffled and that algorithm uses an overcomplete solution - if there are k nodes then typically more than 1/kth of the data is processed at each node (i.e
 sampling with replacement). In practical terms it means that more iterations (or more data) are needed via the parallel approach than the single-machine sequential approach to reach the same error. Whether the parallel approach saves time over sequential approach
 depends on the amount of data and time available to learn the model. We might be able to achieve five iterations over the data in the parallel approach vs. one in the sequential approach in the time available.
The Weka Model Aggregator PDI step is still under development. So far it can aggregate SGD and SGDText Weka models produced in multiple passes over the data via multiple map-reduce jobs. It can also handle any other Weka batch or incremental learner to produce
 a voted ensemble (via the Vote meta classifier) in a single pass/map-reduce job. The ability to aggregate multiple naive Bayes models into a single model in one pass has yet to be added.
#### References
[1] [Alekh Agarwal, Olivier Chapelle, Miroslav Dudík, John Langford: A Reliable
 Effective Terascale Linear Learning System. CoRR abs/1110.4198 (2011)](http://arxiv.org/pdf/1110.4198.pdf)
[2][M.
 Zinkevich, M. Weimer, A. Smola, and L. Li, "Parallelized Stochastic Gradient Descent," in Advances
 in Neural Information Processing Systems 23, 2010, pp. 2595-2603.](http://www.research.rutgers.edu/~lihong/pub/Zinkevich11Parallelized.pdf)
