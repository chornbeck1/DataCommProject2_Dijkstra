#ifndef DijkstraH
#define DijkstraH

struct Path
{
	char source;
	char dest;
	int cost;
	int numNodes;
	char* path[24];
};

struct Node
{
	char name;
	char nextHop;
	int weightToA;
	int numNbrs;
	char pathToA[26];
	char nbrs[25];
	char weights[25];
	struct Path paths[25];
};

struct Message
{
	char source;
	char dest;
	int cost;
};

void ReadFile(char* fileName);
void FindNeighbours();
void DisplayTable();
void DisplayRoutingTable();
void PrintMessage(struct Message msg);
void PrintNode(struct Node nd);
int HasNodeBeenProcessed(char NodeName);
void UpdateNodeCost(char NodeName, int newCost, char newNextHop);
void ClearBuffer(char* buffer);
void InitMatrix();
int FindNextNode();
void DisplayLine(int step);
int FindNodeWithName(char NodeName);

#endif
