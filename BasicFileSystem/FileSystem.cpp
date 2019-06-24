#include "Copy.h"

void print(MD *metadata)
{
	if (metadata == NULL)
		printf("No Files Found\n");
	else
	{
		printf("%20s%17s%17s\n", "FILEName", "Start", "Length");
		while (metadata->next != NULL)
		{
			printf("%20s %17ld %17ld\n", metadata->filename, metadata->start, metadata->length);
			metadata = metadata->next;
		}
		printf("%20s %17ld %17ld\n", metadata->filename, metadata->start, metadata->length);
	}
}

void filesList(MD *metadata)
{
	if (metadata == NULL)
		printf("No Files Found\n");
	else
	{
		while (metadata->next != NULL)
		{
			printf("%s\n", metadata->filename);
			metadata = metadata->next;
		}
		printf("%s\n", metadata->filename);
	}	
}

void storeMeta(MD * metadata, char *freeBlocks)
{
	FILE *fp = fopen("hardDisk.hdd","rb+");
	int num = 0x444e524d;
	if (fp == NULL)
		printf("File Can't Found\n");
	MD *temp = metadata;
	char ch;
	int ind;//fileCount = 0;
	fwrite(&num, 4, 1, fp);
	while (temp != NULL)
	{
		ind = 0;
		while (temp->filename[ind] != '\0')
		{
			ch = temp->filename[ind++];
			fwrite(&ch, 1, 1, fp);
		}
		fwrite("\t", 1, 1, fp);
		fwrite(&temp->start,sizeof(long), 1, fp);
		fwrite("\t", 1, 1, fp);
		fwrite(&temp->blocks, sizeof(long),20, fp);
		fwrite("\t", 1, 1, fp);
		fwrite(&temp->length, sizeof(long), 1, fp);
		fwrite("\t", 1, 1, fp);
		temp = temp->next;
		//fileCount++;
	}
	//fwrite(&fileCount, sizeof(int), 1, fp);
	//fwrite("\t", 1, 1, fp);
	fwrite("\e", 1, 1, fp);
	fwrite(freeBlocks,6000, 1, fp);
	fwrite("\f", 1, 1, fp);
	fclose(fp);
}

MD * retrieveMeta(char *freeBlocks)
{
	FILE *fp = fopen("hardDisk.hdd", "rb");
	if (fp == NULL)
		printf("File Isn't Found\n");
	MD *newNode = newEntry();
	char ch;
	int ind;
	MD *metadata=NULL;
	int num1;
	
	fread(&num1, 4, 1, fp);
	
	int num = 0x444e524d;

	if (num != num1)
	{
		printf("Do You Want To Format .. ?\tY-YES\tN-NO\t");
		char ch;
		scanf("%c", &ch);
		if (ch == 'y' || ch == 'Y')
		{
			return NULL;
		}
	}
	else
	{
		if (metadata == NULL)
		{
			ind = 0;
			while (fread(&ch, 1, 1, fp))
			{
				if (ch != '\t')
					newNode->filename[ind++] = ch;
				else
				{
					newNode->filename[ind++] = '\0';
					break;
				}
			}
			fread(&newNode->start, sizeof(long), 1, fp);
			fread(&ch, 1, 1, fp);
			fread(&newNode->blocks, sizeof(long), 20, fp);
			fread(&ch, 1, 1, fp);
			fread(&newNode->length, sizeof(long), 1, fp);
			fread(&ch, 1, 1, fp);
			metadata = newNode;
		}
		MD *temp = metadata;
		while (temp != NULL)
		{
			newNode = newEntry();
			ind = 0;
			fread(&ch, 1, 1, fp);
			if (ch != '\e')
			{
				while (ch != '\t')
				{
					newNode->filename[ind++] = ch;
					fread(&ch, 1, 1, fp);
				}
				newNode->filename[ind++] = '\0';
				fread(&newNode->start, sizeof(long), 1, fp);
				fread(&ch, 1, 1, fp);
				fread(&newNode->blocks, sizeof(long), 20, fp);
				fread(&ch, 1, 1, fp);
				fread(&newNode->length, sizeof(long), 1, fp);
				fread(&ch, 1, 1, fp);
				temp->next = newNode;
				temp = temp->next;
			}
			else
			{
				fread(freeBlocks, 6000, 1, fp);
				break;
			}
		}
		fclose(fp);
	}
	return metadata;
}

FILE * writeBlock(char buffer[blocksize], FILE *fp,int bsize)
{
	fwrite(buffer,bsize,1,fp);
	return fp;
}

char * readBlock(FILE *fp, char *buffer)
{
	fread(buffer, blocksize, 1, fp);
	return buffer;
}

MD * copyToDisk(MD *metadata,char *path,char *freeBlocks)
{
	FILE *from=fopen(path,"rb");

	char **file = (char **)malloc(sizeof(char *) * 2);
	file[0] = (char *)malloc(sizeof(char) * 20);
	file[1] = (char *)malloc(sizeof(char) * 20);

	parseArg(path, file);

	if (from == NULL)
		printf("Input File Not Found\n");
	else if (isFileExisting(metadata, file[1]))
		printf("File Existing\n");
	else
	{
		FILE *to = fopen("hardDisk.hdd", "rb+");

		if (to == NULL)
			printf("Output File can't be Created\n");
		else
		{
			metadata = toMetaData(metadata, file[1]);

			MD *Curr = toEndOfTable(metadata);

			long offset=(metasize)*blocksize, blockCount;

			if (metadata->next != NULL)
			{
				MD * prev= getOffSet(metadata);
				offset = (prev->start/blocksize + getSize(prev))*blocksize;
			}

			blockCount=offset/blocksize;

			int in = 0;

			char *buffer = (char *)malloc(sizeof(char)*blocksize),ch;
			int count = 0,count1;
			long len = 0, ind = 0;
			while (fread(&ch, 1, 1, from))
			{
				len++;
			}
			fclose(from);
			
			from = fopen(file[0], "rb");

			count = len / blocksize;
			
			count1 = count;
			
			fseek(to, offset, SEEK_CUR);

			Curr->start = ftell(to);
			
			while (count > 0)
			{
				Curr->blocks[ind++] = blockCount;
				blockCount++;
				buffer = readBlock(from,buffer);
				to = writeBlock(buffer, to, blocksize);
				count--;
			}

			buffer = readBlock(from, buffer);
			Curr->blocks[ind] = blockCount;

			to = writeBlock(buffer, to, (len-count1*blocksize));

			Curr->length = ftell(to);

			fclose(to);

		}

	}
	
	return metadata;
}

void copyFromDisk(MD *metadata,char *arg)
{
	char **file = (char **)malloc(sizeof(char *) * 2);
	file[0] = (char *)malloc(sizeof(char) * 20);
	file[1] = (char *)malloc(sizeof(char) * 20);

	parseArg(arg,file);

	int i = 0, op = 0,j;
	for (j = 0; arg[j] != ' '; j++)
	{
		file[0][i++] = arg[j];
	}
	file[0][i] = '\0';
	for (int k = j; arg[k] != '\0'; k++)
	{
		file[1][op++] = arg[k];
	}
	file[1][op] = '\0';

	FILE *from = fopen("hardDisk.hdd", "rb");

	if (from == NULL)
		printf("Input File Not Found\n");
	else if (isFileExisting(metadata, file[0])==0)
		printf("File Doesn't Exist\n");
	else
	{
		FILE *to = fopen(file[1], "wb");

		if (to == NULL)
			printf("Output File can't be Created\n");
		else
		{
			char c;

			MD *curr= getFileLoc(metadata, file[0], i);

			long offset = curr->start;

			fseek(from, offset, SEEK_CUR);
			
			long len=curr->length;

			char *buffer=(char *)malloc(sizeof(char)*blocksize);

			int ind=0;

			while (curr->blocks[ind+1]!=0)
			{
				buffer = readBlock(from, buffer);
				to = writeBlock(buffer,to,blocksize);
				ind++;
				offset += blocksize;
			}

			buffer = readBlock(from, buffer);

			long x = len-((curr->blocks[ind]) * blocksize);
			
			to = writeBlock(buffer, to,x);

			fclose(to);
		}
	}
}

MD * deleteFile(MD * metadata,char *file,char *freeBlocks)
{
	if (isFileExisting(metadata, file)==0)
	{
		printf("File Doesn't Exist\n");
	}
	else
	{
		MD * delFile = getFileLoc(metadata, file, strlen(file));

		long offset = delFile->start * blocksize;

		long len = delFile->length;

		long max = delFile->start;

		while (len < max)
		{
			freeBlocks[max] = '0';
			max = max + blocksize;
		}

		return deleteNode(metadata, delFile);
	}
	return metadata;
}

int main()
{
	char input[100];
	char *freeBlocks = (char *)malloc(sizeof(char) * 6000);
	for (int i = 0; i < 6000; i++)
	{
		freeBlocks[i] = '0';
	}
	MD *metadata=NULL;
	metadata=retrieveMeta(freeBlocks);
	while (1)
	{
		printf(">");
		gets_s(input);
		while (input[0] == '\0')
		{
			printf("\n>");
			gets_s(input);
		}
		int i = 0, k = 0;
		char *command = (char *)malloc(sizeof(char) * 100);
		char *arg = (char *)malloc(sizeof(char) * 100);

		if (input[i] != '\0')
		{
			while (input[i] == 32 || input[i] == 9)
			{
				i++;
			}
			while (input[i] != 32 && input[i] != '\0' && input[i] != 9)
			{
				command[k++] = input[i++];
			}
			command[k] = '\0';

			int k = 0;
			arg[k] = '\0';
			for (int j = i; input[i] != '\0';)
			{
				while (input[j] == 32 || input[j] == 9)
				{
					j++;
				}
				while (input[j] != '\0')
				{
					arg[k++] = input[j++];
				}
				arg[k] = '\0';
				break;
			}
		}

		if (compare(command, "copy-to-disk", 12))
			metadata = copyToDisk(metadata, arg, freeBlocks);
		else if (compare(command, "copy-from-disk", 14))
			copyFromDisk(metadata, arg);
		else if (compare(command, "ls", 2))
			filesList(metadata);
		else if (compare(command, "retrieve", 8))
			metadata = retrieveMeta(freeBlocks);
		else if (compare(command, "delete", 6))
			metadata=deleteFile(metadata,arg,freeBlocks);
		else if (compare(command, "print", 5))
			print(metadata);
		else if (compare(command, "exit", 4))
		{
			storeMeta(metadata,freeBlocks);
			break;
		}
		else if (compare(command, "format", 6))
		{
			metadata = NULL;
		}
		else
			printf("Invalid Command\n");
	}
	return 0;
}
