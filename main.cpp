//Name : Feras Hamam   ID : 134806  
#include<iostream>
#include<string>
#include <cctype>
#include<fstream>
#include <cstdlib>
#include <pthread.h>
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
  void setValue(int row, int column,int value){
    m[row][column] = value; // to convert ascii to int use => -48
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
    delete []m;
  };
};
//end of classes

//globals
Matrix x1 =  Matrix();
Matrix x2 =  Matrix();
Matrix result = Matrix();
int numOfOdd=0;
int numOfEven=0;
int totalCells=0;
int i = -1;
int j = -1;
int k = -1;
//end of globals

//Read functions
int lineAnalyzing(string line, int lineCounter, int matrixSize){
  int counter=0;
  for(int i = 0 ; i < line.length(); i++){//i = characters index of a line
    if(isspace(line[i])){
      continue;
    }
    else if(lineCounter < matrixSize){
      x1.setValue(lineCounter%matrixSize,counter,line[i] - 48);
      counter++;
    }
    else{
      x2.setValue(lineCounter%matrixSize,counter,line[i] - 48);
      counter++;
    }
  }
  return counter;
}

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

//end of read functions

//cross product 
void* crossProduct(void *id){
  printf("oui");
  int sum;
  char val;
  result.setMatrix(x1.getRowsLength(),x1.getColumnsLength());
  while(i<x1.getRowsLength()-1){//# of rows for the first matrix
    i++;
    while(j<x2.getColumnsLength()){//# of cols for the second matrix
      j++;
      sum=0;
      while(k<x1.getColumnsLength()){//# of cols for the first matrix
      k++;
      sum=sum+((x1.getMatrix())[i][k])*((x2.getMatrix())[k][j]);
      //val=sum;
      }
      //cout<<sum<<endl;
      if(sum%2 == 0)//is sum Even??
        numOfEven++;
      else
        numOfOdd++;
      result.setValue(i,j,sum);
      totalCells++;
    }
  }
}
//end of crossproduct

int main(){
  readFile();//do not change order of readfile => always the first line of main
  //thread Number
  int NumOfThreads;
  printf("Enter number of threads : ");
  scanf("%d" , &NumOfThreads);
  printf("\n");
  //end of thread Number
  //declaring threads
  pthread_t threads[NumOfThreads];
  //end of declaring threads
  for(int i = 0 ; i< NumOfThreads ; i++){
    pthread_create(&threads[i],NULL,crossProduct,&NumOfThreads);
  }
  for(int i = 0 ; i< NumOfThreads ; i++){
    pthread_exit(NULL);
  }
  // crossProduct();
  printf("numOfEven=%d numOfOdd=%d totalCells=%d\n",numOfEven,numOfOdd,totalCells);
}