#include "DynamicTensor.h"

DynamicOps::DynamicOps(DynamicTensor* DynamicTensorNode){leafNode = DynamicTensorNode;}

DynamicTensor::DynamicTensor(std::shared_ptr<Tensor> InputTensor,bool IsRequiresGrad)
{
    TensorPointer = InputTensor;
    RequiresGrad = IsRequiresGrad;
    if (IsRequiresGrad)
    {
        Grad = std::make_shared<DynamicTensor>(std::shared_ptr<Tensor>(TensorPointer->CopyNewEmptyTensor()),false);
        Grad->TensorPointer->FillArray(0);
    }
}
DynamicTensor::DynamicTensor(const DynamicTensor& Other){Set(this, &Other);}
DynamicTensor DynamicTensor::CreateDynamicTensor(std::shared_ptr<Tensor> InputTensor, bool IsRequiresGrad){return DynamicTensor(InputTensor);}
DynamicTensor DynamicTensor::CreateDynamicTensor(std::vector<size_t>shape, bool IsRequiresGrad, size_t DeviceNum){return DynamicTensor(std::make_shared<Tensor>(shape, DeviceNum));}
DynamicTensor DynamicTensor::CreateDynamicTensor(std::vector<size_t>shape, std::vector<float>InputData, bool IsRequiresGrad, size_t DeviceNum){return DynamicTensor(std::make_shared<Tensor>(shape, DeviceNum, InputData));}
DynamicTensor DynamicTensor::CreateVector(std::vector<float>InputData, size_t DeviceNum)
{
    std::vector<size_t>ThisNewShape = { 1U,InputData.size() };
    return DynamicTensor(std::make_shared<Tensor>(ThisNewShape, DeviceNum, InputData));
}

DynamicTensor::~DynamicTensor()
{
    SetOutputList(Ops,this);
    for (auto& element : OutNodeList)SetInputList(element->Ops, this);
    if(TensorPointer!=nullptr)
    {
        TensorPointer.reset();
    }
}

void DynamicTensor::SetOutputList(DynamicOps& CurOps, DynamicTensor* TargetOutputNode)
{
    for (size_t a = 0; a < CurOps.InputOpsList.size(); a++)
    {
        if (CurOps.InputOpsList[a].leafNode != nullptr)
        {
            for (auto& element : TargetOutputNode->OutNodeList)CurOps.InputOpsList[a].leafNode->OutNodeList.insert(element);
            CurOps.InputOpsList[a].leafNode->OutNodeList.erase(TargetOutputNode);
            continue;
        }
        SetOutputList(CurOps.InputOpsList[a], TargetOutputNode);
    }
}

void DynamicTensor::SetInputList(DynamicOps& CurOps, DynamicTensor* TargetOutputNode)
{
    for (size_t a = 0; a < CurOps.InputOpsList.size(); a++)
    {
        if (CurOps.InputOpsList[a].leafNode !=nullptr)
        {
            if (CurOps.InputOpsList[a].leafNode == TargetOutputNode)
            {
                CurOps.InputOpsList[a] = TargetOutputNode->Ops;
                CurOps.InputOpsList[a].leafNode = nullptr;
            }
        }
        else SetInputList(CurOps.InputOpsList[a], TargetOutputNode);
    }
}

void DynamicTensor::PrintData()
{
    TensorPointer->PrintData();
}

void DynamicTensor::SetForwardHistory(DynamicTensor& InputRes, size_t InputOptType, std::vector<DynamicTensor*>OpsList, he InputPrams, bool IsRequiresGrad)
{   
    if (!IsRequiresGrad)return;
    InputRes.Ops.DynamicOpsType = InputOptType;
    InputRes.Ops.InputOpsList = {};
    InputRes.Ops.Params = InputPrams;
    for (size_t a = 0; a < OpsList.size(); a++)
    {
        InputRes.Ops.InputOpsList.push_back(DynamicOps(OpsList[a]));
        OpsList[a]->OutNodeList.insert(&InputRes);
        if (OpsList[a]->RequiresGrad)
        {
            InputRes.RequiresGrad = true;
            InputRes.Grad = std::make_shared<DynamicTensor>(std::shared_ptr<Tensor>(InputRes.TensorPointer->CopyNewEmptyTensor()), false);
            InputRes.Grad->TensorPointer->FillArray(0);
        }
    }
}

void DynamicTensor::Backward(DynamicTensor LossResult)
{

}

void DynamicTensor::Backward()
{
    Grad = std::make_shared<DynamicTensor>(std::shared_ptr<Tensor>(TensorPointer->Copy()));//������������������������ʱ���Լ���ֵ���ǳ�ʼ���ݶ�ֵ
    std::map<DynamicOps*, std::shared_ptr<DynamicTensor>>GradGlobalResult;//������ϵ�ȫ�ֵ���(���ڵ�������Ҫ����Щ�ڵ�û��ʵ�壬����ֱ�Ӵ浽tensor��)
    std::map<DynamicOps*, std::shared_ptr<DynamicTensor>>ForwardGlobalResult;//������ϵ�ʵ��
    std::map<std::pair<DynamicOps*, DynamicOps*>, std::shared_ptr<DynamicTensor>>PartGradGlobalResult;//������ϵ�����->����ĵ������(��Ӧһ������ڵ�������ڵ�����)
    GradGlobalResult[&(this->Ops)] = Grad;
    BackwardDfs(GradGlobalResult, ForwardGlobalResult, PartGradGlobalResult, &(this->Ops));
}

void DynamicTensor::BackwardDfs(std::map<DynamicOps*, std::shared_ptr<DynamicTensor>>& GradGlobalResult, std::map<DynamicOps*, std::shared_ptr<DynamicTensor>>& ForwardGlobalResult, std::map<std::pair<DynamicOps*, DynamicOps*>, std::shared_ptr<DynamicTensor>>& PartGradGlobalResult,DynamicOps* CurOps)
{
    //todo
    //1.��ʵ��::���opsû��ʵ�壬�Ǿ���dfs(��дһ������������£����������ǵ�һ����ʵ�壬����û��ʵ�嵫�������)�����ʵ�����ForwardGlobalResult��
    for (size_t a = 0; a < CurOps->InputOpsList.size(); a++)
    {
    }
    //2.ͨ����ǰ��һ�����е��������ڵ㵽�ýڵ�ĵ�����ÿ�������һ�����ֵ������ͼ���ǲ������в��ֶ�������ˣ�������Ǿ����ϣ����Ǿ�ֹͣ
    //3.�����������Ҫ�㵼�����ǾͲ�����
    //4.�����Ƿ���ʵ�嶼Ҫ��map���һ�ݣ���ʵ��Ļ�Ҫ������tensor���
}

void DynamicTensor::Set(DynamicTensor* ThisTensor, const DynamicTensor* OtherTensor)
{
    ThisTensor->TensorPointer = OtherTensor->TensorPointer;
    ThisTensor->Ops = OtherTensor->Ops;
}

DynamicTensor DynamicTensor::Add(std::vector<DynamicTensor*>InputList, he InputPrams, bool IsRequiresGrad)
{
    DynamicTensor Res = DynamicTensor(std::shared_ptr<Tensor>(InputList[0]->TensorPointer->Add(InputList[1]->TensorPointer.get())));
    SetForwardHistory(Res, OpsType::Add, InputList, InputPrams, IsRequiresGrad);
    return Res;
}
