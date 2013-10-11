#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);

	box2d.init();
	box2d.setGravity(0, 10);
	box2d.setFPS(30.0);
    box2d.createBounds();
	box2d.registerGrabbing();
    
    //cursor triangle
    ofPoint cur_a = ofPoint(-10,-10);
    ofPoint cur_b = ofPoint(10,-10);
    ofPoint cur_c = ofPoint(0, 10);
    cursor.setup(box2d.getWorld(), cur_a, cur_b, cur_c);
    
    //anchor
//    anchor.setup(box2d.getWorld(), ofGetWidth()/2, ofGetHeight()/2, 20, 20);
    
    int tri_scale = 20;
    makeAnchor(tri_scale, ofPoint(ofGetWidth()/2-3*tri_scale, tri_scale));
    makeAnchor(tri_scale, ofPoint(ofGetWidth()/2+3*tri_scale, tri_scale));
    
	
	// first we add just a few icicles
//	for (int i=0; i<3; i++) {
//		icicle.setup(box2d.getWorld(), ofGetWidth()/2, ofGetHeight()/2-(i*20), 20, 20);
//	}
    
    makeIcicle(tri_scale, ofPoint(ofGetWidth()/2-2*tri_scale,tri_scale), false);
    makeIcicle(tri_scale, ofPoint(ofGetWidth()/2-tri_scale,tri_scale), true);
    makeIcicle(tri_scale, ofPoint(ofGetWidth()/2+tri_scale,tri_scale), false);
    makeIcicle(tri_scale, ofPoint(ofGetWidth()/2+2*tri_scale,tri_scale), true);
    makeIcicle(tri_scale, ofPoint(ofGetWidth()/2+3*tri_scale,tri_scale), false);
        
	
	// now connect each icicle with a joint
	for (int i=0; i<icicles.size()+1; i++) {
		
		ofxBox2dJoint joint;
		if(i == 0) {
            joint.setup(box2d.getWorld(), anchors[0].body, icicles[0].body);
		}
        else if (i==icicles.size()) {
            joint.setup(box2d.getWorld(), icicles[icicles.size()-1].body, anchors[1].body);
        }
		else {
			joint.setup(box2d.getWorld(), icicles[i-1].body, icicles[i].body);
		}
        joint.setDamping(10);
		joint.setLength(25);
        joint.setFrequency(5.f);
		joints.push_back(joint);
	}
}

void testApp::makeAnchor(int size, ofPoint anchorPos){
    ofxBox2dTriangle anc;
    
    ofPoint anc_a = anchorPos + ofPoint(-1*size,-1*size);
    ofPoint anc_b = anchorPos + ofPoint(size, -1*size);
    ofPoint anc_c = anchorPos + ofPoint(0, size);
    
    anc.setup(box2d.getWorld(), anc_a, anc_b, anc_c);
    anchors.push_back(anc);
    
}

void testApp::makeIcicle(int size, ofPoint iciclePos, bool down){
    ofxBox2dTriangle ici;
    if (!down) size = -size;
    ofPoint ici_a = iciclePos + ofPoint (-1*size, -1*size);
    ofPoint ici_b = iciclePos + ofPoint (size, -1*size);
    ofPoint ici_c = iciclePos + ofPoint (0, size);
    
    ici.setPhysics(3.0, 0.53, 0.1);
    ici.setup(box2d.getWorld(), ici_a, ici_b, ici_c);
    icicles.push_back(ici);
}

//--------------------------------------------------------------
void testApp::update() {
    
	box2d.update();	
    
//    cursor.setPosition(ofPoint(mouseX, mouseY));
    
    if (joints[0].getReactionForce(30).length()>3000) {
        joints[0].destroy();
    }
    if (joints[1].getReactionForce(30).length()>3000) {
        joints[1].destroy();
    }
    if (joints[2].getReactionForce(30).length()>3000) {
        joints[2].destroy();
    }
    
    
//    for (int i; i<icicles.size(); i++){
//        if (joints[i].getReactionForce(30).length()>100) {
//            joints[i].destroy();
//        }
//    }

}


//--------------------------------------------------------------
void testApp::draw() {
	
	ofSetHexColor(0xf2ab01);
    
	for(int i=0; i<anchors.size(); i++) {
        anchors[i].draw();
    }
    ofSetHexColor(0x366272);
	cursor.draw();
    
    
	for(int i=0; i<icicles.size(); i++) {
		ofFill();
		ofSetHexColor(0x01b1f2);
		icicles[i].draw();
	}
	
	for(int i=0; i<joints.size(); i++) {
		ofSetHexColor(0x444342);
		joints[i].draw();
	}
    
	
	string info = "";
	info += "Press [n] to add a new joint\n";
	info += "click and pull the chain around\n";
	info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
    info += "joints[0] reaction force: " + ofToString(joints[0].getReactionForce(30));
    info += "\njoints[1] reaction force: " + ofToString(joints[1].getReactionForce(30));
    info += "\njoints[2] reaction force: " + ofToString(joints[2].getReactionForce(30));
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 30, 30);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	
//	if(key == 'n') {
//		
//		// add a new icicle
//		ofxBox2dTriangle icicle;
//		icicle.setPhysics(3.0, 0.53, 0.1);
//		icicle.setup(box2d.getWorld(), icicles.back().getPosition().x+ofRandom(-30, 30), icicles.back().getPosition().y-30,8, 8);
//		icicles.push_back(icicle);
//	
//		// get this cirlce and the prev cirlce
//		int a = (int)icicles.size()-2;
//		int b = (int)icicles.size()-1; 
//
//		// now connect the new icicle with a joint
//		ofxBox2dJoint joint;
//		joint.setup(box2d.getWorld(), icicles[a].body, icicles[b].body);
//		joint.setLength(10);
//		joints.push_back(joint);
//	}
	
	if(key == 't') ofToggleFullscreen();
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

