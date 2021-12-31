//Name : Feras Hamam   ID : 134806  
#include<iostream>
#include<string>
#include<cctype>
#include<fstream>
#include<cstdlib>
#include<pthread.h>
#include <functional>
using namespace std;
//
//classes
class Matrix{
  private:
  int columns;
  int rows;
  int** m;//nd array => matrix
  int length = 0;
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
    length++;
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
int firstMRow = -1;
bool finished = false;
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
void *crossProduct(void *id){
  int threadID = (intptr_t)id;
  int startLoop = -1;
  int endLoop = -1;
  int sum;
  while(true){//# of rows for the first matrix
    if(firstMRow >= x1.getRowsLength() -1){
      break;
    }
    firstMRow++;
    int i = firstMRow;
    if(startLoop ==-1)//if it is not running yet!
    {
      startLoop = i;
    }
    endLoop = i;//always update
    for(int j = 0;j<x2.getColumnsLength();j++){//# of cols for the second matrix
      sum=0;
      for(int k = 0; k<x1.getColumnsLength();k++){//# of cols for the first matrix 
      sum=sum+((x1.getMatrix())[i][k])*((x2.getMatrix())[k][j]);
      }
      if(sum%2 == 0)//is sum Even??
        numOfEven++;
      else
        numOfOdd++;
      result.setValue(i,j,sum);
      totalCells++;
    }
  }
  if(startLoop == -1 && endLoop ==-1){
    printf("ThreadID=%d\n", threadID+1);
  }
  else{
    printf("ThreadID=%d, startLoop=%d, endLoop=%d\n",threadID+1, startLoop,endLoop);
  }
  
  return NULL;
}
//end of crossproduct

int main(int argc , char* argv[]){
  readFile();//do not change order of readfile => always the first line of main
  //thread Number
  int NumOfThreads;
  //printf("Enter number of threads : ");
  //scanf("%d" , &NumOfThreads);
  NumOfThreads = atoi(argv[1]);
  printf("\n");
  //end of thread Number
  //declaring threads
  pthread_t threads[NumOfThreads];
  //end of declaring threads
  //starting threads
  result.setMatrix(x1.getRowsLength(),x1.getColumnsLength());
  for(int i = 0 ; i < NumOfThreads; i++){
      int rc = pthread_create(threads+i,NULL,*crossProduct,(void *) (intptr_t) i);
      if (rc) {
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
  }
  for(int i = 0 ; i < NumOfThreads; i++){
      int rc = pthread_join(threads[i], NULL);
      if (rc) {
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
  }
  printf("numOfEven=%d numOfOdd=%d totalCells=%d\n",numOfEven,numOfOdd,totalCells);
  for(int i =0 ; i < result.getRowsLength() ; i++){
    for(int j = 0 ; j < result.getColumnsLength();j++){
      printf("%d",result.getMatrix()[i][j]);
    }
    printf("\n");
  }
  //end of starting threads
}
