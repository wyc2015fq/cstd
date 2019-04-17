# JavaScript WebSocket实现长连接通信连接重连 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年11月22日 19:31:52[boonya](https://me.csdn.net/boonya)阅读数：272








JavaScript-JS依靠WebSoket也可以像其他语言一样能够实现通信代码，不过使用的时候需要注意检查连接断开的情况。最基本的必须保证IP和端口都可以访问。

**目录**

[WebSocket代码及测试](#WebSocket%E4%BB%A3%E7%A0%81%E5%8F%8A%E6%B5%8B%E8%AF%95)

[调用初始化代码](#WebSocket%E5%88%9D%E5%A7%8B%E5%8C%96)

[通信示例代码](#WebSocket%E7%A4%BA%E4%BE%8B)

[测试效果](#WebSocket%E6%B5%8B%E8%AF%95%E6%95%88%E6%9E%9C)

[WebSocket连接关闭重连处理](#Websocket%E8%BF%9E%E6%8E%A5%E5%85%B3%E9%97%AD%E9%87%8D%E8%BF%9E%E5%A4%84%E7%90%86)

[连接断开问题](#WebSocket%E8%BF%9E%E6%8E%A5%E6%96%AD%E5%BC%80%E9%97%AE%E9%A2%98)

[代码实现重连](#WebSocket%E4%BB%A3%E7%A0%81%E5%AE%9E%E7%8E%B0%E9%87%8D%E8%BF%9E)

[重连效果](#%E9%87%8D%E8%BF%9E%E6%95%88%E6%9E%9C)

## WebSocket代码及测试

### 调用初始化代码

```java
CvNetVideo.Websocket = new WebSocket('ws://' + websocetHost + ":" + websocketPort);
 CvNetVideo.WebsocketCallbackLoader = new WebsocketCallbackLoader();
 CvNetVideo.WebsocketCallbackLoader.connect(CvNetVideo.Websocket);
```

### 通信示例代码

```java
/*
 * WebsocketCallback Loader
*/
import JT1078 from '../JX/JTT1078';

class WebsocketCallbackLoader {

    constructor() {
        this.websocket = undefined;
        this.onCallback = undefined;
        this.sendMessage = this.sendMessage;
        this.medisPackageAnalysis = new JT1078.MedisPackageAnalysis();
        this.splitData = new JT1078.SplitData();
        this.checkHelper = new JT1078.CheckHelper();
        //分包缓存数据 按SIM做key
        this.AllSimPackData = {};
    }

    connect(websocket) {
        this.websocket = websocket;
        this.websocket.onopen = this.openSocket.bind(this);
        this.websocket.onclose = this.closeSocket.bind(this);
        this.websocket.binaryType = 'arraybuffer';
        this.websocket.onmessage = this.receiveSocketMessage.bind(this);
    }

    closeSocket(e) {
        console.log('Websocket808 Disconnected!');
    }

    openSocket() {
        console.log('Websocket808 Open!');
    }

    receiveSocketMessage(event) {
        let data = new Uint8Array(event.data);
        this.splitData.SplitDataBy7E(data, 18, data.byteLength - 18, (bGps) => {
            if (bGps.length > 11 && this.checkHelper.CheckXOR(bGps, 0, bGps.length - 1) === bGps[bGps.length - 1])//效验通过
            {
                this.JX808DataPD(new Uint8Array(bGps));
            }
        });
    }
    JX808DataPD(bGps) {
        let head = new JT1078.JTHeader();
        head.FillByBinary(bGps, 0);

        if (head.PackInfo) {
            var sid = JT1078.GetFirstSerialNumber(head);
            let AllPackData = AllSimPackData[head.Sim];
            if (!AllPackData) {
                AllPackData = {};
                AllSimPackData[head.Sim] = AllPackData;
            }

            var pd = AllPackData[sid];
            if (pd) {
                if (!pd.PackData[head.PackInfo.Index]) {
                    pd.PackData.Count += 1;
                }
                pd.PackData[head.PackInfo.Index] = JT1078.GetMsbBody(head, bGps);
                pd.LastRTime = DateTime.Now;
                if (this.CheckPackByPD(pd))
                    AllPackData[sid] = null;
            }
            else {
                pd = {};
                pd.PackData = {};
                pd.PackData[head.PackInfo.Index] = JT1078.GetMsbBody(head, bGps);
                pd.PackData.Count = 1;
                pd.Head = new JT1078.JTHeader(head.MsgId, head.Sub, head.DataEncryptType, head.MsgLen, head.Sim, sid)
                pd.Head.PackInfo = new JT1078.JTPackageInfo(1, head.PackInfo.Sum);
                pd.StartTime = pd.LastRTime = DateTime.Now;
                AllPackData[sid] = pd;
            }

        } else {
            this.JX808Data(head, JT1078.GetMsbBody(head, bGps));
        }
    }
    CheckPackByPD(pd) {
        if (pd.PackData.Count === pd.Head.PackInfo.Sum)//判断数据包数和总包数是否相等
        {
            let tmpbuf = new Uint8Array();
            let index = 0;
            for (var i = 0; i < pd.Head.PackInfo.Sum; i++) {
                tmpbuf.set(pd.PackData[i], index);
                index += pd.PackData[i].byteLength;
            }
            //解析失败时返回true防止一直解析此错误数据
            try {
                this.JX808Data(pd.Head, tmpbuf);
            }
            catch (ex)
            {
                console.error(ex);
                return true;
            }
            return true;
        }
        return false;
    }
    JX808Data(head, bts) {
        switch (head.MsgId) {

            case 0x1205://终端上传音视频资源列表数据格式
                var videolist = new JT1078.JTVideoListInfo();
                videolist.FillByBinary(bts, 0);
                this.onCallback(videolist);
                break;
            case 0x1206://终端上传FTP完成通知
                break;
            default:
        }
    }

    sendMessage(data, callback) {
        this.onCallback = callback;
        switch (this.websocket.readyState) {
            case 0:
                console.log("Websocket808 readyState=CONNECTING");
                break;
            case 1:
                this.websocket.send(data);
                break;
            case 2:
                console.log("Websocket808 readyState=CLOSING");
                break;
            case 3:
                console.log("Websocket808 readyState=CLOSED");
                break;

        }

    }

}

export default WebsocketCallbackLoader;
```

### 测试效果

![](https://img-blog.csdnimg.cn/20181122193022334.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181122193050554.png)

## WebSocket连接关闭重连处理

### 连接断开问题

![](https://img-blog.csdnimg.cn/20181122192541232.png)

### 代码实现重连

改造一下调用代码和发送消息代码：

```java
static initWebsocket(websocetHost, websocketPort) {
        CvNetVideo.WebsocketCallbackLoader = new WebsocketCallbackLoader(websocetHost, websocketPort);
        CvNetVideo.WebsocketCallbackLoader.connect();
    }
```

```java
/*
 * WebsocketCallback Loader
*/
import JT1078 from '../JX/JTT1078';
import { setTimeout } from 'timers';

class WebsocketCallbackLoader {

    constructor(host, port) {
        this.host = host;
        this.port = port;
        this.websocket = undefined;
        this.onCallback = undefined;
        this.sendMessage = this.sendMessage;
        this.medisPackageAnalysis = new JT1078.MedisPackageAnalysis();
        this.splitData = new JT1078.SplitData();
        this.checkHelper = new JT1078.CheckHelper();
        //分包缓存数据 按SIM做key
        this.AllSimPackData = {};
    }

    connect() {
        var url = "ws://" + this.host + ":" + this.port;
        console.log("Websocket808 WebSocket connect ..." + url);
        this.websocket = new WebSocket(url);
        this.websocket.onopen = this.openSocket.bind(this);
        this.websocket.onclose = this.closeSocket.bind(this);
        this.websocket.binaryType = 'arraybuffer';
        this.websocket.onmessage = this.receiveSocketMessage.bind(this);
    }

    closeSocket(e) {
        console.log('Websocket808 Disconnected!');
    }

    openSocket() {
        console.log('Websocket808 Open!');
    }

    receiveSocketMessage(event) {
        let data = new Uint8Array(event.data);
        this.splitData.SplitDataBy7E(data, 18, data.byteLength - 18, (bGps) => {
            if (bGps.length > 11 && this.checkHelper.CheckXOR(bGps, 0, bGps.length - 1) === bGps[bGps.length - 1])//效验通过
            {
                this.JX808DataPD(new Uint8Array(bGps));
            }
        });
    }
    JX808DataPD(bGps) {
        let head = new JT1078.JTHeader();
        head.FillByBinary(bGps, 0);

        if (head.PackInfo) {
            var sid = JT1078.GetFirstSerialNumber(head);
            let AllPackData = AllSimPackData[head.Sim];
            if (!AllPackData) {
                AllPackData = {};
                AllSimPackData[head.Sim] = AllPackData;
            }

            var pd = AllPackData[sid];
            if (pd) {
                if (!pd.PackData[head.PackInfo.Index]) {
                    pd.PackData.Count += 1;
                }
                pd.PackData[head.PackInfo.Index] = JT1078.GetMsbBody(head, bGps);
                pd.LastRTime = DateTime.Now;
                if (this.CheckPackByPD(pd))
                    AllPackData[sid] = null;
            }
            else {
                pd = {};
                pd.PackData = {};
                pd.PackData[head.PackInfo.Index] = JT1078.GetMsbBody(head, bGps);
                pd.PackData.Count = 1;
                pd.Head = new JT1078.JTHeader(head.MsgId, head.Sub, head.DataEncryptType, head.MsgLen, head.Sim, sid)
                pd.Head.PackInfo = new JT1078.JTPackageInfo(1, head.PackInfo.Sum);
                pd.StartTime = pd.LastRTime = DateTime.Now;
                AllPackData[sid] = pd;
            }

        } else {
            this.JX808Data(head, JT1078.GetMsbBody(head, bGps));
        }
    }
    CheckPackByPD(pd) {
        if (pd.PackData.Count === pd.Head.PackInfo.Sum)//判断数据包数和总包数是否相等
        {
            let tmpbuf = new Uint8Array();
            let index = 0;
            for (var i = 0; i < pd.Head.PackInfo.Sum; i++) {
                tmpbuf.set(pd.PackData[i], index);
                index += pd.PackData[i].byteLength;
            }
            //解析失败时返回true防止一直解析此错误数据
            try {
                this.JX808Data(pd.Head, tmpbuf);
            }
            catch (ex)
            {
                console.error(ex);
                return true;
            }
            return true;
        }
        return false;
    }
    JX808Data(head, bts) {
        switch (head.MsgId) {

            case 0x1205://终端上传音视频资源列表数据格式
                var videolist = new JT1078.JTVideoListInfo();
                videolist.FillByBinary(bts, 0);
                this.onCallback(videolist);
                break;
            case 0x1206://终端上传FTP完成通知
                break;
            default:
        }
    }

    printState() {
        switch (this.websocket.readyState) {
            case 0:
                console.log("Websocket808 readyState=CONNECTING");
                break;
            case 1:
                console.log("Websocket808 readyState=OPEN");
                break;
            case 2:
                console.log("Websocket808 readyState=CLOSING");
                break;
            case 3:
                console.log("Websocket808 readyState=CLOSED");
                break;
        }
    }

    sendData(data)
    {
        this.websocket.send(data);
    }

    sendMessage(data, callback) {
        this.onCallback = callback;
        this.printState();
        if (this.websocket.readyState === 0) {
            setTimeout(() => {
                this.sendData(data);
            }, 3000);
        } else if (this.websocket.readyState === 2 || this.websocket.readyState === 3) {
            this.connect();
            setTimeout(() => {
                this.printState();
                this.sendData(data);
            }, 3000);
        } else {
            this.sendData(data);
        }

    }

}

export default WebsocketCallbackLoader;
```

### 重连效果

 下面是连接断开后重连效果：

![](https://img-blog.csdnimg.cn/20181123095026895.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181123101717374.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

注意：重连是异步调用的，使用setTimeout(func,ms) 调用，因为连接创建需要时间，直接连很有可能正在连接。

### 使用队列处理连接失败数据

```java
/*
 * WebsocketCallback Loader
*/
import JT1078 from '../JX/JTT1078';
import Queue from '../utils/queue';

class WebsocketCallbackLoader{

    constructor(host, port) {
        this.host = host;
        this.port = port;
        this.websocket = undefined;
        this.medisPackageAnalysis = new JT1078.MedisPackageAnalysis();
        this.splitData = new JT1078.SplitData();
        this.checkHelper = new JT1078.CheckHelper();
        //分包缓存数据 按SIM做key
        this.AllSimPackData = {};
        this.sendCache = new Queue();
        this.onCallback = {};
    }

    connect() {
        var url = "ws://" + this.host + ":" + this.port;
        this.websocket = new WebSocket(url);
        this.websocket.onopen = this.openSocket.bind(this);
        this.websocket.onclose = this.closeSocket.bind(this);
        this.websocket.binaryType = 'arraybuffer';
        this.websocket.onmessage = this.receiveSocketMessage.bind(this);
    }

    closeSocket(e) {
        console.log('Websocket808 Disconnected!');
    }

    openSocket() {
        console.log('Websocket808 Open!');
        while (this.sendCache.size() > 0) {
            this.sendData(this.sendCache.dequeue());
        }
    }

    receiveSocketMessage(event) {
        let data = new Uint8Array(event.data);
        this.splitData.SplitDataBy7E(data, 18, data.byteLength - 18, (bGps) => {
            if (bGps.length > 11 && this.checkHelper.CheckXOR(bGps, 0, bGps.length - 1) === bGps[bGps.length - 1])//效验通过
            {
                this.JX808DataPD(new Uint8Array(bGps));
            }
        });
    }
    JX808DataPD(bGps) {
        let head = new JT1078.JTHeader();
        head.FillByBinary(bGps, 0);

        if (head.PackInfo) {
            var sid = JT1078.GetFirstSerialNumber(head);
            let AllPackData = this.AllSimPackData[head.Sim];
            if (!AllPackData) {
                AllPackData = {};
                this.AllSimPackData[head.Sim] = AllPackData;
            }

            var pd = AllPackData[sid];
            if (pd) {  
                if (!pd.PackData[head.PackInfo.Index]) {
                    pd.PackData.Count += 1;
                    pd.PackData.totalLength += head.MsgLen;
                }
                pd.PackData[head.PackInfo.Index] = JT1078.GetMsbBody(head, bGps);
                pd.LastRTime = new Date();
                if (this.CheckPackByPD(pd))
                    AllPackData[sid] = null;
            }
            else {
                pd = {};
                pd.PackData = {};
                pd.PackData[head.PackInfo.Index] = JT1078.GetMsbBody(head, bGps);
                pd.PackData.Count = 1;
                pd.PackData.totalLength = head.MsgLen;
                pd.Head = new JT1078.JTHeader(head.MsgId, head.Sub, head.DataEncryptType, head.MsgLen, head.Sim, sid);
                pd.Head.PackInfo = new JT1078.JTPackageInfo(1, head.PackInfo.Sum);
                pd.StartTime = pd.LastRTime = new Date();
                AllPackData[sid] = pd;
            }

        } else {
            this.JX808Data(head, JT1078.GetMsbBody(head, bGps));
        }
    }
    CheckPackByPD(pd) {
        if (pd.PackData.Count === pd.Head.PackInfo.Sum)//判断数据包数和总包数是否相等
        {
            let tmpbuf = new Uint8Array(pd.PackData.totalLength);
            let index = 0;
            for (var i = 1; i <= pd.Head.PackInfo.Sum; i++) {
                tmpbuf.set(pd.PackData[i], index);
                index += pd.PackData[i].byteLength;
            }
            //解析失败时返回true防止一直解析此错误数据
            try {
                this.JX808Data(pd.Head, tmpbuf);
            }
            catch (ex) {
                console.error(ex);
                return true;
            }
            return true;
        }
        return false;
    }
    JX808Data(head, bts) {
        switch (head.MsgId) {

            case 0x1205://终端上传音视频资源列表数据格式
                var videolist = new JT1078.JTVideoListInfo();
                videolist.FillByBinary(bts, 0);
                this.onCallback[0x1205](videolist);
                break;
            case 0x1206://终端上传FTP完成通知
                var fileUploadEndInform = new JT1078.JTVideoFileUploadEndInform();
                fileUploadEndInform.FillByBinary(bts, 0);
                this.onCallback[0x1206](fileUploadEndInform);
                break;
            default:
        }
    }

    printState() {
        switch (this.websocket.readyState) {
            case 0:
                console.log("Websocket808 readyState=CONNECTING");
                break;
            case 1:
                console.log("Websocket808 readyState=OPEN");
                break;
            case 2:
                console.log("Websocket808 readyState=CLOSING");
                break;
            case 3:
                console.log("Websocket808 readyState=CLOSED");
                break;
        }
    }

    sendData(data)
    {
        this.websocket.send(data);
    }

    sendMessage(data, callback, id) {

        this.onCallback[id] = callback;
        if (this.websocket.readyState === 0) {
            this.sendCache.enqueue(data);
        } else if (this.websocket.readyState === 2 || this.websocket.readyState === 3) {
            this.printState();
            this.sendCache.enqueue(data);
            this.connect();
        } else {
            this.sendData(data);
        }

    }

}

export default WebsocketCallbackLoader;
```





