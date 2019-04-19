# C#脚本实践(四): 反射与序列化 - 逍遥剑客 - CSDN博客
2011年11月26日 17:18:37[xoyojank](https://me.csdn.net/xoyojank)阅读数：4880
- 序列化- 这个比较简单, .net直接就支持, 以XML序列化为例:
```
public static string SerializeToString (object obj)
	{
		XmlSerializer serializer = new XmlSerializer (obj.GetType ());
 
		using (StringWriter writer = new StringWriter()) {
			serializer.Serialize (writer, obj);
 
			return writer.ToString ();
		}
	}
	public static T SerializeFromString<T> (string xml)
	{
		XmlSerializer serializer = new XmlSerializer (typeof(T));
  
		using (StringReader reader = new StringReader(xml)) {
			return (T)serializer.Deserialize (reader);
		}
	}
```
```
public struct Vector3
	{
		public float x;
		public float y;
		public float z;
	}
	public class TestClass
	{
		public int testInt = 1234;
		public float testFloat = 0.2341f;
		public bool[] boolArray = {true, false, false, true};
		public Vector3 position;
		public TestClass ()
		{
			this.position.x = 100;
			this.position.y = 100;
			this.position.z = 0;
		}
	}
```
结果如下:
```
<?xml version="1.0" encoding="utf-8"?>
<TestClass xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema">
  <testInt>1234</testInt>
  <testFloat>0.2341</testFloat>
  <boolArray>
    <boolean>true</boolean>
    <boolean>false</boolean>
    <boolean>false</boolean>
    <boolean>true</boolean>
  </boolArray>
  <position>
    <x>100</x>
    <y>100</y>
    <z>0</z>
  </position>
</TestClass>
```
到时候把string用mono api传到C++里, 爱怎么存怎么存. 二进制格式同理
- 反射- .net本身也支持反射, 问题是如何反射到C++里
- 用mono api取得类的meta信息, 然后反应到编辑器上去编辑
- 如果想偷懒的话, 可以直接用mono创建个窗口, 放个propertygrid控件, 再这个窗口嵌入到主编辑器的界面
```cpp
int depth = 0;
void ListFields(MonoObject* object)
{
	assert(NULL != object);
	++depth;
	MonoClass* objectClass = mono_object_get_class(object);
	const char* className = mono_class_get_name(objectClass);
	printf("[%s]\n", className);
	void* iter = NULL;
	MonoClassField* field = NULL;
	while (field = mono_class_get_fields(objectClass, &iter))
	{
		for (int i = 0; i < depth; ++i)
		{
			printf("\t");
		}
		const char* fieldName = mono_field_get_name(field);
		printf("%s : ", fieldName);
		MonoType* fieldType = mono_field_get_type(field);
		int type = mono_type_get_type(fieldType);
		switch (type)
		{
		case MONO_TYPE_BOOLEAN:
			{
				bool boolValue = false;
				mono_field_get_value(object, field, &boolValue);
				printf("%s\n", boolValue ? "true" : "false");
			}
			break;
		case MONO_TYPE_I4:
			{
				int intValue = 0;
				mono_field_get_value(object, field, &intValue);
				printf("%d\n", intValue);
			}
			break;
		case MONO_TYPE_R4:
			{
				float floatValue = 0;
				mono_field_get_value(object, field, &floatValue);
				printf("%f\n", floatValue);
			}
			break;
		case MONO_TYPE_CHAR:
		case MONO_TYPE_I1:
		case MONO_TYPE_U1:
		case MONO_TYPE_I2:
		case MONO_TYPE_U2:
		case MONO_TYPE_U4:
		case MONO_TYPE_I:
		case MONO_TYPE_U:
		case MONO_TYPE_I8:
		case MONO_TYPE_U8:
		case MONO_TYPE_R8:
			{
				//@todo
			}
			break;
		case MONO_TYPE_SZARRAY:
			{
				MonoObject* value = mono_field_get_value_object(mono_domain_get(), field, object);
				MonoArray* array = (MonoArray*)value;
				uintptr_t size = mono_array_length(array);
				MonoClass* elementClass = mono_class_get_element_class(mono_object_get_class(value));
				MonoType* elementType = mono_class_get_type(elementClass);
				switch (mono_type_get_type(elementType))
				{
				case MONO_TYPE_BOOLEAN:
					{
						bool* data = mono_array_addr(array, bool, 0);
						for (int i = 0; i < size; ++i)
						{
							printf("%s ", data[i] ? "true" : "false");
						}
						printf("\n");
					}
					break;
				default: //@todo
					break;
				}
				className = mono_class_get_name(elementClass);
			}
			break;
		case MONO_TYPE_STRING:
			{
				MonoObject* value = mono_field_get_value_object(mono_domain_get(), field, object);
				MonoString* valueString = (MonoString*)value;
				const char* valueCStr = mono_string_to_utf8(valueString);
				printf("%s\n", valueCStr);
			}
			break;
		default:
			{
				MonoObject* value = mono_field_get_value_object(mono_domain_get(), field, object);
				ListFields(value);
			}
			break;
		}
	}
	--depth;
}
```
输出:
```
[TestClass]
        testBool : false
        testInt : 1234
        testFloat : 0.234100
        testString : hello
        boolArray : true false false true
        position : [Vector3]
                x : 100.000000
                y : 100.000000
                z : 0.000000
```
[C#脚本实践(三): 集成到游戏](http://blog.csdn.net/xoyojank/article/details/6979135)
