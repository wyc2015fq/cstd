# Excel数据导出，Java自定义注解 - z69183787的专栏 - CSDN博客
2016年06月03日 15:07:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1414
```java
@Documented
@Target({ElementType.METHOD,ElementType.FIELD})
@Retention(RetentionPolicy.RUNTIME)
public @interface ExcelColumn {
    String name() default "zhoushun";
}
```
```java
public class ActivityDataDetailBean implements Serializable {
    private static final long serialVersionUID = 7689111063501975678L;
    @ExcelColumn(name="活动ID")
    private int activityId;
    @ExcelColumn(name = "日期")
    private String date;
    @ExcelColumn(name = "城市")
    private String cityName;
    @ExcelColumn(name = "投放位置")
    private String slot;
    @ExcelColumn(name="pageId")
    private String topicId;
    @ExcelColumn(name = "页面PV")
    private Long pv;
    @ExcelColumn(name = "页面UV")
    private Long uv;
    @ExcelColumn(name = "页面点击数")
    private Long click;
    @ExcelColumn(name="页面到达率")
    private String reachRate;
    @ExcelColumn(name = "页面关闭率")
    private String closeRate;
    @ExcelColumn(name = "分享量")
    private Long shareCount;
    public int getActivityId() {
        return activityId;
    }
    public void setActivityId(int activityId) {
        this.activityId = activityId;
    }
    public String getDate() {
        return date;
    }
    public void setDate(String date) {
        this.date = date;
    }
    public String getCityName() {
        return cityName;
    }
    public void setCityName(String cityName) {
        this.cityName = cityName;
    }
    public String getSlot() {
        return slot;
    }
    public void setSlot(String slot) {
        this.slot = slot;
    }
    public Long getPv() {
        return pv;
    }
    public void setPv(Long pv) {
        this.pv = pv;
    }
    public Long getClick() {
        return click;
    }
    public void setClick(Long click) {
        this.click = click;
    }
    public String getReachRate() {
        return reachRate;
    }
    public void setReachRate(String reachRate) {
        this.reachRate = reachRate;
    }
    public String getCloseRate() {
        return closeRate;
    }
    public void setCloseRate(String closeRate) {
        this.closeRate = closeRate;
    }
    public Long getShareCount() {
        return shareCount;
    }
    public void setShareCount(Long shareCount) {
        this.shareCount = shareCount;
    }
    public String getTopicId() {
        return topicId;
    }
    public void setTopicId(String topicId) {
        this.topicId = topicId;
    }
    public Long getUv() {
        return uv;
    }
    public void setUv(Long uv) {
        this.uv = uv;
    }
}
```
```java
public class JxlHelper {
    private static Logger logger = Logger.getLogger(JxlHelper.class);
    /**
     * 用于分页的最大记录数
     */
    static int recordNum = 60000;
    @SuppressWarnings("unchecked")
    public static InputStream getExcel(List list) throws IOException,
            WriteException {
        //文件流
        ByteArrayOutputStream ostream = new ByteArrayOutputStream();
        WritableWorkbook book = Workbook.createWorkbook(ostream);
        WritableSheet ws = null;
        // 列标
        WritableFont wfColumn = new WritableFont(WritableFont.TIMES, 10,
                WritableFont.BOLD, false);
        WritableCellFormat wcfColumn = new WritableCellFormat(wfColumn);
        wcfColumn.setAlignment(Alignment.CENTRE);
        wcfColumn.setVerticalAlignment(jxl.format.VerticalAlignment.CENTRE);
        // 内容
        WritableCellFormat wcfCell = new WritableCellFormat();
        wcfCell.setAlignment(Alignment.CENTRE);
        wcfCell.setVerticalAlignment(jxl.format.VerticalAlignment.CENTRE);
        if (CollectionUtils.isEmpty(list)) {
            book.createSheet("Sheet",0).addCell(new Label(0, 0, "无数据", wcfColumn));
        }else {
            Class clazz = list.get(0).getClass();
            //reflect
            List<String> head = new ArrayList<String>();
            List<Method> getMethod = new ArrayList<Method>();
            try {
                getReflectInfo(clazz,head,getMethod);
            } catch (IntrospectionException e) {
                logger.info(e);
            }
            Label lable = null;
            int rowIndex = 0;
            int rCount = list.size() > 0 ? list.size() : 0; // 行数
            int sheetCount = getSheetCount(rCount);// 获取分页工作表的个数
            for (int i = 0; i < sheetCount; i++) {// 初始化工作表的个数
                book.createSheet("Sheet-" + i, i); // 添加一个工作表
            }
            for (int index = 0; index < sheetCount; index++) {
                ws = book.getSheet(index);// 获取工作簿的第一个工作表
                int j = 0;
                for (int i = 0; i <head.size(); i++) {
                    ws.addCell(new Label(j++, 0, head.get(i), wcfColumn));
                }
                int i = 0;
                for (int k = rowIndex; k < list.size(); k++) {
                    Object obj = list.get(k);
                    int r = 0;
                    i++;
                    if (recordNum > i) {
                        for (Method method : getMethod) {
                            try {
                                Object result = method.invoke(obj);
                                if (result instanceof Date) {
                                    result = DateUtils.format((Date) result,DateUtils.getSecondFormatter());
                                }
                                lable = new Label(r++, i, result == null ? "" : result.toString(), wcfCell);
                                ws.addCell(lable);
                            } catch (Exception e) {
                                logger.info(e);
                            }
                        }
                    } else {
                        rowIndex += recordNum - 1;
                        break;
                    }
                }
            }
        }
        book.write();
        book.close();
        InputStream istream = new ByteArrayInputStream(ostream.toByteArray());
        return istream;
    }
    private static void getReflectInfo(Class<?> c,List<String> head,List<Method> getMethod) throws IntrospectionException {
        Field[] fields = c.getDeclaredFields();
        for (int i = 0; i < fields.length; i++) {
            if (!fields[i].getName().equalsIgnoreCase("serialVersionUID") && !fields[i].getName().equalsIgnoreCase("$jacocoData")) {
                ExcelColumn excelColumns = fields[i].getAnnotation(ExcelColumn.class);
                if (excelColumns != null) {
                    head.add(excelColumns.name());
                } else {
                    head.add(fields[i].getName());
                }
                getMethod.add(new PropertyDescriptor(fields[i].getName(), c).getReadMethod());
            }
        }
    }
    /**
     * 确定分页的个数
     *
     * @param rCount 总得记录条数
     * @return
     */
    private static int getSheetCount(int rCount) {
        if (recordNum <= 0)
            return 1;
        if (rCount <= 0)
            return 1;
        int n = rCount % (recordNum - 1); // 余数
        if (n == 0) {
            return rCount / recordNum;
        } else {
            return (int) (rCount / (recordNum - 1)) + 1;
        }
    }
    /**
     * 确定每页的记录数
     *
     * @param rCount 总得记录条数
     * @param page   当前页码
     * @return
     */
    private static int getRows(int rCount, int page) {
        if (recordNum <= 0)
            return rCount;
        if (rCount - page * recordNum >= 0)
            return recordNum;
        else
            return rCount % recordNum; // 余数
    }
}
```
