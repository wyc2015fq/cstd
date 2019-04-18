# Snake 算法 - 深之JohnChen的专栏 - CSDN博客

2006年04月18日 10:26:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：9782


Snake 算法或Active Contour Models。这类算法需要给出初始的轮廓，然后进行迭代，使轮廓沿能量降低的方向靠近，最后得到一个优化的边界。能量函数包括内外力两方面，如边界曲率和梯度。由于用户无法估计迭代的最后结果，应用Snake 算法往往需要进行多次的交互工作。特别当目标比较复杂时，或与其它物体靠得较近时，初始的轮廓不易确定，而迭代的结果往往不能达到要求。

/////////////////////////////////CODE://///////////////////////////////////////////////

bool termination_point
	bool neighbor3by3; /* true means 3x3 neighborhood, false means 5x5 neighborhood */
	int grad_mag[500][500];	/* Magnitude of Gradient */
	int m_Cols; /* Number of Columns */
	int m_Rows; /* Number of Rows */
	int no_of_snake_points;
	CPoint Snake_points[200]; /* Snake Points */
	double *alpha,*beta, *gamma; /* Weights for energies */
	double threshold_curvature; 
	int threshold_movement;

void Snake_algorithm() 
{
	bool flag_change;
	int movement,i,j,iteration=0;
	double avg_distance=0.0, max_curvature;
	CPoint temp;
	double *curvature;	
	alpha=new double[no_of_snake_points];
	beta=new double[no_of_snake_points];
	gamma=new double[no_of_snake_points];
	curvature=new double[no_of_snake_points];
	termination_point=false;

	for(i=0;i<no_of_snake_points;i++)
	{
		*(alpha+i)=1.0;
		*(beta+i)=1.0;
		*(gamma+i)=1.2;
		avg_distance+=find_distance(i, Snake_points[i]);
	}
	j=no_of_snake_points;
	pdc=GetDC();
	while(!termination_point)
	{
		movement=0;
		avg_distance=avg_distance/(double)no_of_snake_points;
		max_curvature=-1000000000.0;
		for(i=0;i<no_of_snake_points;i++)
		{
			temp=find_min_energy(i, Snake_points[i],avg_distance);
			flag_change=false;
			if(temp!=Snake_points[i]&&temp!=Snake_points[(i-1+j)%j]&&temp!=Snake_points[(i+1)%j])
			{
				Snake_points[i]=temp;
				movement++;
			}
			curvature[i]=find_curvature(i, Snake_points[i]);
			if(max_curvature<curvature[i])
				max_curvature=curvature[i];
		}
		avg_distance=0.0;
		for(i=0;i<no_of_snake_points;i++)
			curvature[i]=curvature[i]/max_curvature;
		for(i=0;i<no_of_snake_points;i++)
		{
			avg_distance+=find_distance(i, Snake_points[i]);
			if(curvature[i]>threshold_curvature&&curvature[i]>curvature[(i+1)%no_of_snake_points]&&curvature[i]>curvature[(i-1+no_of_snake_points)%no_of_snake_points])
				*(beta+i)=0;
		}
		if(movement<threshold_movement)
			termination_point=true;
		iteration++;
		if(iteration>Max_Iterations)
			termination_point=true;
	}
	delete alpha;
	delete beta;
	delete gamma;
	delete curvature;
}

double find_distance(int no, CPoint point)
{
	int x=no_of_snake_points;
	point-=Snake_points[(no-1+x)%x];
	return(sqrt(point.x*point.x+point.y*point.y));
}

double find_curvature(int no, CPoint point)
{
	int x=no_of_snake_points;
	point=Snake_points[(no-1+x)%x]-point-point+Snake_points[(no+1)%x];
	return(point.x*point.x+point.y*point.y);
}

double find_continuity(int no, CPoint point, double avg_distance)
{
	return(pow(avg_distance-find_distance(no,point),2));
}

CPoint find_min_energy(int no, CPoint point, double avg_distance)
{
	CPoint p, min_point;
	double max_curvature, max_continuity, max_internal, min_internal, min_energy, energy;
	double curvatures[5][5];
	double continuities[5][5];
	double internal_energies[5][5];
	int i,j, limit=1;
	max_curvature=max_continuity=max_internal=-1000000000000.0;
	min_internal=1000000000000.0;
	if(!neighbor3by3)
		limit++;
	for(i=-limit;i<=limit;i++)
	{
		p.y=point.y+i;
		if(p.y<0)
			p.y=0;
		if(p.y>=m_Rows)
			p.y=m_Rows-1;
		for(j=-limit;j<=limit;j++)
		{
			p.x=point.x+j;
			if(p.x<0)
				p.x=0;
			if(p.x>=m_Cols)
				p.x=m_Cols-1;
			curvatures[i+limit][j+limit]=find_curvature(no, p); //This code can cause problem near
			continuities[i+limit][j+limit]=find_continuity(no,p,avg_distance);	//border of image
			internal_energies[i+limit][j+limit]=(double)grad_mag[p.y][p.x];
			if(curvatures[i+limit][j+limit]>max_curvature)
				max_curvature=curvatures[i+limit][j+limit];
			if(continuities[i+limit][j+limit]>max_continuity)
				max_continuity=continuities[i+limit][j+limit];
			if(internal_energies[i+limit][j+limit]>max_internal)
				max_internal=internal_energies[i+limit][j+limit];
			if(internal_energies[i+limit][j+limit]<min_internal)
				min_internal=internal_energies[i+limit][j+limit];
		}
	}
	for(i=0;i<=2*limit;i++)
	{
		for(j=0;j<=2*limit;j++)
		{
			curvatures[i][j]=curvatures[i][j]/max_curvature;
			continuities[i][j]=continuities[i][j]/max_continuity;
			internal_energies[i][j]=(internal_energies[i][j]-min_internal)/(max_internal-min_internal);
		}
	}
	min_point.x=-limit;
	min_point.y=-limit;
	min_energy=1000000000000.0;
	for(i=-limit;i<=limit;i++)
	{
		for(j=-limit;j<=limit;j++)
		{
			energy=*(alpha+no)*continuities[i+limit][j+limit]+*(beta+no)*curvatures[i+limit][j+limit]-*(gamma+no)*internal_energies[i+limit][j+limit];
			if(energy<min_energy||(energy==min_energy&&i==0&&j==0))
			{
				min_energy=energy;
				min_point.x=j;
				min_point.y=i;
			}
		}
	}
	min_point.x=min_point.x+point.x;
	min_point.y=min_point.y+point.y;
	return(min_point);
}

/////////////////////////////////////////////////////////////////////////////////////////////

参考资料：[1] Williams. Donna and Shah. Mubarak. "A Fast Algorithm for Active Contours and Curvature Estimation", CVGIP: Image Understanding. Vol. 55, No. 1, January 1992. pp. 14-26. 
[2] M. Kass, A. Witkin, D. Terzopoulos. Snake: Active Contour Models. International Journal of Computer Vision. Vol.1, No.4, pp.321-331, 1988. 

