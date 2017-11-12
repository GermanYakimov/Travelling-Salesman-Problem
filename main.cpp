#include<iostream>
#include<cstdlib>
#include<ctime>
#include<algorithm>
using namespace std;
#define cities_number 500
int** matrix;

struct solution{
	int* genotype;	//array
	int fitness;
};

struct population{
	solution* solutions;
	int population_size;	
};

population* sort_solutions(population* x);

int fitness(int* genotype);
solution mutation(solution one);

int** graph_generation(int max_price)
{
//	cout<<endl<<"/graph_generation/"<<endl;
	int** matrix=(int**)malloc(cities_number*sizeof(int*));
	int i, j;
	for(i=0;i<cities_number;i++)
		matrix[i]=(int*)malloc(cities_number*sizeof(int));

	for(i=0;i<cities_number;i++)
		for(j=0;j<cities_number;j++)
		{
			if(i==j)
			{
				matrix[i][j]==0;
				continue;
			}
			matrix[i][j]=(rand() % max_price) + 1;
		}


	for(i=0;i<cities_number;i++)
		for(j=0;j<cities_number;j++)
			matrix[i][j]=matrix[j][i];
//	cout<<endl<<"/end_graph_generation/"<<endl;
	return matrix;	
}

solution crossing(solution parent1, solution parent2)
{
	solution child;
	child.genotype = (int*)malloc((cities_number+1)*sizeof(int));

	int i, j;
	int tmp=cities_number/2;

	bool flag=false;

	for(i=0;i<=cities_number;i++)
		child.genotype[i]=-1;

	for(i=0;i<cities_number/2;i++)
		child.genotype[i]=parent1.genotype[i];

	for(i=0;i<cities_number;i++)
	{
		for(j=0;j<cities_number;j++)
		{
			if(parent2.genotype[i]==child.genotype[j])
			{
				flag=true;
				break;
			}
		}
		if(flag==false)
		{
			child.genotype[tmp]=parent2.genotype[i];
			tmp++;
		}
	
		flag=false;
	}

	child.genotype[cities_number]=child.genotype[0];
	if(rand()%10==0)
		child=mutation(child);

	child.fitness=fitness(child.genotype);

	return child;
}

void delete_matrix()
{
	int i;
	for(i=0;i<cities_number;i++)
		free(matrix[i]);
	free(matrix);
}

int** start(int*  max_price)
{
//	cout<<endl<<"/start/"<<endl;
        cout<<"max price - ";   cin>>*(max_price);

        int** matrix=graph_generation(*(max_price));
//	cout<<endl<<"/end_start/"<<endl;

	return matrix;
}



int fitness(int* genotype)
{
//	cout<<endl<<"/fitness/"<<endl;
	int price=0, i, j;
	
	for(i=0;i<cities_number;i++)
		price+=matrix[genotype[i]][genotype[i+1]];

//	cout<<endl<<"/end_fitness/"<<endl;
	return price;
}

solution generate_solution()
{
//	cout<<endl<<"/generate_solution/"<<endl;
	solution new_solution;
        new_solution.genotype = (int*)malloc((cities_number+1) * sizeof(int));
        int i;
        for (i = 0; i < cities_number; i++)
                new_solution.genotype[i] = i;

        for (i = 0; i < cities_number; i++)
                swap(new_solution.genotype[i], new_solution.genotype[rand() % cities_number]);

	new_solution.genotype[cities_number]=new_solution.genotype[0];
	new_solution.fitness=fitness(new_solution.genotype); 

//	cout<<endl<<"/end_generate_solution/"<<endl;
	return new_solution;
}

solution mutation(solution one)
{
	swap(one.genotype[rand()%cities_number], one.genotype[rand()%cities_number]);
	one.fitness=fitness(one.genotype);
	one.genotype[cities_number]=one.genotype[0];

	return one;
}

void print_solution(solution* one)
{
//	cout<<endl<<"/print_solution/"<<endl;
	int i;
	for(i=0;i<=cities_number;i++)
		cout<<one->genotype[i]<<" ";
	cout<<" - "<<one->fitness<<endl;

//	cout<<endl<<"/end_print_solution/"<<endl;
}

void print_matrix()
{
//	cout<<endl<<"/print_matrix/"<<endl;
	int i, j;
	for(i=0;i<cities_number;i++)
	{
		for(j=0;j<cities_number;j++)
			cout<<matrix[i][j]<<" ";
		cout<<endl;
	}
	cout<<endl;
//	cout<<endl<<"/end_print_matrix/"<<endl;
}

void delete_solution(solution* one)
{
//	cout<<endl<<"/delete_solution/"<<endl;
	free(one->genotype);
	free(one);
//	cout<<endl<<"/end_delete_solution/"<<endl;
}

population* create_population(int population_size)
{
//	cout<<endl<<"/create_population/"<<endl;
	population* one=(population*)malloc(sizeof(population));
	one->solutions=(solution*)malloc(population_size*sizeof(population));
	
	int i;
	for(i=0;i<population_size;i++)
		one->solutions[i]=generate_solution();
	one->population_size=population_size;
	one=sort_solutions(one);
	
//	cout<<endl<<"/end_create_population/"<<endl;
	return one;
}

void delete_population(population* one)
{
//	cout<<endl<<"/delete_population/"<<endl;
	int i;
	for(i=0;i<one->population_size;i++)
		free(one->solutions[i].genotype);
	free(one->solutions);
	free(one);
//	cout<<endl<<"/end_delete_population/"<<endl;
}

void print_population(population* one)
{
//	cout<<endl<<"/print_population/"<<endl;
	int i;
	for(i=0;i<one->population_size;i++)
		print_solution(&(one->solutions[i]));
//	cout<<endl<<"/end_print_population/"<<endl;
}

population* new_generation(population* x)
{
//	cout<<"/new_generation/"<<endl;

	int index=x->population_size - x->population_size/3;
	int i;
	for(i=index;i<x->population_size;i++)
		free(x->solutions[i].genotype);

	for(i=index;i<x->population_size;i++)
		x->solutions[i]=crossing(x->solutions[rand()%index], x->solutions[rand()%index]);

	if(x->solutions[0].fitness==x->solutions[x->population_size/2].fitness)
	{
		for(i=x->population_size/3;i<x->population_size;i++)
		{
			free(x->solutions[i].genotype);
			x->solutions[i]=generate_solution();
		}
	}

	x=sort_solutions(x);		

//	cout<<endl<<"/end_new_generation/"<<endl;

	return x;
}

population* sort_solutions(population* x)
{
	int i, j, swaps_counter;
	solution tmp;

	while (1)
	{
		swaps_counter = 0;
		for (j = 0; j < x->population_size - 1; j++)
			if (x->solutions[j].fitness > x->solutions[j + 1].fitness)
			{
				tmp = x->solutions[j];
				x->solutions[j] = x->solutions[j + 1];
				x->solutions[j + 1] = tmp;
				swaps_counter++;
			}
		if (swaps_counter == 0)
			break;
	}

	return x;
}

int main()
{
	srand(time(NULL));
	int max_price, i, j, population_size, k=1;
	matrix = start(&(max_price));
	print_matrix();
	cout<<endl<<"population size - ";
	cin>>population_size;

	population* A=create_population(population_size);
//	print_population(A);

	while(1)
	{
		cout<<k<<": "<<A->solutions[0].fitness<<" "<<A->solutions[A->population_size/2].fitness<<" "<<A->solutions[A->population_size-1].fitness<<endl;
		k++;
		A=new_generation(A);
		if(k==100000000)
			break;
	}
	
	delete_population(A);
	delete_matrix();
	return 0;
}
