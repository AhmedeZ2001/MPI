#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
int main(int argc , char * argv[])
{
    FILE *fp;
    fp=fopen("/shared/test.txt","r");
    int c,count =1;
	int arr[1000][2],i,j,numPassed=0;
	int a2[1000];
    int my_rank;
    int p;
	int tag = 0;
    int source;
    int dest;
	int elements_per_process,n_elements_recieved;
    char message[100];
    MPI_Status status;
    MPI_Init( &argc , &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);


    if(my_rank == 0)
    {
        int counter=0;
		for (c = getc(fp); c != EOF; c = getc(fp))
			if (c == '\n')
			  count = count + 1;
			  rewind(fp);
		for (i=0;i<count;i++)
			{
				for (j=0;j<2;j++)
				{
					fscanf(fp,"%d",&arr[i][j]);
				}
			}
        int a[1000];
        int size=0;
        for(i=0;i<count;i++)
        {
            a[size]=arr[i][0];
            size++;
            a[size]=arr[i][1];
            size++;
        }
		int index,n;
		elements_per_process=((size/p)%2==0?(size/p):(size/p)+1);
		if(p>1)
		{
			for (n = 1; n < p - 1; n++)
			{
                index = n * elements_per_process;
                MPI_Send(&elements_per_process,1, MPI_INT, n, 0,MPI_COMM_WORLD);
                MPI_Send(&a[index],elements_per_process,MPI_INT, n, 0,MPI_COMM_WORLD);
            }
			index = n * elements_per_process;
            int elements_left = size - index;

            MPI_Send(&elements_left,1, MPI_INT,n, 0,MPI_COMM_WORLD);
            MPI_Send(&a[index],elements_left,MPI_INT, n, 0,MPI_COMM_WORLD);
		}
		for(n=0;n<elements_per_process;n+=2)
			{
				if(a[n+1]>=60)
						{
						    printf("%d Passed The Exam\n",a[n]);
						    counter++;
						}
				else
						printf("%d Please Repeat The Exam\n",a[n]);
			}
			int tmp;
        for(i=1;i<p;i++)
        {
              MPI_Recv(&tmp,1, MPI_INT, i, 0,MPI_COMM_WORLD,&status);
              counter+=tmp;
        }
        printf("Total Number of students passed the exam is %d out of %d\n",counter,count);
	}
    else
	{
	    int counter=0;
		  MPI_Recv(&n_elements_recieved,1, MPI_INT, 0, 0,MPI_COMM_WORLD,&status);
        MPI_Recv(&a2, n_elements_recieved,MPI_INT, 0, 0,MPI_COMM_WORLD,&status);

        for ( i = 0; i < n_elements_recieved; i+=2)
           {
				if(a2[i+1]>=60)
						{
						    printf("%d Passed The Exam\n",a2[i]);
						    counter++;
                        }
				else
						printf("%d Please Repeat The Exam\n",a2[i]);
			}

            MPI_Send(&counter,1, MPI_INT,0, 0,MPI_COMM_WORLD);

	}

MPI_Finalize();
return 0;
}