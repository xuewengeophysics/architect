/*
 * Copyright (c) 2020, NVIDIA CORPORATION. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef TRT_NMS_PLUGIN_H
#define TRT_NMS_PLUGIN_H
#include "kernel.h"
#include "nmsUtils.h"
#include "plugin.h"
#include <string>
#include <vector>

using namespace nvinfer1::plugin;
namespace nvinfer1
{
namespace plugin
{

// 插件
class DetectionOutput : public IPluginV2Ext
{
public:
    // 3个构造函数
    DetectionOutput(DetectionOutputParameters param);

    DetectionOutput(DetectionOutputParameters param, int C1, int C2, int numPriors);

    DetectionOutput(const void* data, size_t length);

    ~DetectionOutput() override = default;

    // 返回插件op返回多少个Tensor
    int getNbOutputs() const override;

    // TensorRT支持Dynamic-shape的时候，batch这个维度必须是explicit的，
    // 也就是说，TensorRT处理的维度从以往的三维[3,-1,-1]变成了[1,3,-1,-1]。
    // 最新的onnx-tensorrt也必须设置explicit的batchsize，
    // 而且这个batch的维度在getOutputDimensions中是可以获取到的。
    Dims getOutputDimensions(int index, const Dims* inputs, int nbInputDims) override;

    // 初始化函数
    int initialize() override;

    void terminate() override;

    // 返回这个插件op需要的中间显存变量的实际数据大小(bytesize)，
    // 这个是通过TensorRT的接口去获取的，是比较规范的方式
    size_t getWorkspaceSize(int maxBatchSize) const override;

    // 插件op的执行函数
    int enqueue(
        int batchSize, const void* const* inputs, void** outputs, void* workspace, cudaStream_t stream) override;

    // 返回序列化时需要写多少字节到buffer中
    size_t getSerializationSize() const override;

    // 把需要用的数据按照顺序序列化到buffer里
    void serialize(void* buffer) const override;

    // supportsFormatCombination：TensorRT调用此方法以判断pos索引的输入/输出是否支持
    // inOut[pos].format和inOut[pos].type指定的格式/数据类型
    bool supportsFormat(DataType type, PluginFormat format) const override;

    const char* getPluginType() const override;

    const char* getPluginVersion() const override;

    void destroy() override;

    // 将这个plugin对象克隆一份给TensorRT的builder、network或者engine
    IPluginV2Ext* clone() const override;

    // 设置插件命名空间
    void setPluginNamespace(const char* pluginNamespace) override;

    // 设置插件命名空间
    const char* getPluginNamespace() const override;

    // 一般来说我们插件op返回结果类型与输入类型一致
    DataType getOutputDataType(int index, const nvinfer1::DataType* inputTypes, int nbInputs) const override;

    bool isOutputBroadcastAcrossBatch(int outputIndex, const bool* inputIsBroadcasted, int nbInputs) const override;

    bool canBroadcastInputAcrossBatch(int inputIndex) const override;

    // 如果这个op使用到了一些其它东西，例如cublas handle，可以直接借助TensorRT内部提供的cublas handle
    void attachToContext(
        cudnnContext* cudnnContext, cublasContext* cublasContext, IGpuAllocator* gpuAllocator) override;

    // 配置这个插件op，判断输入和输出类型数量是否正确。
    // 官方还提到通过这个配置信息可以告知TensorRT去选择合适的算法去调优这个模型
    void configurePlugin(const Dims* inputDims, int nbInputs, const Dims* outputDims, int nbOutputs,
        const DataType* inputTypes, const DataType* outputTypes, const bool* inputIsBroadcast,
        const bool* outputIsBroadcast, PluginFormat floatFormat, int maxBatchSize) override;

    void detachFromContext() override;

private:
    DetectionOutputParameters param;
    int C1, C2, numPriors;
    std::string mPluginNamespace;
};

// 插件工厂类
class NMSPluginCreator : public BaseCreator
{
public:
    // 构造函数
    NMSPluginCreator();

    ~NMSPluginCreator() override = default;

    const char* getPluginName() const override;

    const char* getPluginVersion() const override;

    // PluginFieldCollection是成员变量，也会作为getFieldNames成员函数的返回类型，
    // PluginFieldCollection的主要作用是传递这个插件op所需要的权重和参数，
    // 在实际的engine推理过程中并不使用，而在parse中会用到(例如caffe2trt、onnx2trt)
    const PluginFieldCollection* getFieldNames() override;

    // 这个成员函数的作用是通过PluginFieldCollection去创建plugin，
    // 将op需要的权重和参数一个一个取出来，然后调用上文提到的第一个构造函数
    IPluginV2Ext* createPlugin(const char* name, const PluginFieldCollection* fc) override;

    // 这个函数会被onnx-tensorrt的一个叫做TRT_PluginV2的转换op调用，
    // 这个op会读取onnx模型的data数据，将其反序列化到network中
    IPluginV2Ext* deserializePlugin(const char* name, const void* serialData, size_t serialLength) override;

private:
    static PluginFieldCollection mFC;

    // Parameters for DetectionOutput
    DetectionOutputParameters params;
    static std::vector<PluginField> mPluginAttributes;
};

} // namespace plugin
} // namespace nvinfer1

#endif // TRT_NMS_PLUGIN_H
