# Attention-Based Seq2Seq模型Tacotron2

## 1. 大纲

### 1.1 目录

+ Tacotron2模型整体结构。
+ attention技术回顾。
+ ️teacher forcing技术。️
+ GRU算法的TensorRT实现。

### 1.2 课前需要的知识点

+ Forward Attention
+ Location-sensitive Attention
+ Coverage Attention
+ HCLG模型结构

### 1.3 课前预习

+ 阅读Tacotron2论文：[Natural TTS Synthesis by Conditioning WaveNet on Mel Spectrogram Predictions](https://arxiv.org/abs/1712.05884)
+ 阅读论文：[Forward Attention in Sequence-to-sequence Acoustic Modelling for Speech Synthesis](https://arxiv.org/abs/1807.06736)

### 1.4 预习阅读材料

1.阅读材料1
️ a.地址：https://arxiv.org/pdf/2005.05642.pdf
️ b.题目：《AdaDurIAN: Few-shot Adaptation for Neural Text-to-Speech with DurIAN》
️ c.主要讲述内容及要求
腾讯TTS Ada架构升级，掌握State Expansion的方法，和phoneduration生成方法。
 阅读材料2
️ a.地址：https://docs.nvidia.com/cuda/cuda-c-programming-guide/index.html#cooperative-groups 
️ b.题目《 Appendix C. Cooperative Groups 》
️ c.主要讲述内容及要求
Cooperative Group的粒度定义和线程同步处理方法。
2.上周内容：
️ 《Neural Speech Synthesis with Transformer Network》
主要讲述内容：Transormer 在语音合成中的使用方法
资料地址：https://arxiv.org/abs/1809.08895
️ 《Attention-Based Models for Speech Recognition》
主要讲述内容：attention在语音识别模型中的使用方法
资料地址：https://papers.nips.cc/paper/2015/file/1068c6e4c8051cfd4e9ea8072e3189e2-Paper.pdf

## 2. 主要内容

+ 语音合成声学模型算法：
  + Tacotron
  + Tacotron2
  + FastSpeech
  + HighWay Network
  + GRU
  + DURIAN  
+ Attention算法：
  + Dot-Production Attention
  + Forward-Attention
  + LocationSensitive-Attention
  + Converage Attention  