//
//  ofxBox2dRevoluteJoint.h
//  example-Joint
//
//  Created by Firm Read on 10/4/13.
//
//

#ifndef example_Joint_ofxBox2dRevoluteJoint_h
#define example_Joint_ofxBox2dRevoluteJoint_h
#pragma once
//based on Todd Vanderlin's ofxBox2dJoint class

#include "ofMain.h"
#include "Box2D.h"
#include "ofxBox2dUtils.h"
class ofxBox2dRevoluteJoint {
    
public:
    
    b2World * world;
    b2Joint * joint;
    bool alive;
    b2RevoluteJointDef jd;
    
    ofxBox2dRevoluteJoint() {
        world = NULL;
        alive = false;
    }
    
    void setWorld(b2World * w) {
        if(w == NULL) {
            ofLog(OF_LOG_NOTICE, "- box2d world needed -");
            return;
        }
        world = w;
        alive = true;
    }
    
    void destroyJoint() {
        world->DestroyJoint(joint);
        joint = NULL;
    }
    
    void addJoint(b2Body * a, b2Body * b, float stiffness=3.0f, float damping=0.6f, int Xoffset=0, int Yoffset=0 ) {
        
        if(world == NULL) {
            ofLog(OF_LOG_NOTICE, "- no joint added - box2d world needed -");
            return;
        }
        
        b2Vec2 anchor = a->GetWorldCenter();
        jd.Initialize(a, b, anchor);
        //need to override anchor set in Initialize() if you don't want the anchor to be in b2Body a's center
        //(due to reasons I can't figure out!)
        jd.localAnchor1.Set(Xoffset / OFX_BOX2D_SCALE, Yoffset / OFX_BOX2D_SCALE);
        
        jd.collideConnected = false;
        jd.lowerAngle     = -0.25 * TWO_PI;
        jd.upperAngle     = 0.25 * TWO_PI;
        jd.enableLimit    = true;
        jd.maxMotorTorque = 10.0;
        jd.motorSpeed     = 0.0;
        jd.enableMotor    = true;
        
        joint = world->CreateJoint(&jd);
    }
    
    void draw(int alpha=200) {
        
        if(!alive) return;
        
        ofEnableAlphaBlending();
        ofSetColor(255, 0, 255, alpha);
        
        b2Vec2 p1 = joint->GetAnchor1();
        b2Vec2 p2 = joint->GetAnchor2();
        
        p1 *= OFX_BOX2D_SCALE;
        p2 *= OFX_BOX2D_SCALE;
        ofLine(p1.x, p1.y, p2.x, p2.y);
        ofDisableAlphaBlending();
    }
};


#endif
