#pragma once
#include "header.h"
#include <string>

#define TRUE 1
#define FALSE 0


class BigIntNuralNet
{
private:
	BigInteger* inputs;
	BigInteger** hiddens;
	BigInteger* outputs;
	int **sizes; // size[0][0] -> input, size[0][1]->hidden layers,  size[0][2] -> output, size[1][n] ->hidden's node
	BigInteger*** weight;
	std::string src;

	void useRand();
	void useStatic();
public:
	BigIntNuralNet();
	BigIntNuralNet(std::string);
	void InitNN();
	void InitNN(BigInteger*, int**);
	void FeedForward();
	void ShowResult();

	void FeedForwardTest();
	void InitNNTest();
};