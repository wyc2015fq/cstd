# 开源网络爬虫WebCollector的demo - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年02月03日 16:15:03[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3899








1、环境：jdk7+eclipse mars

2、WebCollector开源网址https://github.com/CrawlScript/WebCollector

      下载[webcollector-2.26-bin.zip](https://github.com/CrawlScript/WebCollector/blob/master/webcollector-2.26-bin.zip)，解压文件夹引入所有jar包到工程。

3、demo源码：



```java
/**
 * Demo of crawling web by webcollector 
 * @author fjs
 */
package com;
import cn.edu.hfut.dmic.webcollector.model.CrawlDatums;
import cn.edu.hfut.dmic.webcollector.model.Page;
import cn.edu.hfut.dmic.webcollector.plugin.berkeley.BreadthCrawler;
import org.jsoup.nodes.Document;

public class demo extends BreadthCrawler {
	/**
     * @param crawlPath crawlPath is the path of the directory which maintains
     * information of this crawler
     * @param autoParse if autoParse is true,BreadthCrawler will auto extract
     * links which match regex rules from page
     */
    public demo(String crawlPath, boolean autoParse) {
        super(crawlPath, autoParse);
        /*start page*/
        this.addSeed("http://guangzhou.qfang.com");
        /*fetch url like the value by setting up RegEx filter rule */
        this.addRegex(".*");
        /*do not fetch jpg|png|gif*/
        this.addRegex("-.*\\.(jpg|png|gif).*");
        /*do not fetch url contains #*/
        this.addRegex("-.*#.*");
    }
    @Override
    public void visit(Page page, CrawlDatums next) {
    	
        String url = page.getUrl();
        Document doc = page.getDoc();
        System.out.println(url);
        System.out.println(doc.title());
        
        /*If you want to add urls to crawl,add them to nextLink*/
        /*WebCollector automatically filters links that have been fetched before*/
        /*If autoParse is true and the link you add to nextLinks does not match the regex rules,the link will also been filtered.*/
        //next.add("http://gz.house.163.com/");
    }
    
    public static void main(String[] args) throws Exception {
        demo crawler = new demo("path", true);
        crawler.setThreads(50);
        crawler.setTopN(100);
        //crawler.setResumable(true);
        /*start crawl with depth 3*/
        crawler.start(3);
    }
}
```


4、实际应用中，对page进行解析抓取网页内容。



