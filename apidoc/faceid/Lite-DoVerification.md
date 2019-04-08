# Lite-DoVerification

## URL

 GET <https://api.megvii.com/faceid/lite/do>

**注意：**在生产环境中，请使用**HTTPS**的通信方式。HTTP方式的通信属于不安全链路，存在安全风险，请勿在生产环境中使用。在生产环境中使用HTTP方式的，将无法得到服务可靠性保障

## 说明

此接口提供 FaceID Lite 验证服务，通过 GetToken 接口获得 token，可利用此接口转跳到对应的网页验证。

## 参数

| 必选／可选 | 参数  | 类型   | 参数说明                 |
| :--------- | :---- | :----- | :----------------------- |
| 必选       | token | String | 由get_token返回的token。 |

## 返回及其他说明

此接口调用返回一个Web页面，所有的错误信息均通过可视化的Web页面返回。