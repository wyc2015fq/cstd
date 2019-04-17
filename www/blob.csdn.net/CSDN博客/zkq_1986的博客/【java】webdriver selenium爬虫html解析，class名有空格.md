# 【java】webdriver selenium爬虫html解析，class名有空格 - zkq_1986的博客 - CSDN博客





2018年10月01日 19:01:46[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：164








```
WebElement S_main = driver.findElement(By.className("S_main"));
WebElement plc_main = S_main.findElement(By.id("plc_main"));
WebElement S_content_clearfix = plc_main.findElement(By
		.className("S_content"));
WebElement pl_weibo_direct = S_content_clearfix.findElement(By
		.id("pl_weibo_direct"));
WebElement search_feed = pl_weibo_direct.findElement(By
		.className("search_feed"));
WebElement feed_lists_W_texta = search_feed.findElement(By.xpath("//div[@class='feed_lists W_texta']"));
```





