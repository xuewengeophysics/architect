# 第3章 CUDA C简介

## 3.1 基础知识

+ **kernel(核函数)**：在GPU设备上执行的函数；
+ `__global__`修饰符：告诉编译器函数应该编译为在**设备(GPU)**上而不是**主机(CPU)**上运行；
+ CUDA编译器和运行时将负责实现从主机代码中调用设备代码；
+ 尖括号`<<<1,1>>>`表示将一些参数传递给运行时系统，告诉运行时如何启动设备代码；
+ 传递给设备代码本身的参数是放在圆括号`()`中传递的；
+ `cudaMalloc()`告诉CUDA运行时在设备上分配内存；
+ `dev_c`用于保存新分配内存地址；
+ `&c`代表目标指针，它是一个主机指针；
+ `dev_c`代表源指针，它是一个设备指针；
+ cudaMemcpyDeviceToHost告诉运行时源指针是一个设备指针，目标指针是一个主机指针；

```c
__global__ void add( int a, int b, int *c ) {
    *c = a + b;
}

int main( void ) {
    int c;
    int *dev_c;
    HANDLE_ERROR( cudaMalloc( (void**)&dev_c, sizeof(int) ) );

    add<<<1,1>>>( 2, 7, dev_c );

    HANDLE_ERROR( cudaMemcpy( &c, dev_c, sizeof(int),
                              cudaMemcpyDeviceToHost ) );
    printf( "2 + 7 = %d\n", c );
    HANDLE_ERROR( cudaFree( dev_c ) );

    return 0;
}
```



# 第4章 CUDA C并行编程

## 4.1 基于GPU的矢量求和

+ 主函数：

```c
#define N   10

int main( void ) {
    int a[N], b[N], c[N];
    int *dev_a, *dev_b, *dev_c;

    // 在GPU上分配内存
    HANDLE_ERROR( cudaMalloc( (void**)&dev_a, N * sizeof(int) ) );
    HANDLE_ERROR( cudaMalloc( (void**)&dev_b, N * sizeof(int) ) );
    HANDLE_ERROR( cudaMalloc( (void**)&dev_c, N * sizeof(int) ) );

    // 在CPU上为数组a和b赋值
    for (int i=0; i<N; i++) {
        a[i] = -i;
        b[i] = i * i;
    }

    // 将数组a和b复制到GPU
    HANDLE_ERROR( cudaMemcpy( dev_a, a, N * sizeof(int),
                              cudaMemcpyHostToDevice ) );
    HANDLE_ERROR( cudaMemcpy( dev_b, b, N * sizeof(int),
                              cudaMemcpyHostToDevice ) );

    add<<<N,1>>>( dev_a, dev_b, dev_c );

    // 将数组c从GPU复制到CPU
    HANDLE_ERROR( cudaMemcpy( c, dev_c, N * sizeof(int),
                              cudaMemcpyDeviceToHost ) );

    // 显示结果
    for (int i=0; i<N; i++) {
        printf( "%d + %d = %d\n", a[i], b[i], c[i] );
    }

    // 释放在GPU上分配的内存
    HANDLE_ERROR( cudaFree( dev_a ) );
    HANDLE_ERROR( cudaFree( dev_b ) );
    HANDLE_ERROR( cudaFree( dev_c ) );

    return 0;
}
```

+ 核函数：
  + blockIdx是一个内置变量，在CUDA运行时中已经预先定义了。

```c
__global__ void add( int *a, int *b, int *c ) {
    int tid = blockIdx.x;    // 计算该索引处的数据
    if (tid < N)
        c[tid] = a[tid] + b[tid];
}
```

