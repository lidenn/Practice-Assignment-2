//By Bradford Wong, Dennis Li, and Thomas Huynh
//
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <time.h>

using namespace std;


struct vertices{
	int* mst; //an array that is composed of all the vertices added to MST. Ex: if '2' is this array, then the second vertex is already in the mst
	int size; //how many vertices are in the mst array
};

int** initialize_arrays(int, fstream&);
void print_array(int**, int);
int find_safe_edge(int**, int, int, bool*, vertices*);
int prim(int**, int, bool*, vertices*);
bool* initializeMST(int);
void add_to_MST(bool*, int);
vertices* initialize_vertices(vertices*, int);
void push(vertices*, int);
int find_smallest_edge(int**, int, bool*, vertices*);

//Function: pushes the vertex that connects the safe edge into the MST
//Parameters: MST-the minimum spanning tree, vertex: the index of the current vertex that is being added to the MST
void push(vertices* MST, int vertex){
	//cout << "MST->Size is " << MST->size <<  endl;
	int temp_size = MST->size;
//	cout << "Temp_size is " << temp_size << endl;
//	cout << "Vertex adding is " << vertex << endl;
	MST->mst[temp_size] = vertex;
	MST->size += 1;
	//cout << "Just added " << MST->mst[MST->size-1] << ", size is now " << MST->size << endl;
//	cout << MST->mst[0] << endl;


}
/*
vertices* initialize_vertices(vertices* MST, int number_vertices){
	cout << "In initialize_vertices " << endl;
	vertices* v;
	v->mst = new int[number_vertices];
	cout << "After assignming mst " << endl;
	v->size = 0;
	cout << "Before return in initialize_vertices " << endl;
	return v;
}*/

//function: initialize MST so that all entries are false, meaning that none of the indices are in the MST
bool* initializeMST(int number_vertices){
	bool *a;
	a = new bool[number_vertices];
	for(int i = 0; i < number_vertices; i++){
		a[i] = false;
	}
	return a;
}

//just prints the array
void print_array(int** array, int number_vertices){
	cout << "-------------Array-------------" << endl;
	for(int i = 0; i <  number_vertices; i++){
		for(int j=0; j < number_vertices; j++){
			cout << array[i][j] << "\t";
		}
		cout << endl;
	}

}

//creates the array from the input file
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
			//a[i][j].in_tree = false;
		}
	//	cout << endl;
	}


	return a;
}
/*
void add_to_MST(bool* in_row_MST, int index){

}*/
/*
int find_safe_edge(int** array, int number_vertices, int row, bool* in_row_MST, vertices* MST){
	int safe_edge = 99999999; //the weight of the safe edge
	int safe_index; //the index where the safe edge is in the array

		for(int i = 0; i < number_vertices; i++){
		//	cout << "In find_safe_edge. i is " << i << endl;
			if (array[row][i] < safe_edge && array[row][i] != 0 && in_row_MST[i] == false){
				safe_index = i;
				safe_edge = array[row][safe_index];
				//cout << "Safe edge is now " << safe_edge << endl;
			}

	}

	in_row_MST[safe_index] = true; //adds the vertex to MST
	//in_row_MST[row] = true;
	//add_to_MST(in_row_MST, safe_index);
	//cout << "Safe_edge: " << safe_edge << ", safe_index " << safe_index << endl;
	return safe_edge;
}*/

//FUNCTION: finds the mst sum using prim's algorithm
//PARAMETERS: array-the adjacency matrix given from the input file, in_row_MST- array where indices with "true" in it mean that vertex is in the MST (vertex # = in_row_MST index)
//MST-the minimum spanning tree, composed of vertices
int prim(int** array, int number_vertices, bool* in_row_MST, vertices* MST){
	int MST_sum = 0;
	//cout << "In prim " << endl;
	for(int i = 0; i < number_vertices -1; i ++){ //iterate number_vertices-1 because total number of edges is # of vertices -1
		MST_sum += find_smallest_edge(array, number_vertices, in_row_MST, MST); //add weight of each safe edge to running total
		//in_row_MST[i] = true;
	}
	cout << "MST SUM IS " << MST_sum << endl;
}

//FUNCTION: returns the weight of the safe edge
//PARAMETERS: array-the adjacency matrix given from the input file, in_row_MST- array where indices with "true" in it mean that vertex is in the MST (vertex # = in_row_MST index)
//MST-the minimum spanning tree, composed of vertices
int find_smallest_edge(int** array, int number_vertices, bool* in_row_MST, vertices* MST){
	int safe_edge = 99999999; //the weight of the safe edge
	int safe_index; //the index where the safe edge is in the array
	for(int j = 0; j < MST->size; j++){ //this steps through each vertex stored in MST array
		int row = MST->mst[j]; //the row is the current vertex in MST
		//cout << "Now checking row: " << row << endl;
		for(int i = 0; i < number_vertices; i++){ //this steps through the actual row in the adjacency matrix array
			//cout << "Looking at [" << row << "][" << i << "]" << endl;
			if (array[row][i] < safe_edge && array[row][i] != 0 && in_row_MST[i] == false){
				safe_index = i;
				safe_edge = array[row][safe_index];
				//cout << "safe_edge is now " << safe_edge << endl;
				//cout << "Safe edge is now " << safe_edge << endl;
			}
		}
	}
	in_row_MST[safe_index] = true;
	push(MST, safe_index);
//	cout << "Safe_index: " << safe_index << ", safe_edge " << safe_edge << endl;
	return safe_edge;
}

int main(){
	int number_vertices;
	fstream file;
	file.open("input.txt");
	file >> number_vertices;
	int **array;
	bool* in_row_MST = new bool[number_vertices]; //index is true if in mst. Initially all false
	struct vertices* MST;
	MST = new vertices;

	int* test;
	test = new int[number_vertices];
	MST->mst = new int[number_vertices];

	MST->size = 0;
	push(MST, 0);

	array = initialize_arrays(number_vertices, file);\
	file.close();
	in_row_MST = initializeMST(number_vertices);
	in_row_MST[0] = true;
	print_array(array, number_vertices);

	prim(array, number_vertices, in_row_MST, MST);
	return 0;

}
