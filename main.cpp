#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include <memory.h>

/**
 * 登录界面做好了，其实差的是一个浏览历史记录的函数
 * 还有清楚历史纪录的函数
 */
//user data, where history holds the last 3 history
typedef struct User
{
	char name[50];
	char psw[50];
	int history[3][5];
}User;

User users[50];
int UserSize = 0;
void ReadDate();
User* login();
void registerUser();
void SaveDate();
void History(User* u);
void ClearData(User *u);
void drawImg(int s);
int verifyInput(char* input,int begin,int end);
void startGame(User *u);
void menu2(User* u);
void menu1();

//To read data from a file
void ReadData()
{
	FILE *file = fopen("userdata.txt", "r");
	if (file == NULL)
	{
		printf("Fail to open!");
		return;
	}

	while (1)
	{
		char name[50];
		char psw[50];
		User u;
		if (fscanf(file, "%s%s", u.name, u.psw) != EOF)	//读到文件尾再输出
		{
			for (int i = 0; i < 3; i++)
			{
				fscanf(file, "%d%d%d%d%d", &u.history[i][0], &u.history[i][1], &u.history[i][2], &u.history[i][3], &u.history[i][4]);
			}
			users[UserSize++] = u;
		}
		else
		{
			break;
		}
	}
	fclose(file);
}

//The user login
User* login()
{
    system("cls");

	char name[50];
	char psw[50];
	printf("enter you name:");
	scanf("%s", name);
	printf("enter your password:");
	scanf("%s", psw);
	for (int i = 0; i < UserSize; i++)
	{
		if (strcmp(users[i].name, name) == 0 && strcmp(users[i].psw, psw) == 0)
		{
			return &users[i];
		}
	}
	return NULL;
}

//The user registration
void registerUser()
{
	system("cls");

	char name[50];
	char psw[50];
	printf("enter you name:");
	scanf("%s", name);
	printf("enter your password:");
	scanf("%s", psw);
	for (int i = 0; i < UserSize; i++)
	{
		if (strcmp(users[i].name, name) == 0)
		{
			printf("The name exists!\n");
			return ;
		}
	}
	User newUser;
	memset(newUser.history,-1,sizeof(newUser.history));
	strcpy(newUser.name, name);
	strcpy(newUser.psw, psw);
	users[UserSize++] = newUser;
	SaveDate();
}

//Save the data
void SaveDate()
{
	FILE *file = fopen("userdata.txt", "w");
	for (int i = 0; i < UserSize; i++)
	{
		User u = users[i];
		fprintf(file, "%s %s", u.name, u.psw);
		for (int i = 0; i < 3; i++)
		{
			fprintf(file, " %d %d %d %d %d", u.history[i][0], u.history[i][1], u.history[i][2], u.history[i][3], u.history[i][4]);
		}
		fprintf(file,"\n");
	}
	fclose(file);
}

//View history
void History(User* u)
{
	for (int i = 0; i < 3; i++)
	{
		if (u->history[i][0] == -1)
			break;
		printf("%d. \n", i + 1);
		printf("Rounds:%d\n", u->history[i][0]);
		printf("Player wins:%d\n", u->history[i][1]);
		printf("Computer wins:%d\n", u->history[i][2]);
		printf("Draws:%d\n", u->history[i][3]);
		printf("Result:%d (0:player win,1:draw,2:computer win)\n", u->history[i][4]);
	}
}


//Delete history
void ClearData(User *u)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 5; j++)
			u->history[i][j] = -1;
    printf("Clear successfully!\n");
	SaveDate();
}


//draw a picture
void drawImg(int s)
{
	switch (s)
	{
	case 0:
		printf("\\/\n");
		printf("scissor\n");
		break;
	case 1:
		printf(" -\n");
		printf("| |\n");
		printf(" -\n");
		printf("Rock\n");
		break;
	case 2:
		printf(" ~\n");
		printf("~ ~\n");
		printf(" ~\n");
		printf("Paper\n");
		break;
	default:
		break;
	}

}

//Validate user input
int verifyInput(char* input,int begin,int end)
{
	if (strlen(input) == 1 && input[0] >= '1'&&input[0] <= '9')
		return input[0] - '0';
	else
		return -1;
}

//Begin games
void startGame(User *u)
{
    char gesture[3][10] = { "scissor", "Rock", "paper" };	//options
	int man;												//players to choose
	int computer;											//computer to choose
	int result;												//consequence
	int ret;												//if the input correct
	int Num;												//game number
	int comCounter = 0;										//the number of games which computer win
	int playerCounter = 0;									//the number of games which players win
	int draw = 0;
	/*random number initialization function*/
	srand(time(NULL));//initialize the seed according to the current time
	printf("Welcome to the scissor Rock paper game\n");
	printf("Enter the number of the game.");

	//enter the number of inputs, if something goes wrong , type until it is right
	while (1)
	{
		ret = scanf("%d", &Num);
		if (ret != 1 || Num<0 || Num>50)
		{
			printf("Invalid input!");
			printf("Enter again!\n");
		}
		else
			break;
	}

	//start the loop acording to the number of games
	for (int i = 0; i<Num; i++)
	{
		srand((int)time(0));
		computer = rand() % 3;//generate random numbers for computer input
		printf("\nInput your gesture 0-scissor 1-Rock 2-paper: \n");
		while(1)
        {
            ret = scanf("%d", &man);//get users' input
            if (ret != 1 || man<0 || man>2)
            {
                printf("Invalid input!\n");
            }
            else
                break;
        }

		//print relevant information based on input drawing
		printf("Your gesture:%s\n", gesture[man]);
		drawImg(man);
		printf("\n");
		printf("Computer gesture:%s\n", gesture[computer]);
		drawImg(computer);
		printf("\n");

		//determine the results
		result = (man - computer + 4) % 3 - 1;

		if (result > 0)
		{
			playerCounter++;
			printf("%s,YOU WIN!\n", u->name);
		}
		else if (result == 0)
		{
			draw++;
			printf("Draw!\n");
		}
		else
		{
			comCounter++;
			printf("YOU LOSE!\n");
		}

		printf("restarting...");
		//pause 5s and clear the screen
		Sleep(5000);
		system("cls");
	}
	//depending on how many times you win, decide who wins
	if (comCounter == playerCounter)
	{
		result = 1;
		printf("DRAW!\n");
	}
	else if (comCounter>playerCounter)
	{
		result = 2;
		printf("%s,YOU LOSE THE GAME!\n", u->name);
	}
	else
	{
		result = 0;
		printf("%s,YOU WIN THE GAME!\n", u->name);
	}
	for (int i = 0; i < 3; i++)
	{
		if (u->history[i][0] == -1)
		{
			u->history[i][0] = Num;
			u->history[i][1] = playerCounter;
			u->history[i][2] = comCounter;
			u->history[i][3] = draw;
			u->history[i][4] = result;
			SaveDate();
			return;
		}
	}
	u->history[0][0] = Num;
	u->history[0][1] = playerCounter;
	u->history[0][2] = comCounter;
	u->history[0][3] = draw;
	u->history[0][4] = result;
	SaveDate();
}

//Login to the menu

void menu2(User* u)
{
	char temp[10];
	int num = 0;
	while (1)
	{
		printf("1.Start game\n");
		printf("2.Review history\n");
		printf("3.Clear history\n");
		printf("4.Logout\n");
		scanf("%s", temp);
		num = verifyInput(temp, 1, 4);
		if (num == -1)
		{
			printf("invalid!\n");
			continue;
		}
		else
		{
			switch (num)
			{
			case 1:
				startGame(u);
				break;
			case 2:
				History(u);
				break;
			case 3:
				ClearData(u);
				break;
			case 4:
				return;
			}
		}
	}
}

//main menu
void menu1()
{
	char temp[10];
	int num = 0;
	while (1)
	{
		printf("1.login\n");
		printf("2.register\n");
		printf("3.quit\n");
		scanf("%s", temp);
		num = verifyInput(temp, 1, 3);
		if (num == -1)
		{
			printf("invalid!\n");
			continue;
		}
		else
		{
			switch (num)
			{
			case 1:
			    {
				User* u;
				u = login();
				if(u!=NULL)
					menu2(u);
                else
                    printf("invalid name or password!\n");
				break;
			    }
			case 2:
				registerUser();
				break;
			case 3:
				return;
				break;
			}
		}
	}

}


int main(void){
    ReadData();
	menu1();
	
	return 0;
}

