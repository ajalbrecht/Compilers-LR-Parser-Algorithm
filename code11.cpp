#include <iostream>
#include <stack>
using namespace std;
#include <stdbool.h>

void check(string Word);
void print_stack(stack<string> Orginal);

int main() {
  cout << "-----------------------------------------------------------" << '\n';
  cout << "     Name      : AJ Albrecht" << '\n';
  cout << "     Assignment: No.11" << '\n';
  cout << "     Due Date  : 11/18" << '\n';
  cout << "Purpose: Use LR parsing method to check given strings." << '\n';
  cout << "------------------------------------------------------------" << '\n';

  check("(i+i)*i$");
  check("(i*)$");
  return 0;
}

void check(string Word) {
  string Table[17][12] = {{"X","i","+","-","*","/","(",")","$","E","T","F"},
                         {"0","S5","X","X","X","X","S4","X","X","1","2","3"},
                         {"1","X","S6","S7","X","X","X","X","Y","X","X","X"},
                         {"2","X","R3","R3","S8","S9","X","R3","R3","X","X","X"},
                         {"3","X","R6","R6","R6","R6","X","R6","R6","X","X","X"},
                         {"4","S5","X","X","X","X","S4","X","X","10","2","3"},
                         {"5","X","R8","R8","R8","R8","","R8","R8","X","X","X"},
                         {"6","S5","X","X","X","X","S4","X","X","X","11","3"},
                         {"7","S5","X","X","X","X","S4","X","X","X","12","3"},
                         {"8","S5","X","X","X","X","S4","X","X","X","X","13"},
                         {"9","S5","X","X","X","X","S4","X","X","X","X","14"},
                         {"10","X","S6","S7","X","X","X","S15","X","X","X","X"},
                         {"11","X","R1","R1","S8","S9","X","R1","R1","X","X","X"},
                         {"12","X","R1","R2","S8","S9","X","R2","R2","X","X","X"},
                         {"13","X","R4","R4","R4","R4","X","R4","R4","X","X","X"},
                         {"14","X","R5","R5","R5","R5","X","R5","R5","X","X","X"},
                         {"15","X","R7","R7","R7","R7","X","R7","R7","X","X","X"}};
  int Rule[9] = {0,6,6,2,6,6,2,6,2};
  string Rule_State[9] = {"X","E","E","E","T","T","T","F","F"};
  stack<string> Stack;
  bool Continue;
  bool valid = 1;
  string Push_String;
  string Current_Char;
  int Char_Number = -1;
  int Char_Table_Pos;
  string Current_State;
  int State_Number;
  int State_Table_Pos;
  string Input_char;
  string NonTerminal_next;

  Stack.push("0");
  std::cout << "Tracing " << Word;

  // as long as the word is good, check each charcter
  for (size_t i = 0; i < Word.length(); i++) {
    if (valid == false) {
      std::cout << "This word is not included in the gramar"  << '\n';
      break;
    }

    Current_Char = Word[++Char_Number];
    // get input row number
    for (size_t i = 1; i < 18; i++) {if (Table[0][i] == Current_Char) {Char_Table_Pos = i; }}

    // While the charcter has not been found, keep looking
    Continue = true;
    while (Continue == true) {
      //print_stack(Stack);
      Current_State = Stack.top();
      std::cout << '\n' << "Pop: " << Current_State;
      Stack.pop();

      // get state row and colloum numbers
      for (size_t i = 1; i < 18; i++) {if (Table[i][0] == Current_State) {State_Table_Pos = i;}}
      for (size_t i = 1; i < 18; i++) {if (Table[0][i] == Current_Char) {Char_Table_Pos = i;}}

      // Get value of elment on table with given cordinates
      Push_String = Table[State_Table_Pos][Char_Table_Pos];
      Input_char = Push_String[0];

      if (Push_String == "X") {
        // An empty box was read. The gramar is incorect.
        print_stack(Stack);
        std::cout << "    Fallied to find-> " << Current_Char <<'\n';
        valid = false;
        break;

      } else if (Push_String == "Y") {
        // Y has been read, and the gramar is correct
        std::cout << "      Go to: [" << Current_Char << "," << Current_State << "] = " << Push_String;
        std::cout << "    The final $ has been reached!" << '\n';
        std::cout << "This word is included in the gramar" << '\n';
        break;

      } else {
        // If the input is RX, followed by X wher x is int, follow this path
        if (Input_char == "R") {
          // get table row and collum positions
          for (size_t i = 1; i < 18; i++) {if (Table[0][i] == Current_Char) {Char_Table_Pos = i;}}
          for (size_t i = 1; i < 18; i++) {if (Table[i][0] == Current_State) {State_Table_Pos = i;}}
          Push_String = Table[State_Table_Pos][Char_Table_Pos];

          // RX part of input
          Stack.push(Current_State);
          string Input_char;
          if (Push_String.length() == 2) { Input_char = Push_String.substr(1,1); }
          else {Input_char = Push_String.substr(1,2);}
          int temp3 = stoi(Input_char);
          int temp = Rule[temp3];
          NonTerminal_next = Rule_State[temp3];
          if (temp == 6) {Stack.pop(); Stack.pop(); Stack.pop(); Stack.pop();}
          Stack.pop(); Stack.pop();

          // Report operations
          std::cout << "      Go to: [" << Current_State << "," << Current_Char << "] = " << Push_String;
          std::cout << "      Pop 2a values: " << temp;
          print_stack(Stack);
          Current_State = Stack.top();
          std::cout << '\n' << "Pop: " << Current_State;
          Stack.pop();

          // # portion --------------------------------------
          // get corsponding state and non terminal input from table
          for (size_t i = 1; i < 18; i++) {if (Table[i][0] == Current_State) {State_Table_Pos = i;}}
          for (size_t i = 1; i < 18; i++) {if (Table[0][i] == NonTerminal_next) {Char_Table_Pos = i;}}
          Push_String = Table[State_Table_Pos][Char_Table_Pos];

          // push # components
          Stack.push(Current_State);
          Stack.push(NonTerminal_next);
          Stack.push(Push_String);

          // report operations
          std::cout << "      Go to: [" << Current_State << "," << Current_Char << "] = " << Push_String;
          std::cout << "      Push: " << Current_State << " " << NonTerminal_next << " " << Push_String;
          print_stack(Stack);

        } else { // SX input path where x equals integer
          Stack.push(Current_State);
          Stack.push(Current_Char);
          if (Push_String.length() == 2) { Input_char = Push_String[1]; }
          else {Input_char = Push_String.substr(1,2);}
          Stack.push(Input_char);

          // report operations
          std::cout << "      Go to: [" << Current_State << "," << Current_Char << "] = " << Push_String;
          std::cout << "    Push: " << Current_State << " " << Current_Char << " " << Input_char;
          print_stack(Stack);
          std::cout << "      Match-> " << Current_Char; // match was found
          break;
        }
      }
    }
  }
  std::cout << '\n';
}

void print_stack(stack<string> Original) {
  stack<string> Print_Stack;
  // Create a new stack with the orginal stack contents backwords
  // This will allow the print mesage to be in the right order
  while (Original.empty() == false) {
    Print_Stack.push(Original.top());
    Original.pop();
  }
  std::cout << "      Stack-> ";
  while (Print_Stack.empty() == false) {
    std::cout << Print_Stack.top();
    Print_Stack.pop();
  }
}
