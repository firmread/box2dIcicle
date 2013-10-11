#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxBox2dTriangle.h"


// -------------------------------------------------

class testApp : public ofBaseApp {
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void resized(int w, int h);
    void makeAnchor(int size, ofPoint anchorPos);
	//	the box2d world
	ofxBox2d				box2d;
    //	fixed anchor
//	ofxBox2dRect			anchor;
    vector <ofxBox2dTriangle> anchors;
//    ofxBox2dTriangle        anchor;
	ofxBox2dTriangle        cursor;
    
    vector <ofxBox2dRect>   circles;
	vector <ofxBox2dJoint>  joints;
    
    
    
	
};

