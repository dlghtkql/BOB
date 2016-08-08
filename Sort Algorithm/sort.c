/* 
	how to exe : ./YK < num.txt
	date : 2016.8.7
*/
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

//////////bubble sort section///////////////
double bubble_sort(int * arr , int size)
{
	int i , j , temp;
	long dif;
	clock_t start,end;

	start = clock();						// start checking time to end
	for(i = 0 ; i<size ; i++)
	{
		for(j = 0 ; j<size-1 ; j++)
		{
			if(arr[j] > arr[j+1])
			{
				 temp = arr[j];				// sort section
				 arr[j] = arr[j+1];
				 arr[j+1] = temp;
			}
		}
	}
	end = clock();						
	dif = end - start;
	printf("bubble sort:");
	for(i = 0; i< size ; i++) printf("%c",(char)arr[i]);
	
	return dif;
}

//////////insert sort section///////////////
double insert_sort(int * arr , int size)
{
	int i , j , temp;
	double dif;
	clock_t start, end;
	
	start = clock();
	for(i = 1 ; i< size ; i++)
	{
		temp = arr[i];
		for( j = i-1 ; (j>=0) && (temp < arr[j]) ; j--)
		{
			arr[j+1] = arr[j];
		}
		arr[j+1] = temp;
	}
	end = clock();
	dif = end - start;
	printf("\tinsert sort:");
	for(i = 0 ; i < size ; i++) printf("%c",(char)arr[i]);

	return dif;
}

/////////////selection sort section//////////////
double selection_sort(int *arr, int size)
{
	int i, j,value,temp;
	double dif;
	clock_t start, end;
	
	start = clock();
	for(i = 0 ; i< size-1 ; i++)
	{
		value = i;
		for(j=i+1; j< size ; j++)
		{
			if(arr[j] < arr[value])value = j;
		}
		temp = arr[i];
		arr[i] = arr[value];
		arr[value] = temp;
	}
	end = clock();
	dif = end - start;
	printf("\tselection sort:");
	for(i = 0 ; i< size ; i++)printf("%c",(char)arr[i]);
	return dif;
}

int main(int argc , char * argv[])
{
	char c;
	int arr[1000];
	int SizeOfArray = 0;
	long time1,time2,time3;

	printf("before Sort:");
	while( (c=getchar()) != EOF )       //    !!! redirection from file !!!
	{
		if( c == '\n')continue;			//because of counting newline
		arr[SizeOfArray] = (int)c ;
		printf("%c",arr[SizeOfArray]);
		SizeOfArray++;					//size of file character
	}
	printf("\n");
	
	//return time value
	time1 = bubble_sort(arr , SizeOfArray);			
	time2 = insert_sort(arr , SizeOfArray);
	time3 = selection_sort(arr , SizeOfArray);
	
	printf("\n");
	printf("bubble_sort time : %f sec\n",(time1/1000.0));
	printf("insert_sort time : %f sec\n",(time2/1000.0));
	printf("selection_sort time : %f sec\n",(time3/1000.0));
	return 0;
}
