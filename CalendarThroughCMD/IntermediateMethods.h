#ifndef _IntermediateMethiods_h
#define _IntermediateMethiods_h

#define _CRT_SECURE_NO_WARNINGS

#include "structures.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

#define TotalDays (30)
#define TimeSlot (15)
#define NumberofEvents (50)
#define MaxPeople (20)

static char Days[7][10] = {"sunday","monday","tuesday","wednesday","thursday","friday","saturday" };
static int WorkingDays[7];
static int StartingDay;
static int Working_Days[30];
static int NumberOfWorkingDays;
static int WorkingHours[2];
static int NumberOfSlots;
static char EventList[NumberofEvents][20];

void trim(char *Name,char ch)
{
	int  i = 0, ind = 0;
	char *name = (char *)malloc(sizeof(char) * 50);

	strcpy(name, Name);

	while (name[i] == ch)
	{
		i++;
	}

	for (int j = i; name[j] != '\0'; j++)
	{
		Name[ind++] = name[j];
	}

	Name[ind] = '\0';
}

int compare(char command[20], const char *str, int len)
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

int parseCommand(char *command, char **arg)
{
	int j, ind = 0, k = 0, i;

	for (i = 0; command[i] == ' '; i++);

	for (j = i; command[j] != ' ' && command[j] != '\0'; j++)
	{
		arg[ind][k++] = command[j];
	}
	arg[ind][k] = '\0';

	ind++; k = 0;

	for (i = j + 1; command[i] != '\0'; i++)
	{
		arg[ind][k++] = command[i];
	}
	arg[ind][k] = '\0';

	return ind;
}

int str_to_number(char *left)
{
	unsigned int number = 0; int i = 0;
	while (left[i] != '\0')
	{
		number = number * 10 + left[i] - '0';
		i++;
	}
	return number;
}

ppl * peopleInit()
{
	ppl *people = (ppl *)malloc(sizeof(ppl)*MaxPeople);

	for (int i = 0; i < MaxPeople; i++)
	{
		people[i].name[0] = '\0';

		for (int j = 0; j < 10; j++)
		{
			people[i].events[j] = -1;
		}
	}

	return people;
}

int isDayInWD(int day)
{
	int i = 0;
	while (i < NumberOfWorkingDays && Working_Days[i] != day)
	{
		i++;
	}

	return i;
}

ppl *CreatePeopleRecord(ppl *people, char *name)
{
	int i;

	for (i = 0; i < MaxPeople && people[i].name[0] != '\0'; i++)
	{
		if (strcmp(name, people[i].name)==0)
			break;
	}

	if (people[i].name[0] == '\0')
	{
		strcpy(people[i].name, name);

		people[i].cal = (char **)malloc(sizeof(char *)*TotalDays);

		for (int j = 0; j < TotalDays; j++)
		{
			people[i].cal[j] = (char *)malloc(sizeof(char )*NumberOfSlots);
		}

		for (int k = 0; k < TotalDays; k++)
		{
			for (int j = 0; j < NumberOfSlots; j++)
			{
				people[i].cal[k][j] = '\0';

			}
		}
	}

	return people;
}

int tokenizer(char *command, char **arg, char ch)
{
	int j, ind = 0, k = 0, i = 0,len=strlen(command);

	if ((command[0] == '[' || command[0] == '{') && (command[len-1]==']' || command[len-1]=='}'))
		i = 1;

	for (j = i; command[j] != '\0'; j++)
	{
		if (command[j] == ch || command[j] == '[' || command[j] == '{')
		{
			if (command[j] == '[' || command[j] == '{')
			{ 
				i = 2;
				ch +=1;
			}
			else
			{
				arg[ind][k] = '\0';
				trim(arg[ind], ' ');
				ind++; k = 0;
			}
		}
		else if (command[j] == ']' || command[j] == '}')
		{
			if(i==2)
				ch-=1;	
		}
		else
			arg[ind][k++] = command[j];
	}
	arg[ind][k] = '\0';
	trim(arg[ind], ' ');

	return ind;
}

void getEventName(char *command,char **args)
{
	int i, ind = 0, k = 0;

	for (i = 1; command[i] != '"'; i++)
	{
		args[ind][k++] = command[i];
	}
	args[ind++][k] = '\0';
	k = 0;

	for (int j = i+1; command[j] != '\0'; j++)
	{
		args[ind][k++] = command[j];
	}
	args[ind][k] = '\0';
}

void workingDays(char *WD)
{
	int j = 0, len = strlen(WD), k = 0, ind = 0,i;
	char *day=(char *)malloc(sizeof(char)*10);

	if ((WD[j] == '{' && WD[len - 1] == '}') || (WD[j] == '[' && WD[len - 1] == ']'))
	{
		for (j++; j<(len-1); j++)
		{
			if (WD[j] != ',')
				day[k++] = WD[j];
			else
			{
				day[k] = '\0';
				trim(day,' ');
				
				for (i = 0; i < 7; i++)
				{
					if (compare(day,Days[i], strlen(Days[i])))
					{
						break;
					}
				}

				if (i != 7)
					WorkingDays[ind++] = i;

				k = 0;
			}
		}
		day[k] = '\0';
		trim(day, ' ');

		for (i = 0; i < 7; i++)
		{
			if (compare(day, Days[i], strlen(Days[i])))
			{
				break;
			}
		}

		if (i != 7)
			WorkingDays[ind++] = i;

		for (; ind < 7; ind++)
			WorkingDays[ind] = -1;
	}
	else
		printf("Invalid Format\n");
}

int slots(int start,int end)
{
	int minutes = abs(start-end);
	int arr[2], i = 0;
	while (i < 2)
	{
		arr[i++] = minutes % 10;
		minutes = minutes / 10;
	}
	minutes = minutes * 60 + (arr[1] * 10 + arr[0]);

	return minutes / TimeSlot;
}

void workingHours(char *command)
{
	char **args = (char **)malloc(sizeof(char *) * 10);

	for (int i = 0; i < 10; i++)
	{
		args[i] = (char *)malloc(sizeof(char) * 20);
	}

	tokenizer(command, args, ' ');

	WorkingHours[0] = str_to_number(args[0]);
	WorkingHours[1] = str_to_number(args[1]);
	
	NumberOfSlots = slots(WorkingHours[0],WorkingHours[1]);
}

void startingDay(char *SD)
{
	int i,len=0;

	for(;WorkingDays[len]!=-1;len++)

	for (i = 0; i < 7; i++)
	{
		if (compare(SD, Days[i], strlen(Days[i])))
		{
			break;
		}
	}

	if (i != 7)
		StartingDay = i;

	for (i = 0; i < len; i++)
	{
		if (WorkingDays[i]==StartingDay)
		{
			break;
		}
	}

	if (i == len)
	{
		printf("%s is not in WorkingDays\n",SD);
	}
	else
	{
		int ind = 0,count=1,m=StartingDay,k=0;

		while (count <= 30)
		{

			for (; m < 7 && count <= 30;m++)
			{
				k = 0;
				while(k < len && WorkingDays[k] != m)
				{
					k++;
				}

				if (k < len)
				{
					Working_Days[ind++] = count;
				}
				count++;
			}
			m = 0;
		}
		NumberOfWorkingDays = ind;

	}
}

void displayWorkingDays()
{
	int m = 0,i;
	for (i = 1; i < TotalDays; i++)
	{
		if (m < NumberOfWorkingDays && i == Working_Days[m])
		{
			printf(" %3d ", Working_Days[m]);
			m++;
		}
		else
			printf(" - ");
		if (i % 7 == 0)
			printf("\n");
	}
	printf("\n");
}

void INIT()
{
	NumberOfSlots = 0;

	NumberOfWorkingDays = 0;

	WorkingHours[0] = 0;
	WorkingHours[1] = 0;

	for (int i = 0; i < NumberofEvents; i++)
	{
		EventList[i][0] = '\0';
	}
}

int eventList(char *eventName)
{
	int i;
	for (i = 0; i < NumberofEvents && EventList[i][0] != '\0'; i++);

	strcpy(EventList[i], eventName);

	return i;
}

int nextSlot(int start, int slot)
{
	int arr[2], i = 0;
	while (i < 2)
	{
		arr[i++] = start % 10;
		start = start / 10;
	}
	
	slot+= (arr[1] * 10 + arr[0]);

	start += slot / (TimeSlot * 4);
	start=start*100+(slot%(TimeSlot * 4));

	return start;
}

ppl *eventToPeopleCal(ppl *people, char *name, int dInd, int startSlot, int numberOfSlots, char eventName[20])
{
	int i, j;

	for (i = 0; i < MaxPeople && people[i].name[0] != '\0'; i++)
	{
		if (strcmp(name, people[i].name) == 0)
			break;
	}

	if (people[i].name[0] == '\0')
	{
		printf("%s is not in PEOPLE list\n", name);
		return people;
	}

	for (j = startSlot - 1; j < numberOfSlots && j < NumberOfSlots && people[i].cal[Working_Days[dInd]-1][j] == '\0'; j++);

	if (j == numberOfSlots)
	{

		int event = eventList(eventName);

		int k;
		for (k = 0; k < 10 && people[i].events[k] != -1; k++);

		people[i].events[k] = event;

		for (j = startSlot - 1; j < numberOfSlots; j++)
		{
			people[i].cal[Working_Days[dInd]-1][j] = event + 48;
		}
		return people;
	}

	printf("SomeOther Event Existing during that slot\n");

	return people;
}

void findNextSlot(ppl *people,int i,int dInd, int startSlot, int numberOfSlots,int count)
{
	int j,temp=0,x,y,z;

	while (dInd<NumberOfWorkingDays)
	{
		j = startSlot;
		while(j < numberOfSlots && people[i].cal[Working_Days[dInd] - 1][j] == '\0')
		{
			j++;
		}
		x = startSlot;
		y = numberOfSlots;
		z = dInd;

		temp = startSlot;
		if ((temp + numberOfSlots) <= NumberOfSlots)
		{
			startSlot = numberOfSlots;
			numberOfSlots = 2 * numberOfSlots - temp;
		}
		else
		{
			z = dInd + 1;
			numberOfSlots = numberOfSlots - startSlot;
			startSlot = 0;

		}

		if (j == y)
		{
			printf("Slot %d : %d %d %d\n",(count),Working_Days[dInd], nextSlot(WorkingHours[0], x*TimeSlot), nextSlot(WorkingHours[0], y*TimeSlot));

			count--;

			if(count>0)
				findNextSlot(people,i,z,startSlot,numberOfSlots,count);
			break;
		}
		dInd = z;
	}
	if (dInd == NumberOfWorkingDays)
	{
		printf("No Free Slots Available for %s\n",people[i].name);
	}
}

void findSlots(ppl *people, char *names, int dInd, int startSlot, int numberOfSlots, int count)
{
	char **args = (char **)malloc(sizeof(char *) * 10);

	for (int i = 0; i < 10; i++)
		args[i] = (char *)malloc(sizeof(char) * 50);

	int noOfNames = tokenizer(names, args, ','), j;

	for (int i = 0; i <= noOfNames; i++)
	{
		for (j = 0; j < MaxPeople && people[j].name[0] != '\0'; j++)
		{
			if (strcmp(args[i], people[j].name) == 0)
				break;
		}

		if (people[j].name[0] == '\0')
			printf("%s is not in PEOPLE list\n", args[i]);
		else
		{
			printf("%s :-\n", args[i]);
			findNextSlot(people, j, dInd, startSlot, numberOfSlots, count);
		}
	}
}

#endif // !_IntermediateMethiods_h
