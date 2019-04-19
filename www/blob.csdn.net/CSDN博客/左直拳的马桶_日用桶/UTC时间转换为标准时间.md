# UTC时间转换为标准时间 - 左直拳的马桶_日用桶 - CSDN博客
2011年01月14日 15:46:00[左直拳](https://me.csdn.net/leftfist)阅读数：1928
        public DateTime UTCToDateTime(Int64 utc)
        {
            DateTime dtZone = new DateTime(1970, 1, 1, 0, 0, 0);
            dtZone = dtZone.AddSeconds(utc);
            return dtZone.ToLocalTime();
        }
MessageBox.Show(UTCToDateTime
(1294675200).ToString());//得到2011-1-11 00：00：00
