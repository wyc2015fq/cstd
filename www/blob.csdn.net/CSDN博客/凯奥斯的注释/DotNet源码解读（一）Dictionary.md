# DotNet源码解读（一）Dictionary - 凯奥斯的注释 - CSDN博客





2017年05月11日 08:53:15[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：412标签：[C#																[dictionary																[源码																[.Net																[hash](https://so.csdn.net/so/search/s.do?q=hash&t=blog)
个人分类：[C#](https://blog.csdn.net/ecidevilin/article/category/6399640)





先放源码，解读稍后补上

[CLR源码地址](https://github.com/dotnet/coreclr/tree/master/src/mscorlib)

[mono源码地址](https://github.com/mono/mono)




核心数据结构



```
private struct Entry
        {
            public int hashCode;    // Lower 31 bits of hash code, -1 if unused
            public int next;        // Index of next entry, -1 if last
            public TKey key;           // Key of entry
            public TValue value;         // Value of entry
        }
```


核心源码（Add）





```
private bool TryInsert(TKey key, TValue value, InsertionBehavior behavior)
        {
            if (key == null)
            {
                ThrowHelper.ThrowArgumentNullException(ExceptionArgument.key);
            }

            if (buckets == null) Initialize(0);
            int hashCode = comparer.GetHashCode(key) & 0x7FFFFFFF;
            int targetBucket = hashCode % buckets.Length;            
#if FEATURE_RANDOMIZED_STRING_HASHING
            int collisionCount = 0;
#endif

            for (int i = buckets[targetBucket]; i >= 0; i = entries[i].next)
            {
                if (entries[i].hashCode == hashCode && comparer.Equals(entries[i].key, key))
                {
                    if (behavior == InsertionBehavior.OverwriteExisting)
                    {
                        entries[i].value = value;
                        version++;
                        return true;
                    }

                    if (behavior == InsertionBehavior.ThrowOnExisting)
                    {
                        ThrowHelper.ThrowAddingDuplicateWithKeyArgumentException(key);
                    }

                    return false;
                }
#if FEATURE_RANDOMIZED_STRING_HASHING
                collisionCount++;
#endif
            }
            int index;
            if (freeCount > 0)
            {
                index = freeList;
                freeList = entries[index].next;
                freeCount--;
            }
            else
            {
                if (count == entries.Length)
                {
                    Resize();
                    targetBucket = hashCode % buckets.Length;
                }
                index = count;
                count++;
            }

            entries[index].hashCode = hashCode;
            entries[index].next = buckets[targetBucket];
            entries[index].key = key;
            entries[index].value = value;
            buckets[targetBucket] = index;
            version++;

#if FEATURE_RANDOMIZED_STRING_HASHING
            // In case we hit the collision threshold we'll need to switch to the comparer which is using randomized string hashing
            // in this case will be EqualityComparer<string>.Default.
            // Note, randomized string hashing is turned on by default on coreclr so EqualityComparer<string>.Default will 
            // be using randomized string hashing

            if (collisionCount > HashHelpers.HashCollisionThreshold && comparer == NonRandomizedStringEqualityComparer.Default)
            {
                comparer = (IEqualityComparer<TKey>)EqualityComparer<string>.Default;
                Resize(entries.Length, true);
            }
#endif

            return true;
        }
```


核心源码（Get）





```
private int FindEntry(TKey key)
        {
            if (key == null)
            {
                ThrowHelper.ThrowArgumentNullException(ExceptionArgument.key);
            }

            if (buckets != null)
            {
                int hashCode = comparer.GetHashCode(key) & 0x7FFFFFFF;
                for (int i = buckets[hashCode % buckets.Length]; i >= 0; i = entries[i].next)
                {
                    if (entries[i].hashCode == hashCode && comparer.Equals(entries[i].key, key)) return i;
                }
            }
            return -1;
        }
```](https://so.csdn.net/so/search/s.do?q=.Net&t=blog)](https://so.csdn.net/so/search/s.do?q=源码&t=blog)](https://so.csdn.net/so/search/s.do?q=dictionary&t=blog)](https://so.csdn.net/so/search/s.do?q=C#&t=blog)




