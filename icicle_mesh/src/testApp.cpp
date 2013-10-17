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
    icicles.loadImage("icicles_S2.png");
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
        //in this app mesh.size seems to always = 1
        meshes[meshes.size()-1].triangulate(line, -1, 10000000);
        }
    }
    
    
    
    
    
    
    // ceiling static rectangles
    for (int i=0; i < 10; i++) {
        ofxBox2dRect temp;
        ofRectangle rect = ofRectangle((i)*ofGetWidth()/10+ofGetWidth()/20, -10, ofGetWidth()/10-10, 14);
        temp.setup(box2d.getWorld(), rect);
        ceilings.push_back(temp);
    }
    
    
    //ofSetFrameRate(5);
    
    
    
}





void testApp::makeJoint(ofxBox2dBaseShape & shape1, ofxBox2dBaseShape & shape2){
    
    ofxBox2dJoint joint;
    joint.setup(box2d.getWorld(), shape1.body, shape2.body);
    
    joint.setDamping(10.f);
    joint.setLength(ofDist(shape1.getPosition().x, shape1.getPosition().y, shape2.getPosition().x, shape2.getPosition().y));
    joint.setFrequency(2.f);
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
    
    
//    for (int i=0; i<joints.size(); i++){
//        if (joints[i].getReactionForce(30).length()>300) {
//            joints[i].destroy();
//        }
//    }
    
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

    cout << joints.size() << endl;
    for(int i=0; i<joints.size(); i++) {
        ofSetHexColor(0xFFFFFF);
        joints[i].draw();
    }
    
    
	// draw the ground
	box2d.drawGround();
	
	
	
	string info = "";
	info += "Press [c] for circles\n";
	info += "Press [b] for blocks\n";
	info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
	info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
	info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
    info += "meshes.size(): " + ofToString(meshes.size())+"\n";
    info += "meshes[0].triangles.size(): " + ofToString(meshes[0].triangles.size())+"\n";
//    info += "meshes[1].triangles.size(): " + ofToString(meshes[1].triangles.size())+"\n";
	ofSetColor(ofColor::white);
	ofDrawBitmapString(info, 30, 30);

    //finder.draw();
    
    for (int i = 0; i < meshes.size(); i++){
//        meshes[i].draw();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	
	if(key == 'c') {
		float r = ofRandom(4, 20);		// a random radius 4px - 20px
		ofxBox2dCircle circle;
		circle.setPhysics(3.0, 0.53, 0.1);
		circle.setup(box2d.getWorld(), mouseX, mouseY, r);
		circles.push_back(circle);
	}
	
	if(key == 'b') {
		float w = ofRandom(4, 20);	
		float h = ofRandom(4, 20);	
		ofxBox2dRect rect;
		rect.setPhysics(3.0, 0.53, 0.1);
		rect.setup(box2d.getWorld(), mouseX, mouseY, w, h);
		boxes.push_back(rect);
	}
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
        // generating ofxBox2dTriangle from mesh
        
//        ofPoint prevA, prevB, prevC;
        
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
//                gon.body->SetType(b2_staticBody);
                gons.push_back(gon);
                
//
                //triangle made
//                int edgeCheck = 0;

                
/// condition type 1
//                if ( a == prevA || a == prevB || a == prevC) edgeCheck ++;
//                if ( b == prevA || b == prevB || b == prevC) edgeCheck ++;
//                if ( c == prevA || c == prevB || c == prevC) edgeCheck ++;
//
               
/// condition type 2
//                if (ofDist(a.x, a.y, prevA.x, prevA.y) < 10 ||
//                    ofDist(a.x, a.y, prevB.x, prevB.y) < 10 ||
//                    ofDist(a.x, a.y, prevC.x, prevC.y) < 10 ) edgeCheck++;
//                
//                if (ofDist(b.x, b.y, prevA.x, prevA.y) < 10 ||
//                    ofDist(b.x, b.y, prevB.x, prevB.y) < 10 ||
//                    ofDist(b.x, b.y, prevC.x, prevC.y) < 10 ) edgeCheck++;
//                
//                if (ofDist(c.x, c.y, prevA.x, prevA.y) < 10 ||
//                    ofDist(c.x, c.y, prevB.x, prevB.y) < 10 ||
//                    ofDist(c.x, c.y, prevC.x, prevC.y) < 10 ) edgeCheck++;
//                
//                
//                
//                if (edgeCheck > 1){
//                    makeJoint(gons[gons.size()-1].body, gons[gons.size()-2].body);
//                }
//                
//                prevA = a;
//                prevB = b;
//                prevC = c;
                
                
                
            }
        }
        
//        //make joints
//        for (int i=0 ; i< gons.size(); i++) {
//            for (int j=0; j<gons.size(); j++) {
//                if (i!=j){
//                    if (ofDist(gons[i].getPosition().x,
//                               gons[i].getPosition().y,
//                               gons[j].getPosition().x,
//                               gons[j].getPosition().y ) > 5)
//                        makeJoint(gons[i].body, gons[j].body);
//                    
//                    
//                    
//                }
//            }
//        }
        
        
        
        //make joints
        for (int i=0 ; i< gons.size(); i++) {
            for (int j=0; j<i; j++) {
                if ( ofDist(gons[i].getPosition().x, gons[i].getPosition().y, gons[j].getPosition().x, gons[j].getPosition().y) < 100){
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
                    
                    
//                    if (ofDist(gons[i].initA.x, gons[i].initA.y, gons[j].initA.x, gons[j].initA.y) < jointArea ||
//                        ofDist(gons[i].initA.x, gons[i].initA.y, gons[j].initB.x, gons[j].initB.y) < jointArea ||
//                        ofDist(gons[i].initA.x, gons[i].initA.y, gons[j].initC.x, gons[j].initC.y) < jointArea ) edgeCheck++;
//                    
//                    if (ofDist(gons[i].initB.x, gons[i].initB.y, gons[j].initA.x, gons[j].initA.y) < jointArea ||
//                        ofDist(gons[i].initB.x, gons[i].initB.y, gons[j].initB.x, gons[j].initB.y) < jointArea ||
//                        ofDist(gons[i].initB.x, gons[i].initB.y, gons[j].initC.x, gons[j].initC.y) < jointArea ) edgeCheck++;
//                    
//                    if (ofDist(gons[i].initC.x, gons[i].initC.y, gons[j].initA.x, gons[j].initA.y) < jointArea ||
//                        ofDist(gons[i].initC.x, gons[i].initC.y, gons[j].initB.x, gons[j].initB.y) < jointArea ||
//                        ofDist(gons[i].initC.x, gons[i].initC.y, gons[j].initC.x, gons[j].initC.y) < jointArea ) edgeCheck++;
                    
                    cout << edgeCheck << endl;
                    if (edgeCheck > 1) makeJoint(gons[i], gons[j]);
                    else {
                        cout << "any in common? " << edgeCheck << endl;
                    }
                }
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
//            }
            
        }

    }
    
    
	
	if(key == 't') ofToggleFullscreen();
    
    if (key=='q') box2d.update();
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

