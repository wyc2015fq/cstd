# C# winform右击导入手机号码 - weixin_33985507的博客 - CSDN博客
2016年11月23日 23:08:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
```
private void 导入手机号ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog Openfile = new OpenFileDialog())
            {
                Openfile.Filter = "文本文件|*.txt";
                Openfile.Multiselect = false;
                if (Openfile.ShowDialog() == DialogResult.OK)
                {
                    Thread threadfile = new Thread(() => ReadFile(Openfile.FileName));
                    threadfile.IsBackground = true;
                    threadfile.Start();
                }
            }
        }
        private void ReadFile(string filename)
        {
            txtlog.Invoke(new Action(() =>
            {
                txtlog.AppendText("开始读取手机号码".SetLog());
            }));
            var file = File.Open(filename, FileMode.Open);
            int num = 0;
            int goods = 0;
            int repeat = 0;
            using (var stream = new StreamReader(file))
            {
                while (!stream.EndOfStream)
                {
                    lock (lock_send)
                    {
                        if (sendlist.Count > 99)
                        {
                            break;
                        }
                    }
                    num++;
                    string linetemp = stream.ReadLine().Trim();
                    if (IsTel(linetemp))
                    {
                        lock (lock_send)
                        {
                            var data = sendlist.Where(m => m.Tel == linetemp).FirstOrDefault();
                            if (data != null)
                            {
                                repeat++;
                                continue;
                            }
                        }
                        goods++;
                        SendTel _send = new SendTel();
                        _send.Tel = linetemp;
                        _send.sms_status = status.待发送;
                        _send.send_time = null;
                        ListViewItem item = new ListViewItem(_send.Tel);
                        item.SubItems.Add(_send.sms_status.ToString());
                        item.SubItems.Add(_send.send_time);
                        item.SubItems.Add("");
                        listSend.Invoke(new Action(() =>
                        {
                            ListViewItem backitem = listSend.Items.Add(item);
                            dic.Add(_send.Tel, backitem);
                        }));
                        lock (lock_send)
                        {
                            sendlist.Add(_send);
                        }
                    }
                }
            }
            txtlog.Invoke(new Action(() =>
            {
                string log = string.Format("添加完成!有效数据为:{0},过滤重复数据:{1},总数据:{2}", goods.ToString(), repeat.ToString(), num.ToString());
                txtlog.AppendText(log.SetLog());
            }));
        }
        /// <summary>
        /// 验证手机号是否合法
        /// </summary>
        /// <param name="tel"></param>
        /// <returns></returns>
        public static bool IsTel(string tel)
        {
            try
            {
                if (string.IsNullOrEmpty(tel) || tel.Length != 11)
                {
                    return false;
                }
                return System.Text.RegularExpressions.Regex.IsMatch(tel, @"^[1]+[3,4,5,6,7,8]+\d{9}");
            }
            catch (Exception ex)
            {
                return false;
            }
        }
```
很久以前写的代码了，读取txt文件，一行一个手机号码，
好像要求txt文本文件必须是utf8格式，否则读取出来会乱码。
