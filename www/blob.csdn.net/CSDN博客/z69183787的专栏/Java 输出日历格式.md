# Java 输出日历格式 - z69183787的专栏 - CSDN博客
2015年08月06日 17:40:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2368
```java
package com.wonders.task.sample.main; /**
 *
 */
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;
/**
 * 一个日期处理类，在该类中，构造函数<code>DateTest()</code>,系统会默认设置年份为当年<br>
 * 而<code>DateTest(int year)</code>,则可以自定义年份<br>
 *
 * <pre>
 * DateTest date = new DateTest();
 * </pre>
 *
 * or<br>
 *
 * <pre>
 * DateTest hd = new DateTest(2014);
 * </pre>
 *
 * 调用<code>printCalendar()</code>可以打印一年的日期<br>
 * 调用<code>printCurrentMonth()</code>可以打印当前月的日期<br>
 * ,当然，你也可以调用<br>
 * <code>printMonthOfYear()</code>设置一个参数，进行打印某个月的日期<br>
 * 这里提供一些参考方案:<br>
 *
 * <pre>
 * // 无参数，系统默认去当前年份
 * DateTest date = new DateTest();
 * date.printCalendar();
 * date.printCurrentMonth();
 * date.printMonthOfYear(4);
 * </pre>
 *
 * or<br>
 *
 * <pre>
 * // 设置为2014年
 * DateTest hd = new DateTest(2014);
 * hd.printCurrentMonth();
 * hd.printMonthOfYear(1);
 * </pre>
 *
 * @date 2013-4-27
 * @author hongten
 *
 */
public class DateTest {
    // MONTHS
    // ============================================
    public static final String JANUARY = "January";
    public static final String FEBUARY = "Febuary";
    public static final String MARCH = "March";
    public static final String APRIL = "April";
    public static final String MAY = "May";
    public static final String JUN = "Jun";
    public static final String JULY = "July";
    public static final String AUGUST = "August";
    public static final String SEPTERMBER = "Septermber";
    public static final String OCTOBER = "October";
    public static final String NOVEMBER = "November";
    public static final String DECEMBER = "December";
    /**
     * 年份
     */
    private int year;
    /**
     * 一月一日星期几(eg:2013-01-01-->星期二,所以<code>whatDayOnJanuaryOne = 2;</code>)
     */
    private int whatDayOnJanuaryOne;
    // main
    // ======================================
    public static void main(String[] args) throws Exception {
        // 无参数，系统默认去当前年份
        DateTest date = new DateTest();
         date.printCalendar();
        //date.printCurrentMonth();
        // date.printMonthOfYear(4);
        // 设置为2014年
        // DateTest hd = new DateTest(2014);
        // hd.printCurrentMonth();
        // hd.printMonthOfYear(1);
    }
    // 无参数，系统默认去当前年份
    public DateTest() {
        Calendar cal = Calendar.getInstance();
        this.year = cal.get(Calendar.YEAR);
        try {
            setWhatDayOnJanuaryOne(getJanuaryOne(year));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    // 有参数，设置年份
    public DateTest(int year) {
        this.year = year;
        try {
            setWhatDayOnJanuaryOne(getJanuaryOne(year));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    /**
     * 打印某个月的所有日期
     *
     * @param mon
     *            月份
     * @throws Exception
     */
    public void printMonthOfYear(int mon) throws Exception {
        if (mon < 1 || mon > 12) {
            System.out.println("你输入的月份[" + mon + "]不对，请检查在进行....");
            return;
        }
        GregorianCalendar now = new GregorianCalendar();
        // 获得一个Date对象
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        Date date = sdf.parse(year + "-" + mon + "-01");
        // 设置当前时间
        now.setTime(date);
        // 从日期中取得当前的月
        int month = now.get(Calendar.MONTH);
        // 设置now的日期为1
        now.set(Calendar.DAY_OF_MONTH, 1);
        // 得到now是一周的第几天
        int week = now.get(Calendar.DAY_OF_WEEK);
        // 打印日历头部标示
        System.out.println("日\t一\t二\t三\t四\t五\t六");
        // 打印当前日期前面的空格
        for (int i = Calendar.SUNDAY; i < week; i++) {
            System.out.print("\t");
        }
        // 打印日历主体
        while (now.get(Calendar.MONTH) == month) {
            int day = now.get(Calendar.DAY_OF_MONTH);
            // 对输出的日历进行对齐，小于10的加上一个空格
            if (day < 10) {
                System.out.print(" " + day + "\t");
            } else {
                System.out.print("" + day + "\t");
            }
            // 如果是周六，进行换行
            if (week == Calendar.SATURDAY) {
                System.out.println();
            }
            // 每次输出日期后，将日期增加一天
            now.add(Calendar.DAY_OF_MONTH, 1);
            // 重新获得一周的第几天
            week = now.get(Calendar.DAY_OF_WEEK);
        }
    }
    /**
     * 打印当前月的所有日期，这个不会因用户设置的年份而改变
     */
    public void printCurrentMonth() {
        GregorianCalendar now = new GregorianCalendar();
        // 获得一个Date对象
        Date date = new Date();
        // 设置当前时间
        now.setTime(date);
        // 从日期中取得当前的日
        int toDay = now.get(Calendar.DAY_OF_MONTH);
        // 从日期中取得当前的月
        int month = now.get(Calendar.MONTH);
        // 设置now的日期为1
        now.set(Calendar.DAY_OF_MONTH, 1);
        // 得到now是一周的第几天
        int week = now.get(Calendar.DAY_OF_WEEK);
        // 打印日历头部标示
        System.out.println("日\t一\t二\t三\t四\t五\t六");
        // 打印当前日期前面的空格
        for (int i = Calendar.SUNDAY; i < week; i++) {
            System.out.print("\t");
        }
        // 打印日历主体
        while (now.get(Calendar.MONTH) == month) {
            int day = now.get(Calendar.DAY_OF_MONTH);
            // 对输出的日历进行对齐，小于10的加上一个空格
            if (day < 10) {
                // 如果是当前日期，加上标示
                if (day == toDay) {
                    System.out.print(":)" + day + "(:\t");
                } else {
                    System.out.print(" " + day + "\t");
                }
            } else {
                // 如果是当前日期，加上标示
                if (day == toDay) {
                    System.out.print(":)" + day + "(:\t");
                } else {
                    System.out.print("" + day + "\t");
                }
            }
            // 如果是周六，进行换行
            if (week == Calendar.SATURDAY) {
                System.out.println();
            }
            // 每次输出日期后，将日期增加一天
            now.add(Calendar.DAY_OF_MONTH, 1);
            // 重新获得一周的第几天
            week = now.get(Calendar.DAY_OF_WEEK);
        }
    }
    /**
     * 获取year这一年的一月一号是星期几
     *
     * @param year
     *            年份
     * @return
     * @throws Exception
     */
    public int getJanuaryOne(int year) throws Exception {
        int[] weekDays = { 0, 1, 2, 3, 4, 5, 6 };
        Calendar cal = Calendar.getInstance();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        Date dt = sdf.parse(year + "-01-01");
        cal.setTime(dt);
        int w = cal.get(Calendar.DAY_OF_WEEK) - 1;
        if (w < 0)
            w = 0;
        return weekDays[w];
    }
    /**
     * 打印一年的所有月份
     */
    public void printCalendar() {
        for (int i = 1; i <= 12; i++) {
            String month = getMonth(i);
            printTitle(month);
            // 打印有31天的月份
            if (i == 1 || i == 3 || i == 5 || i == 7 || i == 8 || i == 10 || i == 12) {
                print31();
            }
            // 打印有30天的月份
            else if (i == 4 || i == 6 || i == 9 || i == 11) {
                print30();
            }
            // 打印二月份
            else if (i == 2) {
                printFebuary();
            }
            System.out.println();
        }
    }
    // 打印格式
    // ============================================== start
    /**
     * 打印二月份，每一年的二月份可能不相同，所以要单独处理
     */
    protected void printFebuary() {
        if (this.year % 4 == 0) {
            // 闰年
            printLeapYear();
        } else {
            // 平年
            printNonleapYear();
        }
    }
    /**
     * 闰年二月份打印格式
     */
    private void printLeapYear() {
        for (int j = 1; j <= 29; j++) {
            String tmp = "";
            if (j == 1) {
                for (int k = 1; k <= this.whatDayOnJanuaryOne % 7; k++) {
                    tmp = tmp + "      ";
                }
                tmp = tmp + "   " + j + "  ";
                if (this.whatDayOnJanuaryOne == 6) {
                    System.out.println(tmp);
                } else {
                    System.out.print(tmp);
                }
            } else if (j > 1 && j < 29) {
                if ((this.whatDayOnJanuaryOne + j) % 7 == 0) {
                    System.out.println("   " + j);
                } else {
                    if (j < 10) {
                        System.out.print("   " + j + "  ");
                    } else {
                        System.out.print("   " + j + " ");
                    }
                }
            } else if (j == 29) {
                System.out.println("   " + j);
                this.whatDayOnJanuaryOne = ((this.whatDayOnJanuaryOne + j) % 7);
            }
        }
    }
    /**
     * 打印平年二月份格式
     */
    private void printNonleapYear() {
        for (int j = 1; j <= 28; j++) {
            String tmp = "";
            if (j == 1) {
                for (int k = 1; k <= this.whatDayOnJanuaryOne % 7; k++) {
                    tmp = tmp + "      ";
                }
                tmp = tmp + "   " + j + "  ";
                if (this.whatDayOnJanuaryOne == 6) {
                    System.out.println(tmp);
                } else {
                    System.out.print(tmp);
                }
            } else if (j > 1 && j < 28) {
                if ((this.whatDayOnJanuaryOne + j) % 7 == 0) {
                    System.out.println("   " + j);
                } else {
                    if (j < 10) {
                        System.out.print("   " + j + "  ");
                    } else {
                        System.out.print("   " + j + " ");
                    }
                }
            } else if (j == 28) {
                System.out.println("   " + j);
                this.whatDayOnJanuaryOne = ((this.whatDayOnJanuaryOne + j) % 7);
            }
        }
    }
    /**
     * 打印有30天的月份
     */
    protected void print30() {
        for (int j = 1; j <= 30; j++) {
            String tmp = "";
            if (j == 1) {
                for (int k = 1; k <= this.whatDayOnJanuaryOne % 7; k++) {
                    tmp = tmp + "      ";
                }
                tmp = tmp + "   " + j + "  ";
                if (this.whatDayOnJanuaryOne == 6) {
                    System.out.println(tmp);
                } else {
                    System.out.print(tmp);
                }
            } else if (j > 1 && j < 30) {
                if ((this.whatDayOnJanuaryOne + j) % 7 == 0) {
                    System.out.println("   " + j);
                } else {
                    if (j < 10) {
                        System.out.print("   " + j + "  ");
                    } else {
                        System.out.print("   " + j + " ");
                    }
                }
            } else if (j == 30) {
                System.out.println("   " + j);
                this.whatDayOnJanuaryOne = ((this.whatDayOnJanuaryOne + j) % 7);
            }
        }
    }
    /**
     * 打印有31天的月份
     */
    protected void print31() {
        for (int j = 1; j <= 31; j++) {
            String tmp = "";
            if (j == 1) {
                for (int k = 1; k <= this.whatDayOnJanuaryOne % 7; k++) {
                    tmp = tmp + "      ";
                }
                tmp = tmp + "   " + j + "  ";
                if (this.whatDayOnJanuaryOne == 6) {
                    System.out.println(tmp);
                } else {
                    System.out.print(tmp);
                }
            } else if (j > 1 && j < 31) {
                if ((this.whatDayOnJanuaryOne + j) % 7 == 0) {
                    System.out.println("   " + j);
                } else {
                    if (j < 10) {
                        System.out.print("   " + j + "  ");
                    } else {
                        System.out.print("   " + j + " ");
                    }
                }
            } else if (j == 31) {
                System.out.println("   " + j);
                this.whatDayOnJanuaryOne = ((this.whatDayOnJanuaryOne + j) % 7);
            }
        }
    }
    /**
     * 打印每个月的标题
     *
     * @param month
     */
    protected void printTitle(String month) {
        System.out.println("             " + month + "  " + this.year + "          ");
        System.out.println("---------------------------------------------");
        System.out.println("   Sun   Mon   Tue   Wed   Thu   Fri   Sat");
    }
    // 打印格式
    // ============================================== end
    /**
     * 获取月份的英文名称
     *
     * @param m
     *            月份的数字表示
     * @return
     */
    public String getMonth(int m) {
        String month = "";
        switch (m) {
            case 1:
                month = JANUARY;
                break;
            case 2:
                month = FEBUARY;
                break;
            case 3:
                month = MARCH;
                break;
            case 4:
                month = APRIL;
                break;
            case 5:
                month = MAY;
                break;
            case 6:
                month = JUN;
                break;
            case 7:
                month = JULY;
                break;
            case 8:
                month = AUGUST;
                break;
            case 9:
                month = SEPTERMBER;
                break;
            case 10:
                month = OCTOBER;
                break;
            case 11:
                month = NOVEMBER;
                break;
            case 12:
                month = DECEMBER;
                break;
        }
        return month;
    }
    public int getYear() {
        return year;
    }
    public void setYear(int year) {
        this.year = year;
    }
    public int getWhatDayOnJanuaryOne() {
        return whatDayOnJanuaryOne;
    }
    public void setWhatDayOnJanuaryOne(int whatDayOnJanuaryOne) {
        this.whatDayOnJanuaryOne = whatDayOnJanuaryOne;
    }
}
```
```java
import java.text.ParseException;  
import java.text.SimpleDateFormat;  
import java.util.Calendar;  
import java.util.Scanner;  
  
public class 打印日历 {  
    public static void main(String[] args) throws ParseException {  
        Calendar cal = Calendar.getInstance();  
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");  
        Scanner input = new Scanner(System.in);  
        System.out.println("请输入要显示的年份");  
        int year = input.nextInt();  
        System.out.println("请输入要显示的月份");  
        int month = input.nextInt();  
        int count = getdataCount(year, month);  
        cal.setTime(sdf.parse(year + "-" + month + "-" + 1));  
        int first = cal.get(Calendar.DAY_OF_WEEK);  
        System.out.println("星期日\t星期一\t星期二\t星期三\t星期四\t星期五\t星期六");  
        for (int i = 1; i < first; i++) {  
            System.out.print("\t");  
        }  
        for (int i = 1; i <= count; i++) {  
            System.out.print(i + "\t");  
            cal.setTime(sdf.parse(year + "-" + month + "-" + i));  
            if (cal.get(Calendar.DAY_OF_WEEK) == 7) {  
                System.out.println();  
            }  
        }  
    }  
    private static int getdataCount(int year, int month) {  
        Calendar cal = Calendar.getInstance();  
        cal.set(Calendar.YEAR, year);  
        cal.set(Calendar.MONTH, month - 1);  
        int count = cal.getActualMaximum(Calendar.DATE);  
        return count;  
    }  
}
```
