/* Cole Hornbeck
 * Dijkstra routing example
 * Not proud of this code, but it works. If I had the time I'd make it cleaner
 */

#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include "Dijkstra.h"



char* dataFile;
char foundNodes[26];
char baseNode = 'A';
struct Node Nodes[26];
char ProcessedNodes[26];
struct Message Messages[1024];
int numNodesFound=0;
int numNodesProcessed=0;
int messagesFound=0;
int adjMat[26][26];

int main(int argc, char *argv[]) {
	if(argc>1)
	{
		printf("%s\n",argv[1]);
		dataFile = argv[1];
	}
	else
	{
		dataFile = "inputTest.txt";
	}
	
	InitMatrix();
	ReadFile(dataFile);
	FindNeighbours();
	DisplayTable();
	
	//for(int i=0;i<messagesFound;i++){PrintMessage(Messages[i]);}
	//for(int i=0;i<numNodesFound;i++){PrintNode(Nodes[i]);}
	
	
	DisplayLine(0);
	while(numNodesProcessed<numNodesFound)
	{
		int idx = FindNextNode();
		ProcessedNodes[numNodesProcessed]=Nodes[idx].name;
		for(int i=0;i<Nodes[idx].numNbrs;i++)
		{
			int nbrIdx=FindNodeWithName(Nodes[idx].nbrs[i]);
			int costToNbr=Nodes[idx].weights[i];
			if((Nodes[nbrIdx].weightToA<0) || (Nodes[idx].weightToA + costToNbr < Nodes[nbrIdx].weightToA))
			{
				Nodes[nbrIdx].weightToA=Nodes[idx].weightToA+costToNbr;
				Nodes[nbrIdx].nextHop=Nodes[idx].nextHop;
			}
		}
		DisplayLine(numNodesProcessed++);
	}
	DisplayRoutingTable();
	
}

//Prints the routing table
void DisplayRoutingTable()
{
	printf("\n\nRouting Table for %c\n",baseNode);
	printf("-------------------\n");
	printf("Dest\tNxtHp\n");
	for(int i=1;i<numNodesFound;i++){printf("%c\t%c\n",Nodes[i].name, Nodes[i].nextHop);}
}

//Returns the index in Nodes[] of the node with the given name
int FindNodeWithName(char NodeName)
{
	for(int i=0;i<numNodesFound;i++)
	{
		if(Nodes[i].name==NodeName)
		{
			return i;
		}
	}
	return -1;
}

//Prints a single step of the algorithm
void DisplayLine(int step)
{
	int numTabs=ceil(numNodesFound/8-(floor(step/7)));
	printf("  %d\t",step);
	printf("%s",ProcessedNodes);
	for(int i=0;i<numTabs;i++){printf("\t");}
	for(int i=0;i<numNodesFound;i++)
	{
		if(HasNodeBeenProcessed(Nodes[i].name)<0)
		{
			if(Nodes[i].weightToA<0)
			{
				printf("-,-\t");
			}
			else
			{
				printf("%d,%c\t",Nodes[i].weightToA,Nodes[i].nextHop);
			}
		}
		else
		{
			if(ProcessedNodes[numNodesProcessed-1]==Nodes[i].name && Nodes[i].name != baseNode)
			{
				printf(" * ");
			}
			printf("\t");
		}
	}
	printf("\n");
	
}

//Clears a buffer
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
	FILE *fp;
	if((fp = fopen(fileName,"r"))==NULL)
	{
		printf("\n\nERROR OPENING FILE\n\n");
	}
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
				if(source==baseNode){Nodes[numNodesFound].weightToA=0;}
				foundNodes[numNodesFound++]=source;
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

//Processes input to build all node neighbours and lay groundwork for first step
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
	
	for(int i=0;i<numNodesFound;i++)
	{
		if(Nodes[i].name==baseNode)
		{
			for(int j=0;j<Nodes[i].numNbrs;j++)
			{
				UpdateNodeCost(Nodes[i].nbrs[j], Nodes[i].weights[j],Nodes[i].nbrs[j]);
			}
			break;
		}
	}
	
	ProcessedNodes[numNodesProcessed++]=baseNode;
}

//Supopsedly updates a nodes neighbours, don't think it works.
void UpdateNodeNeighbours(char sourceNode)
{
	for(int i=0;i<numNodesFound;i++)
	{
		if(Nodes[i].name==sourceNode)
		{
			for(int j=0;j<Nodes[i].numNbrs;j++)
			{
				UpdateNodeCost(Nodes[i].nbrs[j], Nodes[i].weights[j],'C');
			}
			break;
		}
	}
}

//Prints out the header for the table.
void DisplayTable()
{
	//Print first line of formatting adding appropriate tabs
	printf("Step\tN");
	int numTabs=ceil(numNodesFound/8+1);
	for(int i=0;i<numTabs;i++){printf("\t");}
	for(int i=1;i<numNodesFound;i++){printf("D(%c)\t",foundNodes[i]);}
	//Same as first line but with n instead of D
	printf("\n");
	for(int i=-1;i<numTabs;i++){printf("\t");}
	for(int i=1;i<numNodesFound;i++){printf("n(%c)\t",foundNodes[i]);}
	//Print dashes equal to number of tabs * 8, plus some extra.
	int numDashes = (numTabs+numNodesFound-1)*8+4;
	printf("\n");
	for(int i=0;i<numDashes;i++){printf("-");}
	printf("\n");
}

//Helper to print contents of a message
void PrintMessage(struct Message msg)
{
	printf("%c->%c, %d\n",msg.source,msg.dest,msg.cost);
}

//Helper to print contents of a node
void PrintNode(struct Node nd)
{
	printf("\nNode %c\tCost to A: %d\n\tNbrs\tCosts\n",nd.name,nd.weightToA);
	for(int i=0;i<nd.numNbrs;i++)
	{
		printf("\t%c\t%d\n",nd.nbrs[i],nd.weights[i]);
	}
	printf("Next Hop: %c", nd.nextHop);
}

//Checks if the given node has been processed.
int HasNodeBeenProcessed(char NodeName)
{
	for(int i=0;i<numNodesProcessed;i++)
	{
		if(ProcessedNodes[i] == NodeName)
		{
			return 1;
		}
	}
	return -1;
}

//Returns the index in Nodes[] of the lowest-cost non-processed node
int FindNextNode()
{
	int lowestCost=INT_MAX;
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

//Inits a 26x26 int matrix, unused.
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

//Helper to update a node cost, unused
void UpdateNodeCost(char NodeName, int newCost, char newNextHop)
{
	for(int i=0;i<numNodesFound;i++)
	{
		if(Nodes[i].name==NodeName)
		{
			Nodes[i].weightToA=newCost;
			Nodes[i].nextHop=newNextHop;
			break;
		}
	}
}

