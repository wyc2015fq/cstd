# mysql 批量查询 - sxf_123456的博客 - CSDN博客
2019年03月09日 15:00:29[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：85
sql = "SELECT area_id,area_name FROM `t_area_info` WHERE area_id IN ({mark})".format(
        mark=','.join(["%s"] * 1000))
 with conn.cursor() as cursor:
    new_sql = sql % tuple(value_list)  #需要将value_list 转化为tuple
    cursor.execute(new_sql)
