# apache mod_deflate, mod_expires config - Arthur的随笔 - CSDN博客
2010年08月24日 13:03:00[largetalk](https://me.csdn.net/largetalk)阅读数：529标签：[apache																[access																[regex																[insert																[exe																[avi](https://so.csdn.net/so/search/s.do?q=avi&t=blog)](https://so.csdn.net/so/search/s.do?q=exe&t=blog)](https://so.csdn.net/so/search/s.do?q=insert&t=blog)](https://so.csdn.net/so/search/s.do?q=regex&t=blog)](https://so.csdn.net/so/search/s.do?q=access&t=blog)](https://so.csdn.net/so/search/s.do?q=apache&t=blog)
个人分类：[django](https://blog.csdn.net/largetalk/article/category/716448)
<location />
 # Insert filter
 SetOutputFilter DEFLATE
 # Netscape 4.x has some problems...
 BrowserMatch ^Mozilla/4 gzip-only-text/html
 # Netscape 4.06-4.08 have some more problems
 BrowserMatch ^Mozilla/4/.0[678] no-gzip
 # MSIE masquerades as Netscape, but it is fine
 # BrowserMatch /bMSIE !no-gzip !gzip-only-text/html
 # NOTE: Due to a bug in mod_setenvif up to Apache 2.0.48
 # the above regex won't work. You can use the following
 # workaround to get the desired effect:
 BrowserMatch /bMSI[E] !no-gzip !gzip-only-text/html
 # Don't compress 
 SetEnvIfNoCase Request_URI /
 /.(?:gif|jpe?g|png)$ no-gzip dont-vary
 SetEnvIfNoCase Request_URI .(?:exe|t?gz|zip|bz2|sit|rar)$ no-gzip dont-vary
 SetEnvIfNoCase Request_URI .(?:pdf|mov|avi|mp3|mp4|rm)$ no-gzip dont-vary
 #compress
 AddOutputFilterByType DEFLATE text/*
 AddOutputFilterByType DEFLATE application/ms* application/vnd* application/postscript application/javascript application/x-javascript
 AddOutputFilterByType DEFLATE application/x-httpd-php application/x-httpd-fastphp
 # Make sure proxies don't deliver the wrong content
 Header append Vary User-Agent env=!dont-vary
</Location> 
#######################################################################################################
<IfModule expires_module>
ExpiresActive On 
    ExpiresDefault A604800 
    ExpiresByType text/css M259200 
    ExpiresByType application/x-javascript M300 
    ExpiresByType image/gif "access plus 2 month" 
    ExpiresByType image/jpeg "access plus 2 month" 
    ExpiresByType image/png "access plus 2 month" 
    ExpiresByType image/x-icon "access plus 2 month" 
    ExpiresByType application/x-shockwave-flash A2592000 
</IfModule>

