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

void FillArrayInCPP(float* Input, float Scalar,size_t Size)
{
  FillArrayKernel<<<1, Size>>>(Input, Scalar, Size);
}

void AddArrayInCPP(float* Output, float* InputFirst, float* InputSecond, size_t Size) 
{
  //float *OutputTMP, *InputFirstTMP, *InputSecondTMP;
//
  //cudaMallocInCPP(&OutputTMP, Size);
  //cudaMallocInCPP(&InputFirstTMP, Size);
  //cudaMallocInCPP(&InputSecondTMP, Size);
//
  //cudaMemcpy(InputFirstTMP,InputFirst,sizeof(float)*Size,cudaMemcpyHostToDevice);
  //cudaMemcpy(InputSecondTMP,InputSecond,sizeof(float)*Size,cudaMemcpyHostToDevice);

  AddArrayKernel<<<1, Size>>>(Output, InputFirst, InputSecond, Size);

  //cudaMemcpy(Output,OutputTMP,sizeof(float)*Size,cudaMemcpyDeviceToHost);
  //cudaFree(OutputTMP);
  //cudaFree(InputFirstTMP);
  //cudaFree(InputSecondTMP);
}

void DataToCPU(float* CPUPointer, float* GPUPointer, size_t Size)
{
  cudaMemcpy(CPUPointer,GPUPointer,sizeof(float)*Size,cudaMemcpyDeviceToHost);
}

void DataToGPU(float* CPUPointer, float* GPUPointer, size_t Size)
{
  cudaMemcpy(GPUPointer,CPUPointer,sizeof(float)*Size,cudaMemcpyHostToDevice);
}

void cudaFreeInCPP(float* Input)
{
  cudaFree(Input);
}

void cudaMallocInCPP(float** Input, size_t Size)
{
  cudaMalloc(Input, Size*sizeof(float));
}




