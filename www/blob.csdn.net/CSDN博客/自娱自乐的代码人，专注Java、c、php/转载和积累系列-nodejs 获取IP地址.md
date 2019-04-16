# 转载和积累系列 - nodejs 获取IP地址 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年09月11日 20:17:17[initphp](https://me.csdn.net/initphp)阅读数：3405








```java
/*	
	 *	说明：获取客户端IP地址
	 * 	使用：
	 *	initnode.request.getClientIp();
	 */
	this.getClientIp = function () {
    	var ipAddress;
    	var forwardedIpsStr = this.headers('x-forwarded-for'); 
    	if (forwardedIpsStr) {
        	var forwardedIps = forwardedIpsStr.split(',');
        	ipAddress = forwardedIps[0];
    	}
    	if (!ipAddress) {
        	ipAddress = req.connection.remoteAddress;
    	}
    	return ipAddress;
	};
```




