#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
	
	box2d.init();
	box2d.setGravity(0, 10);
    box2d.createGround();
	box2d.setFPS(30.0);
	box2d.registerGrabbing();
	
    //load image and triangulate them
    icicles.loadImage("icicles_S3.png");
    image.allocate(icicles.width, icicles.height);
    
    unsigned char * pixa = icicles.getPixels();
    unsigned char * pixb = image.getPixels();
    
    for (int i = 0; i < icicles.width * icicles.height; i++){
        
        pixb[i] = pixa[i*4+3];
    }
    image.flagImageChanged();
    
    finder.findContours(image, 0, 1000000, 10, false);
    
    for (int i = 0; i < finder.nBlobs; i++){
        //only taking the largest blob!
        if (i == 0){
            ofxTriangleMesh mesh;
            meshes.push_back(mesh);
            ofPolyline line;
            line.addVertices(finder.blobs[i].pts);
            meshes[meshes.size()-1].triangulate(line, -1, 500);
        }
    }
    
    // ceiling static rectangles
    for (int i=0; i < 10; i++) {
        ofxBox2dRect temp;
        ofRectangle rect = ofRectangle((i)*ofGetWidth()/10+ofGetWidth()/20, -10, ofGetWidth()/10-10, 14);
        temp.setup(box2d.getWorld(), rect);
        ceilings.push_back(temp);
    }
    
    bShowTriNum = false;
    bBox2dUpdate = false;
}





void testApp::makeJoint(ofxBox2dBaseShape & shape1, ofxBox2dBaseShape & shape2){
    
    ofxBox2dWeldJoint joint;
    joint.setup(box2d.getWorld(), shape1.body, shape2.body);
    float pointDist = ofDist(shape1.getPosition().x, shape1.getPosition().y, shape2.getPosition().x, shape2.getPosition().y);
//    joint.setDamping(10.f);
//    joint.setLength(pointDist * 0.8);
//    joint.setFrequency(2.f);
    joints.push_back(joint);
    
}

//--------------------------------------------------------------
void testApp::update() {
	
//    box2d.createGround();
//    for (int i=0; i<gons.size(); i++) {
//        //out of screen
//        if (gons[i].getPosition().x<0
//            || gons[i].getPosition().x>ofGetWidth()
//            || gons[i].getPosition().y> ofGetHeight()+100) {
//            gons[i].destroy();
//            gons.erase(gons.begin()+i);
//        }
//        //or stop moving
//        else if (gons[i].getVelocity().length() < .1){
//            gons[i].destroy();
//            gons.erase(gons.begin()+i);
//        }
//
//    }
//    //delete circles those are not moving
//	for(int i=0; i<circles.size(); i++) {
//        if (circles[i].getPosition().x<0
//            || circles[i].getPosition().x>ofGetWidth()
//            || circles[i].getPosition().y> ofGetHeight()+100) {
//            circles[i].destroy();
//            circles.erase(circles.begin()+i);
//            
//        }
//        
////        else if (circles[i].getVelocity().length() < .2){
////            circles[i].destroy();
////            circles.erase(circles.begin()+i);
////        }
//	}
//	//delete rectangles those are not moving
//	for(int i=0; i<boxes.size(); i++) {
//        if (boxes[i].getPosition().x<0
//            || boxes[i].getPosition().x>ofGetWidth()
//            || boxes[i].getPosition().y> ofGetHeight()+100) {
//            boxes[i].destroy();
//            boxes.erase(boxes.begin()+i);
//            
//        }
//        
////        else if (boxes[i].getVelocity().length() < .2){
////            boxes[i].destroy();
////            boxes.erase(boxes.begin()+i);
////        }
//	}
    
    
    for (int i=0; i<joints.size(); i++){
        if (joints[i].getReactionForce(30).length()>50) {
            joints[i].destroy();
        }
    }
    
    if (bBox2dUpdate) box2d.update();
    
}


//--------------------------------------------------------------
void testApp::draw() {
	
    ofBackground(0);
	
	for(int i=0; i<circles.size(); i++) {
		ofFill();
		ofSetHexColor(0xf6c738);
		circles[i].draw();
	}
	
	for(int i=0; i<boxes.size(); i++) {
		ofFill();
		ofSetHexColor(0xBF2545);
		boxes[i].draw();
	}
    //draw ceiling blocks
    for(int i=0; i<ceilings.size(); i++) {
		ofFill();
		ofSetColor(100+(i*10));
		ceilings [i].draw();
	}
    
    
    for(int i=0; i<gons.size(); i++) {
		ofFill();
		ofSetColor(30,30,30);
        if (i == 30 || i == 31) ofSetColor(255,0,0);
        gons[i].draw();
        
        
        ofNoFill();
		ofSetColor(100,100,100);
        if (i == 30 || i == 31) ofSetColor(255,0,0);
        gons[i].draw();
	}

//    cout << joints.size() << endl;
    for(int i=0; i<joints.size(); i++) {
        ofSetHexColor(0xFFFFFF);
        joints[i].draw();
    }
    
    
	// draw the ground
	box2d.drawGround();
	
	
	
	string info = "";
	info += "Press [spacebar] to load object\n";
	info += "Press [q] one step update\n";
	info += "Press [a] update on/off \n";
	info += "Press [c] add circles\n";
	info += "Press [b] add blocks\n";
	info += "Press [t] go fullscreen\n";
	info += "Press [n] show body numbers\n";
	info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
	info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
	info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
    info += "meshes.size(): " + ofToString(meshes.size())+"\n";
    info += "meshes[0].triangles.size(): " + ofToString(meshes[0].triangles.size())+"\n";
	ofSetColor(ofColor::white);
	ofDrawBitmapString(info, ofGetWidth() - 300, 30);
    
    if (bShowTriNum) {
        
        for (int i =0; i<gons.size(); i++) {
            ofDrawBitmapString(ofToString(i), gons[i].getPosition());
        }
            
    }

    //finder.draw();
    
    for (int i = 0; i < meshes.size(); i++){
//        meshes[i].draw();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	
	if (key == 't') ofToggleFullscreen();
    
    //update box2dworld
    if (key == 'q') {
        if (bBox2dUpdate) bBox2dUpdate = false;
        box2d.update();
    }
    if (key == 'a') bBox2dUpdate = !bBox2dUpdate;
    if (key == 'n') bShowTriNum = !bShowTriNum;
    
    
    //make circle
	if (key == 'c') {
		float r = ofRandom(4, 20);
		ofxBox2dCircle circle;
		circle.setPhysics(3.0, 0.53, 0.1);
		circle.setup(box2d.getWorld(), mouseX, mouseY, r);
		circles.push_back(circle);
	}
    
	//make box
	if (key == 'b') {
		float w = ofRandom(4, 20);	
		float h = ofRandom(4, 20);	
		ofxBox2dRect rect;
		rect.setPhysics(3.0, 0.53, 0.1);
		rect.setup(box2d.getWorld(), mouseX, mouseY, w, h);
		boxes.push_back(rect);
	}
    
    //make triangle
    if (key == 'p'){
        ofxBox2dTriangle gon;
        ofPoint a = ofPoint(ofVec2f(-20,-20) + ofPoint(mouseX, mouseY));
        ofPoint b = ofPoint(ofVec2f(0,0) + ofPoint(mouseX, mouseY));
        ofPoint c = ofPoint(ofVec2f(20,-20) + ofPoint(mouseX, mouseY));
        //gon.addVertex(ofVec2f(-20,-20) + ofPoint(mouseX, mouseY));
        gon.setPhysics(3.0, 0.53, 0.1);
       
        gon.setup(box2d.getWorld(), a, b, c);
        gons.push_back(gon);
    }
    
    
    if (key == ' '){
        for (int i = 0; i < meshes.size(); i++){
            for (int j = 0; j < meshes[i].triangles.size(); j++){
                meshTriangle t = meshes[i].triangles[j];
                
                ofxBox2dTriangle gon;
                ofPoint a =  t.pts[0];
                ofPoint b =  t.pts[1];
                ofPoint c =  t.pts[2];
                
                ofPoint midPt = (a+b+c)/3;
                
                colors.push_back(icicles.getColor(midPt.x, midPt.y));
                gon.setPhysics(2.0, 0.53, 0.1);
                gon.setup(box2d.getWorld(), a, b, c);
                gons.push_back(gon);
                
            }
        }
        
        //make joints
        for (int i=0 ; i< gons.size(); i++) {
            for (int j=0; j<gons.size(); j++) {
                cout << " " << i << " " << j << endl;
                int edgeCheck = 0;
                int jointArea = 3;

                if (gons[i].initA.distance(gons[j].initA) < 1 ||
                    gons[i].initA.distance(gons[j].initB) < 1 ||
                    gons[i].initA.distance(gons[j].initC) < 1 ) edgeCheck++;
                if (gons[i].initB.distance(gons[j].initA) < 1 ||
                    gons[i].initB.distance(gons[j].initB) < 1 ||
                    gons[i].initB.distance(gons[j].initC) < 1 ) edgeCheck++;
                if (gons[i].initC.distance(gons[j].initA) < 1 ||
                    gons[i].initC.distance(gons[j].initB) < 1 ||
                    gons[i].initC.distance(gons[j].initC) < 1 ) edgeCheck++;
                
//                cout << edgeCheck << endl;
//                cout << i << "A:" << gons[i].initA.x << " | " << gons[i].initA.y <<endl;
//                cout << i << "B:" << gons[i].initB.x << " | " << gons[i].initB.y <<endl;
//                cout << i << "C:" << gons[i].initC.x << " | " << gons[i].initC.y <<endl;
//                cout << j << "A:" << gons[j].initA.x << " | " << gons[j].initA.y <<endl;
//                cout << j << "B:" << gons[j].initB.x << " | " << gons[j].initB.y <<endl;
//                cout << j << "C:" << gons[j].initC.x << " | " << gons[j].initC.y <<endl;
                
                if (edgeCheck > 1) makeJoint(gons[i], gons[j]);
                else cout << "any in common? " << edgeCheck << endl;
            }
            
            
            //makeJoint(gons[30], gons[31]);
            
            
//            if ( gons[i].getPosition().x < 20 ){
//                for ( int j= 0; j< ceilings.size(); j++){
//                    if (ofDist(gons[i].getPosition().x, gons[i].getPosition().y,
//                               ceilings[j].getPosition().x, ceilings[j].getPosition().y) < 50)  {
//                        makeJoint(gons[i], ceilings[j]);
//                        
//                        
//                    }
//                }
//                
            
        }
    }
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

