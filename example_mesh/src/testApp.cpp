#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
	
	box2d.init();
	box2d.setGravity(0, 10);
	box2d.createBounds();
	box2d.setFPS(30.0);
	box2d.registerGrabbing();
	
    icicles.loadImage("icicles.png");
    image.allocate(icicles.width, icicles.height);
    
    unsigned char * pixa = icicles.getPixels();
    unsigned char * pixb = image.getPixels();
    
    for (int i = 0; i < icicles.width * icicles.height; i++){
        
        pixb[i] = pixa[i*4+3];
    }
    image.flagImageChanged();
    
    finder.findContours(image, 0, 1000000, 10, false);
    
    for (int i = 0; i < finder.nBlobs; i++){
        
        if (i == 0){
        ofxTriangleMesh mesh;
        meshes.push_back(mesh);
        ofPolyline line;
        line.addVertices(finder.blobs[i].pts);
        meshes[meshes.size()-1].triangulate(line, -1, 10000);
        }
    }
    
}

//--------------------------------------------------------------
void testApp::update() {
	box2d.update();	
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
    
    for(int i=0; i<gons.size(); i++) {
		ofFill();
		ofSetColor(colors[i]);
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
	ofSetColor(ofColor::white);
	ofDrawBitmapString(info, 30, 30);

    //finder.draw();
    
    for (int i = 0; i < meshes.size(); i++){
        //meshes[i].draw();
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

