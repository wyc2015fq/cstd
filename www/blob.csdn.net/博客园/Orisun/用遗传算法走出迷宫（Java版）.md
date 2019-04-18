# 用遗传算法走出迷宫（Java版） - Orisun - 博客园







# [用遗传算法走出迷宫（Java版）](https://www.cnblogs.com/zhangchaoyang/articles/2214263.html)





在百度文库上看到一篇文章[《遗传算法及神经网络在游戏开发中的应用》](http://wenku.baidu.com/view/c46d51c65fbfc77da269b180.html)，里面讲到了用遗传算法走迷宫的小游戏，我自己编程实现了一下，并用SWT把游戏的界面做出来了。

算法我就不多说了，上述文章里面讲得很清楚。直接看我做的小软件吧。

刚开始初始化一个迷宫，一般是走不通的，但你点击一个格子它就会变色，由路变为墙，或由墙变为路。

![](https://pic002.cnblogs.com/images/2011/103496/2011101618540460.png)

点击“Run"之后，遗传算法在后台运行，把出路给你找出来，然后在迷宫上把路径标出来。

![](https://pic002.cnblogs.com/images/2011/103496/2011101619011461.png)

点击"Evolution Chart"查看进入情况，即每一代中最优的个体的适应度是多少。

![](https://pic002.cnblogs.com/images/2011/103496/2011101619133583.png)

点击"Options"对遗传算法的参数进行重新设置。

![](https://pic002.cnblogs.com/images/2011/103496/2011101619135713.png)

换用更大规模的迷宫进行实验。

![](https://pic002.cnblogs.com/images/2011/103496/2011101619141869.png)

FAIL了，查看一下进化情况。

![](https://pic002.cnblogs.com/images/2011/103496/2011101619143771.png)

当然也有可能成功

![](https://pic002.cnblogs.com/images/2011/103496/2011101810241586.png)

对于遗传找出的结果如果存在原路返回的情况，我的代码可以检测出来，将这段路径剪去。但是如果出现绕圈子的情况检测不出来，比如下面的结果明显就是绕圈子了

![](https://pic002.cnblogs.com/images/2011/103496/2011101810245357.png)

下面给出核心算法GA类：


package GA;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Random;

/**
 * 用遗传算法走迷宫
 * 
 * @author Orisun
 * 
 */
public class GA {

	int gene_len; // 基因长度
	int chrom_len; // 染色体长度
	int population; // 种群大小
	double cross_ratio; // 交叉率
	double muta_ratio; // 变异率
	int iter_limit; // 最多进化的代数
	List<boolean[]> individuals; // 存储当代种群的染色体

	Labyrinth labyrinth;
	int width;		//迷宫一行有多少个格子
	int height;		//迷宫有多少行

	public class BI {
		double fitness;
		boolean[] indv;

		public BI(double f, boolean[] ind) {
			fitness = f;
			indv = ind;
		}

		public double getFitness() {
			return fitness;
		}

		public boolean[] getIndv() {
			return indv;
		}
	}

	List<BI> best_individual; // 存储每一代中最优秀的个体

	public GA(Labyrinth labyrinth) {
		this.labyrinth=labyrinth;	
		this.width = labyrinth.map[0].length;
		this.height = labyrinth.map.length;
		chrom_len = 4 * (width+height);
		gene_len = 2;
		population = 20;
		cross_ratio = 0.83;
		muta_ratio = 0.002;
		iter_limit = 300;
		individuals = new ArrayList<boolean[]>(population);
		best_individual = new ArrayList<BI>(iter_limit);
	}

	public int getWidth() {
		return width;
	}

	public void setWidth(int width) {
		this.width = width;
	}

	public double getCross_ratio() {
		return cross_ratio;
	}

	public List<BI> getBest_individual() {
		return best_individual;
	}

	public Labyrinth getLabyrinth() {
		return labyrinth;
	}
	public void setLabyrinth(Labyrinth labyrinth) {
		this.labyrinth = labyrinth;
	}
	public void setChrom_len(int chrom_len) {
		this.chrom_len = chrom_len;
	}

	public void setPopulation(int population) {
		this.population = population;
	}

	public void setCross_ratio(double cross_ratio) {
		this.cross_ratio = cross_ratio;
	}

	public void setMuta_ratio(double muta_ratio) {
		this.muta_ratio = muta_ratio;
	}

	public void setIter_limit(int iter_limit) {
		this.iter_limit = iter_limit;
	}

	// 初始化种群
	public void initPopulation() {
		Random r = new Random(System.currentTimeMillis());
		for (int i = 0; i < population; i++) {
			int len = gene_len * chrom_len;
			boolean[] ind = new boolean[len];
			for (int j = 0; j < len; j++)
				ind[j] = r.nextBoolean();
			individuals.add(ind);
		}
	}

	// 交叉
	public void cross(boolean[] arr1, boolean[] arr2) {
		Random r = new Random(System.currentTimeMillis());
		int length = arr1.length;
		int slice = 0;
		do {
			slice = r.nextInt(length);
		} while (slice == 0);
		if (slice < length / 2) {
			for (int i = 0; i < slice; i++) {
				boolean tmp = arr1[i];
				arr1[i] = arr2[i];
				arr2[i] = tmp;
			}
		} else {
			for (int i = slice; i < length; i++) {
				boolean tmp = arr1[i];
				arr1[i] = arr2[i];
				arr2[i] = tmp;
			}
		}
	}

	// 变异
	public void mutation(boolean[] individual) {
		int length = individual.length;
		Random r = new Random(System.currentTimeMillis());
		individual[r.nextInt(length)] ^= false;
	}

	// 轮盘法选择下一代,并返回当代最高的适应度值
	public double selection() {
		boolean[][] next_generation = new boolean[population][]; // 下一代
		int length = gene_len * chrom_len;
		for (int i = 0; i < population; i++)
			next_generation[i] = new boolean[length];
		double[] cumulation = new double[population];
		int best_index = 0;
		double max_fitness = getFitness(individuals.get(best_index));
		cumulation[0] = max_fitness;
		for (int i = 1; i < population; i++) {
			double fit = getFitness(individuals.get(i));
			cumulation[i] = cumulation[i - 1] + fit;
			// 寻找当代的最优个体
			if (fit > max_fitness) {
				best_index = i;
				max_fitness = fit;
			}
		}
		Random rand = new Random(System.currentTimeMillis());
		for (int i = 0; i < population; i++)
			next_generation[i] = individuals.get(findByHalf(cumulation,
					rand.nextDouble() * cumulation[population - 1]));
		// 把当代的最优个体及其适应度放到best_individual中
		BI bi = new BI(max_fitness, individuals.get(best_index));
		// printPath(individuals.get(best_index));
		//System.out.println(max_fitness);
		best_individual.add(bi);
		// 新一代作为当前代
		for (int i = 0; i < population; i++)
			individuals.set(i, next_generation[i]);
		return max_fitness;
	}

	// 折半查找
	public int findByHalf(double[] arr, double find) {
		if (find < 0 || find == 0 || find > arr[arr.length - 1])
			return -1;
		int min = 0;
		int max = arr.length - 1;
		int medium = min;
		do {
			if (medium == (min + max) / 2)
				break;
			medium = (min + max) / 2;
			if (arr[medium] < find)
				min = medium;
			else if (arr[medium] > find)
				max = medium;
			else
				return medium;

		} while (min < max);
		return max;
	}

	// 计算适应度
	public double getFitness(boolean[] individual) {
		int length = individual.length;
		// 记录当前的位置,入口点是（1,0）
		int x = 1;
		int y = 0;
		// 根据染色体中基因的指导向前走
		for (int i = 0; i < length; i++) {
			boolean b1 = individual[i];
			boolean b2 = individual[++i];
			// 00向左走
			if (b1 == false && b2 == false) {
				if (x > 0 && labyrinth.map[y][x - 1] == true) {
					x--;
				}
			}
			// 01向右走
			else if (b1 == false && b2 == true) {
				if (x + 1 < width && labyrinth.map[y][x + 1] == true) {
					x++;
				}
			}
			// 10向上走
			else if (b1 == true && b2 == false) {
				if (y > 0 && labyrinth.map[y - 1][x] == true) {
					y--;
				}
			}
			// 11向下走
			else if (b1 == true && b2 == true) {
				if (y + 1 < height && labyrinth.map[y + 1][x] == true) {
					y++;
				}
			}
		}
		int n = Math.abs(x - labyrinth.x_end) + Math.abs(y -labyrinth.y_end) + 1;
//		if(n==1)
//			printPath(individual);
		return 1.0 / n;
	}

	// 运行遗传算法
	public boolean run() {
		// 初始化种群
		initPopulation();
		Random rand = new Random(System.currentTimeMillis());
		boolean success = false;
		while (iter_limit-- > 0) {
			// 打乱种群的顺序
			Collections.shuffle(individuals);
			for (int i = 0; i < population - 1; i += 2) {
				// 交叉
				if (rand.nextDouble() < cross_ratio) {
					cross(individuals.get(i), individuals.get(i + 1));
				}
				// 变异
				if (rand.nextDouble() < muta_ratio) {
					mutation(individuals.get(i));
				}
			}
			// 种群更替
			if (selection() == 1) {
				success = true;
				break;
			}
		}
		return success;
	}

//	public static void main(String[] args) {
//		GA ga = new GA(8, 8);
//		if (!ga.run()) {
//			System.out.println("没有找到走出迷宫的路径.");
//		} else {
//			int gen = ga.best_individual.size();
//			boolean[] individual = ga.best_individual.get(gen - 1).indv;
//			System.out.println(ga.getPath(individual));
//		}
//	}

	// 根据染色体打印走法
	public String getPath(boolean[] individual) {
		int length = individual.length;
		int x = 1;
		int y = 0;
		LinkedList<String> stack=new LinkedList<String>();
		for (int i = 0; i < length; i++) {
			boolean b1 = individual[i];
			boolean b2 = individual[++i];
			if (b1 == false && b2 == false) {
				if (x > 0 && labyrinth.map[y][x - 1] == true) {
					x--;
					if(!stack.isEmpty() && stack.peek()=="右")
						stack.poll();
					else
						stack.push("左");
				}
			} else if (b1 == false && b2 == true) {
				if (x + 1 < width && labyrinth.map[y][x + 1] == true) {
					x++;
					if(!stack.isEmpty() && stack.peek()=="左")
						stack.poll();
					else
						stack.push("右");
				}
			} else if (b1 == true && b2 == false) {
				if (y > 0 && labyrinth.map[y - 1][x] == true) {
					y--;
					if(!stack.isEmpty() && stack.peek()=="下")
						stack.poll();
					else
						stack.push("上");
				}
			} else if (b1 == true && b2 == true) {
				if (y + 1 < height && labyrinth.map[y + 1][x] == true) {
					y++;
					if(!stack.isEmpty() && stack.peek()=="上")
						stack.poll();
					else
						stack.push("下");
				}
			}
		}
		StringBuilder sb=new StringBuilder(length/4);
		Iterator<String> iter=stack.descendingIterator();
		while(iter.hasNext())
			sb.append(iter.next());
		return sb.toString();
	}
}
















