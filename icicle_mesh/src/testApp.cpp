#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
	
	box2d.init();
	box2d.setGravity(0, 0);
//	box2d.createBounds();
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
        ofRectangle rect = ofRectangle((i)*ofGetWidth()/10+ofGetWidth()/20, -10, ofGetWidth()/10-10, 14);
        ceiling[i].setup(box2d.getWorld(), rect);
    }
    
    
    
    
}



//--------------------------------------------------------------
void testApp::contactStart(ofxBox2dContactArgs &e) {
	if(e.a != NULL && e.b != NULL) {
		if(e.a->GetType() == b2Shape::e_circle && e.b->GetType() == b2Shape::e_circle) {
        }
	}
}

//--------------------------------------------------------------
void testApp::contactEnd(ofxBox2dContactArgs &e) {
	if(e.a != NULL && e.b != NULL) {
		
	}
}


//--------------------------------------------------------------



void testApp::makeJoint(b2Body *body1, b2Body *body2){
    
    ofxBox2dJoint joint;
    joint.setup(box2d.getWorld(), body1, body2);
    
    joint.setDamping(10.f);
    joint.setLength(5);
    joint.setFrequency(2.f);
    joints.push_back(joint);
    
    
}

//--------------------------------------------------------------
void testApp::update() {
	box2d.update();
    
    //delete icicles those are not moving
    for (int i=0; i<gons.size(); i++) {
        if (gons[i].isActive) gons[i].addForce(ofVec2f(0, 1), 10);
        
        //out of screen
        if (gons[i].getPosition().x<0
            || gons[i].getPosition().x>ofGetWidth()
            || gons[i].getPosition().y> ofGetHeight()+100) {
            gons[i].destroy();
            gons.erase(gons.begin()+i);
        }
        //stop moving
        else if (gons[i].getVelocity().length() < .2 && gons[i].isActive){
            gons[i].destroy();
            gons.erase(gons.begin()+i);
        }

    }
    //delete circles those are not moving
	for(int i=0; i<circles.size(); i++) {
        if (circles[i].getPosition().x<0
            || circles[i].getPosition().x>ofGetWidth()
            || circles[i].getPosition().y> ofGetHeight()+100) {
            circles[i].destroy();
            circles.erase(circles.begin()+i);
            
        }
        
        else if (circles[i].getVelocity().length() < .2){
            circles[i].destroy();
            circles.erase(circles.begin()+i);
        }
	}
	//delete rectangles those are not moving
	for(int i=0; i<boxes.size(); i++) {
        if (boxes[i].getPosition().x<0
            || boxes[i].getPosition().x>ofGetWidth()
            || boxes[i].getPosition().y> ofGetHeight()+100) {
            boxes[i].destroy();
            boxes.erase(boxes.begin()+i);
            
        }
        
        else if (boxes[i].getVelocity().length() < .2){
            boxes[i].destroy();
            boxes.erase(boxes.begin()+i);
        }
	}
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
    for(int i=0; i<10; i++) {
		ofFill();
		ofSetColor(100+(i*10));
		ceiling[i].draw();
	}
    
    
    for(int i=0; i<gons.size(); i++) {
		ofFill();
		ofSetColor(colors[i]);
        if(i==0) ofSetColor(ofColor::salmon);
		gons[i].draw();
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
                gons.push_back(gon);
                
                
                //triangle made
                
//                int edgeCheck = 0;
//                
//                if ( a == prevA || a == prevB || a == prevC) edgeCheck ++;
//                if ( b == prevA || b == prevB || b == prevC) edgeCheck ++;
//                if ( c == prevA || c == prevB || c == prevC) edgeCheck ++;
//                
//                if (edgeCheck > 1){
//                    makeJoint(gons., <#b2Body *body2#>)
//                }
            }
        }
        
        // compare two triangles to form joints
        
        
    }
    
    
	
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

