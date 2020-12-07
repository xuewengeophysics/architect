# TensorRT安装

+ 下载地址：https://developer.nvidia.com/nvidia-tensorrt-download；

+ 安装步骤：

  1. 解压tar文件：

     ```shell
     tar xzvf TensorRT-${version}.${os}.${arch}-gnu.${cuda}.${cudnn}.tar.gz
     ```

     ```
     version="7.x.x.x"
     os="<os>"
     arch=$(uname -m)
     cuda="cuda-x.x"
     cudnn="cudnn8.x"
     ```

     ![image-20201207163352489](C:\Users\86138\AppData\Roaming\Typora\typora-user-images\image-20201207163352489.png)  

  2. 添加环境变量，在`.bashrc`中添加：

     ```shell
     export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:<TensorRT-${version}/lib>
     ```

     

  3. 安装Python TensorRT wheel：

     ```shell
     cd TensorRT-${version}/python
     pip install tensorrt-*-cp3x-none-linux_x86_64.whl
     ```

     

  4. 安装Python UFF wheel：

     ```shell
     cd TensorRT-${version}/uff
     pip install uff-0.6.9-py2.py3-none-any.whl
     ```

     

  5. 安装Python graphsurgeon wheel：

     ```shell
     cd TensorRT-${version}/graphsurgeon
     pip install graphsurgeon-0.4.5-py2.py3-none-any.whl
     ```

     

  6. 安装Python onnx-graphsurgeon wheel：

     ```shell
     cd TensorRT-${version}/onnx_graphsurgeon
     pip install onnx_graphsurgeon-0.2.6-py2.py3-none-any.whl
     ```

     