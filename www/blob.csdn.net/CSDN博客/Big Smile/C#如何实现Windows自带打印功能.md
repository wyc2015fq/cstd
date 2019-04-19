# C#如何实现Windows自带打印功能 - Big Smile - CSDN博客
2017年10月29日 12:52:34[王啸tr1912](https://me.csdn.net/tr1912)阅读数：14374标签：[windows																[打印](https://so.csdn.net/so/search/s.do?q=打印&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[C#																[项目经验](https://blog.csdn.net/tr1912/article/category/6211594)](https://blog.csdn.net/tr1912/article/category/5855387)
        接着上回说，在使用打印机自带的SDK开发失利只后，经过一系列的实验，发现，这个打印机可以直接用我安装好的驱动直接进行打印操作，用word直接调整好字体之后打印是完全没有问题的，所以，推测，应该可以直接调用人家封装好的一些驱动进行打印，但是要怎么连接这些驱动呢？
# 一、打印控件
        首先我们要提到的就是在C#中的一个关于打印的控件了，叫：PrintDocument，说他是一个控件，其实也就是一个关于windows打印的属性和代码的集合而已，但是结合着windows自带的一些对话框窗体，我们可以轻而易举的做一个打印的小程序。
        我们要做的是，首先建立一个窗体，然后从工具箱中找到PrintDocument这个控件，然后添加上就可以了，可以看到控件的属性是这样的：
![](https://img-blog.csdn.net/20171029152815792?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        并没有什么特殊的地方，但是在方法里面却有一个很重要的方法，那就是他的printPage这个方法，这个就是每次触发这个控件的打印的内容核心，先贴上如下代码：
```
private void pd1_PrintPage(object sender, System.Drawing.Printing.PrintPageEventArgs e)
        {
            Graphics g = e.Graphics; //获得绘图对象
            float linesPerPage = 0; //页面的行号
            float yPosition = 0;   //绘制字符串的纵向位置
            int count = 0; //行计数器
            float leftMargin = 1; //左边距
            float topMargin = 1; //上边距
            string line = ""; //行字符串
            Font printFont = this.textBox1.Font; //当前的打印字体
            SolidBrush myBrush = new SolidBrush(Color.Black);//刷子
            linesPerPage = e.MarginBounds.Height / printFont.GetHeight(g);//每页可打印的行数
            //逐行的循环打印一页
            while (count < linesPerPage && ((line = lineReader.ReadLine()) != null))
            {
                yPosition = topMargin + (count * printFont.GetHeight(g));
                g.DrawString(line, printFont, myBrush, leftMargin, yPosition, new StringFormat());
                count++;
            }
            // 注意：使用本段代码前，要在该窗体的类中定义lineReader对象：
            //       StringReader lineReader = null;
            //如果本页打印完成而line不为空,说明还有没完成的页面,这将触发下一次的打印事件。在下一次的打印中lineReader会
            //自动读取上次没有打印完的内容，因为lineReader是这个打印方法外的类的成员，它可以记录当前读取的位置
            if (line != null)
                e.HasMorePages = true;
            else
            {
                e.HasMorePages = false;
                // 重新初始化lineReader对象，不然使用打印预览中的打印按钮打印出来是空白页
                lineReader = new StringReader(textBox1.Text); // textBox是你要打印的文本框的内容
            }
        }
```
        这里需要注意的是，使用了lineReader这个类的对象，这个对象的特点就是可以把对象中赋值的字符串按照行（以\r\n为准的换行）来进行字符串的获取，上面代码中的line就是指的获得的一行的数据。我们用system.draw的Graphics里面的绘图对象来进行基本的字符绘图，最后把绘图对象打印到我们纸上，就是这个打印里面的内容。
        所以，我们这个里面需要引入的一个引用就是using System.Drawing.Printing;   这样整个控件加代码就可以运行了，说明一下，这个打印的调用需要触发，使用的是如下代码：
```
lineReader = new StringReader(stb.ToString());   // 获取要打印的字符串
pd1.Print();      //执行打印方法
```
        另外说明一下，这里执行的时候，有一些辅助设置，比如打印预览，打印机使用的默认的，还有页面的配置等都是有专门的对话框，我把他们都放在了菜单里面，如图：
![](https://img-blog.csdn.net/20171029155605308?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
下面我们来说一下他们的实现，其实很是简单：
打印设置：
```
private void FileMenuItem_PrintSet_Click(object sender, EventArgs e)
{
   PrintDialog printDialog = new PrintDialog();
   printDialog.Document = pd1;
   printDialog.ShowDialog();
            
}
```
![](https://img-blog.csdn.net/20171029160035689?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
页面设置：
```
private void FileMenuItem_PageSet_Click(object sender, EventArgs e)
{
     PageSetupDialog pageSetupDialog = new PageSetupDialog();
     pageSetupDialog.Document = pd1;
     pageSetupDialog.ShowDialog();
}
```
![](https://img-blog.csdn.net/20171029160052575?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
打印预览：
```
private void FileMenuItem_PrintView_Click(object sender, EventArgs e)
{
    PrintPreviewDialog printPreviewDialog = new PrintPreviewDialog();
    printPreviewDialog.Document = pd1;
    lineReader = new StringReader(stb.ToString());
    try
    {
        printPreviewDialog.ShowDialog();
    }
    catch(Exception excep)
    {
        MessageBox.Show(excep.Message, "打印出错", MessageBoxButtons.OK, MessageBoxIcon.Error);
    }
}
```
![](https://img-blog.csdn.net/20171029160107800?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
     这些页面里面的设置在选择保存的时候会自动存储到Graphic对象当中，因此，可以配置之后使用。
# 二、系统API接口
        既然有系统定制的控件，那么系统的接入API肯定也少不了，不过，我目前从网络上找到的只有一小部分而已，并且都是一些获取配置，设置默认等一些列的代码，在此分享给大家
```
using System;
using System.Text;
using System.Runtime.InteropServices;
using System.Security;
using System.ComponentModel;
using System.IO;
 
namespace WindowsFormsApplication1
{
    public class PrinterHelper
    {
        private PrinterHelper(){ }  
        #region API声明
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
        internal struct structPrinterDefaults
        {
            [MarshalAs(UnmanagedType.LPTStr)]
            public String pDatatype;
            public IntPtr pDevMode;
            [MarshalAs(UnmanagedType.I4)]
            public int DesiredAccess;
        };
        [DllImport("winspool.Drv", EntryPoint = "OpenPrinter", SetLastError = true,CharSet = CharSet.Unicode, ExactSpelling = false, CallingConvention = CallingConvention.StdCall),
                    SuppressUnmanagedCodeSecurityAttribute()]
        internal static extern bool OpenPrinter([MarshalAs(UnmanagedType.LPTStr)] string printerName,out IntPtr phPrinter,ref structPrinterDefaults pd);
 
        [DllImport("winspool.Drv", EntryPoint = "ClosePrinter", SetLastError = true,CharSet = CharSet.Unicode, ExactSpelling = false,CallingConvention = CallingConvention.StdCall), SuppressUnmanagedCodeSecurityAttribute()]
        internal static extern bool ClosePrinter(IntPtr phPrinter);
 
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
        internal struct structSize
        {
            public Int32 width;
            public Int32 height;
        }
 
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
        internal struct structRect
        {
            public Int32 left;
            public Int32 top;
            public Int32 right;
            public Int32 bottom;
        }
 
        [StructLayout(LayoutKind.Explicit, CharSet = CharSet.Unicode)]
        internal struct FormInfo1
        {
            [FieldOffset(0), MarshalAs(UnmanagedType.I4)]
            public uint Flags;
            [FieldOffset(4), MarshalAs(UnmanagedType.LPWStr)]
            public String pName;
            [FieldOffset(8)]
            public structSize Size;
            [FieldOffset(16)]
            public structRect ImageableArea;
        };
 
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        internal struct structDevMode
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
            public String
             dmDeviceName;
            [MarshalAs(UnmanagedType.U2)]
            public short dmSpecVersion;
            [MarshalAs(UnmanagedType.U2)]
            public short dmDriverVersion;
            [MarshalAs(UnmanagedType.U2)]
            public short dmSize;
            [MarshalAs(UnmanagedType.U2)]
            public short dmDriverExtra;
            [MarshalAs(UnmanagedType.U4)]
            public int dmFields;
            [MarshalAs(UnmanagedType.I2)]
            public short dmOrientation;
            [MarshalAs(UnmanagedType.I2)]
            public short dmPaperSize;
            [MarshalAs(UnmanagedType.I2)]
            public short dmPaperLength;
            [MarshalAs(UnmanagedType.I2)]
            public short dmPaperWidth;
            [MarshalAs(UnmanagedType.I2)]
            public short dmScale;
            [MarshalAs(UnmanagedType.I2)]
            public short dmCopies;
            [MarshalAs(UnmanagedType.I2)]
            public short dmDefaultSource;
            [MarshalAs(UnmanagedType.I2)]
            public short dmPrintQuality;
            [MarshalAs(UnmanagedType.I2)]
            public short dmColor;
            [MarshalAs(UnmanagedType.I2)]
            public short dmDuplex;
            [MarshalAs(UnmanagedType.I2)]
            public short dmYResolution;
            [MarshalAs(UnmanagedType.I2)]
            public short dmTTOption;
            [MarshalAs(UnmanagedType.I2)]
            public short dmCollate;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 32)]
            public String dmFormName;
            [MarshalAs(UnmanagedType.U2)]
            public short dmLogPixels;
            [MarshalAs(UnmanagedType.U4)]
            public int dmBitsPerPel;
            [MarshalAs(UnmanagedType.U4)]
            public int dmPelsWidth;
            [MarshalAs(UnmanagedType.U4)]
            public int dmPelsHeight;
            [MarshalAs(UnmanagedType.U4)]
            public int dmNup;
            [MarshalAs(UnmanagedType.U4)]
            public int dmDisplayFrequency;
            [MarshalAs(UnmanagedType.U4)]
            public int dmICMMethod;
            [MarshalAs(UnmanagedType.U4)]
            public int dmICMIntent;
            [MarshalAs(UnmanagedType.U4)]
            public int dmMediaType;
            [MarshalAs(UnmanagedType.U4)]
            public int dmDitherType;
            [MarshalAs(UnmanagedType.U4)]
            public int dmReserved1;
            [MarshalAs(UnmanagedType.U4)]
            public int dmReserved2;
        }
 
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
        internal struct PRINTER_INFO_9
        {
            public IntPtr pDevMode;
        }
 
        [DllImport("winspool.Drv", EntryPoint = "AddFormW", SetLastError = true,CharSet = CharSet.Unicode, ExactSpelling = true,
                    CallingConvention = CallingConvention.StdCall), SuppressUnmanagedCodeSecurityAttribute()]
        internal static extern bool AddForm(IntPtr phPrinter,[MarshalAs(UnmanagedType.I4)] int level,ref FormInfo1 form);
 
        [DllImport("winspool.Drv", EntryPoint = "DeleteForm", SetLastError = true,CharSet = CharSet.Unicode, ExactSpelling = false, CallingConvention = CallingConvention.StdCall),
                  SuppressUnmanagedCodeSecurityAttribute()]
        internal static extern bool DeleteForm(IntPtr phPrinter,[MarshalAs(UnmanagedType.LPTStr)] string pName);
 
        [DllImport("kernel32.dll", EntryPoint = "GetLastError", SetLastError = false,ExactSpelling = true, CallingConvention = CallingConvention.StdCall),SuppressUnmanagedCodeSecurityAttribute()]
        internal static extern Int32 GetLastError();
 
        [DllImport("GDI32.dll", EntryPoint = "CreateDC", SetLastError = true,CharSet = CharSet.Unicode, ExactSpelling = false,CallingConvention = CallingConvention.StdCall),SuppressUnmanagedCodeSecurityAttribute()]
        internal static extern IntPtr CreateDC([MarshalAs(UnmanagedType.LPTStr)] string pDrive,[MarshalAs(UnmanagedType.LPTStr)] string pName,[MarshalAs(UnmanagedType.LPTStr)] string pOutput,ref structDevMode pDevMode);
 
        [DllImport("GDI32.dll", EntryPoint = "ResetDC", SetLastError = true,
          CharSet = CharSet.Unicode, ExactSpelling = false,CallingConvention = CallingConvention.StdCall),SuppressUnmanagedCodeSecurityAttribute()]
        internal static extern IntPtr ResetDC(IntPtr hDC,ref structDevMode  pDevMode);
 
        [DllImport("GDI32.dll", EntryPoint = "DeleteDC", SetLastError = true, CharSet = CharSet.Unicode, ExactSpelling = false,
                    CallingConvention = CallingConvention.StdCall),SuppressUnmanagedCodeSecurityAttribute()]
        internal static extern bool DeleteDC(IntPtr hDC);
 
        [DllImport("winspool.Drv", EntryPoint = "SetPrinterA", SetLastError = true,
          CharSet = CharSet.Auto, ExactSpelling = true,CallingConvention = CallingConvention.StdCall), SuppressUnmanagedCodeSecurityAttribute()]
        internal static extern bool SetPrinter(IntPtr hPrinter,[MarshalAs(UnmanagedType.I4)] int level,IntPtr pPrinter,[MarshalAs(UnmanagedType.I4)] int command);
 
        [DllImport("winspool.Drv", EntryPoint = "DocumentPropertiesA", SetLastError = true,ExactSpelling = true, CallingConvention = CallingConvention.StdCall)]
        internal static extern int DocumentProperties(IntPtr hwnd,IntPtr hPrinter,[MarshalAs(UnmanagedType.LPStr)] string pDeviceName,IntPtr pDevModeOutput,IntPtr pDevModeInput,int fMode);
 
        [DllImport("winspool.Drv", EntryPoint = "GetPrinterA", SetLastError = true,ExactSpelling = true, CallingConvention = CallingConvention.StdCall)]
        internal static extern bool GetPrinter(IntPtr hPrinter,int dwLevel,IntPtr pPrinter,int dwBuf,out int dwNeeded);
 
        [Flags]
        internal enum SendMessageTimeoutFlags : uint
        {
            SMTO_NORMAL = 0x0000,
            SMTO_BLOCK = 0x0001,
            SMTO_ABORTIFHUNG = 0x0002,
            SMTO_NOTIMEOUTIFNOTHUNG = 0x0008
        }
        const int WM_SETTINGCHANGE = 0x001A;
        const int HWND_BROADCAST = 0xffff;
 
        [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        internal static extern IntPtr SendMessageTimeout(IntPtr windowHandle,uint Msg,IntPtr wParam,IntPtr lParam,SendMessageTimeoutFlags flags,uint timeout,out IntPtr result);
 
        //EnumPrinters用到的函数和结构体
        [DllImport("winspool.drv", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern bool EnumPrinters(PrinterEnumFlags Flags, string Name, uint Level,IntPtr pPrinterEnum, uint cbBuf,ref uint pcbNeeded, ref uint pcReturned);
 
        [StructLayout(LayoutKind.Sequential)]
        internal struct PRINTER_INFO_2
        {
            public string pServerName;
            public string pPrinterName;
            public string pShareName;
            public string pPortName;
            public string pDriverName;
            public string pComment;
            public string pLocation;
            public IntPtr pDevMode;
            public string pSepFile;
            public string pPrintProcessor;
            public string pDatatype;
            public string pParameters;
            public IntPtr pSecurityDescriptor;
            public uint Attributes;
            public uint Priority;
            public uint DefaultPriority;
            public uint StartTime;
            public uint UntilTime;
            public uint Status;
            public uint cJobs;
            public uint AveragePPM;
        }
 
        [FlagsAttribute]
        internal enum PrinterEnumFlags
        {
            PRINTER_ENUM_DEFAULT = 0x00000001,
            PRINTER_ENUM_LOCAL = 0x00000002,
            PRINTER_ENUM_CONNECTIONS = 0x00000004,
            PRINTER_ENUM_FAVORITE = 0x00000004,
            PRINTER_ENUM_NAME = 0x00000008,
            PRINTER_ENUM_REMOTE = 0x00000010,
            PRINTER_ENUM_SHARED = 0x00000020,
            PRINTER_ENUM_NETWORK = 0x00000040,
            PRINTER_ENUM_EXPAND = 0x00004000,
            PRINTER_ENUM_CONTAINER = 0x00008000,
            PRINTER_ENUM_ICONMASK = 0x00ff0000,
            PRINTER_ENUM_ICON1 = 0x00010000,
            PRINTER_ENUM_ICON2 = 0x00020000,
            PRINTER_ENUM_ICON3 = 0x00040000,
            PRINTER_ENUM_ICON4 = 0x00080000,
            PRINTER_ENUM_ICON5 = 0x00100000,
            PRINTER_ENUM_ICON6 = 0x00200000,
            PRINTER_ENUM_ICON7 = 0x00400000,
            PRINTER_ENUM_ICON8 = 0x00800000,
            PRINTER_ENUM_HIDE = 0x01000000
        }
 
        //打印机状态
        [FlagsAttribute]
        internal enum PrinterStatus
        {
            PRINTER_STATUS_BUSY = 0x00000200,
            PRINTER_STATUS_DOOR_OPEN = 0x00400000,
            PRINTER_STATUS_ERROR = 0x00000002,
            PRINTER_STATUS_INITIALIZING = 0x00008000,
            PRINTER_STATUS_IO_ACTIVE = 0x00000100,
            PRINTER_STATUS_MANUAL_FEED = 0x00000020,
            PRINTER_STATUS_NO_TONER = 0x00040000,
            PRINTER_STATUS_NOT_AVAILABLE = 0x00001000,
            PRINTER_STATUS_OFFLINE = 0x00000080,
            PRINTER_STATUS_OUT_OF_MEMORY = 0x00200000,
            PRINTER_STATUS_OUTPUT_BIN_FULL = 0x00000800,
            PRINTER_STATUS_PAGE_PUNT = 0x00080000,
            PRINTER_STATUS_PAPER_JAM = 0x00000008,
            PRINTER_STATUS_PAPER_OUT = 0x00000010,
            PRINTER_STATUS_PAPER_PROBLEM = 0x00000040,
            PRINTER_STATUS_PAUSED = 0x00000001,
            PRINTER_STATUS_PENDING_DELETION = 0x00000004,
            PRINTER_STATUS_PRINTING = 0x00000400,
            PRINTER_STATUS_PROCESSING = 0x00004000,
            PRINTER_STATUS_TONER_LOW = 0x00020000,
            PRINTER_STATUS_USER_INTERVENTION = 0x00100000,
            PRINTER_STATUS_WAITING = 0x20000000,
            PRINTER_STATUS_WARMING_UP = 0x00010000
        }
 
        //GetDefaultPrinter用到的API函数说明
        [DllImport("winspool.drv", CharSet = CharSet.Auto, SetLastError = true)]
        internal static extern bool GetDefaultPrinter(StringBuilder pszBuffer, ref int size);
 
        //SetDefaultPrinter用到的API函数声明
        [DllImport("winspool.drv", CharSet = CharSet.Auto, SetLastError = true)]
        internal static extern bool SetDefaultPrinter(string Name);
 
        //EnumFormsA用到的函数声明，应该和EnumPrinters类似
        [DllImport("winspool.drv", EntryPoint = "EnumForms")]
        internal static extern int EnumFormsA(IntPtr hPrinter, int Level, ref byte pForm, int cbBuf, ref int pcbNeeded, ref int pcReturned);
 
        #endregion   API声明
        internal static int GetPrinterStatusInt(string PrinterName)
        {
            int intRet = 0;
            IntPtr hPrinter;
            structPrinterDefaults defaults = new structPrinterDefaults();
            if (OpenPrinter(PrinterName, out hPrinter, ref defaults))
            {
                int cbNeeded = 0;
                bool bolRet = GetPrinter(hPrinter, 2, IntPtr.Zero, 0, out cbNeeded);
                if (cbNeeded > 0)
                {
                    IntPtr pAddr = Marshal.AllocHGlobal((int)cbNeeded);
                    bolRet = GetPrinter(hPrinter, 2, pAddr, cbNeeded, out cbNeeded);
                    if (bolRet)
                    {
                        PRINTER_INFO_2 Info2 = new PRINTER_INFO_2();
 
                        Info2 = (PRINTER_INFO_2)Marshal.PtrToStructure(pAddr, typeof(PRINTER_INFO_2));
 
                        intRet = System.Convert.ToInt32(Info2.Status);
                    }
                    Marshal.FreeHGlobal(pAddr);
                }
                ClosePrinter(hPrinter);
            }
            return intRet;
        }
        internal static PRINTER_INFO_2[] EnumPrintersByFlag(PrinterEnumFlags Flags)
        {
            uint cbNeeded = 0;
            uint cReturned = 0;
            bool ret = EnumPrinters(PrinterEnumFlags.PRINTER_ENUM_LOCAL, null, 2, IntPtr.Zero, 0, ref cbNeeded, ref cReturned);
            IntPtr pAddr = Marshal.AllocHGlobal((int)cbNeeded);
            ret = EnumPrinters(PrinterEnumFlags.PRINTER_ENUM_LOCAL, null, 2, pAddr, cbNeeded, ref cbNeeded, ref cReturned);
            if (ret)
            {
                PRINTER_INFO_2[] Info2 = new PRINTER_INFO_2[cReturned];
                int offset = pAddr.ToInt32();
                for (int i = 0; i < cReturned; i++)
                {
                    Info2[i].pServerName = Marshal.PtrToStringAuto(Marshal.ReadIntPtr(new IntPtr(offset)));
                    offset += 4;
                    Info2[i].pPrinterName = Marshal.PtrToStringAuto(Marshal.ReadIntPtr(new IntPtr(offset)));
                    offset += 4;
                    Info2[i].pShareName = Marshal.PtrToStringAuto(Marshal.ReadIntPtr(new IntPtr(offset)));
                    offset += 4;
                    Info2[i].pPortName = Marshal.PtrToStringAuto(Marshal.ReadIntPtr(new IntPtr(offset)));
                    offset += 4;
                    Info2[i].pDriverName = Marshal.PtrToStringAuto(Marshal.ReadIntPtr(new IntPtr(offset)));
                    offset += 4;
                    Info2[i].pComment = Marshal.PtrToStringAuto(Marshal.ReadIntPtr(new IntPtr(offset)));
                    offset += 4;
                    Info2[i].pLocation = Marshal.PtrToStringAuto(Marshal.ReadIntPtr(new IntPtr(offset)));
                    offset += 4;
                    Info2[i].pDevMode = Marshal.ReadIntPtr(new IntPtr(offset));
                    offset += 4;
                    Info2[i].pSepFile = Marshal.PtrToStringAuto(Marshal.ReadIntPtr(new IntPtr(offset)));
                    offset += 4;
                    Info2[i].pPrintProcessor = Marshal.PtrToStringAuto(Marshal.ReadIntPtr(new IntPtr(offset)));
                    offset += 4;
                    Info2[i].pDatatype = Marshal.PtrToStringAuto(Marshal.ReadIntPtr(new IntPtr(offset)));
                    offset += 4;
                    Info2[i].pParameters = Marshal.PtrToStringAuto(Marshal.ReadIntPtr(new IntPtr(offset)));
                    offset += 4;
                    Info2[i].pSecurityDescriptor = Marshal.ReadIntPtr(new IntPtr(offset));
                    offset += 4;
                    Info2[i].Attributes = (uint)Marshal.ReadIntPtr(new IntPtr(offset));
                    offset += 4;
                    Info2[i].Priority = (uint)Marshal.ReadInt32(new IntPtr(offset));
                    offset += 4;
                    Info2[i].DefaultPriority = (uint)Marshal.ReadInt32(new IntPtr(offset));
                    offset += 4;
                    Info2[i].StartTime = (uint)Marshal.ReadInt32(new IntPtr(offset));
                    offset += 4;
                    Info2[i].UntilTime = (uint)Marshal.ReadInt32(new IntPtr(offset));
                    offset += 4;
                    Info2[i].Status = (uint)Marshal.ReadInt32(new IntPtr(offset));
                    offset += 4;
                    Info2[i].cJobs = (uint)Marshal.ReadInt32(new IntPtr(offset));
                    offset += 4;
                    Info2[i].AveragePPM = (uint)Marshal.ReadInt32(new IntPtr(offset));
                    offset += 4;
                }
                Marshal.FreeHGlobal(pAddr);
                return Info2;
            }
            else
            {
                return new PRINTER_INFO_2[0];
            }
        }
        #region 获取当前指定打印机的状态
        /// </summary>
        /// 获取当前指定打印机的状态
        /// </summary>
        /// <param name="PrinterName">打印机名称</param>
        /// <returns>打印机状态描述</returns>
 
        public static string GetPrinterStatus(string PrinterName)
        {
            int intValue = GetPrinterStatusInt(PrinterName);
            string strRet = string.Empty;
            switch (intValue)
            {
                case 0:
                    strRet = "准备就绪（Ready）";
                    break;
                case 0x00000200:
                    strRet = "忙(Busy）";
                    break;
                case 0x00400000:
                    strRet = "门被打开（Printer Door Open）";
                    break;
                case 0x00000002:
                    strRet = "错误(Printer Error）";
                    break;
                case 0x0008000:
                    strRet = "正在初始化(Initializing）";
                    break;
                case 0x00000100:
                    strRet = "正在输入或输出（I/O Active）";
                    break;
                case 0x00000020:
                    strRet = "手工送纸（Manual Feed）";
                    break;
                case 0x00040000:
                    strRet = "无墨粉（No Toner）";
                    break;
                case 0x00001000:
                    strRet = "不可用（Not Available）";
                    break;
                case 0x00000080:
                    strRet = "脱机（Off Line）";
                    break;
                case 0x00200000:
                    strRet = "内存溢出（Out of Memory）";
                    break;
                case 0x00000800:
                    strRet = "输出口已满（Output Bin Full）";
                    break;
                case 0x00080000:
                    strRet = "当前页无法打印（Page Punt）";
                    break;
                case 0x00000008:
                    strRet = "塞纸（Paper Jam）";
                    break;
                case 0x00000010:
                    strRet = "打印纸用完（Paper Out）";
                    break;
                case 0x00000040:
                    strRet = "纸张问题（Page Problem）";
                    break;
                case 0x00000001:
                    strRet = "暂停（Paused）";
                    break;
                case 0x00000004:
                    strRet = "正在删除（Pending Deletion）";
                    break;
                case 0x00000400:
                    strRet = "正在打印（Printing）";
                    break;
                case 0x00004000:
                    strRet = "正在处理（Processing）";
                    break;
                case 0x00020000:
                    strRet = "墨粉不足（Toner Low）";
                    break;
                case 0x00100000:
                    strRet = "需要用户干预（User Intervention）";
                    break;
                case 0x20000000:
                    strRet = "等待（Waiting）";
                    break;
                case 0x00010000:
                    strRet = "正在准备（Warming Up）";
                    break;
                default:
                    strRet = "未知状态（Unknown Status）";
                    break;
            }
            return strRet;
        }
        #endregion 获取当前指定打印机的状态
        #region 删除已经存在的自定义纸张
        /**/
        /// <summary>
        /// 删除已经存在的自定义纸张
        /// </summary>
        /// <param name="PrinterName">打印机名称</param>
        /// <param name="PaperName">纸张名称</param>
        public static void DeleteCustomPaperSize(string PrinterName, string PaperName)
        {
            const int PRINTER_ACCESS_USE = 0x00000008;
            const int PRINTER_ACCESS_ADMINISTER = 0x00000004;
 
            structPrinterDefaults defaults = new structPrinterDefaults();
            defaults.pDatatype = null;
            defaults.pDevMode = IntPtr.Zero;
            defaults.DesiredAccess = PRINTER_ACCESS_ADMINISTER | PRINTER_ACCESS_USE;
 
            IntPtr hPrinter = IntPtr.Zero;
 
            //打开打印机
            if (OpenPrinter(PrinterName, out hPrinter, ref defaults))
            {
                try
                {
                    DeleteForm(hPrinter, PaperName);
                    ClosePrinter(hPrinter);
                }
                catch
                {
 
                }
            }
        }
        #endregion 删除已经存在的自定义纸张
        #region 指定的打印机设置以mm为单位的自定义纸张(Form)
        /**/
        /// <summary>
        /// 指定的打印机设置以mm为单位的自定义纸张(Form)
        /// </summary>
        /// <param name="PrinterName">打印机名称</param>
        /// <param name="PaperName">Form名称</param>
        /// <param name="WidthInMm">以mm为单位的宽度</param>
        /// <param name="HeightInMm">以mm为单位的高度</param>
        public static void AddCustomPaperSize(string PrinterName, string PaperName, float WidthInMm, float HeightInMm)
        {
            if (PlatformID.Win32NT == Environment.OSVersion.Platform)
            {
                const int PRINTER_ACCESS_USE = 0x00000008;
                const int PRINTER_ACCESS_ADMINISTER = 0x00000004;             
                structPrinterDefaults defaults = new structPrinterDefaults();
                defaults.pDatatype = null;
                defaults.pDevMode = IntPtr.Zero;
                defaults.DesiredAccess = PRINTER_ACCESS_ADMINISTER | PRINTER_ACCESS_USE;
                IntPtr hPrinter = IntPtr.Zero;
                //打开打印机
                if (OpenPrinter(PrinterName, out hPrinter, ref defaults))
                {
                    try
                    {
                        //如果Form存在删除之
                        DeleteForm(hPrinter, PaperName);
                        //创建并初始化FORM_INFO_1
                        FormInfo1 formInfo = new FormInfo1();
                        formInfo.Flags = 0;
                        formInfo.pName = PaperName;
                        formInfo.Size.width = (int)(WidthInMm * 1000.0);
                        formInfo.Size.height = (int)(HeightInMm * 1000.0);
                        formInfo.ImageableArea.left = 0;
                        formInfo.ImageableArea.right = formInfo.Size.width;
                        formInfo.ImageableArea.top = 0;
                        formInfo.ImageableArea.bottom = formInfo.Size.height;
                        if (!AddForm(hPrinter, 1, ref formInfo))
                        {
                            StringBuilder strBuilder = new StringBuilder();
                            strBuilder.AppendFormat("向打印机 {1} 添加自定义纸张 {0} 失败！错误代号：{2}",
                             PaperName, PrinterName, GetLastError());
                            throw new ApplicationException(strBuilder.ToString());
                        }
 
                        //初始化
                        const int DM_OUT_BUFFER = 2;
                        const int DM_IN_BUFFER = 8;
                        structDevMode devMode = new structDevMode();
                        IntPtr hPrinterInfo, hDummy;
                        PRINTER_INFO_9 printerInfo;
                        printerInfo.pDevMode = IntPtr.Zero;
                        int iPrinterInfoSize, iDummyInt;
 
 
                        int iDevModeSize = DocumentProperties(IntPtr.Zero, hPrinter, PrinterName, IntPtr.Zero, IntPtr.Zero, 0);
 
                        if (iDevModeSize < 0)
                            throw new ApplicationException("无法取得DEVMODE结构的大小！");
 
                        //分配缓冲
                        IntPtr hDevMode = Marshal.AllocCoTaskMem(iDevModeSize + 100);
 
                        //获取DEV_MODE指针
                        int iRet = DocumentProperties(IntPtr.Zero, hPrinter, PrinterName, hDevMode, IntPtr.Zero, DM_OUT_BUFFER);
 
                        if (iRet < 0)
                            throw new ApplicationException("无法获得DEVMODE结构！");
 
                        //填充DEV_MODE
                        devMode = (structDevMode)Marshal.PtrToStructure(hDevMode, devMode.GetType());
 
 
                        devMode.dmFields = 0x10000;
 
                        //FORM名称
                        devMode.dmFormName = PaperName;
 
                        Marshal.StructureToPtr(devMode, hDevMode, true);
 
                        iRet = DocumentProperties(IntPtr.Zero, hPrinter, PrinterName,
                         printerInfo.pDevMode, printerInfo.pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER);
 
                        if (iRet < 0)
                            throw new ApplicationException("无法为打印机设定打印方向！");
 
                        GetPrinter(hPrinter, 9, IntPtr.Zero, 0, out iPrinterInfoSize);
                        if (iPrinterInfoSize == 0)
                            throw new ApplicationException("调用GetPrinter方法失败！");
 
                        hPrinterInfo = Marshal.AllocCoTaskMem(iPrinterInfoSize + 100);
 
                        bool bSuccess = GetPrinter(hPrinter, 9, hPrinterInfo, iPrinterInfoSize, out iDummyInt);
 
                        if (!bSuccess)
                            throw new ApplicationException("调用GetPrinter方法失败！");
 
                        printerInfo = (PRINTER_INFO_9)Marshal.PtrToStructure(hPrinterInfo, printerInfo.GetType());
                        printerInfo.pDevMode = hDevMode;
 
                        Marshal.StructureToPtr(printerInfo, hPrinterInfo, true);
 
                        bSuccess = SetPrinter(hPrinter, 9, hPrinterInfo, 0);
 
                        if (!bSuccess)
                            throw new Win32Exception(Marshal.GetLastWin32Error(), "调用SetPrinter方法失败，无法进行打印机设置！");
 
                        SendMessageTimeout(
                         new IntPtr(HWND_BROADCAST),
                         WM_SETTINGCHANGE,
                         IntPtr.Zero,
                         IntPtr.Zero,
                         PrinterHelper.SendMessageTimeoutFlags.SMTO_NORMAL,
                         1000,
                         out hDummy);
                    }
                    finally
                    {
                        ClosePrinter(hPrinter);
                    }
                }
                else
                {
                    StringBuilder strBuilder = new StringBuilder();
                    strBuilder.AppendFormat("无法打开打印机{0}, 错误代号: {1}",
                     PrinterName, GetLastError());
                    throw new ApplicationException(strBuilder.ToString());
                }
            }
            else
            {
                structDevMode pDevMode = new structDevMode();
                IntPtr hDC = CreateDC(null, PrinterName, null, ref pDevMode);
                if (hDC != IntPtr.Zero)
                {
                    const long DM_PAPERSIZE = 0x00000002L;
                    const long DM_PAPERLENGTH = 0x00000004L;
                    const long DM_PAPERWIDTH = 0x00000008L;
                    pDevMode.dmFields = (int)(DM_PAPERSIZE | DM_PAPERWIDTH | DM_PAPERLENGTH);
                    pDevMode.dmPaperSize = 256;
                    pDevMode.dmPaperWidth = (short)(WidthInMm * 1000.0);
                    pDevMode.dmPaperLength = (short)(HeightInMm * 1000.0);
                    ResetDC(hDC, ref pDevMode);
                    DeleteDC(hDC);
                }
            }
        }
        #endregion 指定的打印机设置以mm为单位的自定义纸张(Form)
        #region 获取本地打印机列表
        /**/
        /// <summary>
        /// 获取本地打印机列表
        /// 可以通过制定参数获取网络打印机
        /// </summary>
        /// <returns>打印机列表</returns>
        public static System.Collections.ArrayList GetPrinterList()
        {
            System.Collections.ArrayList alRet = new System.Collections.ArrayList();
            PRINTER_INFO_2[] Info2 = EnumPrintersByFlag(PrinterEnumFlags.PRINTER_ENUM_LOCAL);
            for (int i = 0; i < Info2.Length; i++)
            {
                alRet.Add(Info2[i].pPrinterName);
            }
            return alRet;
        }
        #endregion 获取本地打印机列表
        #region 获取本机的默认打印机名称
        /**/
        /// <summary>
        /// 获取本机的默认打印机名称
        /// </summary>
        /// <returns>默认打印机名称</returns>
        public static string GetDeaultPrinterName()
        {
            StringBuilder dp = new StringBuilder(256);
            int size = dp.Capacity;
            if (GetDefaultPrinter(dp, ref size))
            {
                return dp.ToString();
            }
            else
            {
                return string.Empty;
            }
        }
        #endregion 获取本机的默认打印机名称
        #region 设置默认打印机
        /**/
        /// <summary>
        /// 设置默认打印机
        /// </summary>
        /// <param name="PrinterName">可用的打印机名称</param>
        public static void SetPrinterToDefault(string PrinterName)
        {
            SetDefaultPrinter(PrinterName);
        }
        #endregion 设置默认打印机
        #region 判断打印机是否在系统可用的打印机列表中
        /**/
        ///// <summary>
        ///// 判断打印机是否在系统可用的打印机列表中
        ///// </summary>
        ///// <param name="PrinterName">打印机名称</param>
        ///// <returns>是：在；否：不在</returns>
        public static bool PrinterInList(string PrinterName)
        {
            bool bolRet = false;
 
            System.Collections.ArrayList alPrinters = GetPrinterList();
 
            for (int i = 0; i < alPrinters.Count; i++)
            {
                if (PrinterName == alPrinters[i].ToString())
                {
                    bolRet = true;
                    break;
                }
            }
 
            alPrinters.Clear();
            alPrinters = null;
 
            return bolRet;
        }
        #endregion 判断打印机是否在系统可用的打印机列表中
        #region 判断表单是否在指定的打印机所支持的纸张列表中
        /**/
        ///// <summary>
        ///// 判断表单是否在指定的打印机所支持的纸张列表中,表单就是我们平常所说的纸张
        ///// </summary>
        ///// <param name="PrinterName">打印机名称</param>
        ///// <param name="PaperName">纸张名称</param>
        ///// <returns>是：在；否：不在</returns>
        public static bool FormInPrinter(string PrinterName, string PaperName)
        {
            bool bolRet = false;
 
            System.Drawing.Printing.PrintDocument pd = new System.Drawing.Printing.PrintDocument();
 
            pd.PrinterSettings.PrinterName = PrinterName;
 
            foreach (System.Drawing.Printing.PaperSize ps in pd.PrinterSettings.PaperSizes)
            {
                if (ps.PaperName == PaperName)
                {
                    bolRet = true;
                    break;
                }
            }
 
            pd.Dispose();
 
            return bolRet;
        }
        #endregion 判断表单是否在指定的打印机所支持的纸张列表中
        #region 判断指定纸张的宽度和高度和与打印内容指定的宽度和高度是否匹配
        /**/
        /// <summary>
        /// 判断指定纸张的宽度和高度和与打印内容指定的宽度和高度是否匹配
        /// </summary>
        /// <param name="PrinterName">打印机名称</param>
        /// <param name="FormName">表单名称</param>
        /// <param name="Width">宽度</param>
        /// <param name="Height">高度</param>
        /// <returns></returns>
        public static bool FormSameSize(string PrinterName, string FormName, decimal Width, decimal Height)
        {
            bool bolRet = false;
 
            System.Drawing.Printing.PrintDocument pd = new System.Drawing.Printing.PrintDocument();
 
            pd.PrinterSettings.PrinterName = PrinterName;
 
            foreach (System.Drawing.Printing.PaperSize ps in pd.PrinterSettings.PaperSizes)
            {
                if (ps.PaperName == FormName)
                {
                    decimal decWidth = FromInchToCM(System.Convert.ToDecimal(ps.Width));
                    decimal decHeight = FromInchToCM(System.Convert.ToDecimal(ps.Height));
                    //只要整数位相同即认为是同一纸张，毕竟inch到cm的转换并不能整除
                    if (Math.Round(decWidth, 0) == Math.Round(Width, 0) && Math.Round(decHeight, 0) == Math.Round(Height, 0))
                        bolRet = true;
                    break;
                }
            }
 
            pd.Dispose();
 
            return bolRet;
        }
        #endregion 判断指定纸张的宽度和高度和与打印内容指定的宽度和高度是否匹配
        #region 英寸到厘米的转换
        /**/
        /// <summary>
        /// 英寸到厘米的转换
        /// /* = = = = = = = = = = = = = = = = *\
        /// | 换算一下计量单位，将其换算成厘米  |
        /// |    厘米     像素     英寸         |
        /// |     1        38     0.395         |
        /// |   0.026       1      0.01         |
        /// |    2.54      96        1          |
        /// \* = = = = = = = = = = = = = = = = */
        /// </summary>
        /// <param name="inch">英寸数</param>
        /// <returns>厘米数，两位小数</returns>
        ///
        public static decimal FromInchToCM(decimal inch)
        {
            return Math.Round((System.Convert.ToDecimal((inch / 100)) * System.Convert.ToDecimal(2.5400)), 2);
        }
        #endregion 英寸到厘米的转换
        [DllImport("winspool.Drv", EntryPoint = "OpenPrinterA", SetLastError = true, CharSet = CharSet.Ansi, ExactSpelling = true, CallingConvention = CallingConvention.StdCall)]
        public static extern bool OpenPrinter([MarshalAs(UnmanagedType.LPStr)] string szPrinter, out IntPtr hPrinter, IntPtr pd);
        //[DllImport("winspool.Drv", EntryPoint = "ClosePrinter", SetLastError = true, ExactSpelling = true, CallingConvention = CallingConvention.StdCall)]
        //public static extern bool ClosePrinter(IntPtr hPrinter);
        [DllImport("winspool.Drv", EntryPoint = "StartDocPrinterA", SetLastError = true, CharSet = CharSet.Ansi, ExactSpelling = true, CallingConvention = CallingConvention.StdCall)]
        public static extern bool StartDocPrinter(IntPtr hPrinter, Int32 level, [In, MarshalAs(UnmanagedType.LPStruct)] DOCINFOA di);
        [DllImport("winspool.Drv", EntryPoint = "EndDocPrinter", SetLastError = true, ExactSpelling = true, CallingConvention = CallingConvention.StdCall)]
        public static extern bool EndDocPrinter(IntPtr hPrinter);
        [DllImport("winspool.Drv", EntryPoint = "StartPagePrinter", SetLastError = true, ExactSpelling = true, CallingConvention = CallingConvention.StdCall)]
        public static extern bool StartPagePrinter(IntPtr hPrinter);
        [DllImport("winspool.Drv", EntryPoint = "EndPagePrinter", SetLastError = true, ExactSpelling = true, CallingConvention = CallingConvention.StdCall)]
        public static extern bool EndPagePrinter(IntPtr hPrinter);
        [DllImport("winspool.Drv", EntryPoint = "WritePrinter", SetLastError = true, ExactSpelling = true, CallingConvention = CallingConvention.StdCall)]
        public static extern bool WritePrinter(IntPtr hPrinter, IntPtr pBytes, Int32 dwCount, out Int32 dwWritten);
        /// <summary>
        /// 该方法把非托管内存中的字节数组发送到打印机的打印队列
        /// </summary>
        /// <param name="szPrinterName">打印机名称</param>
        /// <param name="pBytes">非托管内存指针</param>
        /// <param name="dwCount">字节数</param>
        /// <returns>成功返回true，失败时为false</returns>
        public static bool SendBytesToPrinter(string szPrinterName, IntPtr pBytes, Int32 dwCount)
        {
            Int32 dwError = 0, dwWritten = 0;
            IntPtr hPrinter = new IntPtr(0);
            DOCINFOA di = new DOCINFOA();
            bool bSuccess = false;
            di.pDocName = "My C#.NET RAW Document";
            di.pDataType = "RAW";
            try
            {
                // 打开打印机
                if (OpenPrinter(szPrinterName.Normalize(), out hPrinter, IntPtr.Zero))
                {
                    // 启动文档打印
                    if (StartDocPrinter(hPrinter, 1, di))
                    {
                        // 开始打印
                        if (StartPagePrinter(hPrinter))
                        {
                            // 向打印机输出字节  
                            bSuccess = WritePrinter(hPrinter, pBytes, dwCount, out dwWritten);
                            EndPagePrinter(hPrinter);
                        }
                        EndDocPrinter(hPrinter);
                    }
                    ClosePrinter(hPrinter);
                }
                if (bSuccess == false)
                {
                    dwError = Marshal.GetLastWin32Error();
                }
            }
            catch (Win32Exception ex)
            {
                WriteLog(ex.Message);
                bSuccess = false;
            }
            return bSuccess;
        }
        /// <summary>
        /// 发送文件到打印机方法
        /// </summary>
        /// <param name="szPrinterName">打印机名称</param>
        /// <param name="szFileName">打印文件的路径</param>
        /// <returns></returns>
        public static bool SendFileToPrinter(string szPrinterName, string szFileName)
        {
            bool bSuccess = false;
            try
            {
                // 打开文件  
                FileStream fs = new FileStream(szFileName, FileMode.Open);
                // 将文件内容读作二进制
                BinaryReader br = new BinaryReader(fs);
                // 定义字节数组
                Byte[] bytes = new Byte[fs.Length];
                // 非托管指针  
                IntPtr pUnmanagedBytes = new IntPtr(0);
                int nLength;
                nLength = Convert.ToInt32(fs.Length);
                // 读取文件内容到字节数组
                bytes = br.ReadBytes(nLength);
                // 为这些字节分配一些非托管内存
                pUnmanagedBytes = Marshal.AllocCoTaskMem(nLength);
                // 将托管字节数组复制到非托管内存指针
                Marshal.Copy(bytes, 0, pUnmanagedBytes, nLength);
                // 将非托管字节发送到打印机
                bSuccess = SendBytesToPrinter(szPrinterName, pUnmanagedBytes, nLength);
                // 释放先前分配的非托管内存
                Marshal.FreeCoTaskMem(pUnmanagedBytes);
                fs.Close();
                fs.Dispose();
            }
            catch (Win32Exception ex)
            {
                WriteLog(ex.Message);
                bSuccess = false;
            }
            return bSuccess;
        }
        /// <summary>
        /// 将字符串发送到打印机方法
        /// </summary>
        /// <param name="szPrinterName">打印机名称</param>
        /// <param name="szString">打印的字符串</param>
        /// <returns></returns>
        public static bool SendStringToPrinter(string szPrinterName, string szString)
        {
            bool flag = false;
            try
            {
                IntPtr pBytes;
                Int32 dwCount;
                // 获取字符串长度  
                dwCount = szString.Length;
                // 将字符串复制到非托管 COM 任务分配的内存非托管内存块，并转换为 ANSI 文本
                pBytes = Marshal.StringToCoTaskMemAnsi(szString);
                // 将已转换的 ANSI 字符串发送到打印机
                flag = SendBytesToPrinter(szPrinterName, pBytes, dwCount);
                // 释放先前分配的非托管内存
                Marshal.FreeCoTaskMem(pBytes);
            }
            catch (Win32Exception ex)
            {
                WriteLog(ex.Message);
                flag = false;
            }
            return flag;
        }
        /// <summary>
        /// 写入日志方法
        /// </summary>
        /// <param name="msg">记录信息</param>
        public static void WriteLog(string msg)
        {
            string str = string.Empty;
            string path = AppDomain.CurrentDomain.BaseDirectory + "log\\" + DateTime.Now.ToString("yyyy-MM-dd") + ".txt";
            FileStream filestream = new FileStream(path, FileMode.OpenOrCreate);
            str += "************" + DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss") + "************\r\n";
            str += msg;
            str += "************" + DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss") + "************\r\n";
            FileStream fs = new FileStream(path, FileMode.Append);
            StreamWriter sw = new StreamWriter(fs, System.Text.Encoding.Default);
            sw.WriteLine(str);
            sw.Flush();
            sw.Close();
            sw.Dispose();
            fs.Close();
            fs.Dispose();
        }
    }
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public class DOCINFOA
    {
        [MarshalAs(UnmanagedType.LPStr)]
        public string pDocName;
        [MarshalAs(UnmanagedType.LPStr)]
        public string pOutputFile;
        [MarshalAs(UnmanagedType.LPStr)]
        public string pDataType;
    }
}
```
        我们可以看到，这些函数都是调用的winspool.drv 这个windows的驱动来实现API调用的，这里我们还真应该好好琢磨琢磨。
