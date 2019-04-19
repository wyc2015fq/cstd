# python 实现分页显示从es中获取的数据 - sxf_123456的博客 - CSDN博客
2017年08月18日 10:36:45[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：1534
**注意:使用该方法，获取的数据总数目不能超过1万，否则出错****#在python3上运行****from **elasticsearch **import  **Elasticsearch
**from **urllib3.connectionpool **import **xrange
**def **get_page_data(result):
    **for **hit **in **result[**'hits'**][**'hits'**]:
        print(hit)
**if **__name__==**'__main__'**:
    es_host = **"0.0.0.0"**port = 9200
timeout = 15000
index = **"gather-v10"**es = Elasticsearch(hosts=es_host,port=port,timeout=timeout)
    *# gather-v10 总条数*count = es.count(index=index)[**'count'**]
    *# 每页显示条数*page_line  = 2
*#显示多少页***if **(count%page_line==0):
        page = (int)(count/page_line)
    **else**:
        page = (int)(count/page_line+1)
    *# 要生成很大的数字序列的时候，**    # 用xrange会比range性能优很多，**    # 因为不需要一上来就开辟一块很大的内存空间。**    # x = range(0,10);type(x) 是一个列表**    # x1 = xrange(0,10);type(x1) 是一个生成器 xrange(0,10)***for **x **in **xrange(0,page):
        rs = es.search(index=index,body={
            **"query"**:{
                **"match_all"**:{}
            },
            **"from"**:x*page_line,
            **"size"**:page_line
        })
        get_page_data(rs)
