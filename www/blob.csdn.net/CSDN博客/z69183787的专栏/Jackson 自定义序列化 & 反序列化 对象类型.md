# Jackson 自定义序列化 & 反序列化 对象类型 - z69183787的专栏 - CSDN博客
2018年02月12日 16:25:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3009
```java
public class JacksonUtils {
    private static final ObjectMapper mapper = new ObjectMapper();
    public static final String CLASS_KEY = "@class";
    public static final String BASE_VALUE_KEY = "\"value\"";
    public static final String BASE_VALUE_NODE_KEY = "value";
    static {
        SimpleModule module = new SimpleModule();
        mapper.enableDefaultTypingAsProperty(NON_FINAL, CLASS_KEY);
        module.addKeyDeserializer(Object.class, new MapKeyDeserializer());
        module.addKeySerializer(Object.class, new MapKeySerializer());
        mapper.disable(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES);
        mapper.registerModule(module);
    }
    public static String serialize(Object obj) throws SerializationException {
        try {
            return mapper.writeValueAsString(obj);
        } catch (Throwable t) {
            throw new SerializationException(t);
        }
    }
    public static <T> T deserialize(String jsonString, Class<T> clazz) throws
            SerializationException {
        try {
            return mapper.readValue(jsonString, clazz);
        } catch (Throwable t) {
            throw new SerializationException(t);
        }
    }
    public static JsonNode readNode(String jsonString) throws SerializationException {
        try {
            return mapper.readTree(jsonString);
        } catch (Throwable t) {
            throw new SerializationException(t);
        }
    }
}
```
```java
public class MapKeyDeserializer extends com.fasterxml.jackson.databind.KeyDeserializer {
    @Override
    public Object deserializeKey(String key, DeserializationContext ctxt) throws IOException {
        JsonNode jsonNode = JacksonUtils.readNode(key);
        JsonNode classNode = jsonNode.get(JacksonUtils.CLASS_KEY);
        Class<?> clz = null;
        try {
            clz = Class.forName(classNode.asText());
        } catch (ClassNotFoundException e) {
            throw new IOException(e);
        }
        JsonNode valueNode = jsonNode.get(JacksonUtils.BASE_VALUE_NODE_KEY);
        if (clz.equals(Integer.class)) {
            return Integer.parseInt(valueNode.asText());
        } else if (clz.equals(Boolean.class)) {
            return Boolean.parseBoolean(valueNode.asText());
        } else if (clz.equals(Byte.class)) {
            return Byte.parseByte(valueNode.asText());
        } else if (clz.equals(Short.class)) {
            return Short.parseShort(valueNode.asText());
        } else if (clz.equals(Long.class)) {
            return Long.parseLong(valueNode.asText());
        } else if (clz.equals(Float.class)) {
            return Float.parseFloat(valueNode.asText());
        } else if (clz.equals(Double.class)) {
            return Double.parseDouble(valueNode.asText());
        } else if (clz.equals(String.class)) {
            return valueNode.asText();
        } else if (clz.isEnum()) {
            return JacksonUtils.deserialize(valueNode.toString(), clz);
        } else if (clz.isArray()) {
            return JacksonUtils.deserialize(valueNode.toString(), clz);
        }
        return JacksonUtils.deserialize(key, clz);
    }
}
```
```java
public class MapKeySerializer extends com.fasterxml.jackson.databind.JsonSerializer {
    @Override
    public void serialize(Object value, JsonGenerator gen, SerializerProvider serializers) throws IOException {
        String keyString = JacksonUtils.serialize(value);
        boolean isFinal = Modifier.isFinal(value.getClass().getModifiers());
        StringBuilder key = new StringBuilder();
        if (isFinal) {
            key.append("{\"").append(JacksonUtils.CLASS_KEY).append("\":\"").append(value.getClass().getName()).append("\",");
            if (value instanceof Integer || value instanceof Boolean
                    || value instanceof Byte || value instanceof Short
                    || value instanceof Long || value instanceof Float
                    || value instanceof Double || value instanceof String) {
                key.append(JacksonUtils.BASE_VALUE_KEY).append(":").append(keyString);
            } else if (value instanceof Enum) {
                key.append(JacksonUtils.BASE_VALUE_KEY).append(":").append(keyString);
            } else if(value.getClass().isArray()){
                key.append(JacksonUtils.BASE_VALUE_KEY).append(":").append(keyString);
            } else if (keyString.startsWith("{")) {
                key.append(keyString.substring(1, keyString.length() - 1));
            } else {
                key.append(keyString);
            }
            key.append("}");
            keyString = key.toString();
        }
        gen.writeFieldName(keyString);
    }
}
```
