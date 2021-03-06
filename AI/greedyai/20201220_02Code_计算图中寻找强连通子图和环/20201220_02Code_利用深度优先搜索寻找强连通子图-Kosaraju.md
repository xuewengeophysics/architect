# 利用深度优先搜索寻找强连通子图

## 1. 先备知识与注意事项

+ **强连通图**：在有向图中，**vi到vj和vj到vi都有路径**。如果一个有向图只有一个**强连通分量(Strongly Connected Component, SCC)**，则它是强连通的。
+ **如果一个有向图不是强连通的，那它一定有2个以上的强连通分量**。在一个directed graph中，若对于**任意两个**vertex(A)与vertex(B)之间，**不是同时存在**「从vertex(A)走到vertex(B)」以及「从vertex(B)走到vertex( A)」的path，那么此directed graph就不是强连通的(Strongly Connected, SC)，里面一定包含了**2个以上**的**strongly connected component(SCC)**。
+ **有向图的强连通分量是一个最大结点集合(强连通子图)，对于该集合中的任意一对结点u和v来说，路径u->v和v->u同时存在**；
+ **对于强连通子图，有一个特定的事实就是，该子图一定形成环，那么从该子图中任意点出发，总能回到出发点**；

![20201220_02Code_利用深度优先搜索寻找强连通子图-f1.png](https://github.com/xuewengeophysics/architect/blob/master/AI/greedyai/docs/20201220_02Code_%E5%88%A9%E7%94%A8%E6%B7%B1%E5%BA%A6%E4%BC%98%E5%85%88%E6%90%9C%E7%B4%A2%E5%AF%BB%E6%89%BE%E5%BC%BA%E8%BF%9E%E9%80%9A%E5%AD%90%E5%9B%BE-f1.png?raw=true)  

+ 如图一(a)，经由path:0-1-2-5，可以从vertex(0)走到vertex(5)，但是无论经过任何vertex，都没有办法从vertex(5)走到vertex(0 )，因此，图一(a)的directed graph并不是strongly connected，其中包含了两个以上的SCC(答案是三个)。

![20201220_02Code_利用深度优先搜索寻找强连通子图-f2.png](https://github.com/xuewengeophysics/architect/blob/master/AI/greedyai/docs/20201220_02Code_%E5%88%A9%E7%94%A8%E6%B7%B1%E5%BA%A6%E4%BC%98%E5%85%88%E6%90%9C%E7%B4%A2%E5%AF%BB%E6%89%BE%E5%BC%BA%E8%BF%9E%E9%80%9A%E5%AD%90%E5%9B%BE-f2.png?raw=true)  

+ **G与G T的SCC完全相同**。原因在于，观察G中包含在同一个SCC里的vertex(2)与vertex(3)。G中同时存在「从vertex(2)走到vertex(3)」的path，以及「从vertex(3)走到vertex(2)」的path。在G进行「Transpose」得到G T后，这两条path分别变成与原方向之相反方向，但是存在于vertex(2)与vertex(3)之间的**cycle**仍然存在（按我的理解就是**原路返回**了）。因此，在G里面属于同一个SCC的vertex，在G T里将形成相同的SCC。严谨证明见[参考资料](https://alrightchiu.github.io/SecondRound/graph-li-yong-dfsxun-zhao-strongly-connected-componentscc.html#ref)。



## 2. 只用一次DFS，行不行？

![20201220_02Code_利用深度优先搜索寻找强连通子图-f3.png](https://github.com/xuewengeophysics/architect/blob/master/AI/greedyai/docs/20201220_02Code_%E5%88%A9%E7%94%A8%E6%B7%B1%E5%BA%A6%E4%BC%98%E5%85%88%E6%90%9C%E7%B4%A2%E5%AF%BB%E6%89%BE%E5%BC%BA%E8%BF%9E%E9%80%9A%E5%AD%90%E5%9B%BE-f3.png?raw=true)  

+ 图二(a)，首先以vertex(8)作为`DFS()`的第一次起点，在搜寻完以vertex(8)作为`root`的Depth-First Tree后，再以vertex(5)作为新的起点。同样的，在搜寻完以vertex(5)作为`root`的Depth-First Tree后，再以vertex(3)作为新的起点。从图二(a)的「时间轴」可以看出，此次`DFS()`找到了一个Depth-First Forest，其中包含三棵Depth-First Tree，而这三棵Depth-First Tree分别就是Graph中的三个SCC 。
  
+ 上面的运气真好，一下子就找到了Graph中的三个SCC。但是问题真得解决了吗？通过这么一次`DFS()`就找到了directed graph中的SCC吗？
+ 再接着看图二(b)，以vertex(0)作为`DFS()`的起点。很遗憾，「时间轴」里形成了一整棵Depth-First Tree，directed graph中的三个SCC没有被分开。

![20201220_02Code_利用深度优先搜索寻找强连通子图-f4.png](https://github.com/xuewengeophysics/architect/blob/master/AI/greedyai/docs/20201220_02Code_%E5%88%A9%E7%94%A8%E6%B7%B1%E5%BA%A6%E4%BC%98%E5%85%88%E6%90%9C%E7%B4%A2%E5%AF%BB%E6%89%BE%E5%BC%BA%E8%BF%9E%E9%80%9A%E5%AD%90%E5%9B%BE-f4.png?raw=true)  



+ 由于SCC需要两个方向的path(「vertex(X)到vertex(Y)」和「vertex(Y)到vertex(X)」)都成立，但是`DFS()`只在意「单方向」的edge（**看到这里，是不是很佩服把G转置的idea**），只要存在edge(X,Y)，便把`predecessor[Y]`更新成vertex(X)，在Predecessor Subgraph里，vertex(X)与vertex(Y)便在同一棵Depth-First Tree中。**因此，只有一次`DFS()`的`predecessor`是不够的，图二(a)只是运气好**。



## 3. 演算法



若考虑具有多个SCC的directed graph，为了方便起见，定义其**Component Graph**为GSCC= (VSCC,ESCC)GSCC=(VSCC,ESCC)，其中：

- VSCCVSCC

  ：把每个SCC视为一个元素，并以该元素作为

  VSCCVSCC

  的vertex。

  - 例如图三(a)，令C1=C1={0 , 1 , 2 , 30,1,2,3}，C2=C2={4 , 54,5}，C3=C3={6 , 7 , 86,7,8}，则VSCC=VSCC={C1,C2,C3C1,C2,C3}。

- ESCCESCC

  ：考虑vertex(X)属于

  C1C1

  ，vertex(Y)属于

  C2C2

  ，若存在「连结两个不同SCC」的edge(X,Y)，则edge(X,Y)便属于

  ESCCESCC

  。

  - 以图三(a)为例，vertex(1)属于C1C1，vertex(4)属于C2C2，则edge(1,4)属于ESCCESCC，依此类推，便得到ESCC=ESCC={( 1 , 4 ) , ( 2 , 5 ) , ( 4 , 6 ) , ( 5 , 6 ) , ( 5 , 7 )(1,4),(2,5),(4,6),(5,6),(5,7)}。



## 4. 程序码

演算法分成4个步骤：

1. 对`G`执行`DFS()`；
2. 生成$G^T$；
3. 按照第一次`DFS()`所得到的**finish时间由大到小的顺序**选取去点，对$G^T$执行`DFS()`；
4. 从第二次`DFS()`的`predecessor`找到Predecessor Subgraph。若directed graph有多个SCC，那么Predecessor Subgraph就会是Depth-First Forest，其中的每一棵Depth-First Tree都是一个SCC。







## 参考资料

1. [Graph: 利用DFS寻找Strongly Connected Component(SCC)](https://alrightchiu.github.io/SecondRound/graph-li-yong-dfsxun-zhao-strongly-connected-componentscc.html)

