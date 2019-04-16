# 转载和积累系列 - js实现 hashMap - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年09月27日 14:42:49[initphp](https://me.csdn.net/initphp)阅读数：1131








```java
var hash_map = function () {

	var hash_table_length = 1024 * 1024; //2的幂次方
	var hash_table = new Array(hash_table_length); //hashTable表
	var total_size = 0; //总长度

	/*	
	 * 新增hashmap值
	 * 参数：
	 * key  : key值
	 * value: 原始Value值
	 */
	this.put = function (key, value) {
		if (key != null) {
			var hash = hashCode(key); //进过hashCode，将key转化成整型
			var index = indexFor(hash, hash_table.length);
			//从冲突链表中查询KEY键是否存在，存在的话覆盖新值
			for (var obj = hash_table[index]; obj != null; obj = obj.next) {
				if (obj.hash == hash && obj.key == key) {
					obj.value = value;
					return obj.value;
				}
			}
			addEntry(hash, key, value, index);
		}
		return false;
	}

	/*	
	 * 获取hashmap对应值
	 * 参数：
	 * key  : key值
	 */
	this.get = function (key) {
		if (key != null) {
			var hash = hashCode(key); //进过hashCode，将key转化成整型
			var index = indexFor(hash, hash_table.length);
			for (var obj = hash_table[index]; obj != null; obj = obj.next) {
				if (obj.hash == hash && obj.key == key) {
					return obj.value;
				}
			}
		}
		return false;
	}

	/*	
	 * 删除一个hash值
	 * 参数：
	 * key  : key值
	 */
	this.remove = function (key) {
		if (key != null) {
			var hash  = hashCode(key); //进过hashCode，将key转化成整型
			var index = indexFor(hash, hash_table.length);
			var entry = hash_table[index];
			var e = entry;
			while (e != null) { //循环跑链表,将需要删除值的next对象放到前一个对象的next中
				var next = e.next;
				if (e.hash == hash && e.key == key) {
					if (entry == e) {
						hash_table[index] = next;
					} else {
						entry.next = next;
					}
					total_size--;
					return true;
				}
				entry = e;
				e = next;
			}
		} 
		return false;
	}

	/*	
	 * 清空hashtable操作
	 * 参数：
	 * key  : key值
	 */
	this.clear = function () {
		hash_table = null;
		hash_table = new Array(hash_table_length);
		total_size = 0;
		return hash_table;
	}

	/*	
	 * 判断KEY值是否存在
	 * 参数：
	 * key  : key值
	 */
	this.isSet = function (key) {
		if (key != null) {
			var hash = hashCode(key); //进过hashCode，将key转化成整型
			var index = indexFor(hash, hash_table.length);
			for (var obj = hash_table[index]; obj != null; obj = obj.next) {
				if (obj.hash == hash && obj.key == key) {
					return true;
				}
			}
		}
		return false;
	}

	/*	
	 * 返回hashMap长度
	 */
	this.size = function () {
		return total_size;
	}

	/*	
	 * 解决hash冲突的链表结构
	 * 参数：
	 * hash : hash值，key经过hashCode的值
	 * key  : key值
	 * value: 原始Value值
	 * index: hashTable 索引值
	 */
	var addEntry = function (hash, key, value, index) {
		 var entry = hash_table[index];
		 var item  = {
		 	"hash"	: hash,
		 	"key"  	: key,
		 	"value"	: value,
		 	"next"	: entry
		 };
		 hash_table[index] = item;
		 total_size++; //统计数据表总长度
	}

	/*	
	 *	经过该函数得到 哈希表 哈希地址
	 */
	var indexFor = function (hash, length) {
		return hash & (length - 1);
	}

	/*	
	 *	通过hashCode函数，将key转化成整型
	 */
	var hashCode = function (key) {
		var h = 0, off = 0;
		var length = key.length;
		for (var i = 0; i < length; i++) {
			var temp = key.charCodeAt(off++);
			h = 31 * h + temp;
			if (h > 0x7fffffff || h < 0x80000000) {
			    h = h & 0xffffffff;
			}
		}
		h ^= (h >>> 20) ^ (h >>> 12);
		return h ^ (h >>> 7) ^ (h >>> 4);
	};
}
```
- 未实现自动扩容，初始化的时候，hashTable设置大点就行了，自动扩容可能会有很大效率问题
- 通过对key值进行hash成一个整型，并且作&与操作，将key值hash到hashTable数组中的一个值中
- 对于hash出来的值冲突的情况，实现了链表结构
- 主要参照java hashMap实现
- 思考：如果更复杂的hashMap，可以实现hashTable表多维数组的hash
- hashTable的length越长，hash冲突越少




