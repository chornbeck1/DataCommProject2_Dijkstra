#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <stdbool.h>
#include <math.h>

struct Path
{
	char source;
	char dest;
	int cost;
	int numNodes;
	char* path[24];
}

struct Node
{
	char name;
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

char* dataFile = "LSInput.txt";
char foundNodes[26];
struct Node Nodes[26];
struct Node ProcessedNodes[26];
struct Message Messages[1024];
int numNodesFound=0;
int numNodesProcessed=0;
int messagesFound=0;
int adjMat[26][26];


void ClearBuffer(char* buffer){
    for(int i = 0; i < 256; i++)
    {
        buffer[i] = '\0';
    }
}

//Parses the input file to read out all individual base ndoes and messages
void ReadFile(char* fileName)
{
	char inputBuffer[256];
	char read;
	FILE *fp = fopen(fileName,"r");
	int count=0;
	while(1)
	{
		read=fgetc(fp);
		if(feof(fp)){ break; }
		else if(read==';')
		{
			//Found completed block
			//Process for node weights
			char source = strtok(inputBuffer,",")[1];
			char dest = strtok(NULL,",")[0];
			int cost = atoi(strtok(NULL,"}"));
			Messages[messagesFound].source=source;
			Messages[messagesFound].dest=dest;
			Messages[messagesFound].cost=cost;
			messagesFound++;
			if(NULL==strchr(foundNodes,source))
			{
				Nodes[numNodesFound].name=source;
				Nodes[numNodesFound].weightToA=-1;
				Nodes[numNodesFound].numNbrs=0;
				foundNodes[numNodesFound++]=source;
				
				if(source=='A'){Nodes[numNodesFound].weightToA=0;}
			}
			
			ClearBuffer(inputBuffer);
			count=0;
		}
		else
		{
			inputBuffer[count++]=read;
		}
	}
	fclose(fp);
	for(int i=1;i<numNodesFound;i++)
	{
		Nodes[0].paths[i].source='A';
		Nodes[0].paths[i].dest=Nodes[i].name;
		Nodes[0].paths[i].cost=-1;
		Nodes[0].paths[i].numNodes=2;
	}
}

void FindNeighbours()
{
	for(int i=0;i<messagesFound;i++)
	{
		char messageSrc=Messages[i].source;
		char messageDest=Messages[i].dest;
		char messageCost=Messages[i].cost;
		for(int j=0;j<numNodesFound;j++)
		{
			if(Nodes[j].name==messageSrc)
			{
				Nodes[j].nbrs[Nodes[j].numNbrs]=messageDest;
				Nodes[j].weights[Nodes[j].numNbrs]=messageCost;
				Nodes[j].numNbrs++;
			}
		}
	}
}

void DisplayTable()
{
	//Print first line of formatting adding appropriate tabs
	printf("Step\tN");
	int numTabs=ceil(numNodesFound/8+1);
	for(int i=0;i<numTabs;i++){printf("\t");}
	for(int i=0;i<numNodesFound;i++){printf("D(%c)\t",foundNodes[i]);}
	//Same as first line but with n instead of D
	printf("\n");
	for(int i=-1;i<numTabs;i++){printf("\t");}
	for(int i=0;i<numNodesFound;i++){printf("n(%c)\t",foundNodes[i]);}
	//Print dashes equal to number of tabs * 8, plus some extra.
	int numDashes = (numTabs+numNodesFound)*8+4;
	printf("\n");
	for(int i=0;i<numDashes;i++){printf("-");}
}

void PrintMessage(struct Message msg)
{
	printf("%c->%c, %d\n",msg.source,msg.dest,msg.cost);
}

void PrintNode(struct Node nd)
{
	printf("\nNode %c\n\tNbrs\tCosts\n",nd.name);
	for(int i=0;i<nd.numNbrs;i++)
	{
		printf("\t%c\t%d\n",nd.nbrs[i],nd.weights[i]);
	}
}


int HasNodeBeenProcessed(char NodeName)
{
	for(int i=0;i<numNodesProcessed;i++)
	{
		if(ProcessedNodes[i].name == NodeName)
		{
			return 1;
		}
	}
	return -1;
}

int FindNextNode()
{
	int lowestCost=0;
	int lowestIndex=0;
	for(int i=0;i<numNodesFound;i++)
	{
		if(Nodes[i].weightToA>=0 && Nodes[i].weightToA<=lowestCost)
		{
			if(HasNodeBeenProcessed(Nodes[i].name)<0)
			{
				lowestCost=Nodes[i].weightToA;
				lowestIndex=i;
			}
		}
	}
	return lowestIndex;
}

void InitMatrix()
{
	for(int i=0;i<26;i++)
	{
		for(int j=0;j<26;j++)
		{
			adjMat[i][j]=-1;
		}
	}
}


int main(int argc, char *argv[]) {
	InitMatrix();
	ReadFile(dataFile);
	FindNeighbours();
	DisplayTable();
	
	for(int i=0;i<messagesFound;i++){PrintMessage(Messages[i]);}
	for(int i=0;i<numNodesFound;i++){PrintNode(Nodes[i]);}
	
	while(numNodesProcessed<numNodesFound)
	{
		int idx = FindNextNode();
		for(int i=0;i<Nodes[idx].numNbrs;i++)
		{
			
		}
		
	}
}
