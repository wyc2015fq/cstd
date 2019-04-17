# Langrangian SVM的使用~· - 家家的专栏 - CSDN博客





2011年09月26日 10:57:06[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：718








转自：[http://bbs.sciencenet.cn/forum.php?mod=viewthread&tid=523129](http://bbs.sciencenet.cn/forum.php?mod=viewthread&tid=523129)

 http://research.cs.wisc.edu/dmi/lsvm/


function [iter, optCond, time, w, gamma] = lsvm(A,D,nu,tol,maxIter,alpha, 

...

       perturb,normalize);

% LSVM Langrangian Support Vector Machine algorithm

%   LSVM solves a support vector machine problem using an iterative

%   algorithm inspired by an augmented Lagrangian formulation.

%

%   iters = lsvm(A,D)

%

%   where A is the data matrix, D is a diagonal matrix of 1s and -1s

%   indicating which class the points are in, and 'iters' is the number

%   of iterations the algorithm used.

%

%   All the following additional arguments are optional:

%

%   [iters, optCond, time, w, gamma] = ...

%     lsvm(A,D,nu,tol,maxIter,alpha,perturb,normalize)

%

%   optCond is the value of the optimality condition at termination.

%   time is the amount of time the algorithm took to terminate.

%   w is the vector of coefficients for the separating hyperplane.

%   gamma is the threshold scalar for the separating hyperplane.

%

%   On the right hand side, A and D are required. If the rest are not

%   specified, the following defaults will be used:

%     nu = 1/size(A,1), tol = 1e-5, maxIter = 100, alpha = 1.9/nu,

%       perturb = 0, normalize = 0

%

%   perturb indicates that all the data should be perturbed by a random

%   amount between 0 and the value given. perturb is recommended only

%   for highly degenerate cases such as the exclusive or.

%

%   normalize should be set to 1 if the data should be normalized before

%   training.

%

%   The value -1 can be used for any of the entries (except A and D) to

%   specify that default values should be used.

%

%   Copyright (C) 2000 Olvi L. Mangasarian and David R. Musicant.

%   Version 1.0 Beta 1

%   This software is free for academic and research use only.

%   For commercial use, contact [musicant@cs.wisc.edu](mailto:musicant@cs.wisc.edu).


  % If D is a vector, convert it to a diagonal matrix.

  [k,n] = size(D);

  if k==1 | n==1

    D=diag(D);

  end;


  % Check all components of D and verify that they are +-1

  checkall = diag(D)==1 | diag(D)==-1;

  if any(checkall==0)

    error('Error in D: classes must be all 1 or -1.');

  end;


  m = size(A,1);


  if ~exist('nu') | nu==-1

    nu = 1/m;

  end;

  if ~exist('tol') | tol==-1

    tol = 1e-5;

  end;

  if ~exist('maxIter') | maxIter==-1

    maxIter = 100;

  end;

  if ~exist('alpha') | alpha==-1

    alpha = 1.9/nu;

  end;

  if ~exist('normalize') | normalize==-1

    normalize = 0;

  end;

  if ~exist('perturb') | perturb==-1

    perturb = 0;

  end;


  % Do a sanity check on alpha

  if alpha > 2/nu,

    disp(sprintf('Alpha is larger than 2/nu. Algorithm may not converge.'));

  end;


  % Perturb if appropriate

  rand('seed',22);

  if perturb,

    A = A + rand(size(A))*perturb;

  end;


  % Normalize if appropriate

  if normalize,

    avg = mean(A);

    dev = std(A);

    if (isempty(find(dev==0)))

      A = (A - avg(ones(m,1), : ) )./dev(ones(m,1), : ) ;

    else

      warning('Could not normalize matrix: at least one column is constant.')

;

    end;

  end;


  % Create matrix H

  [m,n] = size(A);

  e = ones(m,1);

  H = D*[A -e];

  iter = 0;

  time = cputime;


  % "K" is an intermediate matrix used often in SMW calclulations

  K = H*inv((speye(n+1)/nu+H'*H));


  % Choose initial value for x

  x = nu*(1-K*(H'*e));


  % y is the old value for x, used to check for termination

  y = x + 1;


  while iter < maxIter & norm(y-x)>tol

    % Intermediate calculation which is used twice

    z = (1+pl(((x/nu+H*(H'*x))-alpha*x)-1));

    y = x;

    % Calculate new value of x

    x=nu*(z-K*(H'*z));

    iter = iter + 1;

  end;


  % Determine outputs

  time = cputime - time;

  optCond = norm(x-y);

  w = A'*D*x;

  gamma = -e'*D*x;

  disp(sprintf('Running time (CPU secs) = %g',time));

  disp(sprintf('Number of iterations = %d',iter));

  disp(sprintf('Training accuracy = %g',sum(D*(A*w-gamma)>0)/m));


  return;


function pl = pl(x);

  %PLUS function : max{x,0}

  pl = (x+abs(x))/2;

  return;


举例：A =


    41   250

    42   238

    26   196

    63   368

    45   350

    55   316

    57   400

    66   402

    64   424

    38   254

    40   350

    54   318

    55   348

    54   362

    59   428

    74   452

    63   468

    70   484

    56   366

    38   238

    45   328

    61   418

    91   576

    61   346

    66   462

    50   292

    63   430

    51   350

    24   170

    64   410

    64   410

    53   342

    43   270

    79   602

    44   332

    79   536

    58   392

    68   392

    18   104

    64   434

    59   390

    40   246

    40   228

    43   364

    89   602

    23   146

    47   308

    76   492

    52   348

    89   520

    76   596

    77   576

    81   624

    74   580

    70   482

    70   576

    76   616

    52   354

    42   272

    81   634

    56   446

   102   696

    86   584

    88   694

   109   826

    57   468

    61   614

    61   532

    61   430

   101   796

    96   850

    64   526

    94   798

    51   342

    61   538

    68   586

    84   710

   127   974

    93   700

    94   730

    90   680

    66   558

    66   598

    85   748

   102   732

    94   886

    83   730

    89   658

    79   556

    66   536

    83   740

    90   816

    97   756

    80   742

    76   676

   116   952

    86   768

    64   468

    89   668

    73   724


D =


     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

     1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

    -1

>> [iter, optCond, time, w, gamma] = lsvm(A,d,nu,tol,maxIter,alpha, perturb,normalize)


;

Running time (CPU secs) = 0

Number of iterations = 70

Training accuracy = 0.83

>> w


w =


    0.0993

   -0.0140


>> x=[1:140];

>> y=(0.0993.*x - gamma)./0.0140;

>> plot(X(1:50),Y(1:50),'ob')

>> hold on

>> plot(X(51:100),Y(51:100),'or')

>> plot(x,y,'k')




