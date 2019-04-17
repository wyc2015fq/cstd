# Solve Error: MissingSchemaError: Schema hasn't been registered for model "YourModel". - Grandyang - 博客园







# [Solve Error: MissingSchemaError: Schema hasn't been registered for model "YourModel".](https://www.cnblogs.com/grandyang/p/10049209.html)







使用MongoDB的时候，如果遇到下面这个错误：



```
/home/ec2-user/YourProject/node_modules/mongoose/lib/index.js:391
      throw new mongoose.Error.MissingSchemaError(name);
      ^
MissingSchemaError: Schema hasn't been registered for model "YourModel".
Use mongoose.model(name, schema)
    at new MissingSchemaError (/home/ec2-user/YourProject/node_modules/mongoose/lib/error/missingSchema.js:20:11)
    at Mongoose.model (/home/ec2-user/YourProject/node_modules/mongoose/lib/index.js:391:13)
    at Object.<anonymous> (/home/ec2-user/YourProject/YourScript.js:4:27)
    at Module._compile (internal/modules/cjs/loader.js:722:30)
    at Object.Module._extensions..js (internal/modules/cjs/loader.js:733:10)
    at Module.load (internal/modules/cjs/loader.js:620:32)
    at tryModuleLoad (internal/modules/cjs/loader.js:560:12)
    at Function.Module._load (internal/modules/cjs/loader.js:552:3)
    at Function.Module.runMain (internal/modules/cjs/loader.js:775:12)
    at startup (internal/bootstrap/node.js:300:19)
    at bootstrapNodeJSCore (internal/bootstrap/node.js:826:3)
```



一般来说是MongoDB数据库的Model没有链上，在YourScript.js的开头，一般下面几行不能少：



```
require('./yourModel.js');
const mongoose = require('mongoose');
mongoose.connect("mongodb://localhost/YourDatabase");
const YourModel = mongoose.model('YourModel');
```



然后yourModel.js的内容可以为：



```
const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const YourModelSchema = new Schema({
    something1: { type: String, required: '`something1` must be filled', unique: true },
    something2: { type: String },
});

module.exports = mongoose.model('YourModel', YourModelSchema);
```














