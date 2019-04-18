# 获取SISC和IDE硬盘序列号 Linux完整源码 - ljx0305的专栏 - CSDN博客
2011年09月20日 09:24:29[ljx0305](https://me.csdn.net/ljx0305)阅读数：1921
**获取SISC硬盘的序列号linux完整源码**
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <scsi/sg.h>
#define SCSI_TIMEOUT 5000 
static char *device0 = "/dev/sda";
static char *device1 = "/dev/cciss/c0d0";
int hardware_serial(unsigned char* serial, int buflen);
int open_scsi_device(const char *dev);
int close_scsi_device(int fd);
int scsi_inquiry_unit_serial_number(int fd, unsigned char* serial, int buflen);
int scsi_io(int fd, unsigned char *cdb,
       unsigned char cdb_size, int xfer_dir,
       unsigned char *data, unsigned int *data_size,
       unsigned char *sense, unsigned int *sense_len);
int main()
{
 unsigned char serial[256] = {0};
 hardware_serial(serial, sizeof(serial)); 
 printf("The hardWare is %s",serial);
 return 0;
}
int hardware_serial(unsigned char* serial, int buflen)
{
 int fd = open_scsi_device(device0);
 if (fd < 0)
 {
    fd = open_scsi_device(device1);
    if(fd < 0)
    {
  return -1;
    }
 }
 scsi_inquiry_unit_serial_number(fd, serial, buflen);
 close_scsi_device(fd);
 return 0;
}
int open_scsi_device(const char *dev)
{
 int fd, vers;
 if ((fd = open(dev, O_RDWR)) < 0) {
    return -1;
 }
 if ((ioctl(fd, SG_GET_VERSION_NUM, &vers) < 0) || (vers < 30000)) {
    close(fd);
    return -1;
 }
 return fd;
}
int close_scsi_device(int fd)
{
 return close(fd);
}
int scsi_inquiry_unit_serial_number(int fd, unsigned char* serial, int buflen)
{
 unsigned char cdb[] = {0x12,0x01,0x80,0,0,0};
 unsigned int data_size = 0x00ff;
 unsigned char data[data_size];
 unsigned int sense_len = 32;
 unsigned char sense[sense_len];
 int res, pl, i;
 cdb[3] = (data_size>>8)&0xff;
 cdb[4] = data_size&0xff;
 res = scsi_io(fd, cdb, sizeof(cdb), SG_DXFER_FROM_DEV, data, &data_size, sense, &sense_len);
 if(res) {
    return -1;
 }
 if(sense_len){
    return -1;
 }
 /* Page Length */
 pl = data[3];
 memcpy(serial, data + 4, pl+4);
 return 0;
}
int scsi_io(int fd, unsigned char *cdb,
       unsigned char cdb_size, int xfer_dir,
       unsigned char *data, unsigned int *data_size,
       unsigned char *sense, unsigned int *sense_len)
{
 sg_io_hdr_t io_hdr;
 memset(&io_hdr, 0, sizeof(sg_io_hdr_t));
 io_hdr.interface_id = 'S';
 /* CDB */
 io_hdr.cmdp = cdb;
 io_hdr.cmd_len = cdb_size;
 /* Where to store the sense_data, if there was an error */
 io_hdr.sbp = sense;
 io_hdr.mx_sb_len = *sense_len;
 *sense_len=0;
 /* Transfer direction, either in or out. Linux does not yet
 support bidirectional SCSI transfers ?
 */
 io_hdr.dxfer_direction = xfer_dir;
 /* Where to store the DATA IN/OUT from the device and how big the
 buffer is
 */
 io_hdr.dxferp = data;
 io_hdr.dxfer_len = *data_size;
 /* SCSI timeout in ms */
 io_hdr.timeout = SCSI_TIMEOUT;
 if(ioctl(fd, SG_IO, &io_hdr) < 0)
 {
    return -1;
 }
 /* now for the error processing */
 if((io_hdr.info & SG_INFO_OK_MASK) != SG_INFO_OK){
    if(io_hdr.sb_len_wr > 0){
  *sense_len=io_hdr.sb_len_wr;
  return 0;
    }
 }
 if(io_hdr.masked_status){
    return -2;
 }
 if(io_hdr.host_status){
    return -3;
 }
 if(io_hdr.driver_status){
    return -4;
 }
 return 0;
}
**获取IDE硬盘的序列号linux完整源码**
 #include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/hdreg.h>
#include <sys/ioctl.h>
#include <unistd.h>
static void dump_identity (const struct hd_driveid *id)
{
        printf("\n Model=%.40s, FwRev=%.8s, SerialNo=%.20s",
                id->model, id->fw_rev, id->serial_no);
}
int main(int argc,char **argv)
{
        struct hd_driveid id;
        char *devname = "/dev/sda";
        int fd = -1;
        fd   =   open(devname,   O_RDONLY|O_NONBLOCK);
        if (fd < 0)
        {
                perror(devname);
                exit(errno);
        }
        if (!ioctl(fd,HDIO_GET_IDENTITY, &id))
        {
                dump_identity(&id);
        }
        else
        {
            printf("Error Code : %d .  Error  Info %s \n", errno,strerror(errno));
        }
        close(fd);
       return 0;
}
