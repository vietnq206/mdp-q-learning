#include <fstream>
#include <iostream>
#include <string>
using namespace std;
 
int main () {
   string data;

   // open a file in write mode.
   ofstream outfile;
   outfile.open("afile.txt");


   cout << "Writing to the file" << endl;
   cout << "Enter your name: "; 
   cin>>data;

   // write inputted data into the file.
   outfile << data<<" ";

   cout << "Enter your age: "; 
   cin >> data;
   outfile<<endl;
   
   // again write inputted data into the file.
   outfile << data << endl;


      cout << "Writing to the file" << endl;
   cout << "Enter your name: "; 
   cin>>data;

   // write inputted data into the file.
   outfile << data<<" ";

   cout << "Enter your age: "; 
   cin >> data;
   cin.ignore();
   
   // again write inputted data into the file.
   outfile << data << endl;


   

   // close the opened file.
   outfile.close();


}