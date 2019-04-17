# Apache Lucene /Solr历史版本介绍及下载 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月12日 15:06:08[boonya](https://me.csdn.net/boonya)阅读数：1894








原文地址：[http://lucene.apache.org/](http://lucene.apache.org/)


Apache LuceneTM项目开发开源搜索软件，包括：

- Lucene Core是我们的旗舰子项目，提供基于Java的索引和搜索技术，以及拼写检查，突出显示和高级分析/标记化功能。
- SolrTM是使用Lucene Core，XML / HTTP和JSON / Python / Ruby API构建的高性能搜索服务器，突出显示，分面搜索，缓存，复制和Web管理界面。
- PyLucene是Core项目的Python端口。



目前最新版本为6.4.1，高版本的需要的JDK为1.8以上。


# LuceneTM News

## 6 February 2017 - Apache Lucene 6.4.1 and Apache Solr 6.4.1 Available

The Lucene PMC is pleased to announce the release of Apache Lucene 6.4.1 and Apache Solr 6.4.1.

Lucene can be downloaded from [http://www.apache.org/dyn/closer.lua/lucene/java/6.4.1](http://www.apache.org/dyn/closer.lua/lucene/java/6.4.1)and Solr can be downloaded from
[http://www.apache.org/dyn/closer.lua/lucene/solr/6.4.1](http://www.apache.org/dyn/closer.lua/lucene/solr/6.4.1)

### Highlights of this Lucene release include:
- 
Javadocs now build successfully with Java 8u121

- 
Fixed memory leak in the case that TermQuery or SpanTermQuery objects that wrap a TermContext were cached

- 
Fixed native memory leak when the codec is configured with the BEST_COMPRESSION option

- 
AnalyzingInfixSuggester now only opens an IndexWriter when changes need to be applied



### Highlights of this Solr release include:
- 
"Plugin/Stats" section of the UI doesn't display empty metric types

- 
SOLR_SSL_OPTS was mistakenly overwritten in solr.cmd

- 
Better validation of filename params in ReplicationHandler

- 
Core swapping did not work with new metrics changes in place

- 
Admin UI could not find DataImport handlers due to metrics changes

- 
AnalyzingInfixSuggester/BlendedInfixSuggester now work with core reload


See the [Lucene CHANGES.txt](http://lucene.apache.org/core/6_4_1/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/6_4_1/changes/Changes.html) files includedwith the release for a full list of details.

## 23 January 2017 - Apache Lucene 6.4.0 and Apache Solr 6.4.0 Available

The Lucene PMC is pleased to announce the release of Apache Lucene 6.4.0 and Apache Solr 6.4.0.

Lucene can be downloaded from [http://www.apache.org/dyn/closer.lua/lucene/java/6.4.0](http://www.apache.org/dyn/closer.lua/lucene/java/6.4.0)and Solr can be downloaded from
[http://www.apache.org/dyn/closer.lua/lucene/solr/6.4.0](http://www.apache.org/dyn/closer.lua/lucene/solr/6.4.0)

### Highlights of this Lucene release include:
- 
Lucene's best efforts to un-map memory mapped files with "MMapDirectory" now work with the latest Java9 early access builds

- 
A new similarity "BooleanSimilarity" that gives terms a score that is equal to their query boost

- 
The axiomatic family of similarities (6 in total) based on https://www.eecis.udel.edu/~hfang/pubs/sigir05-axiom.pdf

- 
A new token filter "SynonymGraphFilter" that outputs a correct graph structure for multi-token synonyms at query time

- 
Graph token streams, such as those produced by the "SynonymGraphFilter", are now handled accurately by query parsers

- 
A new collector "DocValuesStatsCollector" gives the ability to compute statistics on DocValues field

- 
It is now possible to filter "SortedDocValues" and "SortedSetDocValues" terms enum with a compiled automaton

- 
The "UnifiedHighlighter" can now highlight fields with queries that don't necessarily refer to that field

- 
DrillSideways can now run queries concurrently

- 
Index sorting now supports sorting on multi-valued fields using MIN, MAX, etc. selectors

- 
Points do not store the implicit split dimension in the 1-dimension case. This saves between 6% memory for the largest types such an InetAddressPoint to 33% for the smaller types such as HalfFloatPoint.

- 
The BKD in-memory index for dimensional points now uses a compressed format, using substantially less RAM in some cases

- 
The BKD writing now buffers each leaf block in heap before writing to disk, giving a small speedup in points-heavy use cases

- 
"TermAutomatonQuery" now rewrites to more efficient queries when possible


### Highlights of this Solr release include:

Streaming:
- 
Addition of a HavingStream to Streaming API and Streaming Expressions

- 
Addition of a priority Streaming Expression

- 
Streaming expressions now support collection aliases


Machine Learning:
- Configurable Learning-To-Rank (LTR) support: upload feature definitions, extract feature values, upload your own machine learnt models and use them to rerank search results.

Faceting:
- 
Added "param" query type to facet domain filter specification to obtain filters via query parameters

- 
Any facet command can be filtered using a new parameter filter. Example: { type:terms, field:category, filter:"user:yonik" }


Scripts / Command line:
- 
A new command-line tool to manage the snapshots functionality

- 
bin/solr and bin/solr.cmd now use mkroot command


SolrCloud / SolrJ
- 
LukeResponse now supports dynamic fields

- 
Solrj client now supports hierarchical clusters and other topics marker

- 
Collection backup/restore are extensible.


Security:
- 
Support Secure Impersonation / Proxy User for Solr authentication

- 
Key Store type can be specified in solr.in.sh file for SSL

- 
New generic authentication plugins: 'HadoopAuthPlugin' and 'ConfigurableInternodeAuthHadoopPlugin' that delegate all functionality to Hadoop authentication framework


Query / QueryParser / Highlighting:
- 
A new highlighter: The Unified Highlighter. Try it via `hl.method=unified`; many popular highlighting parameters / features are supported. It's the highest performing highlighter, especially for large documents. Highlighting phrase queries and
 exotic queries are supported equally as well as the Original Highlighter (aka the default/standard one). Please use this new highlighter and report issues since it will likely become the default one day.

- 
Leading wildcard in complexphrase query parser are now accepted and optimized with the
`ReversedWildcardFilterFactory` when it's provided


Metrics:
- 
Use metrics-jvm library to instrument jvm internals such as GC, memory usage and others.

- 
A lot of metrics have been added to the collection: index merges, index store I/Os, query, update, core admin, core load thread pools, shard replication, tlog replay and replicas

- 
A new /admin/metrics API to return all metrics collected by Solr via API.


Misc changes:
- 
The new config parameter 'maxRamMB'can now limit the memory consumed by the FastLRUCache

- 
A new document processor 'SkipExistingDocumentsProcessor' that skips duplicate inserts and ignores updates to missing docs

- 
FieldCache information fetched via the mbeans handler or seen via the UI now displays the total size used.

- 
A new config flag 'enable' allows to enable/disable any cache


Please note, this release cannot be built from source with Java 8 update 121,use an earlier version instead! This is caused by a bug introduced into theJavadocs tool shipped with that update. The workaround was too late for thisLucene release. Of course,
 you can use the binary artifacts.

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/6_4_0/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/6_4_0/changes/Changes.html) files includedwith the release for a full list of details.

## 8 November 2016 - Apache Lucene 6.3.0 and Apache Solr 6.3.0 Available

The Lucene PMC is pleased to announce the release of Apache Lucene 6.3.0 and Apache Solr 6.3.0.

Lucene can be downloaded from [http://www.apache.org/dyn/closer.lua/lucene/java/6.3.0](http://www.apache.org/dyn/closer.lua/lucene/java/6.3.0)and Solr can be downloaded from
[http://www.apache.org/dyn/closer.lua/lucene/solr/6.3.0](http://www.apache.org/dyn/closer.lua/lucene/solr/6.3.0)

### Highlights of this Lucene release include:
- 
A brand new "UnifiedHighlighter" derivative of the PostingsHighlighter that can consume offsets from postings, term vectors, or analysis. It can highlight phrases as accurately as the standard Highlighter. Light term vectors can be used with offsets in postings
 for fast wildcard (MultiTermQuery) highlighting.

- 
SimpleQueryParser now parses '*' to MatchAllDocsQuery

- 
FuzzyQuery now matches all terms within the specified edit distance, even if they are short terms

- 
Points do not store the implicit split dimension in the 1-dimension case. This saves between 6% memory for the largest types such an InetAddressPoint to 33% for the smaller types such as HalfFloatPoint.

- 
Many other changes and bug fixes


### Highlights of this Solr release include:

DocValues, streaming, /export, machine learning
- 
Optimize, store and deploy AI models in Solr

- 
Ability to add custom streaming expressions

- 
New streaming expressions such as "fetch", "executor", and "commit" added.

- 
Parallel SQL accepts <, >, =, etc., symbols.

- 
Support facet scoring with the scoreNodes expression

- 
Retrieving docValues as stored values was sped up by using the proper leaf reader rather than ask for a global view. In extreme cases, this leads to a 100x speedup.


Faceting:
- 
facet.method=enum can bypass exact counts calculation with facet.exists=true, it just returns 1 for terms which exists in result docset

- 
Add "overrequest" parameter to JSON Facet API to control amount of overrequest on a distributed terms facet


Logging:
- 
You can now set Solr's log level through environment variable SOLR_LOG_LEVEL

- 
GC logs are rotated by JVM to a max of 9 files, and backed up via bin/solr scripts

- 
Solr's logging verbosity at the INFO level has been greatly reduced by moving much logging to DEBUG level

- 
The solr-8983-console.log file now only logs STDOUT and STDERR output, not all log4j logs as before

- 
Solr's main log file, solr.log, is now written to SOLR_LOGS_DIR without changing log4j.properties


Start scripts:
- 
Allow 180 seconds for shutdown before killing solr (configurable, old limit 5s) (Unix only)

- 
Start scripts now exits with informative message if using wrong Java version

- 
Fixed "bin/solr.cmd zk upconfig" command which was broken on windows

- 
You can now ask for DEBUG logging simply with '-v' option, and for WARN logging with '-q' option


SolrCloud:
- 
The DELETEREPLICA API can accept a 'count' parameter and remove "count" number of replicas from each shard if the shard name is not provided

- 
The config API shows expanded useParams for request handlers inline

- 
Ability to create/delete/list snapshots at collection level

- 
The modify collection API now waits for the modified properties to show up in the cluster state before returning

- 
Many bug fixes related to SolrCloud recovery for data safety and faster recovery times.


Security:
- 
SolrJ now supports Kerberos delegation tokens

- 
Pooled SSL connections were not being re-used. This is now fixed.

- 
Fix for the blockUnknown property which made inter-node communication impossible

- 
Support SOLR_AUTHENTICATION_OPTS and SOLR_AUTHENTICATION_CLIENT_CONFIGURER in windows bin/solr.cmd script

- 
New parameter -u in bin/post to pass basicauth credentials


Misc changes:
- 
Optimizations to lower memory allocations when indexing JSON as well as for replication between solr cloud nodes.

- 
A new Excel workbook (.xlsx) response writer has been added. Use 'wt=xlsx' request parameter on a query request to enable.


See the [Lucene CHANGES.txt](http://lucene.apache.org/core/6_3_0/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/6_3_0/changes/Changes.html) files includedwith the release for a full list of details.

## 20 September 2016 - Apache Lucene 6.2.1 and Apache Solr 6.2.1 Available

The Lucene PMC is pleased to announce the release of Apache Lucene 6.2.1 and Apache Solr 6.2.1

Lucene can be downloaded from [http://www.apache.org/dyn/closer.lua/lucene/java/6.2.1](http://www.apache.org/dyn/closer.lua/lucene/java/6.2.1)and Solr can be downloaded from
[http://www.apache.org/dyn/closer.lua/lucene/solr/6.2.1](http://www.apache.org/dyn/closer.lua/lucene/solr/6.2.1)

Both releases contain a number of bug fixes.

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/6_2_1/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/6_2_1/changes/Changes.html) files includedwith the release for a full list of details.

## 09 September 2016 - Apache Lucene 5.5.3 and Apache Solr 5.5.3 Available

The Lucene PMC is pleased to announce the release of Apache Lucene 5.5.3 and Apache Solr 5.5.3

Lucene can be downloaded from [http://www.apache.org/dyn/closer.lua/lucene/java/5.5.3](http://www.apache.org/dyn/closer.lua/lucene/java/5.5.3)and Solr can be downloaded from
[http://www.apache.org/dyn/closer.lua/lucene/solr/5.5.3](http://www.apache.org/dyn/closer.lua/lucene/solr/5.5.3)

Both releases contain a number of bug fixes.

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/5_5_3/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/5_5_3/changes/Changes.html) files includedwith the release for a full list of details.

## 25 August 2016 - Apache Lucene 6.2.0 and Apache Solr 6.2.0 Available

The Lucene PMC is pleased to announce the release of Apache Lucene 6.2.0 and Apache Solr 6.2.0.

Lucene can be downloaded from [http://www.apache.org/dyn/closer.lua/lucene/java/6.2.0](http://www.apache.org/dyn/closer.lua/lucene/java/6.2.0)and Solr can be downloaded from
[http://www.apache.org/dyn/closer.lua/lucene/solr/6.2.0](http://www.apache.org/dyn/closer.lua/lucene/solr/6.2.0)

### Highlights of this Lucene release include:
- 
The CREATE_NEW flag is passed when creating a file to ensure Lucene is really write-once

- 
Index numeric ranges (min and max value in a single field) and search by overlapping range

- 
IndexWriter methods return a sequence number indicating effective order of operations across threads

- 
UkrainianMorfologikAnalyzer is a new dictionary based analyzer for the Ukrainian language

- 
The Polygon class can now be created from a GeoJSON string

- 
Compound file creation now verifies checksum of its component files

- 
Index time sorting is now a core feature, and supports dimensional points

- 
StandardAnalyzer is moved to core and is the default analyzer

- 
MatchNoDocsQuery now includes the reason it was created

- 
QueryParser can now be told to not pre-split on whitespace

- 
MMapDirectory tries harder to prevent SIGSEGV if buggy code tries to execute searches after the index was closed, but it's still best effort

- 
MMapDirectory no longer allocates weak references to ease garbage collection

- 
Conjunction (MUST, FILTER) queries are faster

- 
Dimensional points have much faster (~40%) flush time and use less space in the index


### Highlights of this Solr release include:

DocValues, streaming, /export, machine learning
- 
DocValues can now be used with BoolFields

- 
Date and boolean support added to /export handler

- 
Add "scoreNodes" streaming graph expression

- 
Support parallel ETL with the "topic" expression

- 
Feature selection and logistic regression on text via new streaming expressions: "features" and "train"


bin/solr script
- 
Add basic auth support to the bin/solr script

- 
File operations to/from Zookeeper are now supported


SolrCloud
- 
New tag 'role' in replica placement rules, e.g. rule=role:!overseer keeps new repicas off overseer nodes

- 
CDCR: fall back to whole-index replication when tlogs are insufficient

- 
New REPLACENODE command to decommission an existing node and replace it with another new node

- 
New DELETENODE command to delete all replicas on a node


Security
- 
Add Kerberos delegation token support

- 
Support secure impersonation / proxy user for Kerberos authentication


Misc changes
- 
A large number of regressions were fixed in the new Admin UI

- 
New boolean comparison function queries comparing numeric arguments: gt, gte, lt, lte, eq

- 
Upgraded Extraction module to Apache Tika 1.13.

- 
Updated to Hadoop 2.7.2


See the [Lucene CHANGES.txt](http://lucene.apache.org/core/6_2_0/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/6_2_0/changes/Changes.html) files includedwith the release for a full list of details.

## 25 June 2016 - Apache Lucene 5.5.2 and Apache Solr 5.5.2 Available

The Lucene PMC is pleased to announce the release of Apache Lucene 5.5.2 and Apache Solr 5.5.2

Lucene can be downloaded from [http://www.apache.org/dyn/closer.lua/lucene/java/5.5.2](http://www.apache.org/dyn/closer.lua/lucene/java/5.5.2)and Solr can be downloaded from
[http://www.apache.org/dyn/closer.lua/lucene/solr/5.5.2](http://www.apache.org/dyn/closer.lua/lucene/solr/5.5.2)

Both releases contain a number of bug fixes.

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/5_5_2/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/5_5_2/changes/Changes.html) files includedwith the release for a full list of details.

## 17 June 2016 - Apache Lucene 6.1.0 and Apache Solr 6.1.0 Available

The Lucene PMC is pleased to announce the release of Apache Lucene 6.1.0 and Apache Solr 6.1.0.

Lucene can be downloaded from [http://www.apache.org/dyn/closer.lua/lucene/java/6.1.0](http://www.apache.org/dyn/closer.lua/lucene/java/6.1.0)and Solr can be downloaded from
[http://www.apache.org/dyn/closer.lua/lucene/solr/6.1.0](http://www.apache.org/dyn/closer.lua/lucene/solr/6.1.0)

### Highlights of this Lucene release include:
- 
Numerous improvements to LatLonPoint, for indexing a latitude/longitude point and searching by polygon, distance or box, or finding nearest neighbors

- 
Geo3D now has simple APIs for creating common shape queries, matching LatLonPoint

- 
Faster indexing and searching of points.

- 
Faster geo-spatial indexing and searching for LatLonPoint, Geo3D and GeoPoint (see http://home.apache.org/~mikemccand/geobench.html )

- 
HardlinkCopyDirectoryWrapper optimizes file copies using hard links

- 
In case of contention, the query cache now prefers returning an uncached Scorer rather than waiting on a lock.


### Highlights of this Solr release include:
- 
Added graph traversal support, and new "sort" and "random" streaming expressions. It's also now possible to create streaming expressions with the Solr Admin UI.

- 
Fixed the ENUM faceting method to not be unnecessarily rewritten to FCS, which was causing slowdowns.

- 
Reduced garbage creation when creating cache entries.

- 
New [subquery] document transformer to obtatin related documents per result doc.

- 
EmbeddedSolrServer allocates heap much wisely even with plain document list without callbacks.

- 
New GeoJSON response writer for encoding geographic data in query responses.


See the [Lucene CHANGES.txt](http://lucene.apache.org/core/6_1_0/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/6_1_0/changes/Changes.html) files includedwith the release for a full list of details.

## 28 May 2016 - Apache Lucene 6.0.1 and Apache Solr 6.0.1 Available

The Lucene PMC is pleased to announce the release of Apache Lucene 6.0.1 and Apache Solr 6.0.1

Lucene can be downloaded from [http://www.apache.org/dyn/closer.lua/lucene/java/6.0.1](http://www.apache.org/dyn/closer.lua/lucene/java/6.0.1)and Solr can be downloaded from
[http://www.apache.org/dyn/closer.lua/lucene/solr/6.0.1](http://www.apache.org/dyn/closer.lua/lucene/solr/6.0.1)

Both releases contain a number of bug fixes.

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/6_0_1/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/6_0_1/changes/Changes.html) files includedwith the release for a full list of details.

## 5 May 2016 - Apache Lucene 5.5.1 and Apache Solr 5.5.1 Available

The Lucene PMC is pleased to announce the release of Apache Lucene 5.5.1 and Apache Solr 5.5.1

Lucene can be downloaded from [http://www.apache.org/dyn/closer.lua/lucene/java/5.5.1](http://www.apache.org/dyn/closer.lua/lucene/java/5.5.1)and Solr can be downloaded from
[http://www.apache.org/dyn/closer.lua/lucene/solr/5.5.1](http://www.apache.org/dyn/closer.lua/lucene/solr/5.5.1)

Both releases contain a number of bug fixes.

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/5_5_1/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/5_5_1/changes/Changes.html) files includedwith the release for a full list of details.

## 8 April 2016 - Apache Lucene 6.0.0 Available

The Lucene PMC is pleased to announce the release of Apache Lucene 6.0.0 and Apache Solr 6.0.0

Lucene can be downloaded from [http://www.apache.org/dyn/closer.lua/lucene/java/6.0.0](http://www.apache.org/dyn/closer.lua/lucene/java/6.0.0)and Solr can be downloaded from
[http://www.apache.org/dyn/closer.lua/lucene/solr/6.0.0](http://www.apache.org/dyn/closer.lua/lucene/solr/6.0.0)

### Highlights of this Lucene release include:
- 
Java 8 is the minimum Java version required.

- 
Dimensional points, replacing legacy numeric fields, provides fast and space-efficient support for both single- and multi-dimension range and shape filtering. This includes numeric (int, float, long, double), InetAddress, BigInteger and binary range filtering,
 as well as geo-spatial shape search over indexed 2D LatLonPoints. See [this blog post](https://www.elastic.co/blog/lucene-points-6.0) for details. Dependent classes and modules (e.g., MemoryIndex, Spatial Strategies, Join module) have been refactored to use new point types.

- 
Lucene classification module now works on Lucene Documents using a KNearestNeighborClassifier or SimpleNaiveBayesClassifier.

- 
The spatial module no longer depends on third-party libraries. Previous spatial classes have been moved to a new spatial-extras module.

- 
Spatial4j has been updated to a new 0.6 version hosted by locationtech.

- 
TermsQuery performance boost by a more aggressive default query caching policy.

- 
IndexSearcher's default Similarity is now changed to BM25Similarity.

- 
Easier method of defining custom CharTokenizer instances.


### Highlights of this Solr release include:
- 
Improved defaults for "Similarity" used in Solr, in order to provide better default experience for new users.

- 
Improved "Similarity" defaults for users upgrading: DefaultSimilarityFactory has been removed, implicit default Similarity has been changed to SchemaSimilarityFactory, and SchemaSimilarityFactory has been modified to use BM25Similarity as the default for
 field types that do not explicitly declare a Similarity.

- 
Deprecated GET methods for schema are now accessible through the bulk API. The output has less details and is not backward compatible.

- 
Users should set useDocValuesAsStored="false" to preserve sort order on multi-valued fields that have both stored="true" and docValues="true".

- 
Formatted date-times are more consistent with ISO-8601. BC dates are now better supported since they are now formatted with a leading '-'. AD years after 9999 have a leading '+'. Parse exceptions have been improved.

- 
Deprecated SolrServer and subclasses have been removed, use SolrClient instead.

- 
The deprecated configuration in solrconfig.xml has been removed. Users must remove it from solrconfig.xml.

- 
SolrClient.shutdown() has been removed, use SolrClient.close() instead.

- 
The deprecated zkCredientialsProvider element in solrcloud section of solr.xml is now removed. Use the correct spelling (zkCredentialsProvider) instead.

- 
Added support for executing Parallel SQL queries across SolrCloud collections. Includes StreamExpression support and a new JDBC Driver for the SQL Interface.

- 
New features and capabilities added to the streaming API.

- 
Added support for SELECT DISTINCT queries to the SQL interface.

- 
New GraphQuery to enable graph traversal as a query operator.

- 
New support for Cross Data Center Replication consisting of active/passive replication for separate SolrClouds hosted in separate data centers.

- 
Filter support added to Real-time get.

- 
Column alias support added to the Parallel SQL Interface.

- 
New command added to switch between non/secure mode in zookeeper.

- 
Now possible to use IP fragments in replica placement rules.


## 22 February 2016 - Apache Lucene 5.5.0 and Apache Solr 5.5.0 Available

The Lucene PMC is pleased to announce the release of Apache Lucene 5.5.0 and Apache Solr 5.5.0

Lucene can be downloaded from [http://www.apache.org/dyn/closer.lua/lucene/java/5.5.0](http://www.apache.org/dyn/closer.lua/lucene/java/5.5.0)and Solr can be downloaded from
[http://www.apache.org/dyn/closer.lua/lucene/solr/5.5.0](http://www.apache.org/dyn/closer.lua/lucene/solr/5.5.0)

### Highlights of this Lucene release include:
- 
JoinUtil.createJoinQuery can now join on numeric doc values fields

- 
BlendedInfixSuggester now has an exponential reciprocal scoring model, to more strongly favor suggestions with matches closer to the beginning

- 
CustomAnalyzer has improved (compile time) type safety

- 
DFISimilarity implements the divergence from independence scoring model

- 
Fully wrap any other merge policy using MergePolicyWrapper

- 
Sandbox geo point queries have graduated into the spatial module, and now use a more efficient binary term encoding for smaller index size, faster indexing, and decreased search-time heap usage

- 
BooleanQuery performs some new query optimizations

- 
TermsQuery constructors are more GC efficient


### Highlights of this Solr release include:
- 
The schema version has been increased to 1.6, and Solr now returns non-stored doc values fields along with stored fields

- 
The PERSIST CoreAdmin action has been removed

- 
The mergePolicy element is deprecated in favor of a similar mergePolicyFactory element, in solrconfig.xml

- 
CheckIndex now works on HdfsDirectory

- 
RuleBasedAuthorizationPlugin now allows wildcards in the role, and accepts an 'all' permission

- 
Users can now choose compression mode in SchemaCodecFactory

- 
Solr now supports Lucene's XMLQueryParser

- 
Collections APIs now have async support

- 
Uninverted field faceting is re-enabled, for higher performance on rarely changing indices


## 23 January 2016 - Apache Lucene 5.3.2 and Apache Solr 5.3.2 Available

The Lucene PMC is pleased to announce the release of Apache Lucene 5.3.2 and Apache Solr 5.3.2

Lucene can be downloaded from [http://www.apache.org/dyn/closer.lua/lucene/java/5.3.2](http://www.apache.org/dyn/closer.lua/lucene/java/5.3.2)and Solr can be downloaded from
[http://www.apache.org/dyn/closer.lua/lucene/solr/5.3.2](http://www.apache.org/dyn/closer.lua/lucene/solr/5.3.2)

Both releases contain a number of bug fixes.

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/5_3_2/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/5_3_2/changes/Changes.html) files includedwith the release for a full list of details.

## 23 January 2016 - Apache Lucene 5.4.1 and Apache Solr 5.4.1 Available

The Lucene PMC is pleased to announce the release of Apache Lucene 5.4.1 and Apache Solr 5.4.1

Lucene can be downloaded from [http://www.apache.org/dyn/closer.lua/lucene/java/5.4.1](http://www.apache.org/dyn/closer.lua/lucene/java/5.4.1)and Solr can be downloaded from
[http://www.apache.org/dyn/closer.lua/lucene/solr/5.4.1](http://www.apache.org/dyn/closer.lua/lucene/solr/5.4.1)

This release contains an important fix for a corruption bug that wasintroduced in version 5.4.0. If you are on 5.4.0 and using BINARY,SORTED_NUMERIC or SORTED_SET doc values, upgrading to 5.4.1 is stronglyrecommended.

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/5_4_1/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/5_4_1/changes/Changes.html) files includedwith the release for a full list of details.

## 14 December 2015 - Apache Lucene 5.4.0 and Apache Solr 5.4.0 Available

The Lucene PMC is pleased to announce the release of Apache Lucene 5.4.0 and Apache Solr 5.4.0

Lucene can be downloaded from [http://www.apache.org/dyn/closer.lua/lucene/java/5.4.0](http://www.apache.org/dyn/closer.lua/lucene/java/5.4.0)and Solr can be downloaded from
[http://www.apache.org/dyn/closer.lua/lucene/solr/5.4.0](http://www.apache.org/dyn/closer.lua/lucene/solr/5.4.0)

### Highlights of this Lucene release include:

#### API Changes
- Query.getBoost and Query.setBoost are deprecated in favour of the new BoostQuery
- The Filter class is deprecated in favour of FILTER clauses in a BooleanQuery
- DefaultSimilarity has been renamed to ClassicSimilarity to prepare for the move to BM25 in Lucene 6

#### New features
- New Serbian token filter
- New DecimalDigitFilter, to fold unicode digits to latin digits
- New UnicodeWhitespaceTokenizer, that uses Unicode's whitespace definition and splits on NBSP
- New GeoPointDistanceRangeQuery to search for geo-points within a ring
- Query caching is now enabled by default in IndexSearcher, use IndexSearcher.setQueryCache(null) to disable

#### Optimizations
- MatchAllDocsQuery got faster
- Doc values now use less memory for multi-valued fields and less disk in case of sparse fields
- Two-phase iterators got a match cost API so that the costly bits can be checked last

#### Bug fixes
- PatternTokenizer no longer hangs onto heap sized to the maximum input string it's ever seen.

### Highlights of this Solr release include:

#### UI Changes
- The rearchitected Admin UI is now prominently linked to from the existing UI, and includes support for managing collections as well as creating and removing fields via the schema tab. Expect it to be default in the next release.

#### API Features
- New Collections APIs for migrating from clusterstate.json to per-collection state.json and forcing the election of a leader when all replicas in a shard are down.
- A new configset management API has been added.

#### Querying Features
- Filter cache is now accessible via a solr query syntax.
- ScoreJoins can now refer to a single-sharded collection that is replicated on all nodes.
- Add boost support, and 'exclude the queried document' in MoreLikeThis QParser.
- Add a 'sort' local param to the collapse QParser to support using complex sort options to select the representitive doc for each collapsed group.

#### Other Features
- SolrJ now has support for connecting to Solr using basic authentication.
- Analyzing suggesters can now filter suggestions by a context field.
- JSON Facet API: add "method" param to terms/field facets to give an execution hint for what method should be used to facet.
- CloneFieldUpdateProcessorFactory now supports choosing a "dest" field name based on a regex pattern and replacement init options.
- Provide pluggable context tool support for VelocityResponseWriter.

## 24 September 2015 - Apache Lucene 5.3.1 and Apache Solr 5.3.1 Available

The Lucene PMC is pleased to announce the release of Apache Lucene 5.3.1 and Apache Solr 5.3.1

Lucene can be downloaded from [http://www.apache.org/dyn/closer.lua/lucene/java/5.3.1](http://www.apache.org/dyn/closer.lua/lucene/java/5.3.1)and Solr can be downloaded from
[http://www.apache.org/dyn/closer.lua/lucene/solr/5.3.1](http://www.apache.org/dyn/closer.lua/lucene/solr/5.3.1)

### Highlights of this Lucene release include:

#### Bug Fixes
- Remove classloader hack in MorfologikFilter
- UsageTrackingQueryCachingPolicy no longer caches trivial queries like MatchAllDocsQuery
- Fixed BoostingQuery to rewrite wrapped queries

### Highlights of this Solr release include:

#### Bug Fixes
- security.json is not loaded on server start
- RuleBasedAuthorization plugin does not work for the collection-admin-edit permission
- VelocityResponseWriter template encoding issue. Templates must be UTF-8 encoded
- SimplePostTool (also bin/post) -filetypes "*" now works properly in 'web' mode
- example/files update-script.js to be Java 7 and 8 compatible.
- SolrJ could not make requests to handlers with '/admin/' prefix
- Use of timeAllowed can cause incomplete filters to be cached and incorrect results to be returned on subsequent requests
- VelocityResponseWriter's $resource.get(key,baseName,locale) to use specified locale.
- Resolve XSS issue in Admin UI stats page

## 24 August 2015 - Apache Lucene 5.3.0 and Apache Solr 5.3.0 Available

The Lucene PMC is pleased to announce the availabilityof Apache Lucene 5.3.0 and Apache Solr 5.3.0.

Lucene can be downloaded from [http://www.apache.org/dyn/closer.lua/lucene/java/5.3.0](http://www.apache.org/dyn/closer.lua/lucene/java/5.3.0)and Solr can be downloaded from
[http://www.apache.org/dyn/closer.lua/lucene/solr/5.3.0](http://www.apache.org/dyn/closer.lua/lucene/solr/5.3.0)

### Highlights of this Lucene release include:

#### API Changes
- PhraseQuery and BooleanQuery are now immutable

#### New features
- Added a new org.apache.lucene.search.join.CheckJoinIndex class that can be used to validate that an index has an appropriate structure to run join queries
- Added a new BlendedTermQuery to blend statistics across several terms
- New common suggest API that mirrors Lucene's Query/IndexSearcher APIs for Document based suggester.
- IndexWriter can now be initialized from an already open near-real-time or non-NRT reader
- Add experimental range tree doc values format and queries, based on a 1D version of the spatial BKD tree, for a faster and smaller alternative to postings-based numeric and binary term filtering. Range trees can also handle values larger than 64 bits.

#### Geo-related features and improvements
- Added GeoPointField, GeoPointInBBoxQuery, GeoPointInPolygonQuery for simple "indexed lat/lon point in bbox/shape" searching
- Added experimental BKD geospatial tree doc values format and queries, for fast "bbox/polygon contains lat/lon points"
- Use doc values to post-filter GeoPointField hits that fall in boundary cells, resulting in smaller index, faster searches and less heap used for each query

#### Optimizations
- Reduce RAM usage of FieldInfos, and speed up lookup by number, by using an array instead of TreeMap except in very sparse cases
- Faster intersection of the terms dictionary with very finite automata, which can be generated eg. by simple regexp queries
- Various bugfixes and optimizations since the 5.2.0 release.

### Highlights of this Solr release include:
- In addition to many other improvements in the security framework, Solr now includes an AuthenticationPlugin implementing HTTP Basic Auth that stores credentials securely in ZooKeeper. This is a simple way to require a username and password for anyone accessing
 Solr’s admin screen or APIs.
- In built AuthorizationPlugin that provides fine grained control over implementing ACLs for various resources with permisssion rules which are stored in ZooKeeper.
- The JSON Facet API can now change the domain for facet commands, essentially doing a block join and moving from parents to children, or children to parents before calculating the facet data.
- Major improvements in performance of the new Facet Module / JSON Facet API.
- Query and Range Facets under Pivot Facets. Just like the JSON Facet API, pivot facets can how nest other facet types such as range and query facets.
- More Like This Query Parser options. The MoreLikeThis QParser now supports all options provided by the MLT Handler. The query parser is much more versatile than the handler as it works in cloud mode as well as anywhere a normal query can be specified.
- Added Schema API support in SolrJ
- Added Scoring mode for query-time join and block join.
- Added Smile response format

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/5_3_0/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/5_3_0/changes/Changes.html) files includedwith the release for a full list of details.

## 15 June 2015 - Apache Lucene 5.2.1 and Apache Solr 5.2.1 Available

The Lucene PMC is pleased to announce the release of Apache Lucene 5.2.1 and Apache Solr 5.2.1

Lucene can be downloaded from [http://www.apache.org/dyn/closer.lua/lucene/java/5.2.1](http://www.apache.org/dyn/closer.lua/lucene/java/5.2.1)and Solr can be downloaded from
[http://www.apache.org/dyn/closer.lua/lucene/solr/5.2.1](http://www.apache.org/dyn/closer.lua/lucene/solr/5.2.1)

### Highlights of this Lucene release include:
- Fix class loading deadlock relating to Codec initialization, default codec and SPI discovery.
- NRT readers now reflect a new commit even if there is no change to the commit user data
- Queries now get a dummy Similarity when scores are not needed in order to not load unnecessary information like norms

### Highlights of this Solr release include:
- Fix javascript bug introduced by SOLR-7409 that breaks the dataimport screen in the admin UI
- Faceting on a numeric field with a unique() subfacet function on another numeric field can result in incorrect results or an exception
- New Facet Module should respect shards.tolerant and process all non-failing shards instead of throwing an exception
- A request with a json content type but no body caused a null pointer exception
- SolrOutputFormat creates an invalid solr.xml in the solr home zip for MapReduceIndexerTool
- Fix new (Angular-based) admin UI Cloud pane
- The DefaultSolrHighlighter since 5.0 was determining if payloads were present in a way that was slow, especially when lots of fields were highlighted. It's now fast
- Requests are not distributed evenly if the collection isn't present locally

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/5_2_1/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/5_2_1/changes/Changes.html) files includedwith the release for a full list of details.

## 7 June 2015 - Apache Lucene 5.2.0 and Apache Solr 5.2.0 Available

The Lucene PMC is pleased to announce the availabilityof Apache Lucene 5.2.0 and Apache Solr 5.2.0.

Lucene can be downloaded from [http://www.apache.org/dyn/closer.lua/lucene/java/5.2.0](http://www.apache.org/dyn/closer.lua/lucene/java/5.2.0)and Solr can be downloaded from
[http://www.apache.org/dyn/closer.lua/lucene/solr/5.2.0](http://www.apache.org/dyn/closer.lua/lucene/solr/5.2.0)

### Highlights of this Lucene release include:
- 
Span queries now share document conjunction/intersection code with boolean queries, and use two-phased iterators for faster intersection by avoiding loading positions in certain cases.

- 
Added two-phase support to SpanNotQuery, and SpanPositionCheckQuery and its subclasses: SpanPositionRangeQuery, SpanPayloadCheckQuery, SpanNearPayloadCheckQuery, SpanFirstQuery.

- 
Added a new query time join to the join module that uses global ordinals, which is faster for subsequent joins between reopens.

- 
New CompositeSpatialStrategy combines speed of RPT with accuracy of SDV. Includes optimized Intersect predicate to avoid many geometry checks. Uses TwoPhaseIterator.

- 
New LimitTokenOffsetFilter that limits tokens to those before a configured maximum start offset.

- 
New spatial PackedQuadPrefixTree, a generally more efficient choice than QuadPrefixTree, especially for high precision shapes. When used, you should typically disable RPT's pruneLeafyBranches option.

- 
Expressions now support bindings keys that look like zero arg functions

- 
Add SpanWithinQuery and SpanContainingQuery that return spans inside of / containing another spans.

- 
New Spatial "Geo3d" API with partial Spatial4j integration. It is a set of shapes implemented using 3D planar geometry for calculating spatial relations on the surface of a sphere. Shapes include Point, BBox, Circle, Path (buffered line string), and Polygon.

- 
Various bugfixes and optimizations since the 5.1.0 release.


### Highlights of this Solr release include:
- 
Restore API allows restoring a core from an index backup.

- 
JSON Facet API
- unique() is now implemented for numeric and date fields
- Optional flatter form via a "type" parameter
- Added support for "mincount" parameter in range facets to suppress buckets less than that count
- Multi-select faceting support for the Facet Module via the "excludeTags" parameter which disregards any matching tagged filters for that facet.
- hll() facet function for distributed cardinality via HyperLogLog algorithm.See examples at http://yonik.com/solr-count-distinct/

- 
A new "facet.range.method" parameter to let users choose how to do range faceting between an implementation based on filters (previous algorithm, using "facet.range.method=filter") or DocValues ("facet.range.method=dv")

- 
Rule-based Replica assignment during collection, shard, and replica creation.

- 
Stats component:
- New 'cardinality' option for stats.field, uses HyperLogLog to efficiently estimate the cardinality of a field w/bounded RAM. Blog post: https://lucidworks.com/blog/hyperloglog-field-value-cardinality-stats/
- stats.field now supports individual local params for 'countDistinct' and 'distinctValues'. 'calcdistinct' is still supported as an alias for both options.

- 
Solr security
- Authentication and Authorization frameworks that define interfaces, and mechanisms to create, load, and use authorization/authentication plugins have been added.
- A Kerberos authentication plugin which would allow running a Kerberized Solr setup.

- 
Solr Streaming Expressions See https://cwiki.apache.org/confluence/display/solr/Streaming+Expressions

- 
bin/post (and SimplePostTool in -Dauto=yes mode) now sends rather than skips files without a known content type, as "application/octet-stream", provided it still is in the allowed filetypes setting.

- 
HDFS transaction log replication factor is now configurable

- 
A cluster-wide property can now be be added/edited/deleted using the zkcli script and doesn't require a running Solr instance.

- 
New spatial RptWithGeometrySpatialField, based on CompositeSpatialStrategy, which blends RPT indexes for speed with serialized geometry for accuracy. Includes a Lucene segment based in-memory shape cache.

- 
Refactored Admin UI using AngularJS. It isn't the default, but a parallel UI interface in this release.

- 
Solr has internally been upgraded to use Jetty 9.


Both releases contain a number of new features, bug fixes, and optimizations.

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/5_2_0/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/5_2_0/changes/Changes.html) files includedwith the release for a full list of details.

## 14 April 2015 - Apache Lucene 5.1.0 and Apache Solr 5.1.0 Available

The Lucene PMC is pleased to announce the availabilityof Apache Lucene 5.1.0 and Apache Solr 5.1.0.

Lucene can be downloaded from [http://www.apache.org/dyn/closer.lua/lucene/java/5.1.0](http://www.apache.org/dyn/closer.lua/lucene/java/5.1.0)and Solr can be downloaded from
[http://www.apache.org/dyn/closer.lua/lucene/solr/5.1.0](http://www.apache.org/dyn/closer.lua/lucene/solr/5.1.0)

Both releases contain a number of new features, bug fixes, and optimizations.

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/5_1_0/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/5_1_0/changes/Changes.html) files includedwith the release for a full list of details.

## 5 March 2015 - Apache Lucene 4.10.4 and Apache Solr 4.10.4 Available

The Lucene PMC is pleased to announce the availabilityof Apache Lucene 4.10.4 and Apache Solr 4.10.4.

Lucene can be downloaded from [http://www.apache.org/dyn/closer.lua/lucene/java/4.10.4](http://www.apache.org/dyn/closer.lua/lucene/java/4.10.4)and Solr can be downloaded from
[http://www.apache.org/dyn/closer.lua/lucene/solr/4.10.4](http://www.apache.org/dyn/closer.lua/lucene/solr/4.10.4)

Both releases contain a number of bug fixes.

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/4_10_4/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/4_10_4/changes/Changes.html) files includedwith the release for a full list of details.

## 20 February 2015 - Apache Lucene 5.0.0 and Apache Solr 5.0.0 Available

The Lucene PMC is pleased to announce the availabilityof Apache Lucene 5.0.0 and Apache Solr 5.0.0.

Lucene can be downloaded from [http://lucene.apache.org/core/mirrors-core-latest-redir.html](http://lucene.apache.org/core/mirrors-core-latest-redir.html)and Solr can be downloaded from
[http://lucene.apache.org/solr/mirrors-solr-latest-redir.html](http://lucene.apache.org/solr/mirrors-solr-latest-redir.html)

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/5_0_0/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/5_0_0/changes/Changes.html) files includedwith the release for a full list of details.

### Highlights of the Lucene release include:

Stronger index safety
- 
All file access now uses Java’s NIO.2 APIs which give Lucene stronger index safety in terms of better error handling and safer commits.

- 
Every Lucene segment now stores a unique id per-segment and per-commit to aid in accurate replication of index files.

- 
During merging, IndexWriter now always checks the incoming segments for corruption before merging. This can mean, on upgrading to 5.0.0, that merging may uncover long-standing latent corruption in an older 4.x index.


Reduced heap usage
- 
Lucene now supports random-writable and advance-able sparse bitsets (RoaringDocIdSet and SparseFixedBitSet), so the heap required is in proportion to how many bits are set, not how many total documents exist in the index.

- 
Heap usage during IndexWriter merging is also much lower with the new Lucene50Codec, since doc values and norms for the segments being merged are no longer fully loaded into heap for all fields; now they are loaded for the one field currently being merged,
 and then dropped.

- 
The default norms format now uses sparse encoding when appropriate, so indices that enable norms for many sparse fields will see a large reduction in required heap at search time.

- 
5.0 has a new API to print a tree structure showing a recursive breakdown of which parts are using how much heap.


Other features
- 
FieldCache is gone (moved to a dedicated UninvertingReader in the misc module). This means when you intend to sort on a field, you should index that field using doc values, which is much faster and less heap consuming than FieldCache.

- 
Tokenizers and Analyzers no longer require Reader on init.

- 
NormsFormat now gets its own dedicated NormsConsumer/Producer

- 
SortedSetSortField, used to sort on a multi-valued field, is promoted from sandbox to Lucene's core.

- 
PostingsFormat now uses a "pull" API when writing postings, just like doc values. This is powerful because you can do things in your postings format that require making more than one pass through the postings such as iterating over all postings for each
 term to decide which compression format it should use.

- 
New DateRangeField type enables Indexing and searching of date ranges, particularly multi-valued ones.

- 
A new ExitableDirectoryReader extends FilterDirectoryReader and enables exiting requests that take too long to enumerate over terms.

- 
Suggesters from multi-valued field can now be built as DocumentDictionary now enumerates each value separately in a multi-valued field.

- 
ConcurrentMergeScheduler detects whether the index is on SSD or not and does a better job defaulting its settings. This only works on Linux for now; other OS's will continue to use the previous defaults (tuned for spinning disks).

- 
Auto-IO-throttling has been added to ConcurrentMergeScheduler, to rate limit IO writes for each merge depending on incoming merge rate.

- 
CustomAnalyzer has been added that allows to configure analyzers like you do in Solr's index schema. This class has a builder API to configure Tokenizers, TokenFilters, and CharFilters based on their SPI names and parameters as documented by the corresponding
 factories.

- 
Memory index now supports payloads.

- 
Added a filter cache with a usage tracking policy that caches filters based on frequency of use.

- 
The default codec has an option to control BEST_SPEED or BEST_COMPRESSION for stored fields.

- 
Stored fields are merged more efficiently, especially when upgrading from previous versions or using SortingMergePolicy


### Highlights of the Solr release include:
- 
Usability improvements that include improved bin scripts and new and restructured examples.

- 
Scripts to support installing and running Solr as a service on Linux.

- 
Distributed IDF is now supported and can be enabled via the config. Currently, there are four supported implementations for the same:
- LocalStatsCache: Local document stats.
- ExactStatsCache: One time use aggregation
- ExactSharedStatsCache: Stats shared across requests
- LRUStatsCache: Stats shared in an LRU cache across requests

- 
Solr will no longer ship a war file and instead be a downloadable application.

- 
SolrJ now has first class support for Collections API.

- 
Implicit registration of replication,get and admin handlers.

- 
Config API that supports paramsets for easily configuring solr parameters and configuring fields. This API also supports managing of pre-existing request handlers and editing common solrconfig.xml via overlay.

- 
API for managing blobs allows uploading request handler jars and registering them via config API.

- 
BALANCESHARDUNIQUE Collection API that allows for even distribution of custom replica properties.

- 
There's now an option to not shuffle the nodeSet provided during collection creation.

- 
Option to configure bandwidth usage by Replication handler to prevent it from using up all the bandwidth.

- 
Splitting of clusterstate to per-collection enables scalability improvement in SolrCloud. This is also the default format for new Collections that would be created going forward.

- 
timeAllowed is now used to prematurely terminate requests during query expansion and SolrClient request retry.

- 
pivot.facet results can now include nested stats.field results constrained by those pivots.

- 
stats.field can be used to generate stats over the results of arbitrary numeric functions. It also allows for requesting for statistics for pivot facets using tags.

- 
A new DateRangeField has been added for indexing date ranges, especially multi-valued ones.

- 
Spatial fields that used to require units=degrees now take distanceUnits=degrees/kilometers miles instead.

- 
MoreLikeThis query parser allows requesting for documents similar to an existing document and also works in SolrCloud mode.

- 
Logging improvements:
- Transaction log replay status is now logged
- Optional logging of slow requests.


## 29 December 2014 - Apache Lucene 4.10.3 and Apache Solr 4.10.3 Available

The Lucene PMC is pleased to announce the availabilityof Apache Lucene 4.10.3 and Apache Solr 4.10.3.

Lucene can be downloaded from [http://lucene.apache.org/core/mirrors-core-latest-redir.html](http://lucene.apache.org/core/mirrors-core-latest-redir.html)and Solr can be downloaded from
[http://lucene.apache.org/solr/mirrors-solr-latest-redir.html](http://lucene.apache.org/solr/mirrors-solr-latest-redir.html)

Both releases contain a number of bug fixes. The Solr release includes a security vulnerabilityfix that you can read about on the Solr news page:
[http://lucene.apache.org/solr/news.html](http://lucene.apache.org/solr/news.html)

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/4_10_3/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/4_10_3/changes/Changes.html) files includedwith the release for a full list of details, and Happy Holidays!

## 31 October 2014 - Apache Lucene 4.10.2 and Apache Solr 4.10.2 Available

The Lucene PMC is pleased to announce the availabilityof Apache Lucene 4.10.2 and Apache Solr 4.10.2.

Lucene can be downloaded from [http://lucene.apache.org/core/mirrors-core-latest-redir.html](http://lucene.apache.org/core/mirrors-core-latest-redir.html)and Solr can be downloaded from
[http://lucene.apache.org/solr/mirrors-solr-latest-redir.html](http://lucene.apache.org/solr/mirrors-solr-latest-redir.html)

Both releases contain a number of bug fixes.

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/4_10_2/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/4_10_2/changes/Changes.html) files includedwith the release for a full list of details, and Happy Halloween!

## 29 September 2014 - Apache Lucene 4.10.1 and Apache Solr 4.10.1 Available

The Lucene PMC is pleased to announce the availabilityof Apache Lucene 4.10.1 and Apache Solr 4.10.1.

Lucene can be downloaded from [http://lucene.apache.org/core/mirrors-core-latest-redir.html](http://lucene.apache.org/core/mirrors-core-latest-redir.html)and Solr can be downloaded from
[http://lucene.apache.org/solr/mirrors-solr-latest-redir.html](http://lucene.apache.org/solr/mirrors-solr-latest-redir.html)

Both releases contain a number of bug fixes.

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/4_10_1/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/4_10_1/changes/Changes.html) files includedwith the release for a full list of details.

## 22 September 2014 - Apache Lucene 4.9.1 and Apache Solr 4.9.1 Available

The Lucene PMC is pleased to announce the availabilityof Apache Lucene 4.9.1 and Apache Solr 4.9.1.

Lucene can be downloaded from [http://lucene.apache.org/core/mirrors-core-latest-redir.html](http://lucene.apache.org/core/mirrors-core-latest-redir.html)and Solr can be downloaded from
[http://lucene.apache.org/solr/mirrors-solr-latest-redir.html](http://lucene.apache.org/solr/mirrors-solr-latest-redir.html)

Both releases contain a number of bug fixes.

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/4_9_1/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/4_9_1/changes/Changes.html) files includedwith the release for a full list of details.

## 03 September 2014 - Apache Lucene 4.10.0 and Apache Solr 4.10.0 Available

The Lucene PMC is pleased to announce the availabilityof Apache Lucene 4.10.0 and Apache Solr 4.10.0.

Lucene can be downloaded from [http://lucene.apache.org/core/mirrors-core-latest-redir.html](http://lucene.apache.org/core/mirrors-core-latest-redir.html)and Solr can be downloaded from
[http://lucene.apache.org/solr/mirrors-solr-latest-redir.html](http://lucene.apache.org/solr/mirrors-solr-latest-redir.html)

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/4_10_0/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/4_10_0/changes/Changes.html) files includedwith the release for a full list of details.

### Highlights of the Lucene release include:
- 
New TermAutomatonQuery using an automaton for proximity queries. [http://blog.mikemccandless.com/2014/08/a-new-proximity-query-for-lucene-using.html](http://blog.mikemccandless.com/2014/08/a-new-proximity-query-for-lucene-using.html)

- 
New OrdsBlockTree terms dictionary supporting ord lookup.

- 
Simplified matchVersion handling for Analyzers with new setVersion method, as well as Analyzer constructors not requiring Version.

- 
Fixed possible corruption when opening a 3.x index with NRT reader.

- 
Fixed edge case in StandardTokenizer that caused extremely slow parsing times with long text which partially matched grammar rules.


### Highlights of the Solr release include:
- 
This release upgrades Solr Cell's (contrib/extraction) dependency on Apache POI to mitigate
[2 security vulnerabilities](http://lucene.apache.org/solr/solrnews.html#18-august-2014-recommendation-to-update-apache-poi-in-apache-solr-480-481-and-490-installations).

- 
Scripts for starting, stopping, and running Solr examples

- 
Distributed query support for facet.pivot

- 
Interval Faceting for Doc Values fields

- 
New "terms" QParser for efficiently filtering documents by a list of values


## 18 August 2014 - Recommendation to update Apache POI in Apache Solr 4.8.0, 4.8.1, and 4.9.0 installations

Apache Solr versions 4.8.0, 4.8.1, 4.9.0 bundle Apache POI 3.10-beta2 with its binary release tarball.This version (and all previous ones) of Apache POI are vulnerable to the following issues:CVE-2014-3529
*(XML External Entity (XXE) problem in Apache POI's OpenXML parser)*,CVE-2014-3574
*(XML Entity Expansion (XEE) problem in Apache POI's OpenXML parser)*.

The Apache POI PMC released a bugfix version (3.10.1) today.

Solr users are affected by these issues, if they enable the "Apache Solr Content Extraction Library (Solr Cell)"contrib module from the folder "contrib/extraction" of the release tarball.

Users of Apache Solr are strongly advised to keep the module disabled if they don't use it.Alternatively, users of Apache Solr 4.8.0, 4.8.1, or 4.9.0 can update the affected libraries byreplacing the vulnerable JAR files in the distribution folder. Users
 of previous versions haveto update their Solr release first, patching older versions is impossible.

For detailed instructions, see [Solr's News](http://lucene.apache.org/solr/solrnews.html#18-august-2014-recommendation-to-update-apache-poi-in-apache-solr-480-481-and-490-installations)

## 25 June 2014 - Apache Lucene 4.9.0 and Apache Solr 4.9.0 Available

The Lucene PMC is pleased to announce the availabilityof Apache Lucene 4.9.0 and Apache Solr 4.9.0.

Lucene can be downloaded from [http://lucene.apache.org/core/mirrors-core-latest-redir.html](http://lucene.apache.org/core/mirrors-core-latest-redir.html)and Solr can be downloaded from
[http://lucene.apache.org/solr/mirrors-solr-latest-redir.html](http://lucene.apache.org/solr/mirrors-solr-latest-redir.html)

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/4_9_0/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/4_9_0/changes/Changes.html) files includedwith the release for a full list of details.

### Highlights of the Lucene release include:
- 
New Terms.getMin/Max methods to retrieve the lowest and highest terms per field.

- 
New IDVersionPostingsFormat, optimized for ID lookups that associate a monotonically increasing version per ID.

- 
Atomic update of a set of doc values fields.

- 
Numerous optimizations for doc values search-time performance.

- 
New (default) Lucene49NormsFormat to better compress certain cases such as very short fields.

- 
New SORTED_NUMERIC docvalues type for efficient processing of multi-valued numeric fields.

- 
Indexer passes previous token stream for easier reuse.

- 
MoreLikeThis accepts multiple values per field.

- 
All classes that estimate their RAM usage now implement a new Accountable interface.

- 
Lucene files are now written by (File)OutputStream on all platforms, completely disallowing seeking with simplified IO APIs.

- 
Improve the confusing error message when MMapDirectory cannot create a new map.


### Highlights of the Solr release include:
- 
Numerous optimizations for doc values search-time performance

- 
Allow a client application to request the minium achieved replication factor for an update request (single or batch) by sending an optional parameter "min_rf".

- 
Query re-ranking support with the new ReRankingQParserPlugin.

- 
A new [child ...] DocTransformer for optionally including Block-Join decendent documents inline in the results of a search.

- 
A new (default) Lucene49NormsFormat to better compress certain cases such as very short fields.


## 11 June 2014 - Open Relevance sub-project closed

The Apache Lucene Project Management Committee decided in a vote,that the Apache Lucene sub-project "Open Relevance" will be discontinued. There was only modest activity during the lastyears and the project made no releases. Thank you to all committers for
 their support in this project!

## 20 May 2014 - Apache Lucene 4.8.1 and Apache Solr 4.8.1 Available

The Lucene PMC is pleased to announce the availabilityof Apache Lucene 4.8.1 and Apache Solr 4.8.1.

Lucene can be downloaded from [http://lucene.apache.org/core/mirrors-core-latest-redir.html](http://lucene.apache.org/core/mirrors-core-latest-redir.html)and Solr can be downloaded from
[http://lucene.apache.org/solr/mirrors-solr-latest-redir.html](http://lucene.apache.org/solr/mirrors-solr-latest-redir.html)

Both releases contain a number of bug fixes.

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/4_8_1/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/4_8_1/changes/Changes.html) files includedwith the release for a full list of details.

## 28 April 2014 - Apache Lucene 4.8.0 and Apache Solr 4.8.0 Available

The Lucene PMC is pleased to announce the availabilityof Apache Lucene 4.8.0 and Apache Solr 4.8.0.

Lucene can be downloaded from [http://lucene.apache.org/core/mirrors-core-latest-redir.html](http://lucene.apache.org/core/mirrors-core-latest-redir.html)and Solr can be downloaded from
[http://lucene.apache.org/solr/mirrors-solr-latest-redir.html](http://lucene.apache.org/solr/mirrors-solr-latest-redir.html)

Both releases now require Java 7 or greater (recommended isOracle Java 7 or OpenJDK 7, minimum update 55; earlier versionshave known JVM bugs affecting Lucene and Solr). In addition,both are fully compatible with Java 8.

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/4_8_0/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/4_8_0/changes/Changes.html) files includedwith the release for a full list of details.

### Highlights of the Lucene release include:
- 
All index files now store end-to-end checksums, which are now validated during merging and reading. This ensures that corruptions caused by any bit-flipping hardware problems or bugs in the JVM can be detected earlier. For full detection be sure to enable
 all checksums during merging (it's disabled by default).

- 
Lucene has a new Rescorer/QueryRescorer API to perform second-pass rescoring or reranking of search results using more expensive scoring functions after first-pass hit collection.

- 
AnalyzingInfixSuggester now supports near-real-time autosuggest.

- 
Simplified impact-sorted postings (using SortingMergePolicy and EarlyTerminatingCollector) to use Lucene's Sort class to express the sort order.

- 
Bulk scoring and normal iterator-based scoring were separated, so some queries can do bulk scoring more effectively.

- 
Switched to MurmurHash3 to hash terms during indexing.

- 
IndexWriter now supports updating of binary doc value fields.

- 
HunspellStemFilter now uses 10 to 100x less RAM. It also loads all known OpenOffice dictionaries without error.

- 
Lucene now also fsyncs the directory metadata on commits, if the operating system and file system allow it (Linux, MacOSX are known to work).

- 
Lucene now uses Java 7 file system functions under the hood, so index files can be deleted on Windows, even when readers are still open.

- 
A serious bug in NativeFSLockFactory was fixed, which could allow multiple IndexWriters to acquire the same lock. The lock file is no longer deleted from the index directory even when the lock is not held.


### Highlights of the Solr release include:
- 
`<fields>` and `<types>` tags have been deprecated from schema.xml. There is no longer any reason to keep them in the schema file, they may be safely removed. This allows intermixing of
`<fieldType>`, `<field>` and 
```
<copyField>
```
 definitions if desired.

- 
The new {!complexphrase} query parser supports wildcards, ORs etc. inside Phrase Queries.

- 
New Collections API CLUSTERSTATUS action reports the status of collections, shards, and replicas, and also lists collection aliases and cluster properties.

- 
Added managed synonym and stopword filter factories, which enable synonym and stopword lists to be dynamically managed via REST API.

- 
JSON updates now support nested child documents, enabling {!child} and {!parent} block join queries.

- 
Added ExpandComponent to expand results collapsed by the CollapsingQParserPlugin, as well as the parent/child relationship of nested child documents.

- 
Long-running Collections API tasks can now be executed asynchronously; the new REQUESTSTATUS action provides status.

- 
Added a hl.qparser parameter to allow you to define a query parser for hl.q highlight queries.

- 
In Solr single-node mode, cores can now be created using named configsets.

- 
New DocExpirationUpdateProcessorFactory supports computing an expiration date for documents from the "TTL" expression, as well as automatically deleting expired documents on a periodic basis.


## 15 April 2014 - Apache Lucene 4.7.2 and Apache Solr 4.7.2 Available

The Lucene PMC is pleased to announce the availabilityof Apache Lucene 4.7.2 and Apache Solr 4.7.2.

Lucene can be downloaded from [http://lucene.apache.org/core/mirrors-core-latest-redir.html](http://lucene.apache.org/core/mirrors-core-latest-redir.html)and Solr can be downloaded from
[http://lucene.apache.org/solr/mirrors-solr-latest-redir.html](http://lucene.apache.org/solr/mirrors-solr-latest-redir.html)

Both releases contain a number of bug fixes.

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/4_7_2/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/4_7_2/changes/Changes.html) files includedwith the release for a full list of details.

## 02 April 2014 - Apache Lucene 4.7.1 and Apache Solr 4.7.1 Available

The Lucene PMC is pleased to announce the availabilityof Apache Lucene 4.7.1 and Apache Solr 4.7.1.

Lucene can be downloaded from [http://lucene.apache.org/core/mirrors-core-latest-redir.html](http://lucene.apache.org/core/mirrors-core-latest-redir.html)and Solr can be downloaded from
[http://lucene.apache.org/solr/mirrors-solr-latest-redir.html](http://lucene.apache.org/solr/mirrors-solr-latest-redir.html)

Both releases contain a number of bug fixes.

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/4_7_1/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/4_7_1/changes/Changes.html) files includedwith the release for a full list of details.

## 12 March 2014 - Apache Lucene 4.8 and Apache Solr 4.8 will require Java 7

The Apache Lucene/Solr committers decided with a large majority on the vote to require
**Java 7** for the next minor release of Apache Lucene and Apache Solr (version 4.8)!

The next release will also contain some improvements for Java 7:
- 
Better file handling (especially on Windows) in the directory implementations. Files can now be deleted on windows, although the index is still open - like it was always possible on Unix environments (delete on last close semantics).

- 
Speed improvements in sorting comparators: Sorting now uses Java 7's own comparators for integer and long sorts, which are highly optimized by the Hotspot VM.


If you want to stay up-to-date with Lucene and Solr, you should upgrade your infrastructure to Java 7.Please be aware that you must use at least use Java 7u1.The recommended version at the moment is Java 7u25. Later versions like 7u40, 7u45,... have a bug
 causing index corrumption.Ideally use the Java 7u60 prerelease, which has fixed this bug. Once 7u60 is out, this will be the recommended version.In addition, there is no more Oracle/BEA JRockit available for Java 7, use the official Oracle Java 7.JRockit was
 never working correctly with Lucene/Solr (causing index corrumption), so this should not be an issue.Please also review our list of JVM bugs:
[http://wiki.apache.org/lucene-java/JavaBugs](http://wiki.apache.org/lucene-java/JavaBugs)

*EDIT (as of 15 April 2014):* The recently released Java 7u55 fixes the above bug causing index corrumption.This version is now the recommended version for running Apache Lucene and Solr.

## 26 February 2014 - Apache Lucene 4.7.0 and Apache SolrTM 4.7.0 available

The Lucene PMC is pleased to announce the availabilityof Apache Lucene 4.7.0 and Apache Solr 4.7.0.

Lucene can be downloaded from [http://lucene.apache.org/core/mirrors-core-latest-redir.html](http://lucene.apache.org/core/mirrors-core-latest-redir.html)and Solr can be downloaded from
[http://lucene.apache.org/solr/mirrors-solr-latest-redir.html](http://lucene.apache.org/solr/mirrors-solr-latest-redir.html)

Both releases contain a number of bug fixes.

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/4_7_0/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/4_7_0/changes/Changes.html) files includedwith the release for a full list of details.

## 28 January 2014 - Apache Lucene 4.6.1 and Apache SolrTM 4.6.1 available

The Lucene PMC is pleased to announce the availabilityof Apache Lucene 4.6.1 and Apache Solr 4.6.1.

Lucene can be downloaded from [http://lucene.apache.org/core/mirrors-core-latest-redir.html](http://lucene.apache.org/core/mirrors-core-latest-redir.html)and Solr can be downloaded from
[http://lucene.apache.org/solr/mirrors-solr-latest-redir.html](http://lucene.apache.org/solr/mirrors-solr-latest-redir.html)

Both releases contain a number of bug fixes.

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/4_6_1/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/4_6_1/changes/Changes.html) files includedwith the release for a full list of details.

## 24 November 2013 - Apache Lucene 4.6.0 and Apache SolrTM 4.6.0 available

The Lucene PMC is pleased to announce the availabilityof Apache Lucene 4.6.0 and Apache Solr 4.6.0.

Lucene can be downloaded from [http://lucene.apache.org/core/mirrors-core-latest-redir.html](http://lucene.apache.org/core/mirrors-core-latest-redir.html)and Solr can be downloaded from
[http://lucene.apache.org/solr/mirrors-solr-latest-redir.html](http://lucene.apache.org/solr/mirrors-solr-latest-redir.html)

Both releases contain a number of bug fixes.

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/4_6_0/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/4_6_0/changes/Changes.html) files includedwith the release for a full list of details.

## 24 October 2013 - Apache Lucene 4.5.1 and Apache SolrTM 4.5.1 available

The Lucene PMC is pleased to announce the availabilityof Apache Lucene 4.5.1 and Apache Solr 4.5.1.

Lucene can be downloaded from [http://lucene.apache.org/core/mirrors-core-latest-redir.html](http://lucene.apache.org/core/mirrors-core-latest-redir.html)and Solr can be downloaded from
[http://lucene.apache.org/solr/mirrors-solr-latest-redir.html](http://lucene.apache.org/solr/mirrors-solr-latest-redir.html)

Both releases contain a number of bug fixes.

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/4_5_1/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/4_5_1/changes/Changes.html) files includedwith the release for a full list of details.

## 5 October 2013 - Apache Lucene 4.5 and Apache SolrTM 4.5 available

The Lucene PMC is pleased to announce the availabilityof Apache Lucene 4.5 and Apache Solr 4.5.

Lucene can be downloaded from [http://lucene.apache.org/core/mirrors-core-latest-redir.html](http://lucene.apache.org/core/mirrors-core-latest-redir.html)and Solr can be downloaded from
[http://lucene.apache.org/solr/mirrors-solr-latest-redir.html](http://lucene.apache.org/solr/mirrors-solr-latest-redir.html)

See the [Lucene CHANGES.txt](http://lucene.apache.org/core/4_5_0/changes/Changes.html) and[Solr CHANGES.txt](http://lucene.apache.org/solr/4_5_0/changes/Changes.html) files includedwith the release for a full list of details.

### Highlights of the Lucene release include:
- 
Added support for missing values to DocValues fields through AtomicReader.getDocsWithField.

- 
Lucene 4.5 has a new Lucene45Codec with Lucene45DocValues, supporting missing values and with most datastructures residing off-heap.

- 
New in-memory DocIdSet implementations which are especially better than FixedBitSet on small sets: WAH8DocIdSet, PFORDeltaDocIdSet and EliasFanoDocIdSet.

- 
CachingWrapperFilter now caches filters with WAH8DocIdSet by default, which has the same memory usage as FixedBitSet in the worst case but is smaller and faster on small sets.

- 
TokenStreams now set the position increment in end(), so we can handle trailing holes.

- 
IndexWriter no longer clones the given IndexWriterConfig.


Lucene 4.5 also includes numerous optimizations and bugfixes.

### Highlights of the Solr release include:
- 
Custom sharding support, including the ability to shard by field.

- 
DocValue improvements: single valued fields no longer require a default value, allowiing dynamicFields to contain doc values, as well as sortMissingFirst and sortMissingLast on docValue fields.

- 
Ability to store solr.xml in ZooKeeper.

- 
Multithreaded faceting.

- 
CloudSolrServer can now route updates directly to the appropriate shard leader.


Solr 4.5 also includes numerous optimizations and bugfixes.


## The Apache Software Foundation

The [Apache Software Foundation](http://www.apache.org) provides support for the Apache community of open-source software projects. The Apache projects are defined by collaborative consensus based processes, an open, pragmatic
 software license and a desire to create high quality software that leads the way in its field. Apache Lucene, Apache Solr, Apache PyLucene, Apache Open Relevance Project and their respective logos are trademarks of The Apache Software Foundation. All other
 marks mentioned may be trademarks or registered trademarks of their respective owners.



[Download](http://lucene.apache.org/core/mirrors-core-latest-redir.html?)
Click to begin

of Apache Lucene 6.4.1


Apache Lucene 6.4.1

[Download](http://lucene.apache.org/solr/mirrors-solr-latest-redir.html?)
Click to begin

of Apache Solr 6.4.1


Apache Solr 6.4.1

# Projects
- [Lucene Core (Java)](http://lucene.apache.org/core/)
- [Solr](http://lucene.apache.org/solr/)
- [PyLucene](http://lucene.apache.org/pylucene/)
- [Open Relevance (Discontinued)](http://lucene.apache.org/openrelevance/)

# About
- [License](http://www.apache.org/licenses/LICENSE-2.0)
- [Who We are](http://lucene.apache.org/whoweare.html)

# ASF links
- [Apache Software Foundation](http://www.apache.org)
- [Thanks](http://www.apache.org/foundation/thanks.html)
- [Become a Sponsor](http://www.apache.org/foundation/sponsorship.html)
- [Security](http://www.apache.org/security/)

# Editing This Site
- [Instructions](http://lucene.apache.org/site-instructions.html)
- [ASF CMS](http://www.apache.org/dev/cms.html)
- [ASF CMS Reference/FAQ](http://www.apache.org/dev/cmsref.html)
- [Markdown](http://daringfireball.net/projects/markdown/syntax)

# Related Projects
- [Apache Hadoop](http://hadoop.apache.org)
- [Apache ManifoldCF](http://manifoldcf.apache.org/)
- [Apache Lucene.Net](http://lucenenet.apache.org/)
- [Apache Lucy](http://lucy.apache.org/)
- [Apache Mahout](http://mahout.apache.org)
- [Apache Nutch](http://nutch.apache.org)
- [Apache OpenNLP](http://opennlp.apache.org/)
- [Apache Tika](http://tika.apache.org)
- [Apache Zookeeper](http://zookeeper.apache.org)














