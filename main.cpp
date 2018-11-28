//
//  main.cpp
//  P3
//
//  Created by shuyu shen on 11/16/18.
//  Copyright © 2018 shuyu shen. All rights reserved.
//

#include <iostream>
#include <stack>
#include <queue>
#include <fstream>
#include <string>



using namespace std;

//  function declerati
auto start = std::chrono::high_resolution_clock::now();
auto finish = std::chrono::high_resolution_clock::now();

/************CELL Class***************/
class Cell
{
private:
    int x, y;
    
public:
    Cell();
//    ~Cell();
    Cell(int, int);
    int getX();
    int getY();
    bool visited;
    bool check_visited();
    void set_visited ();
};
//constructor
Cell::Cell()
{
    x = 0;
    y = 0;
    visited = false;
}
////destructor
//Cell::~Cell()
//{
//    //do nothing
//}
//assign values to x and y
Cell::Cell(int x, int y)
{
    this->x = x;
    this->y = y;
}
//return cell.x
int Cell::getX()
{
    return x;
}

//return cell.y
int Cell::getY()
{
    return y;
}
bool Cell::check_visited()
{
    if(this->visited)
        return true;
    else
        return false;
}
void Cell::set_visited()
{
    this->visited = true;
}

/****************************END of Cell Class******************************/
/***********Graph class************/
class Graph{
public:
    //constructor
    Graph();
    ~Graph();
    void destructor(int x, int y);
    char **maze;
    void create_maze(int row, int col);
    void findStart_inside(int row, int col, Cell &p);
    void displayMaze_inside(int row, int col);
    stack <Cell> path;
    void solveDFS_inside(Cell &currLoc,int row, int col);
    void DFS(Cell &currLoc,int row,int col);
};
Graph::Graph(){
    //do nothing
};
Graph::~Graph(){
//    delete maze;
}
void Graph::destructor(int x,int y){
//    for(int i = 0; i <= y; i++)
//    {
//        delete maze[i];
//    }
    
    delete [] maze;
}
//create maze
void Graph:: create_maze(int row, int col){
    maze = new char*[row];
    for(int i = 0; i < row; i++)
    {
        maze[i] = new char[col];
    }
//    cout << row << col << endl;

}
//find start location function
void Graph::findStart_inside(int row, int col, Cell &p){
    for (int i = 0; i < row; i++)
    {
        for(int j = 0; j <col+1; j++)
        {

            if(this->maze[i][j] == 's')
            {
                p = Cell(i,j);
            }
            
        }
        
    }
}
//**************************************DFS_Search******************************************/
void Graph:: DFS(Cell &currLoc, int row, int col)
{   //get x,y and mark as visited
    int tempRow = currLoc.getX();
    int tempCol= currLoc.getY();
    //base case find target
    if(this->maze[tempRow][tempCol] == 'f')
    {
        int path_row;
        int path_col;
        //print out the path using "p"
        while(!(path.empty()))
        {
            path_row = path.top().getX();
            path_col = path.top().getY();
            this->maze[path_row][path_col] ='P';
            path.pop();
        }
        this->displayMaze_inside(row,col);
        //calculating the time here because we are going to exit recursive function here
        finish =std::chrono::high_resolution_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << "ns\n";
        exit(0);
    }
    else{
        //push this cell to the path stack
        if(this->maze[tempRow][tempCol] == ' '||this->maze[tempRow][tempCol] == 's')
        {
            this->path.push(currLoc);
            //leaves a trail mark as visiting
            this->maze[tempRow][tempCol] ='.';
        }
        //recursivly check four directions if it is not wall, and not visited and not exceed the boundry of the graph
        //check up
        if (tempRow-1 >0 )
        {
            if((this->maze[tempRow - 1][tempCol] != '*')&&(this->maze[tempRow - 1][tempCol]!='.'))
            {
                currLoc = Cell(tempRow - 1,tempCol);
                DFS(currLoc,row,col);
            }
        }
        //check down
        if(tempRow +1 < row){
        if((this->maze[tempRow +1 ][tempCol] != '*')&&(this->maze[tempRow+1][tempCol] != '.'))
        {
 
            currLoc = Cell(tempRow +1,tempCol);
             DFS(currLoc,row,col);
        }//check right
        }
        if(tempCol+1 < col){
            if((this->maze[tempRow][tempCol+1] != '*')&&(this->maze[tempRow][tempCol+1] != '.'))
            {
                currLoc = Cell(tempRow ,tempCol+1);
                DFS(currLoc,row,col);
            }
        }
        //check left
        if (tempCol-1 > 0 )
        {
            if((this->maze[tempRow][tempCol-1] != '*')&&(this->maze[tempRow][tempCol-1] != '.'))
            {
                currLoc = Cell(tempRow ,tempCol-1);
                DFS(currLoc,row,col);
            }
        }
        //if reach dead end and stack is empty
        if (path.empty())
        {
//            cout << "no path" << endl;
            return;
        }
        //dead end backtracking
        path.pop();
        return;

    }
}
//display function
void Graph::displayMaze_inside(int row, int col){
    for (int i = 0; i < row; i++)
    {
        for(int j = 0; j <col+1; j++)
        {
            cout << this->maze[i][j];
        }
    }
    cout << endl;
}
//****************************END of Graph class******************************************
//helper function
//load input function
void loadInput(fstream &inFile, Graph maze, int row,int col);

//*********************************MAIN Function***********************************/
//input file is a maze, first row has row number and colum number, and * repersent wall, and space represent a path. S repersents starting Cell ,f repersents target
// the DFS algortihm is trying to find the path from s to f. The algorithm will leave ". " in the graph to show the cell is visited, and "p" represent the path the algorithm find from s to p
//example input
/*5 5
 *****
 *s* *
 * * *
 *  f*
 *****
 */
int main(int argc, const char * argv[]) {
    int row;
    int col;
    //starting point
    Cell currLoc;
    //load file
       fstream file(argv[1]);
    if(argc <1 )
    {
        cout << " missing input file argument " << endl;
    }
    if(!file)
    {
        cout << "cant open file" << endl;
    }
    
    //load the first line of input which tells you how big is the maze
        file >> row;
        file >> col;
    // create a maze with this number of row and number of colums
    Graph input_maze;
    input_maze.create_maze(row, col);
    //load input file and store in the maze
    loadInput(file,input_maze,row,col);
    //find the "s"  starting point in the maze
    input_maze.findStart_inside(row, col, currLoc);
   start =std::chrono::high_resolution_clock::now();
    //start the DFS search
    input_maze.DFS(currLoc, row, col);

    input_maze.destructor(row,col);
    cout << " no path find"<< endl;
    return 0;
}
    
///load input file function
void loadInput(fstream &inFile, Graph created_matrix, int row,int col){
    char input;
    for (int i = 0; i < row; i++)
    {
        for(int j = 0; j < col+1; j++)
        {
            input = inFile.get();
            created_matrix.maze[i][j] = input;
//            cout << input;
        }
    }
    inFile.close();
}




