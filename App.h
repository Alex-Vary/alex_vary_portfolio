#ifndef App_h
#define App_h

#include <vector>
#include <string>
using namespace std;

struct Variable
{
  string tag;
  int value;
  /*tag is the variable identifier
  value is the integer attached to that identifier*/
};

class App
{
public:
  /*PURPOSE: Perform all actions to read and parse A++ code.
  PRE: None.
  POST: Performs the A++ code.*/
  App();

  /*PURPOSE: Implicitly deletes all vectors for associated code.
  PRE: None.
  POST: Deallocates vector memory.*/
  ~App();

private:
  /*PURPOSE: Read code from a file.
  PRE: A valid user input of a file.
  POST: The file is opened and each line is read for its label and code.*/
  void fromFile();

  /*PURPOSE: Read code from user input.
  PRE: None.
  POST: The user's lines are read for its label and code.*/
  void fromKeyboard();

  /*PURPOSE: Read a line of code and break it into the label and code.
  PRE: A string with an integer label followed by a space and valid A++ statement.
  POST: The line is broken into its label and code.*/
  void lineReader(string);

  /*PURPOSE: Check for preliminary errors.
  PRE: Filled labels and lines vectors.
  POST: If there are any syntax errors that can be picked up before the code running, the code does not run and the errors are output.*/
  bool errorCheck();

  /*PURPOSE: Read the lines of code and perform their tasks.
  PRE: Filled labels and lines vectors, and no errors from errorCheck.
  POST: The code is performed, or stops and a syntax error is listed.*/
  void parse();

  /*PURPOSE: Evaluate and perform an A++ if statement.
  PRE: The index of a line of code formatted as an if statement.
  POST: The if statement is performed, and the current label is changed to match the goto label, if applicable.*/
  void ifStatement(unsigned int&);

  /*PURPOSE: Reads an identifier from a line and its value, to create a new variable.
  PRE: The index of a line of code that initializes an A++ variable.
  POST: Creates the variable with its identifier and value, and adds it to variables vector.*/
  void createVar(unsigned int);

  /*PURPOSE: Assign an expression value to an A++ variable.
  PRE: The index of a line of code with an already-initialized variable.
  POST: The expression is evaluated and assigned to that variable's value.*/
  bool assignment(unsigned int);

  /*PURPOSE: Print the value of an expression.
  PRE: The index of the line being evaluated.
  POST: The expression's integer value is output.*/
  void print(unsigned int);

  /*PURPOSE: Retrieve the string-form of a variable's value.
  PRE: A pre-existing identifier as a string.
  POST: The value is returned as a string.*/
  string var2val(string);

  /*PURPOSE: Evaluate an A++ expression.
  PRE: A string that contains identifier operands and A++ operators.
  POST: THe value of the expression is returned.*/
  int expression(string);

  /*PURPOSE: Convert an infix expression to a postfix expression.
  PRE: An infix expression as a string.
  POST: Returned psotfix form of the infix expression.*/
  string infix2postfix(const string&);

  /*PURPOSE: Evaluate a postfix expression
  PRE: A valid postfix expression with spaces between all numbers and operators.
  POST: The integer value of the expression is returned.*/
  int evalPostfix(const string&);

  /*PURPOSE: Convert an integer to a string.
  PRE: A positive or negative integer.
  POST: String form of an integer, with 'n' in place of a '-' for easy expression analysis.*/
  string int2str(int);

  /*PURPOSE: Convert a string to an integer.
  PRE: A string of digits, and either a '-' or 'n' at the beginning.
  POST: The integer form of the string is returned.*/
  int str2int(string);

  /*PURPOSE: Tests to see if a string is a valid A++ identifier.
  PRE: A string.
  POST: False is returned if it isn't an identifier, true if it is.*/
  bool isIdentifier(string);

  vector<string> lines;
  vector<int> labels;
  vector<Variable> variables;
  /*lines is a vector of strings, and it's the collection of actual lines of code.
  labels is a vector of integers, and it contains the labels corresponding to the line of the same index in lines.
  variables is a vector of Variables (structure), and contains all the tags and values of the variables in the code.*/
};

#endif