#include "InterMediateFunctions.h"

IL * processIF(IL *IntermediateLang, int index, char **arg, int *stack, int *top)
{
	IntermediateLang[index].Opcode = 7;
	IntermediateLang[index].par[0] = AddressOfRegister(arg[0]);
	IntermediateLang[index].par[1] = AddressOfRegister(arg[2]);
	IntermediateLang[index].par[2] = getOpCode(arg[1]);

	*top += 1;
	stack[*top] = index;

	return IntermediateLang;
}

IL * processElse(IL *IntermediateLang, int *stack, int *top, int index)
{
	IntermediateLang[index].Opcode = 6;

	*top += 1;
	stack[*top] = index;

	return IntermediateLang;
}

IL * processEndIf(IL *IntermediateLang, int *stack, int *top, int index)
{
	int ind = 0;
	if (IntermediateLang[stack[*top]].Opcode != 7)
	{
		for (; ind < 4; ind++)
		{
			if (IntermediateLang[stack[*top]].par[ind] == -1)
			{
				IntermediateLang[stack[*top]].par[ind] = index + 1;
				index = stack[*top];
				break;
			}
		}
		*top -= 1;
	}

	IntermediateLang[stack[*top]].par[3] = index + 1;

	*top -= 1;

	return IntermediateLang;
}

IL * processArithmetic(IL *IntermediateLang, int index, char *Command, char *operands)
{
	char **args = (char **)malloc(sizeof(char *) * 10);

	for (int i = 0; i < 10; i++)
	{
		args[i] = (char *)malloc(sizeof(char) * 20);
	}

	removeSpaces(operands);
	tokenizer(operands, args, ',');

	IntermediateLang[index].Opcode = getOpCode(Command);
	IntermediateLang[index].par[0] = AddressOfRegister(args[0]);
	IntermediateLang[index].par[1] = AddressOfRegister(args[1]);
	IntermediateLang[index].par[2] = AddressOfRegister(args[2]);

	return IntermediateLang;
}

IL *processMove(IL *IntermediateLang, ST *SymbolTable, int index, char **args)
{
	int len = strlen(args[0]);
	if (args[0][len - 1] == 'X')
	{
		IntermediateLang[index].Opcode = 2;
		IntermediateLang[index].par[0] = AddressOfRegister(args[0]);

		len = strlen(args[1]);
		if (len == 1)
		{
			IntermediateLang[index].par[1] = getAddressOfVar(SymbolTable, args[1]);

		}
		else
		{
			char *ch = (char *)malloc(sizeof(char) * 2);
			ch[0] = args[1][0]; ch[1] = '\0';

			char *ch1 = (char *)malloc(sizeof(char) * 2);
			ch1[0] = args[1][2]; ch1[1] = '\0';
			IntermediateLang[index].par[1] = getAddressOfVar(SymbolTable, ch) + str_to_number1(ch1);

		}
	}
	else
	{
		IntermediateLang[index].Opcode = 1;

		len = strlen(args[0]);
		if (len == 1)
		{
			IntermediateLang[index].par[0] = getAddressOfVar(SymbolTable, args[0]);

		}
		else
		{
			char *ch = (char *)malloc(sizeof(char) * 2);
			ch[0] = args[0][0]; ch[1] = '\0';

			char *ch1 = (char *)malloc(sizeof(char) * 2);
			ch1[0] = args[0][2]; ch1[1] = '\0';
			IntermediateLang[index].par[0] = getAddressOfVar(SymbolTable, ch) + str_to_number1(ch1);
		}
		IntermediateLang[index].par[1] = AddressOfRegister(args[1]);
	}

	return IntermediateLang;
}

IL * processInstructions(ST *SymbolTable,char *buffer, IL *IntermediateLang,int count)
{
	char *input = (char *)malloc(sizeof(char) * 30);
	char **Command = (char **)malloc(sizeof(char *) * 2);

	LT *LocalTable = InitializeLT();
	int LTindex = 0;

	int *stack = (int *)malloc(sizeof(int) * maxSize), top = -1;

	int index = 0;

	for (int i = 0; i < 2; i++)
	{
		Command[i] = (char *)malloc(sizeof(char) * 20);
	}

	char **args = (char **)malloc(sizeof(char *) * 10);

	for (int i = 0; i < 10; i++)
	{
		args[i] = (char *)malloc(sizeof(char) * 20);
	}

	input = strtok(buffer, "\n");
	trim(input, '\r');

	while (count > 0)
	{
		input = strtok(NULL,"\n");
		count--;
	}
	input = strtok(NULL, "\n");
	trim(input, '\r');
	parseCommand1(input, Command);
	removeSpaces(Command[1]);

	while (compare1(Command[0], (char *)"END", 3) == 0)
	{
		if (compare1(Command[0], (char *)"READ", 4))
		{
			IntermediateLang[index].Opcode = getOpCode(Command[0]);
			IntermediateLang[index].par[0] = AddressOfRegister(Command[1]);
		}
		else if (compare1(Command[0], (char *) "MOV", 3))
		{
			removeSpaces(Command[1]);
			tokenizer(Command[1], args, ',');

			IntermediateLang = processMove(IntermediateLang, SymbolTable, index, args);
		}
		else if (compare1(Command[0], (char *) "ADD", 3))
		{
			IntermediateLang = processArithmetic(IntermediateLang, index, Command[0], Command[1]);
		}
		else if (compare1(Command[0], (char *)"SUB", 3))
		{
			IntermediateLang = processArithmetic(IntermediateLang, index, Command[0], Command[1]);
		}

		else if (compare1(Command[0], (char *) "MUL", 3))
		{
			IntermediateLang = processArithmetic(IntermediateLang, index, Command[0], Command[1]);
		}

		else if (compare1(Command[0], (char *)"PRINT", 5))
		{
			IntermediateLang[index].Opcode = getOpCode(Command[0]);

			int len = strlen(Command[1]);
			if (Command[1][len - 1] == 'X')
				IntermediateLang[index].par[0] = AddressOfRegister(Command[1]);
			else
			{
				len = strlen(Command[1]);
				if (len == 1)
				{
					IntermediateLang[index].par[0] = getAddressOfVar(SymbolTable, Command[1]);
				}
				else
				{
					char *ch = (char *)malloc(sizeof(char) * 2);
					ch[0] = Command[1][0]; ch[1] = '\0';

					char *ch1 = (char *)malloc(sizeof(char) * 2);
					ch1[0] = Command[1][2]; ch1[1] = '\0';

					IntermediateLang[index].par[0] = getAddressOfVar(SymbolTable, ch) + str_to_number1(ch1);

				}
			}
		}

		else if (compare1(Command[0], (char *)"IF", 2))
		{
			tokenizer(Command[1], args, ' ');
			IntermediateLang = processIF(IntermediateLang, index, args, stack, &top);
		}
		else if (compare1(Command[0], (char *)"ELSE", 4))
		{
			tokenizer(Command[1], args, ' ');
			IntermediateLang = processElse(IntermediateLang, stack, &top, index);
		}
		else if (compare1(Command[0], (char *)"ENDIF", 5))
		{
			IntermediateLang = processEndIf(IntermediateLang, stack, &top, index);
			index--;
		}
		else if (compare1(Command[0], (char *)"JUMP", 4))
		{
			IntermediateLang[index].Opcode = 6;
			IntermediateLang[index].par[0] = addressOfBlock(Command[1], LocalTable);
		}
		else
		{
			trim(input, '\r');
			trim(Command[0],':');
			strcpy(LocalTable[LTindex].BlockName, _strupr(Command[0]));
			LocalTable[LTindex++].Address = index;
			index--;
		}

		input = strtok(NULL, "\n");
		trim(input, '\r');

		if (input[0]=='E' && input[1] == 'N' && input[2] == 'D' && input[3]!='I')
		{
			break;
		}

		parseCommand1(input, Command);
		index++;
	}

	return IntermediateLang;
}

IL * buildSymbolTable(IL *IntermediateLang, char *buffer, ST *SymbolTable, int *memory)
{
	char *input = (char *)malloc(sizeof(char) * 30);
	char **Command = (char **)malloc(sizeof(char *) * 2);
	char *buffer1 = (char *)malloc(sizeof(char)*BlockSize);

	strcpy(buffer1, buffer);

	for (int i = 0; i < 2; i++)
	{
		Command[i] = (char *)malloc(sizeof(char) * 20);
	}

	int index = 0, len = 0,count=0;
	char *number = (char *)malloc(sizeof(char) * 5);

	input = strtok(buffer, "\n");
	trim(input, '\r');

	while (compare1(input, (char *)"START:", 6) == 0)
	{
		parseCommand1(input, Command);

		removeSpaces(Command[1]);
		int i, k = 0;

		if (compare1(Command[0], (char *) "DATA", 4))
		{
			if (Command[1][0] >= 'A' && Command[1][0] <= 'Z')
			{
				number[0] = '1';
				number[1] = '\0';
				SymbolTable[index].Name[0] = Command[1][0];
				SymbolTable[index].Name[1] = '\0';
				strcpy(SymbolTable->Type, "int");
				len = strlen(Command[1]);

				if (Command[1][1] == '[' && Command[1][len - 1] == ']')
				{

					for (i = 2; i < len - 1; i++)
						number[k++] = Command[1][i];
					number[k] = '\0';
					strcpy(SymbolTable->Type, "array");
				}
			}
			SymbolTable->Size = str_to_number1(number);
			if (index == 0)
				SymbolTable[index].Address = str_to_number1(number) - 1;
			else
				SymbolTable[index].Address = str_to_number1(number) + SymbolTable[index - 1].Address;
		}
		else if (compare1(Command[0], (char *)"CONST", 5))
		{
			char num[2];
			int len, i, k = 0;

			num[0] = '1'; num[1] = '\0';

			if (Command[1][0] >= 'A' && Command[1][0] <= 'Z')
			{
				len = strlen(Command[1]);

				if (Command[1][1] == '=')
				{
					for (i = 2; i < len; i++)
						number[k++] = Command[1][i];
					number[k] = '\0';
				}
			}
			SymbolTable[index].Name[0] = Command[1][0];
			SymbolTable[index].Name[1] = '\0';
			strcpy(SymbolTable->Type, "const");
			SymbolTable->Size = 1;
			if (index == 0)
				SymbolTable[index].Address = str_to_number1(num) - 1;
			else
				SymbolTable[index].Address = str_to_number1(num) + SymbolTable[index - 1].Address;

			memory[SymbolTable[index].Address] = str_to_number1(number);
		}
		input = strtok(NULL, "\n");
		trim(input, '\r');
		index++;

		count++;
	}

	return processInstructions(SymbolTable, buffer1, IntermediateLang,count);
}

void writeToFile(IL *IntermediateLang,char *opFile)
{
	FILE *outputFile = fopen(opFile, "w");
	int ind = 0;

	if (outputFile == NULL)
	{
		printf("File Can't be Opened");
	}
	else
	{
		while (IntermediateLang[ind].Opcode != 0)
		{
			fprintf(outputFile, "%d ", IntermediateLang[ind].Opcode);

			for (int i = 0; i < 4; i++)
			{
				if (IntermediateLang[ind].par[i] != -1)
					fprintf(outputFile, "%d ", IntermediateLang[ind].par[i]);
			}
			fprintf(outputFile, "%c", '\n');

			ind++;
		}

	}

	fclose(outputFile);
}

void compiler(char *buffer,char *outputFile)
{
	ST *SymbolTable = InitializeST();

	int *memory = (int *)malloc(sizeof(int)*MaxMemory);

	IL *IntermediateLang = InitializeIL();

	IntermediateLang = buildSymbolTable(IntermediateLang,buffer,SymbolTable,memory);

	writeToFile(IntermediateLang,outputFile);
}

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

void IntermediateCodeFromDisk(FR *fileRecords, char *file1, char *file2, char *ByteVector)
{
	int NumberOfFiles = numberOfFiles(fileRecords);
	int curr = isFileExisting(file1,fileRecords,NumberOfFiles);

	if (curr == NumberOfFiles)
		printf("%s Doesn't Exist\n",file1);
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

		readBlock(buffer, blocks[ind++]);

		compiler(buffer, file2);
	}
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
		else if (compare(Command[0], "generatecode", 12))
			IntermediateCodeFromDisk(fileRecords, Command[1], Command[2], ByteVector);
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
			metadata = MDInitialize(NumberOfBlocks, str_to_number(Command[2]));
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