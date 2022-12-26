#include <iostream>
#include <vector>
#include <Windows.h>
#include <conio.h>

using namespace std;

// Fills the given 2d array with walls (#)
void fillMazeWithWalls(char** maze, int SIZEX, int SIZEY)
{
	for (int i = 0; i < SIZEY; i++)
		for (int j = 0; j < SIZEX; j++)
			maze[i][j] = '#';
}

// Prints out the 2d array (the maze) without the user
void displayMaze(char** maze, int SIZEX, int SIZEY)
{
	for (int i = 0; i < SIZEY; i++)
	{
		for (int j = 0; j < SIZEX; j++)
			cout << maze[i][j];
		cout << endl;
	}
}

// Prints the 2d array with the user/goal and any of the vision debuffs
void displayMaze(char** maze, int SIZEX, int SIZEY, int userX, int userY, int goalX, int goalY, int difficulty)
{
	int radius = 500000;
	switch (difficulty)
	{
	case 1: radius = 1; break;
	case 2: radius = 2; break;
	case 3: radius = 3; break;
	case 4: radius = 5000000; break;
	default:
		break;
	}
	for (int i = 0; i < SIZEY; i++)
	{
		for (int j = 0; j < SIZEX; j++)
		{
			if (abs(i - userY) <= radius && abs(j - userX) <= radius)
			{
				if (i == userY && j == userX)
					cout << "@";
				else if (i == goalY && j == goalX)
					cout << "$";
				else
					cout << maze[i][j];
			}
		}
		if (abs(i - userY) <= radius)
			cout << endl;
	}
}

// Prints the solved maze using colors
void displaySolvedMaze(char** maze, int SIZEX, int SIZEY, int userX, int userY)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	maze[userY][userX] = '@';
	for (int i = 0; i < SIZEY; i++)
	{
		for (int j = 0; j < SIZEX; j++)
		{
			if (maze[i][j] == '.' && (i != userY || j != userX))
			{
				SetConsoleTextAttribute(hConsole, 10);
				cout << ".";
			}
			else
			{
				SetConsoleTextAttribute(hConsole, 15);
				cout << maze[i][j];
			}
		}
		cout << endl;
	}
}

// Checks if the given x/y coordinates are within the bounds of the 2d array
bool isInBounds(int SIZEX, int SIZEY, int x, int y)
{
	if (x > SIZEX-2 || x < 1)
		return false;
	if (y > SIZEY-2 || y < 1)
		return false;
	return true;
}

// Check the x,y position to see if movement is possible
bool isMovementAllowed(char** maze, int x, int y)
{
	if (maze[y][x] == '#')
		return false;
	return true;
}

// Recursive implementation of the 'Randomized Depth-First Seaarch' algorithm
// https://en.wikipedia.org/wiki/Maze_generation_algorithm
void recursiveImplementation(char** maze, int SIZEX, int SIZEY, int x, int y)
{
	maze[y][x] = ' ';
	char dir[5] = { 'N', 'E', 'S', 'W' };

	// Randomize the directions 
	for (int i = 0; i < 4; i++)
	{
		int r = rand() % 3;
		char temp = dir[r];
		dir[r] = dir[i];
		dir[i] = temp;
	}
	
	for (int i = 0; i < 4; i++)
	{
		switch (dir[i])
		{
		case 'N':
			if (isInBounds(SIZEX, SIZEY, x, y - 2))
			{
				if (maze[y - 2][x] == '#')
				{
					maze[y - 1][x] = ' ';
					recursiveImplementation(maze, SIZEX, SIZEY, x, y - 2);
				}
			}
			break;
		case 'E':
			if (isInBounds(SIZEX, SIZEY, x + 2, y))
			{
				if (maze[y][x + 2] == '#')
				{
					maze[y][x + 1] = ' ';
					recursiveImplementation(maze, SIZEX, SIZEY, x + 2, y);
				}
			}
			break;
		case 'S':
			if (isInBounds(SIZEX, SIZEY, x, y + 2))
			{
				if (maze[y + 2][x] == '#')
				{
					maze[y + 1][x] = ' ';
					recursiveImplementation(maze, SIZEX, SIZEY, x, y + 2);
				}
			}
			break;
		case 'W':
			if (isInBounds(SIZEX, SIZEY, x - 2, y))
			{
				if (maze[y][x - 2] == '#')
				{
					maze[y][x - 1] = ' ';
					recursiveImplementation(maze, SIZEX, SIZEY, x - 2, y);
				}
			}
			break;
		default:
			break;
		}
	}
	
}

// find all the valid spaces in the maze (i.e. not a wall)
void getValidSpaces(vector<vector<int>>& validSpaces, char** maze, int SIZEX, int SIZEY)
{
	for (int y = 0; y < SIZEY; y++)
	{
		for (int x = 0; x < SIZEX; x++)
		{
			if (maze[y][x] == ' ')
			{
				vector<int> tempVector;
				tempVector.push_back(x);
				tempVector.push_back(y);
				validSpaces.push_back(tempVector);
			}
		}
	}
}

// Determine if the current user position is in a 'junction'
// A junction is defined as having more than 3 or more ways of moving from the given position
// Dead ends can be thought of as junctions with one entrance
bool isJunction(char** maze, int userX, int userY)
{
	int numberOfExits = 0;
	if (maze[userY + 1][userX] == ' ')
		numberOfExits++;
	if (maze[userY - 1][userX] == ' ')
		numberOfExits++;
	if (maze[userY][userX + 1] == ' ')
		numberOfExits++;
	if (maze[userY][userX - 1] == ' ')
		numberOfExits++;

	if (numberOfExits <= 2)
		return false;
	return true;
}

// Determine if the current user position is at an entrance
// An entrance is defined as a position just AFTER a junction
bool isEntrance(char** maze, int userX, int userY)
{
	return true;
}

// Determine if the current user position is in a 'passage'
// A Passage is defined as having an entrance at both ends.
bool isPassage(char** maze, int userX, int userY)
{
	return true;
}

int getDX(char direction)
{
	switch (direction)
	{
	case 'N':
		return 0;
		break;
	case 'E':
		return 1;
		break;
	case 'S':
		return 0;
		break;
	case 'W':
		return -1;
		break;
	default:
		return -6;
		break;
	}
}

int getDY(char direction)
{
	switch (direction)
	{
	case 'N':
		return -1;
		break;
	case 'E':
		return 0;
		break;
	case 'S':
		return 1;
		break;
	case 'W':
		return 0;
		break;
	default:
		return -6;
		break;
	}
}

bool allPassagesUnmarked(char** maze, int userX, int userY)
{
	char marks[3] = { '1', '2' };
	for (int i = 0; i < 2; i++)
	{
		if (maze[userY - 1][userX] != marks[i])
			if (maze[userY + 1][userX] != marks[i])
				if (maze[userY][userX - 1] != marks[i])
					if (maze[userY][userX + 1] != marks[i])
						return true;
	}
	return false;
}

// TODO: Implement this
void mazeSolverTremaux(char** maze, int SIZEX, int SIZEY, int userX, int userY)
{
	// Univisted is ' '
	// Marked once is '1'
	// Marked twice is '2'
	// Junction is marked '*'


	// Algorithm Rules:
	// 1. Whenever you pass through an entrance of a passage, whether it is to enter or exit a junction, leave a mark at the entrance as you pass
	// 2. When you are at a junction, use the first applicable rule below to pick an entrance to exit through:
	// 2a: If only the entrance you just came from is marked, pick an arbitrary unmarked entrance, if any.
	//     This rule also applied if you're just starting in the middle of the mze and there are no marked entrances at all
	// 2b: Pick the entrance you just came from, unless it is marked twice. This rule will apply whenver you reach a dead end.
	// 2c: Pick any entrance with the fewest marks (zero if possible, else one)

	char dir[5] = { 'N', 'E', 'S', 'W' };
	char currentDir = 'N';
	int dx = getDX(currentDir), dy = getDY(currentDir);

	maze[userY][userX] = '1';

	while (maze[userY][userX] != '$')
	{
		if (isJunction(maze, userX, userY))
		{
			// Place marker behind me
			if (maze[userY - dy][userX - dx] == '1')
				maze[userY - dy][userX - dx] = '2';
			else if (maze[userY - dy][userX - dx] == ' ')
				maze[userY - dy][userX - dx] = '1';

			if (allPassagesUnmarked(maze, userX, userY))
			{
				do
				{
					currentDir = dir[rand() % 4];
					dx = getDX(currentDir);
					dy = getDY(currentDir);
				} while (!isMovementAllowed(maze, userX + dx, userY + dy));
			}
			else if (maze[userY - dy][userX - dx] == '1') // need to see if behind me only has 1 mark
			{
				if (currentDir == 'N') { currentDir = 'S'; }
				else if (currentDir == 'E') { currentDir = 'W'; }
				else if (currentDir == 'S') { currentDir = 'N'; }
				else { currentDir = 'E'; }
				maze[userY + dy][userX + dx] = '2';
			}
			else if (maze[userY - dy][userX - dx] == '2') // If the path behind me has 2 marks, go along any other path with fewer marks (aka, any path)
			{
				do
				{

				} while (maze[userY][userX] != '2' && maze[userY][userX] != '#');
			}
			else
			{
				cout << "Maze solver broke." << endl;
				return;
			}
		}

		dx = getDX(currentDir);
		dy = getDY(currentDir);

		// move forward

		userX += dx;
		userY += dy;
	}
}

bool isDeadEnd(char** maze, int x, int y)
{
	int numberOfExits = 0;
	if (maze[y + 1][x] == ' ' || maze[y + 1][x] == '.' || maze[y + 1][x] == '$')
		numberOfExits++;
	if (maze[y - 1][x] == ' ' || maze[y - 1][x] == '.' || maze[y - 1][x] == '$')
		numberOfExits++;
	if (maze[y][x + 1] == ' ' || maze[y][x + 1] == '.' || maze[y][x + 1] == '$')
		numberOfExits++;
	if (maze[y][x - 1] == ' ' || maze[y][x - 1] == '.' || maze[y][x - 1] == '$')
		numberOfExits++;

	if (numberOfExits == 1)
		return true;
	return false;
}

int numberOfExits(char** maze, int x, int y)
{
	int numberOfExits = 0;
	if (maze[y + 1][x] == ' ' || maze[y + 1][x] == '.')
		numberOfExits++;
	if (maze[y - 1][x] == ' ' || maze[y - 1][x] == '.')
		numberOfExits++;
	if (maze[y][x + 1] == ' ' || maze[y][x + 1] == '.')
		numberOfExits++;
	if (maze[y][x - 1] == ' ' || maze[y][x - 1] == '.')
		numberOfExits++;
	return numberOfExits;
}

bool isBend(char** maze, int x, int y, char currentDir)
{
	int dx = 0; int dy = 0;
	if (numberOfExits(maze, x, y) == 2)
	{
		if (currentDir == 'N') { dx = 0; dy = -1; }
		else if (currentDir == 'E') { dx = 1; dy = 0; }
		else if (currentDir == 'S') { dx = 0; dy = 1; }
		else { dx = -1; dy = 0; }

		if (!isMovementAllowed(maze, x + dx, y + dy))
			return true;
		if (!isMovementAllowed(maze, x - dx, y - dy))
			return true;
	}
	return false;
}

void mazeSolverLeftHandRule(char** maze, int SIZEX, int SIZEY, int userX, int userY, int goalX, int goalY)
{
	char currentDir = 'N';
	int dx = 0, dy = 0, wallCheckX = 0, wallCheckY = 0;
	while (maze[userY][userX] != '$')
	{
		if (isDeadEnd(maze, userX, userY)) // solver is at dead end, remove the .
		{
			maze[userY][userX] = ' ';
		}
		else if (isBend(maze, userX, userY, currentDir)) // user is at a bend, 
		{
			maze[userY][userX] = '.';
		}
		else if (maze[userY][userX] == '.')
		{
			maze[userY][userX] = ' ';
			if (maze[userY + 1][userX] == '.')
				maze[userY + 1][userX] = ' ';
			if (maze[userY - 1][userX] == '.')
				maze[userY - 1][userX] = ' ';
			if (maze[userY][userX + 1] == '.')
				maze[userY][userX + 1] = ' ';
			if (maze[userY][userX - 1] == '.')
				maze[userY][userX - 1] = ' ';
		}
		else
		{
			maze[userY][userX] = '.';
		}

		//maze[userY][userX] = '.';

		switch (currentDir)
		{
		case 'N':
			dx = 0; dy = -1;
			wallCheckX = -1; wallCheckY = 0;
			break;
		case 'E':
			dx = 1; dy = 0;
			wallCheckX = 0; wallCheckY = -1;
			break;
		case 'S':
			dx = 0; dy = 1;
			wallCheckX = 1; wallCheckY = 0;
			break;
		case 'W':
			dx = -1; dy = 0;
			wallCheckX = 0; wallCheckY = 1;
			break;
		default:
			break;
		}

		if (!isMovementAllowed(maze, userX + wallCheckX, userY + wallCheckY)) // means that to the left of the way the user is facing, there is a wall
		{
			if (isMovementAllowed(maze, userX + dx, userY + dy)) // if the user can move forward
			{
				userX += dx;
				userY += dy;
			}
			else // The user cannot move forward, turn to the right
			{
				if (currentDir == 'N') { currentDir = 'E'; }
				else if (currentDir == 'E') { currentDir = 'S'; }
				else if (currentDir == 'S') { currentDir = 'W'; }
				else { currentDir = 'N'; }
			}
		}
		else // There is not a wall to the left of the user, thus, there is an opening
		{
			// Turn to face the opening (left)
			if (currentDir == 'N') { currentDir = 'W'; }
			else if (currentDir == 'W') { currentDir = 'S'; }
			else if (currentDir == 'S') { currentDir = 'E'; }
			else { currentDir = 'N'; }
			// Update dx,dy,wallCheckX,wallCheckY to their proper values with the new direction
			switch (currentDir)
			{
			case 'N':
				dx = 0; dy = -1;
				wallCheckX = -1; wallCheckY = 0;
				break;
			case 'E':
				dx = 1; dy = 0;
				wallCheckX = 0; wallCheckY = -1;
				break;
			case 'S':
				dx = 0; dy = 1;
				wallCheckX = 1; wallCheckY = 0;
				break;
			case 'W':
				dx = -1; dy = 0;
				wallCheckX = 0; wallCheckY = 1;
				break;
			default:
				break;
			}
			
			// Finally, move the user forward
			userX += dx;
			userY += dy;
		}

		// Debug information Below:
		/*
		displayMaze(maze, SIZEX, SIZEY, userX, userY, goalX, goalY, 500000);
		cout << "userX: " << userX << ", userY: " << userY << endl;
		cout << "goalX: " << goalX << ", goalY: " << goalY << endl;
		cout << "current user position: " << maze[userY][userX] << endl;
		cout << "--------" << endl;
		Sleep(500);
		*/
	}
}


// DEBUG ONLY FUNCTIONS
void printValidSpaces(vector<vector<int>> &valid)
{
	cout << endl << "The top left point is (0,0), down is +y, right is +x" << endl;
	for (int i = 0; i < valid.size(); i++)
	{
		for (int j = 0; j < valid[i].size(); j++)
		{
			cout << valid[i][j] << " ";
		}
		cout << endl;
	}
}



int main()
{
	srand(time(NULL));

	int SIZEX = 0;
	int SIZEY = 0;
	int difficulty = 0;
	int secretDoors = 0;
	int doors = 0;
	int randomUserSpawn = 0;
	int randomGoalSpawn = 0;

	// Get size of maze the user wants
	cout << "Maze Length? ";
	cin >> SIZEX;
	cout << "Maze height? ";
	cin >> SIZEY;

	// get settings the user wants
	cout << "Random Player Spawn (0 - No, 1 - Yes): ";
	cin >> randomUserSpawn;
	cout << "Random Goal Spawn (0 - No, 1 - Yes): ";
	cin >> randomGoalSpawn;
	cout << "Difficulty (1 - 3x3 vision, 2 - 5x5 vision, 3 - 7x7 vision, 4 - full map): ";
	cin >> difficulty;

	/* Not implemented yet
	cout << "Secret Doors (0 - No, 1 - Yes): ";
	cin >> secretDoors;
	cout << "Include locked/unlocked doors (0 - No, 1 - Yes): ";
	cin >> doors;
	*/

	// dynamically create a 2d array
	char** maze = new char* [SIZEY];
	for (int i = 0; i < SIZEY; i++)
	{
		maze[i] = new char[SIZEX];
	}

	// Main Code
	fillMazeWithWalls(maze, SIZEX, SIZEY);

	int randomX = -1, randomY = -1;
	// Make sure random starting point is within the maze
	do
	{
		randomX = rand() % SIZEX;
		randomY = rand() % SIZEY;
	} while (!isInBounds(SIZEX, SIZEY, randomX, randomY));

	// Generate Maze
	recursiveImplementation(maze, SIZEX, SIZEY, randomX, randomY);
	cout << endl;

	// Grab valid spaces (paths)
	vector<vector<int>> validSpaces;
	getValidSpaces(validSpaces, maze, SIZEX, SIZEY);

	// Determine starting positions for player/goal
	int goalX = -1, goalY = -1, randGoal = -1;
	int startingX = -1, startingY = -1, randStart = -1;

	if (randomGoalSpawn == 0)
	{
		goalX = validSpaces[validSpaces.size() - 1].front();
		goalY = validSpaces[validSpaces.size() - 1].back();
	}
	else
	{
		do
		{
			randGoal = rand() % validSpaces.size();
			goalX = validSpaces[randGoal].front();
			goalY = validSpaces[randGoal].back();
		} while (maze[goalY][goalX] == '#');
	}
	// Add the goal to the maze
	maze[goalY][goalX] = '$';

	if (randomUserSpawn == 0)
	{
		startingX = validSpaces[0].front();
		startingY = validSpaces[0].back();
	}
	else
	{
		do
		{
			randStart = rand() % validSpaces.size();
			startingX = validSpaces[randStart].front();
			startingY = validSpaces[randStart].back();
		} while (maze[startingY][startingX] == '#');
	}

	int userX = startingX;
	int userY = startingY;

	displayMaze(maze, SIZEX, SIZEY, userX, userY, goalX, goalY, difficulty);

	// Main 'game' loop
	while (userX != goalX || userY != goalY)
	{
		char getChar = _getch();
		switch (getChar)
		{
		case 'w':
			if (isMovementAllowed(maze, userX, userY - 1))
			{
				userY--;
				cout << endl << "--------------------------------------------------------------" << endl << endl;
				displayMaze(maze, SIZEX, SIZEY, userX, userY, goalX, goalY, difficulty);
			}
			break;

		case 'a':
			if (isMovementAllowed(maze, userX - 1, userY))
			{
				userX--;
				cout << endl << "--------------------------------------------------------------" << endl << endl;
				displayMaze(maze, SIZEX, SIZEY, userX, userY, goalX, goalY, difficulty);
			}
			break;

		case 's':
			if (isMovementAllowed(maze, userX, userY + 1))
			{
				userY++;
				cout << endl << "--------------------------------------------------------------" << endl << endl;
				displayMaze(maze, SIZEX, SIZEY, userX, userY, goalX, goalY, difficulty);
			}
			break;

		case 'd':
			if (isMovementAllowed(maze, userX + 1, userY))
			{
				userX++;
				cout << endl << "--------------------------------------------------------------" << endl << endl;
				displayMaze(maze, SIZEX, SIZEY, userX, userY, goalX, goalY, difficulty);
			}
			break;

		case 'q':
			return 0;
			break;

		case 'h':
			displayMaze(maze, SIZEX, SIZEY, userX, userY, goalX, goalY, 500000);
			break;

		case 'p':
			mazeSolverLeftHandRule(maze, SIZEX, SIZEY, userX, userY, goalX, goalY);
			cout << "-----------------------------" << endl;
			cout << "The solved maze is below. The green . represents the path the solver took." << endl;
			displaySolvedMaze(maze, SIZEX, SIZEY, userX, userY);
			maze[userY][userX] = ' ';
			for (int i = 0; i < SIZEY; i++)
				for (int j = 0; j < SIZEX; j++)
					if (maze[i][j] == '.')
						maze[i][j] = ' ';
			break;

		default:
			break;
		}
	}
	
	return 0;
}

