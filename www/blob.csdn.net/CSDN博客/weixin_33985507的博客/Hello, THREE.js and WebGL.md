# Hello, THREE.js and WebGL - weixin_33985507的博客 - CSDN博客
2017年04月19日 10:15:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
```
<html>
<head>
    <title>Hello Three</title>
    <script src="./three.js-master/build/three.min.js"></script>
    <script src="./three.js-master/examples/js/controls/OrbitControls.js"></script>
    <script language="JavaScript">
    <!--//
var scene, camera, renderer;
var controls;
var pointLight;
var floor, box;
document.onreadystatechange = function(e){
    if(document.readyState=='complete'){
        console.log('ready...');
        var W = 640, H = 480;
        var w = 100, h = 10, d = 100;
        scene = new THREE.Scene();
        var renderer = new THREE.WebGLRenderer({antialias:true});
        renderer.setClearColor(0xffffff, 0.2);
        renderer.setSize( W, H );
        document.body.appendChild( renderer.domElement );
        let position = new THREE.Vector3(-d*5, 0, 0);
        let lookAt = new THREE.Vector3(0, 0, 0);
        //camera = new THREE.PerspectiveCamera( 75, W / H, 0.1, 1000 );
        camera = new THREE.OrthographicCamera(-W/2, W/2, H/2, -H/2, -d*8, d*8);
        camera.up.set(0, 0, 1);
        camera.position.set(d*2, d*2, d*2);// = position;
        console.log('position: '+position+', lookAt: '+lookAt);
        camera.lookAt(lookAt);
        let cameraHelper = new THREE.CameraHelper(camera);
        scene.add(camera);
        var floorGeometry = new THREE.PlaneGeometry(W/2, H/2, 10, 10);
        var floorMaterial = new THREE.MeshBasicMaterial( {color: 0x000000} );
        floor = new THREE.Mesh(floorGeometry, floorMaterial);
        scene.add(floor);
        var boxGeometry = new THREE.BoxGeometry( w, h, d );
        var boxMaterial = new THREE.MultiMaterial([
            new THREE.MeshBasicMaterial( { color: 0xff0000 } ), // right
            new THREE.MeshBasicMaterial( { color: 0x0000ff } ), // left
            new THREE.MeshBasicMaterial( { color: 0x00ff00 } ), // top
            new THREE.MeshBasicMaterial( { color: 0xffff00 } ), // bottom
            new THREE.MeshBasicMaterial( { color: 0x00ffff } ), // back
            new THREE.MeshBasicMaterial( { color: 0xff00ff } )  // front
        ]);
        boxMaterial = new THREE.MeshLambertMaterial({
            transparent: true,
            opacity: 0.8,
            color: 0x0000ff
        });
        box = new THREE.Mesh( boxGeometry, boxMaterial );
        //box = new THREE.BoxHelper(box, 0xffffff);
        box.position.z = d/2;
        scene.add(box);
        var axisHelper = new THREE.AxisHelper(600);
        //scene.add(axisHelper);
        var sphere = new THREE.SphereGeometry( 0.5, 16, 8 );
        //Lights
        pointLight = new THREE.PointLight( 0xffffff, 3, d*5);
        //light.add( new THREE.Mesh( sphere, new THREE.MeshBasicMaterial( { color: 0xff0040 } ) ) );
        pointLight.position.set( d*3, d*3, d*3 );
        scene.add(pointLight);
        var ambientLight = new THREE.AmbientLight( 0x404040 ); // soft white light
        scene.add( ambientLight );
        var sphereSize = 1;
        var pointLightHelper = new THREE.PointLightHelper( pointLight, sphereSize );
        scene.add( pointLightHelper );
        controls = new THREE.OrbitControls( camera, render.domElement );
        controls.addEventListener( 'change', function(e){
            //controls.update();
            render();
        });
        function render(){
            requestAnimationFrame( render );
            box.rotation.x += 0.01
            box.rotation.y += 0.01
            renderer.render(scene, camera);
        }
        render();
    }
}
    //-->
    </script>
</head>
<body>
</body>
</html>
```
