# java反射：使用asm获取方法的参数名 - 零度的博客专栏 - CSDN博客
2016年05月05日 11:24:56[零度anngle](https://me.csdn.net/zmx729618)阅读数：1187

```java
<span class="kwd">package</span><span class="pln"> cn</span><span class="pun">.</span><span class="pln">outofmemory</span><span class="pun">.</span><span class="pln">demo</span><span class="pun">;</span><span class="pln">
</span><span class="kwd">import</span><span class="pln"> java</span><span class="pun">.</span><span class="pln">io</span><span class="pun">.</span><span class="typ">IOException</span><span class="pun">;</span><span class="pln">
</span><span class="kwd">import</span><span class="pln"> java</span><span class="pun">.</span><span class="pln">lang</span><span class="pun">.</span><span class="pln">reflect</span><span class="pun">.</span><span class="typ">Method</span><span class="pun">;</span><span class="pln">
</span><span class="kwd">import</span><span class="pln"> java</span><span class="pun">.</span><span class="pln">lang</span><span class="pun">.</span><span class="pln">reflect</span><span class="pun">.</span><span class="typ">Modifier</span><span class="pun">;</span><span class="pln">
</span><span class="kwd">import</span><span class="pln"> java</span><span class="pun">.</span><span class="pln">util</span><span class="pun">.</span><span class="typ">Arrays</span><span class="pun">;</span><span class="pln">
</span><span class="kwd">import</span><span class="pln"> org</span><span class="pun">.</span><span class="pln">objectweb</span><span class="pun">.</span><span class="kwd">asm</span><span class="pun">.</span><span class="typ">ClassReader</span><span class="pun">;</span><span class="pln">
</span><span class="kwd">import</span><span class="pln"> org</span><span class="pun">.</span><span class="pln">objectweb</span><span class="pun">.</span><span class="kwd">asm</span><span class="pun">.</span><span class="typ">ClassVisitor</span><span class="pun">;</span><span class="pln">
</span><span class="kwd">import</span><span class="pln"> org</span><span class="pun">.</span><span class="pln">objectweb</span><span class="pun">.</span><span class="kwd">asm</span><span class="pun">.</span><span class="typ">Label</span><span class="pun">;</span><span class="pln">
</span><span class="kwd">import</span><span class="pln"> org</span><span class="pun">.</span><span class="pln">objectweb</span><span class="pun">.</span><span class="kwd">asm</span><span class="pun">.</span><span class="typ">MethodVisitor</span><span class="pun">;</span><span class="pln">
</span><span class="kwd">import</span><span class="pln"> org</span><span class="pun">.</span><span class="pln">objectweb</span><span class="pun">.</span><span class="kwd">asm</span><span class="pun">.</span><span class="typ">Opcodes</span><span class="pun">;</span><span class="pln">
</span><span class="kwd">import</span><span class="pln"> org</span><span class="pun">.</span><span class="pln">objectweb</span><span class="pun">.</span><span class="kwd">asm</span><span class="pun">.</span><span class="typ">Type</span><span class="pun">;</span><span class="pln">
</span><span class="com">/**
 * <p>
 * 基于asm的工具类
 * </p>
 * 
 * @author dixingxing
 * @date Apr 23, 2012
 */</span><span class="pln">
</span><span class="kwd">public</span><span class="pln"> </span><span class="kwd">final</span><span class="pln"> </span><span class="kwd">class</span><span class="pln"> </span><span class="typ">Classes</span><span class="pln"> </span><span class="pun">{</span><span class="pln">
    </span><span class="kwd">private</span><span class="pln"> </span><span class="typ">Classes</span><span class="pun">()</span><span class="pln"> </span><span class="pun">{</span><span class="pln">
    </span><span class="pun">}</span><span class="pln">
    </span><span class="com">/**
     * 
     * <p>
     * 比较参数类型是否一致
     * </p>
     * 
     * @param types
     *            asm的类型({@link Type})
     * @param clazzes
     *            java 类型({@link Class})
     * @return
     */</span><span class="pln">
    </span><span class="kwd">private</span><span class="pln"> </span><span class="kwd">static</span><span class="pln"> </span><span class="kwd">boolean</span><span class="pln"> sameType</span><span class="pun">(</span><span class="typ">Type</span><span class="pun">[]</span><span class="pln"> types</span><span class="pun">,</span><span class="pln"> </span><span class="typ">Class</span><span class="pun"><?>[]</span><span class="pln"> clazzes</span><span class="pun">)</span><span class="pln"> </span><span class="pun">{</span><span class="pln">
        </span><span class="com">// 个数不同</span><span class="pln">
        </span><span class="kwd">if</span><span class="pln"> </span><span class="pun">(</span><span class="pln">types</span><span class="pun">.</span><span class="pln">length </span><span class="pun">!=</span><span class="pln"> clazzes</span><span class="pun">.</span><span class="pln">length</span><span class="pun">)</span><span class="pln"> </span><span class="pun">{</span><span class="pln">
            </span><span class="kwd">return</span><span class="pln"> </span><span class="kwd">false</span><span class="pun">;</span><span class="pln">
        </span><span class="pun">}</span><span class="pln">
        </span><span class="kwd">for</span><span class="pln"> </span><span class="pun">(</span><span class="kwd">int</span><span class="pln"> i </span><span class="pun">=</span><span class="pln"> </span><span class="lit">0</span><span class="pun">;</span><span class="pln"> i </span><span class="pun"><</span><span class="pln"> types</span><span class="pun">.</span><span class="pln">length</span><span class="pun">;</span><span class="pln"> i</span><span class="pun">++)</span><span class="pln"> </span><span class="pun">{</span><span class="pln">
            </span><span class="kwd">if</span><span class="pln"> </span><span class="pun">(!</span><span class="typ">Type</span><span class="pun">.</span><span class="pln">getType</span><span class="pun">(</span><span class="pln">clazzes</span><span class="pun">[</span><span class="pln">i</span><span class="pun">]).</span><span class="pln">equals</span><span class="pun">(</span><span class="pln">types</span><span class="pun">[</span><span class="pln">i</span><span class="pun">]))</span><span class="pln"> </span><span class="pun">{</span><span class="pln">
                </span><span class="kwd">return</span><span class="pln"> </span><span class="kwd">false</span><span class="pun">;</span><span class="pln">
            </span><span class="pun">}</span><span class="pln">
        </span><span class="pun">}</span><span class="pln">
        </span><span class="kwd">return</span><span class="pln"> </span><span class="kwd">true</span><span class="pun">;</span><span class="pln">
    </span><span class="pun">}</span><span class="pln">
    </span><span class="com">/**
     * 
     * <p>
     * 获取方法的参数名
     * </p>
     * 
     * @param m
     * @return
     */</span><span class="pln">
    </span><span class="kwd">public</span><span class="pln"> </span><span class="kwd">static</span><span class="pln"> </span><span class="typ">String</span><span class="pun">[]</span><span class="pln"> getMethodParamNames</span><span class="pun">(</span><span class="kwd">final</span><span class="pln"> </span><span class="typ">Method</span><span class="pln"> m</span><span class="pun">)</span><span class="pln"> </span><span class="pun">{</span><span class="pln">
        </span><span class="kwd">final</span><span class="pln"> </span><span class="typ">String</span><span class="pun">[]</span><span class="pln"> paramNames </span><span class="pun">=</span><span class="pln"> </span><span class="kwd">new</span><span class="pln"> </span><span class="typ">String</span><span class="pun">[</span><span class="pln">m</span><span class="pun">.</span><span class="pln">getParameterTypes</span><span class="pun">().</span><span class="pln">length</span><span class="pun">];</span><span class="pln">
        </span><span class="kwd">final</span><span class="pln"> </span><span class="typ">String</span><span class="pln"> n </span><span class="pun">=</span><span class="pln"> m</span><span class="pun">.</span><span class="pln">getDeclaringClass</span><span class="pun">().</span><span class="pln">getName</span><span class="pun">();</span><span class="pln">
        </span><span class="typ">ClassReader</span><span class="pln"> cr </span><span class="pun">=</span><span class="pln"> </span><span class="kwd">null</span><span class="pun">;</span><span class="pln">
        </span><span class="kwd">try</span><span class="pln"> </span><span class="pun">{</span><span class="pln">
            cr </span><span class="pun">=</span><span class="pln"> </span><span class="kwd">new</span><span class="pln"> </span><span class="typ">ClassReader</span><span class="pun">(</span><span class="pln">n</span><span class="pun">);</span><span class="pln">
        </span><span class="pun">}</span><span class="pln"> </span><span class="kwd">catch</span><span class="pln"> </span><span class="pun">(</span><span class="typ">IOException</span><span class="pln"> e</span><span class="pun">)</span><span class="pln"> </span><span class="pun">{</span><span class="pln">
            </span><span class="kwd">throw</span><span class="pln"> </span><span class="kwd">new</span><span class="pln"> </span><span class="typ">RuntimeException</span><span class="pun">(</span><span class="pln">e</span><span class="pun">);</span><span class="pln">
        </span><span class="pun">}</span><span class="pln">
        cr</span><span class="pun">.</span><span class="pln">accept</span><span class="pun">(</span><span class="kwd">new</span><span class="pln"> </span><span class="typ">ClassVisitor</span><span class="pun">(</span><span class="typ">Opcodes</span><span class="pun">.</span><span class="pln">ASM4</span><span class="pun">)</span><span class="pln"> </span><span class="pun">{</span><span class="pln">
            </span><span class="lit">@Override</span><span class="pln">
            </span><span class="kwd">public</span><span class="pln"> </span><span class="typ">MethodVisitor</span><span class="pln"> visitMethod</span><span class="pun">(</span><span class="kwd">final</span><span class="pln"> </span><span class="kwd">int</span><span class="pln"> access</span><span class="pun">,</span><span class="pln">
                    </span><span class="kwd">final</span><span class="pln"> </span><span class="typ">String</span><span class="pln"> name</span><span class="pun">,</span><span class="pln"> </span><span class="kwd">final</span><span class="pln"> </span><span class="typ">String</span><span class="pln"> desc</span><span class="pun">,</span><span class="pln">
                    </span><span class="kwd">final</span><span class="pln"> </span><span class="typ">String</span><span class="pln"> signature</span><span class="pun">,</span><span class="pln"> </span><span class="kwd">final</span><span class="pln"> </span><span class="typ">String</span><span class="pun">[]</span><span class="pln"> exceptions</span><span class="pun">)</span><span class="pln"> </span><span class="pun">{</span><span class="pln">
                </span><span class="kwd">final</span><span class="pln"> </span><span class="typ">Type</span><span class="pun">[]</span><span class="pln"> args </span><span class="pun">=</span><span class="pln"> </span><span class="typ">Type</span><span class="pun">.</span><span class="pln">getArgumentTypes</span><span class="pun">(</span><span class="pln">desc</span><span class="pun">);</span><span class="pln">
                </span><span class="com">// 方法名相同并且参数个数相同</span><span class="pln">
                </span><span class="kwd">if</span><span class="pln"> </span><span class="pun">(!</span><span class="pln">name</span><span class="pun">.</span><span class="pln">equals</span><span class="pun">(</span><span class="pln">m</span><span class="pun">.</span><span class="pln">getName</span><span class="pun">())</span><span class="pln">
                        </span><span class="pun">||</span><span class="pln"> </span><span class="pun">!</span><span class="pln">sameType</span><span class="pun">(</span><span class="pln">args</span><span class="pun">,</span><span class="pln"> m</span><span class="pun">.</span><span class="pln">getParameterTypes</span><span class="pun">()))</span><span class="pln"> </span><span class="pun">{</span><span class="pln">
                    </span><span class="kwd">return</span><span class="pln"> </span><span class="kwd">super</span><span class="pun">.</span><span class="pln">visitMethod</span><span class="pun">(</span><span class="pln">access</span><span class="pun">,</span><span class="pln"> name</span><span class="pun">,</span><span class="pln"> desc</span><span class="pun">,</span><span class="pln"> signature</span><span class="pun">,</span><span class="pln">
                            exceptions</span><span class="pun">);</span><span class="pln">
                </span><span class="pun">}</span><span class="pln">
                </span><span class="typ">MethodVisitor</span><span class="pln"> v </span><span class="pun">=</span><span class="pln"> </span><span class="kwd">super</span><span class="pun">.</span><span class="pln">visitMethod</span><span class="pun">(</span><span class="pln">access</span><span class="pun">,</span><span class="pln"> name</span><span class="pun">,</span><span class="pln"> desc</span><span class="pun">,</span><span class="pln">
                        signature</span><span class="pun">,</span><span class="pln"> exceptions</span><span class="pun">);</span><span class="pln">
                </span><span class="kwd">return</span><span class="pln"> </span><span class="kwd">new</span><span class="pln"> </span><span class="typ">MethodVisitor</span><span class="pun">(</span><span class="typ">Opcodes</span><span class="pun">.</span><span class="pln">ASM4</span><span class="pun">,</span><span class="pln"> v</span><span class="pun">)</span><span class="pln"> </span><span class="pun">{</span><span class="pln">
                    </span><span class="lit">@Override</span><span class="pln">
                    </span><span class="kwd">public</span><span class="pln"> </span><span class="kwd">void</span><span class="pln"> visitLocalVariable</span><span class="pun">(</span><span class="typ">String</span><span class="pln"> name</span><span class="pun">,</span><span class="pln"> </span><span class="typ">String</span><span class="pln"> desc</span><span class="pun">,</span><span class="pln">
                            </span><span class="typ">String</span><span class="pln"> signature</span><span class="pun">,</span><span class="pln"> </span><span class="typ">Label</span><span class="pln"> start</span><span class="pun">,</span><span class="pln"> </span><span class="typ">Label</span><span class="pln"> </span><span class="kwd">end</span><span class="pun">,</span><span class="pln"> </span><span class="kwd">int</span><span class="pln"> index</span><span class="pun">)</span><span class="pln"> </span><span class="pun">{</span><span class="pln">
                        </span><span class="kwd">int</span><span class="pln"> i </span><span class="pun">=</span><span class="pln"> index </span><span class="pun">-</span><span class="pln"> </span><span class="lit">1</span><span class="pun">;</span><span class="pln">
                        </span><span class="com">// 如果是静态方法，则第一就是参数</span><span class="pln">
                        </span><span class="com">// 如果不是静态方法，则第一个是"this"，然后才是方法的参数</span><span class="pln">
                        </span><span class="kwd">if</span><span class="pln"> </span><span class="pun">(</span><span class="typ">Modifier</span><span class="pun">.</span><span class="pln">isStatic</span><span class="pun">(</span><span class="pln">m</span><span class="pun">.</span><span class="pln">getModifiers</span><span class="pun">()))</span><span class="pln"> </span><span class="pun">{</span><span class="pln">
                            i </span><span class="pun">=</span><span class="pln"> index</span><span class="pun">;</span><span class="pln">
                        </span><span class="pun">}</span><span class="pln">
                        </span><span class="kwd">if</span><span class="pln"> </span><span class="pun">(</span><span class="pln">i </span><span class="pun">>=</span><span class="pln"> </span><span class="lit">0</span><span class="pln"> </span><span class="pun">&&</span><span class="pln"> i </span><span class="pun"><</span><span class="pln"> paramNames</span><span class="pun">.</span><span class="pln">length</span><span class="pun">)</span><span class="pln"> </span><span class="pun">{</span><span class="pln">
                            paramNames</span><span class="pun">[</span><span class="pln">i</span><span class="pun">]</span><span class="pln"> </span><span class="pun">=</span><span class="pln"> name</span><span class="pun">;</span><span class="pln">
                        </span><span class="pun">}</span><span class="pln">
                        </span><span class="kwd">super</span><span class="pun">.</span><span class="pln">visitLocalVariable</span><span class="pun">(</span><span class="pln">name</span><span class="pun">,</span><span class="pln"> desc</span><span class="pun">,</span><span class="pln"> signature</span><span class="pun">,</span><span class="pln"> start</span><span class="pun">,</span><span class="pln">
                                </span><span class="kwd">end</span><span class="pun">,</span><span class="pln"> index</span><span class="pun">);</span><span class="pln">
                    </span><span class="pun">}</span><span class="pln">
                </span><span class="pun">};</span><span class="pln">
            </span><span class="pun">}</span><span class="pln">
        </span><span class="pun">},</span><span class="pln"> </span><span class="lit">0</span><span class="pun">);</span><span class="pln">
        </span><span class="kwd">return</span><span class="pln"> paramNames</span><span class="pun">;</span><span class="pln">
    </span><span class="pun">}</span><span class="pln">
    </span><span class="kwd">public</span><span class="pln"> </span><span class="kwd">static</span><span class="pln"> </span><span class="kwd">void</span><span class="pln"> main</span><span class="pun">(</span><span class="typ">String</span><span class="pun">[]</span><span class="pln"> args</span><span class="pun">)</span><span class="pln"> </span><span class="kwd">throws</span><span class="pln"> </span><span class="typ">SecurityException</span><span class="pun">,</span><span class="pln">
            </span><span class="typ">NoSuchMethodException</span><span class="pln"> </span><span class="pun">{</span><span class="pln">
        </span><span class="typ">String</span><span class="pun">[]</span><span class="pln"> s </span><span class="pun">=</span><span class="pln"> getMethodParamNames</span><span class="pun">(</span><span class="typ">Classes</span><span class="pun">.</span><span class="kwd">class</span><span class="pun">.</span><span class="pln">getMethod</span><span class="pun">(</span><span class="pln">
                </span><span class="str">"getMethodParamNames"</span><span class="pun">,</span><span class="pln"> </span><span class="typ">Method</span><span class="pun">.</span><span class="kwd">class</span><span class="pun">));</span><span class="pln">
        </span><span class="typ">System</span><span class="pun">.</span><span class="kwd">out</span><span class="pun">.</span><span class="pln">println</span><span class="pun">(</span><span class="typ">Arrays</span><span class="pun">.</span><span class="pln">toString</span><span class="pun">(</span><span class="pln">s</span><span class="pun">));</span><span class="pln">
        s </span><span class="pun">=</span><span class="pln"> getMethodParamNames</span><span class="pun">(</span><span class="typ">Classes</span><span class="pun">.</span><span class="kwd">class</span><span class="pun">.</span><span class="pln">getDeclaredMethod</span><span class="pun">(</span><span class="str">"sameType"</span><span class="pun">,</span><span class="pln">
                </span><span class="typ">Type</span><span class="pun">[].</span><span class="kwd">class</span><span class="pun">,</span><span class="pln"> </span><span class="typ">Class</span><span class="pun">[].</span><span class="kwd">class</span><span class="pun">));</span><span class="pln">
        </span><span class="typ">System</span><span class="pun">.</span><span class="kwd">out</span><span class="pun">.</span><span class="pln">println</span><span class="pun">(</span><span class="typ">Arrays</span><span class="pun">.</span><span class="pln">toString</span><span class="pun">(</span><span class="pln">s</span><span class="pun">));</span><span class="pln">
        s </span><span class="pun">=</span><span class="pln"> getMethodParamNames</span><span class="pun">(</span><span class="typ">MethodVisitor</span><span class="pun">.</span><span class="kwd">class</span><span class="pun">.</span><span class="pln">getDeclaredMethod</span><span class="pun">(</span><span class="str">"visitAnnotation"</span><span class="pun">,</span><span class="pln">
                </span><span class="typ">String</span><span class="pun">.</span><span class="kwd">class</span><span class="pun">,</span><span class="typ">Boolean</span><span class="pun">.</span><span class="pln">TYPE</span><span class="pun">));</span><span class="pln">
        </span><span class="typ">System</span><span class="pun">.</span><span class="kwd">out</span><span class="pun">.</span><span class="pln">println</span><span class="pun">(</span><span class="typ">Arrays</span><span class="pun">.</span><span class="pln">toString</span><span class="pun">(</span><span class="pln">s</span><span class="pun">));</span><span class="pln">
        </span><span class="com">// 对String，Object，thread等jdk自带类型不起作用</span><span class="pln">
    </span><span class="pun">}</span><span class="pln">
</span><span class="pun">}</span><span class="pln">
</span>
```
