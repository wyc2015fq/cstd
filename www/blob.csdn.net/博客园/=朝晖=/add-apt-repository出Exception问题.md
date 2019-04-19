# add-apt-repository出Exception问题 - =朝晖= - 博客园
# [add-apt-repository出Exception问题](https://www.cnblogs.com/dhcn/p/7100301.html)
参考：[http://blog.sina.com.cn/s/blog_5388923c0100nu8h.html](http://blog.sina.com.cn/s/blog_5388923c0100nu8h.html)
症状：
xxxx@xxxxx:~$ sudo add-apt-repository ppa:nikount/orta-desktop
Exception in thread Thread-1:
Traceback (most recent call last):
  File "/usr/lib/python2.6/threading.py", line 532, in __bootstrap_inner
    self.run()
  File "/usr/lib/python2.6/dist-packages/softwareproperties/ppa.py", line 63, in run
    self.add_ppa_signing_key(self.ppa_path)
  File "/usr/lib/python2.6/dist-packages/softwareproperties/ppa.py", line 92, in add_ppa_signing_key
    print "Error reading %s: %s" % (lp_url, e)
UnicodeDecodeError: 'ascii' codec can't decode byte 0xe8 in position 27: ordinal not in range(128)
解决：
说是代理问题，如果在局域网上网的同志估计没招了，虽然出这个提示，但是实际源已经添加了，但是PGP没有添加，所以再用pgp指令添加pgp即可：
```
gpg --keyserver subkeys.pgp.net --recv 6E871C4A881574DE  
gpg --export --armor 6E871C4A881574DE | sudo apt-key add -
```

