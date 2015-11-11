/**********************************************************************
 * main.cpp - CSCI124 - Ass5 - main() driver for displaying text file word statistics
 * <NAME> <STUDENT No.> <DATE>
 **********************************************************************/
 
#include <iostream>
#include "ass5.h"
using namespace std;

char Menu();

int main(){

   cout << "Welcome to the Text File Word Counter\n";
   for(;;){
      switch(Menu()){
         case 't': TestContainers();   break;
         case 'r': ReadTxtFile();      break;
         case 'd': DisplayWordStats(); break;
         case 'q': CleanUp();
                   cout << "Thanks for using Text File Word Counter\n";
                   return(1);
         default: cout << "Invalid command!\n";
      }
   }
}


char Menu(){
   char Cmd[20];
   cout << endl;
   cout << "(t)est container classes\n";
   cout << "(r)ead text file\n";
   cout << "(d)isplay word stats\n";
   cout << "(q)uit\n";
   cout << "Command > ";
   cin.getline(Cmd,20);
   cout<<endl;
   return Cmd[0];
}
