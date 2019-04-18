# Http请求-使用cookie注入登录态访问 - z69183787的专栏 - CSDN博客
2017年11月25日 00:10:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2722
```java
@Test
    public void lotteryTest() throws InterruptedException {
        String locOrigin = "http://event.51ping.com"; //测试
        String drawLotteryUrl = locOrigin + "/emidas/lottery/siteweb/ajax/drawLottery" +
                "?activity=" + "03D39BCAF4AAC406" + //后续点评方给到
                "&prizePackage=" + "FAB7B93B21645FAC" + //后续点评方给到
                "&hint=true" +
                "&date=" + new Date().getTime();
        drawLotteryUrl = "http://event.51ping.com/normal/lottery/draw?cityId=1&activityId=FD42BF015E8CC541";
        while(true){
            sendGetCookie(drawLotteryUrl);
            Thread.sleep(5000);
        }
    }
```
```java
public static String sendGetCookie(String url) {
        String result = "";
        BufferedReader in = null;
        try {
            String urlNameString = url;
            URL realUrl = new URL(urlNameString);
            // 打开和URL之间的连接
            URLConnection connection = realUrl.openConnection();
            // 设置通用的请求属性
            connection.setRequestProperty("accept", "*/*");
            connection.setRequestProperty("connection", "Keep-Alive");
            connection.setRequestProperty("user-agent",
                    "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1;SV1)");
            connection.setRequestProperty("Cookie", "dper=6b1df3e8ff1a57f5082715517179ee3ec1a606f812a858e074a1a9ff8bd7b722");
            // 建立实际的连接
            connection.connect();
            // 获取所有响应头字段
            Map<String, List<String>> map = connection.getHeaderFields();
            // 遍历所有的响应头字段
            for (String key : map.keySet()) {
                //System.out.println(key + "--->" + map.get(key));
            }
            // 定义 BufferedReader输入流来读取URL的响应
            in = new BufferedReader(new InputStreamReader(
                    connection.getInputStream()));
            String line;
            while ((line = in.readLine()) != null) {
                result += line;
            }
//            String cookie = connection.getHeaderField("set-cookie");
//            System.out.println(cookie);
//
//            JSONObject jsonObject = new JSONObject(result);
//            System.out.println(jsonObject.getInt("code"));
//            System.out.println(jsonObject.getJSONObject("msg").getJSONObject("prize").getString("prizeName"));
            System.out.println(result);
        } catch (Exception e) {
            logger.info("发送GET请求出现异常！" + e.getMessage());
        }
        // 使用finally块来关闭输入流
        finally {
            try {
                if (in != null) {
                    in.close();
                }
            } catch (Exception e2) {
                logger.info(e2.getMessage());
            }
        }
        return result;
    }
```
