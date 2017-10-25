#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <windows.h>
#include <dirent.h>
#include <regex>
#include "ConsoleColor.h"

using namespace std;

class Point {
	public:
	int x, y;
	int xParent, yParent;
	double totalCost;
	Point ();
	Point (int,int);
	Point(Point,Point,double);
};

Point::Point(){
	x = -1;
	y = -1;
}

Point::Point (int a, int b) {
	xParent = -1;
	yParent = -1;
	x = a;
	y = b;
}

Point::Point(Point parent, Point current, double tCost){
	xParent = parent.x;
	yParent = parent.y;
	totalCost = tCost;
	x = current.x;
	y = current.y;
}

	int wymY;
	int wymX;
	Point start;
	Point target;
	char** Grid;
	vector<Point> closedList;
	vector<Point> openList;

bool checkIfPointInList(Point point, vector<Point> list){
	for(int i = 0; i<list.size();i++){
		if (list[i].x == point.x && list[i].y == point.y){
			return true;
		}
	}
	return false;
}

vector<Point> removeFromOpenList(Point point){
	for(int i = 0; i<openList.size();i++){
		if (openList[i].x == point.x && openList[i].y == point.y){
			openList.erase(openList.begin()+i);
		}
	}
	return openList;
}

Point findLowestCostItem(){
	if(openList.size()==0){
		return Point(-1,-1);
	}
	double minCost = openList[0].totalCost;
	int minIter = 0;
	for(int i = 0; i<openList.size();i++){
		if (openList[i].totalCost < minCost || openList[i].totalCost == minCost){
			minIter = i;
		}
	}
	return openList[minIter];
}

Point move(Point point, char direction){
	int x = point.x;
	int y = point.y;
	switch (direction){
		case 'd': {
			x++;
			break;
		}
		case 'l': {
			y--;
			break;
		}		
		case 'u': {
			x--;
			break;
		}
		case 'r': {
			y++;
			break;
		}
	}
	return Point(x,y);
}

string whereCanIMove(Point point, vector<Point> closed){
	string possibleMoves = "";
	if(point.x<wymY-1 && Grid[point.x+1][point.y]!=(char)178 && !checkIfPointInList(Point(point.x+1,point.y), closed)) {
		possibleMoves+='d';			
	} 
	if(point.y>0 && Grid[point.x][point.y-1]!=(char)178 && !checkIfPointInList(Point(point.x,point.y-1), closed)){
		possibleMoves+='l';			
	} 
	if(point.x>0 && Grid[point.x-1][point.y]!=(char)178 && !checkIfPointInList(Point(point.x-1,point.y), closed) ){
		possibleMoves+='u';			
	} 
	if(point.y<wymX-1 && Grid[point.x][point.y+1]!=(char)178 && !checkIfPointInList(Point(point.x,point.y+1), closed)){
		possibleMoves+='r';			
	} 
	return possibleMoves;
}

void showGrid(char** grid){
	for(int i=0;i<wymY;i++){
		for(int j=0;j<wymX;j++){
			if(grid[i][j]==(char)178){
				cout<<""<<redBgRed<<grid[i][j];
			} else if(grid[i][j]==(char)205 || grid[i][j]==(char)186 || grid[i][j]==(char)187 || grid[i][j]==(char)188 || grid[i][j]==(char)200 || grid[i][j]==(char)201){
				cout<<""<<greenBgWhite<<grid[i][j];
			} else cout<<""<<whiteBgWhite<<grid[i][j];
		} cout<<"\n";
	} cout<<"\n\n"<<white;	
}

char setCharacter(Point next, Point actual, Point prev){

	char character;
	bool 
		conVer = (prev.y==next.y),
		conHor = (prev.x==next.x),
		conUpPrev = (prev.x<actual.x),
		conUpNext = (next.x<actual.x),
		conDownPrev = (prev.x>actual.x),
		conDownNext = (next.x>actual.x),
		conLeftPrev = (prev.y<actual.y),
		conLeftNext = (next.y<actual.y),
		conRightPrev = (prev.y>actual.y),
		conRightNext = (next.y>actual.y);
		
	//vertical line
	if(conVer){
		return 186;
	}
	//horizontal line
	if(conHor){
		return 205;
	}
	//LU character
	if((conUpPrev && conLeftNext)||(conUpNext && conLeftPrev)){
		return 188;
	}
	//LD character
	if((conDownPrev && conLeftNext)||(conDownNext && conLeftPrev)){
		return 187;
	}
	//RU character
	if((conUpPrev && conRightNext)||(conUpNext && conRightPrev)){
		return 200;
	}
	//RD character
	if((conDownPrev && conRightNext)||(conDownNext && conRightPrev)){
		return 201;
	}
	return 'O';
}

vector<Point> buildShortestWay(char** grid){
	char** tmpGrid = grid;
	vector<Point> path;
	Point prev,next,actual;
	path.push_back(closedList.back());
	
	for(int i=closedList.size()-1; i>0; i--){
		if(path.back().xParent==closedList[i-1].x && path.back().yParent==closedList[i-1].y ){
			path.push_back(closedList[i-1]);
		}
	}
	for(int i=path.size()-1; i>=0; i--){
		prev = path[i+1];
		actual = path[i];
		next = path[i-1];
		if(i==path.size()-1){
			prev=actual;
			grid[path[i].x][path[i].y]=setCharacter(next,actual,prev);
		}
		if(i==0){
			next=actual;
			grid[path[i].x][path[i].y]=setCharacter(next,actual,prev);
		}
			grid[path[i].x][path[i].y]=setCharacter(next,actual,prev);
	}
	showGrid(tmpGrid);
	return path;
}

int cost(Point point){
	int iter;
	int coun;
	for (int i=0;i<closedList.size();i++){
		if(closedList[i].x == point.x && closedList[i].y == point.y){
			iter = i;
		}
	}
	return iter;
}

double heuristic(Point test) {
	return sqrt(pow(test.x-target.x,2)+pow(test.y-target.y,2));
}

double costWithHeuristic(Point test) {
	return cost(test)+heuristic(test);
}

void initializeVariables(string fileName){
	std::ifstream plik(fileName.c_str());
	std::ifstream inFile(fileName); 
    wymY = std::count(std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>(), '\n')+1;
    int count=0;
	string line;
	getline(plik,line);
	for(int i=0;i<line.length();i++){
		if (line[i] == '0' || line[i] == '5'){
			count++;
		}
	}
	wymX = count;
    start = Point(wymY-1,0);
	target = Point(0,wymX-1);
	plik.close();
}

char** initializeMap (string fileName){
	//teraz deklarujemy dynamicznie tablice do, której wczytamyu nasz plik,
	cout<<fileName<<endl;

	cout<<"\n\nNacisnij "<<red<<"ENTER"<<white<<" aby wczytac tablice o nazwie "<<cyan<<fileName<<white<<":\n";
	getchar();
	std::ifstream plik(fileName.c_str());
	std::ifstream inFile(fileName); 
	int rows = wymY;
	char **G;
	G = new char*[rows];
	while(rows--) G[rows] = new char[wymX];
	for ( unsigned int i=0;i<wymY;i++){
		for ( unsigned int j=0;j<wymX;j++){
			plik >> G[i][j];
			if(G[i][j]=='0'){
				G[i][j]=177;
			} else if(G[i][j]=='5'){
				G[i][j]=178;
			}
		}
	}  
	plik.close();
	return G;
}
void setGridFont(){
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 8;                   // Width of each character in the font
	cfi.dwFontSize.Y = 8;                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	std::wcscpy(cfi.FaceName, L"Terminal"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	
}
vector<string> getOptions(vector<string> o){
	std::regex self_regex("(grid).*\\.(txt)", std::regex_constants::ECMAScript | std::regex_constants::icase);
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir ("c:")) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			if (std::regex_search(ent->d_name, self_regex)) {
				o.push_back(ent->d_name);
			}
		}
		closedir (dir);
	} else {
		/* could not open directory */
		perror ("");
	}
return o;
}
void hello(){	
	vector<string> options;
	options = getOptions(options);
	cout<<blackBgWhite<<"\n\n\n                                                \n"<<white;
	cout<<blackBgWhite<<"    Witaj w aplikacji MazeResolver 3000 v1.1    \n"<<white;
	cout<<blackBgWhite<<"                                                \n\n"<<white;
	cout<<"    DOSTEPNE LABIRYNTY:\n";
	cout<<green<<"      ID   NAZWA PLIKU\n"<<cyan;
	for (int i=0;i<options.size();i++){
		if(i<10){
			cout<<"       ";
		} else cout<<"      ";
		cout<<i<<":   "<<options[i]<<"\n";
	}
	cout<<yellow<<"\n    Aby stworzyc losowy labirynt:\n     1: Wyjdz z programu\n     2: Uruchom map_generator.exe\n     3: Uruchom MazeResolver jeszcze raz.\n\n";
	cout<<red<<"    UWAGA!!! "<<yellow<<"Jezeli w katalogu istnieje plik "<<green<<"grid.txt"<<yellow<<" zostanie on nadpisany\n\n"<<white;
	cout<<"\n    Ktory plik wybierasz?"<<red<<" [PODAJ ID]\n"<<white;
	int select;
	cin>>select;
	while(select<0||select>options.size()){
			cout<<red<<"    Niepoprawne ID, podaj jeszcze raz\n"<<white;
			cin>>select;
	}
	initializeVariables(options[select]);
	Grid = initializeMap(options[select]);	
}



int main (void) {
	hello();
	getchar();
	system("cls");
	setGridFont();
	cout<<"\n\n\n\n";
	showGrid(Grid);
	closedList.push_back(start);
	string possibleMoves = "";
	
	while (closedList.back().x!=target.x || closedList.back().y!=target.y){
		possibleMoves = whereCanIMove(closedList.back(),closedList);
		for(int i=0;i<possibleMoves.length();i++){
			Point pointAfterMove = move(closedList.back(),possibleMoves[i]);
			if(!checkIfPointInList(pointAfterMove, openList)){
				openList.push_back(Point(closedList.back(),pointAfterMove,costWithHeuristic(pointAfterMove)));
			}
		}
		Point lowestCostItem = findLowestCostItem();
		if (lowestCostItem.x==-1 && lowestCostItem.y==-1){
			cout<<"Sciezka w tym labiryncie nie istnieje.";
			getchar();
			return 0;
		}
		closedList.push_back(lowestCostItem);
		openList = removeFromOpenList(lowestCostItem);
		possibleMoves = "";
	}
	vector<Point> path = buildShortestWay(Grid);
	getchar();
	return 0;
}

