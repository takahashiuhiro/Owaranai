#include "TensorCoreCudaHead.cuh"

__global__ void AddArrayKernel(float* Output, float* InputFirst, float* InputSecond,size_t Size) 
{
  size_t Index = blockIdx.x * blockDim.x + threadIdx.x;
  if (Index < Size) Output[Index] = InputFirst[Index] + InputSecond[Index];
}

__global__ void FillArrayKernel(float* Input, float Scalar,size_t Size) 
{
  size_t Index = blockIdx.x * blockDim.x + threadIdx.x;
  if (Index < Size) Input[Index] = Scalar;
}

__global__ void DotArrayKernel(float* Output, size_t OutSize, size_t InSize) 
{
  size_t Index = blockIdx.x * blockDim.x + threadIdx.x;
  if (Index < InSize && Index + InSize < OutSize)Output[Index] += Output[Index + InSize];
  __syncthreads();
}

__global__ void AddScalarKernel(float* Output,float* Input, float Scalar,size_t Size) 
{
  size_t Index = blockIdx.x * blockDim.x + threadIdx.x;
  if (Index < Size) Output[Index] = Input[Index] + Scalar;
}

__global__ void MulScalarKernel(float* Output,float* Input, float Scalar,size_t Size) 
{
  size_t Index = blockIdx.x * blockDim.x + threadIdx.x;
  if (Index < Size) Output[Index] = Input[Index] * Scalar;
}

__global__ void AddKernel(float* Output, float* HighDimInput, size_t HighDimSize, float* LowDimInput, size_t LowDimSize) 
{
  size_t Index = blockIdx.x * blockDim.x + threadIdx.x;
  if (Index < HighDimSize)Output[Index] = HighDimInput[Index] + LowDimInput[Index%LowDimSize];
}

__global__ void EleMulKernel(float* Output, float* HighDimInput, size_t HighDimSize, float* LowDimInput, size_t LowDimSize) 
{
  size_t Index = blockIdx.x * blockDim.x + threadIdx.x;
  if (Index < HighDimSize)Output[Index] = HighDimInput[Index] * LowDimInput[Index%LowDimSize];
}

__global__ void MatmulKernel
(
  float* Output, 
  size_t *OutputBatchShape, 
  size_t *OutputMatrixShape,
  float* InputFirst, 
  size_t *InputFirstBatchShape, 
  size_t *InputFirstMatrixShape,
  float* InputSecond, 
  size_t *InputSecondBatchShape, 
  size_t *InputSecondMatrixShape,
  size_t BatchShapeLen,
  size_t OutputShapeCount
)
{
  //todo::段错误应该在这个函数里，写的不对仔细查，反正数据不多，你直接把输入拿来手动检查吧，脑测很麻烦也很烦，能跑一个就能跑一大堆
  size_t Index = blockIdx.x * blockDim.x + threadIdx.x;
  if (Index < OutputShapeCount)
  {
    size_t OutputBatchIndex[8];
    size_t OutputMatrixShapeCount = OutputMatrixShape[0]*OutputMatrixShape[1];
    size_t OutSizeTMP = Index/OutputMatrixShapeCount;
    bool MatZero = OutSizeTMP;
    for(int a=OutputShapeCount-1;a>=0;a--)
    {
      if(!MatZero)OutputBatchIndex[a] = 0;
      else
      {
        OutputBatchIndex[a] = OutSizeTMP%OutputBatchShape[a];
        OutSizeTMP /= OutputBatchShape[a];
      }
    }
    //填充输出矩阵的batch维度
    size_t InputFirstBatchIndex[8];
    for(int a=OutputShapeCount-1;a>=0;a--)
    {
      if(OutputBatchIndex[a] < InputFirstBatchShape[a])InputFirstBatchIndex[a] = OutputBatchIndex[a];
      else InputFirstBatchIndex[a] = 0;
    }
    size_t InputFirstMatrixShapeCount = InputFirstMatrixShape[0]*InputFirstMatrixShape[1];
    size_t InputSecondBatchIndex[8];
    for(int a=OutputShapeCount-1;a>=0;a--)
    {
      if(OutputBatchIndex[a] < InputSecondBatchShape[a])InputSecondBatchIndex[a] = OutputBatchIndex[a];
      else InputSecondBatchIndex[a] = 0;
    }
    size_t InputSecondMatrixShapeCount = InputSecondMatrixShape[0]*InputSecondMatrixShape[1];
    //填充输入矩阵的batch维度

    size_t InputFirstBase = 0;
    size_t InFirstTMP = InputFirstMatrixShapeCount;
    for(int a=OutputShapeCount-1;a>=0;a--)
    {
      InputFirstBase += InFirstTMP*InputFirstBatchIndex[a];
      InFirstTMP*=InputFirstBatchShape[a];
    }

    size_t InputSecondBase = 0;
    size_t InSecondTMP = InputSecondMatrixShapeCount;
    for(int a=OutputShapeCount-1;a>=0;a--)
    {
      InputSecondBase += InSecondTMP*InputSecondBatchIndex[a];
      InSecondTMP*=InputSecondBatchShape[a];
    }
    //计算batch维度

    size_t OutputMatrixIndex = Index%OutputMatrixShapeCount;
    size_t MatIndex[2] = {OutputMatrixIndex/OutputMatrixShape[1], OutputMatrixIndex%OutputMatrixShape[1]};
    Output[Index] = 0;
    for(int a=0;a<InputFirstMatrixShape[1];a++)
    {
      Output[Index] += InputFirst[InputFirstBase + MatIndex[0]*InputFirstMatrixShape[1] + a]*InputSecond[InputSecondBase + a*InputSecondMatrixShape[1] + MatIndex[1]];
    }
    //__syncthreads();
  }
}

void MatmulInCPP
(
  float* Output, 
  size_t OutputBatchShape[8], 
  size_t OutputMatrixShape[2],
  float* InputFirst, 
  size_t InputFirstBatchShape[8], 
  size_t InputFirstMatrixShape[2],
  float* InputSecond, 
  size_t InputSecondBatchShape[8], 
  size_t InputSecondMatrixShape[2],
  size_t BatchShapeLen,
  size_t OutputShapeCount,
  size_t DeviceNum
)
{
  cudaSetDevice(DeviceNum);
  printf("打印一下矩阵信息,准备开始cuda\n");
  printf("打印矩阵batch\n");
  for(int a=0;a<BatchShapeLen;a++)
  {
    printf("%lu, %lu, %lu\n",OutputBatchShape[a],InputFirstBatchShape[a], InputSecondBatchShape[a]);
  }
  printf("打印矩阵shape\n");
  for(int a=0;a<2;a++)
  {
    printf("%lu, %lu, %lu\n",OutputMatrixShape[a],InputFirstMatrixShape[a], InputSecondMatrixShape[a]);
  }
  printf("看看输出长度 %lu\n", OutputShapeCount);
  printf("打印信息结束\n\n");

  size_t *OutputBatchShapeCuda;
  cudaMalloc((void**)&OutputBatchShapeCuda, 8*sizeof(size_t));
  cudaMemcpy(OutputBatchShapeCuda,OutputBatchShape,sizeof(size_t)*8,cudaMemcpyHostToDevice);
  size_t *OutputMatrixShapeCuda;
  cudaMalloc((void**)&OutputMatrixShapeCuda, 2*sizeof(size_t));
  cudaMemcpy(OutputMatrixShapeCuda,OutputMatrixShape,sizeof(size_t)*2,cudaMemcpyHostToDevice);
  size_t *InputFirstBatchShapeCuda;
  cudaMalloc((void**)&InputFirstBatchShapeCuda, 8*sizeof(size_t));
  cudaMemcpy(InputFirstBatchShapeCuda,InputFirstBatchShape,sizeof(size_t)*8,cudaMemcpyHostToDevice);
  size_t *InputFirstMatrixShapeCuda;
  cudaMalloc((void**)&InputFirstMatrixShapeCuda, 2*sizeof(size_t));
  cudaMemcpy(InputFirstMatrixShapeCuda,InputFirstMatrixShape,sizeof(size_t)*2,cudaMemcpyHostToDevice);
  size_t *InputSecondBatchShapeCuda;
  cudaMalloc((void**)&InputSecondBatchShapeCuda, 8*sizeof(size_t));
  cudaMemcpy(InputSecondBatchShapeCuda,InputSecondBatchShape,sizeof(size_t)*8,cudaMemcpyHostToDevice);
  size_t *InputSecondMatrixShapeCuda;
  cudaMalloc((void**)&InputSecondMatrixShapeCuda, 2*sizeof(size_t));
  cudaMemcpy(InputSecondMatrixShapeCuda,InputSecondMatrixShape,sizeof(size_t)*2,cudaMemcpyHostToDevice);


  CudaPair CudaPairInput = GetCudaPair(OutputShapeCount);
  MatmulKernel<<<CudaPairInput.block, CudaPairInput.grid>>>
  (
    Output, 
    OutputBatchShapeCuda, 
    OutputMatrixShapeCuda, 
    InputFirst,
    InputFirstBatchShapeCuda, 
    InputFirstMatrixShapeCuda,
    InputSecond, 
    InputSecondBatchShapeCuda,
    InputSecondMatrixShapeCuda,
    BatchShapeLen,
    OutputShapeCount
  );
}

void FillArrayInCPP(float* Input, float Scalar,size_t Size)
{
  CudaPair CudaPairInput = GetCudaPair(Size);
  FillArrayKernel<<<CudaPairInput.block, CudaPairInput.grid>>>(Input, Scalar, Size);
}

void AddScalarInCPP(float* Output,float* Input, float Scalar,size_t Size) 
{
  CudaPair CudaPairInput = GetCudaPair(Size);
  AddScalarKernel<<<CudaPairInput.block, CudaPairInput.grid>>>(Output,Input, Scalar, Size);
}

void MulScalarInCPP(float* Output,float* Input, float Scalar,size_t Size) 
{
  CudaPair CudaPairInput = GetCudaPair(Size);
  MulScalarKernel<<<CudaPairInput.block, CudaPairInput.grid>>>(Output,Input, Scalar, Size);
}

void AddArrayInCPP(float* Output, float* InputFirst, float* InputSecond, size_t Size) 
{
  CudaPair CudaPairInput = GetCudaPair(Size);
  AddArrayKernel<<<CudaPairInput.block, CudaPairInput.grid>>>(Output, InputFirst, InputSecond, Size);
}

void DotArrayInCPP(float* Output, float* InputFirst, float* InputSecond, size_t Size) 
{
  CudaPair CudaPairInput = GetCudaPair(Size);
  float *OutTMP;
  cudaMalloc((void**)&OutTMP, Size*sizeof(float));
  EleMulKernel<<<CudaPairInput.block, CudaPairInput.grid>>>(OutTMP, InputFirst, Size, InputSecond, Size);
  size_t SizeTMP = Size;
  while(SizeTMP > 1)
  {
    CudaPairInput = GetCudaPair(SizeTMP);
    DotArrayKernel<<<CudaPairInput.block, CudaPairInput.grid>>>(OutTMP, SizeTMP, (SizeTMP + 1)/2);
    SizeTMP = (SizeTMP + 1)/2;
  }
  cudaMemcpy(Output,OutTMP,sizeof(float),cudaMemcpyDeviceToDevice);
  cudaFree(OutTMP);
}

void AddInCPP(float* Output, float* HighDimInput, size_t HighDimSize, float* LowDimInput, size_t LowDimSize) 
{
  CudaPair CudaPairInput = GetCudaPair(HighDimSize);
  AddKernel<<<CudaPairInput.block, CudaPairInput.grid>>>(Output, HighDimInput, HighDimSize, LowDimInput, LowDimSize);
}

void EleMulInCPP(float* Output, float* HighDimInput, size_t HighDimSize, float* LowDimInput, size_t LowDimSize) 
{
  CudaPair CudaPairInput = GetCudaPair(HighDimSize);
  EleMulKernel<<<CudaPairInput.block, CudaPairInput.grid>>>(Output, HighDimInput, HighDimSize, LowDimInput, LowDimSize);
}

void DataToCPU(float* CPUPointer, float* GPUPointer, size_t Size){cudaMemcpy(CPUPointer,GPUPointer,sizeof(float)*Size,cudaMemcpyDeviceToHost);}
void DataToGPU(float* CPUPointer, float* GPUPointer, size_t Size){cudaMemcpy(GPUPointer,CPUPointer,sizeof(float)*Size,cudaMemcpyHostToDevice);}
void cudaFreeInCPP(float* Input){cudaFree(Input);}
void cudaMallocInCPP(float** Input, size_t Size, size_t DeviceNum)
{
  cudaSetDevice(DeviceNum);
  cudaMalloc(Input, Size*sizeof(float));
}




