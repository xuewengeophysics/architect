// C++ code
#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <iomanip>      // for std::setw()

class Graph {
private:
	int num_vertex;
	std::vector< std::list<int> > AdjList;
	int *color,             // 0:white, 1:gray, 2:black
		*predecessor,
		*discover,
		*finish;
public:
	Graph() :num_vertex(0) {};
	Graph(int N) :num_vertex(N) {
		// initialize Adj List
		AdjList.resize(num_vertex);
	};
	void AddEdgeList(int from, int to);
	void DFS(int Start);
	void DFSVisit(int vertex, int &time);
	void PrintPredecessor();
	void PrintDiscover();
	void PrintFinish();
};

void Graph::AddEdgeList(int from, int to) {
	AdjList[from].push_back(to);
}


void Graph::DFS(int Start) {
	color = new int[num_vertex];           // 配置记忆体位置
	discover = new int[num_vertex];
	finish = new int[num_vertex];
	predecessor = new int[num_vertex];

	int time = 0;                          // 初始化
	for (int i = 0; i < num_vertex; i++) {
		color[i] = 0;
		discover[i] = 0;
		finish[i] = 0;
		predecessor[i] = -1;
	}

	int i = Start;
	for (int j = 0; j < num_vertex; j++) { // 检查所有Graph中的vertex都要被搜寻到
		if (color[i] == 0) {               // 若vertex是白色，则进行以该vertex作为起点之搜寻
			DFSVisit(i, time);
		}
		i = j;                             // j会把AdjList完整走过一遍，确保所有vertex都被搜寻过
	}
}

void Graph::DFSVisit(int vertex, int &time) { // 一旦有vertex被发现而且是白色，便进入DFSVisit()
	color[vertex] = 1;                        // 把vertex涂成灰色
	discover[vertex] = ++time;                // 更新vertex的discover时间
	for (std::list<int>::iterator itr = AdjList[vertex].begin(); itr != AdjList[vertex].end(); itr++) {
		if (color[*itr] == 0) {               // 若搜寻到白色的vertex
			predecessor[*itr] = vertex;       // 更新其predecessor
			DFSVisit(*itr, time);             // 立刻以其作为新的搜寻起点，进入新的DFSVisit()
		}
	}
	color[vertex] = 2;                        // 当vertex已经搜寻过所有与之相连的vertex后,将其涂黑
	finish[vertex] = ++time;                  // 并更新finish时间
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

void Graph::PrintDiscover() {
	std::cout << "discover:" << std::endl;
	for (int i = 0; i < num_vertex; i++)
		std::cout << std::setw(4) << i;
	std::cout << std::endl;
	for (int i = 0; i < num_vertex; i++)
		std::cout << std::setw(4) << discover[i];
	std::cout << std::endl;
}

void Graph::PrintFinish() {
	std::cout << "finish:" << std::endl;
	for (int i = 0; i < num_vertex; i++)
		std::cout << std::setw(4) << i;
	std::cout << std::endl;
	for (int i = 0; i < num_vertex; i++)
		std::cout << std::setw(4) << finish[i];
	std::cout << std::endl;
}

int main() {
	// 定义一个有8个vertex的graph
	Graph g2(8);
	// 建立如图之Graph
	g2.AddEdgeList(0, 1); g2.AddEdgeList(0, 2);
	g2.AddEdgeList(1, 3);
	g2.AddEdgeList(2, 1); g2.AddEdgeList(2, 5);
	g2.AddEdgeList(3, 4); g2.AddEdgeList(3, 5);
	// AdjList[4] is empty
	g2.AddEdgeList(5, 1);
	g2.AddEdgeList(6, 4); g2.AddEdgeList(6, 7);
	g2.AddEdgeList(7, 6);

	g2.DFS(0);    // 以vertex(0),也就是vertex(A作为DFS()的起点
	g2.PrintPredecessor();
	g2.PrintDiscover();
	g2.PrintFinish();

	return 0;
}
