# Angular 响应式表单之表单分组 - weixin_33985507的博客 - CSDN博客
2019年02月22日 21:18:41[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
# 1、案例需求
表单提交，表单全部校验成功才能提交，当表单校验错误，表单边框变红，同时有错误提示信息，有重置功能
- [在线预览](https://stackblitz.com/github/h266094/angular/tree/example-nested-formgroup)
- [git仓库](https://github.com/h266094/angular/tree/example-nested-formgroup)
# 2、名词解释
在分析代码之前，首先明确 FormControl、formControl、formControlName、FormGroup、formGroup、formGroupName、FormArray、formArray、formArrayName 都是什么意思以及它们的用法。
## 2.1、FormControl
- FormControl：跟踪独立表单控件的值和验证状态。它和 FormGroup 和 FormArray 是 Angular 表单的三大基本构造块之一。它扩展了 AbstractControl 类，并实现了关于访问值、验证状态、用户交互和事件的大部分基本功能。
当使用响应式表单时，FormControl 类是最基本的构造块。要注册单个的表单控件，在组件中导入 FormControl 类，并创建一个 FormControl 的新实例，把它保存在类的某个属性中。
```
export class AppComponent implements OnInit {
    const control = new FormControl('', Validators.required);
    console.log(control.value);      // ''
    console.log(control.status);     // 'INVALID'
}
```
在组件类中创建了控件之后，还要把它和模板中的一个表单控件关联起来，为表单控件添加 formControl 绑定。
```
<label>
  Name:
  <input type="text" [formControl]="name">
</label>
```
- formControl：是一个输入指令，接受 FormControl 的实例，在模版中使用。
- formControlName: 也是输入指令，但是它接受的是一个字符串，同 formGroup 指令配合使用。
```
<div>
  <input type="text" [formControl]="myForm.controls.firstName"/>
  <input type="text" [formControl]="myForm.controls.lastName"/>
  <input type="text" [formControl]="myForm.controls.email"/>
  <input type="text" [formControl]="myForm.controls.title"/>
</div>
//等同于
<div [formGroup]="myForm">
  <input type="text" formControlName="firstName"/>
  <input type="text" formControlName="lastName"/>
  <input type="text" formControlName="email"/>
  <input type="text" formControlName="title"/>
</div>
```
## 2.2、FormGroup
- FormGroup：跟踪一组 FormControl 实例的值和有效性状态
FormGroup 把每个子 FormControl 的值聚合进一个对象，它的 key 是每个控件的名字。它通过归集其子控件的状态值来计算出自己的状态。如果组中的任何一个控件是无效的，那么整个组就是无效的。
## 2.3、FormArray
- FormArray：跟踪一个控件数组的值和有效性状态
FormArray 聚合了数组中每个表单控件的值。它会根据其所有子控件的状态总结出自己的状态。如果 FromArray 中的任何一个控件是无效的，那么整个数组也会变成无效的。
- FormControl、FormGroup、FormArray 类 用法一致
- formControl、formGroup、formArray 输入指令 值为对应类的实例 用法一致
- formControlName、formGroupName、formArrayName 输入指令 值为字符串 用法一直
# 3、代码分析
fromGroup 可以然我们对表单内容进行分组，方便我们在语义上区分不同类型的输入，本例中，地址细分为“省”、“市”、“区”。
```
this.formGroup = this.fb.group({
      name: ['', nameValidator()],
      age: ['', ageValidator()],
      sex: ['', sexValidator()],
      address: this.fb.group({
        province: ['', requiredValidator('请输入省')],
        city: ['', requiredValidator('请输入市')],
        district: ['', requiredValidator('请输入区')]
      })
    });
```
address 此时不是 fromControl 而是 formGroup。
```
<div class="form-group"
    formGroupName="address">
    <label>地址：</label>
    <div>
      <label>省：</label>
      <input type="text"
        formControlName="province">
      <p>{{errorMessage('province')}}</p>
    </div>
    <div>
      <label>市：</label>
      <input type="text"
        formControlName="city">
      <p>{{errorMessage('city')}}</p>
    </div>
    <div>
      <label>区：</label>
      <input type="text"
        formControlName="district">
      <p>{{errorMessage('district')}}</p>
    </div>
  </div>
```
在获取 省市区的 formControl 时，可以通过这样获取
```
// 太复杂了
this.formGroup.controls['address'].controls['province'];
// 同样复杂
this.formGroup.get('address').controls['province'];
// 还好
this.formGroup.get(['address', 'province']);
```
第三种方式虽然简单，但是不够完美，get方法不能一步到位，必须同时传入 formGroupName 和 formControlName。因此在查看单个表单是否有错误信息时，必须先判断 formControlName 是子组件还是孙子组件。
```
errorMessage(formControlName: string): string {
    let control: AbstractControl;
    if (this.formGroup.contains(formControlName)) {
      control = this.formGroup.get(formControlName);
    } else {
      control = this.formGroup.get(['address', formControlName]);
    }
    return ((control.touched || control.dirty) && control.invalid) ? control.errors.message : '';
  }
```
contains方法：检查组内是否有一个具有指定名字的已启用的控件，存在返回 true，不存在返回 false。
