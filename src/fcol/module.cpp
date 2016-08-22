//
//  module.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-22.
//
//

#include "module.hpp"
#include "ofxCv.h"

using namespace fcol;

FCOL_SINGLETON_INLINE_IMPLEMENTATION_CODE(Module)

void Module::setupParams(){
    parameters.setName("fcol");
    parameters.add(autoSave.set("autoSave", true));
    parameters.add(webcamUpdate.set("webcamUpdate", true));
    parameters.add(webcamDraw.set("webcamDraw", true));
    parameters.add(webcamTracker.set("webcamTracker", true));
    parameters.add(drawFbo.set("drawFbo", true));
    parameters.add(saveImages.set("saveImages", false));
    // parameters.add(eyeDistance.set("eyeDistance", 32.0f, 0.0f, 100.0f));
    parameters.add(leftEyePos.set("leftEyePos", ofVec2f(100.0f, 100.0f), ofVec2f(-100.0f, -100.0f), ofVec2f(700.0f, 300.0f)));
    parameters.add(rightEyePos.set("rightEyePos", ofVec2f(500.0f, 100.0f), ofVec2f(-100.0f, -100.0f), ofVec2f(700.0f, 300.0f)));
    parameters.add(drawEyeCircles.set("drawEyeCircles", true));

    // modules::Mask::instance()->setupParams();
    //parameters.add(modules::Mask::instance()->parameters);
}

void Module::setup(){
    // GUI
    gui.setup(parameters);
    gui.loadFromFile("params.xml");
    
    // load/setup
    // modules::Mask::instance()->setup();

    cam.initGrabber(800, 600);
    fbo.allocate(600, 200);

    tracker.setup();
}

void Module::destroy(){
    if(autoSave){
        gui.saveToFile("params.xml");
    }
}

void Module::update(){
    if(webcamUpdate)
        cam.update();
    
    if(webcamTracker && cam.isFrameNew()){
        tracker.reset();
        if(tracker.update(ofxCv::toCv(cam))) {
            fbo.begin();
            ofClear(0.0f, 0.0f);
            drawEyer();
            fbo.end();
            
            if(saveImages){
                ofPixels pix;
                fbo.readToPixels(pix);
                pix.setImageType(OF_IMAGE_GRAYSCALE);
                string filename = "aligned/" + ofToString(tracker.getOrientation().y) + "_" + ofToString(ofGetFrameNum()) + ".tiff";
                ofSaveImage(pix, filename);
            }
        }
    }
}

void Module::drawEyer(){
    ofPushMatrix();
    // ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    ofVec2f leftEye = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE).getCentroid2D();
    ofVec2f rightEye = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE).getCentroid2D();
    float eyeDistance = leftEyePos->distance(rightEyePos);
    float curDistance = leftEye.distance(rightEye);
    float scale = eyeDistance / curDistance;
    float rotation = (rightEye - leftEye).angle(rightEyePos.get() - leftEyePos.get()); // (ofVec2f(1, 0));
    ofTranslate(leftEyePos.get() - leftEye * scale);
    //ofTranslate(-eyeDistance / 2, 0);
    ofScale(scale, scale);
    ofRotate(rotation);
    //ofTranslate(-leftEye.x, -leftEye.y);
    cam.draw(0,0);
    ofPopMatrix();
    
    if(drawEyeCircles){
        ofNoFill();
        ofSetColor(255,0,0, 100.0f);
        ofDrawCircle(leftEyePos.get(), 30);
        ofDrawCircle(rightEyePos.get(), 30);
    }
}

void Module::draw(){
    ofPushMatrix();
    ofScale(0.5f, 0.5f);

    if(webcamDraw){
        cam.draw(0, 0, cam.getWidth(), cam.getHeight());
        if(drawFbo){
            fbo.draw(cam.getWidth(), 0, fbo.getWidth(), fbo.getHeight());
        }
    } else {
        if(drawFbo)
            fbo.draw(0, 0, fbo.getWidth(), fbo.getHeight());
    }

    if(webcamTracker){
        tracker.getImageMesh().drawWireframe();
    }
    
    ofPopMatrix();

    gui.draw();
}

void Module::threadedFunction(){
}