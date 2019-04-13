
# Node.js中JavaScript与C++的数据类型转换（node_0.12, node_4.3） - cocoonyang的专栏 - CSDN博客


2015年04月07日 00:11:58[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：1228



### args  -> int
node_0.12

void foo(const FunctionCallbackInfo<value>& args) 
{
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	if( args[0]->IsUndefined())
	{
		std::cerr << "In foo(): parameters undefined." << std::endl;
		return;
	}
	v8::Handle<v8::value> theArg = args[0];

	if (theArg->IsInt32()) 
	{
		int value = theArg->Int32Value();
	}
}node_4.3

```python
void MyClass::foo(const FunctionCallbackInfo<value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	MyClass* obj = ObjectWrap::Unwrap<MyClass>(args.Holder());
	// checking arguments number
	if (args.Length() < 3)
	{
		isolate->ThrowException( 
                    v8::Exception::TypeError( 
                        String::NewFromUtf8(isolate, "Wrong arguments number")));
		return;
	}
	// checking arguments
	int value = args[0]->IsUndefined() ? 0 : args[0]->Int32Value();
        // ... 
}
```


### 返回 int变量

node_0.12

void foo(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

	//...
	args.GetReturnValue().Set(v8::Int32::New(isolate, 1));
	return;
}
### args  -> double
node_0.12

void foo(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	if( args[0]->IsUndefined())
	{
		std::cerr << "In foo(): parameters undefined." << std::endl;
		return;
	}
	v8::Handle<v8::Value> theArg = args[0];

	if (theArg->IsNumber()) 
	{
		double value = theArg->NumberValue();
	}
}

node_4.3

```python
void MyClass::foo(const FunctionCallbackInfo<value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	MyClass* obj = ObjectWrap::Unwrap<MyClass>(args.Holder());
	// checking arguments number
	if (args.Length() < 3)
	{
		isolate->ThrowException( 
                    v8::Exception::TypeError( 
                        String::NewFromUtf8(isolate, "Wrong arguments number")));
		return;
	}
	// checking arguments
	double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
        // ... 
}
```
返回 Number变量
void foo(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

	//...
	args.GetReturnValue().Set(v8::Number::New(isolate, 0.0));
	return;
}
### args -> string
node_0.12
void foo(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	if( args[0]->IsUndefined())
	{
		std::cerr << "In foo(): parameters undefined." << std::endl;
		return;
	}
	v8::Handle<v8::Value> theArg = args[0];

	if (theArg->IsString()) 
	{
		v8::String::Utf8Value utf8_value(theArg);
		std::string variableName = *utf8_value;
	}
}
node_4.3

```python
void MyClass::foo(const FunctionCallbackInfo<value>& args)   
{  
    Isolate* isolate = args.GetIsolate();  
    MyClass* obj = ObjectWrap::Unwrap<MyClass>(args.Holder());  
  
	// checking arguments number
	if (args.Length() < 4)
	{
		isolate->ThrowException(
				v8::Exception::TypeError(
						String::NewFromUtf8(isolate,
								"Wrong arguments number")));
		return;
	}
	// checking arguments
	for (int i = 0; i < 4; i++)
	{
		if (args[i]->IsUndefined())
		{
			printf("create_sparse_array: argument %d is an undefined value.\n",
					i);
			isolate->ThrowException(
					v8::Exception::TypeError(
							String::NewFromUtf8(isolate,
									"create_sparse_array: one argument is an undefined value.")));
			return;
		}
	} 
  
	if (args[0]->IsString())
	{
		char *name;
		v8::String::Utf8Value str(args[0]->ToString());
		name = *str;
		// ...  
	}
        // ...   
}
```
返回 String变量
void foo(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

	std::string name = "foo";
	//...
	args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate,
			name.c_str()));
	return;
}
### args -> bool

```python
void foo(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	MyClass* obj = ObjectWrap::Unwrap<MyClass>(args.Holder());
	// checking arguments number
	if (args.Length() < 3)
	{
		isolate->ThrowException( 
                    v8::Exception::TypeError( 
                        String::NewFromUtf8(isolate, "Wrong arguments number")));
		return;
	}
	// checking arguments
	if( args[0]->IsUndefined())
	{
		isolate->ThrowException( 
                     v8::Exception::TypeError( 
                         String::NewFromUtf8(isolate, "The first argument undefined.")));
		return;
	}
	if (args[0]->IsBoolean()) 
	{
		bool value = args[0]->BooleanValue();
	}
        // ... 
}
```

返回Boolean变量
void foo(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

	//...
	args.GetReturnValue().Set(v8::Boolean::New(isolate, true)); 	
	return;
}

### args -> Array
```python
void foo(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	// checking arguments number
	if (args.Length() < 3)
	{
		isolate->ThrowException( 
                    v8::Exception::TypeError( 
                        String::NewFromUtf8(isolate, "Wrong arguments number")));
		return;
	}
	// checking arguments
	if( args[0]->IsUndefined())
	{
		isolate->ThrowException( 
                     v8::Exception::TypeError( 
                         String::NewFromUtf8(isolate, "The first argument undefined.")));
		return;
	}
	v8::Handle<v8::Value> theArg = args[0];
	if (theArg->IsArray()) 
	{
		v8::Handle<v8::Array> theArray = v8::Handle<v8::Array>::Cast(theArg);
		int length = theArray->Length();
		v8::Handle<v8::Value> arrayItem = theArray->Get(v8::Int32::New(isolate, 0));
                // ...
	}
}
```
返回Array变量
void foo(const FunctionCallbackInfo<Value>& args) 
{
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

	//...

	v8::Handle<v8::Array> resultArray = v8::Array::New(isolate);
	resultArray->Set(v8::Integer::New(isolate, i), v8::Number::New(isolate, 1.1));
	args.GetReturnValue().Set(resultArray); 	
	return;
}
## args->char*
```python
void MyClass::foo(const FunctionCallbackInfo<value>& args) 
{
	Isolate* isolate = args.GetIsolate();
	MyClass* obj = ObjectWrap::Unwrap<MyClass>(args.Holder());
	// checking arguments number
	if (args.Length() < 3)
	{
		isolate->ThrowException( 
                        v8::Exception::TypeError( 
                             String::NewFromUtf8(isolate, "Wrong arguments number")));
		return;
	}
	// checking arguments
	v8::Handle<v8::Value> theArg = args[0];
	if (Buffer::HasInstance(theArg)) {
		size_t size = Buffer::Length(theArg->ToObject());
		char* bufferdata = Buffer::Data(theArg->ToObject());
		//to do with size and bufferdata
		return scope.Close(Number::New(size)); 
	}
	else {
		args.GetReturnValue().Set(v8::Boolean::New(isolate, false)); 	
		return;
	}
        // ... 
}
```


