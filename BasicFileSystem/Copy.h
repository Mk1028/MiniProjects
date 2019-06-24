#include "header.h"

struct metadata {
	char filename[16];
	long start;
	long blocks[20];
	long length;
	metadata *next;
};

typedef struct metadata MD;

void parseArg(char *arg, char **file)
{
	int i = 0, op = 0, j;
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
}

int compare(char command[100], const char *str, int len)
{
	for (int i = 0; command[i] != '\0'; i++)
	{
		if (command[i] >= 'A' && command[i] <= 'Z')
			command[i] = command[i] + 32;
	}
	int i = 0;
	for (; str[i] != '\0' && command[i] != '\0'; i++)
	{
		if (command[i] != str[i])
			break;
	}
	if (command[i] == '\0' && i == len)
		return 1;
	return 0;
}

MD * getFileLoc(MD *metadata, char *inpFile, int len)
{
	MD *temp = metadata;
	while (temp != NULL)
	{
		if (compare(temp->filename, inpFile, len))
			return temp;
		else
			temp = temp->next;
	}
}

MD * newEntry()
{
	MD * metadata = (MD *)malloc(sizeof(MD));
	metadata->filename[0] = '\0';
	metadata->start = 0;
	for (int i = 0; i < 20; i++)
		metadata->blocks[i] = 0;
	metadata->length = 0;
	metadata->next = NULL;
	return metadata;
}

MD * getOffSet(MD *metadata)
{
	MD *temp = metadata, *prev = NULL;
	while (temp->next != NULL)
	{
		prev = temp;
		temp = temp->next;
	}
	return prev;
}

MD *toEndOfTable(MD *metadata)
{
	MD *temp;
	if (metadata == NULL)
	{
		return metadata;
	}
	else
	{
		temp = metadata;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		return temp;
	}
}

int getSize(MD * prev)
{
	int ind = 0;
	while (prev->blocks[ind] != 0)
	{
		ind++;
	}
	return ind;
}

int isFileExisting(MD *metadata, char *inpFile)
{
	MD *temp = metadata;
	if (temp == NULL)
		return 0;
	while (temp != NULL)
	{
		if (compare(temp->filename, inpFile, strlen(inpFile)))
			return 1;
		else
			temp = temp->next;
	}
	return 0;
}

MD* toMetaData(MD * metadata, char *outputFile)
{
	int len = strlen(outputFile), k = 0;

	MD *newNode = newEntry(), *temp;

	for (int j = 0; j < len; j++)
	{
		newNode->filename[k++] = outputFile[j];
	}
	newNode->filename[k] = '\0';

	if (metadata == NULL)
	{
		metadata = newNode;
	}
	else
	{
		temp = metadata;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = newNode;
	}
	return metadata;
}

MD * deleteNode(MD *metadata, MD *del)
{
	if (metadata = del)
	{
		metadata = metadata->next;
	}
	else
	{
		MD * temp = metadata->next, *prev = NULL;
		while (temp != del)
		{
			prev = temp;
			temp = temp->next;
		}
		prev->next = temp->next;
	}
	return metadata;
}
