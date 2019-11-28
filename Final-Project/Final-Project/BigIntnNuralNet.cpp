#include "BigIntNerualNet.h"
#include "header.h"
#include <random>
#include <time.h>
#include <iostream>

using namespace std;

#if USERAND == TRUE
void BigIntNuralNet::useRand()
{
	srand(time(NULL));
	weight = new BigInteger ***[2];
	weight[0] = new BigInteger ** [sizes[0]];

	for(int i = 0; i < sizes[0]; i++)
	{
		weight[0][i] = new BigInteger * [sizes[1]];
		for(int j = 0; j < sizes[1]; j++)
		{
			weight[0][i][j] = new BigInteger(rand() % 100 + 1);
		}
	}

	weight[1] = new BigInteger ** [sizes[1] + 1];

	for(int i = 0; i <= sizes[1]; i++)
	{
		weight[1][i] = new BigInteger * [sizes[2]];
		for(int j = 0; j < sizes[2]; j++)
		{
			weight[1][i][j] = new BigInteger(rand() % 100 + 1);
		}
	}
	
	return;
}
#else
void BigIntNuralNet::useStatic()
{
	weight = new BigInteger * **[2];
	weight[0] = new BigInteger * *[sizes[0]];

	for (int i = 0; i < sizes[0]; i++)
	{
		weight[0][i] = new BigInteger * [sizes[1]];
		for (int j = 0; j < sizes[1]; j++)
		{
			weight[0][i][j] = new BigInteger(j % 10 + 1);
		}
	}

	weight[1] = new BigInteger * *[sizes[1] + 1];

	for (int i = 0; i <= sizes[1]; i++)
	{
		weight[1][i] = new BigInteger * [sizes[2]];
		for (int j = 0; j < sizes[2]; j++)
		{
			weight[1][i][j] = new BigInteger(j % 10 + 1);
		}
	}
}
#endif

BigIntNuralNet::BigIntNuralNet()
{
	this->inputs = nullptr;
	this->hiddens = nullptr;
	this->weight = nullptr;
	this->outputs = nullptr;
	for (int i = 0; i < 3; i++)
		sizes[i] = -1;
}

void BigIntNuralNet::InitNN(BigInteger* input, int size[3], BigInteger* bias)
{
	this->inputs = new BigInteger[size[0]];
	
	for(int i = 0; i < 3; i++)
	{
		this->sizes[i] = size[i];
	}

	for(int i = 0; i < size[0]; i++)
	{
		this->inputs[i] = input[i];
	}
	
	this->hiddens = new BigInteger[size[1] + 1];
	if(bias == nullptr)
	{
		hiddens[0] = BigInteger(1);
	}else
	{
		hiddens[0] = *bias;
	}

	for(int i = 1; i <= size[1]; i++)
	{
		hiddens[i].set((uint32_t)0, HEXAGON | POSITIVE);
	}

#if USERAND == TRUE
	useRand();
#else
	useStatic();
#endif

	this->outputs = new BigInteger[size[2]];

	for(int i =0; i < size[2]; i++)
	{
		outputs[i].set((uint32_t)0, HEXAGON | POSITIVE);
	}
}

void BigIntNuralNet::FeedForward()
{
	for(int i = 1; i < sizes[1]; i++)
	{
		for(int j = 0; j < sizes[0]; j++)
		{
			BigInteger tmp = inputs[j];
			tmp.multi(*weight[0][j][i - 1]);
			hiddens[i].add(tmp);
		}
	}

	for(int i = 0; i < sizes[2]; i++)
	{
		for(int j = 0; j <= sizes[1]; j++)
		{
			BigInteger tmp = hiddens[j];
			tmp.multi(*weight[1][j][i]);
			outputs[i].add(tmp);
		}
	}
}

void BigIntNuralNet::ShowResult()
{
	cout << endl << endl;
	for(int i = 0; i < sizes[2]; i++)
	{
		printf("OutNode%d : %s\n", i + 1, outputs[i].get(HEXAGON));
	}
}

void BigIntNuralNet::FeedForwardTest()
{
	cout << endl << endl;

	cout << "Bias/a0(2) : " << hiddens[0].get(HEXAGON) << endl;
	
	for (int i = 1; i <= sizes[1]; i++)
	{
		for (int j = 0; j < sizes[0]; j++)
		{
			//BigInteger tmp = inputs[j] * (*weight[0][j][i]);
			BigInteger tmp = inputs[j];
			tmp.multi(*weight[0][j][i - 1]);
			hiddens[i].add(tmp);
		}
		printf("a%d(2) : %s\n", i, hiddens[i].get(HEXAGON));
	}

	cout << endl;
	
	for (int i = 0; i < sizes[2]; i++)
	{
		for (int j = 0; j <= sizes[1]; j++)
		{
			//BigInteger tmp = hiddens[j] * (*weight[1][j][i]);
			//outputs[i] = outputs[i] + tmp;
			BigInteger tmp = hiddens[j];
			tmp.multi(*weight[1][j][i]);
			outputs[i].add(tmp);
		}
		printf("a%d(3) : %s\n", i, outputs[i].get(HEXAGON));
	}
}

void BigIntNuralNet::InitNNTest()
{
	cout << "The number of node in Layer 1(Input Layer) : " << this->sizes[0] << endl;
	cout << "The number of node in Layer 2(Hidden Layer) : " << this->sizes[1] + 1 << endl;
	cout << "The number of node in Layer 3(Output Layer) : " << this->sizes[2] << endl << endl;

	cout << "Weight between Layer 1 and Layer 2" << endl;
	for(int i = 0; i < sizes[0]; i++)
	{
		for(int j = 0; j < sizes[1]; j++)
		{
			printf("w%d%d(1) : %s\n", i, j + 1, weight[0][i][j]->get(HEXAGON));
		}
	}
	cout << endl << endl;

	cout << "Weight between Layer 2 and Layer3" << endl;
	for(int i = 0; i <= sizes[1]; i++)
	{
		for(int j = 0; j < sizes[2]; j++)
		{
			printf("w%d%d(2) : %s\n", i, j, weight[1][i][j]->get(HEXAGON));
		}
	}
}

