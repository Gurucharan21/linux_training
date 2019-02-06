#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

#define BOARD_GPIO_IOCTL_MAGIC 0xBF
#define RED_LED_ON _IOWR(BOARD_GPIO_IOCTL_MAGIC, 1, unsigned long*)
#define YELLOW_LED_ON _IOWR(BOARD_GPIO_IOCTL_MAGIC, 2, unsigned long*)
#define BLUE_LED_ON _IOWR(BOARD_GPIO_IOCTL_MAGIC, 3, unsigned long*)
#define READ_VALUE _IOR(BOARD_GPIO_IOCTL_MAGIC, 4, unsigned long*)


#define RED 1
#define YELLOW 2
#define BLUE 3


struct my_gpio{
    int pin;
    int value;
}my_gp;
int i=0;
int j=0;

char ask_again(void);

int main()
{
	int fd;
	int io;
	struct my_gpio my_gp1;
	int level = 0;
	int val = 1000;
	int color_select;
	int count = 0;
	long keystatus;
	int color_value;
	char choice;
    char led = RED;
    int delay  = 100;
	int level_val = 0;

	enum color_choice{Exit=0,RED_1,YELLOW_1,BLUE_1};

	fd = open("/dev/guru", O_RDWR, 0777);
 /*   if(fd == -1)
        printf("Error\n");
    else
 	    printf("file is open\n");
*/	
	printf("\n\n        ******************Ready to Start the GAME**************      \n\n");
LABEL1:	
			printf("                             To select the Color             \n\n\n");
			printf("Enter 1 for Red\n");
			printf("Enter 2 for Yellow\n");
			printf("Enter 3 for Blue\n");
			printf("\nEnter 0 to Exit\n");
			printf("\nEnter your choice here:");
	
			scanf("%d",&color_select);
	
			switch(color_select)
			{
				case 1:
						printf("\n\nYou have chosen Red\n");
						break;
				case 2:
						printf("\n\nYou have chosen Yellow\n");
						break;
				case 3:
						printf("\n\nYou have chosen Blue\n");
						break;
				case 0:
						printf("\n\nExiting...!!!\n");
						return 0;
				default:
						printf("\n\nYou have entered wrong choice\n\n\n\n");
						goto LABEL1;
						return 0;
			}
		printf("             *******Game is started......Start Playing..!!!********         \n");

LABEL2:
	while(level < 1)
	{	
		sleep(1);
		while(1)
		{
		    if (count >= delay) {	
            if (led == RED)
			io = ioctl(fd, RED_LED_ON, (unsigned long *) &my_gp);
            else if (led == YELLOW)
		    io = ioctl(fd, YELLOW_LED_ON, (unsigned long *) &my_gp);
            else 
			io = ioctl(fd, BLUE_LED_ON, (unsigned long *) &my_gp);
			color_value = led;
            led++;
            if (led > BLUE)
            led = RED;
            count = 0;
			}
			
			keystatus = ioctl(fd, READ_VALUE, (unsigned long *) &my_gp);
			if(keystatus == 0)
			{
                    usleep(500000);
					printf("\nYou pressed the Switch\n\n");
                    delay = delay - 30;
                    break;
			}
			usleep(10000);
			count++;
			
        }

		if (color_value == color_select)
		{
			level_val++;
			printf("\n        *********You Got the Right Color**********        \n\n");
			printf("\n          *********You Won Level %d..!!!********        \n",level_val);
			ask_again();
			if (i)
				goto LABEL2;
			if (j)
				break;
			
		}			
		else
		{
			printf("\n             *********Ohhhh....You chose the Wrong Color**********        \n\n");
			printf("\n                  *********You Lost at Level %d...!!!**********        \n",level_val);
			ask_again();
			if (i)
				level_val = 0;
				delay = 100;
				goto LABEL2;
			if (j)
				break;
		}
/*		printf("\nDo you wish to continue\n");
		printf("\nEnter y for yes\nEnter n for No\n");
		scanf("%c",&choice);

*/
//		goto LABEL2;

//		val = val - 50;
	
}
    close(fd);
	printf("File is closed\n");

}

char ask_again(void)

{
    char choice;
    printf("\n\nDo you wish to continue..??\n");
    printf("1. Enter y for Yes\n2. Enter n for No\n3. Enter e for Exit\n\n");
    printf("\nEnter here:");
    scanf("%s",&choice);
    switch(choice)
    {
        case 'y':
            	printf("\nYou have chosen Yes..!!\n\n       *******Game is started......Continue Playing..!!!********\n");
				i=1;
            	return i;
        case 'n':
            	printf("\nYou have chosen No..!!\n");
				printf("\nExiting.....!!!!\n");
				//j=1;
            	//return j;
				exit(0);
		case 'e':
				printf("\nExiting....!!!\n");
				exit(0);
        default:
            printf("\nYou entered wrong..Choose again\n");
            ask_again();
    }
 
}
    
