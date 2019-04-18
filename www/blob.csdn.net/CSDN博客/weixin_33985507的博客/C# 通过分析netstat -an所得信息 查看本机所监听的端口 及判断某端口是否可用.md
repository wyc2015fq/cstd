# C# 通过分析netstat -an所得信息 查看本机所监听的端口 及判断某端口是否可用 - weixin_33985507的博客 - CSDN博客
2008年06月13日 08:56:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
以下功能的实现 相关代码实现 参照于网上 在此表示感谢
当然按本示例的操作思路 你可以实现自己想根的更多更有用的功能
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
// TextBox1 用于输入所要查询的端口
// TextBox2 用于显示 netstat -an 原信息
// TextBox3 用于显示 本机端口信息
// TextBox4 用于显示所查端口是否被占用
protected void Button1_Click(object sender, EventArgs e)
{
    this.TextBox2.Text = "";
    Process p = new Process();
    p.StartInfo.FileName = "cmd.exe";
    p.StartInfo.UseShellExecute = false;
    p.StartInfo.RedirectStandardInput = true;
    p.StartInfo.RedirectStandardOutput = true;
    p.StartInfo.RedirectStandardError = true;
    p.StartInfo.CreateNoWindow = true;
    p.Start();
    //查看本机端口占用情况
    p.StandardInput.WriteLine(" netstat -an ");
    p.StandardInput.WriteLine("exit");
    //
    StreamReader reader = p.StandardOutput;//截取输出流
    string strAllInfo = "";
    string strLocalInfo = "";        
    string strLine = reader.ReadLine();//每次读取一行
    int i = 0;
    while (!reader.EndOfStream)
    {
        strAllInfo += strLine + "\r\n";
        i++;
        if (i < 9)//去掉之前相关信息
        {
            /*
            Microsoft Windows [版本 5.2.3790]
            (C) 版权所有 1985-2003 Microsoft Corp.
            E:\Documents and Settings\Administrator>netstat -an
            Active Connections
              Proto  Local Address          Foreign Address        State
            */
        }
        else
        {
            if (strLine.Trim().Length > 0)
            {
                strLine = strLine.Trim();
                Regex r = new Regex(@"\s+");
                string[] strArr = r.Split(strLine);
                strLocalInfo += strArr[1] + "|\r\n";
            }
        }
        strLine = reader.ReadLine();//再下一行          
    }
    p.WaitForExit();//等待程序执行完退出进程
    p.Close();//关闭进程
    reader.Close();//关闭流
    this.TextBox2.Text = strAllInfo;//显示 netstat -an 原信息
    this.TextBox3.Text = strLocalInfo;//显示 本机端口信息
    //比对所查端口
    if (strLocalInfo.IndexOf(":" + this.TextBox1.Text + "|") >= 0)
    {
        this.TextBox4.Text = "已被占用";
    }
    else
    {
        this.TextBox4.Text = "可用";
    }
}
