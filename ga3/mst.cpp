//By Bradford Wong, Dennis Li, and Thomas Huynh
//
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <time.h>

using namespace std;

int MST1Sum = 99999999;
int MST2Sum = 99999999;
int MST3Sum = 99999999;

struct vertices{
	int* mst; //an array that is composed of all the vertices added to MST. Ex: if '2' is this array, then the second vertex is already in the mst
	int size; //how many vertices are in the mst array
};

struct line{
	int p1;
	int p2;
};

struct edge{
	int p1;
	int p2;
	struct line* mst;
	int size;
};

int** initialize_arrays(int, fstream&);
void print_array(int**, int);
int find_safe_edge(int**, int, int, bool*, vertices*);
int prim(int**, int, bool*, vertices*, edge*, int);
int prim3(int**, int, bool*, vertices*, edge*, edge*, edge*, edge*);
bool* initializeMST(int);
void add_to_MST(bool*, int);
vertices* initialize_vertices(vertices*, int);
void push(vertices*, int);
int find_smallest_edge(int**, int, bool*, vertices*, edge*, int);
int find_smallest_edge2(int**, int, bool*, vertices*, edge*, edge*);
void resetMST(bool*, int);
void prim2(int**, int, bool*, vertices*, vertices*);
int secondSmallest(int**, int row, int);
void findSmallestTwo(int, edge*, edge*, edge*, int);
bool checkNotSame(vertices*, vertices*, int);
bool checkNotIgnored(edge*, int, int);
void pushEdge(edge*, int, int);
bool notSameMST(edge*, edge*, int);
void swapMST(edge*, edge*, int);

void swapMST(edge* MST1, edge* MST2, int number_vertices){
	edge* tempEdge;
	tempEdge = new edge;
	tempEdge ->mst = new line[MST1->size];
	tempEdge->size = 0;
	for(int i = 0; i < MST1->size; i++){
		pushEdge(tempEdge, MST1->mst[i].p1, MST1->mst[i].p2);
	}
	MST1->size = 0;
	for(int i = 0; i < tempEdge->size; i++){
		pushEdge(MST1, MST2->mst[i].p1, MST2->mst[i].p2);
	}
	MST2->size = 0;
	for(int i = 0; i < tempEdge->size; i++){
		pushEdge(MST2, tempEdge->mst[i].p1, tempEdge->mst[i].p2);
	}
}

void pushEdge(edge* MST1, int safe_index, int safe_row){
	if(safe_row < safe_index){
		MST1->mst[MST1->size].p1 = safe_row;
		MST1->mst[MST1->size].p2 = safe_index;
	}else{
		MST1->mst[MST1->size].p1 = safe_index;
		MST1->mst[MST1->size].p2 = safe_row;
	}

	MST1->size = MST1->size + 1;
}

void findSmallestTwo(int sum, edge* tempMST, edge* MST2, edge* MST3, int number_vertices){
	int smallest = MST2Sum;
	int second = MST3Sum;
//	cout << "In smallest two. MST2->size is " << MST2->size << endl;
//	cout << "MST 2 was " << MST2Sum  << ", MST3 was " << MST3Sum << endl;
	if(sum < smallest){
//		cout << "In sum <  Smallest. MST2-> size is  " << MST2->size << endl;
	/*	for(int i = 0; i < MST2->size; i++){
			cout << "MSTtemp: " << tempMST->mst[i].p1 << "," << tempMST->mst[i].p2 << ", MST2: " << MST2->mst[i].p1 << ", " << MST2->mst[i].p2 << endl;
			cout << "inside for loop " << endl;
		}*/
		//cout << "Just exited for loop " << endl;
		second = smallest;
		//cout << "Before swap MST3 with MST2, tempMST. Size of MST2 " << MST2->size << ", MST3: " << MST3->size <<", tempMST->size; " << tempMST->size << endl;
		//swapMST(MST3, MST2, number_vertices);
		smallest = sum;
		//cout << "Before swap MST2 with tempMST, tempMST. Size of MST2 " << MST2->size << ", MST3: " << MST3->size <<", tempMST->size; " << tempMST->size << endl;
		swapMST(MST2, tempMST, number_vertices);
	}else if (sum < second){
	//	cout << "In sum < second " << endl;
		second = sum;
	//	cout << "Before swap MST3, tempMST. Size of MST2 " << MST2->size << ", MST3: " << MST3->size <<", tempMST->size; " << tempMST->size << endl;
		//swapMST(MST3, tempMST, number_vertices);
	}
	MST2Sum = smallest;
	MST3Sum = second;
	//cout << "Now MST2 is " << MST2Sum << ", MST3 is " << MST3Sum << endl;
//	cout << "End of smallest two. Size of MST2 " << MST2->size << ", MST3: " << MST3->size << endl;
}

void resetMST(bool* in_row_MST, int number_vertices){
	for(int i = 0; i < number_vertices; i++){
		in_row_MST[i] = false;
	}
}

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
int prim(int** array, int number_vertices, bool* in_row_MST, vertices* MST, edge* MST1, int x){
	int MST_sum = 0;
	//cout << "In prim " << endl;
	for(int i = 0; i < number_vertices -1; i ++){ //iterate number_vertices-1 because total number of edges is # of vertices -1
		MST_sum += find_smallest_edge(array, number_vertices, in_row_MST, MST, MST1, 0); //add weight of each safe edge to running total
		//in_row_MST[i] = true;
	}
	MST1Sum = MST_sum;
	//cout << "MST SUM IS " << MST_sum << endl;
}

bool notSameMST(edge* MST1, edge* MST2 , int number_vertices){
	bool check = false;
	for(int i = 0; i < number_vertices-1; i++){
		//if(MST1->mst[i].p1 != MST2->mst[0].p1 && MST1->mst[i].p2 != MST2->mst[0].p1){
			for(int j = 0; j < number_vertices-1; j++){
				//cout << "Comparing " << MST1->mst[i].p1 << ", " << MST1->mst[i].p2 << " to " << MST2->mst[j].p1 << ", " << MST2->mst[j].p2 << endl;
				if(MST1->mst[i].p1 == MST2->mst[j].p1 && MST1->mst[i].p2 == MST2->mst[j].p2){
			//		cout << "They are the same, so check is false and breaking out. " << endl;
					check = false;
					break;
				}
				check = true;
			}
			//cout << "Just broke out of first loop " << endl;
			if(check == true){
			//	cout << "Check is TRUE, so returning " << endl;
				return true;
			}
		//}
	}
//	cout << "Returning false " << endl;
	return false;
	/*
	bool check = false;
	cout << "Comparing: " << endl;
	for(int i = 0; i < MST2->size; i++){
		cout << "Comparing " << MST1->mst[i].p1 << ", " << MST1->mst[i].p2 << " to " << MST2->mst[i].p1 << ", " << MST2->mst[i].p2 << endl;
	}
	for(int i = 0; i < number_vertices-1; i++){*/
		/*
		if(MST1->mst[i].p1 != MST2->mst[0].p1 && MST1->mst[i].p2 != MST2->mst[0].p1){
			for(int j = 1; j < number_vertices-1; j++){
				//cout << "Comparing " << MST1->mst[i].p1 << ", " << MST1->mst[i].p2 << " to " << MST2->mst[j].p1 << ", " << MST2->mst[j].p2 << endl;
				if(MST1->mst[i].p1 == MST2->mst[j].p1 && MST1->mst[i].p2 == MST2->mst[j].p2){
					//cout << "They are the same, so check is false and breaking out. " << endl;
					check = false;
					break;
				}
				check = true;
			}
			if(check == true){
			//	cout << "Check is TRUE, so returning " << endl;
				return true;
			}
		}
	}
	return false;*/
	/*
	cout << "In notSameMST " << endl;
	bool check = false;
	for(int i = 0; i < number_vertices-1; i++){
		cout << "i is " << i << ", MST1: " << MST1->mst[i].p1 << ", " << MST1->mst[i].p2 << endl;
		for(int j = 0; j < number_vertices-1; j++){

			cout << "MST2: " << MST2->mst[j].p1 << ", " << MST2->mst[j].p2 << endl;
			if(MST1->mst[i].p1 != MST2->mst[j].p1 && MST1->mst[i].p2 != MST2->mst[j].p2){
				cout << "Not same, so returning true " << endl;
				check = true;
				//return true;
			}
		}
	}
	cout << "Returning  " << check << endl;
	return check; //MSTS are the same*/
}

//OVERLOADED ONE

int prim3(int** array, int number_vertices, bool* in_row_MST, vertices* MST, edge* ignored, edge* MST1, edge* MST2, edge* MST3){
	//cout << "In overloaded one " << endl;
	struct edge* tempMST;
	tempMST = new edge;
	tempMST->mst= new line[number_vertices-1];
	tempMST->size = 0;
	for(int i = 0; i < number_vertices-1; i++){
		tempMST->mst[i].p1 = -1;
		tempMST->mst[i].p2 = -1;

	}
	int MST_sum = 0;
	//cout << "In prim " << endl;
	for(int i = 0; i < number_vertices -1; i ++){ //iterate number_vertices-1 because total number of edges is # of vertices -1
		MST_sum += find_smallest_edge2(array, number_vertices, in_row_MST, MST, ignored, tempMST); //add weight of each safe edge to running total
		//in_row_MST[i] = true;
		if (MST_sum > MST3Sum) {
			return 99999999;
		}
	}
	//cout << "MST SUM2 IS " << MST_sum << endl;
//	cout << "Before notSameMst. MST2 size is: " << MST2->size << endl;


	if(notSameMST(MST1, tempMST, number_vertices) && notSameMST(MST2, tempMST, number_vertices)){
	//	cout << "Not same, so returning:  "<<  MST_sum << endl;
		findSmallestTwo(MST_sum, tempMST, MST2, MST3, number_vertices);
		//delete tempMST->mst;
		//delete tempMST;

		return MST_sum;
	}else{
		//cout << "Same, so returning 9999999 " << endl;
		//findSmallestTwo(99999999, tempMST, MST2, MST3, number_vertices);
		//delete tempMST->mst;
		//delete tempMST;

		return 99999999;
	}
}

int secondSmallest(int** array, int row, int number_vertices){
	int smallest = array[row][0];
	int second = array[row][1];
	int smallestIndex = 0;
	int secondIndex = 1;
	if(second < smallest){
		int temp = second;
		second = smallest;
		smallest = temp;
		secondIndex = 0;
		smallestIndex = 1;
	}
	for(int i = 2; i < number_vertices; i++){
		if(array[row][i] < smallest){
			second = smallest;
			secondIndex = smallestIndex;
			smallest = array[row][i];
			smallestIndex = i;
		}else{
			if(array[row][i] < second){
				second = array[row][i];
				secondIndex = i;
			}
		}
	}
	return secondIndex;
}

bool checkNotSame(vertices* originalMST, vertices* MST2, int number_vertices){
	for(int i = 0; i < number_vertices; i++){
		if(originalMST->mst[i] != MST2->mst[i]){
			return true;
		}
	}
	return false;
}
/*
void prim2(int** array, int number_vertices, bool* in_row_MST, vertices* MST2, vertices* originalMST){
	int MST_sum = 0;
	int starting_edge_MST2=-1;
	//cout << "In prim " << endl;
	for(int j = 0; j < number_vertices; j++){ //may need to check so edge isn't same edge in the first edge of mst2
		MST_sum = 0;
		if(starting_edge_MST2 != secondSmallest(array, j, number_vertices)){
			push(MST2, secondSmallest(array, j, number_vertices));


			for(int i = 0; i < number_vertices -1; i ++){ //iterate number_vertices-1 because total number of edges is # of vertices -1
				MST_sum += find_smallest_edge(array, number_vertices, in_row_MST, MST2); //add weight of each safe edge to running total
				//in_row_MST[i] = true;
			}
			if(checkNotSame(originalMST, MST2, number_vertices)){
				findSmallestTwo(MST_sum);
			}
			resetMST(in_row_MST, number_vertices);
			MST2->size = 0;
		}

	}*/
	/*
	for(int i = 0; i < number_vertices -1; i ++){ //iterate number_vertices-1 because total number of edges is # of vertices -1
		MST_sum += find_smallest_edge(array, number_vertices, in_row_MST, MST2); //add weight of each safe edge to running total
		//in_row_MST[i] = true;
	}*/
//	cout << "MST SUM IS " << MST_sum << endl;
//}

//FUNCTION: returns the weight of the safe edge
//PARAMETERS: array-the adjacency matrix given from the input file, in_row_MST- array where indices with "true" in it mean that vertex is in the MST (vertex # = in_row_MST index)
//MST-the minimum spanning tree, composed of vertices
int find_smallest_edge(int** array, int number_vertices, bool* in_row_MST, vertices* MST, edge* MST1, int x){
	int safe_edge = 99999999; //the weight of the safe edge
	int safe_index; //the index where the safe edge is in the array
	int safe_row;
	for(int j = 0; j < MST->size; j++){ //this steps through each vertex stored in MST array
		int row = MST->mst[j]; //the row is the current vertex in MST
		//cout << "Now checking row: " << row << endl;
		for(int i = 0; i < number_vertices; i++){ //this steps through the actual row in the adjacency matrix array
			//cout << "Looking at [" << row << "][" << i << "]" << endl;
			if (array[row][i] < safe_edge && array[row][i] != 0 && in_row_MST[i] == false){
				safe_index = i;
				safe_edge = array[row][safe_index];
				safe_row = row;
				//cout << "safe_edge is now " << safe_edge << endl;
				//cout << "Safe edge is now " << safe_edge << endl;
			}
		}
	}
	in_row_MST[safe_index] = true;
	push(MST, safe_index);
	pushEdge(MST1, safe_index, safe_row);
//	cout << "Safe_index: " << safe_index << ", safe_edge " << safe_edge << endl;
	return safe_edge;
}

//overloaded find_smallest_edge
int find_smallest_edge2(int** array, int number_vertices, bool* in_row_MST, vertices* MST, edge* ignored, edge* tempMST){
//	cout << "In overloaded find smalest edge " << endl;
	int safe_edge = 99999999; //the weight of the safe edge
	int safe_index; //the index where the safe edge is in the array
	int safe_row;
	for(int j = 0; j < MST->size; j++){ //this steps through each vertex stored in MST array
	//	cout << "j is " << j << endl;
		int row = MST->mst[j]; //the row is the current vertex in MST
		//cout << "Now checking row: " << row << endl;
		for(int i = 0; i < number_vertices; i++){ //this steps through the actual row in the adjacency matrix array
			//cout << "i is " << i << endl;
			//cout << "Looking at [" << row << "][" << i << "]" << endl;
			if (array[row][i] < safe_edge && array[row][i] != 0 && in_row_MST[i] == false && checkNotIgnored(ignored, i, row) == false){
			//	cout << "i is " << i << endl;
				safe_index = i;
				safe_row = row;
				//cout << "Safe index is: " << safe_index << " and should be " << i << endl;
				safe_edge = array[row][safe_index];
				//cout << "safe_edge is now " << safe_edge << endl;
				//cout << "Safe edge is now " << safe_edge << endl;
			}
		}
	}
//	cout << "Safe edge is " << safe_edge << endl;
//	cout << "Found safe index " << endl;
//	cout << "Safe index is " << safe_index << endl;
	in_row_MST[safe_index] = true;
//	cout << "Set in_row_MST index " << endl;
	push(MST, safe_index);
	pushEdge(tempMST, safe_index, safe_row);
//	cout << "Safe_index: " << safe_index << ", safe_edge " << safe_edge << endl;
//cout << "Pushed safe index " << endl;
	return safe_edge;
}

bool checkNotIgnored(edge* ignored, int i, int row){
	if(ignored->p1  == i && ignored->p2 == row){
		return true;
	}else if(ignored->p2 == i && ignored->p1 == row){
		return true;
	}else{
		return false;
	}
}

int main(){
	//clock_t tStart = clock();
	int number_vertices;
	fstream file;
	file.open("input.txt");
	file >> number_vertices;
	int **array;
	bool* in_row_MST = new bool[number_vertices]; //index is true if in mst. Initially all false
	struct vertices* MST;
	MST = new vertices;

	MST->mst = new int[number_vertices];

	MST->size = 0;
	push(MST, 0);

	array = initialize_arrays(number_vertices, file);\
	file.close();
	in_row_MST = initializeMST(number_vertices);
	in_row_MST[0] = true;
	//print_array(array, number_vertices);

	struct edge* MST1;
	MST1 = new edge;
	MST1->mst= new line[number_vertices-1];
	MST1->size = 0;
	for(int i = 0; i < number_vertices-1; i++){
		MST1->mst[i].p1 = -1;
		MST1->mst[i].p2 = -1;
	}
	struct edge* MST2;
	MST2 = new edge;
	MST2->mst= new line[number_vertices-1];
	MST2->size = 0;
	for(int i = 0; i < number_vertices-1; i++){
		pushEdge(MST2, -1, -1);
	}
	//cout << "After initial allocation. MST2->size is " << MST2->size << endl;

	struct edge* MST3;
	MST3 = new edge;
	MST3->mst= new line[number_vertices-1];
	/*MST3->size = 0;
	for(int i = 0; i < number_vertices-1; i++){
		pushEdge(MST3, -1, -1);
	}*/

	prim(array, number_vertices, in_row_MST, MST, MST1, 0);

	struct edge* ignored;
	ignored = new edge;

	for(int i = 0; i < number_vertices; i++){
		for(int j  = i+1; j < number_vertices; j++){
			ignored->p1 = i;
			ignored->p2 = j;
			//cout << "Ignoring edge: " << ignored->p1 << "-" << ignored->p2 << endl;
			resetMST(in_row_MST, number_vertices);
			MST->size = 0;
			push(MST, 0);
			in_row_MST[0] = true;
			prim3(array, number_vertices, in_row_MST, MST, ignored, MST1, MST2, MST3); //use overloaded one
			//put findSmallest two inside prim

		}
	}
	//cout << "After prim. MST2->size is " << MST2->size << endl;
//	for(int i = 0; i < MST2->size; i++){
		//cout << "MST2: " << MST2->mst[i].p1 << "," << MST2->mst[i].p2 << ", MST3: " << MST3->mst[i].p1 << ", " << MST3->mst[i].p2 << endl;

//	}

	/*
	delete ignored;

	delete[] MST1->mst;
	delete MST1;

	delete[] MST2->mst;
	delete MST2;

	delete[] MST->mst;
	delete MST;

	delete[] in_row_MST;

	for(int i = 0; i < number_vertices; i++){
		delete [] array[i];
	}
	delete [] array;
	*/
	//cout << "Order of MST1: " << endl;
	//for(int i = 0; i < number_vertices-1; i++){
			//cout << "[" << MST1->mst[i].p1 << "][" << MST1->mst[i].p2  << "]" << endl;
	//}


	/**/
ofstream output_file;
output_file.open("output.txt");
output_file << MST1Sum << "\n" << MST2Sum << "\n" << MST3Sum;
output_file.close();


/*cout << "MST1: " << MST1Sum << endl;
cout << "MST2: " << MST2Sum << endl;
cout << "MST3: " << MST3Sum << endl;*/
//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC); //tbh not sure if this works

/*
cout << "Order of MST1: " << endl;
for(int i = 0; i < number_vertices-1; i++){
		cout << "[" << MST1->mst[i].p1 << "][" << MST1->mst[i].p2  << "]" << endl;
}
*/
/*
//for finding MST 2 and 3
	struct vertices* MST2;
	MST2 = new vertices;
	MST2->mst = new int[number_vertices];
	MST2->size = 0;
	resetMST(in_row_MST, number_vertices);

	prim2(array, number_vertices, in_row_MST, MST2, MST);
	cout << "MST2: " << MST2Sum << endl;
	cout << "MST3: " << MST3Sum << endl;*/
	return 0;

}
