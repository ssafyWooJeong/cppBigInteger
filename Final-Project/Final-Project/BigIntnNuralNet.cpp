#include "BigIntNerualNet.h"
#include "header.h"
#include <random>
#include <time.h>
#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

#if USERAND == TRUE
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
#else
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
#endif

BigIntNuralNet::BigIntNuralNet()
{
	this->inputs = nullptr;
	this->hiddens = nullptr;
	this->weight = nullptr;
	this->outputs = nullptr;
	this->sizes = nullptr;
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

#if USERAND == TRUE
	useRand();
#else
	useStatic();
#endif

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
		}
	}
}

void BigIntNuralNet::ShowResult()
{
	cout << endl << endl;
	for(int i = 0; i < sizes[0][2]; i++)
	{
		printf("OutNode%d : %s\n", i + 1, outputs[i].get(HEXAGON).get());
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
			printf("a%d(%d) : %s\n", i, layer + 1, ptr2[i].get(HEXAGON).get());
		}
	}
}

void BigIntNuralNet::InitNNTest()
{
	cout << "The number of node in Layer 1(Input Layer) : " << this->sizes[0][0] << endl;
	for(int i = 0; i < sizes[0][1]; i++)
	{
		cout << "The number of node in Layer "<< i+2 <<"(Hidden Layer) : " << this->sizes[1][i] << endl;
	}

	cout << "The number of node in Layer "<< sizes[0][1] + 2 <<"(Output Layer) : " << this->sizes[0][2] << endl << endl;

	

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
		cout << "Weight between Layer "<< layer+1 << " and Layer " << layer + 2 << endl;
		for (int i = 0; i < size[0]; i++)
		{
			for (int j = 0; j < size[1]; j++)
			{
				printf("w%d,%d(%d) : %s\n", i, j, layer + 1, weight[layer][i][j].get(HEXAGON).get());
			}
		}
	}
}

void BigIntNuralNet::TestFFAlgorithm(std::string path)
{ // size[0][0] -> input, size[0][1]->hidden layers,  size[0][2] -> output, size[1][n] ->hidden's node
	string str;
	this->sizes = new int* [2];
	this->sizes[0] = new int[3];
	int tmp;
	ifstream file(path.c_str());

	
	file >> tmp;
	this->sizes[0][1] = tmp - 1;
	this->sizes[1] = new int[sizes[0][1]];
	this->hiddens = new BigInteger * [sizes[0][1]];
	for(int i = 0; i < sizes[0][1]; i++)
	{
		file >> sizes[1][i];
		this->hiddens[i] = new BigInteger[sizes[1][i]];
		for (int j = 0; j < sizes[1][i]; j++)
		{
			this->hiddens[i][j].set((uint32_t)0, HEXAGON | POSITIVE);
		}
	}
	file >> sizes[0][2];

	queue<BigInteger> tmpInput;
	queue<int> tmpWeight;

	file.ignore(1, '\n');
	getline(file, str);
	while (str.find(" ") != string::npos)
	{
		string substr = str.substr(0, str.find(" "));
		tmpWeight.push(atoi(substr.c_str()));
		str = str.substr(str.find(" ") + 1);
	}
	tmpWeight.push(atoi(str.c_str()));
	
	getline(file, str);
	while (str.find(" ") != string::npos)
	{
		string substr = str.substr(0, str.find(" "));
		BigInteger obj(const_cast<char*>(substr.c_str()));
		tmpInput.push(obj);
		str = str.substr(str.find(" ") + 1);
	}
	tmpInput.push(BigInteger(const_cast<char*>(str.c_str())));

	this->inputs = new BigInteger[tmpInput.size()];
	this->sizes[0][0] = tmpInput.size();

	for (int i = 0; i < this->sizes[0][0]; i++)
	{
		inputs[i] = tmpInput.front();
		tmpInput.pop();
	}
	
	weight = new BigInteger * *[this->sizes[0][1] + 1];

	
	for (int i = 0; i <= this->sizes[0][1]; i++)
	{
		//cout << "Weight between Layer " << i + 1 << " and Layer " << i + 2 << endl;
		int tmp = (i == 0 ? sizes[0][0] : sizes[1][i - 1]);
		weight[i] = new BigInteger * [tmp];
		int tmp2 = tmpWeight.front();
		tmpWeight.pop();

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
	
	this->outputs = new BigInteger[sizes[0][2]];

	for (int i = 0; i < sizes[0][2]; i++)
	{
		outputs[i].set((uint32_t)0, HEXAGON | POSITIVE);
	}
}