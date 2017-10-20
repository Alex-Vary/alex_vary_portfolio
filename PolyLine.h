#ifndef PolyLine_h
#define PolyLine_h
#include <list>
using namespace std;

//Structure for a Point, which has an x and y value.
struct Point
{
  double x, y;
};

class PolyLine
{
public:
  /*PURPOSE: Initialize a PolyLine object with data from inp.txt
  PRE: A text file to read called inp.txt (In XML format for points)
  POST: An initialized PolyLine object.*/
  PolyLine();

  /*PURPOSE: Insert a point after a given coordinate pair.
  PRE: A Point object, and an x and y value.
  POST: point is inserted in the PolyLine object.*/
  void insert(Point point, double, double);

  /*PURPOSE: Insert point at end of list.
  PRE: A Point object, and an x and y value.
  POST: point is inserted in the PolyLine object.*/
  void insert(Point point);

  /*PURPOSE: Remove a point from the PolyLine object based on position.
  PRE: Int > 0, but less than numPoints. List should have at least one element.
  POST: Point at that position is removed.*/
  void remove(int);

  /*PURPOSE: Returns full length of PolyLine object (Distance between each consecutive pair of points)
  PRE: A PolyLine object with at least 2 elements.
  POST: Length of the distance between points is returned.*/
  double length();

  /*PURPOSE: Returns the number of points in the PolyLine.
  PRE: Initialized PolyLine object.
  POST: numPoints is returned.*/
  int size() const;

  /*PURPOSE: Prints current list of points.
  PRE: Initialized PolyLine object.
  POST: Points are printed.*/
  void show();

  /*PURPOSE: Stores the current PolyLine is out.txt in XML format.
  PRE: Initialize PolyLine object with at least 1 element.
  POST: out.txt has the PolyLine written to it.*/
  void store();

private:
  list<Point> polyLine;
  int numPoints;

  /*PURPOSE: Convert inp.txt to a workable string.
  PRE: A string to be filled with inp.txt
  POST: The inputted string is filled and returned through reference.*/
  void readFile(string&);

  /*PURPOSE: Read the input string and make all capital letters lowercase.
  PRE: A string of XML poitns from inp.txt
  POST: The string will now be workable for str2polyLine*/
  void normalize(string&);

  /*PURPOSE: Reads the string and converts the XML data into a PolyLine.
  PRE: A string of XML point tags with x and y values
  POST: A PolyLine will be populated with XML points.*/
  void str2polyLine(const string&);

  /*PURPOSE: Convert a string to a double.
  PRE: String containing digits, minus signs, plus signs, decimal points, and whitespaces.
  POST: The number in the string will be returned.*/
  double str2double(string);
};

/*PURPOSE: Calculate the distance between two Points.
PRE: Two initialized points.
POST: The distance between the points is returned.*/
double dist(Point, Point);

/*PURPOSE: Provides an interface for the PolyLine program.
PRE: None.
POST: Executes the program until the user ends it.*/
void menu();

/*PURPOSE: Lists commands for menu function.
PRE: None.
POST: Lists all 8 commands.*/
void commandList();
#endif
