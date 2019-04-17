# Java定时多线程任务实现(TimerTask) - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年08月19日 11:22:25[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4213








                
java中实现多线程，可通过继承Thread类或实现Runable接口。实现Runnable接口比继承Thread类所具有的优势：适合多个相同的程序代码的线程去处理同一个资源；可以避免java中的单继承的限制；增加程序的健壮性，代码可以被多个线程共享，代码和数据独立。

这里采用TimerTask来定时执行任务。TimerTask是一个抽象类，实现了Runnable接口，所以具备了多线程的能力。



```java
package ct.tool;

import java.util.Calendar;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

public class smooass {
	
	public static class FtpFileSnapshotTask extends TimerTask{  
		//每隔5分钟取ftp服务器上指定目录的文件名
		  
	    private int id;  
	    public FtpFileSnapshotTask(int id){   
	        this.id = id;  
	    }  
	      
	    @Override  
	    public void run() {   
	        System.out.println("线程"+ id +":  正在 执行。。");    
	        //System.gc();  
	    }      
	}  
	public static class BaseLacCiSnapshotTask extends TimerTask{  
		//每天零点更新基站信息  
	    private int id;  
	    public BaseLacCiSnapshotTask(int id){   
	        this.id = id;  
	    }  
	      
	    @Override  
	    public void run() {   
	        System.out.println("线程"+ id +":  正在 执行。。");    
	        //System.gc();  
	    }      
	}  
	 // 增加或减少天数  
    public static Date addDay(Date date, int num) {  
        Calendar startDT = Calendar.getInstance();  
        startDT.setTime(date);  
        startDT.add(Calendar.DAY_OF_MONTH, num);  
        return startDT.getTime();  
    }  
    
    private static final long PERIOD_DAY = 24 * 60 * 60 * 1000;   //时间间隔(一天)  
	public static void main(String[] args) {  
        Timer timer = new Timer(); 
        //启动ftp文件获取
        FtpFileSnapshotTask ftpTask=new FtpFileSnapshotTask(1);
        timer.schedule(ftpTask, 1000, 5*60*1000);// 1秒后启动任务,以后每隔5分执行一次线程           
        //启动基站文件获取
        Calendar calendar = Calendar.getInstance();  
        calendar.set(Calendar.HOUR_OF_DAY, 1); //凌晨1点  
        calendar.set(Calendar.MINUTE, 0);  
        calendar.set(Calendar.SECOND, 0);  
        Date date=calendar.getTime(); //第一次执行定时任务的时间  
        //如果第一次执行定时任务的时间 小于当前的时间  
        //此时要在 第一次执行定时任务的时间加一天，以便此任务在下个时间点执行。如果不加一天，任务会立即执行。  
        if (date.before(new Date())) {  
            date = smooass.addDay(date, 1);  
        }  
        BaseLacCiSnapshotTask baseTask=new BaseLacCiSnapshotTask(2);
        timer.schedule(baseTask,date,PERIOD_DAY);//以date为参数，指定某个时间点执行线程           
    }  
}
```





