#include "InterMediateFunctions.h"

void  buildSymbolTable(FILE *inpFile,ST *SymbolTable, int *memory)
{
	char *input=(char *)malloc(sizeof(char)*30);
	char **Command = (char **)malloc(sizeof(char *) * 2);

	for (int i = 0; i < 2; i++)
	{
		Command[i] = (char *)malloc(sizeof(char) * 20);
	}

	int index = 0, len = 0;
	char *number = (char *)malloc(sizeof(char) * 5);

	fgets(input, 30, inpFile);
	trim(input);

	while (compare(input,(char *)"START:",6)==0)
	{
		parseCommand(input, Command);

		removeSpaces(Command[1]);
		int i, k = 0;

		if (compare(Command[0], (char *) "DATA", 4))
		{
			if (Command[1][0] >= 'A' && Command[1][0] <= 'Z')
			{
				number[0] = '1';
				number[1] = '\0';
				SymbolTable[index].Name[0]=Command[1][0];
				SymbolTable[index].Name[1] ='\0';
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
			SymbolTable->Size = str_to_number(number);
			if (index == 0)
				SymbolTable[index].Address = str_to_number(number)-1;
			else
				SymbolTable[index].Address = str_to_number(number) + SymbolTable[index-1].Address;
		}
		else if (compare(Command[0], (char *)"CONST", 5))
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
				SymbolTable[index].Address = str_to_number(num) - 1;
			else
				SymbolTable[index].Address = str_to_number(num) + SymbolTable[index - 1].Address;

			memory[SymbolTable[index].Address] = str_to_number(number);
		}
		fgets(input,30,inpFile);
		trim(input);
		index++;
	}
}

IL * processIF(IL *IntermediateLang,int index,char **arg,int *stack,int *top)
{
	IntermediateLang[index].Opcode = 7;
	IntermediateLang[index].par[0] = AddressOfRegister(arg[0]);
	IntermediateLang[index].par[1] = AddressOfRegister(arg[2]);
	IntermediateLang[index].par[2] = getOpCode(arg[1]);
	
	*top+=1;
	stack[*top] = index;

	return IntermediateLang;
}

IL * processElse(IL *IntermediateLang,int *stack,int *top,int index)
{
	IntermediateLang[index].Opcode = 6;

	*top+=1;
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
		*top-=1;
	}

	IntermediateLang[stack[*top]].par[3] = index + 1;

	*top-=1;

	return IntermediateLang;
}

IL * processArithmetic(IL *IntermediateLang, int index, char *Command,char *operands)
{
	char **args = (char **)malloc(sizeof(char *) * 10);

	for (int i = 0; i < 10; i++)
	{
		args[i] = (char *)malloc(sizeof(char) * 20);
	}

	removeSpaces(operands);
	tokenizer(operands,args,',');

	IntermediateLang[index].Opcode = getOpCode(Command);
	IntermediateLang[index].par[0] = AddressOfRegister(args[0]);
	IntermediateLang[index].par[1] = AddressOfRegister(args[1]);
	IntermediateLang[index].par[2] = AddressOfRegister(args[2]);

	return IntermediateLang;
}

IL *processMove(IL *IntermediateLang,ST *SymbolTable,int index, char **args)
{
	int len = strlen(args[0]);
	if (args[0][len - 1] == 'X')
	{
		IntermediateLang[index].Opcode = 2;
		IntermediateLang[index].par[0] = AddressOfRegister(args[0]);

		len = strlen(args[1]);
		if (len==1)
		{
			IntermediateLang[index].par[1] = getAddressOfVar(SymbolTable,args[1]);

		}
		else
		{
			char *ch = (char *)malloc(sizeof(char) * 2);
			ch[0] = args[1][0]; ch[1] = '\0';

			char *ch1 = (char *)malloc(sizeof(char) * 2);
			ch1[0] = args[1][2]; ch1[1] = '\0';
			IntermediateLang[index].par[1] = getAddressOfVar(SymbolTable, ch) + str_to_number(ch1);

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
			char *ch = (char *)malloc(sizeof(char)*2);
			ch[0] = args[0][0]; ch[1] = '\0';

			char *ch1 = (char *)malloc(sizeof(char) * 2);
			ch1[0] = args[0][2]; ch1[1] = '\0';
			IntermediateLang[index].par[0] = getAddressOfVar(SymbolTable, ch) + str_to_number(ch1);
		}
		IntermediateLang[index].par[1] = AddressOfRegister(args[1]);
	}

	return IntermediateLang;
}

IL * processInstructions(ST *SymbolTable,FILE *inpFile,IL *IntermediateLang)
{
	char *input = (char *)malloc(sizeof(char) * 30);
	char **Command = (char **)malloc(sizeof(char *) * 2);

	LT *LocalTable = InitializeLT();
	int LTindex = 0;

	int *stack= (int *)malloc(sizeof(int) * maxSize),top=-1;

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

	fgets(input, 30, inpFile);
	trim(input);

	parseCommand(input, Command);
	removeSpaces(Command[1]);

	while (compare(Command[0], (char *)"END",3)==0)
	{
		if (compare(Command[0], (char *)"READ", 4))
		{
			IntermediateLang[index].Opcode = getOpCode(Command[0]);
			IntermediateLang[index].par[0] = AddressOfRegister(Command[1]);
		}
		else if (compare(Command[0], (char *) "MOV", 3))
		{
			removeSpaces(Command[1]);
			tokenizer(Command[1], args, ',');
			
			IntermediateLang = processMove(IntermediateLang, SymbolTable, index, args);
		}
		else if (compare(Command[0], (char *) "ADD", 3))
		{
			IntermediateLang = processArithmetic(IntermediateLang,index,Command[0],Command[1]);
		}
		else if (compare(Command[0], (char *)"SUB", 3))
		{
			IntermediateLang = processArithmetic(IntermediateLang, index, Command[0], Command[1]);
		}
				
		else if (compare(Command[0], (char *) "MUL", 3))
		{
			IntermediateLang = processArithmetic(IntermediateLang, index, Command[0], Command[1]);
		}
		
		else if (compare(Command[0], (char *)"PRINT", 5))
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

					IntermediateLang[index].par[0] = getAddressOfVar(SymbolTable, ch) + str_to_number(ch1);

				}
			}
		}

		else if (compare(Command[0], (char *)"IF", 2))
		{
			tokenizer(Command[1], args, ' ');
			IntermediateLang = processIF(IntermediateLang,index, args,stack,&top);
		}
		else if (compare(Command[0], (char *)"ELSE", 4))
		{
			tokenizer(Command[1], args, ' ');
			IntermediateLang=processElse(IntermediateLang,stack, &top,index);
		}
		else if (compare(Command[0], (char *)"ENDIF", 5))
		{
			IntermediateLang = processEndIf(IntermediateLang, stack, &top, index);
			index--;
		}
		else if (compare(Command[0], (char *)"JUMP", 4))
		{
			IntermediateLang[index].Opcode = 6;
			IntermediateLang[index].par[0] = addressOfBlock(Command[1],LocalTable);
		}
		else
		{
			trim(Command[0]);
			strcpy(LocalTable[LTindex].BlockName,_strupr(Command[0]));
			LocalTable[LTindex++].Address = index;
			index--;
		}

		fgets(input, 30, inpFile);
		trim(input);

		parseCommand(input, Command);
		index++;
	}

	return IntermediateLang;
}

void writeToFile(IL *IntermediateLang)
{
	FILE *outputFile = fopen("output.txt", "w");
	int ind = 0;

	while (IntermediateLang[ind].Opcode != 0)
	{
		fprintf(outputFile, "%d ", IntermediateLang[ind].Opcode);

		for (int i = 0; i < 4; i++)
		{
			if (IntermediateLang[ind].par[i] != -1)
				fprintf(outputFile, "%d ", IntermediateLang[ind].par[i]);
		}
		fprintf(outputFile, "%c",'\n');

		ind++;
	}

	fclose(outputFile);
}

int main()
{
	ST *SymbolTable = InitializeST();

	FILE *inpFile = fopen("input3.txt","r");

	int *memory = (int *)malloc(sizeof(int)*MaxMemory);

	buildSymbolTable(inpFile,SymbolTable,memory);

	IL *IntermediateLang = InitializeIL();
	IntermediateLang =processInstructions(SymbolTable, inpFile, IntermediateLang);

	writeToFile(IntermediateLang);

	return 0;
}