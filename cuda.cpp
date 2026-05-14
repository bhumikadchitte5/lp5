%%writefile matrix_add.cu
#include<iostream>
#include<cuda.h>

using namespace std;

// CUDA Kernel
__global__ void matrixAdd(int *A, int *B, int *C, int rows, int cols)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    int index = row * cols + col;

    if(row < rows && col < cols)
    {
        C[index] = A[index] + B[index];
    }
}

int main()
{
    int rows = 4;
    int cols = 4;

    int size = rows * cols * sizeof(int);

    int h_A[16], h_B[16], h_C[16];

    // Initialize matrices
    for(int i = 0; i < 16; i++)
    {
        h_A[i] = i + 1;
        h_B[i] = 2;
    }

    int *d_A, *d_B, *d_C;

    cudaMalloc((void**)&d_A, size);
    cudaMalloc((void**)&d_B, size);
    cudaMalloc((void**)&d_C, size);

    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    dim3 threadsPerBlock(2,2);
    dim3 blocksPerGrid((cols+1)/2,(rows+1)/2);

    matrixAdd<<<blocksPerGrid, threadsPerBlock>>>(d_A,d_B,d_C,rows,cols);

    cudaMemcpy(h_C,d_C,size,cudaMemcpyDeviceToHost);

    cout << "Result Matrix:\n";

    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
            cout << h_C[i*cols+j] << " ";
        }
        cout << endl;
    }

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}








%%writefile matrix_mul.cu
#include<iostream>
#include<cuda.h>

using namespace std;

// CUDA Kernel for Matrix Multiplication
__global__ void matrixMul(int *A, int *B, int *C, int rowsA, int colsA, int colsB)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if(row < rowsA && col < colsB)
    {
        int sum = 0;

        for(int k = 0; k < colsA; k++)
        {
            sum += A[row * colsA + k] * B[k * colsB + col];
        }

        C[row * colsB + col] = sum;
    }
}

int main()
{
    // Matrix dimensions
    int rowsA = 2;
    int colsA = 2;
    int rowsB = 2;
    int colsB = 2;

    int sizeA = rowsA * colsA * sizeof(int);
    int sizeB = rowsB * colsB * sizeof(int);
    int sizeC = rowsA * colsB * sizeof(int);

    // Host matrices
    int h_A[] = {1, 2,
                 3, 4};

    int h_B[] = {5, 6,
                 7, 8};

    int h_C[4];

    // Device matrices
    int *d_A, *d_B, *d_C;

    cudaMalloc((void**)&d_A, sizeA);
    cudaMalloc((void**)&d_B, sizeB);
    cudaMalloc((void**)&d_C, sizeC);

    // Copy matrices to GPU
    cudaMemcpy(d_A, h_A, sizeA, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, sizeB, cudaMemcpyHostToDevice);

    // Block and Grid dimensions
    dim3 threadsPerBlock(2,2);
    dim3 blocksPerGrid(1,1);

    // Launch Kernel
    matrixMul<<<blocksPerGrid, threadsPerBlock>>>
    (d_A, d_B, d_C, rowsA, colsA, colsB);

    // Copy result back to CPU
    cudaMemcpy(h_C, d_C, sizeC, cudaMemcpyDeviceToHost);

    // Display Matrix A
    cout << "Matrix A:\n";

    for(int i = 0; i < rowsA; i++)
    {
        for(int j = 0; j < colsA; j++)
        {
            cout << h_A[i * colsA + j] << " ";
        }
        cout << endl;
    }

    // Display Matrix B
    cout << "\nMatrix B:\n";

    for(int i = 0; i < rowsB; i++)
    {
        for(int j = 0; j < colsB; j++)
        {
            cout << h_B[i * colsB + j] << " ";
        }
        cout << endl;
    }

    // Display Result Matrix
    cout << "\nMatrix C (A x B):\n";

    for(int i = 0; i < rowsA; i++)
    {
        for(int j = 0; j < colsB; j++)
        {
            cout << h_C[i * colsB + j] << " ";
        }
        cout << endl;
    }

    // Free GPU memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;
}