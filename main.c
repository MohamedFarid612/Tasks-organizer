#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
\
 \
 |||                     *---__---*****MOHAMED FARID ABDELAZIZ SALAMA ID:6905*****---__---*
 |||                   **********************************************************************
 |||                     *---__---*****ADHAM MOHAMED ALY ABDELHAMID   ID:6744*****---__---*
 /
/
**/


typedef struct
{
    char * name ;
    int starting_time;
    int remaining_time;
} Process;



typedef struct
{
    Process * data;
    struct Node *next;
} Node;



typedef struct
{
    Node * head;
    Node * tail;
} Queue;



// To help in allocating the name for each process in the declared array of processes in function RoundRobin
void initProcess(Process *p)
{
    p->name = malloc(256);
}


// Constructor for queue
Queue* init()
{
    Queue* q = malloc( sizeof(Queue) );
    q->head  = NULL;
    q->tail  = NULL;
    return q;
}


// Constructor for node
Node* newNode(Process *p)
{
    Node *n = malloc( sizeof(Node) );
    n->data = malloc(sizeof(Process) );
    n->data->name=malloc(265*sizeof(char));
    n->data->starting_time = p->starting_time;
    n->data->remaining_time = p->remaining_time;
    strcpy(n->data->name, p->name );
    n->next = NULL;
    return n;
}


// This function adds a process to the queue
void enqueue(Queue * q, Process *p)
{
    Node* n = newNode(p);
    if(q->head==NULL)
        q->head = q->tail = n;
    else
    {
        q->tail->next = (struct Node *)n;
        q->tail = n;
        q->tail->next = NULL;
    }
}


// This boolean function determines whether a queue is empty or not
int isEmpty(Queue *q)
{
    if(q->head==NULL)
        return 1;
    else
        return 0;

}


// This function pops the head of a queue and returns it
Process* dequeue(Queue*q)
{
    if(q->head != NULL)
    {
        Process *result;
        Node* temp = q->head;
        result = temp->data;
        q->head = (Node *)temp->next;
        free(temp);
        if(q->head==NULL)
            q->tail=NULL;

        return result;
    }
    return NULL;
}


// This function returns the number of processes in a file ( Number of lines - 1 )
int getNumberOfProcesses(char *filename)
{
    FILE *fp = fopen(filename, "r");
    int processCount = 0;

    for (char c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') processCount ++;

    fclose(fp);
    return processCount;
}


// This function frees the queue and all its members from the memory
void destroy(Queue *q)
{
    while(!isEmpty(q)) dequeue(q);
    free(q);
}


// This function reads data from a file and incorporates RoundRobin algorithm with the given data
void RoundRobin(char* filename)
{
    Queue *q = init();
    int time, processCount;
    FILE *fp;

    if(!(fp = fopen(filename, "r"))) // Equivalent to catching FileNotFoundException
    {
        printf("\x1b[31m<!> RoundRobin(char*): Error loading file.\x1b[0m\nAborting RoundRobin...");
        return;
    }

/// Start reading from file
    processCount = getNumberOfProcesses(filename);
    Process x[processCount];

    for(int f = 0; f < processCount; f++)
    {
        initProcess(&x[f]);
    }

    printf("\x1b[32mFile opened successfully.\x1b[0m\n");
    fscanf(fp, "Watching time slots = %d\n", &time);
    printf("TIME : %d\n\n", time);

    for(int n = 0; !feof(fp); n++)
        fscanf(fp, "%s %d %d\n", x[n].name, &x[n].starting_time, &x[n].remaining_time);

    fclose(fp);
/// End reading from file

/// Start RoundRobin algorithm
    for(int j = 0; j < time; j++)
    {
        for(int i = 0; i < processCount; i++)
        {
            if(x[i].starting_time == j)
            {
                enqueue(q, &x[i]);
            }
        }
        if(isEmpty(q))
        {
            printf( "idle  (%d-->%d)\n", j, j+1 );
        }
        else if(q->head->data->remaining_time == 1 )
        {
            printf("%s    (%d-->%d) %s aborts\n", q->head->data->name, j, j + 1, q->head->data->name );
            dequeue(q);
        }
        else if(q->head->data->remaining_time > 1 )
        {
            printf("%s    (%d-->%d)\n", q->head->data->name, j, j + 1 );
            q->head->data->remaining_time = q->head->data->remaining_time - 1 ;
            enqueue(q, q->head->data);
            dequeue(q);
        }
    }
    printf( "stop\n\n" );
/// End RoundRobin algorithm

    destroy(q); // Destroy the queue
}







int main()
{
    system("cls"); // Clear screen to allow usage of ANSI escape codes
    char filename[261];
    puts("Enter file name or Ctrl+Z to exit:");
    puts("----------------------------------");
    while(fgets(filename, 260, stdin) != NULL)
    {
        filename[strlen(filename)-1]='\0';
        if(fopen(filename,"r"))
        {
            puts("Initiating RoundRobin...");
            RoundRobin(filename);
        }
        else
        {
            puts("File Not Found!");
            puts("----------------------------------");
        }
        puts("Enter file name or Ctrl+Z to exit:");
        puts("----------------------------------");
    }

    return 0;
}
