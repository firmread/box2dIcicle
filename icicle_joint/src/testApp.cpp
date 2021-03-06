#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);

	box2d.init();
	box2d.setGravity(0, 10);
	box2d.setFPS(30.0);
//    box2d.createBounds();
    box2d.createGround(ofPoint(0,ofGetHeight()), ofPoint(ofGetWidth(),ofGetHeight()));
	box2d.registerGrabbing();
    
    int tri_scale = 20;
    
    //cursor triangle
    ofPoint cur_a = ofPoint(-tri_scale,-tri_scale);
    ofPoint cur_b = ofPoint(tri_scale,-tri_scale);
    ofPoint cur_c = ofPoint(0, tri_scale);
    cursor.setup(box2d.getWorld(), cur_a, cur_b, cur_c);
    
    //anchor
//    anchor.setup(box2d.getWorld(), ofGetWidth()/2, ofGetHeight()/2, 20, 20);
    //0
    makeAnchor(tri_scale, ofPoint(ofGetWidth()/2-3*tri_scale, tri_scale),true);
    //1
    makeAnchor(tri_scale, ofPoint(ofGetWidth()/2-tri_scale, tri_scale), true);
    //2
    makeAnchor(tri_scale, ofPoint(ofGetWidth()/2+tri_scale, tri_scale), true);
    //3
    makeAnchor(tri_scale, ofPoint(ofGetWidth()/2+3*tri_scale, tri_scale),true);
    
	
	// first we add just a few icicles
//	for (int i=0; i<3; i++) {
//		icicle.setup(box2d.getWorld(), ofGetWidth()/2, ofGetHeight()/2-(i*20), 20, 20);
//	}
    
    //ROW 1
    //0
    makeIcicle(tri_scale, ofPoint(ofGetWidth()/2-2*tri_scale,tri_scale), false);
    //1
    makeIcicle(tri_scale, ofPoint(ofGetWidth()/2,tri_scale), false);
    //2
    makeIcicle(tri_scale, ofPoint(ofGetWidth()/2+2*tri_scale,tri_scale), false);

    //ROW 2 part 1
    //3
    makeIcicle(tri_scale, ofPoint(ofGetWidth()/2-2*tri_scale,3*tri_scale), true);
    //4
    makeIcicle(tri_scale, ofPoint(ofGetWidth()/2,3*tri_scale), true);
    //5
    makeIcicle(tri_scale, ofPoint(ofGetWidth()/2+2*tri_scale,3*tri_scale), true);
    
    //ROW 2 part 2
    //6
    makeIcicle(tri_scale, ofPoint(ofGetWidth()/2-tri_scale, 3*tri_scale), false);
    //7
    makeIcicle(tri_scale, ofPoint(ofGetWidth()/2+tri_scale, 3*tri_scale), false);
    
    //ROW 3
    //8
    makeIcicle(tri_scale, ofPoint(ofGetWidth()/2-tri_scale, 5*tri_scale), true);
    //9
    makeIcicle(tri_scale, ofPoint(ofGetWidth()/2+tri_scale, 5*tri_scale), true);
    //10
    makeIcicle(tri_scale, ofPoint(ofGetWidth()/2, 5*tri_scale), false);
    
    //ROW 4
    //11
    makeIcicle(tri_scale, ofPoint(ofGetWidth()/2, 7*tri_scale), true);
    
    
    //row1
    makeJoint(anchors[0].body, icicles[0].body);
	makeJoint(icicles[0].body, anchors[1].body);
    makeJoint(anchors[1].body, icicles[1].body);
    makeJoint(icicles[1].body, anchors[2].body);
    makeJoint(anchors[2].body, icicles[2].body);
    makeJoint(icicles[2].body, anchors[3].body);
    
    //row 1 - 2
    makeJoint(icicles[0].body, icicles[3].body);
    makeJoint(icicles[1].body, icicles[4].body);
    makeJoint(icicles[2].body, icicles[5].body);
    
    //row2
    makeJoint(icicles[3].body, icicles[6].body);
    makeJoint(icicles[6].body, icicles[4].body);
    makeJoint(icicles[4].body, icicles[7].body);
    makeJoint(icicles[7].body, icicles[5].body);
    
    //row 2 - 3
    makeJoint(icicles[6].body, icicles[8].body);
    makeJoint(icicles[7].body, icicles[9].body);
    //row3
    makeJoint(icicles[8].body, icicles[10].body);
    makeJoint(icicles[9].body, icicles[10].body);
    
    //ice tip
    makeJoint(icicles[10].body, icicles[11].body);
    
    
//	// now connect each icicle with a joint
//	for (int i=0; i<icicles.size()+1; i++) {
//		
//		ofxBox2dJoint joint;
//		if(i == 0) {
//            joint.setup(box2d.getWorld(), anchors[0].body, icicles[0].body);
//		}
//        else if (i==icicles.size()) {
//            joint.setup(box2d.getWorld(), icicles[icicles.size()-1].body, anchors[1].body);
//        }
//		else {
//			joint.setup(box2d.getWorld(), icicles[i-1].body, icicles[i].body);
//		}
//        joint.setDamping(10.f);
//		joint.setLength(5);
//        joint.setFrequency(2.f);
//		joints.push_back(joint);
//	}
}

void testApp::makeJoint(b2Body *body1, b2Body *body2){
    
    ofxBox2dJoint joint;
    joint.setup(box2d.getWorld(), body1, body2);

    joint.setDamping(10.f);
    joint.setLength(5);
    joint.setFrequency(2.f);
    joints.push_back(joint);
    

}

void testApp::makeAnchor(int size, ofPoint anchorPos, bool down){
    ofxBox2dTriangle anc;
    if (!down) size = -size;
    ofPoint anc_a = anchorPos + ofPoint(-size ,-size);
    ofPoint anc_b = anchorPos + ofPoint(size  ,-size);
    ofPoint anc_c = anchorPos + ofPoint(0     , size);
    
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
    
    cursor.setPosition(ofPoint(mouseX, mouseY));
    
//    if (joints[0].getReactionForce(30).length()>3000) {
//        joints[0].destroy();
//    }
//    if (joints[1].getReactionForce(30).length()>3000) {
//        joints[1].destroy();
//    }
//    if (joints[2].getReactionForce(30).length()>3000) {
//        joints[2].destroy();
//    }
    
    
    for (int i=0; i<joints.size(); i++){
        if (joints[i].getReactionForce(30).length()>300) {
            joints[i].destroy();
        }
    }
    
    

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
    info += "joints[0] reaction force: " + ofToString(joints[0].getReactionForce(30).length());
    info += "\njoints[1] reaction force: " + ofToString(joints[1].getReactionForce(30).length());
    info += "\njoints[2] reaction force: " + ofToString(joints[2].getReactionForce(30).length());
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

