# 获得本机多个ip地址(源码) - nosmatch的专栏 - CSDN博客
2012年09月04日 17:05:16[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：808标签：[struct																[socket																[up](https://so.csdn.net/so/search/s.do?q=up&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)
个人分类：[C++](https://blog.csdn.net/HDUTigerkin/article/category/1142667)
```cpp
vector<string> get_local_ip()
{
	int s;
	struct ifconf conf;
	struct ifreq *ifr;
	char buff[BUFSIZ];
	int num;
	int i;
	vector<string> ip_vec;
	s = socket(PF_INET, SOCK_DGRAM, 0);
	conf.ifc_len = BUFSIZ;
	conf.ifc_buf = buff;
	ioctl(s, SIOCGIFCONF, &conf);
	num = conf.ifc_len / sizeof(struct ifreq);
	ifr = conf.ifc_req;
	for(i=0;i < num;i++) {
		struct sockaddr_in *sin = (struct sockaddr_in *)(&ifr->ifr_addr);
		ioctl(s, SIOCGIFFLAGS, ifr);
		if(((ifr->ifr_flags & IFF_LOOPBACK) == 0) && (ifr->ifr_flags & IFF_UP)) {
			ip_vec.push_back(inet_ntoa(sin->sin_addr));
		}
		ifr++;
	}
	close(s);
	return ip_vec;
}
```
不解释.....
