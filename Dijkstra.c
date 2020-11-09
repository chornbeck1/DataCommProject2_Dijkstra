#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <math.h>

char* dataFile = "LSInput.txt";
char foundNodes[26];
int numNodesFound=0;



void ClearBuffer(char* buffer){
    for(int i = 0; i < 256; i++)
    {
        buffer[i] = '\0';
    }
}

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
			printf("Found block: %s\n",inputBuffer);
			//Found completed block
			//Process for node weights
			char source = strtok(inputBuffer,",")[1];
			char dest = strtok(NULL,",")[0];
			int cost = atoi(strtok(NULL,"}"));
			printf("Source: %c\tDest: %c\tCost: %d\n",source,dest,cost);
			if(NULL==strchr(foundNodes,source))
			{
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
	printf("\n\nNodes Found: %s\n",foundNodes);
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

int main(int argc, char *argv[]) {
	ReadFile(dataFile);
	DisplayTable();
}
