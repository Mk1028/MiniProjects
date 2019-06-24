#define maxfiles (50)

int numberOfFiles(FR *fileRecords)
{
	int i = 0;
	while (fileRecords[i].FileName[0] != '\0')
	{
		i++;
	}
	return i;
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

unsigned int str_to_number(char *left)
{
	unsigned int number = 0; int i = 0;
	while (left[i] != '\0')
	{
		number = number * 10 + left[i] - '0';
		i++;
	}
	return number;
}

MD * MDInitialize(unsigned int NumberOfBlocks, unsigned int BlockSize)
{
	MD *metadata = (MD *)malloc(sizeof(MD));
	metadata->MagicNumber = magicNumber;
	metadata->BlockSize = BlockSize;
	metadata->NoOfBlocks = NumberOfBlocks;
	metadata->EmptyBlocks = NumberOfBlocks;
	return metadata;
}

int isFileExisting(char *fileName, FR *fileRecords, int NumberOfFiles)
{
	int i = 0;
	for (; i < NumberOfFiles; i++)
	{
		if (compare(fileRecords[i].FileName, fileName, strlen(fileName)))
			return i;
	}
	return i;
}

FR * Creation()
{
	FR *fileRecords = (FR *)malloc(sizeof(FR) * maxfiles);
	//printf("%d\n", sizeof(FR));
	for (int i = 0; i < 50; i++)
	{
		fileRecords[i].FileName[0] = '\0';
		fileRecords[i].Size = 0;
		fileRecords[i].Start = 0;
		fileRecords[i].BlockCount = 0;
	}
	return fileRecords;
}

void readBlock1(FILE *fp, char *buffer)
{
	fread(buffer, BlockSize, 1, fp);
}

FILE * writeBlock1(char *buffer, FILE *fp, int bsize)
{
	fwrite(buffer, bsize, 1, fp);
	return fp;
}
