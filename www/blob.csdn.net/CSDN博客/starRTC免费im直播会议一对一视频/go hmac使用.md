# go hmac使用 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年02月07日 11:51:39[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：162
[https://github.com/danharper/hmac-examples](https://github.com/danharper/hmac-examples)
94 func generateSign(data, key []byte) string {
95 mac := hmac.New(sha1.New, key)
96 mac.Write(data)
97 expectedMAC := mac.Sum(nil)
98 return base64.StdEncoding.EncodeToString(expectedMAC)
99 }
signature := generateSign([]byte(data), []byte("123"))
