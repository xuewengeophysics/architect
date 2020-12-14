# TensorRT推理

## 1. 简介

### 1.1 Why

+ 硬件支持，这是由**硬件特性**提供的便利性。TensorRT深度优化了推理的运行效率，它通过组合层和优化内核选择来优化网络，以改善延迟，吞吐量，能效和内存消耗：
  + 自动选取最优kernel：矩阵乘法、卷积有多种CUDA实现方式，根据数据大小和形状自动选取最优实现；
  + 计算图优化：通过kernel融合、减少数据拷贝等手段，生成网络的优化计算图；
  + 支持fp16/int8：对数值进行精度转换与缩放，充分利用硬件的低精度高通量计算能力；

### 1.2 What

+ GPU inference engine(引擎)

+ 是一种高性能深度学习推理优化器和运行时加速库

### 1.3 How

![image-20201214082605155](C:\Users\86138\AppData\Roaming\Typora\typora-user-images\image-20201214082605155.png) 

### 1.4 Where

+ TensorRT在部署阶段工作（works at deploy stage) 

### 1.5 生成路径

- 通过TensorFlow或MXNet内部集成的TRT 

  - 易于使用 
  - 未达到最佳效率 

  例子：pb -> uff -> plan -> perform inference on the GPU

- 从现有框架导出模型(ONNX)，再导入TRT 

  - 难度适中 
  - 兼容性不佳 

  例子：h5 -> onnx -> engine -> Perform inference on the GPU

- 使用TRT C++/Python API自行构造网络 

  - 兼容性最强，效率最高 
  - 难度最高

### 1.6 流程

```
- 创建Builder
- 创建Network
- 创建Parser
- 绑定输入 输出以及自定义组件
- 序列化和反序列化
- 传输计算数据（host->device）
- 执行计算
- 传输计算结果
```

### 1.7 组件

- **Network Definition**

  - 网络定义接口提供了一些方法来特别的定义一些网络。
  - Input and output tensors can be specified
  - layers can be added, and there is an interface for configuring each supported layer type.
  - a Plugin layer type allows the application to implement functionality not natively supported by TensorRT

- **Optimization Profile**

  - An optimization profile specifies constraints on dynamic dimensions

- **Builder Configuration**

  - The Builder Configuration interface specifies details for creating an engine. 
  - specify optimization profiles, 
  - maximum workspace size
  - the minimum acceptable level of precision
  - timing iteration counts for autotuning
  - and an interface for quantizing networks to run in 8-bit precision

- **Builder**

  - 创造一个引擎

  The Builder interface allows the creation of an optimized engine from a network definition and a builder configuration.

- **Engine** 

  - execute inference 执行推断. It supports synchronous and asynchronous execution, profiling, and enumeration and querying of the bindings for the engine inputs and outputs

- **Paeser ** 解析器 

  parsers for importing trained networks to create network definitions

  - **Caffe** **Parser**
  - **UFF Parser**
  - **ONNX** **Parser**



## 2. TensorRT 推理的整体API组织逻辑

### 2.1 两个阶段

+ **构建(build)**：加载模型和对网络配置进行优化，并生成一个优化了的plan用于计算深度神经网络的前向传播。这个plan是一个优化了的目标代码，可以序列化存储在内存或磁盘上；
+ **部署(deployment)**：部署阶段通常采用长时间运行的服务或用户应用程序的形式，该服务或用户应用程序接受批量输入数据，通过对输入数据执行plan来执行推理，并返回批量输出数据。

### 2.2 两个阶段的主要工作

#### 2.2.1 build阶段

+ build阶段主要完成：
  1. 模型转换：
     + caffe/Pytorch 模型转化 onnx
     + TensorFlow     模型转化 uff
  2. 模型优化：优化过程中的层间融合，精度校准
     + 权重与激活精度校准
     + 层与张量融合
     + ...

<img src="C:\Users\86138\AppData\Roaming\Typora\typora-user-images\image-20201214084323477.png" alt="image-20201214084323477" style="zoom: 80%;" />    



<img src="https://developer.nvidia.com/sites/default/files/akamai/deeplearning/tensorrt/trt-info.png" alt="img" style="zoom:80%;" />  

+ **build阶段的主要步骤**：
  1. ILogger
  2. IBuilder
  3. INetworkDefiniton
  4. IParser
  5. ICudaEngine
  6. serialize成IHostMemory

#### 2.2.2 deployment阶段

+ **runtime阶段的主要步骤**：
  1. IRuntime
  2. ICudaEngine
  3. IExecutionContext：执行的上下文环境；



## 3. TensorRT tensorflow uff模型的支持

![image-20201214090152481](C:\Users\86138\AppData\Roaming\Typora\typora-user-images\image-20201214090152481.png)  

+ **build阶段**：

  1. tf pb模型 转化 uff

     ```shell
     #方法1:
     	convert-to-uff xxxx.pb -o xxxx.uff
     	convert-to-uff --input-file frozen_inference_graph.pb -O NMS -p config.py
     #方法2:
     	uff.from_tensorflow_frozen_model(frozen_file, output_nodes=[], preprocessor=None, **kwargs)
     #方法3:
     	uff.from_tensorflow(graphdef, output_nodes=[], preprocessor=None, **kwargs)
     	uff.from_tensorflow(dynamic_graph.as_graph_def(), [ModelData.OUTPUT_NAME], output_filename=output_uff_path, text=True)
     
     ```

     

  2. uff 生成 优化文件

  3. INetworkDefiniton

+ **部署推理阶段**：
  1. IRuntime；
  2. ICudaEngine；
  3. IExecutionContext；



## 4. 参考资料

##### UFF

通用框架格式（UFF）是描述DNN（深度神经网络）的执行图的数据格式，并且从该执行图与其输入和输出进行绑定。 它有一个明确的核心语言，但也将支持对其核心操作和完全自定义操作的扩展。
格式包括：

- 序列化格式的具体语法，采用protobuf模式。
- 每个操作符的有效性定义，表示为一组python描述符。
- 记录每个核心操作的行为（以便将来交付）。



##### PLAN文件

PLAN文件是运行时引擎用来执行网络的序列化数据。 它包括权重，内核执行网络的时间表，以及应用程序可以查询的网络信息，以确定如何绑定输入和输出缓冲区。
工作流程图：
	用户通过训练数据模型来生成训练有素的网络。 训练好的网络可以用于推断。
	PLAN也可以被序列化到磁盘，以便稍后重新加载到TensorRT运行时，而不必再次执行优化步骤。



##### ONNX

Open Neural Network Exchange（ONNX，开放神经网络交换）格式，是一个用于表示深度学习模型的标准，可使模型在不同框架之间进行转移。

> ONNX是一种针对机器学习所设计的开放式的文件格式，用于存储训练好的模型。它使得不同的人工智能框架（如Pytorch, MXNet）可以采用相同格式存储模型数据并交互

项目地址： https://github.com/onnx/tutorials.git



#### onnx2trt

​	解析onnx模型到tensorrt: Parses ONNX models for execution with TensorRT

​	项目地址： https://github.com/onnx/onnx-tensorrt.git 



#### tensorflow-onnx

​    Convert TensorFlow models to ONNX

​    项目地址： https://github.com/onnx/tensorflow-onnx.git