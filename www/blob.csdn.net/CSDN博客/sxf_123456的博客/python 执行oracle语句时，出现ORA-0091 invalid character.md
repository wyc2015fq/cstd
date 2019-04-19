# python 执行oracle语句时，出现ORA-0091:invalid character - sxf_123456的博客 - CSDN博客
2018年11月27日 16:58:30[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：76标签：[python](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[python](https://blog.csdn.net/sxf_123456/article/category/7078196)
del_sqls = [
    "delete from  TP_COPREGION_GIS_zh where gwm_geometry is null;",
    "delete from  GRID_AREA_GIS_zh where gwm_geometry is null;",
    "delete from  tp_grid_zh where gwm_geometry is null;"
]
oracle_hook.run(del_sqls, autocommit=True)
出现ORA-0091:invalid character 错误的原因是sql语句后边有分号。在mysql中可以使用分号结束，但是在Oracle中结尾不需要使用分号。
