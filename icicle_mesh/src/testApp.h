#pragma once

#include "ofxOpenCv.h"
#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxBox2dTriangle.h"
#include "ofxTriangleMesh.h"




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
	
    
    
    
	// this is the function for contacts
	void contactStart(ofxBox2dContactArgs &e);
	void contactEnd(ofxBox2dContactArgs &e);
    
    
    
    
    
	ofxBox2d						box2d;
    //	the box2d world
	
    vector		<ofxBox2dCircle>	circles;
    //	default box2d circles
	vector		<ofxBox2dRect>		boxes;
    //	defalut box2d rects
    vector		<ofxBox2dTriangle>		gons;
    //	defalut box2d triangles
	
    vector < ofColor > colors;
    
    
    ofxBox2dRect ceiling[10];
    //for icicle anchors
    
    ofxCvGrayscaleImage image;
    ofxCvContourFinder finder;
    ofImage icicles;
    vector < ofxTriangleMesh > meshes;
    
    //ofxTriangleMesh mesh;
    
    
    //joints
    void makeJoint(b2Body *body1, b2Body *body2);
    vector <ofxBox2dJoint>      joints;
    
    
    
    
    
};

