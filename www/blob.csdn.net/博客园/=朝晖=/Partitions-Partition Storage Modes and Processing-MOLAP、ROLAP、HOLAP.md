# Partitions - Partition Storage Modes and Processing-MOLAP、ROLAP、HOLAP - =朝晖= - 博客园
# [Partitions - Partition Storage Modes and Processing-MOLAP、ROLAP、HOLAP](https://www.cnblogs.com/dhcn/p/7502475.html)
https://docs.microsoft.com/en-us/sql/analysis-services/multidimensional-models-olap-logical-cube-objects/partitions-partition-storage-modes-and-processing
The storage mode of a partition affects the query and processing performance, storage requirements, and storage locations of the partition and its parent measure group and cube. The choice of storage mode also affects processing choices.
A partition can use one of three basic storage modes:
- 
Multidimensional OLAP (MOLAP)
- 
Relational OLAP (ROLAP)
- 
Hybrid OLAP (HOLAP)
Microsoft SQL Server Analysis Services supports all three basic storage modes. It also supports proactive caching, which enables you to combine the characteristics of ROLAP and MOLAP storage for both immediacy of data and query performance. For more information, see [Proactive Caching (Partitions)](https://docs.microsoft.com/en-us/sql/analysis-services/multidimensional-models-olap-logical-cube-objects/partitions-proactive-caching).
## MOLAP
The MOLAP storage mode causes the aggregations of the partition and a copy of its source data to be stored in a multidimensional structure in Analysis Services when the partition is processed. This MOLAP structure is highly optimized to maximize query performance. The storage location can be on the computer where the partition is defined or on another computer running Analysis Services. Because a copy of the source data resides in the multidimensional structure, queries can be resolved without accessing the partition's source data. Query response times can be decreased substantially by using aggregations. The data in the partition's MOLAP structure is only as current as the most recent processing of the partition.
As the source data changes, objects in MOLAP storage must be processed periodically to incorporate those changes and make them available to users. Processing updates the data in the MOLAP structure, either fully or incrementally. The time between one processing and the next creates a latency period during which data in OLAP objects may not match the source data. You can incrementally or fully update objects in MOLAP storage without taking the partition or cube offline. However, there are situations that may require you to take a cube offline to process certain structural changes to OLAP objects. You can minimize the downtime required to update MOLAP storage by updating and processing cubes on a staging server and using database synchronization to copy the processed objects to the production server. You can also use proactive caching to minimize latency and maximize availability while retaining much of the performance advantage of MOLAP storage. For more information, see [Proactive Caching (Partitions)](https://docs.microsoft.com/en-us/sql/analysis-services/multidimensional-models-olap-logical-cube-objects/partitions-proactive-caching), [Synchronize Analysis Services Databases](https://docs.microsoft.com/en-us/sql/analysis-services/multidimensional-models/synchronize-analysis-services-databases), and [Processing a multidimensional model (Analysis Services)](https://docs.microsoft.com/en-us/sql/analysis-services/multidimensional-models/processing-a-multidimensional-model-analysis-services).
## ROLAP
The ROLAP storage mode causes the aggregations of the partition to be stored in indexed views in the relational database that was specified in the partition's data source. Unlike the MOLAP storage mode, ROLAP does not cause a copy of the source data to be stored in the Analysis Services data folders. Instead, when results cannot be derived from the query cache, the indexed views in the data source is accessed to answer queries. Query response is generally slower with ROLAP storage than with the MOLAP or HOLAP storage modes. Processing time is also typically slower with ROLAP. However, ROLAP enables users to view data in real time and can save storage space when you are working with large datasets that are infrequently queried, such as purely historical data.
##### Note
When using ROLAP, Analysis Services may return incorrect information related to the unknown member if a join is combined with a GROUP BY clause. Analysis Services eliminates relational integrity errors instead of returning the unknown member value.
If a partition uses the ROLAP storage mode and its source data is stored in SQL Server Database Engine, Analysis Services tries to create indexed views to contain aggregations of the partition. If Analysis Services cannot create indexed views, it does not create aggregation tables. Although Analysis Services handles the session requirements for creating indexed views on SQL Server Database Engine, the following conditions must be met by the ROLAP partition and the tables in its schema in order for Analysis Services to create indexed views for aggregations:
- 
The partition cannot contain measures that use the Min or Max aggregate functions.
- 
Each table in the schema of the ROLAP partition must be used only one time. For example, the schema cannot contain [dbo].[address] AS "Customer Address" and [dbo].[address] AS "SalesRep Address".
- 
Each table must be a table, not a view.
- 
All table names in the partition's schema must be qualified with the owner name, for example, [dbo].[customer].
- 
All tables in the partition's schema must have the same owner; for example, you cannot have a FROM clause that references the tables [tk].[customer], [john].[store], and [dave].[sales_fact_2004].
- 
The source columns of the partition's measures must not be nullable.
- 
All tables used in the view must have been created with the following options set to ON:
- 
ANSI_NULLS
- 
QUOTED_IDENTIFIER
- 
The total size of the index key, in SQL Server Database Engine, cannot exceed 900 bytes. SQL Server Database Engine will assert this condition based on the fixed length key columns when the CREATE INDEX statement is processed. However, if there are variable length columns in the index key, SQL Server Database Engine will also assert this condition for every update to the base tables. Because different aggregations have different view definitions, ROLAP processing using indexed views can succeed or fail depending on the aggregation design.
- 
The session creating the indexed view must have the following options set to ON: ARITHABORT, CONCAT_NULL_YEILDS_NULL, QUOTED_IDENTIFIER, ANSI_NULLS, ANSI_PADDING, and ANSI_WARNING. This setting can be made in SQL Server Management Studio.
- 
The session creating the indexed view must have the following option set to OFF: NUMERIC_ROUNDABORT. This setting can be made in SQL Server Management Studio.
## HOLAP
The HOLAP storage mode combines attributes of both MOLAP and ROLAP. Like MOLAP, HOLAP causes the aggregations of the partition to be stored in a multidimensional structure in an SQL Server Analysis Services instance. HOLAP does not cause a copy of the source data to be stored. For queries that access only summary data in the aggregations of a partition, HOLAP is the equivalent of MOLAP. Queries that access source data—for example, if you want to drill down to an atomic cube cell for which there is no aggregation data—must retrieve data from the relational database and will not be as fast as they would be if the source data were stored in the MOLAP structure. With HOLAP storage mode, users will typically experience substantial differences in query times depending upon whether the query can be resolved from cache or aggregations versus from the source data itself.
Partitions stored as HOLAP are smaller than the equivalent MOLAP partitions because they do not contain source data and respond faster than ROLAP partitions for queries involving summary data. HOLAP storage mode is generally suited for partitions in cubes that require rapid query response for summaries based on a large amount of source data. However, where users generate queries that must touch leaf level data, such as for calculating median values, MOLAP is generally a better choice.

