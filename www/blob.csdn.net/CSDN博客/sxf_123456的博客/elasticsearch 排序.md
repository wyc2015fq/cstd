# elasticsearch 排序 - sxf_123456的博客 - CSDN博客
2018年03月16日 13:13:04[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：74标签：[elk](https://so.csdn.net/so/search/s.do?q=elk&t=blog)
个人分类：[ELK](https://blog.csdn.net/sxf_123456/article/category/7036606)
[elasticsearch之排序](https://segmentfault.com/a/1190000004433102)
GET gather-034-20180316/_search
{
  "query": {
    "match_all": {}
  },
  "sort": [
    {
      "flr": {
        "order": "desc"
      }
    }
  ]
}
