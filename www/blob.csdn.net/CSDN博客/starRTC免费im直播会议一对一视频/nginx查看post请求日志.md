# nginx查看post请求日志 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月24日 16:22:01[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：2200
在http段加上
log_format access '$remote_addr - $remote_user [$time_local] "$request" $status $body_bytes_sent $request_body "$http_referer" "$http_user_agent" $http_x_forwarded_for';
在server段加上
access_log logs/post.access.log access;
