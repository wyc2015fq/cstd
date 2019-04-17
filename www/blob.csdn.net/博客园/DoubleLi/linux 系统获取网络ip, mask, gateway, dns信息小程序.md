# linux 系统获取网络ip, mask, gateway, dns信息小程序 - DoubleLi - 博客园






net_util.c



       #define WIRED_DEV                   "eth0" 
    #define WIRELESS_DEV                "ra0"           
    #define PPPOE_DEV                   "ppp0"

#define DEBUG_PRT(fmt, arg...)      printf(fmt,##arg)

/**
 * get ip address.
 * @param net_dev net device.
 * @param ipaddr a pointer to save ip address.
 * @return 0 success, or fail.
 */
int get_ipaddr(const char *net_dev,  char *ipaddr)
{
    struct ifreq ifr;
    int fd = 0;
    struct sockaddr_in *pAddr;

    if((NULL == net_dev) || (NULL == ipaddr))
    {
        DEBUG_PRT("illegal call function SetGeneralIP! \n");
        return -1;
    }

    if((fd = socket(AF_INET,SOCK_DGRAM,0)) < 0)
    {
        DEBUG_PRT("open socket failed \n");
        return -1;
    }

    memset(&ifr,0,sizeof(ifr));
    strcpy(ifr.ifr_name, net_dev);

    if(ioctl(fd, SIOCGIFADDR, &ifr) < 0)
    {
        DEBUG_PRT("SIOCGIFADDR socket failed \n");
        close(fd);
        return -1;
    }

    pAddr = (struct sockaddr_in *)&(ifr.ifr_addr);

    strcpy(ipaddr, inet_ntoa(pAddr->sin_addr));

    close(fd);

    return 0;
}

/**
 * get gateway.
 * @param gateway a pointer to save geteway.
 * @return none.
 */
void get_gateway(ITI_CHAR *gateway)
{
    char buf[1024]={0};
    char *p = NULL;
    char *q = NULL;
    int count = 0;
    if(NULL == gateway)
    {
        DEBUG_PRT("gateway is NULL \n");
        return; 
    }
    cmd_run("route | grep default", buf, 1024);
    if(0 == strlen(buf))
    {
        DEBUG_PRT("get gateway error \n");
        return;
    }

    p = strstr2(buf, "default");
    q = p;
    while(*q == ' ')
    {
        q++;
    }

    p = q;
    while(*p != ' ')
    {
        p++;
        count++;
    }
    if(NULL != q)
    {
        memcpy(gateway, q, count);
    }
    gateway[count] = '\0';  
}

/**
 * get mask.
 * @param net_dev net device.
 * @param mask a pointer to save mask.
 * @return none.
 */
void get_mask(const char *net_dev, ITI_CHAR *mask)
{
    char buf[1024]={0};
    char *p = NULL;
    if(NULL == mask)
    {
        DEBUG_PRT("mask is NULL \n");
        return; 
    }
    if(0 == (memcmp(WIRED_DEV, net_dev, sizeof(WIRED_DEV))))
    {
        cmd_run("ifconfig eth0 | grep Mask", buf, 1024);
    }
    else if(0 == (memcmp(WIRELESS_DEV, net_dev, sizeof(WIRELESS_DEV))))
    {
        cmd_run("ifconfig ra0 | grep Mask", buf, 1024);
    }
    else if(0 == (memcmp(PPPOE_DEV, net_dev, sizeof(PPPOE_DEV))))
    {
        cmd_run("ifconfig ppp0 | grep Mask", buf, 1024);
    }
    else 
    {
        DEBUG_PRT("net device not support \n");
        return; 
    }

    if(0 == strlen(buf))
    {
        DEBUG_PRT("get mask error \n");
        return;
    }
    p = strstr2(buf, "Mask:");
    if(NULL == p)
    {
        DEBUG_PRT("get mask error \n");
        return;
    }
    strcpy(mask, p);
}

/**
 * get dns.
 * @param dns1 a pointer to save first dns.
 * @param dns2 a pointer to save second dns.
 * @return 0 success, or fail.
 */
int get_dns(char *dns1, char *dns2)
{
    int fd = -1;
    int size = 0;
    char strBuf[100];
    char tmpBuf[100];
    int buf_size = sizeof(strBuf);
    char *p = NULL;
    char *q = NULL;
    int i = 0;
    int j = 0;
    int count = 0;

    fd = open("/etc/resolv.conf", O_RDONLY);
    if(-1 == fd)
    {
        DEBUG_PRT("%s open error \n", __func__);
        return -1;
    }
    size = read(fd, strBuf, buf_size);
    if(size < 0)
    {
        DEBUG_PRT("%s read file len error \n", __func__);
        close(fd);
        return -1;
    }
    strBuf[buf_size] = '\0';
    close(fd);

    while(i < buf_size)
    {
        if((p = strstr2(&strBuf[i], "nameserver")) != NULL)
        {
            j++;
            p += 1;
            count = 0;

            memset(tmpBuf, 0xff, 100);
            memcpy(tmpBuf, p, 100);
            tmpBuf[sizeof(tmpBuf) -1 ] = '\0';

            q = p;
            while(*q != '\n')
            {
                q++;
                count++;
            }
            i += (sizeof("nameserver") + count);

            if(1 == j)
            {
                memcpy(dns1, p, count);
                dns1[count]='\0';
            }
            else if(2 == j)
            {
                memcpy(dns2, p, count);
                dns2[count]='\0';
            }
        } 
        else 
        {
            i++;
        }
    }

    return 0;
}



main.c

void main(void)

{

        #define WIRED_DEV                   "eth0" 
    #define WIRELESS_DEV                "ra0"           
    #define PPPOE_DEV                   "ppp0"

        char buf[1024] = {'\0'};

    extern int get_ipaddr(const char *net_dev,  char *ipaddr);
    get_ipaddr(WIRED_DEV, buf);
    printf("get_ipaddr: %s \n", buf);
    memset(buf, '\0', sizeof(buf));

    extern void get_mask(const char *net_dev, ITI_CHAR *mask);
    get_mask(WIRED_DEV, buf);
    printf("get_mask: %s \n", buf);
    memset(buf, '\0', sizeof(buf));

    extern void get_gateway(ITI_CHAR *gateway);
    get_gateway(buf);
    printf("get_gateway: %s \n", buf);
    memset(buf, '\0', sizeof(buf));

    extern int get_dns(char *dns1, char *dns2);
    get_dns(buf, &buf[100]);
    printf("get_dns: %s %s\n", buf, &buf[100]);
    memset(buf, '\0', sizeof(buf));

}



makefile:

gcc main.c net_util.c -o get_net_info



./get_net_info

运行结果:

get_ipaddr: 192.168.9.142 
get_mask: 255.255.255.0
get_gateway: 192.168.9.254 
get_dns: 192.168.9.11 192.168.9.10





为提高执行效率, 更新获取网关和子网掩码程序:

static void get_gateway(const char *net_dev, char *gateway)
{
    FILE *fp;    
    char buf[1024];  
    char iface[16];    
    unsigned char tmp[100]={'\0'};
    unsigned int dest_addr=0, gate_addr=0;
    if(NULL == gateway)
    {
        DEBUG_PRT("gateway is NULL \n");
        return; 
    }
    fp = fopen("/proc/net/route", "r");    
    if(fp == NULL){  
        DEBUG_PRT("fopen error \n");
        return;   
    }

    fgets(buf, sizeof(buf), fp);    
    while(fgets(buf, sizeof(buf), fp)) 
    {    
        if((sscanf(buf, "%s\t%X\t%X", iface, &dest_addr, &gate_addr) == 3) 
            && (memcmp(net_dev, iface, strlen(net_dev)) == 0)
            && gate_addr != 0) 
        {
                memcpy(tmp, (unsigned char *)&gate_addr, 4);
                sprintf(gateway, "%d.%d.%d.%d", (unsigned char)*tmp, (unsigned char)*(tmp+1), (unsigned char)*(tmp+2), (unsigned char)*(tmp+3));
                break;    
        }
    }    

    fclose(fp);
}

static void get_mask(const char *net_dev, ITI_CHAR *mask)
{
    struct sockaddr_in *pAddr;
    struct ifreq ifr;
    int sockfd;

    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    memset(&ifr,0,sizeof(ifr));
    strcpy(ifr.ifr_name, net_dev);

    if(ioctl(sockfd, SIOCGIFNETMASK, &ifr) < 0){
        DEBUG_PRT("SIOCGIFADDR socket failed \n");
        close(sockfd);
        return ;
    }

    pAddr = (struct sockaddr_in *)&(ifr.ifr_addr);
    strcpy(mask, (char *)(inet_ntoa(pAddr->sin_addr)));

    close(sockfd);
}









