

#pragma once
#include "ofMain.h"
#include "ofxBox2dBaseShape.h"

class ofxBox2dTriangle : public ofxBox2dBaseShape {

private:
    
	float width, height;
    ofPolyline   shape;
public:
	
	//------------------------------------------------
	ofxBox2dTriangle();	
	
	//------------------------------------------------
	void setup(b2World * b2dworld, vector < ofPoint >  pts);
	void setup(b2World * b2dworld, ofPoint a, ofPoint b, ofPoint c);
	
    ofPolyline& getTriangleShape();
	
	//------------------------------------------------
    void draw();

	//------------------------------------------------
	void addAttractionPoint(float x, float y, float amt);
	void addAttractionPoint(ofVec2f pt, float amt);

	void addRepulsionForce(float x, float y, float amt);
	void addRepulsionForce(ofVec2f pt, float amt);
    
    bool isActive;
    ofPoint initA, initB, initC;

};














