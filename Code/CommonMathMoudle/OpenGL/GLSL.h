#pragma once
#include <vector>
#include <string>

class GLSL
{
private:
    GLSL(){AddGLSLFun();}

public:
    GLSL(const GLSL&) = delete;
    GLSL& operator=(const GLSL&) = delete;

    static GLSL& I() 
    {
        static GLSL instance; // 静态局部变量，线程安全
        return instance;
    }

    std::vector<std::string> GLSLFun;
    int GLSLFunNum = 0;
    char* GetFunStr(int FunNum){return GLSLFun[FunNum].data();}
    void RegFun(int& InputRegFun, std::string InputFunContent)
    {
        InputRegFun = GLSLFunNum++;
        GLSLFun.push_back(InputFunContent);
    }

    int AddArrayInCPP;
    int FillArrayInCPP;
    int AddInCPP;
    int AddScalarInCPP;
    int EleMulInCPP;
    int MulScalarInCPP;
    int MatmulInCPP;
    int TInCPP;
    int SumTensorDimInCPP;
    int MaximumOrMinimumTensorDimInCPP;
    int TensorSpliceInCPP;
    int GetUnitTensorInCPP;
    int EleExpInCPP;
    int EleInverseInCPP;
    int BroadCastToInCPP;
    int FillRandomValNormalInCPP;

void AddGLSLFun()
{

RegFun(AddArrayInCPP,R"(
#version 430
layout(local_size_x = 256, local_size_y = 1) in;
layout(std430, binding = 0) buffer bufferOutput {
    float Output[];
};
layout(std430, binding = 1) buffer bufferInputFirst {
    float InputFirst[];
};
layout(std430, binding = 2) buffer bufferInputSecond {
    float InputSecond[];
};
layout(std430, binding = 3) buffer bufferSize {
    int Size;
};
void main() 
{
    uint Index = gl_GlobalInvocationID.x;
    if (Index < Size) Output[Index] = InputFirst[Index] + InputSecond[Index];
}
)");

RegFun(FillArrayInCPP,R"(
#version 430
layout(local_size_x = 256, local_size_y = 1) in;
layout(std430, binding = 0) buffer bufferInput {
    float Input[];
};
layout(std430, binding = 1) buffer bufferScalar {
    float Scalar;
};
layout(std430, binding = 2) buffer bufferSize {
    int Size;
};
void main() 
{
    uint Index = gl_GlobalInvocationID.x;
    if (Index < Size) Input[Index] = Scalar;
}
)");

RegFun(AddInCPP,R"(
#version 430
layout(local_size_x = 256, local_size_y = 1) in;
layout(std430, binding = 0) buffer bufferOutput {
    float Output[];
};
layout(std430, binding = 1) buffer bufferHighDimInput {
    float HighDimInput[];
};
layout(std430, binding = 2) buffer bufferHighDimSize {
    int HighDimSize;
};
layout(std430, binding = 3) buffer bufferLowDimInput {
    float LowDimInput[];
};
layout(std430, binding = 4) buffer bufferLowDimSize {
    int LowDimSize;
};
void main() 
{
    uint Index = gl_GlobalInvocationID.x;
    if (Index < HighDimSize)Output[Index] = HighDimInput[Index] + LowDimInput[Index%LowDimSize];
}
)");

RegFun(AddScalarInCPP,R"(
#version 430
layout(local_size_x = 256, local_size_y = 1) in;
layout(std430, binding = 0) buffer bufferOutput {
    float Output[];
};
layout(std430, binding = 1) buffer bufferInput {
    float Input[];
};
layout(std430, binding = 2) buffer bufferScalar {
    float Scalar;
};
layout(std430, binding = 3) buffer bufferSize {
    int Size;
};
void main() 
{
    uint Index = gl_GlobalInvocationID.x;
    if (Index < Size) Output[Index] = Input[Index] + Scalar;
}
)");

RegFun(EleMulInCPP,R"(
#version 430
layout(local_size_x = 256, local_size_y = 1) in;
layout(std430, binding = 0) buffer bufferOutput {
    float Output[];
};
layout(std430, binding = 1) buffer bufferHighDimInput {
    float HighDimInput[];
};
layout(std430, binding = 2) buffer bufferHighDimSize {
    int HighDimSize;
};
layout(std430, binding = 3) buffer bufferLowDimInput {
    float LowDimInput[];
};
layout(std430, binding = 4) buffer bufferLowDimSize {
    int LowDimSize;
};
void main() 
{
    uint Index = gl_GlobalInvocationID.x;
    if (Index < HighDimSize)Output[Index] = HighDimInput[Index] * LowDimInput[Index%LowDimSize];
}
)");

RegFun(MulScalarInCPP,R"(
#version 430
layout(local_size_x = 256, local_size_y = 1) in;
layout(std430, binding = 0) buffer bufferOutput {
    float Output[];
};
layout(std430, binding = 1) buffer bufferInput {
    float Input[];
};
layout(std430, binding = 2) buffer bufferScalar {
    float Scalar;
};
layout(std430, binding = 3) buffer bufferSize {
    int Size;
};
void main() 
{
    uint Index = gl_GlobalInvocationID.x;
    if (Index < Size) Output[Index] = Input[Index] * Scalar;
}
)");

RegFun(MatmulInCPP,R"(
#version 430
layout(local_size_x = 256, local_size_y = 1) in;
layout(std430, binding = 0) buffer bufferOutput {
    float Output[];
};
layout(std430, binding = 1) buffer bufferOutputBatchShape {
    int OutputBatchShape[];
};
layout(std430, binding = 2) buffer bufferOutputMatrixShape {
    int OutputMatrixShape[];
};
layout(std430, binding = 3) buffer bufferInputFirst {
    float InputFirst[];
};
layout(std430, binding = 4) buffer bufferInputFirstBatchShape {
    int InputFirstBatchShape[];
};
layout(std430, binding = 5) buffer bufferInputFirstMatrixShape {
    int InputFirstMatrixShape[];
};
layout(std430, binding = 6) buffer bufferInputSecond {
    float InputSecond[];
};
layout(std430, binding = 7) buffer bufferInputSecondBatchShape {
    int InputSecondBatchShape[];
};
layout(std430, binding = 8) buffer bufferInputSecondMatrixShape {
    int InputSecondMatrixShape[];
};
layout(std430, binding = 9) buffer bufferBatchShapeLen {
    int BatchShapeLen;
};
layout(std430, binding = 10) buffer bufferOutputShapeCount {
    int OutputShapeCount;
};
void main() 
{
    uint Index = gl_GlobalInvocationID.x;
    if (Index < OutputShapeCount)
    {
        int OutputBatchIndex[8];
        int OutputMatrixShapeCount = OutputMatrixShape[0]*OutputMatrixShape[1];
        int OutSizeTMP = int(Index)/OutputMatrixShapeCount;
        bool MatZero = bool(OutSizeTMP);
        for(int a=BatchShapeLen-1;a>=0;a--)
        {
            if(!MatZero)OutputBatchIndex[a] = 0;
            else
            {
              OutputBatchIndex[a] = OutSizeTMP%OutputBatchShape[a];
              OutSizeTMP /= OutputBatchShape[a];
            }
        }
        int InputFirstBatchIndex[8];
        for(int a=BatchShapeLen-1;a>=0;a--)
        {
          if(OutputBatchIndex[a] < InputFirstBatchShape[a])InputFirstBatchIndex[a] = OutputBatchIndex[a];
          else InputFirstBatchIndex[a] = 0;
        }
        int InputFirstMatrixShapeCount = InputFirstMatrixShape[0]*InputFirstMatrixShape[1];
        int InputSecondBatchIndex[8];
        for(int a=BatchShapeLen-1;a>=0;a--)
        {
          if(OutputBatchIndex[a] < InputSecondBatchShape[a])InputSecondBatchIndex[a] = OutputBatchIndex[a];
          else InputSecondBatchIndex[a] = 0;
        }
        int InputSecondMatrixShapeCount = InputSecondMatrixShape[0]*InputSecondMatrixShape[1];
        int InputFirstBase = 0;
        int InFirstTMP = InputFirstMatrixShapeCount;
        for(int a=BatchShapeLen-1;a>=0;a--)
        {
          InputFirstBase += InFirstTMP*InputFirstBatchIndex[a];
          InFirstTMP*=InputFirstBatchShape[a];
        }
        int InputSecondBase = 0;
        int InSecondTMP = InputSecondMatrixShapeCount;
        for(int a=BatchShapeLen-1;a>=0;a--)
        {
          InputSecondBase += InSecondTMP*InputSecondBatchIndex[a];
          InSecondTMP*=InputSecondBatchShape[a];
        }
        int OutputMatrixIndex = int(Index)%OutputMatrixShapeCount;
        int MatIndex[2] = {OutputMatrixIndex/OutputMatrixShape[1], OutputMatrixIndex%OutputMatrixShape[1]};
        Output[Index] = 0;
        for(int a=0;a<InputFirstMatrixShape[1];a++)
        {
            Output[Index] += InputFirst[InputFirstBase + MatIndex[0]*InputFirstMatrixShape[1] + a]*InputSecond[InputSecondBase + a*InputSecondMatrixShape[1] + MatIndex[1]];
        }
    }
}
)");

RegFun(TInCPP,R"(
#version 430
layout(local_size_x = 256, local_size_y = 1) in;
layout(std430, binding = 0) buffer bufferOutput {
    float Output[];
};
layout(std430, binding = 1) buffer bufferInput {
    float Input[];
};
layout(std430, binding = 2) buffer bufferMatrixShape {
    int MatrixShape[];
};
layout(std430, binding = 3) buffer bufferShapeCount {
    int ShapeCount;
};
void main() 
{
    uint Index = gl_GlobalInvocationID.x;
    if(Index < ShapeCount)
    {
        int MatrixShapeCount = MatrixShape[0]*MatrixShape[1];
        int InputMatIndex = int(Index)%MatrixShapeCount;
        int BaseCount = int(Index) - InputMatIndex;
        int InputMatIndexFirst = InputMatIndex/MatrixShape[1];
        int InputMatIndexSecond = InputMatIndex%MatrixShape[1];
        Output[BaseCount + InputMatIndexSecond*MatrixShape[0] + InputMatIndexFirst] = Input[Index];
    }
}
)");

RegFun(SumTensorDimInCPP,R"(
#version 430
layout(local_size_x = 256, local_size_y = 1) in;
layout(std430, binding = 0) buffer bufferOutputData {
    float OutputData[];
};
layout(std430, binding = 1) buffer bufferInputData {
    float InputData[];
};
layout(std430, binding = 2) buffer bufferInputShape {
    int InputShape[];
};
layout(std430, binding = 3) buffer bufferInputShapeLen {
    int InputShapeLen;
};
layout(std430, binding = 4) buffer bufferInputDim {
    int InputDim;
};
layout(std430, binding = 5) buffer bufferOutputShapeCount {
    int OutputShapeCount;
};
void main() 
{
    uint Index = gl_GlobalInvocationID.x;
    if(Index < OutputShapeCount)
    {
        int OutputIndex[8];
        int OutputSizeTMP = int(Index);
        for(int a=InputShapeLen-1;a>=0;a--)
        {
            if(a != InputDim) 
            {
                OutputIndex[a] = OutputSizeTMP%InputShape[a];
                OutputSizeTMP /= InputShape[a];
            }
            else
            {
                OutputIndex[a] = 0;
            }
        }
        OutputData[Index] = 0;
        for(int a =0;a<InputShape[InputDim];a++)
        {
            int InputDimIndex = 0;
            int InputSizeTMP = 1;
            for(int b = InputShapeLen - 1;b>=0;b--)
            {
                if(b!=InputDim)InputDimIndex += InputSizeTMP*OutputIndex[b];
                else InputDimIndex += InputSizeTMP*a;
                InputSizeTMP*=InputShape[b];
            }
            OutputData[Index] += InputData[InputDimIndex];
        }
    }
}
)");

RegFun(MaximumOrMinimumTensorDimInCPP,R"(
#version 430
layout(local_size_x = 256, local_size_y = 1) in;
layout(std430, binding = 0) buffer bufferOutputData {
    float OutputData[];
};
layout(std430, binding = 1) buffer bufferInputData {
    float InputData[];
};
layout(std430, binding = 2) buffer bufferInputShape {
    int InputShape[];
};
layout(std430, binding = 3) buffer bufferInputShapeLen {
    int InputShapeLen;
};
layout(std430, binding = 4) buffer bufferInputDim {
    int InputDim;
};
layout(std430, binding = 5) buffer bufferOutputShapeCount {
    int OutputShapeCount;
};
layout(std430, binding = 6) buffer bufferIsMaximum {
    int IsMaximum;
};
void main() 
{
    uint Index = gl_GlobalInvocationID.x;
    if(Index < OutputShapeCount)
    {
        if(bool(IsMaximum))
        {
            OutputData[Index] = -1e9+7;
        }
        else
        {
            OutputData[Index] = 1e9+7;
        }
        int OutputIndex[8];
        int OutputSizeTMP = int(Index);
        for(int a=InputShapeLen-1;a>=0;a--)
        {
            if(a != InputDim) 
            {
                OutputIndex[a] = OutputSizeTMP%InputShape[a];
                OutputSizeTMP /= InputShape[a];
            }
            else
            {
                OutputIndex[a] = 0;
            }
        }
        for(int a =0;a<InputShape[InputDim];a++)
        {
            int InputDimIndex = 0;
            int InputSizeTMP = 1;
            for(int b = InputShapeLen - 1;b>=0;b--)
            {
                if(b!=InputDim)InputDimIndex += InputSizeTMP*OutputIndex[b];
                else InputDimIndex += InputSizeTMP*a;
                InputSizeTMP*=InputShape[b];
            }
            if(bool(IsMaximum))
            {
                OutputData[Index] = max(OutputData[Index], InputData[InputDimIndex]);
            }
            else
            {
                OutputData[Index] = min(OutputData[Index], InputData[InputDimIndex]);
            }
        }
    }
}
)");

RegFun(TensorSpliceInCPP,R"(
#version 430
layout(local_size_x = 256, local_size_y = 1) in;
layout(std430, binding = 0) buffer bufferOutputData {
    float OutputData[];
};
layout(std430, binding = 1) buffer bufferInputDataFirst {
    float InputDataFirst[];
};
layout(std430, binding = 2) buffer bufferInputDataSecond {
    float InputDataSecond[];
};
layout(std430, binding = 3) buffer bufferShapeCount {
    int InputShapeFirst[];
};
layout(std430, binding = 4) buffer bufferInputShapeSecond {
    int InputShapeSecond[];
};
layout(std430, binding = 5) buffer bufferInputShapeLen {
    int InputShapeLen;
};
layout(std430, binding = 6) buffer bufferInputDim {
    int InputDim;
};
layout(std430, binding = 7) buffer bufferOutputShapeCount {
    int OutputShapeCount;
};
void main() 
{
    uint Index = gl_GlobalInvocationID.x;
    if(Index >= OutputShapeCount)return;
    int RightShapeCount = 1;
    //算出指定维度右边的单元大小
    for(int a=InputDim + 1;a<InputShapeLen;a++)
    {
        RightShapeCount*= InputShapeFirst[a];
    }
    //算出指定维度的大小
    int InputDimCount = InputShapeFirst[InputDim] + InputShapeSecond[InputDim];
    int LeftDimCount = int(Index)/RightShapeCount;
    int NowDimCount = LeftDimCount%InputDimCount;
    int StrictLeftDimCount = LeftDimCount/InputDimCount;
    if(NowDimCount < InputShapeFirst[InputDim])
    {
        OutputData[Index] = InputDataFirst[Index - StrictLeftDimCount*InputShapeSecond[InputDim]*RightShapeCount];
    }
    else
    {
        OutputData[Index] = InputDataSecond[Index - (StrictLeftDimCount+1)*InputShapeFirst[InputDim]*RightShapeCount];
    }
}
)");

RegFun(GetUnitTensorInCPP,R"(
#version 430
layout(local_size_x = 256, local_size_y = 1) in;
layout(std430, binding = 0) buffer bufferOutputData {
    float OutputData[];
};
layout(std430, binding = 1) buffer bufferInputShape {
    int InputShape[];
};
layout(std430, binding = 2) buffer bufferOutputShapeCount {
    int OutputShapeCount;
};
layout(std430, binding = 3) buffer bufferInputShapeLen {
    int InputShapeLen;
};
void main() 
{
    uint Index = gl_GlobalInvocationID.x;
    if(Index >= OutputShapeCount)return;
    int MatrixShapeCount = InputShape[InputShapeLen - 2]*InputShape[InputShapeLen - 1];
    int MatrixIndex = int(Index)%MatrixShapeCount;
    if(MatrixIndex%InputShape[InputShapeLen - 2] == MatrixIndex/InputShape[InputShapeLen - 2])
    {
      OutputData[Index] = 1;
    }
}
)");

RegFun(EleExpInCPP,R"(
#version 430
layout(local_size_x = 256, local_size_y = 1) in;
layout(std430, binding = 0) buffer bufferOutputData {
    float OutputData[];
};
layout(std430, binding = 1) buffer bufferOutputShape {
    int OutputShape;
};
layout(std430, binding = 2) buffer bufferBaseNum {
    float BaseNum;
};
void main() 
{
    uint Index = gl_GlobalInvocationID.x;
    if(Index >= OutputShape)return;
    OutputData[Index] = pow(BaseNum, OutputData[Index]);
}
)");

RegFun(EleInverseInCPP,R"(
#version 430
layout(local_size_x = 256, local_size_y = 1) in;
layout(std430, binding = 0) buffer bufferOutputData {
    float OutputData[];
};
layout(std430, binding = 1) buffer bufferOutputShape {
    int OutputShape;
};
void main() 
{
    uint Index = gl_GlobalInvocationID.x;
    if(Index >= OutputShape)return;
    OutputData[Index] = 1./OutputData[Index];
}
)");

RegFun(BroadCastToInCPP,R"(
#version 430
layout(local_size_x = 256, local_size_y = 1) in;
layout(std430, binding = 0) buffer bufferOutputData {
    float OutputData[];
};
layout(std430, binding = 1) buffer bufferInputData {
    float InputData[];
};
layout(std430, binding = 2) buffer bufferOutputShape {
    int OutputShape[];
};
layout(std430, binding = 3) buffer bufferInputShape {
    int InputShape[];
};
layout(std430, binding = 4) buffer bufferShapeLen {
    int ShapeLen;
};
layout(std430, binding = 5) buffer bufferOutputShapeCount {
    int OutputShapeCount;
};
void main() 
{
    uint Index = gl_GlobalInvocationID.x;
    if(Index >= OutputShapeCount)return;
    int ShapeIndex[10];
    int NowIndex = int(Index);
    for(int a = ShapeLen - 1 ;a >= 0;a--)
    {
        ShapeIndex[a] = NowIndex%OutputShape[a];
        NowIndex = int(NowIndex/OutputShape[a]);
        if(OutputShape[a] > InputShape[a])ShapeIndex[a] = 0;
    }
    int FixedInputIndex = 0;
    for(int a = 0;a<ShapeLen;a++)
    {
        FixedInputIndex *= InputShape[a];
        FixedInputIndex += ShapeIndex[a];
    }
    OutputData[Index] = InputData[FixedInputIndex];
}
)");

RegFun(FillRandomValNormalInCPP,R"(
#version 430
layout(local_size_x = 256, local_size_y = 1) in;
layout(std430, binding = 0) buffer bufferOutputData {
    float OutputData[];
};
layout(std430, binding = 1) buffer bufferInputData {
    int  OutputShapeCount;
};
layout(std430, binding = 2) buffer bufferOutputShape {
    float MeanV;
};
layout(std430, binding = 3) buffer bufferInputShape {
    float VarianceV;  
};
layout(std430, binding = 4) buffer bufferShapeLen {
    uint Seed;
};
float rand(inout uint seed)
{
    seed ^= 2747636419u;
    seed *= 2654435769u;
    seed ^= (seed >> 16u);
    // 将整型映射到 [0,1)
    return float(seed & 0xFFFFFFFFu) / 4294967295.0;
}
// Box-Muller 生成正态随机数
float generateGaussian(uint baseSeed, float mean, float param)
{
    uint threadID = gl_GlobalInvocationID.x;
    // 保证不同线程使用不同 seed
    uint seed = baseSeed ^ threadID;
    // 取两个 [0,1) 随机数
    float u1 = rand(seed);
    float u2 = rand(seed);
    // Box-Muller 核心公式
    float z0 = sqrt(-2.0 * log(u1)) * cos(6.28318530718 * u2);
    // 将 z0 调整为你想要的分布
    // param 可以是你事先准备好的 σ (标准差)；也可以是任何你想用来乘以 z0 的散度参数。
    return z0 * param + mean;
}
void main()
{
    uint idx = gl_GlobalInvocationID.x;
    if (idx >= uint(OutputShapeCount))return;
    OutputData[idx] = generateGaussian(Seed, MeanV, VarianceV);
}
)");

}};
