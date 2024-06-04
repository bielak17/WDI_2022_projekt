#include <iostream>
#include <fstream>
#include <climits>
#include <queue>

using namespace std;

struct Board {					//Board structure with map, row and column
	int row;
	int col;
	char** map;
};
struct cordinates {				//Cordinates structure with x,y and distance
	int x;
	int y;
	int distance;
};

Board load_board(char name[])	//Function to load board from file
{
	fstream file;
	file.open(name, ios::in);
	if (!file.is_open())
	{
		cout << "Error: File not found" << endl;
		exit(0);
	}
	Board B;
	int row, col;
	file >> row >> col;
	row = row + 2;								//adding 2 to row and column to add walls
	col = col + 2;
	char** map = new char* [row+2];				//creating 2D dynamic array for map
	for (int i = 0; i < row; i++)
	{
		map[i] = new char[col];
	}
	for (int i = 0; i < row; i++)
	{
		if (file.eof())
		{
			cout << "Error 1: Table has different number of rows or columns ending the program." << endl;			//checking if the numbers of columns and rows arent too big
			exit(0);
		}
		for (int j = 0; j < col; j++)
		{
			if(i == 0 || i == row-1 || j == 0 || j == col-1)		//adding walls to the map
				map[i][j] = '#';
			else
				file >> map[i][j];
			if (map[i][j] != '#' && map[i][j] != '.' && map[i][j] != 'S' && map[i][j] != 'K')		//checking if the map has any other characters than the ones allowed
			{
				cout << "Error 3: Table has different characters than allowed ending the program." << endl;
				exit(0);
			}
		}
	}
	char temp;
	file.get(temp);
	if (temp == '#' || temp == 'S' || temp == 'K' || temp=='.')		//checking if the numbers of columns and rows arent too small
	{
		cout << "Error 2: Table has different number of rows or columns ending the program." << endl;	
		exit(0);
	}
	file.close();
	B.row = row;
	B.col = col;
	B.map = map;
	return B;
}
void print_board(Board B)					//function to print board in console to test	
{
	for (int i = 0; i < B.row; i++)
	{
		for (int j = 0; j < B.col; j++)
		{
			cout << B.map[i][j];
		}
		cout << endl;
	}
}
void print_table(int** table, int row, int col)			//function to print other tables in console to test
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if(table[i][j] == INT_MAX)
				cout << "X" << " ";
			else
				cout << table[i][j] << " ";
		}
		cout << endl;
	}
}
void print_table_parent(pair<int, int>** table, int row, int col)		//function to print parent table in console to test
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			cout << "(" << table[i][j].first << "," << table[i][j].second << ") ";
		}
		cout << endl;
	}
}
cordinates find_place(Board B,char t)		//function to find starting and finishing point
{
	cordinates start;
	for (int i = 0; i < B.row; i++)
	{
		for (int j = 0; j < B.col; j++)
		{
			if (B.map[i][j] == t)
			{
				start.x = i;
				start.y = j;
				start.distance = 0;
				return start;
			}
		}
	}
}
int BFS(Board B, cordinates start, int** visited, int** distance, pair<int,int>** parent)	//BFS algorithm with distance and parent table
{
	int x = start.x;
	int y = start.y;
	visited[x][y] = 1;					//to check if the cordinate is visited
	distance[x][y] = 0;					//distance from starting point
	parent[x][y] = make_pair(-1,-1);	//pair (-1,-1) is used to indicate that there is no parent it is the starting point
	queue<cordinates> q;				//queue in BFS algorithm
	q.push(start);
	while (!q.empty())
	{
		cordinates current = q.front();			//poping the front element
		q.pop();
		visited[current.x][current.y] = 1;
		if(current.distance < distance[current.x][current.y])		//if the distance is less than the distance in the table then update the distance
			distance[current.x][current.y]= current.distance;
		if (B.map[current.x][current.y] == 'K')						//ending the loop if the finish point is reached
			return current.distance;
		if (B.map[current.x - 1][current.y - 1] != '#' && visited[current.x-1][current.y-1]==0)			//checking all the possible moves and adding them to the queue including the diagonal moves
		{
			q.push({ current.x - 1, current.y - 1, current.distance + 1 });
			visited[current.x - 1][current.y - 1] = 1;
			parent[current.x - 1][current.y - 1]=make_pair(current.x,current.y);
		}
		if (B.map[current.x - 1][current.y] != '#' && visited[current.x - 1][current.y] == 0)
		{
			q.push({ current.x - 1, current.y, current.distance + 1 });
			visited[current.x - 1][current.y] = 1;
			parent[current.x - 1][current.y] = make_pair(current.x, current.y);
		}
		if (B.map[current.x - 1][current.y + 1] != '#' && visited[current.x - 1][current.y + 1] == 0)
		{
			q.push({ current.x - 1, current.y + 1, current.distance + 1 });
			visited[current.x - 1][current.y + 1] = 1;
			parent[current.x - 1][current.y + 1] = make_pair(current.x, current.y);
		}
		if (B.map[current.x][current.y - 1] != '#' && visited[current.x][current.y - 1] == 0)
		{
			q.push({ current.x, current.y - 1, current.distance + 1 });
			visited[current.x][current.y - 1] = 1;
			parent[current.x][current.y - 1] = make_pair(current.x, current.y);
		}
		if (B.map[current.x][current.y + 1] != '#' && visited[current.x][current.y + 1] == 0)
		{
			q.push({ current.x, current.y + 1, current.distance + 1 });
			visited[current.x][current.y + 1] = 1;
			parent[current.x][current.y + 1] = make_pair(current.x, current.y);
		}
		if (B.map[current.x + 1][current.y - 1] != '#' && visited[current.x + 1][current.y - 1] == 0)
		{
			q.push({ current.x + 1, current.y - 1, current.distance + 1 });
			visited[current.x + 1][current.y - 1] = 1;
			parent[current.x + 1][current.y - 1] = make_pair(current.x, current.y);
		}
		if (B.map[current.x + 1][current.y] != '#' && visited[current.x + 1][current.y] == 0)
		{
			q.push({ current.x + 1, current.y, current.distance + 1 });
			visited[current.x + 1][current.y] = 1;
			parent[current.x + 1][current.y] = make_pair(current.x, current.y);
		}
		if (B.map[current.x + 1][current.y + 1] != '#' && visited[current.x + 1][current.y + 1] == 0)
		{
			q.push({ current.x + 1, current.y + 1, current.distance + 1 });
			visited[current.x + 1][current.y + 1] = 1;
			parent[current.x + 1][current.y + 1] = make_pair(current.x, current.y);
		}
	}
	return 0;
}
void make_path(Board B, pair<int, int>** parent, cordinates finish)
{
	int x = finish.x;
	int y = finish.y;
	while (parent[x][y].first != -1 && parent[x][y].second != -1)			//starting from finish to start going back with parent table and changing the path to '*'
	{
		if(B.map[x][y] != 'K' && B.map[x][y] != 'S')
			B.map[x][y] = '*';
		int temp = x;
		x = parent[x][y].first;
		y = parent[temp][y].second;
	}
}
void print_result(Board B, const char name[100],int distance)				//function to print the result to a file
{
	fstream fileout;
	fileout.open(name, ios::out);
	if (!fileout.is_open())
	{
		cout << "Error: File not found" << endl;
		exit(0);
	}
	if (!distance)															//adding distance to the file
		fileout << "No path found" << endl;
	else
		fileout<< "Path found. Moves needed: " << distance << endl;
	for (int i = 1; i < B.row-1; i++)										//printing the map without walls
	{
		for (int j = 1; j < B.col-1; j++)
		{
			fileout << B.map[i][j];											//adding map to the file
		}
		fileout << endl;
	}
	cout<<"Success! Path saved to file: " << name << endl;					//console confirmation message
	return;
}

int main()
{
	cout << "Enter the name of the file: ";
	char name[100];
	cin >> name;
	Board B = load_board(name);
	cordinates start = find_place(B,'S');
	cordinates finish = find_place(B,'K');
	int** visited = new int* [B.row];
	int** distance = new int* [B.row];
	pair<int,int>** parent = new pair<int,int>* [B.row];
	for (int i = 0; i < B.row; i++)
	{
		visited[i] = new int[B.col];
		distance[i] = new int[B.col];
		parent[i] = new pair<int,int> [B.col];
	}
	for (int i = 0; i < B.row; i++)
	{
		for (int j = 0; j < B.col; j++)
		{
			visited[i][j] = 0;
			distance[i][j] = INT_MAX;
			parent[i][j] = make_pair(-2,-2);
		}
	}
	int d = BFS(B, start, visited, distance,parent);
	//print_board(B);
	//cout << "\n Distance:" << endl;					//testing the tables in console
	//print_table(distance, B.row, B.col);
	//cout<< "\n Visited:" << endl;
	//print_table(visited, B.row, B.col);
	//cout << "\n Parent:" << endl;
	//print_table_parent(parent, B.row, B.col);
	//cout << "Distance to the exit: " << d << endl;
	if(d)
		make_path(B, parent, finish);
	print_result(B, "trasa.txt", d);
	for (int i = 0; i < B.row; i++)						//deallocating memory
	{
		delete[] visited[i];
		delete[] distance[i];
		delete[] parent[i];
		delete[] B.map[i];
	}
	delete[] visited;
	delete[] distance;
	delete[] parent;
	delete B.map;
	return 0;
}