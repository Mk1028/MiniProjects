#include "DiskIO.h"
#include "structures.h"
#include "SubMethods.h"

void saveMetaData(MD * metadata,FR *fileRecords,char *bytevector)
{
	char *buffer = (char *)malloc(sizeof(char)*BlockSize);
	int blockNumber = 0;

	memcpy(buffer, metadata, sizeof(MD));
	writeBlock(buffer, blockNumber);	blockNumber++;

	memcpy(buffer, bytevector,BlockSize);
	writeBlock(buffer, blockNumber);	blockNumber++;

	memcpy(buffer,fileRecords, sizeof(FR)*maxfiles);
	writeBlock(buffer, blockNumber);	
}

FR * retrieveFileRecords()
{
	char *buffer = (char *)malloc(sizeof(char)*BlockSize);
	int blockNumber = 2;

	FR *fileRecords = Creation();

	readBlock(buffer, blockNumber);
	memcpy(fileRecords,buffer,sizeof(FR)*maxfiles);

	return fileRecords;
}

MD * retrieveMetaData(MD * metadata,char *bytevector)
{
	char *buffer = (char *)malloc(sizeof(char)*BlockSize);
	int blockNumber = 0;

	readBlock(buffer, blockNumber);
	memcpy(metadata, buffer, sizeof(FR)*maxfiles); blockNumber++;

	readBlock(buffer, blockNumber);
	memcpy(bytevector, buffer, sizeof(FR)*maxfiles);

	return metadata;
}

void ListOfFiles(FR *fileRecords)
{
	int NumberOfFiles = numberOfFiles(fileRecords);
	if (NumberOfFiles == 0)
		printf("No Files Exisiting\n");
	else
	{
		for (int i = 0; i < NumberOfFiles; i++)
		{
			printf("%s\n",fileRecords[i].FileName);
		}
	}
}

void Debug(FR *fileRecords)
{
	int NumberOfFiles = numberOfFiles(fileRecords);
	if (NumberOfFiles == 0)
		printf("No Files Exisiting\n");
	else
	{
		printf("%15s %8s %8s %8s\n", "FileName", "Size", "BlockCount", "Start");
		for (int i = 0; i < NumberOfFiles; i++)
		{
			printf("%15s %8u %8u %8u\n", fileRecords[i].FileName,fileRecords[i].Size,fileRecords[i].BlockCount,fileRecords[i].Start);
		}
	}
}

FR * Delete(char *fileName,FR *fileRecords,char *ByteVector)
{
	int NumberOfFiles = numberOfFiles(fileRecords);
	int i = isFileExisting(fileName, fileRecords, NumberOfFiles);

	if (i==NumberOfFiles)
		printf("%s Doesn't Exist in HardDisk\n",fileName);
	else
	{
		int blockCount = fileRecords[i].BlockCount;
		int start = fileRecords[i].Start;

		strcpy(fileRecords[i].FileName,fileRecords[NumberOfFiles - 1].FileName);
		fileRecords[i].Size=fileRecords[NumberOfFiles - 1].Size;
		fileRecords[i].Start = fileRecords[NumberOfFiles - 1].Start;
		fileRecords[i].BlockCount = fileRecords[NumberOfFiles - 1].BlockCount;
		fileRecords[NumberOfFiles-1].FileName[0] = '\0';
		fileRecords[NumberOfFiles-1].Size = 0;
		fileRecords[NumberOfFiles-1].Start = 0;
		fileRecords[NumberOfFiles-1].BlockCount = 0;

		if (blockCount > 1)
		{
			char *buffer = (char *)malloc(sizeof(char)*BlockSize);
			int *blocks = (int *)malloc(sizeof(int)*blockCount);
			readBlock(buffer, start);
			memcpy(blocks,buffer,sizeof(blocks)*blockCount);
			while (blockCount > 0)
			{
				ByteVector[blocks[blockCount - 1]] = '0';
				blockCount--;
			}
		}
		else
		{
			ByteVector[start] = '0';
		}	
	}
	return fileRecords;
}

FR * copyToDisk(FR *fileRecords,char *file1,char *file2,char *ByteVector)
{
	FILE *fp = fopen(file1, "rb");

	if (fp == NULL)
		printf("%s Doesn't Exist\n", file1);
	else
	{
		int NumberOfFiles = numberOfFiles(fileRecords);

		strcpy(fileRecords[NumberOfFiles].FileName, file2);

		unsigned int fileSize = 0;

		char ch;

		while (fread(&ch, 1, 1, fp))
		{
			fileSize++;
		}

		fclose(fp);

		fileRecords[NumberOfFiles].Size = fileSize;

		int count = fileSize / BlockSize ,blockCount=count;

		if (fileSize % BlockSize > 0)
			blockCount = count + 1;

		if (blockCount > 1)
			blockCount += 1;

		fileRecords[NumberOfFiles].BlockCount = blockCount;

		int blockNumber = metaSize,BCount=0;

		int *blocks = (int *)malloc(sizeof(int)*blockCount),ind=0;

		while (BCount!=blockCount)
		{
			blockNumber++;
			if (ByteVector[blockNumber] == '0')
			{
				blocks[ind++]=blockNumber;
				ByteVector[blockNumber] = '1';
				BCount++;
			}
			if (BCount == 1)
			{
				fileRecords[NumberOfFiles].Start = blockNumber;
			}
		}

		char *buffer = (char *)malloc(sizeof(char)*BlockSize);

		ind = 0;

		if (blockCount > 1)
		{
			memcpy(buffer,blocks,sizeof(blocks)*(blockCount));
			writeBlock(buffer,fileRecords[NumberOfFiles].Start);
			ind = 1;
			BCount--;
		}

		fp = fopen(file1, "rb");

		while (BCount > 0)
		{
			readBlock1(fp,buffer);
			writeBlock(buffer,blocks[ind++]);
			BCount--;
		}

		fclose(fp);
	}

	return fileRecords;
}

void copyFromDisk(FR *fileRecords, char *file1, char *file2, char *ByteVector)
{
	int NumberOfFiles = numberOfFiles(fileRecords);
	int curr = isFileExisting(file1,fileRecords,NumberOfFiles);

	if (curr == NumberOfFiles)
		printf("%s Doesn't Exist\n",file1);
	else
	{
		FILE *fp = fopen(file2, "wb");

		if(fp==NULL)
			printf("Can't Open %s\n", file2);
		else
		{
			int blockCount = fileRecords[curr].BlockCount;
			int start = fileRecords[curr].Start;
			int ind = 0,remaining=(fileRecords[curr].Size)%BlockSize;

			char *buffer = (char *)malloc(sizeof(char)*BlockSize);
			int *blocks = (int *)malloc(sizeof(int)*blockCount);
			if (blockCount > 1)
			{
				readBlock(buffer, start);
				memcpy(blocks, buffer, sizeof(blocks)*blockCount);
				ind++;
				blockCount--;
			}
			else
				blocks[0] = start;

			while (blockCount > 0)
			{
				readBlock(buffer,blocks[ind++]);
				if (blockCount == 1)
				{
					fp = writeBlock1(buffer,fp, remaining);
				}
				else
					fp = writeBlock1(buffer, fp, BlockSize);
				blockCount--;
			}
		}
		fclose(fp);
	}
}

int main()
{
	char input[100],*ByteVector = (char *)malloc(sizeof(char) * 6400);

	FR *fileRecords=Creation();

	int NumberOfFiles = 0;

	for (int i = 0; i < 6000; i++)
	{
		ByteVector[i] = '0';
	}

	MD *metadata=(MD *)malloc(sizeof(MD));

	while (1)
	{
		printf(">");
		gets_s(input);
		while (input[0] == '\0')
		{
			printf("\n>");
			gets_s(input);
		}
		
		char **Command = (char **)malloc(sizeof(char *) * 3);

		for (int i = 0; i < 3; i++)
		{
			Command[i] = (char *)malloc(sizeof(char) * 20);
		}

		parseCommand(input,Command);

		unsigned int NumberOfBlocks;

		if (compare(Command[0], "mount", 5))
		{
			NumberOfBlocks = init(Command[1], str_to_number(Command[2]));
			char *buffer = (char *)malloc(sizeof(char)*BlockSize);
			readBlock(buffer, 0);
			unsigned int num = 0;
			memcpy(&num, buffer, 4);
			if (num == magicNumber)
			{
				metadata = retrieveMetaData(metadata, ByteVector);
				fileRecords = retrieveFileRecords();
			}
			else
			{
				char ch;
				printf("Do You Want to Format Y-YES\tN-NO\t");
				scanf("%c", &ch);
				if (ch == 'y' || ch == 'Y')
					writeBlock(ByteVector, 0);
			}
			metadata = MDInitialize(NumberOfBlocks, str_to_number(Command[2]));
		}
		else if (compare(Command[0], "copytofs", 8))
			fileRecords=copyToDisk(fileRecords,Command[1],Command[2],ByteVector);
		else if (compare(Command[0], "copyfromfs", 10))
			copyFromDisk(fileRecords, Command[1], Command[2], ByteVector);
		else if (compare(Command[0], "ls", 2))
			ListOfFiles(fileRecords);
		else if (compare(Command[0], "delete", 6))
			fileRecords = Delete(Command[1],fileRecords,ByteVector);
		else if (compare(Command[0], "debug", 5))
			Debug(fileRecords);
		else if (  compare( Command[0] , "exit", 4) )
		{
			saveMetaData(metadata, fileRecords, ByteVector);
			break;
		}
		else if (  compare( Command[0] , "format", 6) )
		{
			unsigned int blocksize = str_to_number(Command[1]);
			char *buffer =(char *)malloc(sizeof(char)*(blocksize));
			for (int i = 0; i < blocksize; i++)
				buffer[i] = '\0';
			writeBlock(buffer,0);
		}
		else if (  compare( Command[0] , "unmount", 7) )
		{
			init((char *)"\0", BlockSize);
			metadata = NULL;
			fileRecords = NULL;
			for (int i = 0; i < 6000; i++)
			{
				ByteVector[i] = '0';
			}
		}
		else if (  compare( Command[0] , "clear", 5) )
			system("cls");
		else
			printf("Invalid Command\n");
	}
	return 0;
}