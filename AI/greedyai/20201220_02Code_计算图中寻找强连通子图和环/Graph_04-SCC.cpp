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
	int *color,             // 0:white, 1:gray, 2:black
		*predecessor,
		*distance,          // for BFS()
		*discover,          // for DFS()
		*finish;
public:
	Graph() :num_vertex(0) {};
	Graph(int N) :num_vertex(N) {
		// initialize Adj List
		AdjList.resize(num_vertex);
	};

	int GetColor(int i) { return color[i]; };              // 取得private data: color
	int GetFinish(int i) { return finish[i]; };            // 取得private data: finish
	int GetPredecessor(int i) { return predecessor[i]; };  // 取得private data: predecessor

	void AddEdgeList(int from, int to);

	void BFS(int Start);
	void DFS(int Start);
	void DFSVisit(int vertex, int &time);
	void VariableInitializeDFS();     // 對DFS()需要的資料：color, discover, finish, predecessor
									  // 進行「配置記憶體」與「初始化」

	void CCDFS(int vertex);           // 吃一個int, 表示起點vertex, 若沒給就從0開始
	void CCBFS(int vertex = 0);
	void SetCollapsing(int vertex);
	void PrintDataArray(int *array);  // 列印出array[]
	void PrintFinish();               // 列印出 finish[]
	void PrintPredecessor();          // 列印出 predecessor[]

	Graph GraphTranspose();           // 產生Transpose of Graph
	void PrintSCCs(int Start = 0);    // 吃一個int, 表示起點vertex, 若沒給就從0開始

									  // 利用QuickSort()得到 finish[] 由大致小的vertex順序
	friend void QuickSort(int *vec, int front, int end, int *vec2);
	friend int Partition(int *vec, int front, int end, int *vec2);
	friend void swap(int *x, int *y);
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

void swap(int *x, int *y) {
	int temp = *x;
	*x = *y;
	*y = temp;
}
int Partition(int *vec, int front, int end, int *vec2) {
	int pivot = vec[end];
	int i = front - 1;
	for (int j = front; j < end; j++) {
		if (vec[j] > pivot) {
			i++;
			swap(&vec[i], &vec[j]);
			swap(&vec2[i], &vec2[j]);
		}
	}
	swap(&vec[i + 1], &vec[end]);
	swap(&vec2[i + 1], &vec2[end]);

	return i + 1;   // 把 i + 1 當成下一個 recurrsive call 的 中間斷點
}
void QuickSort(int *vec, int front, int end, int *vec2) {
	if (front < end) {
		int pivot = Partition(vec, front, end, vec2);
		QuickSort(vec, front, pivot - 1, vec2);
		QuickSort(vec, pivot + 1, end, vec2);
	}
}

void Graph::PrintSCCs(int Start) {
	// 第一次DFS(), 目的是取得finish[]
	DFS(Start);

	// 顯示 第一次DFS()後的finish[]
	std::cout << "First DFS() on G, finish time:" << std::endl;
	PrintFinish();

	// gT代表Transpose of Graph
	Graph gT(num_vertex);
	gT = GraphTranspose();

	// 矩陣 finishLargetoSmall[] 用來儲存 finish[] 由大至小的vertex順序
	int *finishLargetoSmall;
	finishLargetoSmall = new int[num_vertex];
	for (int i = 0; i < num_vertex; i++) {
		finishLargetoSmall[i] = i;
	}
	// QuickSort()會更新 finishLargetoSmall[] 成 finish[] 由大至小的vertex順序
	QuickSort(finish, 0, num_vertex - 1, finishLargetoSmall);

	// 列印出 finish[] 由大至小的vertex順序
	std::cout << "finish time Large to Small:" << std::endl;
	PrintDataArray(finishLargetoSmall);

	// 第二次DFS(), 執行在gT上, 先對四個資料「配置記憶體」且「初始化」
	gT.VariableInitializeDFS();
	int time = 0;
	for (int i = 0; i < num_vertex; i++) {
		if (gT.GetColor(finishLargetoSmall[i]) == 0) {
			gT.DFSVisit(finishLargetoSmall[i], time);
		}
	}

	// 顯示 第二次DFS()後的finish[]
	std::cout << "Second DFS() on gT, finish time:\n";
	gT.PrintFinish();
	// 顯示 第二次DFS()後的predecessor[]
	std::cout << "predecessor[] before SetCollapsing:\n";
	gT.PrintPredecessor();

	for (int i = 0; i< num_vertex; i++)
		gT.SetCollapsing(i);

	// 顯示 SetCollapsing後的predecessor[]
	std::cout << "predecessor after SetCollapsing:\n";
	gT.PrintPredecessor();

	// 如同在undirected graph中尋找connected component
	int num_cc = 0;
	for (int i = 0; i < num_vertex; i++) {
		if (gT.GetPredecessor(i) < 0) {
			std::cout << "SCC#" << ++num_cc << ": " << i << " ";
			for (int j = 0; j < num_vertex; j++) {
				if (gT.GetPredecessor(j) == i) {
					std::cout << j << " ";
				}
			}
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}
void Graph::VariableInitializeDFS() {
	color = new int[num_vertex];
	discover = new int[num_vertex];
	finish = new int[num_vertex];
	predecessor = new int[num_vertex];

	for (int i = 0; i < num_vertex; i++) {
		color[i] = 0;
		discover[i] = 0;
		finish[i] = 0;
		predecessor[i] = -1;
	}
}

void Graph::SetCollapsing(int current) {
	int root;  // root
	for (root = current; predecessor[root] >= 0; root = predecessor[root]);

	while (current != root) {
		int parent = predecessor[current];
		predecessor[current] = root;
		current = parent;
	}
}

void Graph::CCDFS(int vertex = 0) {

	DFS(vertex);      // 
	PrintPredecessor();
	for (int i = 0; i< num_vertex; i++) {
		SetCollapsing(i);
	}
	PrintPredecessor();

	int num_cc = 0;
	for (int i = 0; i < num_vertex; i++) {
		if (predecessor[i] < 0) {
			std::cout << "Component#" << ++num_cc << ": " << i << " ";
			for (int j = 0; j < num_vertex; j++) {
				if (predecessor[j] == i) {
					std::cout << j << " ";
				}
			}
			std::cout << std::endl;
		}
	}
}

void Graph::CCBFS(int vertex) {

	BFS(vertex);
	PrintPredecessor();
	for (int i = 0; i< num_vertex; i++) {
		SetCollapsing(i);
	}
	PrintPredecessor();

	int num_cc = 0;
	for (int i = 0; i < num_vertex; i++) {
		if (predecessor[i] < 0) {
			std::cout << "Component#" << ++num_cc << ": " << i << " ";
			for (int j = 0; j < num_vertex; j++) {
				if (predecessor[j] == i) {
					std::cout << j << " ";
				}
			}
			std::cout << std::endl;
		}
	}
}


Graph Graph::GraphTranspose() {
	Graph gT(num_vertex);
	for (int i = 0; i < num_vertex; i++) {
		for (std::list<int>::iterator itr = AdjList[i].begin(); itr != AdjList[i].end(); itr++) {
			gT.AddEdgeList(*itr, i);
		}
	}
	return gT;
}
void Graph::PrintDataArray(int *array) {
	for (int i = 0; i < num_vertex; i++)
		std::cout << std::setw(4) << i;
	std::cout << std::endl;
	for (int i = 0; i < num_vertex; i++)
		std::cout << std::setw(4) << array[i];
	std::cout << std::endl;
}
void Graph::PrintFinish() {
	for (int i = 0; i < num_vertex; i++)
		std::cout << std::setw(4) << i;
	std::cout << std::endl;
	for (int i = 0; i < num_vertex; i++)
		std::cout << std::setw(4) << finish[i];
	std::cout << std::endl;
}
void Graph::PrintPredecessor() {
	for (int i = 0; i < num_vertex; i++)
		std::cout << std::setw(4) << i;
	std::cout << std::endl;
	for (int i = 0; i < num_vertex; i++)
		std::cout << std::setw(4) << predecessor[i];
	std::cout << std::endl;
}

int main() {
	Graph g4(9);
	g4.AddEdgeList(0, 1);
	g4.AddEdgeList(1, 2); g4.AddEdgeList(1, 4);
	g4.AddEdgeList(2, 0); g4.AddEdgeList(2, 3); g4.AddEdgeList(2, 5);
	g4.AddEdgeList(3, 2);
	g4.AddEdgeList(4, 5); g4.AddEdgeList(4, 6);
	g4.AddEdgeList(5, 4); g4.AddEdgeList(5, 6); g4.AddEdgeList(5, 7);
	g4.AddEdgeList(6, 7);
	g4.AddEdgeList(7, 8);
	g4.AddEdgeList(8, 6);

	std::cout << "Vertex(0) as starting point for the First DFS():\n\n";
	g4.PrintSCCs();
	std::cout << "Vertex(3) as starting point for the First DFS():\n\n";
	g4.PrintSCCs(3);

	return 0;
}