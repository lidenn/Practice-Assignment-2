//By Bradford Wong, Dennis Li, and Thomas Huynh
//
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <time.h>

using namespace std;

int** initialize_arrays(int, fstream&);
void print_array(int**, int);

void print_array(int** array, int number_vertices){
	cout << "-------------Array-------------" << endl;
	for(int i = 0; i <  number_vertices; i++){
		for(int j=0; j < number_vertices; j++){
			cout << array[i][j] << "\t";
		}
		cout << endl;
	}

}

int** initialize_arrays(int number_vertices, fstream& file){
	int **a;
	int temp;
	a = new int*[number_vertices];

	for(int i=0; i < number_vertices; i++){
		a[i] = new int[number_vertices];
	}

	for(int i = 0; i < number_vertices; i++){
		for(int j = 0; j < number_vertices; j++){
			if(file.peek() == ','){
				file.ignore();
			}
			file >> temp;
			//cout << "Temp: " << temp << "\t";
			a[i][j] = temp;
		}
	//	cout << endl;
	}


	return a;
}

int main(){
	int number_vertices;
	fstream file;
	file.open("input.txt");
	file >> number_vertices;
	int **array;
	
	cout << "Number of vertices " << number_vertices << endl;
	array = initialize_arrays(number_vertices, file);
	print_array(array, number_vertices);
	return 0;
}
