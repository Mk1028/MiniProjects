#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define DiskSize (104857600)
#define magicNumber (0x444e524d)
static int metaSize;
static int BlockSize;
static char hardDisk[20];

unsigned int init(char *harddisk, unsigned int blockSize)
{
	BlockSize = blockSize;
	metaSize = (2 * 1024 * 1024) / blockSize;
	strcpy(hardDisk,harddisk);
	return DiskSize / blockSize;
}

void writeBlock(char *buffer,int blockNumber)
{
	FILE *fp = fopen(hardDisk, "rb+");
	fseek(fp, blockNumber*BlockSize, SEEK_SET);
	fwrite(buffer,BlockSize, 1, fp);
	fclose(fp);
}

void readBlock(char *buffer,int blockNumber)
{
	FILE *fp = fopen(hardDisk, "rb");
	fseek(fp, blockNumber*BlockSize, SEEK_SET);
	fread(buffer, BlockSize, 1, fp);
	fclose(fp);
}

void parseCommand(char *command, char **arg)
{
	int j, ind=0, k = 0;
	for (j = 0; command[j] != '\0'; j++)
	{
		if(command[j]!=' ')
			arg[ind][k++] = command[j];
		else
		{
			arg[ind][k] = '\0';
			ind++; k = 0;
		}
	}
	arg[ind][k] = '\0';
}
