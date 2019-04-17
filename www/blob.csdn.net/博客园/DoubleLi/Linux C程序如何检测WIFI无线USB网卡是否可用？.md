# Linux C程序如何检测WIFI无线USB网卡是否可用？ - DoubleLi - 博客园






最新做一个WIFI应用项目。如何检测WIFI USB设备是否插上了呢？特此共享。

      第一种方法，采用读取文件的方式。在linux下，任何一种设备都可看成文件。通过分析相关文件信息，可得知WIFI设备是否存在；代码示例如下：



static void WIFI_Enum_Device(void)
{
    char  buff[1024];
    FILE * fh;



    /* Check if /proc/net/wireless is available */
    fh = fopen(PROC_NET_WIRELESS, "r");


    if(fh != NULL)
    {
        /* Success : use data from /proc/net/wireless */

        /* Eat 2 lines of header */
        fgets(buff, sizeof(buff), fh);
        fgets(buff, sizeof(buff), fh);

        /* Read each device line */
        while(fgets(buff, sizeof(buff), fh))
        {
            char name[IFNAMSIZ + 1];
            char *s;

            /* Skip empty or almost empty lines. It seems that in some
            * cases fgets return a line with only a newline. */
            if((buff[0] == '\0') || (buff[1] == '\0'))
                continue;

            /* Extract interface name */
            s = WIFI_Get_DeviceName(name, sizeof(name), buff);

            if(!s)
            {
            /* Failed to parse, complain and continue */
#ifndef IW_RESTRIC_ENUM
                fprintf(stderr, "Cannot parse " PROC_NET_DEV "\n");
#else
                fprintf(stderr, "Cannot parse " PROC_NET_WIRELESS "\n");
#endif
            }
            else
                /* Got it, save the name about this interface */

            {//we always use the first detected device when doing first time detecting
                if(s_DeviceCount == 0)
                {
                    if(strcmp(s_Deviceinfo.DeviceName,name))
                    {
                        memset((char *)&s_Deviceinfo, 0, sizeof(DeviceInfo_t));

                        memcpy(s_Deviceinfo.DeviceName,name,IFNAMSIZ);
                    }
                    if(strlen(s_SavedDevice) == 0)//this is the first detected device when doing first time detecting, we save it
                        memcpy(s_SavedDevice,name,IFNAMSIZ);
                }
                else
                {//there is more than one device, we should use the first detected
                    if(!strcmp(s_SavedDevice,name))
                    {
                        memset((char *)&s_Deviceinfo, 0, sizeof(DeviceInfo_t));

                        memcpy(s_Deviceinfo.DeviceName,name,IFNAMSIZ);
                    }
                }
                s_DeviceCount++;
            }
        }

        fclose(fh);
    }

}



static char* WIFI_Get_DeviceName(char * name, /* Where to store the name */
       int nsize, /* Size of name buffer */
       char * buf) /* Current position in buffer */
{
    char * end;

    /* Skip leading spaces */
    while(isspace(*buf))
        buf++;

#ifndef IW_RESTRIC_ENUM
    /* Get name up to the last ':'. Aliases may contain ':' in them,
    * but the last one should be the separator */
    end = strrchr(buf, ':');
#else
    /* Get name up to ": "
    * Note : we compare to ": " to make sure to process aliased interfaces
    * properly. Doesn't work on /proc/net/dev, because it doesn't guarantee
    * a ' ' after the ':'*/
    end = strstr(buf, ": ");
#endif

    /* Not found ??? To big ??? */
    if((end == NULL) || (((end - buf) + 1) > nsize))
        return(NULL);

    /* Copy */
    memcpy(name, buf, (end - buf));
    name[end - buf] = '\0';

    /* Return value currently unused, just make sure it's non-NULL */
    return(end);
}



RETURN_TYPE APP_WIFI_DetectDevice(void)
{
    char command[50] = {'\0'};
    s_DeviceCount = 0;  //reset count
    WIFI_Enum_Device();
    s_LastDeviceCount = s_DeviceCount;
    if(s_DeviceCount > 0)
    {
        sprintf(command,"ifconfig %s up",s_Deviceinfo.DeviceName);
        system(command);  //boot up the device firstly
        return SYS_NOERROR;
    }
    else
        return SYS_FAILED;        
}

      第二种方法，利用linux ioctl函数读取I/O接口的相关信息。



/*****************************************************************************
*  Name        : trid_char * APP_NetLink_GetIFFLAGS(char *NetDev )
*  Description : Get net interface IFFLAGS
*  Params      : NetDev
*  Returns     : the string of the NetDev
*  Author/date : Danny.Hu /2011.11.16
*****************************************************************************/
RETURN_TYPE APP_NetLink_GetIFFlags( trid_char *NetDev )
{
    int fd = -1;
    int InterfaceFlags;
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, NetDev);
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) 
    {
        printf("Cannot get control socket\n");
        close(fd);
        return SYS_FAILED;
    }
    else if( 0!=(ioctl(fd, SIOCGIFFLAGS, (char*)&ifr)) )
    {
       printf("Cannot get Network Interface Flags!\n");
       close(fd);
       return SYS_FAILED;
    }

    InterfaceFlags = ifr.ifr_flags; 

    printf("<");
    if ( InterfaceFlags & IFF_UP)                        printf("Network %s is UP, ", NetDev);
    if ( InterfaceFlags & IFF_BROADCAST)     printf("Network %s is BCAST, ", NetDev);
    if ( InterfaceFlags & IFF_MULTICAST)      printf("Network %s is MCAST, ", NetDev);
    if ( InterfaceFlags & IFF_LOOPBACK)       printf("Network %s is LOOP, ", NetDev);
    if ( InterfaceFlags & IFF_POINTOPOINT)   printf("Network %s is P2P, ", NetDev);
    printf(">\n");
    close(fd);
    return  SYS_NOERROR;
}









