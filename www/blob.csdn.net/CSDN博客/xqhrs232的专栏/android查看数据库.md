# android查看数据库 - xqhrs232的专栏 - CSDN博客
2011年10月29日 12:24:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：847
原文地址::[http://blog.csdn.net/kome2000/article/details/4762238](http://blog.csdn.net/kome2000/article/details/4762238)
相关网页::Android 中查看SQLite数据库方法[http://www.liuzhaocn.com/database/android-%E4%B8%AD%E6%9F%A5%E7%9C%8Bsqlite%E6%95%B0%E6%8D%AE%E5%BA%93%E6%96%B9%E6%B3%95/](http://www.liuzhaocn.com/database/android-%E4%B8%AD%E6%9F%A5%E7%9C%8Bsqlite%E6%95%B0%E6%8D%AE%E5%BA%93%E6%96%B9%E6%B3%95/)
1、运行输入 adb shell (前提是模拟器正在运行) 
2、进入命令界面后 输入 ls 指令 会列出文件的目录 
3、cd 进入你想要的目录里 
4、一层一层进去后会发现 databases目录 你的数据文件就在这个目录下放着 
5、sqlite3 test  (test就是你创建的数据库的名称 注意：不要加.db 后缀) 
6、现在你就进入你创建的test数据库了使用  .tables 就可以查看所有的表了； 
[](http://artbase.javaeye.com/)
adb shell 
# find data -name "*.db" -print 
data/data/com.google.android.providers.contacts/databases/contacts.db 
data/data/com.google.android.providers.googleapps/databases/accounts.db 
data/data/com.google.android.providers.im/databases/im.db 
data/data/com.google.android.providers.media/databases/media.db 
data/data/com.google.android.providers.telephony/databases/mms.db 
data/data/com.google.android.providers.telephony/databases/sms.db 
data/data/com.google.android.providers.telephony/databases/telephony.db 
data/data/com.google.android.providers.settings/databases/settings.db 
data/data/com.google.android.maps/databases/maps.db 
# sqlite3 data/data/com.google.android.providers.contacts/databases/contacts.db 
sqlite3 data/data/com.google.android.providers.contacts/databases/contacts.db 
SQLite version 3.5.0 
Enter ".help" for instructions 
sqlite> 
sqlite> .tables 
sqlite> .schema phones 
.schema phones 
CREATE TABLE phones (_id INTEGER PRIMARY KEY,person INTEGER,type INTEGER,number TEXT,number_key TEXT,label TEXT);
CREATE INDEX phonesIndex1 ON phones (person); 
CREATE INDEX phonesIndex2 ON phones (number_key); 
CREATE TRIGGER phones_delete DELETE ON phones BEGIN UPDATE people SET _sync_dirty=1 WHERE people._id=old.person;END;
CREATE TRIGGER phones_insert INSERT ON phones BEGIN UPDATE people SET _sync_dirty=1 WHERE people._id=new.person;END;
CREATE TRIGGER phones_update UPDATE ON phones BEGIN UPDATE people SET _sync_dirty=1 WHERE people._id=old.person;END;
CREATE TRIGGER preferred_phone_cleanup DELETE ON phones BEGIN UPDATE people SET preferred_phone = NULL WHERE preferred_phone = old._id; END;

