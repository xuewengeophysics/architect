# CUDA Programming Guide



围绕着CUDA官方的编程指南，然后结合着NV官方提供的thrust，cub这些开源库内核级实现，进行并行编程范例的进一步扩展。

翻译：找一些针对性的，普通书籍会漏掉的一些部分。

拓展：结合着典型的优化实例来拓展。比如warp-ctc这样的并行的解码方案。



reduction.pdf 关于reduction优化讲的很透彻的文档

|    时间    |                             内容                             |                             简介                             | 认领人 |  时间节点  | 备注 |
| :--------: | :----------------------------------------------------------: | :----------------------------------------------------------: | :----: | :--------: | :--: |
| 2021-03-09 | https://developer.nvidia.com/blog/gpu-pro-tip-cuda-7-streams-simplify-concurrency/ |        对default stream使用可能造成问题的一个详细说明        | 章长安 | 2021-03-13 |      |
| 2021-03-11 | https://moocaholic.medium.com/fp64-fp32-fp16-bfloat16-tf32-and-other-members-of-the-zoo-a1ca7897d407 | 这个是基础，数据类型的。在做框架的时候，溢出，计算效率，精度 这些都需要平衡。所以要熟练掌握以下这几种数据类型。 |        |            |      |
|            |                                                              |                                                              |        |            |      |





+ Kernel 1: interleaved addressing with divergent branching
+ Kernel 2: interleaved addressing with bank conflicts
+ Kernel 3: sequential addressing
+ Kernel 4: first add during global load
+ Kernel 5: unroll last warp
+ Kernel 6: completely unrolled
+ Kernel 7: multiple elements per thread