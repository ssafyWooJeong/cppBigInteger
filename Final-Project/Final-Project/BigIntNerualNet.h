#pragma once
#include "header.h"
#include <string>

#define TRUE 1
#define FALSE 0

#define USERAND FALSE

class BigIntNuralNet
{
private:
	BigInteger* inputs;
	BigInteger** hiddens;
	BigInteger* outputs;
	int **sizes; // size[0][0] -> input, size[0][1]->hidden layers,  size[0][2] -> output, size[1][n] ->hidden's node
	BigInteger*** weight;
#if USERAND == TRUE
	void useRand();
#else
	void useStatic();
#endif
public:
	BigIntNuralNet();
	void InitNN(BigInteger*, int**);
	void FeedForward();
	void ShowResult();

	void FeedForwardTest();
	void InitNNTest();

	void TestFFAlgorithm(std::string);
};