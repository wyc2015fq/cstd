# (转载)Nutch 2.0 之 抓取流程简单分析 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年04月06日 15:20:22[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2093
个人分类：[Big data](https://blog.csdn.net/fjssharpsword/article/category/6309934)








Nutch 2.0 抓取流程介绍
---------------------



## 1. 整体流程

InjectorJob => GeneratorJob => FetcherJob => ParserJob => DbUpdaterJob => SolrIndexerJob


InjectorJob : 从文件中得到一批种子网页，把它们放到抓取数据库中去
GeneratorJob: 从抓取数据库中产生要抓取的页面放到抓取队列中去
FetcherJob:   对抓取队列中的网页进行抓取,在reducer中使用了生产/消费者模型
ParserJob:    对抓取完成的网页进行解析，产生一些新的链接与网页内容的解析结果
DbUpdaterJob: 把新产生的链接更新到抓取数据库中去
SolrIndexerJob: 对解析后的内容进行索引建立



## 2. InjectorJob分析


  下面是InjectorJob的启动函数，代码如下




**[java]**[view
 plain](http://blog.csdn.net/amuseme_lu/article/details/7777426#)[copy](http://blog.csdn.net/amuseme_lu/article/details/7777426#)



- public Map<String,Object> run(Map<String,Object> args) throws Exception {  
-    getConf().setLong("injector.current.time", System.currentTimeMillis());  
-    Path input;  
-    Object path = args.get(Nutch.ARG_SEEDDIR);  
- if (path instanceof Path) {  
-      input = (Path)path;  
-    } else {  
-      input = new Path(path.toString());  
-    }  
-    numJobs = 2;  
-    currentJobNum = 0;  
-    status.put(Nutch.STAT_PHASE, "convert input");  
-    currentJob = new NutchJob(getConf(), "inject-p1 " + input);  
-    FileInputFormat.addInputPath(currentJob, input);  
- // mapper方法，从文件中解析出url，写入数据库
-    currentJob.setMapperClass(UrlMapper.class);  
-    currentJob.setMapOutputKeyClass(String.class);  
- // map 的输出为WebPage，它是用Gora compile生成的，可以通过Gora把它映射到不同的数据库中，
-    currentJob.setMapOutputValueClass(WebPage.class);  
- // 输出到GoraOutputFormat
-    currentJob.setOutputFormatClass(GoraOutputFormat.class);  
-    DataStore<String, WebPage> store = StorageUtils.createWebStore(currentJob.getConfiguration(),  
-        String.class, WebPage.class);  
-    GoraOutputFormat.setOutput(currentJob, store, true);  
-    currentJob.setReducerClass(Reducer.class);  
-    currentJob.setNumReduceTasks(0);  
-    currentJob.waitForCompletion(true);  
-    ToolUtil.recordJobStatus(null, currentJob, results);  
-    currentJob = null;  
- 
- 
-    status.put(Nutch.STAT_PHASE, "merge input with db");  
-    status.put(Nutch.STAT_PROGRESS, 0.5f);  
-    currentJobNum = 1;  
-    currentJob = new NutchJob(getConf(), "inject-p2 " + input);  
-    StorageUtils.initMapperJob(currentJob, FIELDS, String.class,  
-        WebPage.class, InjectorMapper.class);  
-    currentJob.setNumReduceTasks(0);  
-    ToolUtil.recordJobStatus(null, currentJob, results);  
-    status.put(Nutch.STAT_PROGRESS, 1.0f);  
- return results;  
-  }  



   因为InjectorJob扩展自NutchTool，实现了它的run方法。


   我们可以看到，这里有两个MR任务，第一个主要是从文件中读入种子网页，写到DataStore数据库中，第二个MR任务主要是对数据库中的WebPage对象做一个分数与抓取间隔的设置。它使用到一个initMapperJob方法，代码如下




**[java]**[view
 plain](http://blog.csdn.net/amuseme_lu/article/details/7777426#)[copy](http://blog.csdn.net/amuseme_lu/article/details/7777426#)



- publicstatic <K, V> void initMapperJob(Job job,  
-     Collection<WebPage.Field> fields,  
-     Class<K> outKeyClass, Class<V> outValueClass,  
-     Class<? extends GoraMapper<String, WebPage, K, V>> mapperClass,  
-     Class<? extends Partitioner<K, V>> partitionerClass, boolean reuseObjects)  
- throws ClassNotFoundException, IOException {  
- // 这里是生成一个DataStore的抽象，这里的DataStore用户可以不同的模块，如Hbase，MySql等
-   DataStore<String, WebPage> store = createWebStore(job.getConfiguration(),  
-       String.class, WebPage.class);  
- if (store==null) thrownew RuntimeException("Could not create datastore");  
-   Query<String, WebPage> query = store.newQuery();  
-   query.setFields(toStringArray(fields));  
-   GoraMapper.initMapperJob(job, query, store,  
-       outKeyClass, outValueClass, mapperClass, partitionerClass, reuseObjects);  
-   GoraOutputFormat.setOutput(job, store, true);  
- }  




## 3. GeneratorJob 源代码分析


   下面是GeneratorJob的run方法代码




**[java]**[view
 plain](http://blog.csdn.net/amuseme_lu/article/details/7777426#)[copy](http://blog.csdn.net/amuseme_lu/article/details/7777426#)



- public Map<String,Object> run(Map<String,Object> args) throws Exception {  
- // map to inverted subset due for fetch, sort by score
-    Long topN = (Long)args.get(Nutch.ARG_TOPN);  
-    Long curTime = (Long)args.get(Nutch.ARG_CURTIME);  
- if (curTime == null) {  
-      curTime = System.currentTimeMillis();  
-    }  
-    Boolean filter = (Boolean)args.get(Nutch.ARG_FILTER);  
-    Boolean norm = (Boolean)args.get(Nutch.ARG_NORMALIZE);  
- // map to inverted subset due for fetch, sort by score
-    getConf().setLong(GENERATOR_CUR_TIME, curTime);  
- if (topN != null)  
-      getConf().setLong(GENERATOR_TOP_N, topN);  
- if (filter != null)  
-      getConf().setBoolean(GENERATOR_FILTER, filter);  
- int randomSeed = Math.abs(new Random().nextInt());  
-    batchId = (curTime / 1000) + "-" + randomSeed;  
-    getConf().setInt(GENERATOR_RANDOM_SEED, randomSeed);  
-    getConf().set(BATCH_ID, batchId);  
-    getConf().setLong(Nutch.GENERATE_TIME_KEY, System.currentTimeMillis());  
- if (norm != null)  
-      getConf().setBoolean(GENERATOR_NORMALISE, norm);  
-    String mode = getConf().get(GENERATOR_COUNT_MODE, GENERATOR_COUNT_VALUE_HOST);  
- if (GENERATOR_COUNT_VALUE_HOST.equalsIgnoreCase(mode)) {  
-      getConf().set(URLPartitioner.PARTITION_MODE_KEY, URLPartitioner.PARTITION_MODE_HOST);  
-    } elseif (GENERATOR_COUNT_VALUE_DOMAIN.equalsIgnoreCase(mode)) {  
-        getConf().set(URLPartitioner.PARTITION_MODE_KEY, URLPartitioner.PARTITION_MODE_DOMAIN);  
-    } else {  
-      LOG.warn("Unknown generator.max.count mode '" + mode + "', using mode=" + GENERATOR_COUNT_VALUE_HOST);  
-      getConf().set(GENERATOR_COUNT_MODE, GENERATOR_COUNT_VALUE_HOST);  
-      getConf().set(URLPartitioner.PARTITION_MODE_KEY, URLPartitioner.PARTITION_MODE_HOST);  
-    }  
- 
- 
- // 上面是设置一些要使用要的常量
-    numJobs = 1;  
-    currentJobNum = 0;  
- // 生成一个job
-    currentJob = new NutchJob(getConf(), "generate: " + batchId);  
- // 初始化Map,这里的Map的输出类型为<SelectorEntry,WebPage>, 使用 SelectorEntryPartitioner来进行切分
-    StorageUtils.initMapperJob(currentJob, FIELDS, SelectorEntry.class,  
-        WebPage.class, GeneratorMapper.class, SelectorEntryPartitioner.class, true);  
- // 初始化Reducer, 使用了generatorReducer来进行聚合处理
-    StorageUtils.initReducerJob(currentJob, GeneratorReducer.class);  
-    currentJob.waitForCompletion(true);  
-    ToolUtil.recordJobStatus(null, currentJob, results);  
-    results.put(BATCH_ID, batchId);  
- return results;  
-  }  
- 



  好像比原来的Generate简单很多，这里的GeneratorMapper完成的工作与之前的版本是一样的，如url的正规化，过滤，分数的设置，而GeneratorReducer完成的工作也和之前差不多，只是输出变成了DataStore，如HBase，完成以后会每个WebPage进行打标记，表示当前WebPage所完成的一个状态。





## 4. FetcherJob 源代码分析


   使用了Gora的 fetcher比原来简单了很多，下面是其run的源代码




**[java]**[view
 plain](http://blog.csdn.net/amuseme_lu/article/details/7777426#)[copy](http://blog.csdn.net/amuseme_lu/article/details/7777426#)



- public Map<String,Object> run(Map<String,Object> args) throws Exception {  
-    checkConfiguration();  
-    String batchId = (String)args.get(Nutch.ARG_BATCH);  
-    Integer threads = (Integer)args.get(Nutch.ARG_THREADS);  
-    Boolean shouldResume = (Boolean)args.get(Nutch.ARG_RESUME);  
-    Integer numTasks = (Integer)args.get(Nutch.ARG_NUMTASKS);  
- 
- if (threads != null && threads > 0) {  
-      getConf().setInt(THREADS_KEY, threads);  
-    }  
- if (batchId == null) {  
-      batchId = Nutch.ALL_BATCH_ID_STR;  
-    }  
-    getConf().set(GeneratorJob.BATCH_ID, batchId);  
- if (shouldResume != null) {  
-      getConf().setBoolean(RESUME_KEY, shouldResume);  
-    }  
- 
-    LOG.info("FetcherJob : timelimit set for : " + getConf().getLong("fetcher.timelimit", -1));  
-    LOG.info("FetcherJob: threads: " + getConf().getInt(THREADS_KEY, 10));  
-    LOG.info("FetcherJob: parsing: " + getConf().getBoolean(PARSE_KEY, false));  
-    LOG.info("FetcherJob: resuming: " + getConf().getBoolean(RESUME_KEY, false));  
- 
- 
- // set the actual time for the timelimit relative
- // to the beginning of the whole job and not of a specific task
- // otherwise it keeps trying again if a task fails
- long timelimit = getConf().getLong("fetcher.timelimit.mins", -1);  
- if (timelimit != -1) {  
-      timelimit = System.currentTimeMillis() + (timelimit * 60 * 1000);  
-      getConf().setLong("fetcher.timelimit", timelimit);  
-    }  
-    numJobs = 1;  
-    currentJob = new NutchJob(getConf(), "fetch");  
- // 得到它过滤的字段
-    Collection<WebPage.Field> fields = getFields(currentJob);  
- // 初始化mapper, 其输出为<IntWritable,FetchEntry>
- // 在mapper中输入数据进行过滤，主要是对不是同一个batch与已经fetch的数据进行过滤
-    StorageUtils.initMapperJob(currentJob, fields, IntWritable.class,  
-        FetchEntry.class, FetcherMapper.class, FetchEntryPartitioner.class, false);  
- // 初始化reducer
-    StorageUtils.initReducerJob(currentJob, FetcherReducer.class);  
- if (numTasks == null || numTasks < 1) {  
-      currentJob.setNumReduceTasks(currentJob.getConfiguration().getInt("mapred.map.tasks",  
-          currentJob.getNumReduceTasks()));  
-    } else {  
-      currentJob.setNumReduceTasks(numTasks);  
-    }  
-    currentJob.waitForCompletion(true);  
-    ToolUtil.recordJobStatus(null, currentJob, results);  
- return results;  
-  }  



  这里把原来在Mapper中使用到的生产者与消费者模型用到了reducer中，重写了reducer的run方法，在其中打开多个抓取线程，对url进行多线程抓取，有兴趣可以看一下FetcherReducer这个类。





## 5. ParserJob 代码分析


   下面是ParserJob.java中的run代码




**[java]**[view
 plain](http://blog.csdn.net/amuseme_lu/article/details/7777426#)[copy](http://blog.csdn.net/amuseme_lu/article/details/7777426#)



- @Override
- public Map<String,Object> run(Map<String,Object> args) throws Exception {  
-    String batchId = (String)args.get(Nutch.ARG_BATCH);  
-    Boolean shouldResume = (Boolean)args.get(Nutch.ARG_RESUME);  
-    Boolean force = (Boolean)args.get(Nutch.ARG_FORCE);  
- 
- if (batchId != null) {  
-      getConf().set(GeneratorJob.BATCH_ID, batchId);  
-    }  
- if (shouldResume != null) {  
-      getConf().setBoolean(RESUME_KEY, shouldResume);  
-    }  
- if (force != null) {  
-      getConf().setBoolean(FORCE_KEY, force);  
-    }  
-    LOG.info("ParserJob: resuming:\t" + getConf().getBoolean(RESUME_KEY, false));  
-    LOG.info("ParserJob: forced reparse:\t" + getConf().getBoolean(FORCE_KEY, false));  
- if (batchId == null || batchId.equals(Nutch.ALL_BATCH_ID_STR)) {  
-      LOG.info("ParserJob: parsing all");  
-    } else {  
-      LOG.info("ParserJob: batchId:\t" + batchId);  
-    }  
-    currentJob = new NutchJob(getConf(), "parse");  
- 
-    Collection<WebPage.Field> fields = getFields(currentJob);  
- // 初始化mapper，输出类型为<String,WebPage>, 解析全部在maper完成
-    StorageUtils.initMapperJob(currentJob, fields, String.class, WebPage.class,  
-        ParserMapper.class);  
- // 初始化reducer,这里是支持把<key,values>写到数据库中
-    StorageUtils.initReducerJob(currentJob, IdentityPageReducer.class);  
-    currentJob.setNumReduceTasks(0);  
- 
- 
-    currentJob.waitForCompletion(true);  
-    ToolUtil.recordJobStatus(null, currentJob, results);  
- return results;  
-  }  





## 6. DbUpdaterJob 代码分析

下面是DbUpdaterjob的run方法代码




**[java]**[view
 plain](http://blog.csdn.net/amuseme_lu/article/details/7777426#)[copy](http://blog.csdn.net/amuseme_lu/article/details/7777426#)



- public Map<String,Object> run(Map<String,Object> args) throws Exception {  
-    String crawlId = (String)args.get(Nutch.ARG_CRAWL);  
-    numJobs = 1;  
-    currentJobNum = 0;  
-    currentJob = new NutchJob(getConf(), "update-table");  
- if (crawlId != null) {  
-      currentJob.getConfiguration().set(Nutch.CRAWL_ID_KEY, crawlId);  
-    }  
- //job.setBoolean(ALL, updateAll);
-    ScoringFilters scoringFilters = new ScoringFilters(getConf());  
-    HashSet<WebPage.Field> fields = new HashSet<WebPage.Field>(FIELDS);  
-    fields.addAll(scoringFilters.getFields());  
- 
- // Partition by {url}, sort by {url,score} and group by {url}.
- // This ensures that the inlinks are sorted by score when they enter
- // the reducer.
- 
-    currentJob.setPartitionerClass(UrlOnlyPartitioner.class);  
-    currentJob.setSortComparatorClass(UrlScoreComparator.class);  
-    currentJob.setGroupingComparatorClass(UrlOnlyComparator.class);  
- 
- // 这里的maper读取webpage中的outlinks字段值，对每个外链接计算分数
-    StorageUtils.initMapperJob(currentJob, fields, UrlWithScore.class,  
-        NutchWritable.class, DbUpdateMapper.class);  
- // 对新生成的外链接设置一些分数，状态等信息，再把新的WebPage写回数据库
-    StorageUtils.initReducerJob(currentJob, DbUpdateReducer.class);  
-    currentJob.waitForCompletion(true);  
-    ToolUtil.recordJobStatus(null, currentJob, results);  
- return results;  
-  }  





## 7. SolrIndexerJob 代码分析

下面是其run方法的源代码




**[java]**[view
 plain](http://blog.csdn.net/amuseme_lu/article/details/7777426#)[copy](http://blog.csdn.net/amuseme_lu/article/details/7777426#)



- @Override
- public Map<String,Object> run(Map<String,Object> args) throws Exception {  
-     String solrUrl = (String)args.get(Nutch.ARG_SOLR);  
-     String batchId = (String)args.get(Nutch.ARG_BATCH);  
-     NutchIndexWriterFactory.addClassToConf(getConf(), SolrWriter.class);  
-     getConf().set(SolrConstants.SERVER_URL, solrUrl);  
- 
- 
- // 初始化 job
-     currentJob = createIndexJob(getConf(), "solr-index", batchId);  
-     Path tmp = new Path("tmp_" + System.currentTimeMillis() + "-"
-                 + new Random().nextInt());  
- // 设置输出索引到文件，输出格式使用IndexeroutputFormat, 其默认调用Solr的API把数据传给Solr建立索引
-     FileOutputFormat.setOutputPath(currentJob, tmp);  
-     currentJob.waitForCompletion(true);  
-     ToolUtil.recordJobStatus(null, currentJob, results);  
- return results;  
-   }  




有兴趣可以看一下SolrWriter，它实现了NutchIndexerWriter这个接口，来把数据写到不同的后台搜索引擎中，这里默认使用了Solr，当然你也可以通过实现它来扩展你自己的搜索引擎，当然nutch还提供了插件来自定义索引的字段值,也就是IndexingFilter.java这个接口。



## 8. 总结


    Nutch 2.0个人感觉现在还是不成熟的，有很多功能还没有完成，主要的改变还是在它的数据存储层，把原来的数据存储进行了抽象，使其可以更好的运行在大规模数据抓取中，而且可以让用户来扩展具体的数据存储。当然数据存储层的变化带来了一些流程上的变化，有一些操作可以支持使用数据库操作来完成，这也大大减少了一些原来要MR任务来完成的代码。总之nutch 2.0  还是让我们看到了nutch的一个发展方向。希望它发现的越来越好吧。





转载地址：http://blog.csdn.net/amuseme_lu/article/details/7777426



