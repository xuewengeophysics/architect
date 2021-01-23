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
	int *color,             // 0:°×É«, 1:»ÒÉ«, 2:ºÚÉ«
		*distance,          // 0:Æðµã, ÎÞÏÞ´ó:´ÓÆðµã×ß²»µ½µÄ¶¥µã
		*predecessor;       // -1:›]ÓÐpredecessor, ±íÊ¾ÎªÆðµãvertex
public:
	Graph() :num_vertex(0) {};           // Ä¬ÈÏ¹¹Ôìº¯Êý
	Graph(int N) :num_vertex(N) {        // ´øÊäÈëµÄ¹¹Ôìº¯Êý:¶¥µãÊý
		// ³õÊ¼»¯ÁÚ½Ó±í
		AdjList.resize(num_vertex);
	};
	void AddEdgeList(int from, int to);
	void BFS(int Start);
	void PrintPredecessor();               // ´òÓ¡predecessor, ¹©ÑéÖ¤ÓÃ, ·Ç±ØÒª
	void PrintDistance();               // Ó¡³ödistance, ¹©ÑéÖ¤ÓÃ, ·Ç±ØÒª
};

void Graph::AddEdgeList(int from, int to) {
	AdjList[from].push_back(to);
}

void Graph::BFS(int Start) {

	color = new int[num_vertex];
	predecessor = new int[num_vertex];
	distance = new int[num_vertex];

	for (int i = 0; i < num_vertex; i++) {  // ³õÊ¼»¯£¬ÈçÍ¼¶þ(b)
		color[i] = 0;                       // 0:°×É«;
		predecessor[i] = -1;                // -1±íÊ¾›]ÓÐpredecessor
		distance[i] = num_vertex + 1;       // num_vertex¸övertex, 
	}                                       // ×î³¤¾àÀë distance = num_vertex-1Ìõedge

	std::queue<int> q;
	int i = Start;

	for (int j = 0; j < num_vertex; j++) {  // j´Ó0Êýµ½num_vertex-1, Òò´Ëj»á×ß¹ýgraphÖÐËùÓÐvertex
		if (color[i] == 0) {                // µÚÒ»´Îi»áÊÇÆðµãvertex, ÈçÍ¼¶þ(c)
			color[i] = 1;                   // 1:»ÒÉ«
			distance[i] = 0;                // Ã¿Ò»¸öconnected componentµÄÆðµãÖ®¾àÀëÉè³É0
			predecessor[i] = -1;            // Ã¿Ò»¸öconnected componentµÄÆðµã›]ÓÐpredecessor
			q.push(i);
			while (!q.empty()) {
				int u = q.front();                  // uÎªÐÂµÄËÑË÷Æðµã
				for (std::list<int>::iterator itr = AdjList[u].begin(); itr != AdjList[u].end(); itr++) {
					if (color[*itr] == 0) {                // Èô±»¡¸ÕÒµ½¡¹µÄvertexÊÇ°×É«
						std::cout << "Æðµã = " << u << ", itr = " << *itr << endl;
						color[*itr] = 1;                   // Í¿³É»ÒÉ«, ±íÊ¾ÒÑ¾­±»¡¸ÕÒµ½¡¹
						distance[*itr] = distance[u] + 1;  // ¾àÀëÊÇpredecessorÖ®¾àÀë¼Ó1
						predecessor[*itr] = u;             // ¸üÐÂ±»¡¸ÕÒµ½¡¹µÄvertexµÄpredecessor
						q.push(*itr);                      // °ÑvertexÍÆ½øqueue
					}
				}
				q.pop();        // °ÑuÒÆ³öqueue
				color[u] = 2;   // ²¢ÇÒ°ÑuÍ¿³ÉºÚÉ«
			}
		}
		// ÈôÒ»´Î»ØÈ¦›]ÓÐ°ÑËùÓÐvertex×ß¹ý, ±íÊ¾graphÓÐ¶à¸öconnected component
		// ¾Í°ÑiÁí³Éj, ¼ÌÐø¼ì²égraphÖÐµÄÆäËûvertexÊÇ·ñÈÔÊÇ°×É«, ÈôÊÇ, ÖØ¸´while loop
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
	// ½¨Á¢³öÍ¼¶þ(a)µÄAdjacency List
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