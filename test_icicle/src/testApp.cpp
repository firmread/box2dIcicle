#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
//	ofToggleFullscreen();
    
	world.init();
	world.setGravity(0, 10);
    //	world.createBounds();
    world.createGround();
	world.setFPS(30.0);
	world.registerGrabbing();
	
}

//--------------------------------------------------------------
void testApp::update() {
	world.update();
    for(int i=0; i<gons.size(); i++) {
		if (   gons[i].body->GetPosition().x > ofGetWidth()+50
            || gons[i].body->GetPosition().x < -50
            || gons[i].body->GetPosition().y > ofGetHeight()+50 )
        {
            gons[i].destroy();
            gons.erase(gons.begin()+i);
        }
	}
    
    
//    if (gons.size()> 10) {
//        //need to destroy body from box2d world before erase the object out of the vector
//        gons.begin()->destroy();
//        gons.erase(gons.begin());
//    }
}


//--------------------------------------------------------------
void testApp::draw() {
	
	
    
    for(int i=0; i<gons.size(); i++) {
		ofFill();
		ofSetHexColor(0x1eb2ff);
		gons[i].draw();
	}

	// draw the ground
	world.drawGround();
	
	
	
	string info = "";
	info += "Press [p] for triangles\n";
    info += "Press [f] for toggle fullscreen\n";
	info += "Total Bodies: "+ofToString(world.getBodyCount())+"\n";
	info += "Total Joints: "+ofToString(world.getJointCount())+"\n\n";
	info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 30, 30);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	
    if (key == 'p'){
        
        ofxBox2dTriangle gon;
        ofPoint a = ofPoint(ofVec2f(ofRandom(-10,-20),ofRandom(-10,-20)) + ofPoint(mouseX, mouseY));
        ofPoint b = ofPoint(ofVec2f(ofRandom(10,20),ofRandom(-10,-20)) + ofPoint(mouseX, mouseY));
        ofPoint c = ofPoint(ofVec2f(ofRandom(-5,5),ofRandom(10,20)) + ofPoint(mouseX, mouseY));
        //gon.addVertex(ofVec2f(-20,-20) + ofPoint(mouseX, mouseY));
        gon.setPhysics(3.0, 0.53, 0.1);
       
        gon.setup(world.getWorld(), a, b, c);
        gons.push_back(gon);
        
        
    }
	
	if(key == 'f') ofToggleFullscreen();
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
}

