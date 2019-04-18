# c# 可获取系统环境 - javalzy - 博客园
# [c# 可获取系统环境](https://www.cnblogs.com/javalzy/p/9517920.html)
c# 可获取系统环境, 启动进程执等 *.shell
[MenuItem("Tools/DesignHelper/Clean and Pull")]
    private static void CleanAndPull()
    {
        var proc = new System.Diagnostics.ProcessStartInfo();
        proc.UseShellExecute = true;
        proc.WorkingDirectory = Environment.GetEnvironmentVariable("KFC_TW");
        proc.Arguments = "";
        proc.FileName = "manager_pull.sh";
        proc.ErrorDialog = true;
        proc.WindowStyle = System.Diagnostics.ProcessWindowStyle.Normal;
        var p = System.Diagnostics.Process.Start(proc);
        p.WaitForExit();
        LogUtil.Debug("manager pull completed");
    }

