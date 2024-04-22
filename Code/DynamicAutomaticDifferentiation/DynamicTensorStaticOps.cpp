#include "DynamicTensor.h"

void DynamicTensor::OpsSetInMap()
{
	BackwardOps[OpsType::Add] = DynamicStdOps_Backward_Add;
	BackwardOps[OpsType::MatMul] = DynamicStdOps_Backward_Matmul;
}


DynamicTensor DynamicTensor::DynamicStdOps_Forward_Add(std::vector<DynamicTensor>InputList, he InputParams, bool RequiresGrad)
{
	auto ResTensorContent = InputList[0].Ops->TensorPointer->Copy();
	for (size_t a = 1; a < InputList.size(); a++)
	{
		Tensor* TMPTensor = ResTensorContent->Add(InputList[a].Ops->TensorPointer.get());
		delete ResTensorContent;
		ResTensorContent = TMPTensor;
	}
	return SetComputationalHistory(ResTensorContent, InputList, InputParams, OpsType::Add, RequiresGrad);
}
void DynamicTensor::DynamicStdOps_Backward_Add(std::map<DynamicOps*, std::map<DynamicOps*, std::shared_ptr<DynamicOps>>>& BackwardOpsMap, std::shared_ptr<DynamicOps>CurOps)
{
	for (size_t a = 0; a < CurOps->InputOpsList.size(); a++)
	{
		if (!CurOps->InputOpsList[a]->RequiresGrad)continue;
		auto AddRes = DynamicTensor::DynamicStdOps_Forward_Add({ DynamicTensor(CurOps->GradOps) }, he(), true);
		BackwardOpsMap[CurOps->InputOpsList[a].get()][CurOps.get()] = AddRes.Ops;
	}
}

DynamicTensor DynamicTensor::DynamicStdOps_Forward_Matmul(std::vector<DynamicTensor>InputList, he InputParams, bool RequiresGrad)
{
	bool IsInput1stT = InputParams["is_input_1st_T"].i();
	bool IsInput2ndT = InputParams["is_input_2nd_T"].i();
	Tensor* Input1stRes, * Input2ndRes;
	if (IsInput1stT)Input1stRes = InputList[0].Ops->TensorPointer->T();
	else Input1stRes = InputList[0].Ops->TensorPointer.get();
	if (IsInput2ndT)Input2ndRes = InputList[1].Ops->TensorPointer->T();
	else Input2ndRes = InputList[1].Ops->TensorPointer.get();
	auto TensorResult = Input1stRes->Matmul(Input2ndRes);
	if (IsInput1stT)delete Input1stRes;
	if (IsInput2ndT)delete Input2ndRes;
	return SetComputationalHistory(TensorResult, InputList, InputParams, OpsType::MatMul, RequiresGrad);
}
void DynamicTensor::DynamicStdOps_Backward_Matmul(std::map<DynamicOps*, std::map<DynamicOps*, std::shared_ptr<DynamicOps>>>&BackwardOpsMap,std::shared_ptr<DynamicOps>CurOps)
{
	bool IsInput1stT = CurOps->Params["is_input_1st_T"].i();
	bool IsInput2ndT = CurOps->Params["is_input_2nd_T"].i();
	if (CurOps->InputOpsList[0]->RequiresGrad)
	{
		he GradParams = he::NewDict();
		DynamicTensor TensorRes;
		if (IsInput1stT == false && IsInput2ndT == false)
		{
			GradParams["is_input_1st_T"] = false;
			GradParams["is_input_2nd_T"] = true;
			TensorRes = DynamicStdOps_Forward_Matmul({ DynamicTensor(CurOps->GradOps),DynamicTensor(CurOps->InputOpsList[1]) }, GradParams, true);
		}
		if (IsInput1stT == false && IsInput2ndT == true)
		{
			GradParams["is_input_1st_T"] = false;
			GradParams["is_input_2nd_T"] = false;
			TensorRes = DynamicStdOps_Forward_Matmul({ DynamicTensor(CurOps->GradOps),DynamicTensor(CurOps->InputOpsList[1]) }, GradParams, true);
		}
		if (IsInput1stT == true && IsInput2ndT == false)
		{
			GradParams["is_input_1st_T"] = false;
			GradParams["is_input_2nd_T"] = true;
			TensorRes = DynamicStdOps_Forward_Matmul({ DynamicTensor(CurOps->InputOpsList[1]),DynamicTensor(CurOps->GradOps) }, GradParams, true);
		}
		if (IsInput1stT == true && IsInput2ndT == true)
		{
			GradParams["is_input_1st_T"] = true;
			GradParams["is_input_2nd_T"] = true;
			TensorRes = DynamicStdOps_Forward_Matmul({ DynamicTensor(CurOps->InputOpsList[1]),DynamicTensor(CurOps->GradOps) }, GradParams, true);
		}
		BackwardOpsMap[CurOps->InputOpsList[0].get()][CurOps.get()] = TensorRes.Ops;
	}
	if (CurOps->InputOpsList[1]->RequiresGrad)
	{
		he GradParams = he::NewDict();
		DynamicTensor TensorRes;
		if (IsInput1stT == false && IsInput2ndT == false)
		{
			GradParams["is_input_1st_T"] = true;
			GradParams["is_input_2nd_T"] = false;
			TensorRes = DynamicStdOps_Forward_Matmul({ DynamicTensor(CurOps->InputOpsList[0]),DynamicTensor(CurOps->GradOps) }, GradParams, true);
		}
		if (IsInput1stT == false && IsInput2ndT == true)
		{
			GradParams["is_input_1st_T"] = true;
			GradParams["is_input_2nd_T"] = false;
			TensorRes = DynamicStdOps_Forward_Matmul({ DynamicTensor(CurOps->GradOps),DynamicTensor(CurOps->InputOpsList[0]) }, GradParams, true);
		}
		if (IsInput1stT == true && IsInput2ndT == false)
		{
			GradParams["is_input_1st_T"] = false;
			GradParams["is_input_2nd_T"] = false;
			TensorRes = DynamicStdOps_Forward_Matmul({ DynamicTensor(CurOps->InputOpsList[0]),DynamicTensor(CurOps->GradOps) }, GradParams, true);
		}
		if (IsInput1stT == true && IsInput2ndT == true)
		{
			GradParams["is_input_1st_T"] = true;
			GradParams["is_input_2nd_T"] = true;
			TensorRes = DynamicStdOps_Forward_Matmul({ DynamicTensor(CurOps->GradOps),DynamicTensor(CurOps->InputOpsList[0]) }, GradParams, true);
		}
		BackwardOpsMap[CurOps->InputOpsList[1].get()][CurOps.get()] = TensorRes.Ops;
	}
}