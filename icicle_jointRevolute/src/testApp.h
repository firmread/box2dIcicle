#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxBox2dTriangle.h"

#include "ofxBox2dRevoluteJoint.h"
#include "ofxBox2dWeldJoint.h"

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

    
    
    
    //	the box2d world
	ofxBox2d                    box2d;


    
    //fixed part of icicle
    void makeAnchor(int size, ofPoint anchorPos, bool down);
    vector <ofxBox2dTriangle>   anchors;

    //breakable part of icicle
    void makeIcicle(int size, ofPoint iciclePos, bool down);
    vector <ofxBox2dTriangle>   icicles;

    //joints
    void makeJoint(b2Body *body1, b2Body *body2);
    vector <ofxBox2dRevoluteJoint>      joints;

    
    
    // object follow mouse
    ofxBox2dTriangle            cursor;
    
    
	
    
    
    
	
};

