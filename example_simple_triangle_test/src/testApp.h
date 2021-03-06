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
	
	ofxBox2d						world;			  //	the box2d world
    vector	<ofxBox2dTriangle>		gons;			  //	defalut box2d rects
	
};

