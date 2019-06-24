#include "excel.h"

int get(char *arg, Cell **emptysheet);

int eval(char *expression, Cell **sheet) {
	char left[10], right[10];
	int index = 0, left_val, right_val;
	while (!is_operator(*expression) && *expression) {
		left[index++] = *expression;
		expression++;
	}

	left[index] = '\0';
	if (is_number(left))
		left_val = str_to_number(left);
	else
		left_val = get(left, sheet);

	if (!*expression)
		return left_val;

	char op = *expression++;

	index = 0;
	while (*expression) {
		right[index++] = *expression;
		expression++;
	}

	right[index] = '\0';

	if (is_number(right))
		right_val = str_to_number(right);
	else
		right_val = get(right, sheet);

	if (op == '+')
		return left_val + right_val;
	if (op == '-')
		return left_val - right_val;
	if (op == '*')
		return left_val * right_val;
	if (op == '/')
		return left_val / right_val;
}

void print(Cell **emptysheet)
{
	emptysheet[0][0].val = 32;
	for (int j = 1; j < 11; j++)
	{
		emptysheet[0][j].val = 65 + j - 1;
	}
	for (int i = 1; i < 11; i++)
	{
		emptysheet[i][0].val = i;
	}
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			if (i == 0)
				printf("%6c  |", emptysheet[i][j].val);
			else
			{
				if (emptysheet[i][j].expression == NULL)
					printf("%6d  |", emptysheet[i][j].val);
				else
				{
					printf("%6d  |", evaluate(emptysheet[i][j].expression, emptysheet));
				}
			}
		}
		printf("\n");
		for (int i = 1; i < 13; i++)
		{
			printf("%6c%c%c", 45, 45, 45);
		}
		printf("\n");
	}
}

int get(char *arg, Cell **emptysheet) {
	if (!validGetArg(arg))
	{
		printf("Invalid Format\n");
	}
	else
	{
		int row = arg[1] - 48, col = 0;
		if (arg[0] >= 'a' && arg[0] <= 'z')
		{
			col = arg[0] - 96;
		}
		else if (arg[0] >= 'A' && arg[0] <= 'Z')
		{
			col = arg[0] - 64;
		}
		if (arg[2] != '\0')
		{
			row = row * 10 + arg[2] - 48;
		}
		if (emptysheet[row][col].expression == NULL)
			return emptysheet[row][col].val;
		else
			return evaluate(emptysheet[row][col].expression,emptysheet);
	}
}

void set(char *arg, Cell **emptysheet) {
	/*removeSpaces(arg);
	int ret_val = validSetArg(arg);
	if (ret_val==0)
	{
		printf("Invalid Format\n");
	}
	else {*/
		int row = arg[1] - 48, col = 0;
		if (arg[0] >= 'a' && arg[0] <= 'z')
		{
			col = arg[0] - 96;
		}
		else if (arg[0] >= 'A' && arg[0] <= 'Z')
		{
			col = arg[0] - 64;
		}
		int i = 2;
		if (arg[i] >= 48 && arg[i] <= 57)
		{
			row = row * 10 + arg[i] - 48;
			i = 3;
		}
		/*if(ret_val == 1)
		{
			int val = 0;
			i++;
			while ((arg[i] >= 48 && arg[i] <= 57) && arg[i] != '\0')
			{
				val = val * 10 + arg[i] - 48;
				i++;
			}
			emptysheet[row][col].val = val;
		}
		else
		{*/
			char *expr = (char *)malloc(sizeof(char) * 10);
			i = 0;
			for (int j = 0; arg[j] != '\0';)
			{
				if (arg[j] == '=')
				{
					j++;
					while (arg[j] != '\0')
					{
						expr[i++] = arg[j++];
					}
					expr[i] = '\0';
					continue;
				}
				j++;
			}
			int *indices = (int *)malloc(sizeof(int) * 2);
			indices[0] = row;
			indices[1] = col;
			//change
			if (is_number(expr))
				emptysheet[row][col].val = str_to_number(expr);
			else
			{
				if (!hasCycle(expr, indices, emptysheet))
					emptysheet[row][col].expression = expr;
				else
					printf("Expression Has Cycle\n");
				//emptysheet[row][col].expression = (expr);
			}
		//}
	//}
}

char* export1(Cell **temp_excel, char *argument)
{
	if (!exportValidation(argument))
	{
		printf("Invalid Format\n");
		return '\0';
	}
	char *exprfile = (char*)malloc(sizeof(char) * 1000);
	strcpy(exprfile, argument);
	char *updatedfilename = (char*)malloc(sizeof(char) * 1000);
	int length = strlen(argument);
	const char *extend = ".csv";
	if (!(argument[length - 1] == extend[3] && argument[length - 2] == extend[2] && argument[length - 3] == extend[1] && argument[length - 4] == extend[0]))
		strcat(argument, extend);
	FILE *fstream = fopen(argument, "w");
	for (int row = 1; row <= 10; row++)
	{
		for (int col = 1; col <= 10; col++)
		{
			if (col == 10)
			{
				if(temp_excel[row][col].expression==NULL)
					fprintf(fstream, "%d\n", temp_excel[row][col].val);
				else
					fprintf(fstream, "%d\n", evaluate(temp_excel[row][col].expression, temp_excel));
			}
				
			else
			{
				if (temp_excel[row][col].expression == NULL)
					fprintf(fstream, "%d,", temp_excel[row][col].val);
				else

					fprintf(fstream, "%d,", evaluate(temp_excel[row][col].expression, temp_excel));
			}
				
		}
	}
	fclose(fstream);

	strcpy(updatedfilename, argument);

	//Exporting Cell Expressions
	const char *xtend = ".txt";
	if (!(exprfile[length - 1] == extend[3] && exprfile[length - 2] == extend[2] && exprfile[length - 3] == extend[1] && exprfile[length - 4] == extend[0]))
		strcat(exprfile, xtend);
	FILE *fstream1 = fopen(exprfile, "w");
	for (int row = 1; row <= 10; row++)
	{
		for (int col = 1; col <= 10; col++)
		{
			if(temp_excel[row][col].expression!=NULL)
				fprintf(fstream1, "%d %d %s\n", row,col,temp_excel[row][col].expression);
		}
	}
	fclose(fstream1);
	return updatedfilename;
}

char* import(Cell **temp_excel, char *argument)
{
	char *updatedfilename = (char*)malloc(sizeof(char) * 1000);
	char *exprfile = (char*)malloc(sizeof(char) * 1000);
	strcpy(exprfile, argument);
	int length = strlen(argument);
	const char *extend = ".csv";
	if (!(argument[length - 1] == extend[3] && argument[length - 2] == extend[2] && argument[length - 3] == extend[1] && argument[length - 4] == extend[0]))
		strcat(argument, extend);
	FILE *fstream;
	fopen_s(&fstream, argument, "r");
	if (fstream == NULL)
	{
		printf("FILE NOT FOUND\n");
		return NULL;
	}
	for (int row = 1; row <= 10; row++)
	{
		for (int col = 1; col <= 10; col++)
		{
			fscanf_s(fstream, "%d,", &temp_excel[row][col].val);
		}
	}
	fclose(fstream);
	strcpy(updatedfilename, argument);

	const char *xtend = ".txt";
	if (!(exprfile[length - 1] == extend[3] && exprfile[length - 2] == extend[2] && exprfile[length - 3] == extend[1] && exprfile[length - 4] == extend[0]))
		strcat(exprfile, xtend);
	FILE *fstream1 = fopen(exprfile, "r");
	char *str = ( char * ) malloc ( sizeof ( char ) * 80 );
	char *exp = (char *)malloc(sizeof(char) * 80);
	int *rowCol=(int *)malloc(sizeof(int) * 2);
	int ind,i,k=0;
	while (fgets(str, 50, fstream1) != NULL)
	{
		ind = 0; k = 0;
		for (int i = 0; str[i] != '\n'; i++)
		{
			if (str[i] == ' ')
			{
				exp[ind] = '\0';
				rowCol[k++] = str_to_number(exp);
				ind = 0;
				continue;
			}
			exp[ind++] = str[i];
		}
		exp[ind] = '\0';
		temp_excel[rowCol[0]][rowCol[1]].expression = (char *)malloc(sizeof(char) * 20);
		ind = 0;
		for (;exp[ind]; ind++)
			temp_excel[rowCol[0]][rowCol[1]].expression[ind] = exp[ind];
		temp_excel[rowCol[0]][rowCol[1]].expression[ind] = '\0';
	}
	fclose(fstream1);

	return updatedfilename;
}

void save(Cell **temp_excel, char *filename)
{
	char *exprfile = (char*)malloc(sizeof(char) * 1000);
	strcpy(exprfile, filename);
	FILE *fstream = fopen(filename, "w+");
	for (int row = 1; row <= 10; row++)
	{
		for (int col = 1; col <= 10; col++)
		{
			if (col == 10)
			{
				if (temp_excel[row][col].expression == NULL)
					fprintf(fstream, "%d\n", temp_excel[row][col].val);
				else
					fprintf(fstream, "%d\n", evaluate(temp_excel[row][col].expression, temp_excel));
			}

			else
			{
				if (temp_excel[row][col].expression == NULL)
					fprintf(fstream, "%d,", temp_excel[row][col].val);
				else
					fprintf(fstream, "%d,", evaluate(temp_excel[row][col].expression, temp_excel));
			}
		}
	}
	fclose(fstream);

	int len = strlen(exprfile);
	exprfile[len - 1] = 't'; exprfile[len - 2] = 'x'; exprfile[len - 3] = 't';
	FILE *fstream1 = fopen(exprfile, "w");
	for (int row = 1; row <= 10; row++)
	{
		for (int col = 1; col <= 10; col++)
		{
			if (temp_excel[row][col].expression != NULL)
				fprintf(fstream1, "%d %d %s\n", row, col, temp_excel[row][col].expression);
		}
	}
	fclose(fstream1);
}

Cell ** creation()
{
	Cell **start = (Cell **)malloc(sizeof(Cell *) * 11);
	for (int i = 0; i < 11; i++)
		start[i] = (Cell *)malloc(sizeof(Cell) * 11);
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			start[i][j].val = 0;
			start[i][j].expression = NULL;
		}
	}
	return start;
}

int main()
{
	char input[100];
	char *expFile = (char *)malloc(sizeof(char) * 100); expFile = NULL;
	char *impFile = (char *)malloc(sizeof(char) * 100); impFile = NULL;
	Cell **emptysheet = creation();
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
		if (compare(command, "get", 3))
			printf("%d\n", get(arg, emptysheet));
		else if (compare(command, "set", 3))
			set(arg, emptysheet);
		else if (compare(command, "print", 5))
			print(emptysheet);
		else if (compare(command, "export", 6))
		{
			if (!exportValidation(arg))
				printf("Invalid Format\n");
			else
				expFile = export1(emptysheet, arg);
		}
		else if (compare(command, "import", 6))
		{
			if (!exportValidation(arg))
				printf("Invalid Format\n");
			else
				impFile = import(emptysheet, arg);
		}
		else if (compare(command, "save", 4))
		{
			if (saveValidation(arg))
			{
				if (impFile)
					save(emptysheet, impFile);
				else if (expFile)
					save(emptysheet, expFile);
				else
					printf("No File to Save\n");
			}
			else
				printf("Invalid Format\n");
		}
		else if (compare(command, "exit", 4))
			break;
		else if (compare(command, "clear", 5))
			system("cls");
		else if (compare(command, "help", 4))
			commands();
		else
			printf("Invalid Command\n");
	}
	return 0;
}