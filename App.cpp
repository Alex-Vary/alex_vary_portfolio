#include "App.h"
#include <iostream>
#include <fstream>
#include <stack>

App::App()
{
  char input = 0;
  //input reads which option the user picks

  cout << "Choose 1 or 2:\n";
  cout << "1. Input A++ Program from file.\n";
  cout << "2. Input A++ Program from the keyboard.\n\n";

  //Loop runs until a valid choice is input.
  while (input != '1' && input != '2')
  {
    cin >> input;
    if (input == '1')
      fromFile();
    else if (input == '2')
      fromKeyboard();
    else
      cout << "Invalid input.\n";
  };

  //If no initial errors are found, the inputted code is parsed.
  if (errorCheck())
    parse();
}

App::~App()
{}

void App::fromFile()
{
  char fileName[51];
  string line;
  /*fileName is the user's choice of what file is to be read.
  line grabs the current line for reading*/
  cout << "Input file name with file extension, up to 50 characters long: ";
  cin >> fileName;

  ifstream inFile;

  inFile.open(fileName);
  //If the file wasn't opened, cancel the program execution.
  if (!inFile)
  {
    cout << "Cannot open " << fileName << endl;
    exit(1);
  }

  getline(inFile, line);
  //The loop starts on the first line, and reads it to put the parts into the correct vectors.
  while (!inFile.eof())
  {
    if (line.size() > 0)
      lineReader(line);
    getline(inFile, line);
  };
  //The loop ends before the very last line is read, so it needs to read it.
  if (line.size() > 0)
    lineReader(line);
  inFile.close();
}

void App::fromKeyboard()
{
  string line;
  bool runLoop = true;
  /*line is the current line being input by the user.
  runLoop controls when the user can input lines.*/

  cout << "Input lines of code with labels, pressing RETURN after each line. When finished, type /END.\n\n";
  while (runLoop)
  {
    //Whatever the user types is taken, up until each RETURN.
    std::getline(std::cin, line);
    //If the user did not input anything, the line is not counted at all.
    if (line.size() == 0)
      continue;
    //If the user ends their input, the loop ends.
    if (line.find("/END") != string::npos)
      runLoop = false;
    //Calls the lineReader function to take the line and put it into our vectors.
    else
      lineReader(line);
  };
}

void App::lineReader(string line)
{
  string *labelHolder;
  labelHolder = new string;
  size_t i = 0;
  size_t len = line.length();
  /*labelHolder is created and destroyed to take each label and put it in the vector.
  i is used to find where the label ends.
  len is the length of the entire line.*/

  //Runs until the end of the line, or breaks once the label ends.
  while (i < len)
  {
    //If the current character isn't between 0 and 9, it's not part of the label.
    if (line[i] < '0' || line[i] > '9')
    {
      //The current label being held is the full label, so it is pushed into the labels vector.
      labels.push_back(str2int(*labelHolder));
      //The current label being held is destroyed, and a new holder is made.
      delete labelHolder;
      labelHolder = new string;
      break;
    }
    else
    {
      //If the current character is a number, it's added to the holder, and i increases.
      *labelHolder += line[i];
      i++;
    }
  };

  //The rest of the line immediately after the label is pushed into the lines vector.
  lines.push_back(line.substr(i));
}

bool App::errorCheck()
{
  int errorNum = 0;
  bool stopState = false;
  //errorNum counts the errors encountered initially.
  //stopState detects if there are any stop statements in the code.

  cout << "List of Initial Syntax Errors: " << endl;
  for (unsigned int i = 0; i < lines.size(); i++)
  {
    //If there is no semicolon on a line, it's an error.
    if (lines[i].find(';') == string::npos)
    {
      errorNum++;
      cout << "No semicolon on line with label " << labels[i] << '.' << endl;
    }

    //The following if statement checks if the labels are out of numerical order, comparing the current to the next, as long as there is a next.
    if (i < (lines.size() - 1))
      if (labels[i] >= labels[i + 1])
      {
        errorNum++;
        cout << "Line with label " << labels[i] << " is greater than or equal to the next label." << endl;
      }

    //If the line doesn't start with a space, there was no space between the label and line.
    if (lines[i][0] != ' ')
    {
      errorNum++;
      cout << "No space after label " << labels[i] << '.' << endl;
    }

    //If there are no reserved words or an equality in a line, it is not a valid A++ statement.
    if (lines[i].find(" var ") == string::npos && lines[i].find(" print ") == string::npos && lines[i].find(" if ") == string::npos && lines[i].find(" stop;") == string::npos && lines[i].find('=') == string::npos)
    {
      errorNum++;
      cout << "Line with label " << labels[i] << " is an invalid A++ statement." << endl;
    }

    //If there are no stop statements in the entire code, stopState remains false.
    if (lines[i].find(" stop;") != string::npos)
      stopState = true;
  }

  //if the final statement of the code is not a stop statement, there is an error.
  if (lines[lines.size() - 1] != " stop;")
  {
    errorNum++;
    cout << "Final statement is not a stop statement.\n";
  }

  //if stopState is false, there are no stop statements, which is an error.
  if (!stopState)
  {
    errorNum++;
    cout << "No stop statement found.\n";
  }

  //If there are any errors, the function returns false. Otherwise, the object will parse its code.
  if (errorNum)
  {
    cout << endl;
    return false;
  }
  else
  {
    cout << "None!\n";
    return true;
  }
}

void App::parse()
{
  bool stopFound = false;

  cout << "\n-----\n" << "Program Execution:\n";
  //The loop executes for each line of input code, and based on the line, executes the proper function.
  for (unsigned int i = 0; !stopFound;)
  {
    if (lines[i].find(" if ") != string::npos)
      ifStatement(i);
    else if (lines[i].find(" var ") != string::npos)
    {
      createVar(i);
      i++;
    }
    else if (lines[i].find('=') != string::npos)
    {
      //If the assignment fails, the parsing stops and an error is output.
      if (!assignment(i))
        break;

      i++;
    }
    else if (lines[i].find(" print ") != string::npos)
    {
      print(i);
      i++;
    }
    else if (lines[i].find(" stop;") != string::npos)
      stopFound = true;
  }
}

void App::ifStatement(unsigned int& i)
{
  size_t start = lines[i].find('(') + 1;
  size_t end = lines[i].find(')') - start;
  int compNum, j;
  /*start is where the expression starts.
  end is the length of characters of the expression.
  compNum is the label after goto.
  j is used to find the index in labels that compNum matches a label.*/

  if (expression(lines[i].substr(start, end)))
  {
    //If the expression is not 0, start is set to after goto, and end is the length of the input label.
    start = lines[i].find("goto ") + 5;
    end = lines[i].find(";") - start;
    //compNum is set to the substring after goto and before ;, covnerted to an int.
    compNum = str2int(lines[i].substr(start, end));
    //compNum is searched for in labels, and once it is found, i is set equal to j. This returns i back to parse as the new label.
    for (j = 0; labels[j] != compNum && j < labels.size(); j++) {}
    i = j;
  }
  else
  {
    //If the expression is 0, i increases, and the next line in parse is read.
    i++;
    return;
  }
}

void App::createVar(unsigned int i)
{
  size_t tagLength = 0, valLength = 0;
  int j = lines[i].find(" var ") + 5, stringAdj = 1;
  Variable newVar;
  /*tagLength is the length of the identifier being parsed.
  valLength is the length of the value being assigned to the identifier.
  j is used to find the starting point of the identifier.
  stringAdj adjusts the string for the value.*/

  //The following loop determines the identifier's name, by finding where the first semicolon, space, or equality appears after " var ".
  while (lines[i][j] != ' ' && lines[i][j] != ';' && lines[i][j] != '=')
  {
    tagLength++;
    j++;
  }

  //The tag of this new variable is set to the substring of the current line, defined by the tagLength, starting right after " var ".
  newVar.tag = lines[i].substr(lines[i].find("var") + 4, tagLength);

  //If a space appears after the equality, the code must make an adjustment for that.
  if (lines[i].find("= ") != string::npos)
    stringAdj++;

  //If the new identifier has been assigned a value, the following executes.
  if (lines[i].find('=') != string::npos)
  {
    //The length of the value is the distance between the equality and semicolon, minus the adjustment.
    valLength = lines[i].find(';') - lines[i].find('=') - stringAdj;
    //The following statement converts the number extracted from lines[i] to an integer and assigns it to our new variable.
    newVar.value = str2int(lines[i].substr(lines[i].find('=') + stringAdj, valLength));
  }

  //If the tag is not a valid A++ identifier, then the program closes after outputting an error.
  if (!isIdentifier(newVar.tag))
  {
    cout << newVar.tag << " is an invalid A++ identifier." << endl;
    exit(1);
  }

  variables.push_back(newVar);
}

bool App::assignment(unsigned int i)
{
  string assign = lines[i], var;
  size_t end, start;
  int inputVal;
  /*assign is the line that the assignment occurs on. The line will be edited, so we need to copy it.
  var will hold the variable name from the assignment statement.
  end and start will be used to find the variable name and expression.
  inputVal is the value that is being assigned.*/

  //The loop removes all spaces from the statement.
  for (unsigned int i = 0; i < assign.size(); i++)
  {
    if (assign[i] == ' ')
    {
      assign.erase(i, 1);
      i--;
    }
  }

  //var is everything from the start of the string to the equality sign.
  end = assign.find('=');
  var = assign.substr(0, end);

  //the inputVal is everything between the equality and semicolon.
  start = assign.find('=') + 1;
  end = assign.find(';') - start;
  inputVal = expression(assign.substr(start, end));

  //The loop compares var to our current variables, and assigns inputVal to that variable if it exists.
  for (unsigned int j = 0; j < variables.size(); j++)
  {
    if (var == variables[j].tag)
    {
      variables[j].value = inputVal;
      return true;
    }
  }

  //If nothing matches var, there was a syntax error of an uninitialized variable.
  cout << "Error: No initialized variable of identifier " << var << " found." << endl;
  return false;
}

void App::print(unsigned int i)
{
  size_t start = lines[i].find("print ") + 6;
  size_t end = lines[i].find(';') - start;
  /*start is where the beginning of the expression to be printed is.
  end is the length from the start to the end of the expression*/
  cout << expression(lines[i].substr(start, end)) << endl;
}

string App::var2val(string var)
{
  //The loop searches the list of variables for a tag that matches the input string.
  for (unsigned int i = 0; i < variables.size(); i++)
  {
    //If a match is found, the value is converted to a string and returned.
    if (variables[i].tag == var)
      return int2str(variables[i].value);
  }

  cout << "Error: Initialized variable with identifier " << var << " not found." << endl;
  exit(1);
}

int App::expression(string exp)
{
  string* termHolder = new string;
  size_t place = 0, length = exp.length();
  string infixExp;
  /*termHolder is a pointer to a string, used to manage the current variable name in the expression.
  place helps nevigate the input expression.
  length is the length of the expression.
  infixExp is the final expression with the identifier replaced with their integer values.*/
  
  //Spaces are removed from the expression.
  for (unsigned int i = 0; i < exp.size(); i++)
  {
    if (exp[i] == ' ')
    {
      exp.erase(i, 1);
      i--;
      length--;
    }
  }
  
  for (; place < length; place++)
  {
    //If an operator is found, current term needs to be added to the expression.
    if (exp[place] == '+' || exp[place] == '-' || exp[place] == '/' || exp[place] == '*' || exp[place] == '%')
    {
      infixExp += var2val(*termHolder);
      infixExp += exp[place];
      //the term is added to infixExp, and then the current operator is as well.
      delete termHolder;
      termHolder = new string;
    }
    else
    {
      //If the character is anything other than an operator, it's a part of an identifier.
      *termHolder += exp[place];
    }
  }
  
  //Once the loop finishes, whatever termHolder holds is the final operand.
  infixExp += var2val(*termHolder);
  delete termHolder;
  //The '#' marks the end of the expression.
  infixExp += '#';

  //The infix expression is converted to postfix, evaluated in postfix, and that value is returned.
  return evalPostfix(infix2postfix(infixExp));
}

string App::infix2postfix(const string &infix)
{
  stack<char> opStack;
  string postfixExp;
  /*opStack is the stack of operators.
  postfixExp is the expression in postfix form that will be returned.*/

  //'#' is pushed onto the stack so we know when to stop.
  opStack.push('#');

  for (size_t i = 0; i < infix.size(); i++)
  {
    //if a digit is encountered and there is a digit after it, or just 'n' is encountered (Due to our choice of negative representation in an expression), then it's added to the postfixExp.
    if ((infix[i] <= '9' && infix[i] >= '0') && (infix[i + 1] <= '9' && infix[i + 1] >= '0') || infix[i] == 'n')
      postfixExp += infix[i];
    //If a digit is encountered and the next character is not a digit, the digit is appended with a space after it.
    else if ((infix[i] <= '9' && infix[i] >= '0') && (infix[i + 1] > '9' || infix[i + 1] < '0'))
    {
      postfixExp += infix[i];
      postfixExp += ' ';
    }
    //If the end of the infix expression is reached, all operators are popped and appended.
    else if (infix[i] == '#')
    {
      while (!opStack.empty())
      {
        postfixExp += opStack.top();
        postfixExp += ' ';
        opStack.pop();
      }
    }
    //If a + or - is found, all operators in the stack must be popped off to satisfy order of operations.
    else if (infix[i] == '+' || infix[i] == '-')
    {
      while (opStack.top() != '#')
      {
        postfixExp += opStack.top();
        postfixExp += ' ';
        opStack.pop();
      };
      opStack.push(infix[i]);
    }
    //If the other operators are found, just operators of equal priority are popped off and added to the postfixExp.
    else if (infix[i] == '/' || infix[i] == '*' || infix[i] == '%')
    {
      while (opStack.top() == '/' || opStack.top() == '*' || opStack.top() == '%')
      {
        postfixExp += opStack.top();
        postfixExp += ' ';
        opStack.pop();
      };
      opStack.push(infix[i]);
    }
  }

  //After the loop, the postfixExp will be perfectly formatted for reading and evaluating.
  return postfixExp;
}

int App::evalPostfix(const string &postfix)
{
  stack<int> evalStk;
  string* numHolder = new string;
  int left, right;
  /*evalStk holds the integer values of the operands.
  numHolder is used to fetch each number from the expression.
  left and right manage which value is on the left and right when performing an operation.*/

  for (size_t i = 0; postfix[i] != '#'; i++)
  {
    //Similar to the converting from infix to postfix, the numbers are extracted
    if ((postfix[i] <= '9' && postfix[i] >= '0') || postfix[i] == 'n')
      *numHolder += postfix[i];
    //If a space is found after a number,t hat marks the end of that value.
    else if (postfix[i] == ' ' && (postfix[i-1] <= '9' && postfix[i-1] >= '0'))
    {
      evalStk.push(str2int(*numHolder));
      //the string of digits is converted to an integer, and pushed onto the stack.
      delete numHolder;
      numHolder = new string;
    }
    //If an operator is found, an evaluation must be done.
    else if (postfix[i] == '+' || postfix[i] == '-' || postfix[i] == '/' || postfix[i] == '*' || postfix[i] == '%')
    {
      //The top element is the right, and the second is te left.
      right = evalStk.top();
      evalStk.pop();
      left = evalStk.top();
      evalStk.pop();

      //Depending on the operator, an actual operation is done. That value is then pushed onto the stack.
      if (postfix[i] == '+')
        evalStk.push(left + right);
      else if (postfix[i] == '-')
        evalStk.push(left - right);
      else if (postfix[i] == '/')
        evalStk.push(left / right);
      else if (postfix[i] == '*')
        evalStk.push(left * right);
      else
        evalStk.push(left % right);
    }
  }

  //The final expression value is returned.
  return evalStk.top();
}

string App::int2str(int n)
{
  int places = 0, modNum = 1, holder = n, negPlace = 1;
  string finalNum;
  /*
  places is the number of places in the integer.
  modNum is the number that will be used to reduce the integer to each digit.
  holder is used to count the places in the number.
  negPlace is used to adjust for negatives.
  finalNum is the string that will be returned.
  */

  if (holder < 0)
    holder = -holder;

  //This loop counts the places.
  do
  {
    holder = holder / 10;
    places++;
  } while (holder >= 1);

  //The loop runs until modNum is of the same magnitude as int n.
  for (int i = 1; i < places; i++)
  {
    modNum = modNum * 10;
  }

  //Checks to see if n is negative and adjusts accordingly.
  if (n < 0)
  {
    //n is used in place of a negative sign, since the infix and postfix expressions look for negative signs.
    finalNum += 'n';
    places++;
    n = -n;
  }
  else
    negPlace = 0;


  for (int i = negPlace; i < places; i++)
  {
    //First, the current entry has the correct digit put into it, by finding the number and adding the character 0 to it.
    finalNum += (n / modNum) + '0';
    //Next, we take what's left over from after the current digit.
    n = n % modNum;
    //Magnitude is decreased for next iteration.
    modNum = modNum / 10;
  }
  
  return finalNum;
}

int App::str2int(string num)
{
  int length = num.length(), placeMultiplier = 1, finalInt = 0, loopAdj = 0;
  bool isNegative = false;
  /*length is the size of the string
  placeMultiplier is used to get the correct decimal place for each digit.
  finalInt is what will be returned.
  loopAdj and isNegative are adjustments for negative numbers.*/

  //The following loop removes all spaces from the string.
  for (unsigned int i = 0; i < num.size(); i++)
  {
    if (num[i] == ' ')
    {
      num.erase(i, 1);
      i--;
      length--;
    }
  }
  
  //In this function, the string could start with an n or -, they both mean negative.
  if (num[0] == 'n' || num[0] == '-')
  {
    isNegative = true;
    loopAdj++;
  }

  //The loop builds the integer. It starts at the right right, in the ones column, then runs until the first digit.
  for (int i = length - 1; i >= loopAdj; i--)
  {
    //The character is taken, turned into an equivalent integer, multiplied by its place, then added to the finalInt.
    finalInt += placeMultiplier * (num[i] - '0');
    //The place is moved to the left.
    placeMultiplier = placeMultiplier * 10;
  }
  
  if (isNegative)
    return -finalInt;
  else
    return finalInt;

}

bool App::isIdentifier(string a)
{
  int charCount = a.length();
  //charCount will count the number of entries in the array.

  if ((a[0] >= 'A' && a[0] <= 'Z') || (a[0] >= 'a' && a[0] <= 'z') || (a[0] == '_'))
  {
    for (int i = 1; i < charCount; i++)
    {
      if ((a[i] >= 'A' && a[i] <= 'Z') || (a[i] >= 'a' && a[i] <= 'z') || (a[i] == '_') || (a[i] >= '0' && a[i] <= '9'))
        {}
      else
        return false;
    }
  }
  else
    return false;

  return true;
  /*
  The first character in the string is tested to be a letter or an underscore, since that is the qualification of an identifier.
  If the first character is not a letter or underscore, false is returned. If it is, each individual character is tested to be a letter, underscore, or number.
  If there is a char besides a letter, number, or underscore, false is returned. If each character is valid for an identifier, true is returned.
  Style is not accounted for, just the strict definition of what can be an identifier.
  */
}