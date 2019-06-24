#define _CRT_SECURE_NO_WARNINGS
# include <stdio.h> 
# include <string.h> 
#include<conio.h>
#include<stdlib.h>

struct cell
{
	int val;
	char *expression;
};

typedef struct cell Cell;
int precedence(char op) {
	if (op == '+' || op == '-')
		return 1;
	if (op == '*' || op == '/')
		return 2;
	if (op == '%')
		return 3;
	return 0;
}

int applyOp(int a, int b, char op) {
	switch (op) {
	case '+': return a + b;
	case '-': return a - b;
	case '*': return a * b;
	case '/': return a / b;
	case '%': return a % b;
	}
	return 0;
}

int isOperator(char ch)
{
	switch (ch) {
	case '+': return 1;
	case '-': return 1;
	case '*': return 1;
	case '/': return 1;
	case '%': return 1;
	}
	return 0;
}

int isChar(char ch)
{
	if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z')
		return 1;
	return 0;
}

int isdigit(char ch)
{
	switch (ch) {
	case '0': return 1;
	case '1': return 1;
	case '2': return 1;
	case '3': return 1;
	case '4': return 1;
	case '5': return 1;
	case '6': return 1;
	case '7': return 1;
	case '8': return 1;
	case '9': return 1;
	}
	return 0;
}

int evaluate(char *expr,Cell **sheet) {
	int expr_index = 0;

	int *values_stack = (int *)malloc(30 * (sizeof(int)));
	int values_top = -1;

	char *operators_stack = (char *)malloc(30 * sizeof(char));
	int operators_top = -1;

	char character;
	for (expr_index = 0; expr[expr_index] != '\0'; expr_index++) {
		character = expr[expr_index];
		if (character == ' ')
			continue;
		if (character == '(')
			operators_stack[++operators_top] = character;
		else if (character == ')')
		{
			while (operators_top != -1 && operators_stack[operators_top] != '(')
			{
				int val2 = values_stack[values_top--];
				int val1 = values_stack[values_top--];
				char op = operators_stack[operators_top--];

				values_stack[++values_top] = applyOp(val1, val2, op);
			}
			operators_top--;
		}
		else if (isdigit(character)) {
			int val = 0;
			while (expr[expr_index] != '\0' && isdigit(character))
			{
				val = (val * 10) + (character - '0');
				expr_index++;
				character = expr[expr_index];
			}
			values_stack[++values_top] = val;
			expr_index--;
		}
		else if (isChar(character))
		{
			int row = 0,col;
			if (character >= 'a' && character <= 'z')
				col = character - 96;
			else if (character >= 'A' && character <= 'Z')
				col = character - 64;
			expr_index++;
			character = expr[expr_index];
			while (expr[expr_index] != '\0' && isdigit(character))
			{
				row = (row * 10) + (character - '0');
				expr_index++;
				character = expr[expr_index];
			}
			values_stack[++values_top] = sheet[row][col].val;
			expr_index--;
		}
		else if (isOperator(character)) {

			while (operators_top != -1 && precedence(operators_stack[operators_top]) >= precedence(character)) {
				int val2 = values_stack[values_top--];
				int val1 = values_stack[values_top--];
				char op = operators_stack[operators_top--];

				values_stack[++values_top] = applyOp(val1, val2, op);
			}
			operators_stack[++operators_top] = character;
		}
	}
	while (operators_top != -1) {
		int val2 = values_stack[values_top--];
		int val1 = values_stack[values_top--];
		char op = operators_stack[operators_top--];

		values_stack[++values_top] = applyOp(val1, val2, op);
	}

	return values_stack[values_top];
}

int hasCycle(char *expr, int *indices, Cell **sheet);

int str_to_number(char left[10])
{
	int number = 0, i = 0;
	while (left[i] != '\0')
	{
		number = number * 10 + left[i] - '0';
		i++;
	}
	return number;
}

int is_number(char left[10])
{
	int i = 0;
	while (left[i] != '\0' && (left[i] >= '0' &&left[i] <= '9'))
	{
		i++;
	}
	if (left[i] == '\0')
		return 1;
	return 0;
}

int is_operator(char oper)
{
	if (oper == '+' || oper == '-' || oper == '*' || oper == '/')
		return 1;
	return 0;
}

int is_PointingToCell(char arg[4])
{
	if (arg[0] >= 'a' && arg[0] <= 'j' || arg[0] >= 'A' && arg[0] <= 'J')
	{

		if (arg[1] >= 48 && arg[1] <= 57)
		{
			int row = arg[1] - 48;
			if (arg[2] == '\0' || (arg[2] >= 48 && arg[2] <= 57))
			{
				if (arg[2] == '\0')
				{
					return 1;
				}
				else
				{
					row = row * 10 + arg[2] - 48;
					if (row <= 10 && arg[3] == '\0')
					{
						return 1;
					}
					return 0;

				}
			}
			return 0;
		}
		return 0;
	}
	return 0;
}

void cellIndices(char *cell,int *ind)
{
	int row = cell[1] - 48, col = 0;
	if (cell[0] >= 'a' && cell[0] <= 'z')
	{
		col = cell[0] - 96;
	}
	else if (cell[0] >= 'A' && cell[0] <= 'Z')
	{
		col = cell[0] - 64;
	}
	if (cell[2] >= 48 && cell[2] <= 57)
	{
		row = row * 10 + cell[2] - 48;
	}
	ind[0] = row;
	ind[1] = col;
}

int verify(char left[10], int *indices,Cell **sheet)
{
	int *ind = (int *)malloc(sizeof(int) * 2);
	if (!is_number(left))
	{
		cellIndices(left, ind);
		if (indices[0] == ind[0] && indices[1] == ind[1])
		{
			return 1;
		}
		else if (sheet[ind[0]][ind[1]].expression != NULL)
			return hasCycle(sheet[ind[0]][ind[1]].expression, indices, sheet);
	}
	return 0;
}

int divideCells(char *expr, int *indices, Cell **sheet,int flag)
{
	char left[10];
	int l;
	int index = 0;
	while (*expr && flag!=1) {
		if (is_operator(*expr))
		{
			expr++;
			flag = 1;
			continue;
		}
		else if (*expr==')' || *expr=='(')
		{
			expr++;
			continue;
		}
		left[index++] = *expr;
		expr++;
	}

	left[index] = '\0';
	l = verify(left, indices, sheet);
	if ((!*expr || l == 1))
		return l;
	return divideCells(expr, indices, sheet,0);
}
int hasCycle(char *expr, int *indices,Cell **sheet) 
{
	return divideCells(expr, indices, sheet,1);
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

void removeSpaces(char *str)
{
	int count = 0;
	for (int i = 0; str[i]; i++) {
		if (str[i] != ' ' && str[i] != 9)
			str[count++] = str[i];
	}
	str[count] = '\0';
}

int validGetArg(char arg[100])
{
	if (arg[0] >= 'a' && arg[0] <= 'j' || arg[0] >= 'A' && arg[0] <= 'J')
	{
		if (arg[1] >= 48 && arg[1] <= 57)
		{
			int row = arg[1] - 48;
			if (arg[2] >= 48 && arg[2] <= 57 || (arg[2] == '\0') || (arg[2] == 32))
			{
				if (arg[2] >= 48 && arg[2] <= 57 || (arg[2] == 32))
				{
					if (arg[2] == 32)
					{
						int i = 3;
						while (arg[i] == 32) { i++; }
						if (arg[i] == '\0')
							return 1;
						return 0;
					}
					row = row * 10 + arg[2] - 48;
					if ((row <= 10 && arg[3] == '\0') || (row <= 10 && arg[3] == 32))
					{
						if (arg[3] == 32)
						{
							int i = 4;
							while (arg[i] == 32) { i++; }
							if (arg[i] == '\0')
								return 1;
							return 0;
						}
						return 1;
					}
					return 0;
				}
				return 1;
			}
			return 0;
		}
		return 0;
	}
	return 0;
}

/*int validSetArg(char arg[100])
{
	if (arg[0] >= 'a' && arg[0] <= 'j' || arg[0] >= 'A' && arg[0] <= 'J')
	{
		if (arg[1] >= 48 && arg[1] <= 57)
		{
			int row = arg[1] - 48;
			if ((arg[2] >= 48 && arg[2] <= 57) || arg[2] == 61)
			{
				int i = 2;
				if (arg[2] >= 48 && arg[2] <= 57)
				{
					row = row * 10 + arg[2] - 48;
					i = 3;
				}
				if (row <= 10 && arg[i] == 61)
				{
					i++;
					if ((arg[i] >= 48 && arg[i] <= 57))
					{
						i++;
						while ((arg[i] >= 48 && arg[i] <= 57) && arg[i] != '\0' && arg[i] != 42 && arg[i] != 43 && arg[i] != 45 && arg[i] != 47)
						{
							i++;
						}
						if (arg[i] == '\0')
							return 1;
						else if (arg[i] == 42 || arg[i] != 43 || arg[i] != 45 || arg[i] != 47)
						{
							i++;
							if (arg[i] >= 'a' && arg[i] <= 'j' || arg[i] >= 'A' && arg[i] <= 'J')
							{
								i++;
								if (arg[i] >= 48 && arg[i] <= 57)
								{
									row = arg[i] - 48;
									i++;
									if ((arg[i] >= 48 && arg[i] <= 57) || arg[i] == '\0')
									{
										if (arg[i] >= 48 && arg[i] <= 57)
										{
											row = row * 10 + arg[i] - 48;
											i++;
										}
										if (row <= 10 && arg[i] == '\0')
										{
											return 2;
										}
										return 0;
									}
									return 0;
								}
								return 0;
							}
							return 0;
						}
						return 0;
					}
					else if (arg[i] >= 'a' && arg[i] <= 'j' || arg[i] >= 'A' && arg[i] <= 'J')
					{
						i++;
						if (arg[i] >= 48 && arg[i] <= 57)
						{
							row = arg[i] - 48;
							i++;
							if ((arg[i] >= 48 && arg[i] <= 57) || (arg[i] != 42 || arg[i] != 43 || arg[i] != 45 || arg[i] != 47))
							{
								if (arg[i] >= 48 && arg[i] <= 57)
								{
									row = row * 10 + arg[i] - 48;
									i++;
								}
								if (row <= 10 && (arg[i] != 42 || arg[i] !=43 || arg[i] != 45 || arg[i] != 47 || arg[i] == '\0'))
								{
									if (arg[i] == '\0')
										return 2;
									i++;
									if (arg[i] >= 48 && arg[i] <= 57)
									{
										while ((arg[i] >= 48 && arg[i] <= 57) && arg[i] != '\0')
										{
											i++;
										}
										if (arg[i] == '\0')
											return 2;
										return 0;
									}
									else if (arg[i] >= 'a' && arg[i] <= 'j' || arg[i] >= 'A' && arg[i] <= 'J')
									{
										i++;
										if (arg[i] >= 48 && arg[i] <= 57)
										{
											row = arg[i] - 48;
											i++;
											if ((arg[i] >= 48 && arg[i] <= 57) || arg[i] == '\0')
											{
												if (arg[i] >= 48 && arg[i] <= 57)
												{
													row = row * 10 + arg[i] - 48;
													i++;
												}
												if (row <= 10 && arg[i] == '\0')
												{
													return 2;
												}
												return 0;
											}
											return 0;
										}
										return 0;
									}
								}
								return 0;
							}
							return 0;
						}
						return 0;
					}
					return 0;
				}
			}
			return 0;
		}
		return 0;
	}
	return 0;
}*/

int exportValidation(char *arg)
{
	if (arg[0] != '\0')
	{
		int i = 1;
		while (arg[i] != 32 && arg[i] != '\0')
		{
			i++;
		}
		if (arg[i] == '\0')
			return 1;
		else
		{
			while (arg[i] != 32 && arg[i] != '\0')
			{
				i++;
			}
			if (arg[i] == '\0')
				return 1;
			return 0;
		}
	}
	return 0;
}

int saveValidation(char *arg)
{
	removeSpaces(arg);
	if (arg[0] == '\0')
		return 1;
	return 0;
}

void commands()
{
	printf("COMMAND   SYNTAX\n");
	printf("GET	- GET Cell\n");
	printf("SET	- SET Expression (In terms of Cell(s) and numbers)\n");
	printf("PRINT	- PRINT \n");
	printf("EXPORT	- EXPORT FileName  \n");
	printf("IMPORT	- IMPORT FileName\n");
	printf("SAVE	- Save\n");
	printf("CLEAR	- CLEAR\n");
	printf("EXIT	- EXIT\n");
	printf("   ---***All Commands Are CaSe InSeNsItIvE***---\n");
}

