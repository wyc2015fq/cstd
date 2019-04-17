# AWS Nginx Started but not Serving AWS上Nginx服务器无法正常工作 - Grandyang - 博客园







# [AWS Nginx Started but not Serving AWS上Nginx服务器无法正常工作](https://www.cnblogs.com/grandyang/p/10125620.html)







After install the Nginx on AWS instance, and visit your public ip address, you might see the following:

![](https://img2018.cnblogs.com/blog/391947/201812/391947-20181216063350083-453533908.png)

This site can't be reached

xxx.xxx.xx take too long to respond.



Try the following steps to solve:

Go to **EC2 Dashboard** -> **NETWORK & SECURITY** -> **Security Groups**

Select your current group, and right click, choose **Edit inbound rules**:

Add two items:

HTTP - TCP - 80 - Custom - 0.0.0.0/0

HTTP - TCP - 80 - Custom - ::/0

![](https://img2018.cnblogs.com/blog/391947/201812/391947-20181216063755913-935314503.png)



Then try to visit the public ip address again, hopefully it works like this:



![](https://img2018.cnblogs.com/blog/391947/201812/391947-20181216064039862-464841862.png)



If you try to visit your subdomain, and see the following page:



![](https://img2018.cnblogs.com/blog/391947/201812/391947-20181217012321837-1266071714.png)

An error occurred.

Sorry, the page you are looking for is currently unavailable.

Please try again later.



Then try use the following command to check the log:

```
grep "error_log" /etc/nginx/* -R
tail -n 100 /var/log/nginx/error.log
```



If you see the following error:

```
(13: Permission denied) while connecting to upstream:[nginx]
```



Try the following command to fix:

```
setsebool -P httpd_can_network_connect 1
```














