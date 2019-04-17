# WinCE 读取Mac地址实践测试 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年02月13日 17:02:18[boonya](https://me.csdn.net/boonya)阅读数：4995








** 由于错误的理解WinCE 读取Mac地址就是在注册表中所以怎么读都读不到数据，在此记录测试过程。**

**1、通过注册表中读取Mac地址**



```
using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using SmartDeviceProjectWtms;
using Microsoft.Win32;
using System.Windows.Forms;

namespace SmartDeviceProjectWtms
{
    class MacReader
    {
        public enum HKEY { HKEY_LOCAL_MACHINE = 0, HKEY_CLASSES_ROOT = 1, HKEY_CURRENT_USER = 2, HKEY_USERS = 3 };

        private RegistryKey[] reg = new RegistryKey[4];

        public MacReader()
        {
            reg[(int)HKEY.HKEY_LOCAL_MACHINE] = Registry.LocalMachine;
            reg[(int)HKEY.HKEY_CLASSES_ROOT] = Registry.ClassesRoot;
            reg[(int)HKEY.HKEY_CURRENT_USER] = Registry.CurrentUser;
            reg[(int)HKEY.HKEY_USERS] = Registry.Users;
        }

        //读指定变量值
        public string ReadValue(HKEY Root, string SubKey, string ValueName)
        {
            RegistryKey subKey = reg[(int)Root];
            if (ValueName.Length == 0) return "[ERROR]";
            try
            {
                if (SubKey.Length > 0)
                {
                    string[] strSubKey = SubKey.Split('\\');
                    foreach (string strKeyName in strSubKey)
                    {
                        subKey = subKey.OpenSubKey(strKeyName);
                    }
                }
                string strKey = subKey.GetValue(ValueName).ToString();
                subKey.Close();
                return strKey;
            }
            catch(Exception ex)
            {
                MessageBox.Show(ex.Message);
                return "[ERROR]";
            }
        }

        //获取mac地址
        public static string GetMacAddr()
        {
            MacReader reg = new MacReader();
            string mac = reg.ReadValue(MacReader.HKEY.HKEY_LOCAL_MACHINE, @"Comm\DM9CE1\Parms", "NetWorkAddress").ToUpper();
            if (mac.Length == 12)
            {
                mac = mac.Substring(0, 2) + "-" + mac.Substring(2, 2) + "-" + mac.Substring(4, 2) + "-" +
                      mac.Substring(6, 2) + "-" + mac.Substring(8, 2) + "-" + mac.Substring(10, 2);
            }
            return mac;
        }
    }
}
```



注：智能设备中按照@“Comm\DM9CE1\Parms”地址读取不到所谓的mac值，无论使用“NetWorkAddress”或是“NetWorkAddress0”，应该是找不到路径所致。

**2、通过SendARP获取MAC地址**



```
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.IO;
using System.Security.Cryptography;
using System.Net; 

namespace SmartDeviceProjectWtms
{
    class SysInfoReader
    {
        private static string[] strEncrypt = new string[] { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "AA", "AB", "AC", "AD", "AE", "AF", "AG", "AH", "AI", "AJ", "AK", "AL", "AM", "AN", "AO", "AP" };

        private static Int32 METHOD_BUFFERED = 0;

        private static Int32 FILE_ANY_ACCESS = 0;

        private static Int32 FILE_DEVICE_HAL = 0x00000101;

        private const Int32 ERROR_NOT_SUPPORTED = 0x32;

        private const Int32 ERROR_INSUFFICIENT_BUFFER = 0x7A;

        private static Int32 IOCTL_HAL_GET_DEVICEID = ((FILE_DEVICE_HAL) << 16) | ((FILE_ANY_ACCESS) << 14) | ((21) << 2) | (METHOD_BUFFERED);

        [DllImport("coredll.dll", SetLastError = true)]
        private static extern bool KernelIoControl(Int32 dwIoControlCode, IntPtr lpInBuf, Int32 nInBufSize, byte[] lpOutBuf, Int32 nOutBufSize, ref   Int32 lpBytesReturned);

        [DllImport("Iphlpapi.dll", EntryPoint = "SendARP")]
        public static extern uint SendARP(uint DestIP, uint SrcIP, byte[] pMacAddr, ref uint PhyAddrLen);

        /// 获取MAC地址 
        public string GetMac()
        {
            uint ip = 0;
            string mac = string.Empty;
            //取本机IP列表 
            IPAddress[] ips = Dns.GetHostEntry(Dns.GetHostName()).AddressList;
            //取本机IP 
            byte[] ipp = ips[1].GetAddressBytes();
            ip = (uint)((ipp[0]) | (ipp[1] << 8) | (ipp[2] << 16) | (ipp[3] << 24));
            //取MAC 
            byte[] MacAddr = new byte[6];
            uint PhyAddrLen = 6;
            uint hr = SendARP(ip, 0, MacAddr, ref PhyAddrLen);
            if (MacAddr[0] != 0 || MacAddr[1] != 0 || MacAddr[2] != 0 || MacAddr[3] != 0 || MacAddr[4] != 0 || MacAddr[5] != 0)
            {
                mac = MacAddr[0].ToString("X2") + ":" + MacAddr[1].ToString("X2") + ":" + MacAddr[2].ToString("X2") + ":" + MacAddr[3].ToString("X2") + ":" + MacAddr[4].ToString("X2") + ":" + MacAddr[5].ToString("X2");
            }
            return mac;
        }

        ///获取本机IP
        public string GetIpAddress() 
        { 
            string strHostName = Dns.GetHostName(); //得到本机的主机名 
            IPHostEntry ipEntry = Dns.GetHostEntry(strHostName);//Dns.GetHostByName(strHostName); //取得本机IP ,此方法已过时
            string strAddr = ipEntry.AddressList[1].ToString(); 
            return strAddr;
        } 
    }
}
```



此法则成功获取到了mac地址。






