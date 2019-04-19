# 批量删除table,只保留最近几天的table - sxf_123456的博客 - CSDN博客
2018年02月01日 09:48:36[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：85
个人分类：[python](https://blog.csdn.net/sxf_123456/article/category/7078196)
*#!/usr/bin/python3**"""**批量删除table,只保留最近几天的table**"""***import  **pymysql
**import  **re
**def **conn_(host=**''**,usr=**''**,passwd=**''**,db=**''**,port=3306,):
    conn = pymysql.connect(host, usr, passwd, db, port,charset=**'utf8'**)
    **return  **conn
**def **del_table(conn_,table_pre=**''**,table_suff=**'%Y%m%d'**,keep_count=3):
    date_form = **None****    if **table_suff == **"%Y%m%d"**:
        date_form = **"_(\d{4}\d{1,2}\d{1,2})$"**date_len = 8
**elif **table_suff == **"%Y-%m-%d"**:
        date_form = **"_(\d{4}-\d{1,2}-\d{1,2})$"**date_len = 10
**elif **table_suff == **"%Y%m"**:
        date_form = **"_(\d{4}\d{1,2})$"**date_len = 6
**elif **table_suff == **"%Y-%m"**:
        date_form = **"_(\d{4}-\d{1,2})$"**date_len = 7
**else**:
        **raise **Exception(**"暂时不支持其他类型的时间后缀"**)
    curs = conn_.cursor()
    curs.execute(**'SHOW TABLES'**)
    data = curs.fetchall()
    table_ = **r'%s'**%table_pre+date_form
    list_table = []
    i = 0
**for **table **in **data:
        mt = re.search(table_, table[0])
        **if **mt:
            **if **len(mt.groups()[0]) == date_len:
                list_table.append((table[0], mt.groups()[0]))
                i += 1
sorted(list_table, key=**lambda **date: date[1]) *#按照表结构后缀时间升序排序***for **j **in **range(i-keep_count):
        sql = **'DROP TABLE if exists %s'**%list_table[j][0]
        curs.execute(sql)
    curs.close()
    conn_.close()
**if **__name__ == **'__main__'**:
    table_pre = **"tree_product"**table_suff = **"%Y%m%d"***# table_suff = "%Y-%m-%d"**    # table_suff = "%Y%m"**    # table_suff = "%Y-%m"*conn=conn_(**'10.0.0.11'**,**'root'**,**'sctele@root'**,**'sxf'**,port=3306)
    del_table(conn,table_pre=table_pre,table_suff=table_suff,keep_count=1)
