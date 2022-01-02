//Authors: Feras Iyad Awad Hamam,134806+TALA HANI AHMAD ABU QIDDEES,132333+AMAL ISHAQ AHMED RASSAS,132835
#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <cstdlib>
#include <pthread.h>
#include <math.h>
#include<semaphore.h>
using namespace std;
//
//classes
class Matrix{
  private:
  int columns;
  int rows;
  int** m;//nd array => matrix
  int numOfOdd=0;//Number of odd cells
  int numOfEven=0;//Number of even cells
  int totalCells=0;//Number of overall cells
  public:
  Matrix(){}
  //setters
  void setMatrix(int rows, int columns){ //creates nd array
    this->rows = rows;
    this->columns = columns;
    m = new int*[rows];
    for(int i = 0 ; i < rows; i++){
      m[i] = new int[columns];
    }
  }
  void setValue(int row, int column,int value){
    m[row][column] = value; // to convert ascii to int use => -48
    totalCells++;
    if(value%2==0)
      numOfEven++;
    else
      numOfOdd++;
  };
  //getters
  int** getMatrix(){
    return m;
  };
  int getRowsLength(){
    return rows;
  }
  int getColumnsLength(){
    return columns;
  }
  int getTotalCells(){
    return totalCells;
  }
  int getNoEven(){
    return numOfEven;
  }
  int getNoOdd(){
    return numOfOdd;
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
int firstMRow = 0;//first Matrix Row Counter to use it in threads
int threadWork  = 0;//how much each thread should work determined in main
sem_t semaphore;
//end of globals

//Read functions
int lineAnalyzing(string line, int lineCounter, int matrixSize){
  int counter=0;
  string value="";
  line+=" ";
  for(int i = 0 ; i < line.length(); i++){//i = characters index of a line
    if(isspace(line[i])){
      value="";
      continue;
    }
    else if(lineCounter < matrixSize){
      value += line[i];
      if((isspace(line[i+1]) && value.length() > 0 ))
      {
        x1.setValue(lineCounter%matrixSize,counter,stoi(value));
        counter++;
        value="";
      }
      else{
        continue;
      }
    }
    else{
      value += line[i];
      if(isspace(line[i+1]) && value.length() > 0)
      {
        x2.setValue(lineCounter%matrixSize,counter,stoi(value));
        counter++;
        value="";
      }
      else{
        continue;
      }
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

//write functions
void writeResult(){
  ofstream file;
  string s = "";
  file.open("out.txt");
    for(int i =0 ; i < result.getRowsLength() ; i++){
      for(int j = 0 ; j < result.getColumnsLength();j++){
        s += " " + to_string(result.getMatrix()[i][j]);
      }
      file<<s<<endl;
      s="";
  }
  file.close();
}
//end of write functions

//cross product 
void *crossProduct(void *id){
  int threadID = (intptr_t)id;
  int startLoop = -1;
  int endLoop = -1;
  int i = 0;//rows of first matrix
  int sum = 0;
  int counter = 0;//counter to determine when a thread should stop
  while(true){//# of rows for the first matrix
    if(threadWork < 1)
    {
      threadWork = 1;
    }
    sem_wait(&semaphore);
    if(firstMRow >= x1.getRowsLength() || counter >= threadWork){
      endLoop = i+1;
      sem_post(&semaphore);
      break;//end thread if all rows have been calculated before or if the thread has finished his work(threadWork)
    }
    i = firstMRow;
    firstMRow++;
    sem_post(&semaphore);
    if(startLoop == -1)//if it is not running yet!
    {
      startLoop = i;
      
    }
    endLoop = i;
    for(int j = 0;j<x2.getColumnsLength();j++){//# of cols for the second matrix
      sum=0;
      for(int k = 0; k<x1.getColumnsLength();k++){//# of cols for the first matrix 
      sum=sum+((x1.getMatrix())[i][k])*((x2.getMatrix())[k][j]);
      }
      sem_wait(&semaphore);
      result.setValue(i,j,sum);
      sem_post(&semaphore);
    }
    counter++;
  }
  if(startLoop == -1 && endLoop ==-1){
    printf("ThreadID=%d\n", threadID);
  }
  else{
    printf("ThreadID=%d, startLoop=%d, endLoop=%d\n",threadID, startLoop,endLoop);
  }  
  return NULL;
}
//end of crossproduct

int main(int argc , char* argv[]){
  readFile();//do not change order of readfile => always the first line of main
  //thread Number
  int NumOfThreads = 0;//number of threads
  NumOfThreads = atoi(argv[1]);
  printf("\n");
  //end of thread Number
  //declaring threads
  pthread_t threads[NumOfThreads];
  
  threadWork = ceil(x1.getRowsLength()/NumOfThreads);//how much each thread should work
  //end of declaring threads
  //starting threads
  result.setMatrix(x1.getRowsLength(),x1.getColumnsLength());
  sem_init(&semaphore,0,1);
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
  sem_destroy(&semaphore);
  printf("numOfEven=%d numOfOdd=%d totalCells=%d\n",result.getNoEven(),result.getNoOdd(),result.getTotalCells());
  writeResult();
  //end of starting threads
}
