#include "./cnf2sat.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <vector>
using namespace std;

struct bulb{
      int switch1;
      int switch2;
};

int main(){
      fstream file;
      file.open("input.txt");
      int number_bulbs, number_switches;
      file >> number_switches;
      file.ignore();
      file >> number_bulbs;
      cout << "There are " << number_switches << " switches, bulbs: " << number_bulbs << endl;
      struct bulb  bulbs[number_bulbs];
      int initialState[number_bulbs];


/* INITIALIZE THE SWITCHES */
      for(int i = 0; i < number_bulbs; i++){
            bulbs[i].switch1 = -9999999;
            bulbs[i].switch2 = -9999999;
      }

/*Read in initial states from input file. */
      for(int i = 0; i < number_bulbs; i++){
            if(file.peek() == ','){
                  file.ignore();
            }
            file >> initialState[i];
           cout << initialState[i] << ","; //uncomment if want to see initial states
      }
cout << endl;

      /*Read in which switches are connected to what bulbs and pass them into the array of bulbs*/
      string temporaryString = "";
      getline(file, temporaryString);
      for(int line = 0; line < number_switches; line++){
            bool atEnd = false;
            string temp = "";
            string s;
            getline(file, s);
            cout << "Just read in the line: " << s << endl;
            for(int i = 0; i < s.length(); i++){
            //      cout << "End of string is " << s[s.length()] << endl;
                  //cout << "string length is " << s.length() << endl;
                  if(s[i] != ',' && s[i] != s[s.length()]) {
                        temp += s[i];
                        //cout << "Temp is now " << temp << ", i is" << i << endl;
                        if(i + 1 == s.length()){
                              int tempBulb = atoi(temp.c_str());
                              temp = "";
                              tempBulb--;
                              //cout << "Putting " << line + 1 << ", into bulb " << tempBulb << endl;
                              if(bulbs[tempBulb].switch1 == -9999999){
                                    bulbs[tempBulb].switch1 = line + 1;
                              }else{
                                    bulbs[tempBulb].switch2 = line + 1;
                              }
                        }
                  }else{
                        int tempBulb = atoi(temp.c_str());
                        temp = "";
                        tempBulb--;
                        //cout << "Putting " << line + 1 << ", into bulb " << tempBulb << endl;
                        if(bulbs[tempBulb].switch1 == -9999999){
                              bulbs[tempBulb].switch1 = line + 1;
                        }else{
                              bulbs[tempBulb].switch2 = line + 1;
                        }
                  }
            }
            //cout << "Line " << line << ": ";
            /*
            while(atEnd == false){
                  if(file.peek() == ','){
                        file.ignore();
                  }

                  int tempBulb;
                  file >> tempBulb;
                  cout << tempBulb << ", ";
                  tempBulb--;
                  if(bulbs[tempBulb].switch1 == -9999999){
                        bulbs[tempBulb].switch1 = line + 1;
                  }else{
                        bulbs[tempBulb].switch2 = line + 1;
                  }

                  if(file.eof() || file.peek() == '\n')
                        atEnd = true;

            }*/
            cout << endl;
      }
      /*Set up vector of 2SAT statements. */
      vector<pair<int, int> > test;
      for(int i = 0; i < number_bulbs; i++){
            //cout << i << endl;
            if(initialState[i] == 1){
                  test.push_back(make_pair(bulbs[i].switch1 * -1, bulbs[i].switch2));
                  //cout << "Pushed " << bulbs[i].switch1 * -1 << ", " << bulbs[i].switch2 << endl;
                  test.push_back(make_pair(bulbs[i].switch1, bulbs[i].switch2 * -1));
                  //cout << "Pushed " <<bulbs[i].switch1 << ", " <<  bulbs[i].switch2 * -1 << endl;
            }else{
                  test.push_back(make_pair(bulbs[i].switch1, bulbs[i].switch2));
            //      cout << "Pushed " << bulbs[i].switch1  << ", " << bulbs[i].switch2 << endl;
                  test.push_back(make_pair(bulbs[i].switch1*-1, bulbs[i].switch2*-1));
            //      cout << "Pushed " <<bulbs[i].switch1 * -1 << ", " <<  bulbs[i].switch2 * -1 << endl;
            }

      }
      /*
      for(int i = 0; i < number_bulbs; i++){
            cout << " Bulb " << i << " is connected to " << bulbs[i].switch1 << ", " << bulbs[i].switch2 << endl;
      }*/
      cout << "Answer is: " << satisfiable(test) << endl;
      test.clear();
      //cout << "At end of for loop " << endl;
      /*


            test.push_back(make_pair(-1, 3));
            test.push_back(make_pair(1, -3));
            test.push_back(make_pair(-3, 4));
            test.push_back(make_pair(3, -4));
            test.push_back(make_pair(-1, 4));
            test.push_back(make_pair(1, -4));
            test.push_back(make_pair(1, 2));
            test.push_back(make_pair(-1, -2));
            test.push_back(make_pair(-3, 4));
            test.push_back(make_pair(3, -4));
            cout << "Answer is: " << satisfiable(test) << endl;
            //
            /*
            string temp;
            getline(file, temp);
            cout << temp << endl;
            vector<int> vect;
            stringstream ss(temp);

            int i;
            while(ss >> i){
                  vect.push_back(i);

                  if(ss.peek() == ',')
                        ss.ignore();
            }
            temp = "";
            */




      cout << endl;

      file.close();
      return 0;
}
