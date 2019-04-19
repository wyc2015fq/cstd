# ByteBuffer、byte[]、String相互转化 - sxf_123456的博客 - CSDN博客
2019年01月17日 11:17:09[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：729
@Override
public String decodeKey(ByteBuffer bytes) {
    Charset charset = Charset.forName("utf-8");
    return charset.decode(bytes).toString();
}
@Override
public byte[] decodeValue(ByteBuffer bytes) {
    int len = bytes.limit() - bytes.position();
    byte[] bytes1 = new byte[len];
    bytes.get(bytes1);
    return bytes1;
}
@Override
public ByteBuffer encodeKey(String key) {
    try {
        return ByteBuffer.wrap(key.getBytes("utf-8"));
    } catch (UnsupportedEncodingException e) {
        e.printStackTrace();
    }
    return ByteBuffer.wrap(key.getBytes());
}
@Override
public ByteBuffer encodeValue(byte[] value) {
    ByteBuffer byteBuffer = ByteBuffer.allocate(value.length);
    byteBuffer.clear();
    byteBuffer.get(value, 0, value.length);
    return byteBuffer;
}
