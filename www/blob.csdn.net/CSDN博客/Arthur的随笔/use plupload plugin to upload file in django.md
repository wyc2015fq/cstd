# use plupload plugin to upload file in django  - Arthur的随笔 - CSDN博客
2010年07月29日 23:37:00[largetalk](https://me.csdn.net/largetalk)阅读数：1439
# django upload file needs 
# "Note that FILES will only contain data if the request method was POST and the <form> that posted to the request had enctype="multipart/form-data". Otherwise, FILES will be a blank dictionary-like object." 
**so only need add "multipart : 'form-data'," option to plupload setup function**
**see more configuration at: **
[**http://www.plupload.com/documentation.php**](http://www.plupload.com/documentation.php)
# [](http://www.plupload.com/documentation.php)
