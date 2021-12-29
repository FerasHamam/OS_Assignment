//Name : Feras Hamam   ID : 134806  
#include<iostream>
#include<string>
#include <cctype>
#include<fstream>
using namespace std;
//
//classes
class Matrix{
  private:
  int columns;
  int rows;
  int** m;//nd array => matrix
  public:
  Matrix(){}
  //creates nd array
  void setMatrix(int rows, int columns){
    this->rows = rows;
    this->columns = columns;
    m = new int*[rows];
    for(int i = 0 ; i < rows; i++){
      m[i] = new int[columns];
    }
  }
  void setValue(int row, int column,char value){
    m[row][column] = value - 48; // to convert ascii to int use => -48
    // cout<<a[row][column];
  };
  int** getMatrix(){
    return m;
  };
  int getRowsLength(){
    return rows;
  }
  int getColumnsLength(){
    return columns;
  }
  ~Matrix(){
    delete m;
  };
};
//

//globals
Matrix x1 =  Matrix();
Matrix x2 =  Matrix();
//

//declare functions

//read
int lineAnalyzing(string, int ,int );
void readFile();

  //write
void writeFile();
//

//Read functions

void readFile(){
    int matrixSize = 0;//columns and rows of the matrix
    bool isFirst = true;
    ifstream file("in.txt");//input file name
    string fileLine;//holds the each line in a loop
    int lineCounter = 0;
    do
    { 
      if(isFirst){
        getline(file,fileLine);//gets first line which holds matrixSize
        matrixSize = stoi(fileLine);//converts first line to int
        x1.setMatrix(matrixSize,matrixSize);
        x2.setMatrix(matrixSize,matrixSize);
        isFirst = false;
        continue;
      }
      int isInfo = lineAnalyzing(fileLine,lineCounter, matrixSize);
      if(isInfo > 0)
        lineCounter++;
    } while (getline(file,fileLine));
  file.close();
}

int lineAnalyzing(string line, int lineCounter, int matrixSize){
  int counter=0;
  for(int i = 0 ; i < line.length(); i++){//i = characters index of a line
    if(isspace(line[i])){
      continue;
    }
    else if(lineCounter < matrixSize){
      x1.setValue(lineCounter%matrixSize,counter,line[i]);
      counter++;
    }
    else{
      x2.setValue(lineCounter%matrixSize,counter,line[i]);
      counter++;
    }
  }
  return counter;
}
int main(){
readFile();//do not change order of readfile => always the first line of main
  for (int i = 0; i < x1.getRowsLength(); i++){
    for(int j = 0 ; j < x1.getColumnsLength(); j++)
    {
      cout<<x1.getMatrix()[i][j];
    }
    cout<<endl;
  }
  for (int i = 0; i < x1.getRowsLength(); i++){
    for(int j = 0 ; j < x1.getColumnsLength(); j++)
    {
      cout<<x2.getMatrix()[i][j];
    }
    cout<<endl;
  }
}