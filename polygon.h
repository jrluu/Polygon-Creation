#ifndef polygon_h
  #define polygon_h

#include <vector>
#include <math.h>
using namespace std;
//bool SUCCESS = 1;
//bool FAILURE = 0;


void clearBuffer();

//bool checkExtrema(int x, int y);
void storeData();
void findExtrema();
void drawPolygon();
void scanline();

//Extra functions;
void addPoints(int x, int  y);

//For scanLine
float getPixelColor(int x, int y);
int checkExtrema(int x, int y);
void flipColor(float *currColor);

//2d Transformations
void translate(int x, int y);
void rotate(float theta);
void scale(float xFactor, float yFactor);

int mean(vector<int> arr, int arrSize);

//Borrowed from DDAline
void DDALine(int* vertex1, int* vertex2);
void swapVertex(int* vertexA, int* vertexB);
float findSlope(int* vertxA, int* vertxB);
void colorPixel(int xCoor, int yCoor, float R, float G, float B);

//Cohen-Sutherhand Clipping
/*
void CSClipping(int xMin, int yMin, int xMax, int yMax);
void checkABLR(int * vertex, int * vertexABLR, int xMin, int yMin, int xMax, int yMax);
*/

class polygon
{
public:
	int polygonNum;
	float* PixelBuffer;

	//Set in functions(when asked for vertiecs or translating) /// May take out if does not need optimizing
	int maxXSize;
	int maxYSize;
	int minXSize;
	int minYSize;

	//Set in function(storeData())
	int numOfVertices;
	vector<int> vertXArray;
	vector<int> vertYArray;

	//Set in function(storedata())
	vector<int> extremaXArr;
	vector<int> extremaYArr;
	int numOfExtrema;


	int xWindowSize;
	int yWindowSize;
	polygon(int polygonNumm, int xWindowSizee, int yWindowSizee); //Constructor
	void clearBuffer();



	void storeData();
	void findExtrema();
	void drawPolygon();
	void scanline();

	void addPoints(int x, int  y);

	//For scanLine
	float getPixelColor(int x, int y);
	int checkExtrema(int x, int y);
	void flipColor(float *currColor);

	//2d Transformations
	void translate(int x, int y);

	void rotate(float theta);

	void scale(float xFactor, float yFactor);

	int mean(vector<int> arr, int arrSize);



//	bool placeExtrema(int *vertex, int *xArray, int *yArray, int maxIndex);

	//DDA Line ----------------------------------------------------------
	void DDALine(int* vertex1, int* vertex2);
	void swapVertex(int* vertexA, int* vertexB);
	float findSlope(int* vertxA, int* vertxB);
//	void Bresline(int * vertex1, int * vertex2);
	//void swapPoints(int * point1, int * point2);
	void colorPixel(int xCoor, int yCoor, float R, float G, float B);
	
	//CLIPPING!
	void wholeClip(int xMin, int yMin, int xMax, int yMax);
	void leftClip(int xMin);
	void rightClip(int xMax);
	void botClip(int yMin);
	void topClip(int yMax);

	//CS Clipping
	void CSClipping(int xMin, int yMin, int xMax, int yMax);

	void checkABLR(int * vertex, int * vertexABLR, int xMin, int yMin, int xMax, int yMax);

	void findNewVertex(int * vertex, int * vertexABLR, int xMin, int yMin, int xMax, int yMax, float m, float b);

};
#endif