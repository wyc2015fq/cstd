
# Javascript 中 include 文件 - cocoonyang的专栏 - CSDN博客


2017年08月04日 09:31:59[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：572




https://stackoverflow.com/questions/5797852/in-node-js-how-do-i-include-functions-from-my-other-files


```python
var fs = require("fs");
function read(f) {
  return fs.readFileSync(f).toString();
}
function include(f) {
  eval.apply(global, [read(f)]);
}
include('somefile_with_some_declarations.js');
```



```python
function include(file_) {
    with (global) {
        eval(fs.readFileSync(file_) + '');
    };
};
include('somefile_with_some_declarations.js');
```
[https://docs.plm.automation.siemens.com/tdoc/nxnastran/11/help/\#uid:index_dmap:xid666580:id496821](https://docs.plm.automation.siemens.com/tdoc/nxnastran/11/help/#uid:index_dmap:xid666580:id496821)


