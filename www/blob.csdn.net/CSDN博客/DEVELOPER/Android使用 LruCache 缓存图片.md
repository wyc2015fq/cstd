# Android使用 LruCache 缓存图片 - DEVELOPER - CSDN博客





2014年07月21日 11:58:12[学术袁](https://me.csdn.net/u012827205)阅读数：762标签：[android																[LruCache](https://so.csdn.net/so/search/s.do?q=LruCache&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android使用 LruCache 缓存图片](https://blog.csdn.net/u012827205/article/category/2411157)








**使用图片缓存技术**

在你应用程序的UI界面加载一张图片是一件很简单的事情，但是当你需要在界面上加载一大堆图片的时候，情况就变得复杂起来。在很多情况下，（比如使用ListView, GridView 或者 ViewPager 这样的组件），屏幕上显示的图片可以通过滑动屏幕等事件不断地增加，最终导致OOM。


为了保证内存的使用始终维持在一个合理的范围，通常会把被移除屏幕的图片进行回收处理。此时垃圾回收器也会认为你不再持有这些图片的引用，从而对这些图片进行GC操作。用这种思路来解决问题是非常好的，可是为了能让程序快速运行，在界面上迅速地加载图片，你又必须要考虑到某些图片被回收之后，用户又将它重新滑入屏幕这种情况。这时重新去加载一遍刚刚加载过的图片无疑是性能的瓶颈，你需要想办法去避免这个情况的发生。

这个时候，使用内存缓存技术可以很好的解决这个问题，它可以让组件快速地重新加载和处理图片。下面我们就来看一看如何使用内存缓存技术来对图片进行缓存，从而让你的应用程序在加载很多图片的时候可以提高响应速度和流畅性。

内存缓存技术对那些大量占用应用程序宝贵内存的图片提供了快速访问的方法。其中最核心的类是LruCache (此类在android-support-v4的包中提供) 。这个类非常适合用来缓存图片，它的主要算法原理是把最近使用的对象用强引用存储在 LinkedHashMap 中，并且把最近最少使用的对象在缓存值达到预设定值之前从内存中移除。

在过去，我们经常会使用一种非常流行的内存缓存技术的实现，即软引用或弱引用 (SoftReference or WeakReference)。但是现在已经不再推荐使用这种方式了，因为从 Android 2.3 (API Level 9)开始，垃圾回收器会更倾向于回收持有软引用或弱引用的对象，这让软引用和弱引用变得不再可靠。另外，Android 3.0 (API Level
 11)中，图片的数据会存储在本地的内存当中，因而无法用一种可预见的方式将其释放，这就有潜在的风险造成应用程序的内存溢出并崩溃。

为了能够选择一个合适的缓存大小给LruCache, 有以下多个因素应该放入考虑范围内，例如：

你的设备可以为每个应用程序分配多大的内存？

设备屏幕上一次最多能显示多少张图片？有多少图片需要进行预加载，因为有可能很快也会显示在屏幕上？

你的设备的屏幕大小和分辨率分别是多少？一个超高分辨率的设备（例如 Galaxy Nexus) 比起一个较低分辨率的设备（例如 Nexus S），在持有相同数量图片的时候，需要更大的缓存空间。

图片的尺寸和大小，还有每张图片会占据多少内存空间。

图片被访问的频率有多高？会不会有一些图片的访问频率比其它图片要高？如果有的话，你也许应该让一些图片常驻在内存当中，或者使用多个LruCache 对象来区分不同组的图片。

你能维持好数量和质量之间的平衡吗？有些时候，存储多个低像素的图片，而在后台去开线程加载高像素的图片会更加的有效。

并没有一个指定的缓存大小可以满足所有的应用程序，这是由你决定的。你应该去分析程序内存的使用情况，然后制定出一个合适的解决方案。一个太小的缓存空间，有可能造成图片频繁地被释放和重新加载，这并没有好处。而一个太大的缓存空间，则有可能还是会引起 java.lang.OutOfMemory 的异常。

下面是一个使用 LruCache 来缓存图片的例子：




```java
private LruCache<String, Bitmap> mMemoryCache;  
@Override
protected void onCreate(Bundle savedInstanceState) { 
    // 获取到可用内存的最大值，使用内存超出这个值会引起OutOfMemory异常。 
    // LruCache通过构造函数传入缓存值，以KB为单位。 
    int maxMemory = (int) (Runtime.getRuntime().maxMemory() / 1024); 
    // 使用最大可用内存值的1/8作为缓存的大小。 
    int cacheSize = maxMemory / 8; 
    mMemoryCache = new LruCache<String, Bitmap>(cacheSize) { 
        @Override
        protected int sizeOf(String key, Bitmap bitmap) { 
            // 重写此方法来衡量每张图片的大小，默认返回图片数量。 
            return bitmap.getByteCount() / 1024; 
        } 
    }; 
} 
    
public void addBitmapToMemoryCache(String key, Bitmap bitmap) { 
    if (getBitmapFromMemCache(key) == null) { 
        mMemoryCache.put(key, bitmap); 
    } 
} 
    
public Bitmap getBitmapFromMemCache(String key) { 
    return mMemoryCache.get(key); 
}
```


在这个例子当中，使用了系统分配给应用程序的八分之一内存来作为缓存大小。在中高配置的手机当中，这大概会有4兆(32/8)的缓存空间。一个全屏幕的 GridView 使用4张 800x480分辨率的图片来填充，则大概会占用1.5兆的空间(800*480*4)。因此，这个缓存大小可以存储2.5页的图片。

当向 ImageView 中加载一张图片时,首先会在 LruCache 的缓存中进行检查。如果找到了相应的键值，则会立刻更新ImageView ，否则开启一个后台线程来加载这张图片。

```java
public void loadBitmap(int resId, ImageView imageView) { 
    final String imageKey = String.valueOf(resId); 
    final Bitmap bitmap = getBitmapFromMemCache(imageKey); 
    if (bitmap != null) { 
        imageView.setImageBitmap(bitmap); 
    } else { 
        imageView.setImageResource(R.drawable.image_placeholder); 
        BitmapWorkerTask task = new BitmapWorkerTask(imageView); 
        task.execute(resId); 
    } 
}
```

BitmapWorkerTask 还要把新加载的图片的键值对放到缓存中


```java
class BitmapWorkerTask extends AsyncTask<Integer, Void, Bitmap> { 
    // 在后台加载图片。 
    @Override
    protected Bitmap doInBackground(Integer... params) { 
        final Bitmap bitmap = decodeSampledBitmapFromResource( 
                getResources(), params[0], 100, 100); 
        addBitmapToMemoryCache(String.valueOf(params[0]), bitmap); 
        return bitmap; 
    } 
}
```


下面是LruCache 的代码实现作为简介：



```java
package android.util;  
  
import java.util.LinkedHashMap;  
import java.util.Map;  
  
/** 
 * A cache that holds strong references to a limited number of values. Each time 
 * a value is accessed, it is moved to the head of a queue. When a value is 
 * added to a full cache, the value at the end of that queue is evicted and may 
 * become eligible for garbage collection. 
 * Cache保存一个强引用来限制内容数量，每当Item被访问的时候，此Item就会移动到队列的头部。
 * 当cache已满的时候加入新的item时，在队列尾部的item会被回收。
 * <p>If your cached values hold resources that need to be explicitly released, 
 * override {@link #entryRemoved}. 
 * 如果你cache的某个值需要明确释放，重写entryRemoved()
 * <p>If a cache miss should be computed on demand for the corresponding keys, 
 * override {@link #create}. This simplifies the calling code, allowing it to 
 * assume a value will always be returned, even when there's a cache miss. 
 * 如果key相对应的item丢掉啦，重写create().这简化了调用代码，即使丢失了也总会返回。
 * <p>By default, the cache size is measured in the number of entries. Override 
 * {@link #sizeOf} to size the cache in different units. For example, this cache 
 * is limited to 4MiB of bitmaps: 默认cache大小是测量的item的数量，重写sizeof计算不同item的
 *  大小。
 * <pre>   {@code 
 *   int cacheSize = 4 * 1024 * 1024; // 4MiB 
 *   LruCache<String, Bitmap> bitmapCache = new LruCache<String, Bitmap>(cacheSize) { 
 *       protected int sizeOf(String key, Bitmap value) { 
 *           return value.getByteCount(); 
 *       } 
 *   }}</pre> 
 * 
 * <p>This class is thread-safe. Perform multiple cache operations atomically by 
 * synchronizing on the cache: <pre>   {@code 
 *   synchronized (cache) { 
 *     if (cache.get(key) == null) { 
 *         cache.put(key, value); 
 *     } 
 *   }}</pre> 
 * 
 * <p>This class does not allow null to be used as a key or value. A return 
 * value of null from {@link #get}, {@link #put} or {@link #remove} is 
 * unambiguous: the key was not in the cache.
 * 不允许key或者value为null
 *  当get（），put（），remove（）返回值为null时，key相应的项不在cache中
 */  
public class LruCache<K, V> {  
    private final LinkedHashMap<K, V> map;  
  
    /** Size of this cache in units. Not necessarily the number of elements. */  
    private int size; //已经存储的大小
    private int maxSize; //规定的最大存储空间
  
    private int putCount;  //put的次数
    private int createCount;  //create的次数
    private int evictionCount;  //回收的次数
    private int hitCount;  //命中的次数
    private int missCount;  //丢失的次数
  
    /** 
     * @param maxSize for caches that do not override {@link #sizeOf}, this is 
     *     the maximum number of entries in the cache. For all other caches, 
     *     this is the maximum sum of the sizes of the entries in this cache. 
     */  
    public LruCache(int maxSize) {  
        if (maxSize <= 0) {  
            throw new IllegalArgumentException("maxSize <= 0");  
        }  
        this.maxSize = maxSize;  
        this.map = new LinkedHashMap<K, V>(0, 0.75f, true);  
    }  
  
    /** 
     * Returns the value for {@code key} if it exists in the cache or can be 
     * created by {@code #create}. If a value was returned, it is moved to the 
     * head of the queue. This returns null if a value is not cached and cannot 
     * be created. 通过key返回相应的item，或者创建返回相应的item。相应的item会移动到队列的头部，
     * 如果item的value没有被cache或者不能被创建，则返回null。
     */  
    public final V get(K key) {  
        if (key == null) {  
            throw new NullPointerException("key == null");  
        }  
  
        V mapValue;  
        synchronized (this) {  
            mapValue = map.get(key);  
            if (mapValue != null) {  
                hitCount++;  //命中
                return mapValue;  
            }  
            missCount++;  //丢失
        }  
  
        /* 
         * Attempt to create a value. This may take a long time, and the map 
         * may be different when create() returns. If a conflicting value was 
         * added to the map while create() was working, we leave that value in 
         * the map and release the created value. 
         * 如果丢失了就试图创建一个item
         */  
  
        V createdValue = create(key);  
        if (createdValue == null) {  
            return null;  
        }  
  
        synchronized (this) {  
            createCount++;//创建++  
            mapValue = map.put(key, createdValue);  
  
            if (mapValue != null) {  
                // There was a conflict so undo that last put  
                //如果前面存在oldValue，那么撤销put（） 
                map.put(key, mapValue);  
            } else {  
                size += safeSizeOf(key, createdValue);  
            }  
        }  
  
        if (mapValue != null) {  
            entryRemoved(false, key, createdValue, mapValue);  
            return mapValue;  
        } else {  
            trimToSize(maxSize);  
            return createdValue;  
        }  
    }  
  
    /** 
     * Caches {@code value} for {@code key}. The value is moved to the head of 
     * the queue. 
     * 
     * @return the previous value mapped by {@code key}. 
     */  
    public final V put(K key, V value) {  
        if (key == null || value == null) {  
            throw new NullPointerException("key == null || value == null");  
        }  
  
        V previous;  
        synchronized (this) {  
            putCount++;  
            size += safeSizeOf(key, value);  
            previous = map.put(key, value);  
            if (previous != null) {  //返回的先前的value值
                size -= safeSizeOf(key, previous);  
            }  
        }  
  
        if (previous != null) {  
            entryRemoved(false, key, previous, value);  
        }  
  
        trimToSize(maxSize);  
        return previous;  
    }  
  
    /** 
     * @param maxSize the maximum size of the cache before returning. May be -1 
     *     to evict even 0-sized elements. 
     *  清空cache空间
     */  
    private void trimToSize(int maxSize) {  
        while (true) {  
            K key;  
            V value;  
            synchronized (this) {  
                if (size < 0 || (map.isEmpty() && size != 0)) {  
                    throw new IllegalStateException(getClass().getName()  
                            + ".sizeOf() is reporting inconsistent results!");  
                }  
  
                if (size <= maxSize) {  
                    break;  
                }  
  
                Map.Entry<K, V> toEvict = map.eldest();  
                if (toEvict == null) {  
                    break;  
                }  
  
                key = toEvict.getKey();  
                value = toEvict.getValue();  
                map.remove(key);  
                size -= safeSizeOf(key, value);  
                evictionCount++;  
            }  
  
            entryRemoved(true, key, value, null);  
        }  
    }  
  
    /** 
     * Removes the entry for {@code key} if it exists. 
     * 删除key相应的cache项，返回相应的value
     * @return the previous value mapped by {@code key}. 
     */  
    public final V remove(K key) {  
        if (key == null) {  
            throw new NullPointerException("key == null");  
        }  
  
        V previous;  
        synchronized (this) {  
            previous = map.remove(key);  
            if (previous != null) {  
                size -= safeSizeOf(key, previous);  
            }  
        }  
  
        if (previous != null) {  
            entryRemoved(false, key, previous, null);  
        }  
  
        return previous;  
    }  
  
    /** 
     * Called for entries that have been evicted or removed. This method is 
     * invoked when a value is evicted to make space, removed by a call to 
     * {@link #remove}, or replaced by a call to {@link #put}. The default 
     * implementation does nothing. 
     * 当item被回收或者删掉时调用。改方法当value被回收释放存储空间时被remove调用，
     * 或者替换item值时put调用，默认实现什么都没做。
     * <p>The method is called without synchronization: other threads may 
     * access the cache while this method is executing. 
     * 
     * @param evicted true if the entry is being removed to make space, false 
     *     if the removal was caused by a {@link #put} or {@link #remove}. 
     * true---为释放空间被删除；false---put或remove导致
     * @param newValue the new value for {@code key}, if it exists. If non-null, 
     *     this removal was caused by a {@link #put}. Otherwise it was caused by 
     *     an eviction or a {@link #remove}. 
     */  
    protected void entryRemoved(boolean evicted, K key, V oldValue, V newValue) {}  
  
    /** 
     * Called after a cache miss to compute a value for the corresponding key. 
     * Returns the computed value or null if no value can be computed. The 
     * default implementation returns null. 
     * 当某Item丢失时会调用到，返回计算的相应的value或者null
     * <p>The method is called without synchronization: other threads may 
     * access the cache while this method is executing. 
     * 
     * <p>If a value for {@code key} exists in the cache when this method 
     * returns, the created value will be released with {@link #entryRemoved} 
     * and discarded. This can occur when multiple threads request the same key 
     * at the same time (causing multiple values to be created), or when one 
     * thread calls {@link #put} while another is creating a value for the same 
     * key. 
     */  
    protected V create(K key) {  
        return null;  
    }  
  
    private int safeSizeOf(K key, V value) {  
        int result = sizeOf(key, value);  
        if (result < 0) {  
            throw new IllegalStateException("Negative size: " + key + "=" + value);  
        }  
        return result;  
    }  
  
    /** 
     * Returns the size of the entry for {@code key} and {@code value} in 
     * user-defined units.  The default implementation returns 1 so that size 
     * is the number of entries and max size is the maximum number of entries. 
     * 返回用户定义的item的大小，默认返回1代表item的数量，最大size就是最大item值
     * <p>An entry's size must not change while it is in the cache. 
     */  
    protected int sizeOf(K key, V value) {  
        return 1;  
    }  
  
    /** 
     * Clear the cache, calling {@link #entryRemoved} on each removed entry. 
     * 清空cacke
     */  
    public final void evictAll() {  
        trimToSize(-1); // -1 will evict 0-sized elements  
    }  
  
    /** 
     * For caches that do not override {@link #sizeOf}, this returns the number 
     * of entries in the cache. For all other caches, this returns the sum of 
     * the sizes of the entries in this cache. 
     */  
    public synchronized final int size() {  
        return size;  
    }  
  
    /** 
     * For caches that do not override {@link #sizeOf}, this returns the maximum 
     * number of entries in the cache. For all other caches, this returns the 
     * maximum sum of the sizes of the entries in this cache. 
     */  
    public synchronized final int maxSize() {  
        return maxSize;  
    }  
  
    /** 
     * Returns the number of times {@link #get} returned a value that was 
     * already present in the cache. 
     */  
    public synchronized final int hitCount() {  
        return hitCount;  
    }  
  
    /** 
     * Returns the number of times {@link #get} returned null or required a new 
     * value to be created. 
     */  
    public synchronized final int missCount() {  
        return missCount;  
    }  
  
    /** 
     * Returns the number of times {@link #create(Object)} returned a value. 
     */  
    public synchronized final int createCount() {  
        return createCount;  
    }  
  
    /** 
     * Returns the number of times {@link #put} was called. 
     */  
    public synchronized final int putCount() {  
        return putCount;  
    }  
  
    /** 
     * Returns the number of values that have been evicted. 
     * 返回被回收的数量
     */  
    public synchronized final int evictionCount() {  
        return evictionCount;  
    }  
  
    /** 
     * Returns a copy of the current contents of the cache, ordered from least 
     * recently accessed to most recently accessed. 返回当前cache的副本，从最近最少访问到最多访问
     */  
    public synchronized final Map<K, V> snapshot() {  
        return new LinkedHashMap<K, V>(map);  
    }  
  
    @Override public synchronized final String toString() {  
        int accesses = hitCount + missCount;  
        int hitPercent = accesses != 0 ? (100 * hitCount / accesses) : 0;  
        return String.format("LruCache[maxSize=%d,hits=%d,misses=%d,hitRate=%d%%]",  
                maxSize, hitCount, missCount, hitPercent);  
    }  
}
```






