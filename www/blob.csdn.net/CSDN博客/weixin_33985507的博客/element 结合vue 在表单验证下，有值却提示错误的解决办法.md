# element 结合vue 在表单验证下，有值却提示错误的解决办法 - weixin_33985507的博客 - CSDN博客
2018年01月21日 13:12:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：79
**绑定的值与规则指定的值一定要相同**
#### 第一步：加上rules  ref
```
<el-form :model="ruleForm" :rules="rules" ref="ruleForm" label-width="100px" class="demo-ruleForm">
```
#### 第二步： 加上prop
```
<el-form-item label="活动名称" prop="name"> <el-input v-model="ruleForm.name"></el-input> </el-form-item>
```
#### 第三步：
```
rules: {
          name: [
            { required: true, message: '请输入活动名称', trigger: 'blur' },
            { min: 3, max: 5, message: '长度在 3 到 5 个字符', trigger: 'blur' }
          ],
　　　　　resource: [
            { required: true, message: '请选择活动资源', trigger: 'change' }
          ],
}
```
这里加了required的话  prop的后面就不用加了；这里其他的详细验证看文档
#### 第四部：点击提交表单
##### 这里有一个地方切记，<el-button type="primary" @click="submitForm('ruleForm')">提交表单</el-button>这里一定要引号引起来，这样问题就解决了
```
submitForm(formName) {
        this.$refs[formName].validate((valid) => {
            if (valid) {
                // 这里是验证成功后该干嘛干嘛
             } else {
                   console.log('error submit!!');
                  return false;
             }
        });
  },
```
