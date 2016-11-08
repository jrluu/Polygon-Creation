#include <GL/glut.h>
#include "polygon.h"
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

float *PixelBuffer;  // it's global.
void display();
void clearBuffer();
void readFile(char *tmp);
void userInterfaceMenu();
void combinePolygons();

void swapVertex(int* vertexA, int* vertexB);
void DDALine(int* vertex1, int* vertex2);
float findSlope(int* vertxA, int* vertxB);
void colorPixel(int xCoor, int yCoor, float R, float G, float B);



void CSClipping(int xMin, int yMin, int xMax, int yMax);
void checkABLR(int * vertex, int * vertexABLR, int xMin, int yMin, int xMax, int yMax);

void findNewVertex(int * vertex, int * vertexABLR, int xMin, int yMin, int xMax, int yMax, float m, float b);
void Bresline(int *vertexx1, int *vertexx2);
void swapPoints(int *vertex);

int numberOfPolygons = 0;
vector<polygon*> polygonVec;
int xWindowSize = 400;
int yWindowSize = 400;
int xMax = xWindowSize - 1;
int yMax = yWindowSize - 1;
bool combineBool = 1;
void renderScene(void);

int main(int argc, char *argv[])
{
	//allocate new pixel buffer, need initialization!!
	cout << "x Window Size: ";
	cin >> xWindowSize;
	cout << "y Window Size: ";
	cin >> yWindowSize;

	PixelBuffer = new float[xWindowSize * yWindowSize * 3];

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(xWindowSize, yWindowSize);
	glutInitWindowPosition(1200, 100);

	//create and set main window title
	int MainWindow = glutCreateWindow("Hello Graphics!!");

	char* tmp = new char[100];
	cout << "Enter file to load : ";
	cin >> tmp;
	readFile(tmp);
	
	userInterfaceMenu();	

	delete tmp; // may have a delete function

	glutDisplayFunc(display);   //sets display function

	glutMainLoop();//main display loop, will display until terminate
	return 0;
}

/*
void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	glVertex3f(-0.5, -0.5, 0.0);
	glVertex3f(0.5, 0.0, 0.0);
	glVertex3f(0.0, 0.5, 0.0);
	glEnd();
	glutSwapBuffers();
}*/
//main display loop, this function will be called again and again by OpenGL
void display()
{
	//Misc.
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	if (combineBool == 1) {
		clearBuffer();
		combinePolygons();
	}
//	cout << "SHOULD BE DRAWING " << endl;

	//draws pixel on screen, width and height must match pixel buffer dimension
	glDrawPixels(xWindowSize, yWindowSize, GL_RGB, GL_FLOAT, PixelBuffer);

	//window refresh
	glEnd();
	glFlush();
	userInterfaceMenu();
}


void clearBuffer() {
	for (int i = 0; i < (xWindowSize * yWindowSize * 3); i++)
		PixelBuffer[i] = 0.0;
}

void readFile(char *tmp) {

	string currLine;
	ifstream inputFile(tmp, istream::in);

	getline(inputFile, currLine);
	stringstream ss(currLine);

	ss >> numberOfPolygons;
	cout << "Number of Polygons Loaded: " << numberOfPolygons << endl;
	cout << endl;

	for (int i = 0; i < numberOfPolygons; i++) {
		polygonVec.push_back(new polygon(i, xWindowSize, yWindowSize));

		getline(inputFile, currLine);
		stringstream ss2(currLine);
		int numOfVertices;
		ss2 >> numOfVertices;
//		cout << "Num of Vertices " << numOfVertices << endl;

//		cout << "Number of Vertices : " << numOfVertices << endl;
		if (numOfVertices > 0) {
			for (int verticeNum = 0; verticeNum < numOfVertices; verticeNum++) {
				getline(inputFile, currLine);
				stringstream ss3(currLine);
				int x, y;
				ss3 >> x;
				ss3 >> y;
				//			cout << "(" << x << "," << y << ")" << endl;
				polygonVec[i]->addPoints(x, y);
			}
			//		cout << "passed array" << endl;
			if (numOfVertices > 2) {
				polygonVec[i]->findExtrema();
				polygonVec[i]->drawPolygon();
				//		cout << "passed drawPolygon" << endl;
				polygonVec[i]->scanline();
			}
			else if (numOfVertices == 1) {
				//that's like not even a fucking line or polygon.
			}
			else {
				polygonVec[i]->drawPolygon(); //probably could use bres
			}
		}
		else if (numOfVertices == 0) {
			polygonVec.pop_back();
			numberOfPolygons--;
			i--;
		}
			
//		cout << "passed scanLine" << endl;
		
		
	}

}

void userInterfaceMenu()
{
	polygon* tempPoly;
	int userInput = 0;
	int initialPolyNum = numberOfPolygons;
	int polyNumSelect;
	ofstream myfile;
	myfile.open("saveFile.txt");
	int xMin;
	int xMax;
	int yMin;
	int yMax;


//	do{
		cout << "Menu" << endl;
		cout << "0: Refresh." << endl;
		cout << "1: Line Drawing Using DDA." << endl;
		cout << "2: Line Drawing Using Bresenham." << endl;
		cout << "3: Create Polygons." << endl;
		cout << "4: Translation" << endl;
		cout << "5: Rotation" << endl;
		cout << "6: Scaling" << endl;
		cout << "7: Clip a line" << endl;
		cout << "8: Clip an area" << endl;
		cout << "9: Save Polygons" << endl;
		cout << "Please enter your choice number: ";
		cin >> userInput;
		cout << endl;
		switch (userInput)
		{

		case 0:
			break;
		case 1:
			int vertex1[2], vertex2[2];
			cout << "Please enter two integer values:";
			cin >> vertex1[0];
			cin >> vertex1[1];
			cout << "Please enter another two integer values:";
			cin >> vertex2[0];
			cin >> vertex2[1];
			polygonVec.push_back(new polygon(numberOfPolygons, xWindowSize, yWindowSize));
			polygonVec[numberOfPolygons]->addPoints(vertex1[0] , vertex1[1]);
			polygonVec[numberOfPolygons]->addPoints(vertex2[0], vertex2[1]);
			polygonVec[numberOfPolygons]->drawPolygon();
			numberOfPolygons++;
//			userInput = 0;
			break;
		case 2:
			float x1, y1, x2, y2;
			int vertexA[2];
			int vertexB[2];
			cout << "Please enter two integer values:";
			cin >> x1;
			cin >> y1;
			cout << "Please enter another two integer values:";
			cin >> x2;
			cin >> y2;
			vertexA[0] = x1;
			vertexA[1] = y1;
			vertexB[0] = x2;
			vertexB[1] = y2;
			cout << "Vertex 1 (" << x1 << " " << y1 << ")" << endl;
			cout << "Vertex 2 (" << x2 << " " << y2 << ")" << endl;
			Bresline(vertexA, vertexB);

			break;

		case 3:
			int morePolyNum;
			cout << "Please enter number of polygons:";
			cin >> morePolyNum;
			for (int i = initialPolyNum; i < (initialPolyNum + morePolyNum); i++) {
				polygonVec.push_back(new polygon(i, xWindowSize, yWindowSize)); 
				polygonVec[i]->storeData();
				polygonVec[i]->drawPolygon();
				if (polygonVec[i]->numOfVertices > 2) {
					polygonVec[i]->scanline();
				}

				numberOfPolygons++;
			}
			/*
			cout << "First Polygon Num" << polygonVec[0]->polygonNum << endl;
			cout << "Last polygon Num" << polygonVec[numberOfPolygons - 1]->polygonNum << endl;
			cout << "Number of Polygons:" << numberOfPolygons << endl;
			*/
			break;
		case 4:
			int t_x;
			int t_y;
			cout << "There are currently " << numberOfPolygons << " polygons" << endl;
			cout << "Enter a polygon to translate: ";
			cin >> polyNumSelect;
			cout << "Translate x by: ";
			cin >> t_x;
			cout << "Translate y by: ";
			cin >> t_y;
			cout << "Translating by Vector: " << "(" << t_x << "," << t_y << ")" << endl;
			polygonVec[polyNumSelect]->translate(t_x, t_y);
//			glutPostRedisplay();
			break;
		case 5:
			int theta;
			cout << "There are currently " << numberOfPolygons << " polygons" << endl;
			cout << "Enter a polygon to rotate: ";
			cin >> polyNumSelect;
			cout << "Rotate polygon by: ";
			cin >> theta;
			polygonVec[polyNumSelect]->rotate(theta);
			break;

		case 6:
			int xFactor;
			int yFactor;
			cout << "There are currently " << numberOfPolygons << " polygons" << endl;
			cout << "Enter a polygon to Scale: ";
			cin >> polyNumSelect;
			cout << "Scale x by: ";
			cin >> xFactor;
			cout << "Scale y by: ";
			cin >> yFactor;
			cout << "Scaling x by " << xFactor << " and y by " << yFactor << endl;
			polygonVec[polyNumSelect]->scale(xFactor, yFactor);
			break;
		case 7:
			clearBuffer();
			cout << "Clipping a line" << endl;
			cout << "Enter xMin by: ";
			cin >> xMin;
			cout << "Enter yMin by: ";
			cin >> yMin;
			cout << "Enter xMax by: ";
			cin >> xMax;
			cout << "Enter yMax by: ";
			cin >> yMax;
			CSClipping(xMin, yMin, xMax, yMax);
			break;
		case 8:
			cout << "Clipping polygons" << endl;
			cout << "Enter xMin by: ";
			cin >> xMin;
			cout << "Enter yMin by: ";
			cin >> yMin;
			cout << "Enter xMax by: ";
			cin >> xMax;
			cout << "Enter yMax by: ";
			cin >> yMax;
			for (int i = 0; i < numberOfPolygons; i++) {
				cout << polygonVec[i]->numOfVertices << endl;
				if (polygonVec[i]->numOfVertices == 2) {
					polygonVec[i]->CSClipping(xMin, yMin, xMax, yMax);
//					cout << "CS clipping" << endl;
				}
				else {
					polygonVec[i]->wholeClip(xMin, yMin, xMax, yMax);
//					cout << "Whole clipping" << endl;
				}
				//cout << "Fails here" << i << endl;
			}
			glutPostRedisplay();
			break;
		case 9:
			cout << "Making save file..." << endl;
			myfile << numberOfPolygons << '\n';
			for (int i = 0; i < numberOfPolygons; i++) {
				int currPolyVerticeNum = polygonVec[i]->numOfVertices;
		//		cout << currPolyVerticeNum << endl;
				myfile << currPolyVerticeNum << '\n';
				for (int verticeIndex = 0; verticeIndex < currPolyVerticeNum; verticeIndex++) {
	//				cout << verticeIndex << endl;
					myfile << polygonVec[i]->vertXArray[verticeIndex] << ' ' << polygonVec[i]->vertYArray[verticeIndex];
					myfile << '\n';

				}
			}

			myfile.close();
			break;
		default:
			cout << userInput << " is a not a valid choice. Please enter a value from 0 - 9." << endl;
		}
		cout << endl;
		
		if (userInput == 2 || userInput == 7) {
			combineBool = 0;

		}
		else if (userInput < 9) {
			combineBool = 1;
	//		clearBuffer();
		//	combinePolygons();
			glutPostRedisplay();
	//		cout << "Yeah Boy" << endl;
		}
	//	cout << "Yes" << endl;
	
		initialPolyNum = numberOfPolygons;
//	} while (userInput != 0);

}//userInterfaceMenu

void combinePolygons() {
	for (int polyNum = 0; polyNum < numberOfPolygons; polyNum++) {
		for (int i = 0; i < (xWindowSize * yWindowSize * 3); i++) {
			if (PixelBuffer[i] || (polygonVec[polyNum]->PixelBuffer[i])) {
	//			cout << polygonVec[polyNum]->PixelBuffer[i];
	//			cout << PixelBuffer[i];
				PixelBuffer[i] = 1.0;
	//			if (i = 300)
	//				break;
			}
		}
	}
}

///DDA LINE

void DDALine(int* vertex1, int* vertex2) {

	colorPixel(vertex1[0], vertex1[1], 1.0, 1.0, 1.0);
	colorPixel(vertex2[0], vertex2[1], 1.0, 1.0, 1.0);

	if (vertex1[0] > vertex2[0])
		swapVertex(vertex1, vertex2);	//makes it go right to left

	float m = findSlope(vertex1, vertex2);
	
	if (m == 99999) {
		//Vertical Line
		if (vertex1[1] > vertex2[1])
			swapVertex(vertex1, vertex2);	//makes it from the bottom to the top

		int currY = vertex1[1];

		while (currY != vertex2[1]) {
			currY = currY + 1;

			colorPixel(vertex1[0], currY, 1.0, 1.0, 1.0);
		}
	}
	else if (abs(m) > 1) {
//		cout << "Steep Line" << endl;
		if (vertex1[1] > vertex2[1])
			swapVertex(vertex1, vertex2);	//makes it from the bottom to the top

		int currX = vertex1[0];
		int currY = vertex1[1];
		float tempX = vertex1[0];

		while (currY != vertex2[1]) {
			tempX = tempX + 1/m + .5;
			currX = (int)tempX;
			tempX = tempX - .5;
			currY = currY + 1;

			colorPixel(currX, currY, 1.0, 1.0, 1.0);
		}
	}
	else{ // m <= 1
//		cout << "Regular Line" << endl;
		int currX = vertex1[0];
		int currY = vertex1[1];
		float tempY = vertex1[1];

		while (currX != vertex2[0]) {
			tempY = tempY + m + .5;
			currY = (int)tempY;
			tempY = tempY - .5;
			currX = currX + 1;

			colorPixel(currX, currY, 1.0, 1.0, 1.0);
		}
	}
} //DDALine()


void swapVertex(int* vertexA, int* vertexB) {
/*
	cout << "Before swap" << endl;
	cout << "Vertex 1 : (" << vertexA[0] << "," << vertexA[1] << ")" << endl;
	cout << "Vertex 2 : (" << vertexB[0] << "," << vertexB[1] << ")" << endl;
*/
	float tempfloat[2] = { vertexA[0], vertexA[1] };

	vertexA[0] = vertexB[0];
	vertexA[1] = vertexB[1];
	vertexB[0] = tempfloat[0];
	vertexB[1] = tempfloat[1];

	/*
	cout << "After swap" << endl;
	cout << "Vertex 1 : (" << vertexA[0] << "," << vertexA[1] << ")" << endl;
	cout << "Vertex 2 : (" << vertexB[0] << "," << vertexB[1] << ")" << endl;
	*/
}

float findSlope(int* vertxA, int* vertxB) {

	float deltaX = float(vertxB[0] - vertxA[0]);
	float deltaY = float(vertxB[1] - vertxA[1]);

	if (deltaX == 0)
		return 99999;

	//cout << "Delta Y:" << deltaY << endl;
	//cout << "Delta X:" << deltaX << endl;

	float m = deltaY / deltaX;
	//cout << "Slope:" << m << endl;
	return m;

}

void colorPixel(int xCoor, int yCoor, float R, float G, float B) {

	PixelBuffer[(3 * xCoor) + (yCoor * yWindowSize * 3)] = R;
	PixelBuffer[(3 * xCoor) + (yCoor * yWindowSize * 3) + 1] = G;
	PixelBuffer[(3 * xCoor) + (yCoor * yWindowSize* 3) + 2] = B;

}


void CSClipping(int xMin, int yMin, int xMax, int yMax)
{

	int tempVertex1[2];
	int tempVertex2[2];
	int vertex1ABLR[4] = { 0, 0, 0, 0 };
	int vertex2ABLR[4] = { 0, 0, 0, 0 };
	int accept = 1;
	int v1Total = 0;  //Will count how many vertex 1;
	int v2Total = 0;  //Will count how many vertex 1;

	cout << "Insert first x:" << endl;
	cin >> tempVertex1[0];
	cout << "Insert first y:" << endl;
	cin >> tempVertex1[1];
	cout << "Insert second x:" << endl;
	cin >> tempVertex2[0];
	cout << "Insert second y:" << endl;
	cin >> tempVertex2[1];

	for (int i = 0; i < 4; i++) {
		vertex1ABLR[i] = 0;
		vertex2ABLR[i] = 0;
	} //resets ABLR

	  /*
	  for (int i = 0; i < (numOfVertices - 1); i++) {
	  tempVertex1[0] = vertXArray[i];
	  tempVertex1[1] = vertYArray[i];
	  //		cout << "finished tempVertex1" << endl;
	  tempVertex2[0] = vertXArray[i + 1];
	  tempVertex2[1] = vertYArray[i + 1];
	  //		cout << "passed tempVertex2" << endl;
	  }
	  */


	checkABLR(tempVertex1, vertex1ABLR, xMin, yMin, xMax, yMax);
	checkABLR(tempVertex2, vertex2ABLR, xMin, yMin, xMax, yMax);

	for (int i = 0; i < 4; i++) {
		v1Total = v1Total + vertex1ABLR[i];
		v2Total = v2Total + vertex2ABLR[i];
//		cout << "Vertex1 A:" << vertex1ABLR[i] << " Vertex2:" << vertex2ABLR[i] << endl;
		if (vertex1ABLR[i] == 1) {
			if (vertex2ABLR[i] == 1) {
//				cout << "Vertex1 A:" << vertex1ABLR[i] << " Vertex2:" << vertex2ABLR[i];
//				cout << "i is " << i << endl;
				accept = 0;
				break;
			}
		}// if both cases == 1 equal at the same time, reject
	}

	if (accept == 0) {
//		cout << "Rejected!" << endl;
	}
	else if (!(v1Total || v2Total)) {
		DDALine(tempVertex1, tempVertex2);
//		cout << "It works" << endl;
	}
	else {
		if (tempVertex1[0] > tempVertex2[0])
			swapVertex(tempVertex1, tempVertex2);	//makes it go right to left

		float m = findSlope(tempVertex1, tempVertex2); 
		if (m == 99999) {
			if (tempVertex1[1] < yMin) {
				tempVertex1[1] = yMin;
			}
			else if (tempVertex1[1] > yMax) {
				tempVertex1[1] = yMax;
			}

			if (tempVertex2[1] < yMin) {
				tempVertex2[1] = yMin;
			}
			else if (tempVertex2[1] > yMax) {
				tempVertex2[1] = yMax;
			}
			DDALine(tempVertex1, tempVertex2);

		}
		else {
			float b = tempVertex1[1] - (m*(tempVertex1[0])); //finding b

			if (v1Total != 0) {
//				cout << "First vertex is out of bounds" << endl;
				findNewVertex(tempVertex1, vertex1ABLR, xMin, yMin, xMax, yMax, m, b);

			}

			if (v2Total != 0) {
//				cout << "Second vertex is out of bounds" << endl;
				findNewVertex(tempVertex2, vertex2ABLR, xMin, yMin, xMax, yMax, m, b);
			}
			DDALine(tempVertex1, tempVertex2);
		}
	}
}

void checkABLR(int *vertex, int *vertexABLR, int xMin, int yMin, int xMax, int yMax)
{
	if (vertex[1] > yMax)
		vertexABLR[0] = 1; //Above

	if (vertex[1] < yMin)
		vertexABLR[1] = 1; //Below

	if (vertex[0] < xMin)
		vertexABLR[2] = 1; //Left

	if (vertex[0] > xMax)
		vertexABLR[3] = 1; //Right

}

void findNewVertex(int *vertex, int *vertexABLR, int xMin, int yMin, int xMax, int yMax, float m, float b) {
	int x;
	int y;

	if (vertexABLR[0] == 1) {
		y = yMax;
		x = (y - b) / m;

		if (x < xMin) {
			x = xMin;
			y = m*(x)+b;
		} // 1010 case
		if (x > xMax) {
			x = xMax;
			y = m*(x)+b;
		} //1001 case

		vertex[0] = x;
		vertex[1] = y;
	//	cout << "1th Case" << endl;
	//	cout << "Vertex1 : " << vertex[0] << " , " << vertex[1] << endl;

	}

	else if (vertexABLR[1] == 1) {
		y = yMin;
		x = (y - b) / m;

		if (x < xMin) {
			x = xMin;
			y = m*(x)+b;
		} // 0110 case
		if (x > xMax) {
			x = xMax;
			y = m*(x)+b;
		} //0101 case

		vertex[0] = x;
		vertex[1] = y;

	//	cout << "2th Case" << endl;
	//	cout << "Vertex1 : " << vertex[0] << " , " << vertex[1] << endl;
	}//below case
	else if (vertexABLR[2] == 1) {
		x = xMin;
		y = m*(x)+b;

		vertex[0] = x;
		vertex[1] = y;
	//	cout << "3th Case" << endl;
	//	cout << "Vertex1 : " << vertex[0] << " , " << vertex[1] << endl;
	}
	else if (vertexABLR[2] == 1) {
		x = xMax;
		y = m*(x)+b;

		vertex[0] = x;
		vertex[1] = y;
	//	cout << "4th Case" << endl;
	//	cout << "Vertex1 : " << vertex[0] << " , " << vertex[1] << endl;

	}

}

void Bresline(int *vertexx1, int *vertexx2){
	bool steep = 0;
	// Bresenham's line algorithm
	float m = findSlope(vertexx1, vertexx2);

//	cout << " coloring the first coordinates" << endl;
//	colorPixel(vertexx1[0], vertexx1[0], 1.0, 1.0, 1.0);

	if (abs(m) > 1) {
		steep = 1;
//		cout << "Steep slope!" << endl;
	}
	if (steep)
	{
//		cout << "Swapping points" << endl;
		swapPoints(vertexx1);
		swapPoints(vertexx2);
		
	}

	if (vertexx1[0] > vertexx2[0])
	{
//		cout << "swapping vertices " << endl;
		swapVertex(vertexx1, vertexx2);
	}

	float deltaX = vertexx2[0] - vertexx1[0];
	float deltaY = abs(vertexx2[1] - vertexx1[1]);

	float error = deltaX / 2.0f;
	const int yStep = (vertexx1[1] < vertexx2[1]) ? 1 : -1;
	int y = vertexx1[1];

	int xMax = vertexx2[0];

	for (int x = vertexx1[0]; x < xMax; x++)
	{
		if (steep)
		{
	//		cout << "Coloring " << x << ' ' << y << endl;
			colorPixel(y, x, 1.0, 1.0, 1.0);
		}
		else
		{	
	//		cout << "Coloring " << x << ' ' << y << endl;
			colorPixel(x, y, 1.0, 1.0, 1.0);
		}

		error -= deltaY;
		if (error < 0)
		{
			y += yStep;
			error += deltaX;
		}
	}
}

void swapPoints(int *vertex) {
	int tempInt = vertex[0];

	vertex[0] = vertex[1];
	vertex[1] = tempInt;
}