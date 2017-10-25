//By Bradford Wong, Dennis Li, and Thomas Huynh

#include<iostream>
#include<stdio.h>
#include<fstream>
#include <time.h>


using namespace std;

int find_max(int, int, int, int, int, int);
int find_largest_sum(int, int, int,  int**, int**);
void print_array(int**, int);
int** initialize_arrays(int, bool, fstream&);
void delete_arrays(int, int**, int**);

int max_sum = -9999;

//Create array first
int **initialize_arrays(int dimension, bool is_max_array, fstream& file){
      int **a;
      a = new int*[dimension];
      for(int i = 0; i < dimension; i++){
            a[i] = new int[dimension];
      }
      if(is_max_array){
            for(int i = 0; i < dimension; i ++){
                  for(int j = 0; j < dimension; j++){
                        a[i][j] = -99999;
                  }
            }
      }else{
            for(int i = 0; i < dimension; i ++){
                  for(int j = 0; j < dimension; j++){
                        if (file.peek() == ','){
                           file.ignore();
                        }
                        file >> a[i][j];
                  }
            }
      }
      return a;
}


void print_array(int** array, int** max, int dimension){
      cout << "-----Array-----" << endl;
      for (int i =0; i<dimension; i++){
         for (int j =0; j<dimension; j++){
            cout<<array[i][j]<<"\t";
         }
         cout<<endl;
      }
      cout << "-----Max-----" << endl;
      for (int i =0; i<dimension; i++){
         for (int j =0; j<dimension; j++){
            cout<<max[i][j]<<"\t";
         }
         cout<<endl;
      }
}

void delete_arrays(int dimension, int** max, int **array){
      for(int j = 0; j < dimension; j++){
            delete [] max[j];
            delete [] array[j];
      }
      delete [] max;
      delete [] array;
}

// Get max Function
//Input: Max up, max left, Current Value
//Output: Max value

int find_max(int up, int left, int current, int dimension, int x, int y){
   int temp;
   temp = up;
   if (temp < left){
      temp = left;
   }
   if (temp < current){
      temp = current;
   }
   if(temp > max_sum){
         if(x == dimension-1 || y == dimension-1){ //checks that in last row or column
                  max_sum = temp;
         }

   }


   return temp;
}

// Recursion Function

int find_largest_sum(int dimension, int x, int y,  int** array, int** max){
      //base case

      if(max[x][y] != -99999){
            return max[x][y];
      }else{
            int max_top , max_left, current;
            if(y == 0){ //meaning you can't go up., already on top most row
                  max_top = 0;
            }else{ //if you can go up, then go up
                  max_top = find_largest_sum(dimension, x, y-1, array, max);
            }
            //go left
            if(x == 0){ //meaning you can't go left, already on left most column
                  max_left = 0;
            }else{ //if you can go left, go left
                  max_left= find_largest_sum(dimension, x-1, y, array, max);
            }
            current = array[x][y];
            int biggest = find_max(max_top + current, max_left + current, current, dimension, x, y);
            max[x][y] = biggest;
            return biggest;
      }
}

//Input: Array
//Description: Recursive runs through each index in the array for the max


int main(){
      clock_t tStart = clock();
// File Input
   int dimension;
   fstream file;
   file.open("input.txt");
   file>> dimension;
   int **array;
   int **max;
   array = initialize_arrays(dimension, false, file);
   max = initialize_arrays(dimension, true, file);
   if(dimension >= 1)
      max[0][0] = array[0][0];

   file.close();

   find_largest_sum(dimension, dimension-1, dimension-1, array, max);
 //  print_array(array, max, dimension);
// File Output
   ofstream output_file;
   output_file.open("output.txt");
   output_file << max_sum;



   delete_arrays(dimension, max, array);
   output_file.close();
   //cout << "MAX_SUM IS " << max_sum << endl;

   //printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC); //tbh not sure if this works
}
