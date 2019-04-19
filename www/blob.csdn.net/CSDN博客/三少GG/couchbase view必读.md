# couchbase view必读 - 三少GG - CSDN博客
2014年08月31日 21:42:55[三少GG](https://me.csdn.net/scut1135)阅读数：1964
0.
# Views and Indexes
[https://github.com/couchbaselabs/docs-ng/blob/master/content/couchbase-manual-2.0/views-and-indexes.markdown](https://github.com/couchbaselabs/docs-ng/blob/master/content/couchbase-manual-2.0/views-and-indexes.markdown)
1.  
# [CouchDB view is extremely slow](http://stackoverflow.com/questions/3909566/couchdb-view-is-extremely-slow)
[http://stackoverflow.com/questions/3909566/couchdb-view-is-extremely-slow](http://stackoverflow.com/questions/3909566/couchdb-view-is-extremely-slow)
Views are only updated the next time they are read. Upon reading, it processes all the documents that have been updated (created, updated, deleted) since the last time the view was read.
So even if you're view was defined before inserting the 3890000 documents, it will be processing the 3890000 documents for the view.
From [http://wiki.apache.org/couchdb/Introduction_to_CouchDB_views](http://wiki.apache.org/couchdb/Introduction_to_CouchDB_views)
2. 
# [In CouchDB, are there ways to improve performance of the View index process?](http://stackoverflow.com/questions/9236217/in-couchdb-are-there-ways-to-improve-performance-of-the-view-index-process)
[http://stackoverflow.com/questions/9236217/in-couchdb-are-there-ways-to-improve-performance-of-the-view-index-process](http://stackoverflow.com/questions/9236217/in-couchdb-are-there-ways-to-improve-performance-of-the-view-index-process)
i would take a deeper look into the reduce function... try to use the built-in erlang functions like _sum, _count, instead of writing javascript.
complex views can take hours and more.. thats normal.
Maybe post such not too complex map/reduce.
And don't forget: indexing ALL docs is only done once after changing the view (or pushing a hole bunch of new Docs) .. All new docs are indexed incrementally.
**Use a view with &stale=ok to retrieve the "old" data instantely**.. so you do not have to wait. (but pay attention: you allways have to call a view without stale=ok to trigger the indexing process).. or better: use stale=update_after
The code you write in views is more like 
```
CREATE
 INDEX
```
 than `SELECT`.
 It should be irrelevant how long it takes, as long as the view builds keep up with the document change rate. Building
 a view is a sunk (one-time) cost.
When you query the view, that is always a binary tree scan, which operates against a static data set in logarithmic time.
 That is usually the performance people care about more (in production.)
If you are not seeing behavior like I describe, perhaps we could discuss your view functions and your general approach to your problem.
**CouchDB is very different from relational databases. In the latter, you have highly structured data and free-form queries. In CouchDB, you have free-form data but highly structured index definitions (views).**Except during development, changing and rebuilding views should be rare.
3.
4.
5. [Rows
 count of Couchbase view subset](http://stackoverflow.com/questions/18124471/rows-count-of-couchbase-view-subset)
[http://stackoverflow.com/questions/18124471/rows-count-of-couchbase-view-subset](http://stackoverflow.com/questions/18124471/rows-count-of-couchbase-view-subset)
When I query some view in Couchbase I get the response that has following structure:
```
{
  "total_rows":100,
  "rows":[...]
}
```
'total_rows' is very useful property that I can use for paging. But lets say I select only a subset of view using 'start_key' and 'end_key' and of course I don't know how big this subset will be. 'total_rows' is still the same number (as I understand it's just
 total of whole view). Is there any easy way to know how many rows was selected in subset?
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
If you have to paginate the view in the efficient way, you actually don't need to specify both start and the end.
Generally it is possible to use startkey/startkey_id and limit. In this case the limit will tell you that the page won't be bigger than known size.
Both cases are described in CouchDB book: [http://guide.couchdb.org/draft/recipes.html#pagination](http://guide.couchdb.org/draft/recipes.html#pagination)
> 
Here is how it works:
- 
Request rows_per_page + 1 rows from the view
- 
Display rows_per_page rows, store + 1 row as next_startkey and next_startkey_docid
- 
As page information, keep startkey and next_startkey
- 
Use the next_* values to create the next link, and use the others to create the previous link
6.
