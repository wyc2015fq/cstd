# C# .NET RTP MJPEG Player - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年01月04日 13:48:46[boonya](https://me.csdn.net/boonya)阅读数：640








英文原文地址：[https://www.codeproject.com/Articles/85763/C-NET-RTP-MJPEG-Player](https://www.codeproject.com/Articles/85763/C-NET-RTP-MJPEG-Player)


通过RTP实现RTP和MJPEG，支持大型帧和组播。


- [Download source - 38.47 KB](https://www.codeproject.com/KB/audio-video/CSharp_RTP_MJPEG_Player/RTP_PROJECT.zip)
![](https://img-blog.csdn.net/20180104134043392)
## 介绍


这是C＃中RTP / MJPEG协议的实现。 我在2005年写了这个代码。



## 背景


这个代码被用于Elphel网络摄像机，组播RTP / MJPEG，但它应该几乎兼容所有的RTP / MJPEG技术。 此代码是通过阅读相关的RFC编写的。



## 使用代码


RtpPacket.cs



```
using System;
using System.Collections.Generic;
using System.Text;

namespace RTPLib
{
    public class RtpPacket
    {
        public static readonly int rtp_version = 2; /* the version of RTP supported */
        public static readonly int rtp_length = 12; /* fixed RTP packet header size */
        public int version; /* the version of RTP should be 2 */ 
        public byte padding; /* the padding flag **/
        public byte extension; /* the extension flag - if any extra headers 
				for higher protocol */
        public int csrc_count; /* the source count */
        public bool marker; /* the marker status */
        public int payload_type;    /* the type of payload  */
        public ushort sequence_no;  /* the sequence number of the RTP packet */
        public uint timestamp;  /* the timestamp of the RTP packet */
        public uint source_id;  /* the source id  of the RTP packet */
        public RtpPacket(byte[] _data)
        {
            decode(_data);
        }
        public void decode(byte[] data)
        {
            /*byte[] data = new byte[12];
            Array.Copy(_data, data, 12);*/ /* there should be no need 
					to copy the header */
            version = data[0] >> 6;
            padding = (byte)(0x1 & (data[0] >> 5));
            extension = (byte)(0x1 & (data[0] >> 4));
            csrc_count = 0x1F & (data[0]);
            marker = ((data[1] >> 7) == 1);
            payload_type = data[1] & 0x7f; /* we will assume it's 26 -> RTP/MJPEG */
            sequence_no = Utils.HostToNetworkOrderShort
			(System.BitConverter.ToUInt16(data, 2));
            timestamp = Utils.SwapUnsignedInt(System.BitConverter.ToUInt32(data, 4));
            source_id = Utils.SwapUnsignedInt(System.BitConverter.ToUInt32(data, 8));
        }
    }
}
```

JPEGFrame.cs


```
using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Drawing;
using System.Runtime.InteropServices;
using System.ComponentModel;
namespace RTPLib
{
    public unsafe class JPEGFrame : IDisposable
    {
        private byte[] _buffer;
        private int _offset;
        private MemoryStream _stream_buffer;
        private bool _alreadyDisposed;
        private Image _frame_img;
        private bool _initialized;

        public int type_specific; /** 8 bits **/
        public int fragment_offset; /** 24 bits **/
        public int next_fragment_offset;
        /** 0-63 fixed q no restart markers **/
        public int jpeg_type; /** 8 bits **/
        /** 64-127 Restart Marker header appears immediately 
	following the main JPEG header **/
        public int q;   /** 8 bits **/ /** multiply by 8 **/
        /** 128-255 qtable is present and right after the 
	restart marker header if present **/
        /** 128 to 254 q is static to be read only once**/
        public int width; /** 8 bits **/ /** multiply by 8 **/
        public int height; /** 8 bits **/ /** multiply by 8 **/

        /** Restart Marker header **/
        public ushort restart_interval; /** 16 bits **/
        public ushort f; /** 1 bit **/
        public ushort l; /** 1 bit **/
        public ushort restart_count; /** 14 bits **/ /** f and l must be set to 1 
	and 0x3FFF if set reassemble the frame before decoding **/

        /** Quantization Table header **/
        /** must be present after the restart marker header if it is present**/
        public ushort mbz; /** 8 bits **/
        public ushort precision; /** 8 bits **/
        public ushort qlength; /** 16 bits **/
        public byte[] qtable; /** qtables **/
        public static int max_size = 8192 * 250;

        private byte[] _frag; /** hold the fragment offset number for conversion **/
        private ImageProcessing imgProceesor;
        private unsafe byte* buffer_ptr;

        public JPEGFrame()
        {
            //qtable = new byte[1024];
            //imgProceesor = new ImageProcessing();
            _buffer = new byte[max_size];
            unsafe
            {
                fixed (byte* buf_ptr = _buffer)
                    buffer_ptr = buf_ptr;
            }
            _frag = new byte[4];
            _initialized = false;
            _offset = 0;
        }
        public bool Decode(byte * data, int offset) //Decode(ref byte[] data, int offset)
        {
            if (_initialized == false)
            {
                type_specific = data[offset + 0];
                _frag[0] = data[offset + 3];
                _frag[1] = data[offset + 2];
                _frag[2] = data[offset + 1];
                _frag[3] = 0x0;
                fragment_offset = System.BitConverter.ToInt32(_frag, 0);
                jpeg_type = data[offset + 4];
                q = data[offset + 5];
                width = data[offset + 6];
                height = data[offset + 7];
                _frag[0] = data[offset + 8];
                _frag[1] = data[offset + 9];
                restart_interval = (ushort)(System.BitConverter.ToUInt16
					(_frag, 0) & 0x3FF);
                //restart_interval = (ushort)(System.BitConverter.ToUInt16
					(data, offset + 8) & 0x3FF);
                if (width == 0) /** elphel 333 full image size more than 
				just one byte less that < 256 **/
                    width = 256;
                byte[] tmp = new byte[1024];
                _offset = Utils.MakeTables(q, jpeg_type, height, width, tmp);
                qtable = new byte[_offset];
               
                Array.Copy(tmp, 0, _buffer, 0, _offset);
                Array.Copy(tmp, 0, qtable, 0, _offset); //not needed but 
                _initialized = true;
                tmp = null;
                /*Utils.StaticDispose();
                Utils.Dispose();*/
                GC.Collect();
            }
            else
            {
                _frag[0] = data[15]; //12 + 3
                _frag[1] = data[14]; //12 + 2
                _frag[2] = data[13]; //12 + 1]
                _frag[3] = 0x0;
                fragment_offset = System.BitConverter.ToInt32(_frag, 0);
                _frag[0] = data[offset + 8];
                _frag[1] = data[offset + 9];
                restart_interval = (ushort)(System.BitConverter.ToUInt16
					(_frag, 0) & 0x3FF);
                //restart_interval = (ushort)(System.BitConverter.ToUInt16
					(data, offset + 8) & 0x3FF);
            }

            return (next_fragment_offset == fragment_offset);
        }
        public unsafe bool Write(byte * data, 
	int size, out bool sync) //Write(ref byte[] data, int size,out bool sync)
        {
            if (Decode(data, 12))
            {
                    for (int i = 20; i < size;)
                    {
                        buffer_ptr[_offset] = data[i++];
                        ++_offset;
                        buffer_ptr[_offset] = data[i++];
                        ++_offset;
                    }
                size -= 20;
                next_fragment_offset += size;
                sync = true;
                return ((data[1] >> 7) == 1); 
            }
            /*
            if (Decode(data, 12))
            {
                size -= 20;
                Array.Copy(data, 20, buffer, _offset, size);
                next_fragment_offset += size;
                _offset += size;
                sync = true;
                return ((data[1] >> 7) == 1);
            }*/
            else
            {
                _offset = qtable.Length;
                next_fragment_offset = 0;
                sync = false;
                return false;
            }
        }
        public Image GetFrame(out int motion_level)
        {
            if (_initialized == false)
                throw new Exception();
            _stream_buffer = new MemoryStream(_buffer, 0, _offset, false);
            _frame_img = Image.FromStream(_stream_buffer, false, true);
            _offset = qtable.Length;
            motion_level = 0;
            next_fragment_offset = 0;
            _stream_buffer.Close();
            _stream_buffer.Dispose();
            _stream_buffer = null;
            return _frame_img;

            /*stream_buffer = new MemoryStream(_offset);
            stream_buffer.Write(buffer, 0, _offset); */
            //stream_buffer = new MemoryStream(10);
           /* GCHandle handle = GCHandle.Alloc(buffer, GCHandleType.Normal);
            IntPtr ptr = Marshal.UnsafeAddrOfPinnedArrayElement(buffer, 0);
            frame_img = new Bitmap(width * 8, height * 8, 
		(width * 8), System.Drawing.Imaging.PixelFormat.Format24bppRgb, ptr);
            */
             //Console.WriteLine(frame_img.PixelFormat.ToString() + 
             //" " + Image.GetPixelFormatSize(frame_img.PixelFormat) / 8);
           
            /*GC.ReRegisterForFinalize(stream_buffer);*/
            //Bitmap map = frame_img as Bitmap;    
            //imgProceesor.CompareUnsafeFaster(out motion_level, ref frame_img);
            //motion_level = imgProceesor.MotionLevel;
            //Console.WriteLine("motion " + motion_level);   
        }
        ~JPEGFrame()
        {
            Dispose(true);
           
        }
       protected virtual void Dispose(bool isDisposing)
       {
           if (_alreadyDisposed)
               return;
           // Don't dispose more than once.          
           if (isDisposing)
           {
               // TODO: free managed resources here.
               if(_frame_img != null)
                    _frame_img.Dispose();
               if(_stream_buffer != null)
                    _stream_buffer.Close();
               //stream_buffer.Dispose();
                _initialized = false;
               _frame_img = null;
               _stream_buffer = null;
               qtable = null;
               _buffer = null;
               _alreadyDisposed = true;
               
           }
           // TODO: free unmanaged resources here.
           // Set disposed flag:          
       }
       public void Dispose()
       {
           Dispose(true);
           GC.SuppressFinalize(true);
       }
        public override string ToString()
        {
            string ret = "type_specific= " + type_specific + "\r\n";
            ret += "fragment_offset= " + fragment_offset + "\r\n";
            ret += "type=" + jpeg_type + "\r\n";
            ret += "q=" + q + "\r\n";
            ret += "width=" + width + "\r\n";
            ret += "height=" + height;
            if (jpeg_type > 63)
            {
                ret += "\r\n" + "restart_interval=" + restart_interval + "\r\n";
                ret += "f=" + f + " l=" + l + "\r\n";
                ret += "restart_count=" + restart_count;
            }
            return ret;
        }
    }
}
```


## 笔记


请理解，我在2005年编写了这个代码，并没有做任何更新。 如果您认为可以重新排列代码并提供意见，请让我知道。 我也希望现有的开源项目的RTP / H.264托管API实现。



## 兴趣点


我有兴趣创建互联网交换点。



## 历史


2005年第一版

![](https://img-blog.csdn.net/20180104134655916)








注：本篇文章值得关注的是RTP包封装和Frame数据获取。










