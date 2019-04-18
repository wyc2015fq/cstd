# UGUI内核大探究（十七）拾遗补缺 - 凯奥斯的注释 - CSDN博客





2016年09月19日 13:51:08[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1553
所属专栏：[UGUI内核大探究](https://blog.csdn.net/column/details/12897.html)









IndexedSet是UGUI的内部类（internal class），是一个不会存在相同对象的表，如果有需要的话可以直接复制一份到工程里。

按照惯例，附上[UGUI源码下载地址](https://bitbucket.org/Unity-Technologies/ui/downloads?tab=downloads)。


主要看这些代码：



```
readonly List<T> m_List = new List<T>();
        Dictionary<T, int> m_Dictionary = new Dictionary<T, int>();

        public void Add(T item)
        {
            if (m_Dictionary.ContainsKey(item))
                return;

            m_List.Add(item);
            m_Dictionary.Add(item, m_List.Count - 1);
        }
```

其实就是额外维护了一个m_Dictionary，item做key，item在m_List的下标做value。


Add方法很简单，但是Remove就稍微麻烦一点。



```
public bool Remove(T item)
        {
            int index = -1;
            if (!m_Dictionary.TryGetValue(item, out index))
                return false;

            RemoveAt(index);
            return true;
        }
```
RemoveAt：

```
public void RemoveAt(int index)
        {
            T item = m_List[index];
            m_Dictionary.Remove(item);
            if (index == m_List.Count - 1)
                m_List.RemoveAt(index);
            else
            {
                int replaceItemIndex = m_List.Count - 1;
                T replaceItem = m_List[replaceItemIndex];
                m_List[index] = replaceItem;
                m_Dictionary[replaceItem] = index;
                m_List.RemoveAt(replaceItemIndex);
            }
        }
```

如果不是最后一个，把最后一个item移动到被移除的位置。这里需要注意，如果你的IndexedSet是有序的，在移除对象的之后，就有可能要重新排序了。


Sort：



```
//Sorts the internal list, this makes the exposed index accessor sorted as well.
        //But note that any insertion or deletion, can unorder the collection again.
        public void Sort(Comparison<T> sortLayoutFunction)
        {
            //There might be better ways to sort and keep the dictionary index up to date.
            m_List.Sort(sortLayoutFunction);
            //Rebuild the dictionary index.
            for (int i = 0; i < m_List.Count; ++i)
            {
                T item = m_List[i];
                m_Dictionary[item] = i;
            }
        }
```

对m_List排序之后，m_Dictionary里保存的下标也会修改。





ObjectPool是一个对象池，用于回收被释放的对象以达到节省内存的作用。


方法都很简单：



```
internal class ObjectPool<T> where T : new()
    {
        private readonly Stack<T> m_Stack = new Stack<T>();
        private readonly UnityAction<T> m_ActionOnGet;
        private readonly UnityAction<T> m_ActionOnRelease;

        public int countAll { get; private set; }
        public int countActive { get { return countAll - countInactive; } }
        public int countInactive { get { return m_Stack.Count; } }

        public ObjectPool(UnityAction<T> actionOnGet, UnityAction<T> actionOnRelease)
        {
            m_ActionOnGet = actionOnGet;
            m_ActionOnRelease = actionOnRelease;
        }

        public T Get()
        {
            T element;
            if (m_Stack.Count == 0)
            {
                element = new T();
                countAll++;
            }
            else
            {
                element = m_Stack.Pop();
            }
            if (m_ActionOnGet != null)
                m_ActionOnGet(element);
            return element;
        }

        public void Release(T element)
        {
            if (m_Stack.Count > 0 && ReferenceEquals(m_Stack.Peek(), element))
                Debug.LogError("Internal error. Trying to destroy object that is already released to pool.");
            if (m_ActionOnRelease != null)
                m_ActionOnRelease(element);
            m_Stack.Push(element);
        }
    }
```



Get时，stack里面没有就创建，有就pop一个出来作为返回值。Release时，把对象放入stack。




ListPool是ObjectPool的一个特殊应用，在UGUI里经常被用到，用于回收被释放的List。

实现：



```
internal static class ListPool<T>
    {
        // Object pool to avoid allocations.
        private static readonly ObjectPool<List<T>> s_ListPool = new ObjectPool<List<T>>(null, l => l.Clear());

        public static List<T> Get()
        {
            return s_ListPool.Get();
        }

        public static void Release(List<T> toRelease)
        {
            s_ListPool.Release(toRelease);
        }
    }
```
因为是一个静态泛型类，所以对于每个类型只有一个s_ListPool。



s_ListPool是一个类型为List<T>的ObjectPool，并添加了Release时的lamda回调，即清空List。




CoroutineTween是一个命名空间（namespace UnityEngine.UI.CoroutineTween），其中有ITweenValue接口，继承自ITweenValue的ColorTween和FloatTween两个结构，以及以ITweenValue为泛型的TweenRunner，这个泪在Graphic（[UGUI内核大探究（七）Graphic](http://blog.csdn.net/ecidevilin/article/details/52548747)）和Dropdown（[UGUI内核大探究（十三）Dropdown](http://blog.csdn.net/ecidevilin/article/details/52570871)）里被用到，分别用于实现颜色和透明度渐变效果。



```
public void StartTween(T info)
        {
            if (m_CoroutineContainer == null)
            {
                Debug.LogWarning("Coroutine container not configured... did you forget to call Init?");
                return;
            }

            if (m_Tween != null)
            {
                m_CoroutineContainer.StopCoroutine(m_Tween);
                m_Tween = null;
            }

            if (!m_CoroutineContainer.gameObject.activeInHierarchy)
            {
                info.TweenValue(1.0f);
                return;
            }

            m_Tween = Start(info);
            m_CoroutineContainer.StartCoroutine(m_Tween);
        }
```

使用m_CoroutineContainer（Init时传入的组件）的协程执行Start方法。




```
private static IEnumerator Start(T tweenInfo)
        {
            if (!tweenInfo.ValidTarget())
                yield break;

            var elapsedTime = 0.0f;
            while (elapsedTime < tweenInfo.duration)
            {
                elapsedTime += tweenInfo.ignoreTimeScale ? Time.unscaledDeltaTime : Time.deltaTime;
                var percentage = Mathf.Clamp01(elapsedTime / tweenInfo.duration);
                tweenInfo.TweenValue(percentage);
                yield return null;
            }
            tweenInfo.TweenValue(1.0f);
        }
```


根据经过时间（elapsedTime）除以tweenInfo的持续时间（duration）得到百分比，调用TweenValue方法。


ColorTween的TweenValue方法：



```
public void TweenValue(float floatPercentage)
        {
            if (!ValidTarget())
                return;

            var newColor = Color.Lerp(m_StartColor, m_TargetColor, floatPercentage);

            if (m_TweenMode == ColorTweenMode.Alpha)
            {
                newColor.r = m_StartColor.r;
                newColor.g = m_StartColor.g;
                newColor.b = m_StartColor.b;
            }
            else if (m_TweenMode == ColorTweenMode.RGB)
            {
                newColor.a = m_StartColor.a;
            }
            m_Target.Invoke(newColor);
        }
```

根据输入的百分比就算出色彩值或Alpha，并发送事件将新的色彩值传给监听方法。



FloatTween的TweenValue方法：




```
public void TweenValue(float floatPercentage)
        {
            if (!ValidTarget())
                return;

            var newValue = Mathf.Lerp(m_StartValue, m_TargetValue, floatPercentage);
            m_Target.Invoke(newValue);
        }
```
似乎不用再多讲了。





以上都是很简单的类，可能在实际开发中不一定会用到它们（例如DOTween就是相对于CoroutineTween功能更完善的实现），但是作为参考也是值得学习一下的。



