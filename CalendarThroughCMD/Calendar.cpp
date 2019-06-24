#include "IntermediateMethods.h"

void listOfPeople(ppl *people)
{
	if (people[0].name[0] == '\0')
		printf("No People\n");
	else
	{
		int i = 0;
		for (; i < MaxPeople && people[i].name[0] != '\0'; i++)
		{
			printf("%s\n", people[i].name);
		}
	}
}

ppl *addPeople(ppl *people,char *ListOfNames)
{
	int j = 0, len = strlen(ListOfNames), k = 0;

	char **Name = (char **)malloc(sizeof(char *) * MaxPeople);

	for (int i = 0; i < MaxPeople; i++)
		Name[i] = (char *)malloc(sizeof(char) * 20);

	if ((ListOfNames[j] == '{' && ListOfNames[len - 1] == '}') ||(ListOfNames[j] == '[' && ListOfNames[len - 1] == ']'))
	{
		int numberOfNames=tokenizer(ListOfNames, Name, ',');

		for (int i = 0; i <= numberOfNames; i++)
		{
			trim(Name[i], ' ');
			people=CreatePeopleRecord(people, Name[i]);
		}

	}
	else
		printf("Invalid Format\n");
	return people;
}

ppl *addEvent(ppl *people, char *command)
{
	char **args = (char **)malloc(sizeof(char *) * 10);

	for (int i = 0; i < 10; i++)
		args[i] = (char *)malloc(sizeof(char) * 50);

	getEventName(command,args);

	char eventName[20];
	strcpy(eventName, args[0]);

	trim(args[1], ' ');
	tokenizer(args[1],args,' ');

	int day = str_to_number(args[0]);

	int dInd = isDayInWD(day);
	if (dInd == NumberOfWorkingDays)
	{
		printf("%d is not in WorkingDays\n", day);
	}
	else
	{
		int startSlot = slots(WorkingHours[0], str_to_number(args[1]));
		int numberOfSlots = slots(str_to_number(args[1]), str_to_number(args[2]));

		int noOfNames = tokenizer(args[3], args, ',');

		for (int i = 0; i <= noOfNames; i++)
		{
			people = eventToPeopleCal(people, args[i], dInd, startSlot, numberOfSlots, eventName);
		}
	}
	return people;
}

void listEvents(ppl *people,int numberOfArgs, char **Command)
{
	int i;

	if (numberOfArgs == 1)
	{
		for (i = 0; i < NumberofEvents && EventList[i][0] != '\0'; i++)
		{
			printf("%s\n", EventList[i]);
		}

		if (i == 0)
		{
			printf("No Events Added\n");
		}
	}
	else
	{
		for (i = 0; i < MaxPeople && people[i].name[0] != '\0'; i++)
		{
			if (strcmp(Command[1], people[i].name) == 0)
				break;
		}

		if (people[i].name[0] == '\0')
		{
			printf("%s is not in PEOPLE list\n", Command[1]);
		}
		else
		{
			int k;
			for (k = 0; k < 10 && people[i].events[k] != -1; k++)
			{
				printf("%s\n",EventList[people[i].events[k]]);
			}
		}
	}	
}

void isEventPossible(ppl *people, char *command)
{
	char **args = (char **)malloc(sizeof(char *) * 10);

	for (int i = 0; i < 10; i++)
		args[i] = (char *)malloc(sizeof(char) * 50);

	tokenizer(command, args, ' ');

	int day = str_to_number(args[1]);

	int dInd = isDayInWD(day);
	if (dInd == NumberOfWorkingDays)
	{
		printf("%d is not in WorkingDays\n", day);
	}
	else
	{

		int startSlot = slots(WorkingHours[0], str_to_number(args[2]));
		int numberOfSlots = slots(WorkingHours[0], str_to_number(args[3]));

		findSlots(people, args[0], dInd, startSlot, numberOfSlots, 1);
	}
}

void next3Slots(ppl *people,char *command)
{
	char **args = (char **)malloc(sizeof(char *) * 10);

	for (int i = 0; i < 10; i++)
		args[i] = (char *)malloc(sizeof(char) * 50);

	tokenizer(command, args, ' ');

	int day = str_to_number(args[1]);

	int dInd = isDayInWD(day);
	if (dInd == NumberOfWorkingDays)
	{
		printf("%d is not in WorkingDays\n",day);
	}
	else
	{
		int startSlot = 0;
		int numberOfSlots = (str_to_number(args[2])) / (TimeSlot);

		findSlots(people, args[0], dInd, startSlot, numberOfSlots, 3);
	}
}

/*void saveCal()
{
	FILE *fp = fopen("cal.txt", "w");

	char *buffer = (char *)malloc(sizeof(char)*BlockSize);
	int blockNumber = 0;

	memcpy(buffer, metadata, sizeof(MD));
	writeBlock(buffer, blockNumber);	blockNumber++;
}*/

int main()
{
	char input[100];

	ppl * people = NULL;

	char **Command = (char **)malloc(sizeof(char *) * 3);

	for (int i = 0; i < 3; i++)
	{
		Command[i] = (char *)malloc(sizeof(char) * 20);
	}

	while (1)
	{
		printf(">");
		gets_s(input);
		while (input[0] == '\0')
		{
			printf("\n>");
			gets_s(input);
		}

		int numberOfArgs=parseCommand(input, Command);

		if (compare(Command[0], "workingdays", 11))
			workingDays(Command[1]);
		else if (compare(Command[0], "startingday", 11))
			startingDay(Command[1]);
		else if (compare(Command[0], "workinghours", 12))
		{
			workingHours(Command[1]);
			people = peopleInit();
		}
		else if (compare(Command[0], "addpeople", 9))
			people = addPeople(people, Command[1]);
		else if (compare(Command[0], "list", 4))
			listOfPeople(people);
		else if (compare(Command[0], "init", 4))
		{
			people = NULL;
			INIT();
		}
		else if (compare(Command[0], "listevents", 10))
			listEvents(people, numberOfArgs, Command);
		else if (compare(Command[0], "iseventpossible", 15))
			isEventPossible(people, Command[1]);
		else if (compare(Command[0], "exit", 4))
		{
			saveCal();
			break;
		}
		else if (compare(Command[0], "addevent", 8))
			people = addEvent(people, Command[1]);
		else if (compare(Command[0], "next3possibleslots", 18))
			next3Slots(people, Command[1]);
		else if (compare(Command[0], "displayworkingdays", 18))
			displayWorkingDays();
		else if (compare(Command[0], "clear", 5))
			system("cls");
		else
			printf("Invalid Command\n");

		printf("\n");
	}
	return 0;
}