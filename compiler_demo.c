#include <stdio.h>
#include <string.h>

void parse_arg1(char *str, int atable[3][255])
{
	int i=0,j=0,k=0;
	while(*str != '\0')
	{
		if(*str == '%')
		{
			str++;
			atable[1][i] = *str;
			i++;
		}
		else if(*str == 92)
		{
			str++;
			atable[2][j] = *str;
			j++;
		}
		else
		{
			atable[0][k] = *str;
			k++;
		}
		str++;
	}
}

int main()
{
	enum state {SS=0,FS=1,AS=2,AE=3,FN=4,SE=5};
	enum state cs;
	
	char * cp;
    char * buf = "printf(\"hello world\\n\");";
    char * buf1 = "int a,b;";
    char * buf2 = "a=10;";
    char * buf3 = "b=20;";
    char * buf4 = "printf(“a is %d\n”,a);";
    char * buf5 = "printf(“b is %d\n”,b);";

	cp = buf;

	char str_table[256]; int i = 0;
	char func_name[20]; int j = 0;
	char vari_table[256];
	int len = 0,k=13;
	int atable[3][255];

	printf("%s\n",buf);
	while(*cp != ';'){cp++;}				// cp의 포인터를 맨 끝으로 옮긴다.

	while(cp != buf-1)
	{	
		switch( *cp )
		{
			case 59: cs=SS;cp--;break;			// ; = 59			
			case 41: cs=FS;cp--;break;			// ) = 41
			case 40: cs=FN;cp--;break;			// ( = 40
			case 34: 							// \" = 34
			  switch(cs)
			  	case FS : cs=AS;cp--;break;
			  	case AS : cs=FN;cp--;break;
			default : 														
				switch(cs)
					case AS : str_table[i]= *cp;cs=AS;i++;cp--;break;
					case FN : func_name[i]= *cp;j++;cp--;break;
		}		
	}

	printf("====sring table======\n");
	printf("%s\n",str_table);
  
 /* 	i=0;							//문자열 반대로 설정 부분
	for(k=12;k>=0;k--)
	{
		printf("%c",str_table[k]);
		str_table[i] = str_table[k];
		i++;
	}*/

	printf("\n");
	i=0;
	printf("=====printf table=====\n");
	parse_arg1(str_table,atable);
	while(atable[0][i] != '\0')
	{
		putchar((char)atable[0][i]);					
		printf(",");
		i++;
	}
	i=0;
	printf("\n");
	while(atable[2][i] != '\0')
	{
		putchar((char)atable[2][i]);					//거꾸로 넣어서 n 대신에 d가 들어감, bug!!   
		printf(" ");
		i++;
	}
	return 0;
}




/*
int a, b; 
a = 10; 
b = 20;

printf(“Hello, world!\n”);
printf(“a is %d\n”,a);
printf(“b is %d\n”,b);
*/