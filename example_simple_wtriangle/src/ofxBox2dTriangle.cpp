/*
 *  ofxBox2dTriangle.cpp
 *  ofxBox2dExample
 *
 *  Created by Todd Vanderlin on 3/7/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "ofxBox2dTriangle.h"



//------------------------------------------------

ofxBox2dTriangle::ofxBox2dTriangle() {
}

//------------------------------------------------
void ofxBox2dTriangle::setup(b2World * b2dworld,vector < ofPoint > pts) {
	//setup(b2dworld, rec.x, rec.y, rec.width, rec.height);
}

ofPolyline& ofxBox2dTriangle::getTriangleShape(){
    
    
    if(isBody()) {

        shape.clear();
        const b2Transform& xf = body->GetTransform();

        for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
            b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
            if(poly) {
                for(int i=0; i<poly->m_vertexCount; i++) {
                    b2Vec2 pt = b2Mul(xf, poly->m_vertices[i]);
                    shape.addVertex(worldPtToscreenPt(pt));
                }
            }
        }
    }
    // we are a rectangle so close it
    shape.setClosed(true);
    return shape;
    
}

//------------------------------------------------
void ofxBox2dTriangle::setup(b2World * b2dworld, ofPoint a, ofPoint b, ofPoint c) {
	
	if(b2dworld == NULL) {
		ofLog(OF_LOG_NOTICE, "- must have a valid world -");
		return;
	}
	
    
    /*
     
     http://stackoverflow.com/questions/9473570/polygon-vertices-clockwise-or-counterclockwise
     
     Select two edges with a common vertex.
     Lets say, edge1 is between vertex A and B. Edge2 is between vertex B and C.
     Define to vectors: vect1: A----->B
     vect2: B----->C
     Cross product vect1 and vect2.
     If the result is positive, the sequence A-->B-->C is Counter-clockwise.
     If the result is negative, the sequence A-->B-->C is clockwise.
     
     */
    
    //cout << (b-a).cross(c-b) << endl;
    
    // swap if we are not in the right order: 
    if ((b-a).cross(c-b).z < 0){
        ofPoint temp = c;
        c = a;
        a = temp;
    }
   
    
    
    ofPoint midPt = (a + b + c)/ 3.0;
	
    a -= midPt;
    b -= midPt;
    c -= midPt;
	
	b2PolygonShape shape;
    
    b2Vec2 pts[3];
    pts[0].x = a.x / OFX_BOX2D_SCALE;
    pts[0].y = a.y / OFX_BOX2D_SCALE;
    pts[1].x = b.x / OFX_BOX2D_SCALE;
    pts[1].y = b.y / OFX_BOX2D_SCALE;
    pts[2].x = c.x / OFX_BOX2D_SCALE;
    pts[2].y = c.y / OFX_BOX2D_SCALE;
   
	shape.Set(pts, 3);
    //SetAsBox(w/OFX_BOX2D_SCALE, h/OFX_BOX2D_SCALE);
	
	fixture.shape		= &shape;
	fixture.density		= density;
	fixture.friction	= friction;
	fixture.restitution = bounce;
	
	b2BodyDef bodyDef;
	if(density == 0.f)
		bodyDef.type	= b2_staticBody;
	else
		bodyDef.type	= b2_dynamicBody;
    
    cout << b2_dynamicBody << endl;
	bodyDef.position.Set(midPt.x/OFX_BOX2D_SCALE, midPt.y/OFX_BOX2D_SCALE);
	
	
	body = b2dworld->CreateBody(&bodyDef);
	body->CreateFixture(&fixture);

    
    // update the rectShape
    getTriangleShape();
    
}

////------------------------------------------------
//ofPolyline& ofxBox2dTriangle::getRectangleShape() {
//    
//    if(isBody()) {
//        
//        shape.clear();
//        const b2Transform& xf = body->GetTransform();
//        
//        for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
//            b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
//            if(poly) {
//                for(int i=0; i<poly->m_vertexCount; i++) {
//                    b2Vec2 pt = b2Mul(xf, poly->m_vertices[i]);
//                    shape.addVertex(worldPtToscreenPt(pt));
//                }
//            }
//        }
//    }
//    // we are a rectangle so close it
//    shape.setClosed(true);
//    return shape;
//    
//}

////------------------------------------------------
//float ofxBox2dTriangle::getWidth() {
//	return width;
//}
//
////------------------------------------------------
//float ofxBox2dTriangle::getHeight() {
//	return height;
//}

//------------------------------------------------
void ofxBox2dTriangle::addRepulsionForce(float x, float y, float amt) {
	addRepulsionForce(ofVec2f(x, y), amt);
}

//------------------------------------------------
void ofxBox2dTriangle::addRepulsionForce(ofVec2f pt, float amt) {
	if(body != NULL) {
		const b2Transform& xf = body->GetTransform();
		for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
			b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
			
			if(poly) {
				b2Vec2 P(pt.x/OFX_BOX2D_SCALE, pt.y/OFX_BOX2D_SCALE);
				for (int i=0; i<poly->GetVertexCount(); i++) {
					b2Vec2 qt = b2Mul(xf, poly->GetVertex(i));
					b2Vec2 D = P - qt; 
					b2Vec2 F = amt * D;
					body->ApplyForce(-F, P);
				}                        
			}
		}
	}
}


//------------------------------------------------
// In ofxBox2dTriangle.h:
// We compute the force for all four (transformed) corners of the rect.
// This keeps the rect's orientation correct!
void ofxBox2dTriangle::addAttractionPoint (float x, float y, float amt) {
	addAttractionPoint(ofVec2f(x, y), amt);
}

//------------------------------------------------
void ofxBox2dTriangle::addAttractionPoint (ofVec2f pt, float amt) {
	
	if(body != NULL) {
		const b2Transform& xf = body->GetTransform();
		for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
			b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
			
			if(poly) {
				b2Vec2 P(pt.x/OFX_BOX2D_SCALE, pt.y/OFX_BOX2D_SCALE);
				for (int i=0; i<poly->GetVertexCount(); i++) {
					b2Vec2 qt = b2Mul(xf, poly->GetVertex(i));
					b2Vec2 D = P - qt; 
					b2Vec2 F = amt * D;
					body->ApplyForce(F, P);
				}                        
			}
		}
	}
}

//------------------------------------------------
void ofxBox2dTriangle::draw() {
	
	if(body == NULL) {
		return;	
	}
    
    
    // update the polyline
    getTriangleShape();
    
    ofPath path;
    for (int i=0; i<shape.size(); i++) {
        if(i==0)path.moveTo(shape[i]);
        else path.lineTo(shape[i]);
    }
    
    // draw the path
    path.setColor(ofGetStyle().color);
    path.setFilled(ofGetStyle().bFill);
    path.draw();
    
    // are we sleeping
    if(isSleeping()) {
        ofPushStyle();
        ofEnableAlphaBlending();
        path.setColor(ofColor(255, 100));
        path.setFilled(true);
        path.draw();
        ofPopStyle();
    }
	
}
















