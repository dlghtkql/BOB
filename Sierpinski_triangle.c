#include <stdio.h>
int main()
{
	int line ,i ,j;
	int k = 0, l = 0;

	scanf("%d",&line);				
	l = 3 * line ;			 
	for(i=0;i<line*3;i++)			
	{
		k = l;		
		while(k>0)
		{
			printf(" ");
			k--;
		}
		for(j=1;j<= i/3+1 ;j++)
		{
			for(k=0;k<2*(i%3)+1;k++)
			{
				printf("*");
			}
			for(k=0;k<5-2*(i%3);k++)
			{
				printf(" ");
			}
		}
		printf("\n");
		l--;
	}
}
// ====================================================
#include <stdio.h>
int main(void)
{
	char * str[3];
	str[0] ="   *  ";
	str[1] ="  *** ";
	str[2] =" *****";
	int line;
	int i,j,k;
	scanf("%d",&line);

	for(i=0;i<line*3;i++)
	{
		for(j=(line-i/3)*3-3 ;j>0;j--)
		{
			printf(" ");
		}
		for(k=0;k<(i/3)+1 ;k++)
		{
			printf("%s",str[i%3]);
		}
		printf("\n");
	}
	return 0;
}

