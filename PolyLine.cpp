#include "PolyLine.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <cmath>
#include <iomanip>
#include <string>
using namespace std;

PolyLine::PolyLine()
{
  string inpFile;
  //inpFile is the string that will hold all of the text from inp.txt.

  readFile(inpFile);
  //The function reads the file and stores it in inpFile.

  normalize(inpFile);
  //The function changes all capital P's, X's, and Y's from the file to lowercase, to allow the following function to work.

  str2polyLine(inpFile);
  //Reads inpFile and constructs the polyLine using that data.
}

void PolyLine::insert(Point point, double x1, double y1)
{
  list<Point>::iterator it = polyLine.begin();
  int i;
  //An iterator for PolyLine is made, starting at the first element.
  //i is used to position the iterator so the point is inserted in the correct spot.
  
  for (i = 1; i <= numPoints && (it->x != x1 || it->y != y1); i++, it++) {}
  //The loop increments the iterator and i until a point matching the input coordinates is found, or until the end of the list is reached.

  if (i < numPoints)
    it++;
  //If a point matching the input coordinates is found, the next point will be input after those found coordinates.

  polyLine.insert(it, point);
  numPoints++;
  //The new point is inserted in the correct spot, and another point is added to the list.
}

void PolyLine::insert(Point point)
{
  list<Point>::iterator it = polyLine.end();
  //Iterator pointing to the last element of the point list is made.

  polyLine.insert(it, point);
  numPoints++;
  //The point is inserted at the end of the list, and the number of points increases.
}

void PolyLine::remove(int n)
{
  if (n > numPoints || n < 1)
    return;
  
  list<Point>::iterator it = polyLine.begin();
  //Iterator pointing to the beginning is created.

  if (polyLine.empty())
  {
    cout << "Polyline is empty!\n";
    return;
  }
  //If there are no elements in the list, the function ends.
  
  for (int i = 1; i < n; it++, i++) {}
  //The iterator is placed at the position of the point to be deleted.

  polyLine.erase(it);
  numPoints--;
  //The point is erased, and the number of points decreases.
}

double PolyLine::length()
{
  if (numPoints < 2)
    return 0;
  //If there is less than 2 points, the function will not work.

  list<Point>::iterator it1 = polyLine.begin(), it2 = polyLine.begin();
  it2++;
  double sum = 0;
  //Two iterators are created, with it2 pointing to the element following it1's element.
  //sum will be the full length of the PolyLine.

  for (int i = 1; i < numPoints; i++, it1++, it2++)
    sum += dist(*it1, *it2);
  //The function runs numPoints-1 times, getting the distance between a point and the point that follows it, until the end is reached.

  return sum;
}

int PolyLine::size() const
{
  return numPoints;
  //The number of points is returned.
}

void PolyLine::show()
{
  if (numPoints == 0)
  {
    cout << "The PolyLine is empty!\n";
    return;
  }
  
  list<Point>::iterator it = polyLine.begin();
  //An iterator pointing to the first element is created.

  for (int i = 1; it != polyLine.end(); it++, i++)
    cout << "Point " << i << ") [x = " << setw(6) << it->x << ", y = " << setw(6) << it->y << "]\n";
  //The loop outputs each point's x and y value, as well as its position in the list.
}

void PolyLine::store()
{
  ofstream outFile("out.txt");
  list<Point>::iterator it = polyLine.begin();
  //A file to store the points is opened, and and iterator to the first element is made.

  for (int i = 1; i <= numPoints; i++, it++)
  {
    outFile << "<Point>\n\t<x> " << fixed << it->x << " </x>\n\t<y> " << it->y << " </y>\n</Point>\n";
  }
  //The loops places each point in XML form into the document.

  outFile.close();
}

void PolyLine::readFile(string& inpFile)
{
  ifstream inpF;
  inpF.open("inp.txt", ios::out);
  string holder;
  //The input file is opened, and a string to hold each line is created.

  if (!inpF)
  {
    cout << "Error reading file." << endl;
    exit(1);
  }
  //If the file does not open properly, the program terminates.

  while (!inpF.eof())
  {
    getline(inpF, holder);
    inpFile += holder;
  };
  //Each line is stored in inpFile, which will be used in other fucntions.

  inpF.close();
}

void PolyLine::normalize(string& inpFile)
{
  int length = inpFile.length();
  //The length of the input string is found.

  for (int i = 0; i < length; i++)
    if (inpFile[i] == 'P')
      inpFile[i] = 'p';
    else if (inpFile[i] == 'X')
      inpFile[i] = 'x';
    else if (inpFile[i] == 'Y')
      inpFile[i] = 'y';
  //The loop runs through the whole string, and changes all uppercase P's, Y's, and X's to lowercase, so the other functions can run on the string.
}

void PolyLine::str2polyLine(const string& inpFile)
{
  size_t start, end;
  bool pFound = false, xFound = false, yFound = false, loopRun = true;
  Point point;
  /*start and end are the values of the start and end of the current part of the string we're evaluating.
  xFound and yFound are used to find out if an x and y value has been pulled from the string.
  loopRun is used to run and terinate the list once no more points can be found.
  point holds the data for the current point to be inserted in the PolyLine.*/

  start = inpFile.find("<point>");
  //The start is placed where the first <point> tag is found.

  while (loopRun)
  {
    //If a <point> tag is found, pFound becomes true so the loop knows it's time to find x and y. The start also changes to after the <point> tag.
    //If no <point> tag is found, that means there are no valid points left to add, so the loop will stop.
    if (inpFile.find("<point>", start) != string::npos)
    {
      pFound = true;
      start = inpFile.find("<point>", start) + 7; //7 is length of "<point>"
    }
    else
    {
      loopRun = false;
      continue;
    }

    //The <x> is looked for.
    if (inpFile.find("<x>", start) != string::npos && pFound)
    {
      //The start is right after the <x> tag.
      start = inpFile.find("<x>", start) + 3; //3 is length of "<x>"
      //The end is right before the </x> tag.
      end = inpFile.find("</x>", start) - 1;
      //The substring between the tags is converted to a double value.
      point.x = str2double(inpFile.substr(start, end - start));
      //xFound is marked as true.
      xFound = true;
      //The new start is right after the </x> tag.
      start = end + 4; //4 is length of "</x>"
    }
    else
    {
      cout << "Error: An x component for point " << numPoints + 1 << " not found.\n";
      exit(1);
    }

    //The <y> is looked for.
    if (inpFile.find("<y>", start) != string::npos && pFound)
    {
      //This block is identical to the <x> block above, except for y.
      start = inpFile.find("<y>", start) + 3; //3 is length of "<y>"
      end = inpFile.find("</y>", start) - 1;
      point.y = str2double(inpFile.substr(start, end - start));
      yFound = true;
      start = end + 4;
    }
    else
    {
      cout << "Error: A y component for point " << numPoints + 1 << " not found.\n";
      exit(1);
    }

    if (xFound && yFound)
    {
      //Once x and y are found, their point is put into the list, and the cycle restarts.
      pFound = false;
      xFound = false;
      yFound = false;
      insert(point);
    }
  };
}

double PolyLine::str2double(string s)
{
  int sign = 1, place = 1;
  double numberLeft = 0.0, numberRight = 0.0, decPlace = .1;
  size_t decFound, length = s.length(), decLength = 0;
  /*sign is used to keep the negative or positive sign for the number.
  place is used to compile the value left of the decimal.
  numberLeft and numberRight are the values to the right and to the left of the decimal.
  decPlace is used to compile the value right of the decimal.
  decFound is used to hold the place in the string where the decimal appears, if any.
  length is the length of the string to the left of the decimal.
  decLength is the length of the string right of the decimal.*/


  if (s.find('-') != string::npos)
    sign = -1;
  //Changes sign to -1 if the number in the string is negative.
  
  for (unsigned int i = 0; i < length; i++)
    if (s[i] == ' ' || s[i] == '-' || s[i] == '+')
    {
      s.erase(i, 1);
      i--;
      length--;
    }
  //The loop removes all whitespaces, minus signs, and plus signs from the string.

  decFound = s.find('.');

  if (decFound != string::npos)
  {
    length = s.length() - (s.substr(decFound)).length();
    decLength = (s.substr(decFound)).length() - 1;
  }
  //If there is a decimal, the length is changed to be the length of the string up until the decimal.
  //decLength is changed to the length of everything after the decimal, not including the decimal.

  for (int i = length - 1; i >= 0; i--)
  {
    numberLeft += place * (s[i] - '0');
    place = place * 10;
  }
  //The first loop changes the string to the left of the decimal to a double value.

  for (unsigned int i = 0; i < decLength ; i++)
  {
    numberRight += decPlace * (s[length + 1 + i] - '0');
    decPlace = decPlace / 10;
  }
  //The second loop converts the decimal part of the string to a decimal double value.

  return (numberLeft + numberRight) * sign;
  //The values from the left and right are added, and is multiplied by the sign.
}

double dist(Point p1, Point p2)
{
  return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
  //The distance formula (sqrt((x2 - x1)^2 + (y2 - y1)^2)) is used on the values of each input point.
}

void menu()
{
  string input;
  PolyLine* line;
  line = new PolyLine;
  Point point;
  double x, y, xIn, yIn;
  int removeNum;
  char insertAfter;
  /*input is used to read the user's command.
  line is a pointer of type PolyLine, used to create and delete a Polyline.
  point is the pointto be inserted by the user.
  x and y are the user's wanted value for the inserted point.
  xIn and yIn are the values for the point that the user want to put their point after.
  removeNum is the point number the user wants to remove.
  insertAfter is used to see if the user wants to put the point after an existing point, or at the end of the PolyLine.*/

  cout << "Welcome to the Polyline program! There is a list of commands, which must be typed in all capital letters. They are:\n";
  commandList();
  //Introduces the program and gives a list of commands.

  cout << "Initial Polyline:\n";
  line->show();
  //Prints the initial polyline, read from file.

  //The loop runs until the user types "EXIT".
  while (1)
  {
    cin >> input;
    //Reads the user's current command.
    if (input == "CREATE")
    {
      //The current PolyLine is completely erased, and reset to the one from file.
      delete line;
      line = new PolyLine;
      cout << "New Polyline created from file!\n";
    }
    else if (input == "LENGTH")
      cout << "The current length is: " << line->length() << endl;
    else if (input == "INSERT")
    {
      cout << "Input x coordinate: ";
      cin >> x;
      cout << "\nInput y coordinate: ";
      cin >> y;
      point.x = x;
      point.y = y;
      //User's values for the point to be inserted are gotten.
      cout << "Would you like to place the point after an existing point? Y/N" << endl;
      cin >> insertAfter;
      //Gives user the choice of putting point after an existing one, or just putting it at the end.
      if (insertAfter == 'Y')
      {
        cout << "Input the x coordinate of the pre-existing point: ";
        cin >> xIn;
        cout << "\nInput the y coordinate of the pre-existing point: ";
        cin >> yIn;
        //User inputs coordinates of existing point.
        line->insert(point, xIn, yIn);
      }
      else if (insertAfter == 'N')
        line->insert(point);
      else
      {
        //If the user puts any invalid entry in, which would cause problems, the current input buffer is cleaned and the loop starts a new iteration.
        cout << "Invalid input, point not inserted.\n";
        cin.clear();
        break;
      }
      cout << "Point successfully inserted!\n";
    }
    else if (input == "REMOVE")
    {
      cout << "Insert the number of the point you would like to remove.\n";
      cin >> removeNum;
      //User inputs position for the point they want to delete, starting from 1 at the first element.
      if (removeNum <= line->size() && removeNum > 0)
      {
        line->remove(removeNum);
        cout << "Point " << removeNum << " removed!\n";
      }
      else
        cout << "Invalid point position.\n";
    }
    else if (input == "SHOW")
      line->show();
    else if (input == "SIZE")
      cout << "Polyline has " << line->size() << " points.\n";
    else if (input == "EXIT")
    {
      //When the user is done, the program stores the current PolyLine in out.txt, and returns to main function.
      line->store();
      cout << "Current Polyline stored in out.txt" << endl;
      return;
    }
    else if (input == "HELP")
      commandList();
    //If the user forgets the commands, they can have them pritned again.
    else
      cout << "Invalid command.\n";

  };
}

void commandList()
{
  //List of commands to operate the menu function.
  cout << "1) CREATE - Changes current Polyline to the Polyline on file.\n";
  cout << "2) LENGTH - Calculates length of the Polyline.\n";
  cout << "3) INSERT - Adds a new point to the Polyline, either after a chosen point or at the end.\n";
  cout << "4) REMOVE - Removes a point based on position in the Polyline.\n";
  cout << "5) SHOW - Prints out current points in the Polyline.\n";
  cout << "6) SIZE - Shows the current number of points in the Polyline.\n";
  cout << "7) EXIT - Ends the program.\n";
  cout << "8) HELP - Shows command list again.\n\n";
}