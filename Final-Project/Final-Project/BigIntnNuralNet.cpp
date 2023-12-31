#include "BigIntNerualNet.h"
#include "header.h"
#include <random>
#include <time.h>
#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

void BigIntNuralNet::useRand()
{
	srand(time(NULL));
	weight = new BigInteger **[this->sizes[0][1] + 1];

	for(int i = 0; i <= this->sizes[0][1]; i++)
	{
		int tmp = (i == 0 ? sizes[0][0] : sizes[1][i - 1]);
		weight[i] = new BigInteger *[tmp];

		for(int j = 0; j < tmp; j++)
		{
			int tmp2 = (i == this->sizes[0][1] ? sizes[0][2] : sizes[1][i]);
			weight[i][j] = new BigInteger[tmp2];
			for(int k = 0; k < tmp2; k++)
			{
				weight[i][j][k].set(rand() % 100 + 1, HEXAGON | POSITIVE);
			}
		}
	}
	
	return;
}
void BigIntNuralNet::useStatic()
{
	weight = new BigInteger * *[this->sizes[0][1] + 1];

	for (int i = 0; i <= this->sizes[0][1]; i++)
	{
		int tmp = (i == 0 ? sizes[0][0] : sizes[1][i - 1]);
		weight[i] = new BigInteger * [tmp];

		for (int j = 0; j < tmp; j++)
		{
			int tmp2 = (i == this->sizes[0][1] ? sizes[0][2] : sizes[1][i]);
			weight[i][j] = new BigInteger[tmp2];
			for (int k = 0; k < tmp2; k++)
			{
				weight[i][j][k].set(j % 100 + 1, HEXAGON | POSITIVE);
			}
		}
	}
}

BigIntNuralNet::BigIntNuralNet()
{
	this->inputs = nullptr;
	this->hiddens = nullptr;
	this->weight = nullptr;
	this->outputs = nullptr;
	this->sizes = nullptr;
	this->src = "";
}

BigIntNuralNet::BigIntNuralNet(std::string path)
{
	this->inputs = nullptr;
	this->hiddens = nullptr;
	this->weight = nullptr;
	this->outputs = nullptr;
	this->sizes = nullptr;
	this->src = path;
}

void BigIntNuralNet::InitNN()
{
	if(this->src == "")
	{
		return; // error
	}

	string str;
	this->sizes = new int* [2];
	this->sizes[0] = new int[3];
	int tmp, mode;
	ifstream file(this->src.c_str());

	file >> mode;

	file >> tmp; // layer ��
	this->sizes[0][1] = tmp - 2;
	this->sizes[1] = new int[sizes[0][1]];
	this->hiddens = new BigInteger * [sizes[0][1]];
	file >> this->sizes[0][0];
	for (int i = 0; i < sizes[0][1]; i++)
	{
		file >> sizes[1][i];
		this->hiddens[i] = new BigInteger[sizes[1][i]];
		for (int j = 0; j < sizes[1][i]; j++)
		{
			this->hiddens[i][j].set((uint32_t)0, HEXAGON | POSITIVE);
		}
	}
	file >> sizes[0][2]; // output node ��

	if(mode == 0)
	{
		this->useRand();
	}else if(mode == 1)
	{
		weight = new BigInteger * *[this->sizes[0][1] + 1];
		
		
		for (int i = 0; i <= this->sizes[0][1]; i++)
		{
			//cout << "Weight between Layer " << i + 1 << " and Layer " << i + 2 << endl;
			int tmp = (i == 0 ? sizes[0][0] : sizes[1][i - 1]);
			weight[i] = new BigInteger * [tmp];
			int tmp2;
			file >> tmp2;
		
			for (int j = 0; j < tmp; j++)
			{
				int tmp3 = (i == this->sizes[0][1] ? sizes[0][2] : sizes[1][i]);
				weight[i][j] = new BigInteger[tmp3];
				for (int k = 0; k < tmp3; k++)
				{
					weight[i][j][k].set(tmp2, HEXAGON | POSITIVE);
					//printf("w%d,%d(%d) : %s\n", j, k, i + 1, weight[i][j][k].get(HEXAGON).get());
				}
			}
		}
	}else
	{
		//error
	}

	this->inputs = new BigInteger[this->sizes[0][0]];

	for (int i = 0; i < this->sizes[0][0]; i++)
	{
		file >> tmp;
		this->inputs[i].set((uint32_t)tmp, POSITIVE | HEXAGON);
	}

	this->outputs = new BigInteger[sizes[0][2]];

	for (int i = 0; i < sizes[0][2]; i++)
	{
		outputs[i].set((uint32_t)0, HEXAGON | POSITIVE);
	}
}

void BigIntNuralNet::InitNN(BigInteger* input, int **size)
{// size[0][0] -> input, size[0][1]->layers    size[0][2] -> output, size[1][n] ->hidden's node
	this->inputs = new BigInteger[size[0][0]];

	this->sizes = new int*[2];
	this->sizes[0] = new int[3];
	this->sizes[0][0] = size[0][0];
	this->sizes[0][1] = size[0][1];
	this->sizes[0][2] = size[0][2];
	this->sizes[1] = new int[size[0][1]];
	
	for(int i = 0; i < size[0][1]; i++)
	{
		this->sizes[1][i] = size[1][i];
	}

	for(int i = 0; i < size[0][0]; i++)
	{
		this->inputs[i] = input[i];
	}
	
	this->hiddens = new BigInteger*[size[0][1]];
	for(int i = 0; i < size[0][1]; i++)
	{
		this->hiddens[i] = new BigInteger[size[1][i]];
		for(int j = 0; j < size[1][i]; j++)
		{
			this->hiddens[i][j].set((uint32_t)0, HEXAGON | POSITIVE);
		}
	}

	useRand();

	this->outputs = new BigInteger[size[0][2]];

	for(int i = 0; i < size[0][2]; i++)
	{
		outputs[i].set((uint32_t)0, HEXAGON | POSITIVE);
	}
}

void BigIntNuralNet::FeedForward()
{
	for(int layer = 0; layer <= sizes[0][1]; layer++)
	{
		BigInteger* ptr;
		BigInteger* ptr2;
		int size[2];
		
		if(layer == 0)
		{
			ptr = this->inputs;
			ptr2 = this->hiddens[0];
			size[0] = sizes[0][0];
			size[1] = sizes[1][0];
		}else if(layer == sizes[0][1])
		{
			ptr = this->hiddens[sizes[0][1] -1];
			ptr2 = this->outputs;
			size[0] = sizes[1][layer - 1];
			size[1] = sizes[0][2];
		}else
		{
			ptr = this->hiddens[layer - 1];
			ptr2 = this->hiddens[layer];
			size[0] = sizes[1][layer - 1];
			size[1] = sizes[1][layer ];
		}

		for(int i =0; i < size[1]; i++)
		{
			for(int j =0; j < size[0]; j++)
			{
				BigInteger tmp = ptr[j];
				tmp.multi(weight[layer][j][i]);
				ptr2[i].add(tmp);
			}
			if(i == 0)
			printf("a%d(%d) : %s\n", i, layer + 1, ptr2[i].get(DECIMAL).get());
		}
	}
}

void BigIntNuralNet::ShowResult()
{
	cout << endl << endl;
	for(int i = 0; i < sizes[0][2]; i++)
	{
		printf("OutNode%d : %s\n", i + 1, outputs[i].get(DECIMAL).get());
	}
}

void BigIntNuralNet::FeedForwardTest()
{
	cout << endl << endl;

	for (int layer = 0; layer <= sizes[0][1]; layer++)
	{
		BigInteger* ptr;
		BigInteger* ptr2;
		int size[2];

		if (layer == 0)
		{
			ptr = this->inputs;
			ptr2 = this->hiddens[0];
			size[0] = sizes[0][0];
			size[1] = sizes[1][0];
		}
		else if (layer == sizes[0][1])
		{
			ptr = this->hiddens[sizes[0][1] - 1];
			ptr2 = this->outputs;
			size[0] = sizes[1][layer - 1];
			size[1] = sizes[0][2];
		}
		else
		{
			ptr = this->hiddens[layer - 1];
			ptr2 = this->hiddens[layer];
			size[0] = sizes[1][layer - 1];
			size[1] = sizes[1][layer];
		}

		for (int i = 0; i < size[1]; i++)
		{
			for (int j = 0; j < size[0]; j++)
			{
				BigInteger tmp = ptr[j];
				tmp.multi(weight[layer][j][i]);
				ptr2[i].add(tmp);
			}
			printf("a%d(%d) : %s\n", i, layer + 1, ptr2[i].get(DECIMAL).get());
		}
	}
}

void BigIntNuralNet::InitNNTest()
{
	for (int layer = sizes[0][1] + 1; layer >= 0; layer--)
	{
		BigInteger* ptr;
		BigInteger* ptr2;
		int size[2];

		if (layer == 0)
		{
			ptr = this->inputs;
			ptr2 = this->hiddens[0];
			size[0] = sizes[0][0];
			size[1] = sizes[1][0];
			
			cout << "Input : " << this->sizes[0][0] << endl;
		}
		else if (layer == sizes[0][1] + 1)
		{
			ptr = this->hiddens[sizes[0][1] - 1];
			ptr2 = this->outputs;
			size[0] = sizes[1][layer - 1];
			size[1] = sizes[0][2];

			cout << "Output : " << this->sizes[0][2] << endl;
		}
		else
		{
			ptr = this->hiddens[layer - 1];
			ptr2 = this->hiddens[layer];
			size[0] = sizes[1][layer - 1];
			size[1] = sizes[1][layer];

			printf("H%d : %d\n", layer, this->sizes[1][layer - 1]);
		}

		if (layer != 0)
			printf("  ==> w : %s\n", weight[layer - 1][0][0].get(DECIMAL).get());
		else printf("\n\n");
	}
}
