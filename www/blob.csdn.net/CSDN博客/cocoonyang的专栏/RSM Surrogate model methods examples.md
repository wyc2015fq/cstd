# RSM Surrogate model methods examples - cocoonyang的专栏 - CSDN博客





2017年10月17日 22:32:04[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：297








RSM Surrogate model methods examples：

```
// Module
var  RSM = require ("../Build/Release/rsm").RSM;
// Module Object 
var rsm = new RSM;

// Data Set
var data = [
    [[37.5, 37.5], [803.78]],
    [[230.101, 805.777], [815.73]],
    [[447.21, 557.351], [813.86]],
    [[520.839, 354.306], [813.8]],
    [[999.713, 801.97], [819.05]],
    [[77.3747, 506.866], [815.59]],
    [[916.19, 211.731], [821.18]],
    [[170.566, 918.607], [816.67]],
    [[703.838, 975.497], [818.97]],
    [[685.363, 189.721], [819.41]],
    [[43.4607, 987.401], [817.23]],
    [[181.685, 359.548], [816.67]],
    [[516.911, 12.7494], [814]],
    [[520.597, 354.396], [813.8]],
    [[221.671, 554.194], [816.45]],
    [[38.4605, 535.462], [817.28]],
    [[628.684, 584.226], [812.84]],
    [[419.248, 428.101], [813.87]],
    [[358.279, 319.41], [814.88]],
    [[163.466, 185.673], [817.64]],
    [[147.272, 948.653], [816.74]],
    [[852.848, 922.912], [819.05]],
    [[669.124, 345.997], [819.6]],
    [[853.498, 636.962], [819.05]],
    [[369.065, 493.693], [814.87]],
    [[719.035, 248.153], [819.14]],
    [[572.292, 452.362], [813.12]],
    [[740.381, 25.9373], [819.41]],
 ]; 

// Methods
function relation( x1, x2 )
{
      //var res = x1*x1*x1 +x2*x2*x2 + 2.0*x1*x1*x2;
      var res =  x1*x1 +x2*x2  + 2.0*x1*x2 + x1*x1*x1 +x2*x2*x2;
      // var res =  x1*x1 + 3.0*x2*x2  + 2.0*x1*x2 +100.0; 
      // var res =  100.0 + x1 + x2 
      return res;
}
function reformData() 
{
    var len = data.length;
    for( var i = 0; i < len; i++ )
    {
      var x1 = data[i][0][0];
      var x2 = data[i][0][1];
      var res = relation( x1, x2 );        
      data[i][1][0] = res ;
    }
    //console.log("data:", data );
} 

reformData();

var rsm =  new RSM();
rsm.setRankNumber( 3 );

rsm.setExperimentData( data );

var RSMModel = rsm.getModelExpression()
console.log("RSM Model:", RSMModel );

var response =  relation( 740.381, 25.9373 );
var result = rsm.evaluate( [740.381, 25.9373] );
console.log("Response:", response );
console.log("RSM:", result );
```

1-order RSM Test Results:

```
Response Surface Method Report
         ==============================
     input variable number: 2
  response variable number: 1
               rank number: 1
             require cross: 1
   require standardization: 0
  minimize data set number: 3
     input data set number: 28

       RSM experiment data result 
Experiment data set number: 3
          Response average: 805.146
    RSM prediction average: 805.146
       Error value average: 0
                  Error(%): 0

              Testing data result 
   Testing data set number: 25
          Response average: 1077.78
    RSM prediction average: 1077.78
       Error value average: 0.000113525
                  Error(%): 1.05333e-05

            Total data set statistics 
     Total data set number: 28
          Response average: 1048.57
    RSM prediction average: 1048.57
       Error value average: 0.000101362
                  Error(%): 9.66673e-06

RSM Model: 100 + 1.000000*varX_0 + 1.000000*varX_1
Response: 866.3183
RSM: 866.3184566497803
```

2-order RSM Test Results:

```
Response Surface Method Report
         ==============================
     input variable number: 2
  response variable number: 1
               rank number: 2
             require cross: 1
   require standardization: 0
  minimize data set number: 6
     input data set number: 28

       RSM experiment data result 
Experiment data set number: 6
          Response average: 1.73592e+06
    RSM prediction average: 1.73592e+06
       Error value average: 0.0208333
                  Error(%): 1.20013e-06

              Testing data result 
   Testing data set number: 22
          Response average: 1.69869e+06
    RSM prediction average: 1.69869e+06
       Error value average: 0.214489
                  Error(%): 1.26267e-05

            Total data set statistics 
     Total data set number: 28
          Response average: 1.70667e+06
    RSM prediction average: 1.70667e+06
       Error value average: 0.172991
                  Error(%): 1.01362e-05

RSM Model: 100.015 + 0.000105*varX_0 -0.000537*varX_1 + 1.000001*varX_0*varX_0 + 1.999999*varX_1*varX_0 + 3.000001*varX_1*varX_1
Response: 588689.2239774701
RSM: 588689.6932533896
```

3-order RSM Test Results:

```
Response Surface Method Report
         ==============================
     input variable number: 2
  response variable number: 1
               rank number: 3
             require cross: 1
   require standardization: 0
  minimize data set number: 10
     input data set number: 28

       RSM experiment data result 
Experiment data set number: 10
          Response average: 5.80628e+08
    RSM prediction average: 5.80628e+08
       Error value average: 26.4
                  Error(%): 4.5468e-06

              Testing data result 
   Testing data set number: 18
          Response average: 3.94894e+08
    RSM prediction average: 3.94894e+08
       Error value average: 111.278
                  Error(%): 2.81792e-05

            Total data set statistics 
     Total data set number: 28
          Response average: 4.61227e+08
    RSM prediction average: 4.61227e+08
       Error value average: 80.9643
                  Error(%): 1.75541e-05

RSM Model: -85.2369 + 2.659389*varX_0 -0.107714*varX_1 + 0.994998*varX_0*varX_0 + 1.997093*varX_1*varX_0 + 1.000277*varX_1*varX_1 + 1.000003*varX_0*varX_0*varX_0 + 0.000001*varX_1*varX_0*varX_0 + 0.000002*varX_1*varX_1*varX_0 + 1.000000*varX_1*varX_1*varX_1
Response: 406454922.00043535
RSM: 406455147.7619492
```



