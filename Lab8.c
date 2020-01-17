#include <stdio.h>
#include <stdlib.h>

#define SIZE (10)

typedef struct _debugLab
{
    int i;
    char c;
} debugLab;


// Prototypes
void PrintUsage(char *);
void DebugOption1(void);
void DebugOption2(void);
void DebugOption3(void);
int  DebugOption4(int *arr, int n);

int main(int argc, char **argv)
{
    int option = 0;
    int sum = 0;
    int arr[] = { 1, 2, 3, 5, 7, 11, 13, 17, 19 }; // Sum equals 78

    if (argc == 1)
    {
        PrintUsage(argv[0]);
	exit(0);
    }

    option = atoi(argv[1]);

    if (option == 1)
    {
	DebugOption1();
    }
    else if (option == 2)
    {
	DebugOption2();
    }
    else if (option == 3)
    {
	DebugOption3();
    }
    else if (option == 4)
    {
	sum = DebugOption4(arr, 9);
	fprintf(stdout, "The sum of the values in the array \"arr\" is: %d\n", sum);
    }
    else
    {
        PrintUsage(argv[0]);
        exit(0);
    }
}

void PrintUsage(char *command)
{
	fprintf(stderr, "Usage: %s [1 2 3 4]\n", command);
	fprintf(stderr, "           1 = Mode 1\n");
	fprintf(stderr, "           2 = Mode 2\n");
	fprintf(stderr, "           3 = Mode 3\n");
	fprintf(stderr, "           4 = Mode 4\n");
}

void DebugOption1()
{
    int i, j;
    int sum = 0;
    char charArray[SIZE];

    fprintf(stdout, "Here we are in DebugOption1()\n");

    
    for (i = 0; i < SIZE; i++)
    {
	sum += sum + i;
    }

    fprintf(stdout, "The sum of integers from 0 to %d is: %d\n", SIZE, sum);

}

void DebugOption2()
{
    char num;
    int i, j;

    for (i = 0; i < 25; i++)
    {
	num = 0;
	for (j = 0; j <= i; j++)
	{
	    num += j;
	}

	fprintf(stdout, "The sum of the numbers from 0 to %d is: %d\n", i, num);
    }
}

void DebugOption3()
{
    debugLab *dl1 = NULL;
    debugLab *dl2 = malloc(sizeof(debugLab)); // Always check for a NULL Pointer after a call to malloc
    debugLab dl3;

    dl3.i = 37;
    dl3.c = 'A';
    dl2->i = 36;
    dl2->c = 'B';
    dl1->i =35;   // Oops! dl1 is NULL!
    dl1->c = 'C';

}

int DebugOption4(int *arr, int n)
{
  int i;
  int sum=0;

  for (i=0;1<n;i++) 
    sum += arr[i];

  return sum;

}
