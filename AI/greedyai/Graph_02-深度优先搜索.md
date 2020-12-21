# Depth-First Search(DFS, 深度优先搜索)

## 1. 先备知识与注意事项

+ DFS的核心精神便如同Pre-Order Traversal：「先遇到的vertex就先Visiting」，并且以先遇到的vertex作为新的搜寻起点，直到所有「有edge相连的vertex」都被探索过。

![深度优先搜索-f1.png](https://github.com/xuewengeophysics/architect/blob/master/AI/greedyai/docs/%E6%B7%B1%E5%BA%A6%E4%BC%98%E5%85%88%E6%90%9C%E7%B4%A2-f1.png?raw=true)  



## 2. 演算法

+ 四项武器：
  1. `time`：在整个`DFS()`的过程会有一条「时间轴」，若Graph中有N个vertex，「时间轴」上一共会有2N个「时间点」；
  2. `discover`与`finish` array：每个vertex会被标记上两个「时间点」，分别是「被发现(`discover`)」的时间与「结束(`finish`)」的时间：
     - `discover`：例如，vertex(B)被vertex(A)找到，则`discover[B]`会是`discover[A]`加一，表示vertex(B)在整个时间轴上是在vertex(A)之后被找到(这其中便存在「ancestor-descendant」的关系)。
     - `finish`：若vertex(B)已经经由有效edge探索过所有与之相连的vertex，表示以vertex(B)为起点的探索已经结束，便标上`finish[B]`。
  3. `color` array：利用color标记哪些vertex已经「被发现」与「结束」。
     - 白色表示该vertex还没有「被发现」；
     - 灰色表示该vertex已经「被发现」，但是还没有「结束」；
     - 黑色表示该vertex已经「结束」。
  4. `predecessor` array：记录某个顶点是被哪一个顶点找到的，如此便能回溯路径(如同`BFS()`，`DFS()`亦能生成一个**Predecessor Subgraph**)。



![深度优先搜索-f19.png](https://github.com/xuewengeophysics/architect/blob/master/AI/greedyai/docs/%E6%B7%B1%E5%BA%A6%E4%BC%98%E5%85%88%E6%90%9C%E7%B4%A2-f19.png?raw=true)  

+ `DFS()`的方法如下：

  1. 初始化(initialization)：
     + 先把`time`设为`0`，表示还没有任何vertex「被发现」；
     + 由于vertex(A)已经「被发现」，便把`discover[A]`设为`++time`。原本`time=0`，而vertex(A)是`DFS()`的起点，所以`++time`之后`distance[A]=1`便表示vertex(A)是第一个被发现；
     + 把所有vertex的`predecessor`清除(或者设成`NULL`、`-1`，视资料形态(data type)而定)；
     + 把所有vertex的`discover`与`finish`设成`0`，表示还没有开始进行`DFS()`。
  2. 以vertex(A)作为起点：
     + 将vertex(A)涂成灰色，表示已经「被发现」；
     + 由于vertex(A)已经「被发现」，便把`discover[A]`设为`++time`。原本`time=0`，而vertex(A)是`DFS()`的起点，所以`++time`之后`discover[A]=1`便表示vertex(A)是第一个被发现；
     + 接着寻找所有与vertex(A)相连之vertex，只要遇到第一个**仍为白色**的vertex，便把该vertex设为新的搜寻起点，并将该vertex之`predecessor`设为vertex(A)：
       + 第一个与vertex(A)相连的vertex为vertex(B)，接着便**以vertex(B)为起点**，继续寻找与vertex(B)相连之「最近的」vertex；
     + 由于从vertex(A)找到了vertex(B)，便表示「从vertex(A)出发的path」还在更新，于是vertex(A)便还没有「结束」，所以`finish[A]`不需要更新：
       + 那么，什么时候会更新`finish[A]`呢？在`Adjacency List`中，与vertex(A)相连之vertex有vertex(B)与vertex(C)，要在这两个vertex都「作为搜寻起点」，并且「探索完所有相连的vertex」后(也就是更新完`finish[B]`与`finish[C]`后)，才会更新到`finish[A]`。
  3. 从vertex(A)作为探索起点，「最先发现」的是vertex(B)，便以其作为新的起点：
     - 把vertex(B)涂成灰色，表示已经「被发现」；
     - 由于vertex(B)已经「被发现」，便把`discover[B]`设为`++time`，也就是`discover[B]=2`；
     - 接着，找到`Adjacency List`中第一个与vertex(B)相连，且为**白色**的vertex(D)，将vertex(D)视为新的搜寻起点。
  4. 来到「以vertex(D)作为起点」之搜寻，大致上与「以vertex(B)作为起点」之搜寻相同：
     + 把vertex(D)涂成灰色，表示已经「被发现」；
     + 由于vertex(D)已经「被发现」，便把`discover[D]`设为`++time`，也就是`discover[D]=3`；
     + 接着，找到`Adjacency List`中第一个与vertex(D)相连，且为**白色**的vertex(E)，将vertex(E)视为新的搜寻起点。
  5. 来到「以vertex(E)作为起点」之搜寻，与前两个步骤相同，`discover[E]=4`，不再赘述；
  6. 当vertex(E)再也找不到任何「能够抵达、且为白色」的vertex时，就表示「以vertex (E)作为起点」之搜寻已经「结束」，此时：
     + 把vertex(E)涂成黑色，表示「以vertex(E)作为起点」之搜寻已经「结束」；
     + 把`finish[E]`设成`++time`；原先在vertex(E)「被发现」时`time=4`，更新后，表示vertex(E)之搜寻在`time=5`时「结束」；
     + 当vertex(E)「结束」后，便回到vertex(E)的`predecessor`，也就是「发现vertex(E)」的vertex(D)，继续探索其他vertex(D)能够走到的vertex；
     + 对于vertex(E)，`discover[E]=4`，`finish[E]=5`，往后搜寻过程就不会再有vertex(E)出现。而以vertex(A)、vertex(B)、vertex(D)作为起点的搜寻则持续进行。
  7. 接着，从vertex(E)退回到vertex(D)，并从vertex(D)继续发现vertex(F)还是白色，于是便以「vertex(F)作为起点」进行搜寻：
     + 把vertex(F)涂成灰色；
     + 把`discover[F]`设为`++time`；
     + 继续寻找所有从vertex(F)能够走到，且为白色的vertex。
  8. vertex(F)有邻接点vertex(B)，从vertex(F)能够走到vertex(B)，但是由于vertex(B)已经是「灰色」，表示「以vertex(B)为起点」之搜寻已经开始且尚未结束，于是vertex(F)无法「发现」vertex(B)，也无法走到其余vertex，所以，vertex(F)便宣告「结束」：
     - 把vertex(F)涂成黑色，表示「以vertex(F)作为起点」之搜寻已经「结束」。
     - 把`finish[F]`设成`++time`。
     - 当vertex(F)「结束」后，便回到vertex(F)的`predecessor`，也就是「发现vertex(F)」的vertex(D)，继续探索其他vertex(D)能够走到的vertex。
  9. 所有vertex(D)能够抵达的vertex都已经变成黑色，于是「以vertex(D)作为起点」之搜寻便结束：
     + 把vertex(D)涂成黑色，表示「以vertex(D)作为起点」之搜寻已经「结束」；
     + 把`finish[D]`设成`++time`。
     + 当vertex(D)「结束」后，便回到vertex(D)的`predecessor`，也就是「发现vertex(D)」的vertex(B)，继续探索其他vertex(B)能够走到的vertex。
  10. 接着便以上述逻辑重复步骤，直到vertex(A)被涂成黑色。
  11. 在「以vertex(A)为起点」之搜寻结束后，必须确认Graph中还有没有白色的vertex。Graph里还有vertex(G)与vertex(H)仍然是白色，因此挑选其中一个vertex作为新的起点。这里是按照`Adjacency List`的顺序，由于vertex(G)在vertex(H)之前，便先挑选vertex(G)，接着重复上述步骤。
  12. 当Graph中所有vertex都被涂成黑色，便完成`DFS()`。

  

## 3. 程式码

`DFS()`本质上是一种「**递归(recursion)结构**」，递归的结束条件是**某个vertex能够抵达的所有的vertex都已经变成黑色**。



## 4.参考资料

1. [Graph: Depth-First Search(DFS, 深度优先搜索)](https://alrightchiu.github.io/SecondRound/graph-depth-first-searchdfsshen-du-you-xian-sou-xun.html)