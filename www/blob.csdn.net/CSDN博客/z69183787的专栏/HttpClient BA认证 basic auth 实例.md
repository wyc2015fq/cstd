# HttpClient BA认证 basic auth 实例 - z69183787的专栏 - CSDN博客
2017年07月24日 11:40:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1766
httpclient 4.5
httpcore 4.4.1
jackson 2.4.4
```java
private static final ObjectMapper objectMapper;
    static {
        objectMapper = new ObjectMapper();
        //去掉默认的时间戳格式
        objectMapper.configure(SerializationFeature.WRITE_DATES_AS_TIMESTAMPS, false);
        //设置为中国上海时区
        objectMapper.setTimeZone(TimeZone.getTimeZone("GMT+8"));
        objectMapper.configure(SerializationFeature.WRITE_NULL_MAP_VALUES, false);
        //空值不序列化
        objectMapper.setSerializationInclusion(JsonInclude.Include.NON_NULL);
        //反序列化时，属性不存在的兼容处理
        objectMapper.getDeserializationConfig().withoutFeatures(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES);
        //序列化时，日期的统一格式
        objectMapper.setDateFormat(new SimpleDateFormat(DatePatternEnum.TimePattern.pattern));
        objectMapper.configure(SerializationFeature.FAIL_ON_EMPTY_BEANS, false);
        objectMapper.configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false);
        //单引号处理
        objectMapper.configure(com.fasterxml.jackson.core.JsonParser.Feature.ALLOW_SINGLE_QUOTES, true);
    }
    public static final String DateHeader = "Date";
    public static final String AuthorizationHeader = "Authorization";
    private static final String HMAC_SHA1_ALGORITHM = "HmacSHA1";
    private static final String CLIENT_ID = "iiiiii";
    private static final String CLIENT_SECRET = "xxxxxxxxxxxxx";
    public static HttpRequestBase authHttpRequest(HttpRequestBase httpRequest, String clientId, String secret) {
        String dateHeader = BAAuthUtil.getAuthDate(new Date());
        String authorizationHeader = BAAuthUtil.getAuthorization(
                httpRequest.getURI().getPath(), httpRequest.getMethod(), dateHeader, clientId, secret);
        httpRequest.setHeader("Content-Type", "application/x-www-form-urlencoded");
        //httpRequest.setHeader("Content-Type", "application/json");
        httpRequest.setHeader(BAAuthUtil.DateHeader, dateHeader);
        httpRequest.setHeader(BAAuthUtil.AuthorizationHeader, authorizationHeader);
        return httpRequest;
    }
    public static String getAuthorization(String uri, String method, String date, String clientId, String secret) {
        String stringToSign = method + " " + uri + "\n" + date;
        String signature = getSignature(stringToSign, secret);
        return "MWS" + " " + clientId + ":" + signature;
    }
    public static String getAuthDate(Date date) {
        DateFormat df = new SimpleDateFormat("EEE, d MMM yyyy HH:mm:ss z", Locale.ENGLISH);
        df.setTimeZone(TimeZone.getTimeZone("GMT"));
        return df.format(date);
    }
    private static String getSignature(String data, String secret) {
        String result;
        try {
            SecretKeySpec signingKey = new SecretKeySpec(secret.getBytes(), HMAC_SHA1_ALGORITHM);
            Mac mac = Mac.getInstance(HMAC_SHA1_ALGORITHM);
            mac.init(signingKey);
            byte[] rawHmac = mac.doFinal(data.getBytes());
            result = Base64.encodeToString(rawHmac);
        } catch (Exception e) {
            throw new IllegalStateException("Failed to generate HMAC : " + e.getMessage());
        }
        return result;
    }
    public static String httpGet(String api,String clientId,String secret) throws IOException {
        CloseableHttpClient client = HttpClientBuilder.create().build();
        HttpGet httpGet = new HttpGet(api);
        CloseableHttpResponse httpResponse = null;
        String result = null;
        try {
            httpResponse = client.execute(BAAuthUtil.authHttpRequest(httpGet, clientId, secret));
            HttpEntity httpEntity = httpResponse.getEntity();
            result = EntityUtils.toString(httpEntity);
            EntityUtils.consume(httpEntity);
        } catch (IOException e) {
            logger.error("httpGet",e);
        }finally {
            if(httpResponse != null){
                try {
                    httpResponse.close();
                } catch (IOException e) {
                    logger.error("httpGet",e);
                }
            }
            if(client != null){
                client.close();
            }
        }
        return result;
    }
    public static String httpPost(String api,String param,String clientId,String secret) throws IOException {
        CloseableHttpClient client = HttpClientBuilder.create().build();
        HttpPost httpPost = new HttpPost(api);
        CloseableHttpResponse httpResponse = null;
        String result = null;
        try {
            httpPost.setEntity(new StringEntity(param));
            //httpPost.setHeader("Content-Type", "application/json");
            httpResponse = client.execute(BAAuthUtil.authHttpRequest(httpPost, clientId, secret));
            HttpEntity httpEntity = httpResponse.getEntity();
            result = EntityUtils.toString(httpEntity);
            EntityUtils.consume(httpEntity);
        } catch (IOException e) {
            logger.error("httpPost",e);
        }finally {
            if(httpResponse != null){
                try {
                    httpResponse.close();
                } catch (IOException e) {
                    logger.error("httpPost",e);
                }
            }
            if(client != null){
                client.close();
            }
        }
        return result;
    }
    /**
     * map key http status,result
     * @param api
     * @param param
     * @param clientId
     * @param secret
     * @return
     * @throws IOException
     */
    public static Map<String,Object> httpPostForStatus(String api,String param,String clientId,String secret) throws IOException {
        Map<String,Object> map = null;
        CloseableHttpClient client = HttpClientBuilder.create().build();
        HttpPost httpPost = new HttpPost(api);
        CloseableHttpResponse httpResponse = null;
        try {
            httpPost.setEntity(new StringEntity(param));
            //httpPost.setHeader("Content-Type", "application/json");
            httpResponse = client.execute(BAAuthUtil.authHttpRequest(httpPost, clientId, secret));
            HttpEntity httpEntity = httpResponse.getEntity();
            map = new HashMap<String, Object>();
            map.put("status",httpResponse.getStatusLine().getStatusCode());
            map.put("result",EntityUtils.toString(httpEntity));
            EntityUtils.consume(httpEntity);
        } catch (IOException e) {
            logger.error("httpPost",e);
            throw e;
        }finally {
            if(httpResponse != null){
                try {
                    httpResponse.close();
                } catch (IOException e) {
                    logger.error("httpPost",e);
                }
            }
            if(client != null){
                client.close();
            }
        }
        return map;
    }
```
