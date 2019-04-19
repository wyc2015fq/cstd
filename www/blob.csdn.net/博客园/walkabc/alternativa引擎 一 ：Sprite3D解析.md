# alternativa引擎<一>：Sprite3D解析 - walkabc - 博客园
# [alternativa引擎<一>：Sprite3D解析](https://www.cnblogs.com/hitfire/articles/3265882.html)
```
1 public function render(stage3D:Stage3D):void {
  2         var i:int;
  3         var j:int;
  4         var light:Light3D;
  5         var occluder:Occluder;
  6         // Error checking
  7         if (stage3D == null) throw new TypeError("Parameter stage3D must be non-null.");
  8         // Reset the counters
  9         numDraws = 0;
 10         numTriangles = 0;
 11         // Reset the occluders
 12         occludersLength = 0;
 13         // Reset the lights
 14         lightsLength = 0;
 15         ambient[0] = 0;
 16         ambient[1] = 0;
 17         ambient[2] = 0;
 18         ambient[3] = 1;
 19         // Receiving the context
 20         var currentContext3D:Context3D = stage3D.context3D;
 21         if (currentContext3D != context3D) {
 22             if (currentContext3D != null) {
 23                 context3DProperties = context3DPropertiesPool[currentContext3D];
 24                 if (context3DProperties == null) {
 25                     context3DProperties = new RendererContext3DProperties();
 26                     context3DProperties.isConstrained = currentContext3D.driverInfo.lastIndexOf("(Baseline Constrained)") >= 0;
 27                     context3DPropertiesPool[currentContext3D] = context3DProperties;
 28                 }
 29                 context3D = currentContext3D;
 30             } else {
 31                 context3D = null;
 32                 context3DProperties = null;
 33             }
 34         }
 35         if (context3D != null && view != null && renderer != null && (view.stage != null || view._canvas != null)) {
 36             renderer.camera = this;
 37             // Projection argument calculating
 38             calculateProjection(view._width, view._height);
 39             // Preparing to rendering
 40             view.configureContext3D(stage3D, context3D, this);
 41             // Transformations calculating
 42             if (transformChanged) composeTransforms();
 43             localToGlobalTransform.copy(transform);
 44             globalToLocalTransform.copy(inverseTransform);
 45             // Searching for upper hierarchy point
 46             var root:Object3D = this;
 47             while (root.parent != null) {
 48                 root = root.parent;
 49                 if (root.transformChanged) root.composeTransforms();
 50                 localToGlobalTransform.append(root.transform);
 51                 globalToLocalTransform.prepend(root.inverseTransform);
 52             }
 53 
 54             // Check if object of hierarchy is visible
 55             if (root.visible) {
 56                 // Calculating the matrix to transform from the camera space to local space
 57                 root.cameraToLocalTransform.combine(root.inverseTransform, localToGlobalTransform);
 58                 // Calculating the matrix to transform from local space to the camera space
 59                 root.localToCameraTransform.combine(globalToLocalTransform, root.transform);
 60 
 61                 globalMouseHandlingType = root.mouseHandlingType;
 62                 // Checking the culling
 63                 if (root.boundBox != null) {
 64                     calculateFrustum(root.cameraToLocalTransform);
 65                     root.culling = root.boundBox.checkFrustumCulling(frustum, 63);
 66                 } else {
 67                     root.culling = 63;
 68                 }
 69                 // Calculations of content visibility
 70                 if (root.culling >= 0) root.calculateVisibility(this);
 71                 // Calculations  visibility of children
 72                 root.calculateChildrenVisibility(this);
 73                 // Calculations of transformations from occluder space to the camera space
 74                 for (i = 0; i < occludersLength; i++) {
 75                     occluder = occluders[i];
 76                     occluder.localToCameraTransform.calculateInversion(occluder.cameraToLocalTransform);
 77                     occluder.transformVertices(correctionX, correctionY);
 78                     occluder.distance = orthographic ? occluder.localToCameraTransform.l : (occluder.localToCameraTransform.d * occluder.localToCameraTransform.d + occluder.localToCameraTransform.h * occluder.localToCameraTransform.h + occluder.localToCameraTransform.l * occluder.localToCameraTransform.l);
 79                     occluder.enabled = true;
 80                 }
 81                 // Sorting the occluders by disance
 82                 if (occludersLength > 1) sortOccluders();
 83                 // Constructing the volumes of occluders, their intersections, starts from closest
 84                 for (i = 0; i < occludersLength; i++) {
 85                     occluder = occluders[i];
 86                     if (occluder.enabled) {
 87                         occluder.calculatePlanes(this);
 88                         if (occluder.planeList != null) {
 89                             for (j = i + 1; j < occludersLength; j++) { // It is possible, that start value should be 0
 90                                 var compared:Occluder = occluders[j];
 91                                 if (compared.enabled && compared != occluder && compared.checkOcclusion(occluder, correctionX, correctionY)) compared.enabled = false;
 92                             }
 93                         } else {
 94                             occluder.enabled = false;
 95                         }
 96                     }
 97                     // Reset of culling
 98                     occluder.culling = -1;
 99                 }
100                 //  Gather the occluders which will affects now
101                 for (i = 0, j = 0; i < occludersLength; i++) {
102                     occluder = occluders[i];
103                     if (occluder.enabled) {
104                         // Debug
105                         occluder.collectDraws(this, null, 0, false);
106                         if (debug && occluder.boundBox != null && (checkInDebug(occluder) & Debug.BOUNDS)) Debug.drawBoundBox(this, occluder.boundBox, occluder.localToCameraTransform);
107                         occluders[j] = occluder;
108                         j++;
109                     }
110                 }
111                 occludersLength = j;
112                 occluders.length = j;
113                 // Check light influence
114                 for (i = 0, j = 0; i < lightsLength; i++) {
115                     light = lights[i];
116                     light.localToCameraTransform.calculateInversion(light.cameraToLocalTransform);
117                     if (light.boundBox == null || occludersLength == 0 || !light.boundBox.checkOcclusion(occluders, occludersLength, light.localToCameraTransform)) {
118                         light.red = ((light.color >> 16) & 0xFF) * light.intensity / 255;
119                         light.green = ((light.color >> 8) & 0xFF) * light.intensity / 255;
120                         light.blue = (light.color & 0xFF) * light.intensity / 255;
121                         // Debug
122                         light.collectDraws(this, null, 0, false);
123                         if (debug && light.boundBox != null && (checkInDebug(light) & Debug.BOUNDS)) Debug.drawBoundBox(this, light.boundBox, light.localToCameraTransform);
124 
125                         // Shadows preparing
126                         if (light.shadow != null) {
127                             light.shadow.process(this);
128                         }
129                         lights[j] = light;
130                         j++;
131                     }
132                     light.culling = -1;
133                 }
134                 lightsLength = j;
135                 lights.length = j;
136 
137                 // Sort lights by types
138                 if (lightsLength > 0) sortLights(0, lightsLength - 1);
139 
140                 // Calculating the rays of mouse events
141                 view.calculateRays(this, (globalMouseHandlingType & Object3D.MOUSE_HANDLING_MOVING) != 0,
142                                          (globalMouseHandlingType & Object3D.MOUSE_HANDLING_PRESSING) != 0,
143                                          (globalMouseHandlingType & Object3D.MOUSE_HANDLING_WHEEL) != 0,
144                                          (globalMouseHandlingType & Object3D.MOUSE_HANDLING_MIDDLE_BUTTON) != 0,
145                                          (globalMouseHandlingType & Object3D.MOUSE_HANDLING_RIGHT_BUTTON) != 0);
146                 for (i = origins.length; i < view.raysLength; i++) {
147                     origins[i] = new Vector3D();
148                     directions[i] = new Vector3D();
149                 }
150                 raysLength = view.raysLength;
151                 
152                 if (renderClearsContext) {
153                     var r:Number = ((view.backgroundColor >> 16) & 0xff)/0xff;
154                     var g:Number = ((view.backgroundColor >> 8) & 0xff)/0xff;
155                     var b:Number = (view.backgroundColor & 0xff)/0xff;
156                     if (view._canvas != null) {
157                         r *= view.backgroundAlpha;
158                         g *= view.backgroundAlpha;
159                         b *= view.backgroundAlpha;
160                     }
161                     context3D.clear(r, g, b, view.backgroundAlpha);
162                 }
163                 
164                 // Check getting in frustum and occluding
165                 if (root.culling >= 0 && (root.boundBox == null || occludersLength == 0 || !root.boundBox.checkOcclusion(occluders, occludersLength, root.localToCameraTransform))) {
166                     // Check if the ray crossing the bounding box
167                     if (globalMouseHandlingType > 0 && root.boundBox != null) {
168                         calculateRays(root.cameraToLocalTransform);
169                         root.listening = root.boundBox.checkRays(origins, directions, raysLength);
170                     } else {
171                         root.listening = globalMouseHandlingType > 0;
172                     }
173                     // Check if object needs in lightning
174                     var excludedLightLength:int = root._excludedLights.length;
175                     if (lightsLength > 0 && root.useLights) {
176                         // Pass the lights to children and calculate appropriate transformations
177                         var childLightsLength:int = 0;
178                         if (root.boundBox != null) {
179                             for (i = 0; i < lightsLength; i++) {
180                                 light = lights[i];
181                                 // Checking light source for existing in excludedLights
182                                 j = 0;
183                                 while (j<excludedLightLength && root._excludedLights[j]!=light)    j++;
184                                 if (j<excludedLightLength) continue;
185 
186                                 light.lightToObjectTransform.combine(root.cameraToLocalTransform, light.localToCameraTransform);
187                                 // Detect influence
188                                 if (light.boundBox == null || light.checkBound(root)) {
189                                     childLights[childLightsLength] = light;
190                                     childLightsLength++;
191                                 }
192                             }
193                         } else {
194                             // Calculate transformation from light space to object space
195                             for (i = 0; i < lightsLength; i++) {
196                                 light = lights[i];
197                                 // Checking light source for existing in excludedLights
198                                 j = 0;
199                                 while (j<excludedLightLength && root._excludedLights[j]!=light)    j++;
200                                 if (j<excludedLightLength) continue;
201 
202                                 light.lightToObjectTransform.combine(root.cameraToLocalTransform, light.localToCameraTransform);
203 
204                                 childLights[childLightsLength] = light;
205                                 childLightsLength++;
206                             }
207                         }
208                         root.collectDraws(this, childLights, childLightsLength, root.useShadow);
209                     } else {
210                         root.collectDraws(this, null, 0, root.useShadow);
211                     }
212                     // Debug the boundbox
213                     if (debug && root.boundBox != null && (checkInDebug(root) & Debug.BOUNDS)) Debug.drawBoundBox(this, root.boundBox, root.localToCameraTransform);
214                 }
215                 // Gather the draws for children
216                 root.collectChildrenDraws(this, lights, lightsLength, root.useShadow);
217                 // Mouse events prosessing
218                 view.processMouseEvents(context3D, this);
219                 // Render
220                 renderer.render(context3D);
221             }
222             // Output
223             if (view._canvas == null) {
224                 if (renderPresentsContext) {
225                     context3D.present();
226                 }
227             } else {
228                 context3D.drawToBitmapData(view._canvas);
229                 context3D.present();
230             }
231         }
232         // Clearing
233         lights.length = 0;
234         childLights.length = 0;
235         occluders.length = 0;
236     }
```
看到上面的216行：root.collectChildrenDraws(this, lights, lightsLength, root.useShadow)，这句话的意思就是收集所有子节点的绘制信息。
下面详细介绍一下，是如何把各种顶点，纹理，法线信息传送到context3D的。
```
1 alternativa3d function collectChildrenDraws(camera:Camera3D, lights:Vector.<Light3D>, lightsLength:int, useShadow:Boolean):void {
 2             var i:int;
 3             var light:Light3D;
 4 
 5             for (var child:Object3D = childrenList; child != null; child = child.next) {
 6                 // Checking visibility flag
 7                 if (child.visible) {
 8                     // Check getting in frustum and occluding
 9                     if (child.culling >= 0 && (child.boundBox == null || camera.occludersLength == 0 || !child.boundBox.checkOcclusion(camera.occluders, camera.occludersLength, child.localToCameraTransform))) {
10                         // Check if the ray crossing the bounding box
11                         if (child.boundBox != null) {
12                             camera.calculateRays(child.cameraToLocalTransform);
13                             child.listening = child.boundBox.checkRays(camera.origins, camera.directions, camera.raysLength);
14                         } else {
15                             child.listening = true;
16                         }
17                         // Check if object needs in lightning
18                         var excludedLightLength:int = child._excludedLights.length;
19                         if (lightsLength > 0 && child.useLights) {
20                             // Pass the lights to children and calculate appropriate transformations
21                             var childLightsLength:int = 0;
22                             var j:int;
23                             if (child.boundBox != null) {
24                                 for (i = 0; i < lightsLength; i++) {
25                                     light = lights[i];
26                                     // Checking object for existing in excludedLights
27                                     j = 0;
28                                     while (j<excludedLightLength && child._excludedLights[j]!=light)    j++;
29                                     if (j<excludedLightLength) continue;
30 
31                                     light.lightToObjectTransform.combine(child.cameraToLocalTransform, light.localToCameraTransform);
32                                     // Detect influence
33                                     if (light.boundBox == null || light.checkBound(child)) {
34                                         camera.childLights[childLightsLength] = light;
35                                         childLightsLength++;
36                                     }
37                                 }
38                             } else {
39                                 // Calculate transformation from light space to object space
40                                 for (i = 0; i < lightsLength; i++) {
41                                     light = lights[i];
42                                     // Проверка источника света на отсутствие в excludedLights
43                                     j = 0;
44                                     while (j<excludedLightLength && child._excludedLights[j]!=light)    j++;
45                                     if (j<excludedLightLength) continue;
46                                     light.lightToObjectTransform.combine(child.cameraToLocalTransform, light.localToCameraTransform);
47                                     camera.childLights[childLightsLength] = light;
48                                     childLightsLength++;
49                                 }
50                             }
51                             child.collectDraws(camera, camera.childLights, childLightsLength, useShadow&&child.useShadow);
52                         } else {
53                             child.collectDraws(camera, null, 0, useShadow&&child.useShadow);
54                         }
55                         // Debug the boundbox
56                         if (camera.debug && child.boundBox != null && (camera.checkInDebug(child) & Debug.BOUNDS)) Debug.drawBoundBox(camera, child.boundBox, child.localToCameraTransform);
57                     }
58                     // Gather the draws for children
59                     if (child.childrenList != null) child.collectChildrenDraws(camera, lights, lightsLength, useShadow && child.useShadow);
60                 }
61             }
62         }
```
看上面代码的51行：child.collectDraws(camera, camera.childLights, childLightsLength, useShadow&&child.useShadow);每个Child都有一个collectDraws方法，这个方法是整个程序的关键之处。看下面代码
```
1 override alternativa3d function collectDraws(camera:Camera3D, lights:Vector.<Light3D>, lightsLength:int, useShadow:Boolean):void {
2             var geometry:Geometry = getGeometry(camera.context3D);
3             if (surface.material != null) surface.material.collectDraws(camera, surface, geometry, lights, lightsLength, useShadow, alwaysOnTop ? Renderer.NEXT_LAYER : -1);
4             // Mouse events.
5             if (listening) camera.view.addSurfaceToMouseEvents(surface, geometry, transformProcedure);
6         }
```
看第二行的GetGeometry方法，这个方法就是收集各种顶点信息的。
```
1 alternativa3d function getGeometry(context:Context3D):Geometry {
 2             var geometry:Geometry = geometries[context];
 3             if (geometry == null) {
 4                 geometry = new Geometry(4);
 5 
 6                 var attributes:Array = [];
 7                 attributes[0] = VertexAttributes.POSITION;
 8                 attributes[1] = VertexAttributes.POSITION;
 9                 attributes[2] = VertexAttributes.POSITION;
10                 attributes[3] = VertexAttributes.NORMAL;
11                 attributes[4] = VertexAttributes.NORMAL;
12                 attributes[5] = VertexAttributes.NORMAL;
13                 attributes[6] = VertexAttributes.TEXCOORDS[0];
14                 attributes[7] = VertexAttributes.TEXCOORDS[0];
15                 attributes[8] = VertexAttributes.TEXCOORDS[1];
16                 attributes[9] = VertexAttributes.TEXCOORDS[1];
17                 attributes[10] = VertexAttributes.TEXCOORDS[2];
18                 attributes[11] = VertexAttributes.TEXCOORDS[2];
19                 attributes[12] = VertexAttributes.TEXCOORDS[3];
20                 attributes[13] = VertexAttributes.TEXCOORDS[3];
21                 attributes[14] = VertexAttributes.TEXCOORDS[4];
22                 attributes[15] = VertexAttributes.TEXCOORDS[4];
23                 attributes[16] = VertexAttributes.TEXCOORDS[5];
24                 attributes[17] = VertexAttributes.TEXCOORDS[5];
25                 attributes[18] = VertexAttributes.TEXCOORDS[6];
26                 attributes[19] = VertexAttributes.TEXCOORDS[6];
27                 attributes[20] = VertexAttributes.TEXCOORDS[7];
28                 attributes[21] = VertexAttributes.TEXCOORDS[7];
29                 attributes[22] = VertexAttributes.TANGENT4;
30                 attributes[23] = VertexAttributes.TANGENT4;
31                 attributes[24] = VertexAttributes.TANGENT4;
32                 attributes[25] = VertexAttributes.TANGENT4;
33                 geometry.addVertexStream(attributes);
34 
35                 geometry.setAttributeValues(VertexAttributes.POSITION, Vector.<Number>([0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0]));
36                 geometry.setAttributeValues(VertexAttributes.NORMAL, Vector.<Number>([0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1]));
37                 geometry.setAttributeValues(VertexAttributes.TEXCOORDS[0], Vector.<Number>([0, 0, 0, 1, 1, 1, 1, 0]));
38                 geometry.setAttributeValues(VertexAttributes.TEXCOORDS[1], Vector.<Number>([0, 0, 0, 1, 1, 1, 1, 0]));
39                 geometry.setAttributeValues(VertexAttributes.TEXCOORDS[2], Vector.<Number>([0, 0, 0, 1, 1, 1, 1, 0]));
40                 geometry.setAttributeValues(VertexAttributes.TEXCOORDS[3], Vector.<Number>([0, 0, 0, 1, 1, 1, 1, 0]));
41                 geometry.setAttributeValues(VertexAttributes.TEXCOORDS[4], Vector.<Number>([0, 0, 0, 1, 1, 1, 1, 0]));
42                 geometry.setAttributeValues(VertexAttributes.TEXCOORDS[5], Vector.<Number>([0, 0, 0, 1, 1, 1, 1, 0]));
43                 geometry.setAttributeValues(VertexAttributes.TEXCOORDS[6], Vector.<Number>([0, 0, 0, 1, 1, 1, 1, 0]));
44                 geometry.setAttributeValues(VertexAttributes.TEXCOORDS[7], Vector.<Number>([0, 0, 0, 1, 1, 1, 1, 0]));
45 
46                 geometry.indices = Vector.<uint>([0, 1, 3, 2, 3, 1]);
47 
48                 geometry.upload(context);
49                 geometries[context] = geometry;
50             }
51             return geometry;
52         }
```
请看48行的代码；看到没有，对了，这个时候，顶点信息就已经上传到context3d中了。
```
1 override public function upload(context3D:Context3D):void {
 2             var vBuffer:VertexStream;
 3             var i:int;
 4             var numBuffers:int = _vertexStreams.length;
 5             if (_indexBuffer != null) {
 6                 // Clear old resources
 7                 _indexBuffer.dispose();
 8                 _indexBuffer = null;
 9                 for (i = 0; i < numBuffers; i++) {
10                     vBuffer = _vertexStreams[i];
11                     if (vBuffer.buffer != null) {
12                         vBuffer.buffer.dispose();
13                         vBuffer.buffer = null;
14                     }
15                 }
16             }
17             if (_indices.length <= 0 || _numVertices <= 0) {
18                 return;
19             }
20 
21             for (i = 0; i < numBuffers; i++) {
22                 vBuffer = _vertexStreams[i];
23                 var numMappings:int = vBuffer.attributes.length;
24                 var data:ByteArray = vBuffer.data;
25                 if (data == null) {
26                     throw new Error("Cannot upload without vertex buffer data.");
27                 }
28                 vBuffer.buffer = context3D.createVertexBuffer(_numVertices, numMappings);
29                 vBuffer.buffer.uploadFromByteArray(data, 0, 0, _numVertices);
30             }
31             var numIndices:int = _indices.length;
32             _indexBuffer = context3D.createIndexBuffer(numIndices);
33             _indexBuffer.uploadFromVector(_indices, 0, numIndices);
34         }
```
至此，这个3d引擎的Sprite3D就介绍完了。

