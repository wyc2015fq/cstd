# angular源码分析之StaticInjector - weixin_33985507的博客 - CSDN博客
2018年12月14日 11:43:33[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
上一篇说到了平台实例在初始化的时候会创建根注入器,那现在就一起看看注入器是如何创建的,又是如何工作的.(*所有引用的代码都被简化了*)
#### 创建注入器
程序初始化时调用的创建根注入器的静态方法:
```
abstract class Injector{
  static create(options: StaticProvider[]|{providers: StaticProvider[], parent?: Injector, name?: string},parent?: Injector): Injector {
      if (Array.isArray(options)) {
          return new StaticInjector(options, parent);
      } else {
      return new StaticInjector(options.providers, options.parent, options.name || null);
  }
}
```
调用此方法会返回一个`StaticInjector`类型的实例(也就是注入器).
`StaticInjector`类
```
export class StaticInjector implements Injector {
  readonly parent: Injector;
  readonly source: string|null;
  private _records: Map<any, Record>;
  constructor(providers: StaticProvider[], parent: Injector = NULL_INJECTOR, source: string|null = null) {
    this.parent = parent;
    this.source = source;
    const records = this._records = new Map<any, Record>();
    records.set(Injector, <Record>{token: Injector, fn: IDENT, deps: EMPTY, value: this, useNew: false});
    records.set(INJECTOR, <Record>{token: INJECTOR, fn: IDENT, deps: EMPTY, value: this, useNew: false});
    recursivelyProcessProviders(records, providers);
  }
}
```
注入器的构造函数在初始化过程中的操作:
- 设置当前注入器的父注入器
- 设置注入器的源
- 新建注册表(`_records`属性,是一个`Map`类型)
- 将参数`providers`全部添加到注册表中
向注册表中添加服务调用了`recursivelyProcessProviders`函数
```
const EMPTY = <any[]>[];
const MULTI_PROVIDER_FN = function (): any[] { return Array.prototype.slice.call(arguments) };
function recursivelyProcessProviders(records: Map<any, Record>, provider: StaticProvider) {
    if (provider instanceof Array) {
      for (let i = 0; i < provider.length; i++) {
        recursivelyProcessProviders(records, provider[i]);
      }
    } else (provider && typeof provider === 'object' && provider.provide) {
      let token = resolveForwardRef(provider.provide);// 方法`resolveForwardRef`的作用可能是向前兼容,可以忽略
      const resolvedProvider = resolveProvider(provider);
      if (provider.multi === true) {
        let multiProvider: Record | undefined = records.get(token);
        if (multiProvider) {
          if (multiProvider.fn !== MULTI_PROVIDER_FN) {
            throw multiProviderMixError(token);
          }
        } else {
          records.set(token, multiProvider = <Record>{
            token: provider.provide,
            deps: [],
            useNew: false, // 这个值在后面获取依赖实例的时候会用到,当做判断条件
            fn: MULTI_PROVIDER_FN,
            value: EMPTY // 这个值在后面获取依赖实例的时候会用到,当做判断条件
          });
        }
        token = provider;
        multiProvider.deps.push({ token, options: OptionFlags.Default });
      }
      records.set(token, resolvedProvider);
    }
}
```
`recursivelyProcessProviders`函数具体的执行过程:
如果`provider`是个数组,那就遍历后依次调用此方法.
如果`provider`是个对象:
1  获取`token`
`  let token = resolveForwardRef(provider.provide);`
2  调用`resolveProvider`方法处理服务中可能出现的属性和依赖,返回一个`Record`对象,此对象会作为`token`的值<!-- (`useValue`,`useClass`,`deps`,`useExisting`,`useFactory`) -->
```
function resolveProvider(provider: SupportedProvider): Record {
  const deps = computeDeps(provider);
  let fn: Function = function (value) { return value };
  let value: any = [];
  // useUew用来标识是否需要 new 
  let useNew: boolean = false;
  let provide = resolveForwardRef(provider.provide);
  if (USE_VALUE in provider) {
    value = provider.useValue;
  } else if (provider.useFactory) {
    fn = provider.useFactory;
  } else if (provider.useExisting) {
    //do nothing
  } else if (provider.useClass) {
    useNew = true;
    fn = resolveForwardRef(provider.useClass);
  } else if (typeof provide == 'function') {
    useNew = true;
    fn = provide;
  } else {
    throw staticError('StaticProvider does not have [useValue|useFactory|useExisting|useClass] or [provide] is not newable', provider);
  }
  return { deps, fn, useNew, value }; // provider中不同的属性会返回包含不同值的对象
}
```
这个方法会先调用`computeDeps`函数处理服务需要的依赖,它将`useExisting`类型的服务也转换成`deps`,最后返回`[{ token, OptionFlags }]`形式的数组(`OptionFlags`是枚举常量)
```
function computeDeps(provider: StaticProvider): DependencyRecord[] {
    let deps: DependencyRecord[] = EMPTY;
    const providerDeps: any[] = provider.deps;
    if (providerDeps && providerDeps.length) {
      deps = [];
      for (let i = 0; i < providerDeps.length; i++) {
        let options = OptionFlags.Default;
        let token = resolveForwardRef(providerDeps[i]);
        deps.push({ token, options });
      }
    } else if ((provider as ExistingProvider).useExisting) {
      const token = resolveForwardRef((provider as ExistingProvider).useExisting);
      deps = [{ token, options: OptionFlags.Default }];
    } 
    return deps;
  }
```
`resolveProvider`函数最终返回的`Record`对象有一个缺省值:
```
{
  deps:[], // 包含依赖时 [{ token, options },{ token, options }]
  fn:function(value) { return value },
  useNew:false,
  value:[]
}
```
执行过程中会根据`provider`不同的属性修改`Record`对象的变量为不同的值:
- `useValue` : 修改`value`为`useValue`的值
- `useFactory` : 修改`fn`为对应的函数
- `useClass` 或 `typeof provide == 'function'`(令牌为一个函数时) : 修改`fn`为对应的函数,并设置`useNew`为`true`
- `useExisting` : 不做修改,直接使用默认值
3  如果是多处理服务(`multi:ture`)且为首次注册,那么在注册表中额外注册一个占位的`Record`
```
records.set(token, multiProvider = <Record>{
  token: provider.provide,
  deps: [],
  useNew: false,
  fn: MULTI_PROVIDER_FN,
  value: EMPTY
});
```
4  非多处理服务以`token`为键,多处理服务以`provider`对象为键,返回的`Record`对象为值,存入注册表`records`中
#### 从注入器中获取实例
服务注册完,下一步就是怎么从注入器中获取服务的实例了,这会调用`StaticInjector`的`get`方法
```
export class StaticInjector implements Injector {
  get(token: any, notFoundValue?: any, flags: InjectFlags = InjectFlags.Default): any {
    // 获取token对应的record
    const record = this._records.get(token);
    return resolveToken(token, record, this._records, this.parent, notFoundValue, flags);
}
```
`get`方法调用了`resolveToken`函数,这个函数会返回`token`对应的实例(就是被注入的对象)
```
const EMPTY = <any[]>[];
const CIRCULAR = IDENT;
const IDENT = function <T>(value: T): T { return value };
function resolveToken(token: any, record: Record | undefined, records: Map<any, Record>, parent: Injector,
  notFoundValue: any, flags: InjectFlags): any {
  let value;
  if (record && !(flags & InjectFlags.SkipSelf)) {
    value = record.value;
    if (value == CIRCULAR) {
      throw Error(NO_NEW_LINE + 'Circular dependency');
    } else if (value === EMPTY) {
      record.value = CIRCULAR;
      let obj = undefined;
      let useNew = record.useNew;
      let fn = record.fn;
      let depRecords = record.deps;
      let deps = EMPTY;
      if (depRecords.length) {
        deps = [];
        for (let i = 0; i < depRecords.length; i++) {
          const depRecord: DependencyRecord = depRecords[i];
          const options = depRecord.options;
          const childRecord = options & OptionFlags.CheckSelf ? records.get(depRecord.token) : undefined;
          deps.push(tryResolveToken(
            depRecord.token,
            childRecord,
            records,
            !childRecord && !(options & OptionFlags.CheckParent) ? NULL_INJECTOR : parent,
            options & OptionFlags.Optional ? null : Injector.THROW_IF_NOT_FOUND,
            InjectFlags.Default));
        }
      }
      record.value = value = useNew ? new (fn as any)(...deps) : fn.apply(obj, deps);
    }
  } else if (!(flags & InjectFlags.Self)) {
    value = parent.get(token, notFoundValue, InjectFlags.Default);
  }
  return value;
}
```
函数中会先判断当前请求的`token`是否存在,如果不存在则去当前注入器的父注入器中寻找,如果存在:
获取`token`对应的`record`
判断`record.value`是否为`[]`(非`useValue`类型的服务/多处理服务的默认值是`[]`):
`ture` : 非`useValue`类型的服务/多处理服务或此服务没有被创建过
- 查看是否包含依赖,包含则优先创建依赖的实例,也是调用这个函数
- 根据`record.fn`创建当前`token`对应的实例并更新`record.value`(这里需要根据`record.useNew`来判断是否需要用`new`来实例化,比如`useFactory`类型就不需要`new`,而`useExisting`更是直接返回了`deps`)
- 返回这个值
`false` : `useValue`类型的服务或此服务已经被创建过
- 直接返回这个值
