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

