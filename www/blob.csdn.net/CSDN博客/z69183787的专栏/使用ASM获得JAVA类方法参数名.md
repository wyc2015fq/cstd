# 使用ASM获得JAVA类方法参数名 - z69183787的专栏 - CSDN博客
2018年07月19日 15:27:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：202
在Java1.8之后，可以通过反射API java.lang.reflect.Executable.getParameters来获取到方法参数的元信息，
（在使用编译器时加上-parameters参数，它会在生成的.class文件中额外存储参数的元信息）
但是在JDK1.7及以下版本的API并不能获取到函数的参数名称，这时候可以使用字节码工具ASM来实现这一功能
```java
import org.objectweb.asm.*;
import java.io.IOException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.Arrays;
public class GetMethodParamNameTest {
    static class Test {
        void method(String name, Object value) {
        }
    }
    public static void main(String[] args) throws SecurityException, NoSuchMethodException, IOException {
        Method method1 = Test.class.getDeclaredMethod("method", String.class, Object.class);
        System.out.println(Arrays.toString(getMethodParamNames(method1)));
    }
    /** 使用字节码工具ASM来获取方法的参数名 */
    public static String[] getMethodParamNames(final Method method) throws IOException {
        final String methodName = method.getName();
        final Class<?>[] methodParameterTypes = method.getParameterTypes();
        final int methodParameterCount = methodParameterTypes.length;
        final String className = method.getDeclaringClass().getName();
        final boolean isStatic = Modifier.isStatic(method.getModifiers());
        final String[] methodParametersNames = new String[methodParameterCount];
        ClassReader cr = new ClassReader(className);
        ClassWriter cw = new ClassWriter(ClassWriter.COMPUTE_MAXS);
        cr.accept(new ClassAdapter(cw) {
            public MethodVisitor visitMethod(int access, String name, String desc, String signature, String[] exceptions) {
                MethodVisitor mv = super.visitMethod(access, name, desc, signature, exceptions);
                final Type[] argTypes = Type.getArgumentTypes(desc);
                //参数类型不一致
                if (!methodName.equals(name) || !matchTypes(argTypes, methodParameterTypes)) {
                    return mv;
                }
                return new MethodAdapter(mv) {
                    public void visitLocalVariable(String name, String desc, String signature, Label start, Label end, int index) {
                        //如果是静态方法，第一个参数就是方法参数，非静态方法，则第一个参数是 this ,然后才是方法的参数
                        int methodParameterIndex = isStatic ? index : index - 1;
                        if (0 <= methodParameterIndex && methodParameterIndex < methodParameterCount) {
                            methodParametersNames[methodParameterIndex] = name;
                        }
                        super.visitLocalVariable(name, desc, signature, start, end, index);
                    }
                };
            }
        }, 0);
        return methodParametersNames;
    }
    /**
     * 比较参数是否一致
     */
    private static boolean matchTypes(Type[] types, Class<?>[] parameterTypes) {
        if (types.length != parameterTypes.length) {
            return false;
        }
        for (int i = 0; i < types.length; i++) {
            if (!Type.getType(parameterTypes[i]).equals(types[i])) {
                return false;
            }
        }
        return true;
    }
}
```
.
