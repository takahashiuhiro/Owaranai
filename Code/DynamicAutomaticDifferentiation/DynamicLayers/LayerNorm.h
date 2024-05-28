#pragma once
#include "BaseDynamicLayer.h"

class LayerNorm : public BaseDynamicLayer
{
public:
	virtual void Init(he InputParams);
	virtual std::vector<DynamicTensor> Forward(std::vector<DynamicTensor>InputForwardList, he InputParams = he());

	std::vector<int>NormalizedShape;
	float eps;
	bool ElementwiseAffine;
	bool Bias;
};