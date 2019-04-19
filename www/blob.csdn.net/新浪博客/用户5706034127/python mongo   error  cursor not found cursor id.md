# python mongo   error: cursor not found cursor id_用户5706034127_新浪博客
||分类：[python](http://blog.sina.com.cn/s/articlelist_5706034127_2_1.html)|

Cursors in MongoDB can timeout on the server if they’ve been open
for a long time without any operations being performed on them.
This can lead to an CursorNotFound exception being raised when
attempting to iterate the cursor.							
		
