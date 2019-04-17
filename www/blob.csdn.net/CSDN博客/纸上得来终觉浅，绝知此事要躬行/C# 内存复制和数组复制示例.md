# C# 内存复制和数组复制示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年03月22日 16:55:54[boonya](https://me.csdn.net/boonya)阅读数：1335








内存复制-Marshal.Copy：

```
// 复制当前传入的数据
 byte[] pcm_src = new byte[len];
 // 复制数据到二进制数组
 Marshal.Copy(pcm, pcm_src, 0, len);
```

数组复制-Array.Copy：

```
// 复制当前传入的数据
byte[] pcm_src = new byte[len];
// 复制数据到二进制数组
Marshal.Copy(pcm, pcm_src, 0, len);
// 复制新传入的数据到新的数组结构
Array.Copy(pcm_src, 0, pcm_dest, bts_left.Length, len)
```


测试示例：


```
// 采样数
        ushort samples = 320;

        // 上次剩余的字节
        byte[] bts_left;

        class aa
        {
            public byte[] pcm;
            public int len;
        }

        private List<aa> data = new List<aa>();
    
        public void PlayAudio(IntPtr pcm, int len)
        {
            lock (this)
            {
                int samples_len = samples * 2;

                if (len == samples_len)
                {
                    byte[] bts = new byte[len];
                    Marshal.Copy(pcm, bts, 0, len);
                    data.Add(new aa
                    {
                        len = len,
                        pcm = bts
                    });
                }
                else
                // 此处解决采样数为1024时分割可以复制数据的大小
                {
                    var len_temp = bts_left ==null? len : len + bts_left.Length;

                    byte[] pcm_dest = new byte[len_temp];
                    byte[] bts = new byte[samples_len];
                    // 复制数据
                    if (bts_left != null)
                    {
                        // 复制长度不够的数据
                        Array.Copy(bts_left,0, pcm_dest, 0, bts_left.Length);
                        // 复制当前传入的数据
                        byte[] pcm_src = new byte[len];
                        // 复制数据到二进制数组
                        Marshal.Copy(pcm, pcm_src, 0, len);
                        // 复制新传入的数据到新的数组结构
                        Array.Copy(pcm_src, 0, pcm_dest, bts_left.Length, len);
                       
                        bts_left = null;
                    }
                    else
                    {
                        Marshal.Copy(pcm, pcm_dest, 0, len_temp);
                    }
                    // 获取播放数据
                    int index = 0,count= len_temp / samples_len;
                    int startIndex=0;
                    Console.WriteLine(">>>total:"+ len_temp + ",times:" + count + ",left:" + len_temp % samples_len);
                    while (index< count)
                    {
                        // 复制满足长度的数据
                        Array.Copy(pcm_dest, startIndex, bts, 0, samples_len);
                        // 添加数据
                        data.Add(new aa
                        {
                            len = samples_len,
                            pcm = bts
                        });
                        index++;
                        // 计算下一次的开始位置
                        startIndex = index * samples_len; 
                    }
                    // 暂存长度不够的数据
                    var left_len = len_temp % samples_len;
                    if (left_len > 0)
                    {
                        bts_left = new byte[left_len];
                        // 复制不够长度的数据
                        Array.Copy(pcm_dest, startIndex, bts_left, 0, left_len);
                    }
                }
            }

            //SDL.SDL_Delay(10);
        }
```




