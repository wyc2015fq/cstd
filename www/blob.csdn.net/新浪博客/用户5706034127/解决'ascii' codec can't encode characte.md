# 解决'ascii' codec can't encode characte_用户5706034127_新浪博客
|||
指定文件字符集为utf-8
在文件头部加入以下代码：
import sys
reload(sys)
sys.setdefaultencoding('utf-8')
