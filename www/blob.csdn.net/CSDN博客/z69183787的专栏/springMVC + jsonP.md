# springMVC + jsonP - z69183787的专栏 - CSDN博客
2016年05月10日 15:25:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：534
个人分类：[SpringMvc																[跨域-Jsonp](https://blog.csdn.net/z69183787/article/category/2175491)](https://blog.csdn.net/z69183787/article/category/2176803)

**[java]**[view
 plain](http://blog.csdn.net/d8111/article/details/45249871#)[copy](http://blog.csdn.net/d8111/article/details/45249871#)
- /**
-  * 根据分类id，取新闻列表  jsonp对象
-  * @param stub
-  * @param newsItemId rss分类id编号
-  * @param function  jsonp回调函数名
-  * @return 
-  * @return  jsonp对象
-  */
- @RequestMapping(value ="/news/l")  
- @ResponseBody
- public JSONPObject newsList(long newsItemId, String function) throws IOException{  
-     List<RssNews> rssNews = newsService.searchNewsByRssId(newsItemId, EnumConstans.NEWS_PAGE_NUM);  
- returnnew JSONPObject(function, rssNews);  
- }  
 跟json返回的做法一样，使用@ResponseBody ，jsonp也是一种json
使用org.codehaus.jackson.map.util.JSONPObject 组装对象
实际也就是  function +( +json +) 的格式。
