# TensorRT Plugin插件开发

## 1. Plugin开发的基本流程

### 1.1 基本流程

1. 观察网络结构，确认算子的版本、名称、输入、输出、参数及相应权重；
2. 开发算子的解析模块Parser；
3. 插件的实现过程：继承IPluginV2Ext，完成Kernel及相应API的开发；
4. 实现Creator相关的操作。



## 2. nmsPlugin插件开发

### 2.1 定义插件DetectionOutput

1. 构造函数

   ```c++
   class DetectionOutput : public IPluginV2Ext
   {
   public:
       DetectionOutput(DetectionOutputParameters param);
   
       DetectionOutput(DetectionOutputParameters param, int C1, int C2, int numPriors);
   
       DetectionOutput(const void* data, size_t length);
   ```

   

2. terminate()

3. getNbOutputs()返回插件op返回对少个tensor

4. 初始化函数initialize

5. 返回结果的类型

6. getWorkspaceSize

7. enqueue：插件op的执行函数；也就是nms的执行；

8. getOutputDimensions：



### 2.2 NMSPluginCreator插件工厂类

1. 构造函数；
2. createPlugin：这个成员函数的作用是通过PluginFieldCollection去创建plugin，将op需要的权重和参数一个一个取出来，然后调用上文提到的第一个构造函数；
3. deserializePlugin：这个函数会被onnx-tensorrt的一个叫做TRT_PluginV2的转换op调用，这个op会读取onnx模型的data数据将其反序列化到network中；







## 3. 参考资料

1. 