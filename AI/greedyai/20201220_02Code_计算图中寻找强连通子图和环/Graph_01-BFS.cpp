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
	int *color,             // 0:��ɫ, 1:��ɫ, 2:��ɫ
		*distance,          // 0:���, ���޴�:������߲����Ķ���
		*predecessor;       // -1:�]��predecessor, ��ʾΪ���vertex
public:
	Graph() :num_vertex(0) {};           // Ĭ�Ϲ��캯��
	Graph(int N) :num_vertex(N) {        // ������Ĺ��캯��:������
		// ��ʼ���ڽӱ�
		AdjList.resize(num_vertex);
	};
	void AddEdgeList(int from, int to);
	void BFS(int Start);
	void PrintPredecessor();               // ��ӡpredecessor, ����֤��, �Ǳ�Ҫ
	void PrintDistance();               // ӡ��distance, ����֤��, �Ǳ�Ҫ
};

void Graph::AddEdgeList(int from, int to) {
	AdjList[from].push_back(to);
}

void Graph::BFS(int Start) {

	color = new int[num_vertex];
	predecessor = new int[num_vertex];
	distance = new int[num_vertex];

	for (int i = 0; i < num_vertex; i++) {  // ��ʼ������ͼ��(b)
		color[i] = 0;                       // 0:��ɫ;
		predecessor[i] = -1;                // -1��ʾ�]��predecessor
		distance[i] = num_vertex + 1;       // num_vertex��vertex, 
	}                                       // ����� distance = num_vertex-1��edge

	std::queue<int> q;
	int i = Start;

	for (int j = 0; j < num_vertex; j++) {  // j��0����num_vertex-1, ���j���߹�graph������vertex
		if (color[i] == 0) {                // ��һ��i�������vertex, ��ͼ��(c)
			color[i] = 1;                   // 1:��ɫ
			distance[i] = 0;                // ÿһ��connected component�����֮�������0
			predecessor[i] = -1;            // ÿһ��connected component�����]��predecessor
			q.push(i);
			while (!q.empty()) {
				int u = q.front();                  // uΪ�µ��������
				for (std::list<int>::iterator itr = AdjList[u].begin(); itr != AdjList[u].end(); itr++) {
					if (color[*itr] == 0) {                // �������ҵ�����vertex�ǰ�ɫ
						std::cout << "��� = " << u << ", itr = " << *itr << endl;
						color[*itr] = 1;                   // Ϳ�ɻ�ɫ, ��ʾ�Ѿ������ҵ���
						distance[*itr] = distance[u] + 1;  // ������predecessor֮�����1
						predecessor[*itr] = u;             // ���±����ҵ�����vertex��predecessor
						q.push(*itr);                      // ��vertex�ƽ�queue
					}
				}
				q.pop();        // ��u�Ƴ�queue
				color[u] = 2;   // ���Ұ�uͿ�ɺ�ɫ
			}
		}
		// ��һ�λ�Ȧ�]�а�����vertex�߹�, ��ʾgraph�ж��connected component
		// �Ͱ�i���j, �������graph�е�����vertex�Ƿ����ǰ�ɫ, ����, �ظ�while loop
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
	// ������ͼ��(a)��Adjacency List
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