#pragma once
#include "header.h"

#define TRUE 1
#define FALSE 0

#define USERAND FALSE

class BigIntNuralNet
{
private:
	BigInteger* inputs;
	BigInteger* hiddens;
	BigInteger* outputs;
	int sizes[3];
	BigInteger**** weight;
#if USERAND == TRUE
	void useRand();
#else
	void useStatic();
#endif
public:
	BigIntNuralNet();
	void InitNN(BigInteger*, int[3], BigInteger* bias = nullptr);
	void FeedForward();
	void ShowResult();

	void FeedForwardTest();
	void InitNNTest();
};