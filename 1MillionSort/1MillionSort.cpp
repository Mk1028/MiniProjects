#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h> 
#include<string.h> 
#include<stdlib.h>

int * merge1(int *arr, int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	int *L = (int *)malloc(sizeof(int)*n1);
	int *R = (int *)malloc(sizeof(int)*n1);

	for (i = 0; i < n1; i++)
		L[i] = arr[l + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[m + 1 + j];

	i = 0; 
	j = 0; 
	k = l; 
	while (i < n1 && j < n2)
	{
		if (L[i] <= R[j])
		{
			arr[k] = L[i];
			i++;
		}
		else
		{
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	while (i < n1)
	{
		arr[k] = L[i];
		i++;
		k++;
	}

	while (j < n2)
	{
		arr[k] = R[j];
		j++;
		k++;
	}
	return arr;
}

int *mergeSort1(int *arr, int l, int r)
{
	if (l < r)
	{
		int m = l + (r - l) / 2;

		mergeSort1(arr, l, m);
		mergeSort1(arr, m + 1, r);

		return merge1(arr, l, m, r);
	}
}

void printArray(int A[], int size,FILE *fp)
{
	int i;
	
	
	for (i = 0; i < size; i++)
	{
		//printf("%d\n", A[i]);
		fprintf(fp, "%d\n", A[i]);
	}
}

void Files(int digits,FILE *inputFile, FILE *fp)
{
	int number = 0,temp=-1,noOfDigits;
	rewind(inputFile);
	while(1)
	{
		fscanf_s(inputFile, "%d", &number);
		if (temp == number)
			break;
		temp = number;
		noOfDigits = 0;
		if (number == 0)
			noOfDigits++;
		else
		{
			while (temp > 0)
			{
				temp = temp / 10;
				noOfDigits++;
			}
		}
		if (noOfDigits==digits)
		{
			fprintf(fp, "%d\n", number);
		}
		temp = number;
	}
	fclose(fp);
}

void OneMillionSort(char *filename)
{
	FILE *fp;
	fopen_s(&fp,filename, "r");
	FILE *fp1;
	fopen_s(&fp1, "File1.txt", "w");
	Files(1, fp, fp1);
	FILE *fp2;
	fopen_s(&fp2, "File2.txt", "w");
	Files(2, fp, fp2);
	FILE *fp3;
	fopen_s(&fp3, "File3.txt", "w");
	Files(3, fp, fp3);
	FILE *fp4;
	fopen_s(&fp4, "File4.txt", "w");
	Files(4, fp, fp4);
	FILE *fp5;
	fopen_s(&fp5, "File5.txt", "w");
	Files(5, fp, fp5);
	FILE *fp6;
	fopen_s(&fp6, "File6.txt", "w");
	Files(6, fp, fp6);
	FILE *fp7;
	fopen_s(&fp7, "File7.txt", "w");
	Files(7, fp, fp7);
	fclose(fp);
}

void sortFile(int digits, FILE *fp,FILE *fp1)
{
	int number = 0, temp = -1, noOfDigits,ind=0;
	int *count = (int *)malloc(sizeof(int) * 10000000);
	while (1)
	{
		fscanf_s(fp, "%d", &number);
		if (temp == number)
			break;
		temp = number;
		count[ind++] = number;
	}
	count=mergeSort1(count, 0, ind - 1);
	
	printArray(count, ind,fp1);
	/*for (int i = 0;i < 10; i++)
	{
		count[i] = 0;
	}
	while (1)
	{
		fscanf_s(fp, "%d", &number);
		if (temp == number)
			break;
		temp = number;
		count[number] += 1;
	}
	for (int i = 0; i <= 9; i++)
	{
		while (count[i] > 0)
		{
			printf("%d\n", i);
			count[i] = count[i]-1;
		}
	}*/
	fclose(fp);
}

/*int main()
{
	OneMillionSort((char *)"input.txt");
	FILE *fp9;
	fopen_s(&fp9, "output.txt", "w");
	FILE *fp1;
	fopen_s(&fp1, "File1.txt", "r");
	sortFile(1,fp1,fp9);
	FILE *fp2;
	fopen_s(&fp2, "File2.txt", "r");
	sortFile(2,fp2,fp9);
	FILE *fp3;
	fopen_s(&fp3, "File3.txt", "r");
	sortFile(3,fp3,fp9);
	FILE *fp4;
	fopen_s(&fp4, "File4.txt", "r");
	sortFile(4,fp4,fp9);
	FILE *fp5;
	fopen_s(&fp5, "File5.txt", "r");
	sortFile(5,fp5,fp9);
	FILE *fp6;
	fopen_s(&fp6, "File6.txt", "r");
	sortFile(6,fp6,fp9);
	FILE *fp7;
	fopen_s(&fp7, "File7.txt", "r");
	sortFile(7,fp7,fp9);
	fclose(fp9);
	return 0;
}*/