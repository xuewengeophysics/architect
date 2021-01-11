# Breadth-First Search(BFS, 广度优先搜索)

## 1. 先备知识与注意事项

+ 广度优先搜索(BFS)则是广义的Level-Order遍历

![广度优先搜索-f1.png](https://github.com/xuewengeophysics/architect/blob/master/AI/greedyai/docs/%E5%B9%BF%E5%BA%A6%E4%BC%98%E5%85%88%E6%90%9C%E7%B4%A2-f1.png?raw=true)  



## 2. 演算法

+ 四项武器：
  1. `queue`：一级关卡遍历，`BFS()`将使用`queue`来确保“**先被搜寻到的顶点，就先成为新的搜寻起点**”；
  2. `color` array：利用颜色标记的顶点已经被“找到”了，有些还没有；
     + 白色表示该顶点还没有被找过；
     + 灰色表示该顶点已经被某个顶点找过；
     + 黑色表示该顶点已经从`queue`的队伍中移除了(**该顶点所有的邻接点都被找过**)。
  3. `distance` array：记录每一个顶点与起点顶点之距离；
  4. `predecessor` array：记录某个顶点是被哪一个顶点找到的，如此便能回溯路径。



+ `BFS()`的方法如下：

  1. 初始化(initialization)：

     + 把所有顶点涂成白色，表示没有任何顶点被“找到”；
     + 把所有vertex的`distance`设为无限大，表示从起点vertex走不到，或者还没有走到；
     + 把所有顶点的`predecessor`清除(或者设成`NULL`，`-1`，可以辨识出何者为「起点」即可)；
     + 建立空的`queue`。

  2. 将起点vertex(A)推进`queue`：

     + 将顶点(A)涂成灰色，表示顶点(A)在之后的`BFS()`过程中，将不可能再被其他顶点“找到”；
     + `distance[A]`设为0。补充，`distance`为0的顶点就是在“一个连接的组件”上进行`BFS()`的起点；
     + `predecessor[A]`不变。若将初始化`predecessor`成`-1`，即表示，在`BFS()`结束后，`predecessor`仍为`-1`的顶点即为起点。

  3. 接着，以`queue`的`front`当作新的起点搜寻。新的起点是顶点(A)，便检查所有与顶点(A)相邻的顶点，修改其`color`，`distance`，`predecessor`。如图所示，顶点(B)、顶点(C)、顶点(D)与顶点(A)相邻，如果顶点的颜色是白色，表示没有被其他顶点“找到”，便执行以下步骤：

     - 将三个顶点的`color`涂成灰色；
     - 将`distance[B]、distance[C]、distance[D]`设成`distance[A]+1=2`；
     - 将`predecessor[B]、predecessor[C]、predecessor[D]`设成顶点(A)；
     - 把三个顶点按照“找到”的顺序，依序推进`queue`里。
     - 最后，把vertex(A)涂黑，移出`queue`。

     经过以上步骤，顶点(B)，顶点(C)，顶点(D)便被顶点(A)“找到”，并把`predecessor`设成成顶点(A)，所以回溯路径时，任何经过顶点(B)的路径，必定是由顶点(A)来。同理，顶点(C)与顶点(D)也是。

  4. 接着，以继续`queue`的`front`当作新的起点搜寻，重复上述步骤。当`queue`中的顶点都被移除(pop())表示`Adjacency List`中的所有顶点被都当作起点搜寻过其相邻的顶点，此时`BFS()`便完成，得到以顶点(A)为起点，所有其余顶点与之相对应的`distance`与`predecessor`。

## 3. 程式码

## 4. 参考资料

1. [Graph: Breadth-First Search(BFS, 广度优先搜索)](https://alrightchiu.github.io/SecondRound/graph-breadth-first-searchbfsguang-du-you-xian-sou-xun.html)