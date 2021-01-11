// C++ code
#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <iomanip>      // for std::setw()

using namespace std;

class Graph {
private:
	int num_vertex;
	std::vector< std::list<int> > AdjList;
	int *color,             // 0:白色, 1:灰色, 2:黑色
		*distance,          // 0:起点, 无限大:从起点走不到的顶点
		*predecessor;       // -1:沒有predecessor, 表示为起点vertex
public:
	Graph() :num_vertex(0) {};           // 默认构造函数
	Graph(int N) :num_vertex(N) {        // 带输入的构造函数:顶点数
		// 初始化邻接表
		AdjList.resize(num_vertex);
	};
	void AddEdgeList(int from, int to);
	void BFS(int Start);
	void PrintPredecessor();               // 打印predecessor, 供验证用, 非必要
	void PrintDistance();               // 印出distance, 供验证用, 非必要
};

void Graph::AddEdgeList(int from, int to) {
	AdjList[from].push_back(to);
}

void Graph::BFS(int Start) {

	color = new int[num_vertex];
	predecessor = new int[num_vertex];
	distance = new int[num_vertex];

	for (int i = 0; i < num_vertex; i++) {  // 初始化，如图二(b)
		color[i] = 0;                       // 0:白色;
		predecessor[i] = -1;                // -1表示沒有predecessor
		distance[i] = num_vertex + 1;       // num_vertex个vertex, 
	}                                       // 最长距离 distance = num_vertex-1条edge

	std::queue<int> q;
	int i = Start;

	for (int j = 0; j < num_vertex; j++) {  // j从0数到num_vertex-1, 因此j会走过graph中所有vertex
		if (color[i] == 0) {                // 第一次i会是起点vertex, 如图二(c)
			color[i] = 1;                   // 1:灰色
			distance[i] = 0;                // 每一个connected component的起点之距离设成0
			predecessor[i] = -1;            // 每一个connected component的起点沒有predecessor
			q.push(i);
			while (!q.empty()) {
				int u = q.front();                  // u为新的搜索起点
				for (std::list<int>::iterator itr = AdjList[u].begin(); itr != AdjList[u].end(); itr++) {
					if (color[*itr] == 0) {                // 若被「找到」的vertex是白色
						std::cout << "起点 = " << u << ", itr = " << *itr << endl;
						color[*itr] = 1;                   // 涂成灰色, 表示已经被「找到」
						distance[*itr] = distance[u] + 1;  // 距离是predecessor之距离加1
						predecessor[*itr] = u;             // 更新被「找到」的vertex的predecessor
						q.push(*itr);                      // 把vertex推进queue
					}
				}
				q.pop();        // 把u移出queue
				color[u] = 2;   // 并且把u涂成黑色
			}
		}
		// 若一次回圈沒有把所有vertex走过, 表示graph有多个connected component
		// 就把i另成j, 继续检查graph中的其他vertex是否仍是白色, 若是, 重复while loop
		i = j;
	}
}

void Graph::PrintPredecessor() {
	std::cout << "predecessor:" << std::endl;
	for (int i = 0; i < num_vertex; i++)
		std::cout << std::setw(4) << i;
	std::cout << std::endl;
	for (int i = 0; i < num_vertex; i++)
		std::cout << std::setw(4) << predecessor[i];
	std::cout << std::endl;
}

void Graph::PrintDistance() {
	std::cout << "distance:" << std::endl;
	for (int i = 0; i < num_vertex; i++)
		std::cout << std::setw(4) << i;
	std::cout << std::endl;
	for (int i = 0; i < num_vertex; i++)
		std::cout << std::setw(4) << distance[i];
	std::cout << std::endl;
}

int main() {
	Graph g1(9);
	// 建立出图二(a)的Adjacency List
	g1.AddEdgeList(0, 1); g1.AddEdgeList(0, 2); g1.AddEdgeList(0, 3);
	g1.AddEdgeList(1, 0); g1.AddEdgeList(1, 4);
	g1.AddEdgeList(2, 0); g1.AddEdgeList(2, 4); g1.AddEdgeList(2, 5); g1.AddEdgeList(2, 6); g1.AddEdgeList(2, 7);
	g1.AddEdgeList(3, 0); g1.AddEdgeList(3, 7);
	g1.AddEdgeList(4, 1); g1.AddEdgeList(4, 2); g1.AddEdgeList(4, 5);
	g1.AddEdgeList(5, 2); g1.AddEdgeList(5, 4); g1.AddEdgeList(5, 8);
	g1.AddEdgeList(6, 2); g1.AddEdgeList(6, 7); g1.AddEdgeList(6, 8);
	g1.AddEdgeList(7, 2); g1.AddEdgeList(7, 3); g1.AddEdgeList(7, 6);
	g1.AddEdgeList(8, 5); g1.AddEdgeList(8, 6);

	g1.BFS(0);

	g1.PrintPredecessor();
	g1.PrintDistance();


	return 0;
}