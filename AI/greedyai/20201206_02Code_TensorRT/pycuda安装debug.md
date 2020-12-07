# pycuda安装debug

+ 安装pucuda时，gcc报错：`Fatal error: cuda.h, No such file or directory`。解决办法是在`~/.bashrc`中设置环境变量

```shell
export CUDA_HOME=/usr/local/cuda
export PATH=$CUDA_HOME/bin:$PATH
export LD_LIBRARY_PATH=$CUDA_HOME/lib64:$LD_LIBRARY_PATH
```





## 参考资料

1. [Fatal error: cuda.h, No such file or directory](https://blog.csdn.net/ecnu_lzj/article/details/104475098)