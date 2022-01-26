#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

const int LENGTH = 15;
const char BORDER = 'X';
const char SNAKE = 'O';
const char FOOD = '%';
const char EMPTY = ' ';
const int MAXSNAKELENGTH = ((LENGTH - 2) * (LENGTH - 2)) - 1;

typedef struct{
	int bodyX;
	int bodyY;
} body;

void resetGrid(char grid[LENGTH][LENGTH]){
    for(int i = 0; i < LENGTH; i++)
    {
        for(int j = 0; j < LENGTH; j++)
        {
            if(i == 0 || i == LENGTH - 1 || j == 0 || j == LENGTH - 1)
                grid[i][j] = BORDER;
            else
                grid[i][j] = EMPTY;
        }
    }
}

void placeDude(char grid[LENGTH][LENGTH], int snakeX, int snakeY, body snakeBody[MAXSNAKELENGTH], int foodCounter){
	int i;
	
    grid[snakeY][snakeX] = SNAKE;
	
	if(foodCounter > 0){
		for(i = 0; i < foodCounter; i++){
			grid[snakeBody[i].bodyY][snakeBody[i].bodyX] = SNAKE;
		}
	}
}

void placeFood(char grid[LENGTH][LENGTH], int foodX, int foodY){
	grid[foodY][foodX] = FOOD;
}

void printGrid(char grid[LENGTH][LENGTH]) {
    for(int i = 0; i < LENGTH; i++)
    {
        for(int j = 0; j < LENGTH; j++)
        {
            printf("%c", grid[i][j]);
            printf("%c", grid[i][j]);
        }
        
        printf("\n");
    }
}

char startGame(char userInput){
	while(userInput == '+'){
		userInput = getch();
	}
	
	return(userInput);
}

void changeSnakeLocation(int *snakeX, int *snakeY, int *placeholderX, int *placeholderY, char *userInput){
	*placeholderX = *snakeX;
	*placeholderY = *snakeY;

    if(kbhit()){
		*userInput = getch();
	}
	
	switch (*userInput){
        case 'w':
            *snakeY -= 1;
            break;
        case 's':
            *snakeY += 1;
            break;
        case 'd':
            *snakeX += 1;
            break;
        case 'a':
            *snakeX -= 1;
            break;
        default:
            printf("Not a control\n");
            break;
    }
}

int changeFoodLocation(int *foodX, int *foodY, int snakeX, int snakeY, char grid[LENGTH][LENGTH], int foodCounter){
	srand(time(0));
	
	if(*foodX == snakeX && *foodY == snakeY){
		grid[snakeY][snakeX] = EMPTY;
		foodCounter++;
		
		if(foodCounter != MAXSNAKELENGTH){
			while((*foodX == snakeX && *foodY == snakeY) || grid[*foodY][*foodX] != EMPTY){
				*foodX = rand() % ((LENGTH-2) + 1 - 1) + 1;
				*foodY = rand() % ((LENGTH-2) + 1 - 1) + 1;
			}
		}
	}
	
	return(foodCounter);
}

bool checkSnakeCollision(char grid[LENGTH][LENGTH], int snakeX, int snakeY, bool isSnakeAlive, body snakeBody[MAXSNAKELENGTH], int foodCounter){
	if(foodCounter > 2 && grid[snakeY][snakeX] == grid[snakeBody[foodCounter-1].bodyY][snakeBody[foodCounter-1].bodyX]){
		grid[snakeBody[foodCounter-1].bodyY][snakeBody[foodCounter-1].bodyX] = EMPTY;
	}
	
	if(grid[snakeY][snakeX] != EMPTY){
		isSnakeAlive = false;
    }
    
    return(isSnakeAlive);
}

void addBody(int foodCounter, body snakeBody[MAXSNAKELENGTH], int placeholderX, int placeholderY){
	int i;
	
	if(foodCounter > 1){
		for(i = foodCounter; i > 0; i--){
			snakeBody[i].bodyX = snakeBody[i-1].bodyX;
			snakeBody[i].bodyY = snakeBody[i-1].bodyY;
		}
	}
	
	snakeBody[0].bodyX = placeholderX;
	snakeBody[0].bodyY = placeholderY;
}

void resetScreenPosition(){
	HANDLE hOut;
	COORD Position;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}

int main()
{
	body snakeBody[MAXSNAKELENGTH];
    char grid[LENGTH][LENGTH];
	char userInput = '+';
	int foodCounter = 0;
    int snakeX = LENGTH/2;
    int snakeY = LENGTH/2;
	int foodX = LENGTH/2 - 1;
	int foodY = LENGTH/2 - 1;
	int placeholderX;
	int placeholderY;
	bool isSnakeAlive = true;
	
	system("cls");

	while(isSnakeAlive){
		resetGrid(grid);
		placeDude(grid, snakeX, snakeY, snakeBody, foodCounter);
		placeFood(grid, foodX, foodY);
		printGrid(grid);
		userInput = startGame(userInput);
		changeSnakeLocation(&snakeX, &snakeY, &placeholderX, &placeholderY, &userInput);
		foodCounter = changeFoodLocation(&foodX, &foodY, snakeX, snakeY, grid, foodCounter);
		
		if(foodCounter != MAXSNAKELENGTH){
			isSnakeAlive = checkSnakeCollision(grid, snakeX, snakeY, isSnakeAlive, snakeBody, foodCounter);
			
			if(foodCounter > 0){
				addBody(foodCounter, snakeBody, placeholderX, placeholderY);
			}
			
			if(isSnakeAlive == false){
				printf("Game Over.\n");
			}
		}
		else {
			printf("You Won!\n");
			isSnakeAlive = false;
		}
		
		resetScreenPosition();
		Sleep(75);
	}
	
	Sleep(2000);
	
	system("cls");
    
    return(0);
}
