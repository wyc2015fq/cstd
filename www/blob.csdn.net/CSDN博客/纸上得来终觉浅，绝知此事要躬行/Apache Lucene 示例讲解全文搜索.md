# Apache Lucene 示例讲解全文搜索 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年04月05日 11:22:08[boonya](https://me.csdn.net/boonya)阅读数：1227








文章出处：[http://blog.csdn.net/xiaoyao880609/article/details/51011652](http://blog.csdn.net/xiaoyao880609/article/details/51011652)

Lucene是apache软件基金会4 jakarta项目组的一个子项目，是一个开放源代码的全文检索引擎工具包，但它不是一个完整的全文检索引擎，而是一个全文检索引擎的架构，提供了完整的查询引擎和索引引擎，部分文本分析引擎。Lucene的目的是为软件开发人员提供一个简单易用的工具包，以方便的在目标系统中实现全文检索的功能，或者是以此为基础建立起完整的全文检索引擎。(这里博主只是在系统中实现全文检索的功能)


Lucene是一套用于全文检索和搜寻的开源程式库，由Apache软件基金会支持和提供。Lucene提供了一个简单却强大的应用程式接口，能够做全文索引和搜寻。在Java开发环境里Lucene是一个成熟的免费开源工具。就其本身而言，Lucene是当前以及最近几年最受欢迎的免费Java信息检索程序库。人们经常提到信息检索程序库，虽然与搜索引擎有关，但不应该将信息检索程序库与搜索引擎相混淆。


![](https://img-blog.csdn.net/20160330105624187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**（参考图）**

首先需要导入lucene相关包 maven如下(博主用的是当初最新版本，因为新版本对索引库进行了优化，可以相对来讲减少磁盘的开销。)


```
<span style="white-space:pre">    </span>   <!-- Lucene -->  
    <span style="white-space:pre">    </span>   <dependency>  
                <groupId>org.apache.lucene</groupId>  
                <artifactId>lucene-core</artifactId>  
                <version>4.9.0</version>  
            </dependency>  
            <dependency>  
                <groupId>org.apache.lucene</groupId>  
                <artifactId>lucene-queryparser</artifactId>  
                <version>4.9.0</version>  
            </dependency>  
            <dependency>  
                <groupId>org.apache.lucene</groupId>  
                <artifactId>lucene-highlighter</artifactId>  
                <version>4.9.0</version>  
            </dependency>  
            <dependency>  
                <groupId>org.apache.lucene</groupId>  
                <artifactId>lucene-analyzers-common</artifactId>  
                <version>4.9.0</version>  
            </dependency>  
            <dependency>  
                <groupId>org.apache.lucene</groupId>  
                <artifactId>lucene-queryparser</artifactId>  
                <version>4.9.0</version>  
            </dependency>
```




首先需要指定lucene的索引库路径(推荐将Lucene索引库路径配置在配置文件中)


```
# lucene index path  
batch.lucene.index.path=/home1/irteam/owfs/lucene_index
```



创建索引时候需要将数据库中取出的javabean对象转换为lucene的document对象，因此我做了工具类将基本的lucene相关方法封装好服用。

在这里就把关键代码贴出来提供参考


```java
@Service  
    public class IndexUtil {  
      
        @Value("${batch.lucene.index.path}")  
        private String indexPath;
```

`    <span style="white-space:pre">    </span>//将java对象转换为lucene元素  `

```java
public Document toDocument(IndexMstr index, Document oldDoc) {  
        Document doc = new Document();  
        if (StringUtils.isNotBlank(index.getTaskId())) {//对应字符串类型(常用)  
            doc.add(new StringField("taskId", index.getTaskId(), Store.YES));  
        } else if (StringUtils.isNotBlank(oldDoc.get("taskId"))) {  
            doc.add(new StringField("taskId", oldDoc.get("taskId"), Store.YES));  
        }  
        if (StringUtils.isNotBlank(index.getSummary())) {//对应text类型  
            doc.add(new TextField("summary", index.getSummary(), Store.YES));  
        } else if (StringUtils.isNotBlank(oldDoc.get("summary"))) {  
            doc.add(new TextField("summary", oldDoc.get("summary"), Store.YES));  
        }  
        if (index.getRegistDate() != null) {//对应date类型，由于lucene不支持date所以我保存了时间戳，显示时候需要转换  
            doc.add(new LongField("registDate", index.getRegistDate().getTime(), Store.YES));  
        } else if (StringUtils.isNotBlank(oldDoc.get("registDate"))) {  
            doc.add(new LongField("registDate", new Long(oldDoc.get("registDate")), Store.YES));  
        }  
        if (StringUtils.isNotBlank(index.getSyncIndexType())) {//由于lucene不支持一对多等关系，但是项目需要多表中对应字段作为检索条件，所以我将对应多数据用空格分隔保存(注：由于该字段只需要作为检索条件所以为了省事这样处理，如需要检索可以将LIST转为json存储)  
            if (StringUtils.isNotBlank(index.getAnalysisNmArr())) {  
                doc.add(new StringField("analysisNmArr", index.getAnalysisNmArr(), Store.YES));  
            }  
            if (StringUtils.isNotBlank(index.getAnalysisConArr())) {  
                doc.add(new StringField("analysisConArr", index.getAnalysisConArr(), Store.YES));  
            }  
        } else {  
            List<AnalysisIndexMstr> analysisIndexList = index.getAnalysisIndexList();  
            if (CollectionUtils.isNotEmpty(analysisIndexList)) {//심화분석 analysisNmArr,analysisConArr  
                StringBuffer analysisNmSB = new StringBuffer();  
                StringBuffer analysisConSB = new StringBuffer();  
                for (AnalysisIndexMstr analysisIndex : analysisIndexList) {  
                    if (analysisIndex != null) {  
                        if (StringUtils.isNotBlank(analysisIndex.getAnalysisNm())) {  
                            analysisNmSB.append(SOCUtil.html2Text(analysisIndex.getAnalysisNm().trim()) + " ");  
                        }  
                        if (StringUtils.isNotBlank(analysisIndex.getAnalysisCon())) {  
                            analysisConSB.append(SOCUtil.html2Text(analysisIndex.getAnalysisCon()) + " ");  
                        }  
                    }  
                }  
                if (StringUtils.isNotBlank(analysisNmSB.toString())) {  
                    doc.add(new StringField("analysisNmArr", SOCUtil.html2Text(analysisNmSB.toString().trim()), Store.YES));  
                }  
                if (StringUtils.isNotBlank(analysisConSB.toString())) {  
                    doc.add(new TextField("analysisConArr", SOCUtil.html2Text(analysisConSB.toString().trim()), Store.YES));  
                }  
            }  
        }  
        return doc;  
    }
```


```java
//批量将java对象转换为lucene元素  
        public List<Document> toDocuments(List<IndexMstr> indexMstrList) {  
            List<Document> docList = new ArrayList<Document>();  
            for (IndexMstr index : indexMstrList) {  
                docList.add(toDocument(index, new Document()));  
            }  
            return docList;  
        }
```


```java
//lucene分页查询，并且批量将lucene元素转换为java对象  
        public List<IndexMstr> toObjectByPage(IndexSearcher indexSearcher, TopFieldDocs topDocs, PagerInfo pagerInfo, IndexParam indexParam) throws IOException {  
            if (StringUtils.isEmpty(indexParam.getPage())) {  
                indexParam.setPage(SocConstant.PAGE_ONE);  
            }  
            if (StringUtils.isEmpty(indexParam.getPageSize())) {  
                indexParam.setPageSize(String.valueOf(SocConstant.PAGE_SIZE));  
            }  
            SimpleDateFormat sdf = new SimpleDateFormat(DateUtil.DATE_FULL_PATTERN_DB_KOREAN);  
            List<IndexMstr> indexMstrList = new ArrayList<IndexMstr>();  
            ScoreDoc[] scoreDocs = topDocs.scoreDocs;  
            pagerInfo.setPage(Integer.parseInt(indexParam.getPage()));  
            pagerInfo.init(SocConstant.PAGE_AJAX_DEFAULT_TYPE, scoreDocs.length,  
                Integer.parseInt(indexParam.getPageSize()),  
                SocConstant.INDEX_SIZE);  
            indexParam.setStart(pagerInfo.getStartRownum() - 1);  
            indexParam.setEnd(pagerInfo.getEndRownum());  
            for(int i = indexParam.getStart(); i < indexParam.getEnd(); i++) {  
                int docId = scoreDocs[i].doc;  
                Document doc = indexSearcher.doc(docId);  
                IndexMstr indexMstr = new IndexMstr();  
                indexMstr.setTaskId(doc.get("taskId"));  
                indexMstr.setTicketId(doc.get("ticketId"));  
                //use highlighter for ticketNm  
                String ticketNm = StringUtils.isBlank(indexParam.getSearch()) ? doc.get("ticketNm") : toHighlighter(indexParam.getSearch(), doc.get("ticketNm"));  
                indexMstr.setTicketNm(ticketNm);  
                //use highlighter for taskNm  
                String taskNm = StringUtils.isBlank(indexParam.getSearch()) ? doc.get("taskNm") : toHighlighter(indexParam.getSearch(), doc.get("taskNm"));  
                indexMstr.setTaskNm(taskNm);  
                indexMstr.setEmpNm(doc.get("empNm"));  
                indexMstr.setCorpNm(doc.get("corpNm"));  
                indexMstr.setTaskIndexType(TaskIndexType.instanceOf(doc.get("taskTypeId")));  
                if (StringUtils.isNotBlank(doc.get("registDate"))) {  
                    indexMstr.setRegistDateStr(sdf.format(new Date(Long.parseLong(doc.get("registDate")))));  
                }  
                indexMstrList.add(indexMstr);  
            }  
            return indexMstrList;  
        }  
    <span style="white-space:pre">    </span>//实现高亮(由于lucene高亮是作用于分词后的结果，项目中需要将整体匹配高亮所以自己实现了，如果有兴趣看源码你会发现，源码也是类似这样实现的)  
        private String toHighlighter(String search, String fieldValue) {  
            if (StringUtils.isBlank(fieldValue)) {  
                return fieldValue;  
            }  
            return fieldValue.replaceAll("(?i)" + search, "<font color='#FF0000'>" + search + "</font>");  
        }  
      
    }
```




参考图中init sync batch实现如下：

- 将积存的数据批量创建索引,由于是积存的数据，所以为了防止查询DB超时这里分批次创建索引。  


- （如果创建多个<span style="font-family: Arial, Helvetica, sans-serif;">indexWriter实例同时对索引库操作会报错，最好将IndexWriter交于spring管理</span><span style="font-family: Arial, Helvetica, sans-serif;">）</span>  



```java
@Override  
    public void execute(ExtendedMap jobDataMap) {  
        log.info("[IndexRsyncJob] create lucene index job start........");  
        File indexFile = new File(indexPath);  
        initIndexFile(indexFile);  
        CJKAnalyzer analyzer = new CJKAnalyzer(Version.LUCENE_4_9);  
        IndexWriterConfig conf = new IndexWriterConfig(Version.LUCENE_4_9, analyzer);  
        try {  
            FSDirectory directory = FSDirectory.open(indexFile);  
            IndexWriter indexWriter = new IndexWriter(directory, conf);  
            System.err.println("create lucene index start...................." + DateUtil.getFullDate());  
            taskIndexBO.createTaskIndex(TaskIndexType.TASK_INTRUSION.getCode(), 200, indexFile, indexWriter);//create intrusion task lucene index  
            taskIndexBO.createTaskIndex(TaskIndexType.TASK_INCIDENT.getCode(), 200, indexFile, indexWriter);//create incident task lucene index  
            taskIndexBO.createTaskIndex(TaskIndexType.TASK_REQUEST.getCode(), 200, indexFile, indexWriter);//create request task lucene index  
            System.err.println("create lucene index end...................." + DateUtil.getFullDate());  
        } catch (IOException e) {  
            log.error("[IndexRsyncJob] create lucene index error:" + e.getMessage());  
        } finally {  
            try {  
                indexWriter.close();  
            } catch (IOException e) {  
                log.error("[IndexRsyncJob] close lucene IndexWriter error:" + e.getMessage());  
            }  
        }  
    }  
   public void createTaskIndex(String taskTypeId, int synCnt, File indexFile, IndexWriter indexWriter) {  
        int num = 0;  
        try{  
            IndexParam indexParam = new IndexParam();  
            indexParam.setTaskTypeId(taskTypeId);  
            int taskTotal = taskIndexDAO.selectTaskCnt(indexParam);  
            for (int i = num; i < taskTotal; i += synCnt) {  
                num = i;  
                indexParam.setStart(i);  
                indexParam.setEnd(synCnt);  
                List<IndexMstr> taskIndexList = taskIndexDAO.selectTaskList(indexParam);  
                List<Document> docList = new IndexUtil().toDocuments(taskIndexList);  
                indexWriter.addDocuments(docList);  
                indexWriter.commit();  
                log.info("[task(total:" + taskTotal + ")] create end by count >>>>>>>>> " + i);  
            }  
        } catch (Exception e) {  
            log.error("[TaskIndexBO] create lucene index error:" + e.getMessage());  
            createTaskIndex(taskTypeId, synCnt, indexFile, indexWriter);  
        }  
    }
```




参考图中incremental sync batch实现如下：

索引表中的数据是系统每次对关联表操作时候将变动的数据存储临时索引表中，再通过10分钟一次的batch实现增量同步的。



```java
public void execute(ExtendedMap jobDataMap) {  
        log.info("[IndexRsyncDBJob] create lucene index job start........");  
        File indexFile = new File(indexPath);  
        indexBO.createIndex(indexFile);  
        log.info("[IndexRsyncDBJob] create lucene index job end........");  
    }  
    public void createIndex(File indexFile) {  
        List<Integer> indexIdList = new ArrayList<Integer>();  
        List<IndexMstr> indexList = indexDAO.selectIndexList();  
        CJKAnalyzer analyzer = new CJKAnalyzer(Version.LUCENE_4_9);  
        IndexWriterConfig conf = new IndexWriterConfig(Version.LUCENE_4_9, analyzer);  
        try {  
            FSDirectory directory = FSDirectory.open(indexFile);  
            indexWriter = new IndexWriter(directory, conf);  
            for (IndexMstr indexMstr : indexList) {  
                try {  
                    indexReader = DirectoryReader.open(directory);  
                    IndexSearcher indexSearcher = new IndexSearcher(indexReader);  
                    if ("A".equalsIgnoreCase(indexMstr.getSyncIndexType())) {  
                        TopDocs topDocs = indexSearcher.search(new TermQuery(new Term("taskId", indexMstr.getTaskId())), Integer.MAX_VALUE);  
                        if (topDocs.totalHits == 0) {  
                            Document doc = indexUtil.toDocument(indexMstr, new Document());  
                            indexWriter.addDocument(doc);  
                        }  
                    } else if ("D".equalsIgnoreCase(indexMstr.getSyncIndexType())) {  
                        indexWriter.deleteDocuments(new Term("taskId", indexMstr.getTaskId()));  
                    } else if ("U".equalsIgnoreCase(indexMstr.getSyncIndexType())) {  
                        TopDocs topDocs = indexSearcher.search(new TermQuery(new Term("taskId", indexMstr.getTaskId())), Integer.MAX_VALUE);  
                        if (topDocs.totalHits > 0) {  
                            Document oldDoc = indexSearcher.doc(topDocs.scoreDocs[0].doc);  
                            Document doc = indexUtil.toDocument(indexMstr, oldDoc);  
                            indexWriter.updateDocument(new Term("taskId", indexMstr.getTaskId()), doc);  
                        }  
                    }  
                    indexWriter.commit();  
                    indexIdList.add(indexMstr.getIndexId());  
                } catch (IOException e) {  
                    log.error("[IndexBO] before create lucene index for db error:" + e.getMessage());  
                } finally {  
                    try {  
                        indexReader.close();  
                    } catch (IOException e) {  
                        log.error("[IndexBO] close IndexReader error : " + e.getMessage());  
                    }  
                }  
            }  
        } catch (Exception e) {  
            log.error("[IndexBO] after create lucene index for db error:" + e.getMessage());  
        } finally {  
            try {  
                indexWriter.close();  
            } catch (IOException e) {  
                log.error("[IndexBO] close IndexWriter error : " + e.getMessage());  
            }  
        }  
        if (CollectionUtils.isNotEmpty(indexIdList)) {  
            indexDAO.deleteIndexs(indexIdList);  
        }  
    }
```




到这里就把最麻烦的创建索引和数据库和索引库同步部分做完了。

下面说一下查询部分。查询时可以定义多个query对象用Occur逻辑实现复杂查询。

而且lucene也支持使用通配符做查询条件。

**Occur.SHOULD，或的意思Occur.MUST_NOT，非的意思**
**Occur.MUST， 与的意**



```java
private void doSearch(Model model, PagerInfo pagerInfo,IndexParam indexParam, boolean isInit) {  
        File indexFile = new File(indexPath);     
        try {  
            dir = FSDirectory.open(indexFile);  
            reader= DirectoryReader.open(dir);  
            BooleanQuery query = new BooleanQuery();  
            BooleanQuery searchQuery = new BooleanQuery();  
            searchQuery.add(new WildcardQuery(new Term("ticketNm", "*" + indexParam.getSearch() + "*")), Occur.SHOULD);  
            searchQuery.add(new WildcardQuery(new Term("taskNm", "*" + indexParam.getSearch() + "*")), Occur.SHOULD);  
            searchQuery.add(new WildcardQuery(new Term("summary", "*" + indexParam.getSearch() + "*")), Occur.SHOULD);  
            searchQuery.add(new WildcardQuery(new Term("detectionLog", "*" + indexParam.getSearch() + "*")), Occur.SHOULD);  
            searchQuery.add(new WildcardQuery(new Term("analysisNmArr", "*" + indexParam.getSearch() + "*")), Occur.SHOULD);  
            searchQuery.add(new WildcardQuery(new Term("analysisConArr", "*" + indexParam.getSearch() + "*")), Occur.SHOULD);  
            query.add(searchQuery, Occur.MUST);  
            if(StringUtils.isNotEmpty(indexParam.getTaskTypeIds())) {  
                BooleanQuery taskTypeQuery = new BooleanQuery();  
                String[] taskTypeIdArr = indexParam.getTaskTypeIds().split(",");  
                for (int i = 0; i < taskTypeIdArr.length; i++) {  
                    taskTypeQuery.add(new TermQuery(new Term("taskTypeId", taskTypeIdArr[i])), Occur.SHOULD);  
                }  
                query.add(taskTypeQuery, Occur.MUST);  
            }  
            if(StringUtils.isNotEmpty(indexParam.getCorpId())) {  
                query.add(new WildcardQuery(new Term("corpId", indexParam.getCorpId())), Occur.MUST);  
            }  
            if(StringUtils.isNotEmpty(indexParam.getEmpNm())) {  
                query.add(new WildcardQuery(new Term("empNm", "*" + indexParam.getEmpNm() + "*")), Occur.MUST);  
            }  
            if(StringUtils.isNotEmpty(indexParam.getStartDt()) || StringUtils.isNotEmpty(indexParam.getEndDt())) {  
                Long startDtLong = StringUtils.isNotEmpty(indexParam.getStartDt()) ? DateUtil.stringTODate(indexParam.getStartDt(), DateUtil.DATE_FULL_PATTERN_DB_KOREAN).getTime() : null;  
                Long endDtLong = StringUtils.isNotEmpty(indexParam.getEndDt()) ? DateUtil.stringTODate(indexParam.getEndDt(), DateUtil.DATE_FULL_PATTERN_DB_KOREAN).getTime() : null;  
                  
                query.add(NumericRangeQuery.newLongRange("registDate", startDtLong, endDtLong, true, true) , Occur.MUST);  
            }  
            IndexSearcher indexSearcher = new IndexSearcher(reader);  
            TopFieldDocs topDocs = indexSearcher.search(query, 100, new Sort(new SortField("registDate", SortField.Type.LONG, true)));  
            List<IndexMstr> indexList = new IndexUtil().toObjectByPage(indexSearcher, topDocs, pagerInfo, indexParam);  
            model.addAttribute("indexList", indexList);  
            model.addAttribute("indexParam", indexParam);  
            if(isInit) {  
                model.addAttribute("page", pagerInfo.getPage());  
                initTaskCount(indexSearcher, model);  
            }  
        } catch (IOException e) {  
            log.error("[PublicSearchController] doSearch error : " + e.getMessage());  
        } finally {  
            try {  
                if(reader != null) {  
                    reader.close();  
                }  
            } catch (IOException e) {  
                log.error("[PublicSearchController] close IndexReader for doSearch error : " + e.getMessage());  
            }  
        }  
    }  
  
    private void initTaskCount(IndexSearcher indexSearcher, Model model) throws IOException {  
        TermQuery query_I = new TermQuery(new Term("taskTypeId", TaskIndexType.TASK_INTRUSION.getCode()));  
        TermQuery query_D = new TermQuery(new Term("taskTypeId", TaskIndexType.TASK_DDos.getCode()));  
        TermQuery query_A = new TermQuery(new Term("taskTypeId", TaskIndexType.TASK_INCIDENT.getCode()));  
        TermQuery query_T = new TermQuery(new Term("taskTypeId", TaskIndexType.TASK_REQUEST.getCode()));  
        TermQuery query_P = new TermQuery(new Term("taskTypeId", TaskIndexType.POLICY_DEVELOP.getCode()));  
        TermQuery query_R = new TermQuery(new Term("taskTypeId", TaskIndexType.POLICY_VENDOR.getCode()));  
        TermQuery query_M = new TermQuery(new Term("taskTypeId", TaskIndexType.POLICY_ESM.getCode()));  
        TermQuery query_N = new TermQuery(new Term("taskTypeId", TaskIndexType.POLICY_EXCEPTION.getCode()));  
        int total_I = indexSearcher.search(query_I, Integer.MAX_VALUE).totalHits;  
        model.addAttribute("total_I", total_I);  
        int total_A = indexSearcher.search(query_A, Integer.MAX_VALUE).totalHits;  
        model.addAttribute("total_A", total_A);  
        int total_T = indexSearcher.search(query_T, Integer.MAX_VALUE).totalHits;  
        model.addAttribute("total_T", total_T);  
        int total_D = indexSearcher.search(query_D, Integer.MAX_VALUE).totalHits;  
        model.addAttribute("total_D", total_D);  
        int total_P = indexSearcher.search(query_P, Integer.MAX_VALUE).totalHits;  
        model.addAttribute("total_P", total_P);  
        int total_R = indexSearcher.search(query_R, Integer.MAX_VALUE).totalHits;  
        model.addAttribute("total_R", total_R);  
        int total_M = indexSearcher.search(query_M, Integer.MAX_VALUE).totalHits;  
        model.addAttribute("total_M", total_M);  
        int total_N = indexSearcher.search(query_N, Integer.MAX_VALUE).totalHits;  
        model.addAttribute("total_N", total_N);  
    }
```





由于博主应用系统中数据量不是很大所以直接对磁盘进行检索。如果数据量大的话可以通过RAMDirectory缓存来优化创建索引，和查询功能。

博主经测试几百万级以下的数据量可以直接对磁盘进行检索，相反如果数据量不大时候用缓存反而起反效果。

如有不了解地方,或不对的地方，欢迎提出，互相探讨~



