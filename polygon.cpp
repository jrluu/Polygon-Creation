#include <iostream>
#include <cmath>
#include "polygon.h"

polygon::polygon(int polygonNumm, int xWindowSizee, int yWindowSizee) {
	polygonNum = polygonNumm;
	xWindowSize = xWindowSizee;
	yWindowSize = yWindowSizee;
	PixelBuffer = new float[xWindowSize * yWindowSize * 3];
	clearBuffer();

	numOfVertices = 0;
	numOfExtrema = 0;
	maxXSize = xWindowSizee;
	maxYSize = yWindowSizee;
	minXSize = xWindowSizee;  //Will replace with something smaller
	minYSize = yWindowSizee;  //Will replace with something smaller

							  //	cout << "PolyNum: " << polygonNum << endl << "yWindowSize :" << yWindowSize << endl;
}

void polygon::clearBuffer() {
	for (int i = 0; i < (xWindowSize * yWindowSize * 3); i++)
		PixelBuffer[i] = 0.0;
}


void polygon::storeData() {

	cout << "Insert number of vertices for polygon " << polygonNum << ":";
	cin >> numOfVertices;
	int tempValue;

	for (int i = 0; i < numOfVertices; i++) {
		cout << "Please enter point " << i << "'s x value : ";
		cin >> tempValue;
		vertXArray.push_back(tempValue);
		cout << "Please enter point " << i << "'s y value : ";
		cin >> tempValue;
		vertYArray.push_back(tempValue);
		cout << "(" << vertXArray[i] << "," << vertYArray[i] << ")" << endl;
	}

	if (numOfVertices > 0) {
		findExtrema();
	}
}

void polygon::addPoints(int x, int  y) {
	vertXArray.push_back(x);
	vertYArray.push_back(y);
	numOfVertices++;
}

void polygon::findExtrema()
{
	if (vertYArray[0] > vertYArray[numOfVertices - 1]) {
		if (vertYArray[0] > vertYArray[1]) {
			extremaXArr.push_back(vertXArray[0]);
			extremaYArr.push_back(vertYArray[0]);
			//			cout << "array[0] = max " << endl;
		}
	}
	else if (vertYArray[0] < vertYArray[numOfVertices - 1]) {
		if (vertYArray[0] < vertYArray[1]) {
			extremaXArr.push_back(vertXArray[0]);
			extremaYArr.push_back(vertYArray[0]);
			//			cout << "array[0] = min " << endl;
		}
	}

	for (int i = 1; i < (numOfVertices - 1); i++) {
		if (vertYArray[i] > vertYArray[i - 1]) {
			if (vertYArray[i] > vertYArray[i + 1]) {
				extremaXArr.push_back(vertXArray[i]);
				extremaYArr.push_back(vertYArray[i]);
				//				cout << "array[i] = max " << endl;
			}
		}
		else if (vertYArray[i] < vertYArray[i - 1]) {
			if (vertYArray[i] < vertYArray[i + 1]) {
				extremaXArr.push_back(vertXArray[i]);
				extremaYArr.push_back(vertYArray[i]);
				//				cout << "array[i] = min " << endl;
			}
		}
	}

	if (vertYArray[numOfVertices - 1] > vertYArray[numOfVertices - 2]) {
		if (vertYArray[numOfVertices - 1] > vertYArray[0]) {
			extremaXArr.push_back(vertXArray[numOfVertices - 1]);
			extremaYArr.push_back(vertYArray[numOfVertices - 1]);
			//			cout << "array[end] = max " << endl;
		}
	}
	else if (vertYArray[numOfVertices - 1] < vertYArray[numOfVertices - 2]) {
		if (vertYArray[numOfVertices - 1] < vertYArray[0]) {
			extremaXArr.push_back(vertXArray[numOfVertices - 1]);
			extremaYArr.push_back(vertYArray[numOfVertices - 1]);
			//			cout << "array[end] = min " << endl;
		}
	}

	//Other method(incomplete)
	/*
	int deltaX1;
	int deltaY1;

	int deltaX2;
	int deltaY2;

	for (int i = 0; i < (numOfVertices - 1); i++) {
	deltaX1 = vertXArray[i + 1] - vertYArray[i];
	deltaX2 = vertXArray[i + 2] - vertYArray[i + 1];

	deltaY1 = vertYArray[i + 1] - vertYArray[i];
	deltaY2 = vertYArray[i + 2] - vertYArray[i + 1];

	if (abs(deltaX1 + deltaX2) > deltaX1) {
	if (abs(deltaY1 + deltaY2) < deltaY1)

	}
	}
	*/

	numOfExtrema = extremaXArr.size();
	//test
/*
	for (int i = 0; i < numOfExtrema; i++) {
		cout << "Extrema " << (i + 1) << ": (" << extremaXArr[i] << "," << extremaYArr[i] << ")" << endl;
	}
*/
//	cout << "extrema x arr size: " << extremaXArr.size() << endl;
//	cout << "extrema y arr size: " << extremaYArr.size() << endl;


}


void polygon::drawPolygon()
{
	//	cout << "Starting drawPolygon Func" << endl;
	int tempVertex1[2];
	int tempVertex2[2];

	//	cout << "Made temp arrays" << endl;

	for (int i = 0; i < (numOfVertices - 1); i++) {
		tempVertex1[0] = vertXArray[i];
		tempVertex1[1] = vertYArray[i];
		//		cout << "finished tempVertex1" << endl;
		tempVertex2[0] = vertXArray[i + 1];
		tempVertex2[1] = vertYArray[i + 1];
		//		cout << "passed tempVertex2" << endl;
		DDALine(tempVertex1, tempVertex2);
	}
	if (numOfVertices != 0) {
		tempVertex1[0] = vertXArray[0];
		tempVertex1[1] = vertYArray[0];
		tempVertex2[0] = vertXArray[numOfVertices - 1];
		tempVertex2[1] = vertYArray[numOfVertices - 1];
		DDALine(tempVertex1, tempVertex2);
	}
}

void polygon::scanline()
{
//	cout << "Scan line called" << endl;
	for (int currY = 0; currY < yWindowSize; currY++) {
		float fillColor = getPixelColor(0, currY);
		float *fillColorPtr = &fillColor;
		float currPixelColor = getPixelColor(0, currY);
		float prevPixelColor = currPixelColor;
		for (int currX = 1; currX < xWindowSize; currX++) {
			//Won't change the first pixel, so we start at 1

			currPixelColor = getPixelColor(currX, currY);
			if (currPixelColor == 1.0) {
				//flipColor(fillColorPtr);
				if (prevPixelColor != 1.0) {
					flipColor(fillColorPtr);					//Makes doesn't flip repeatedly.
																//					cout << "Flipped color Once at" << "(" << currX << ", " << currY << ")" << endl;
				}

				int check = checkExtrema(currX, currY);
				//				cout << "check = " << check << endl;
				if (check == 1) {
					flipColor(fillColorPtr);
//					cout << "Flipped color twice" << "(" << currX << ", " << currY << ")" << endl;
				}


			}
			else
				colorPixel(currX, currY, fillColor, fillColor, fillColor);
			prevPixelColor = currPixelColor;
		}
	}
}


float polygon::getPixelColor(int x, int y)
{
	float pixelColor = PixelBuffer[(3 * x) + (y * yWindowSize * 3)];
	return pixelColor;
}

int polygon::checkExtrema(int x, int y) {
	for (int i = 0; i < numOfExtrema; i++) {
		if (y == extremaYArr[i]) {
			if (x == extremaXArr[i]) {
				return 1;           //found
			}
		}
	}
	return 0;
}


void polygon::flipColor(float *currColor) {
	*currColor = fabs(currColor[0] - 1.0);
}//flipColor

 //2d Transformations
void polygon::translate(int x, int y) {

//	cout << "Beginning Translation" << endl;
	for (int i = 0; i < numOfVertices; i++) {
		vertXArray[i] = vertXArray[i] + x;
		vertYArray[i] = vertYArray[i] + y;
	} //Translates vertices

	for (int i = 0; i < numOfExtrema; i++) {
		extremaXArr[i] = extremaXArr[i] + x;
		extremaYArr[i] = extremaYArr[i] + y;
	} //Translates vertices

	clearBuffer();
	drawPolygon();

	if (numOfVertices > 2)
		scanline();


}

#define PI 3.141592
void polygon::rotate(float theta) {

	int pivPointX = mean(vertXArray, numOfVertices);
	int pivPointY = mean(vertYArray, numOfVertices);
	int diffX;
	int diffY;
//	cout << "piv Point X = " << pivPointX << endl;
//	cout << " point point Y + " << pivPointY << endl;

	for (int i = 0; i < numOfVertices; i++) {
		diffX = vertXArray[i] - pivPointX;
		diffY = vertYArray[i] - pivPointY;

		vertXArray[i] = pivPointX + (diffX * cos(theta * PI / 180)) - (diffY * sin(theta * PI / 180));
		vertYArray[i] = pivPointY + (diffX * sin(theta * PI / 180)) + (diffY * cos(theta * PI / 180));
	}

	clearBuffer();
	drawPolygon();
	if(numOfVertices > 2)
		scanline();

}

void polygon::scale(float xFactor, float yFactor) {
	int pivPointX = mean(vertXArray, numOfVertices);
	int pivPointY = mean(vertYArray, numOfVertices);

	for (int i = 0; i < numOfVertices; i++) {
		vertXArray[i] = vertXArray[i] * xFactor + (pivPointX * (1 - xFactor));
		vertYArray[i] = vertYArray[i] * yFactor + (pivPointY * (1 - yFactor));
	}

	clearBuffer();
	drawPolygon();

	if (numOfVertices > 2)
		scanline();


}

int polygon::mean(vector<int> arr, int arrSize) {
	int sum = 0;
	int average = 0;

	for (int i = 0; i < arrSize; i++) {
		sum = sum + arr[i];
	}
	average = sum / arrSize;

	return average;
}
//-----------------------------------------------------------------------------------

void polygon::DDALine(int* vertex1, int* vertex2) {
	//MODIFIED. DOES NOT DRAW HORIZONTAL LINE OR MAX

	//	cout << "Drawing DDALINE" << endl;
	if (vertex1[0] > vertex2[0])
		swapVertex(vertex1, vertex2);	//makes it go right to left

	float m = findSlope(vertex1, vertex2);

	//cout << "slope is :" << m << endl;
	if (m == 99999) {
		//Vertical Line
		if (vertex1[1] > vertex2[1])
			swapVertex(vertex1, vertex2);	//makes it from the bottom to the top
		colorPixel(vertex1[0], vertex1[1], 1.0, 1.0, 1.0); //colors bottom one

		int currY = vertex1[1];

		while (currY != vertex2[1]) {
			colorPixel(vertex1[0], currY, 1.0, 1.0, 1.0);
			currY = currY + 1;
		}
	}
	if (m == 0) {
		//		cout << "slope is nothing!" << endl;
		//do nothing.
	}
	else if (abs(m) > 1) {
		//		cout << "Steep Line" << endl;
		if (vertex1[1] > vertex2[1])
			swapVertex(vertex1, vertex2);	//makes it from the bottom to the top
		colorPixel(vertex1[0], vertex1[1], 1.0, 1.0, 1.0); //colors min

		int currX = vertex1[0];
		int currY = vertex1[1];
		float tempX = vertex1[0];

		while (currY != vertex2[1]) {
			colorPixel(currX, currY, 1.0, 1.0, 1.0);
			tempX = tempX + 1 / m + .5;
			currX = (int)tempX;
			tempX = tempX - .5;
			currY = currY + 1;
		}
	}
	else { // m < 1
		   //		cout << "Regular Line" << endl;

		   //Color Min
		if (vertex1[1] > vertex2[1]) {
			swapVertex(vertex1, vertex2);	//makes it from the bottom to the top
			colorPixel(vertex1[0], vertex1[1], 1.0, 1.0, 1.0); //colors min
			swapVertex(vertex1, vertex2);
		}
		else
			colorPixel(vertex1[0], vertex1[1], 1.0, 1.0, 1.0); //colors min

		int currX = vertex1[0];
		int currY = vertex1[1];
		float tempY = vertex1[1];
		int lineMaxY = vertex1[1];
		if (vertex2[1] > vertex1[1]) {
			lineMaxY = vertex2[1];
		}


		while (currX != vertex2[0]) {
			tempY = tempY + m + .5;
			currY = (int)tempY;
			tempY = tempY - .5;
			currX = currX + 1;

			if (currY != lineMaxY) {
				colorPixel(currX, currY, 1.0, 1.0, 1.0);
			}
		}
	}
	//	cout << "Finished drawing line" << endl;
} //DDALine()

void polygon::swapVertex(int* vertexA, int* vertexB) {
	float tempfloat[2] = { vertexA[0], vertexA[1] };

	vertexA[0] = vertexB[0];
	vertexA[1] = vertexB[1];
	vertexB[0] = tempfloat[0];
	vertexB[1] = tempfloat[1];

}


float polygon::findSlope(int* vertxA, int* vertxB) {

	float deltaX = float(vertxB[0] - vertxA[0]);
	float deltaY = float(vertxB[1] - vertxA[1]);

	if (deltaX == 0)
		return 99999;

	float m = deltaY / deltaX;
	return m;

}


void polygon::colorPixel(int xCoor, int yCoor, float R, float G, float B) {

	PixelBuffer[(3 * xCoor) + (yCoor * yWindowSize * 3)] = R;
	PixelBuffer[(3 * xCoor) + (yCoor * yWindowSize * 3) + 1] = G;
	PixelBuffer[(3 * xCoor) + (yCoor * yWindowSize * 3) + 2] = B;

}

//Polygon clipping
//Sutherhand-Hodgman Polygon Clipping

void polygon::wholeClip(int xMin, int yMin, int xMax, int yMax)
{
	leftClip(xMin);
	rightClip(xMax);
	botClip(yMin);
	topClip(yMax);

	clearBuffer();
	if (numOfVertices > 0) 
		drawPolygon();

	if (numOfVertices > 3)
		scanline();

}

void polygon::leftClip(int xMin)
{
//	cout << " Starting left Clip" << endl;
	int v1Results = 0;
	int v2Results = 0;
	
	vector<int> newXArray;
	vector<int> newYArray;
	int newNumOfVert = 0;
	int tempVertex1[2];
	int tempVertex2[2];
	float m, b;
	int x, y;
	if (numOfVertices < 3)
		return;

	for (int i = 0; i < (numOfVertices - 1); i++) {
	//	cout << "i =" << i << endl;
		tempVertex1[0] = vertXArray[i];
		tempVertex1[1] = vertYArray[i];
		tempVertex2[0] = vertXArray[i+1];
		tempVertex2[1] = vertYArray[i+1];

		if (vertXArray[i] < xMin) 
			v1Results = 1;
	
		if (vertXArray[i + 1] < xMin) {
			v2Results = 1;
		}

		if (v1Results && v2Results) {
//cout << " Both out" << endl;
		} //both out of bounds
		else if (!(v1Results || v2Results)) {
	//		cout << "Both in" << endl;
			newXArray.push_back(vertXArray[i + 1]);
			newYArray.push_back(vertYArray[i + 1]);
			newNumOfVert++;
		} //both in bounds
		else if (v1Results == 1) {
	//		cout << " v1 out of bounds " << endl;
			m = findSlope(tempVertex1, tempVertex2);
			b = tempVertex1[1] - (m*(tempVertex1[0]));
			x = xMin;
			y = (m*x) + b;

			newXArray.push_back(x);
			newYArray.push_back(y);
			newXArray.push_back(vertXArray[i + 1]);
			newYArray.push_back(vertYArray[i + 1]);
			newNumOfVert++;
			newNumOfVert++;
		} //v1 is in bounds, v2 is out of bounds. Keep new v2
		else {
	//		cout << " v2 out of bounds " << endl;
			m = findSlope(tempVertex1, tempVertex2);
			b = tempVertex1[1] - (m*(tempVertex1[0]));
			x = xMin;
			y = (m*x) + b;

			newXArray.push_back(x);
			newYArray.push_back(y);
			newNumOfVert++;

		} //v2 is in bounds, v1 is out of bounds, keep new v2 and 2;

		v1Results = 0;
		v2Results = 0;
	}

//	cout << "Moving onto last line" << endl;
	//One more iteration
	tempVertex1[0] = vertXArray[numOfVertices - 1];
	tempVertex1[1] = vertYArray[numOfVertices - 1];
	tempVertex2[0] = vertXArray[0];
	tempVertex2[1] = vertYArray[0];

	if (tempVertex1[0] < xMin)
		v1Results = 1;

	if (tempVertex2[0] < xMin) {
		v2Results = 1;
	}

	if (v1Results && v2Results) {
//		cout << " both out " << endl;
	} //both out of bounds
	else if (!(v1Results || v2Results)) {
	//	cout << " both in " << endl;
		newXArray.push_back(tempVertex2[0]);
		newYArray.push_back(tempVertex2[1]);
		newNumOfVert++;
	} //both in bounds
	else if (v1Results == 1) {
//		cout << " v1 out of bounds " << endl;
		m = findSlope(tempVertex1, tempVertex2);
		b = tempVertex1[1] - (m*(tempVertex1[0]));
		x = xMin;
		y = (m*x) + b;

		newXArray.push_back(x);
		newYArray.push_back(y);
		newXArray.push_back(tempVertex2[0]);
		newYArray.push_back(tempVertex2[1]);
		newNumOfVert++;
		newNumOfVert++;

	} //v1 is out of bounds, v2 is in bounds. Keep new v2, v2
	else {
	//	cout << " v2 out of bounds " << endl;
		m = findSlope(tempVertex1, tempVertex2);
		b = tempVertex1[1] - (m*(tempVertex1[0]));
		x = xMin;
		y = (m*x) + b;

		newXArray.push_back(x);
		newYArray.push_back(y);
		newNumOfVert++;

	} //v1 is in bounds, v2 is out of bounds, keep new v2;
	//Finish that last iteration;

	vertXArray.swap(newXArray);
	vertYArray.swap(newYArray);
	numOfVertices = newNumOfVert;
//	delete newXArray;
//	delete newYArray;
}

void polygon::rightClip(int xMax)
{
//	cout << "Starting Right Clip" << endl;
	int v1Results = 0;
	int v2Results = 0;

	vector<int> newXArray;
	vector<int> newYArray;
	int newNumOfVert = 0;
	int tempVertex1[2];
	int tempVertex2[2];
	float m, b;
	int x, y;

	if (numOfVertices < 3)
		return;
	for (int i = 0; i < (numOfVertices - 1); i++) {
		tempVertex1[0] = vertXArray[i];
		tempVertex1[1] = vertYArray[i];
		tempVertex2[0] = vertXArray[i + 1];
		tempVertex2[1] = vertYArray[i + 1];

		if (vertXArray[i] > xMax)
			v1Results = 1;

		if (vertXArray[i + 1] > xMax) {
			v2Results = 1;
		}

		if (v1Results && v2Results) {
			//do nothing
		} //both out of bounds
		else if (!(v1Results || v2Results)) {
			newXArray.push_back(vertXArray[i + 1]);
			newYArray.push_back(vertYArray[i + 1]);
			newNumOfVert++;
		} //both in bounds
		else if (v1Results == 1) {

			m = findSlope(tempVertex1, tempVertex2);
			b = tempVertex1[1] - (m*(tempVertex1[0]));
			x = xMax;
			y = (m*x) + b;

			newXArray.push_back(x);
			newYArray.push_back(y);
			newXArray.push_back(vertXArray[i + 1]);
			newYArray.push_back(vertYArray[i + 1]);
			newNumOfVert++;
			newNumOfVert++;

		} //v2 is in bounds, v1 is out of bounds. Keep new v2 and 2
		else {

			m = findSlope(tempVertex1, tempVertex2);
			b = tempVertex1[1] - (m*(tempVertex1[0]));
			x = xMax;
			y = (m*x) + b;

			newXArray.push_back(x);
			newYArray.push_back(y);
			newNumOfVert++;


		} //v1 is in bounds, v2 is out of bounds, keep new v2 ;
		v1Results = 0;
		v2Results = 0;
	}

	//One more iteration
	tempVertex1[0] = vertXArray[numOfVertices - 1];
	tempVertex1[1] = vertYArray[numOfVertices - 1];
	tempVertex2[0] = vertXArray[0];
	tempVertex2[1] = vertYArray[0];

	if (tempVertex1[0] > xMax)
		v1Results = 1;

	if (tempVertex2[0] > xMax) {
		v2Results = 1;
	}

	if (v1Results && v2Results) {
		//do nothing
	} //both out of bounds
	else if (!(v1Results || v2Results)) {
		newXArray.push_back(tempVertex2[0]);
		newYArray.push_back(tempVertex2[1]);
		newNumOfVert++;
	} //both in bounds
	else if (v1Results == 1) {

		m = findSlope(tempVertex1, tempVertex2);
		b = tempVertex1[1] - (m*(tempVertex1[0]));
		x = xMax;
		y = (m*x) + b;

		newXArray.push_back(x);
		newYArray.push_back(y);
		newXArray.push_back(tempVertex2[0]);
		newYArray.push_back(tempVertex2[1]);
		newNumOfVert++;
		newNumOfVert++;

	} //v2 is in bounds, v1 is out of bounds. Keep new v2 and 2
	else {

		m = findSlope(tempVertex1, tempVertex2);
		b = tempVertex1[1] - (m*(tempVertex1[0]));
		x = xMax;
		y = (m*x) + b;

		newXArray.push_back(x);
		newYArray.push_back(y);
		newNumOfVert++;


	} //v1 is in bounds, v2 is out of bounds, keep new v2 ;

	  //Finish that last iteration;

	vertXArray.swap(newXArray);
	vertYArray.swap(newYArray);
	numOfVertices = newNumOfVert;
	//	delete newXArray;


}

void polygon::botClip(int yMin)
{

//	cout << "Starting bot Clip" << endl;
	int v1Results = 0;
	int v2Results = 0;

	vector<int> newXArray;
	vector<int> newYArray;
	int newNumOfVert = 0;
	int tempVertex1[2];
	int tempVertex2[2];
	float m, b;
	int x, y;
	if (numOfVertices < 3)
		return;

	for (int i = 0; i < (numOfVertices - 1); i++) {
		tempVertex1[0] = vertXArray[i];
		tempVertex1[1] = vertYArray[i];
		tempVertex2[0] = vertXArray[i + 1];
		tempVertex2[1] = vertYArray[i + 1];

		if (vertYArray[i] < yMin)
			v1Results = 1;

		if (vertYArray[i + 1] < yMin) {
			v2Results = 1;
		}

		if (v1Results && v2Results) {
			//do nothing
		} //both out of bounds
		else if (!(v1Results || v2Results)) {
			newXArray.push_back(vertXArray[i + 1]);
			newYArray.push_back(vertYArray[i + 1]);
			newNumOfVert++;
		} //both in bounds
		else if (v1Results == 1) {

			m = findSlope(tempVertex1, tempVertex2);
			if (m == 99999) {
				y = yMin;
				x = vertXArray[i];
			}
			else {
				b = tempVertex1[1] - (m*(tempVertex1[0]));
				y = yMin;
				x = (y - b) / m;
			}
			newXArray.push_back(x);
			newYArray.push_back(y);
			newXArray.push_back(vertXArray[i + 1]);
			newYArray.push_back(vertYArray[i + 1]);
			newNumOfVert++;
			newNumOfVert++;
		} //v1 is out of bounds, v2 is in bounds. Keep new v2 and 2
		else {
			m = findSlope(tempVertex1, tempVertex2);
			if (m == 99999) {
				y = yMin;
				x = vertXArray[i];
			}
			else {
				b = tempVertex1[1] - (m*(tempVertex1[0]));
				y = yMin;
				x = (y - b) / m;
			}
			newXArray.push_back(x);
			newYArray.push_back(y);
			newNumOfVert++;

		} //v1 is in bounds, v2 is out of bounds, keep new v2';
		v1Results = 0;
		v2Results = 0;
	}

	//One more iteration
	tempVertex1[0] = vertXArray[numOfVertices - 1];
	tempVertex1[1] = vertYArray[numOfVertices - 1];
	tempVertex2[0] = vertXArray[0];
	tempVertex2[1] = vertYArray[0];

	if (tempVertex1[1]  < yMin)
		v1Results = 1;

	if (tempVertex2[1]  < yMin) {
		v2Results = 1;
	}

	if (v1Results && v2Results) {
		//do nothing
	} //both out of bounds
	else if (!(v1Results || v2Results)) {
		newXArray.push_back(tempVertex2[0]);
		newYArray.push_back(tempVertex2[1]);
		newNumOfVert++;
	} //both in bounds
	else if (v1Results == 1) {

		m = findSlope(tempVertex1, tempVertex2);
		if (m == 99999) {
			y = yMin;
			x = tempVertex1[0];
		}
		else {
			b = tempVertex1[1] - (m*(tempVertex1[0]));
			y = yMin;
			x = (y - b) / m;
		}
		newXArray.push_back(x);
		newYArray.push_back(y);
		newXArray.push_back(tempVertex2[0]);
		newYArray.push_back(tempVertex2[1]);
		newNumOfVert++;
		newNumOfVert++;
	} //v1 is out of bounds, v2 is in bounds. Keep new v2 and 2
	else {
		m = findSlope(tempVertex1, tempVertex2);
		if (m == 99999) {
			y = yMin;
			x = tempVertex1[0];
		}
		else {
			b = tempVertex1[1] - (m*(tempVertex1[0]));
			y = yMin;
			x = (y - b) / m;
		}
		newXArray.push_back(x);
		newYArray.push_back(y);
		newNumOfVert++;

	} //v1 is in bounds, v2 is out of bounds, keep new v2';

	vertXArray.swap(newXArray);
	vertYArray.swap(newYArray);
	numOfVertices = newNumOfVert;
	//	delete newXArray;

}

void polygon::topClip(int yMax)
{

	//cout << "Starting top Clip" << endl;
	int v1Results = 0;
	int v2Results = 0;

	vector<int> newXArray;
	vector<int> newYArray;
	int newNumOfVert = 0;
	int tempVertex1[2];
	int tempVertex2[2];
	float m, b;
	int x, y;
	if (numOfVertices < 3)
		return;

	for (int i = 0; i < (numOfVertices - 1); i++) {
		tempVertex1[0] = vertXArray[i];
		tempVertex1[1] = vertYArray[i];
		tempVertex2[0] = vertXArray[i + 1];
		tempVertex2[1] = vertYArray[i + 1];

		if (vertYArray[i] > yMax)
			v1Results = 1;

		if (vertYArray[i + 1] > yMax) {
			v2Results = 1;
		}

		if (v1Results && v2Results) {
//			cout << "Both out of bounds" << endl;
		} //both out of bounds
		else if (!(v1Results || v2Results)) {
//			cout << "Both in bounds" << endl;
			newXArray.push_back(vertXArray[i + 1]);
			newYArray.push_back(vertYArray[i + 1]);
//			cout << "Pushing ( " << vertXArray[i + 1] << "," << vertYArray[i + 1] << ")" << endl;
			newNumOfVert++;
		} //both in bounds
		else if (v1Results == 1) {
//			cout << "v1 is out of bounds" << endl;

			m = findSlope(tempVertex1, tempVertex2);
			if (m == 99999) {
				y = yMax;
				x = vertXArray[i];
			}
			else {
				b = tempVertex1[1] - (m*(tempVertex1[0]));
				y = yMax;
				x = (y - b) / m;
			}
			newXArray.push_back(x);
			newYArray.push_back(y);
			newXArray.push_back(vertXArray[i + 1]);
			newYArray.push_back(vertYArray[i + 1]);
			newNumOfVert++;
			newNumOfVert++;
		} //v1 is out of bounds, v2 is in bounds. Keep new v2 and 2
		else {
//			cout << "v2 is out of bounds" << endl;
			m = findSlope(tempVertex1, tempVertex2);
			if (m == 99999) {
				y = yMax;
				x = vertXArray[i];
			}
			else {
				b = tempVertex1[1] - (m*(tempVertex1[0]));
				y = yMax;
				x = (y - b) / m;
			}
			newXArray.push_back(x);
			newYArray.push_back(y);
			newNumOfVert++;

		} //v1 is in bounds, v2 is out of bounds, keep new v2';
		v1Results = 0;
		v2Results = 0;
	}

	//One more iteration
	tempVertex1[0] = vertXArray[numOfVertices - 1];
	tempVertex1[1] = vertYArray[numOfVertices - 1];
	tempVertex2[0] = vertXArray[0];
	tempVertex2[1] = vertYArray[0];

	if (tempVertex1[1] > yMax)
		v1Results = 1;

	if (tempVertex2[1]  > yMax) {
		v2Results = 1;
	}

	if (v1Results && v2Results) {
		//cout << "both out" << endl;
		//do nothing
	} //both out of bounds
	else if (!(v1Results || v2Results)) {
	//	cout << "both in" << endl;
		newXArray.push_back(tempVertex2[0]);
		newYArray.push_back(tempVertex2[1]);
		newNumOfVert++;
	} //both in bounds
	else if (v1Results == 1) {
		//cout << "v1 is out of bounds" << endl;
		m = findSlope(tempVertex1, tempVertex2);
		if (m == 99999) {
			y = yMax;
			x = tempVertex1[0];
		}
		else {
			b = tempVertex1[1] - (m*(tempVertex1[0]));
			y = yMax;
			x = (y - b) / m;
		}
		newXArray.push_back(x);
		newYArray.push_back(y);
		newXArray.push_back(tempVertex2[0]);
		newYArray.push_back(tempVertex2[1]);
		newNumOfVert++;
		newNumOfVert++;
	} //v1 is out of bounds, v2 is in bounds. Keep new v2 and 2
	else {
	//	cout << "v2 is out of bounds" << endl;
		m = findSlope(tempVertex1, tempVertex2);
		if (m == 99999) {
			y = yMax;
			x = tempVertex1[0];
		}
		else {
			b = tempVertex1[1] - (m*(tempVertex1[0]));
			y = yMax;
			x = (y - b) / m;
		}
		newXArray.push_back(x);
		newYArray.push_back(y);
		newNumOfVert++;

	} //v1 is in bounds, v2 is out of bounds, keep new v2';

	vertXArray.swap(newXArray);
	vertYArray.swap(newYArray);
	numOfVertices = newNumOfVert;
	//	delete newXArray;


}



//CS Clipping
void polygon::CSClipping(int xMin, int yMin, int xMax, int yMax)
{
	clearBuffer();
	int tempVertex1[2];
	int tempVertex2[2];
	int vertex1ABLR[4] = { 0, 0, 0, 0 };
	int vertex2ABLR[4] = { 0, 0, 0, 0 };
	int accept = 1;
	int v1Total = 0;  //Will count how many vertex 1;
	int v2Total = 0;  //Will count how many vertex 1;

	tempVertex1[0] = vertXArray[0];
	tempVertex1[1] = vertYArray[0];
	tempVertex2[0] = vertXArray[1];
	tempVertex2[1] = vertYArray[1];

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
		vertXArray[0] = 0;
		vertYArray[0] = 0;
		vertXArray[1] = 0;
		vertYArray[1] = 0;
		clearBuffer();
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
			vertXArray[0] = tempVertex1[0];
			vertYArray[0] = tempVertex1[1];
			vertXArray[1] = tempVertex2[0];
			vertYArray[1] = tempVertex2[1];

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
			vertXArray[0] = tempVertex1[0];
			vertYArray[0] = tempVertex1[1];
			vertXArray[1] = tempVertex2[0];
			vertYArray[1] = tempVertex2[1];
		}
	}
}

void polygon::checkABLR(int *vertex, int *vertexABLR, int xMin, int yMin, int xMax, int yMax)
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

void polygon::findNewVertex(int *vertex, int *vertexABLR, int xMin, int yMin, int xMax, int yMax, float m, float b) {
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