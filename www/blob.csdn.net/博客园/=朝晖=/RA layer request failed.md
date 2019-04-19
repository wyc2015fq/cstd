# RA layer request failed - =朝晖= - 博客园
# [RA layer request failed](https://www.cnblogs.com/dhcn/p/7105517.html)
   新整的Eclipse环境出现这个问题，细化内容是不能connect,后来想起切换Eclipse底层库的事情,然后打开Eclipse的SVN设置。把SVN Client借口由JavaHL改为PureJavaSVNKit解决问题，这个问题的根本原因和之前那个LoginCancel其实一样，只是提示不同而已。

