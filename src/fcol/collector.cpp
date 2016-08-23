//
//  collector.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-23.
//
//

#include "collector.hpp"
#include "ofxCv.h"

using namespace fcol;

FCOL_SINGLETON_INLINE_IMPLEMENTATION_CODE(Collector)

void Collector::setupParams(){
    parameters.setName("Collector");
    parameters.add(parUpdate.set("update", true));
    parameters.add(parDraw.set("draw", true));
    parameters.add(saveImages.set("saveImages", false));
    parameters.add(leftEyePos.set("leftEyePos", ofVec2f(100.0f, 100.0f), ofVec2f(-100.0f, -100.0f), ofVec2f(700.0f, 300.0f)));
    parameters.add(rightEyePos.set("rightEyePos", ofVec2f(500.0f, 100.0f), ofVec2f(-100.0f, -100.0f), ofVec2f(700.0f, 300.0f)));
    parameters.add(drawEyeCircles.set("drawEyeCircles", true));
}

void Collector::setup(){
    tracker.setup();
    fbo.allocate(600, 200);
    
    startTimestamp =  ofToString(ofGetYear()) + ofToString(ofGetMonth()) + ofToString(ofGetDay()) + ofToString(ofGetHours()) + ofToString(ofGetMinutes()) + ofToString(ofGetSeconds());
    frameCount = 0;
}

void Collector::destroy(){
    
}

void Collector::update(){
    if(!parUpdate) return;
}

void Collector::draw(){
    if(!parDraw) return;
    
    ofSetColor(255.0);
    fbo.draw(0,0);
    
    if(drawEyeCircles){
        ofNoFill();
        ofSetColor(255,0,0, 100.0f);
        ofDrawCircle(leftEyePos.get(), 30);
        ofDrawCircle(rightEyePos.get(), 30);
    }
}

void Collector::addFrame(ofVideoPlayer & player){
    tracker.reset();

    if(tracker.update(ofxCv::toCv(player))) {
        fbo.begin();
        ofClear(0.0f, 0.0f);
        ofPushMatrix();
        applyTrackerEyesMatrix();
        player.draw(0,0);
        ofPopMatrix();
        fbo.end();

        if(saveImages){
            string filename = ofFilePath::getBaseName(player.getMoviePath());
            saveFbo(filename + "/f" + ofToString(player.getCurrentFrame()) + ".tiff");
            frameCount++;
        }
    }
}

void Collector::addFrame(ofVideoGrabber &cam){
    tracker.reset();
    
    if(tracker.update(ofxCv::toCv(cam))) {
        fbo.begin();
        ofClear(0.0f, 0.0f);
        ofPushMatrix();
        applyTrackerEyesMatrix();
        cam.draw(0,0);
        ofPopMatrix();
        fbo.end();

        if(saveImages){
            saveFbo("webcam/" + startTimestamp + "-" + ofToString(frameCount) + ".tiff");
            frameCount++;
        }
    }
}

void Collector::applyTrackerEyesMatrix(){
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
}

void Collector::saveFbo(){
    saveFbo("f"+ofToString(ofGetFrameNum())+"_"+ofToString(tracker.getOrientation()) + ".tiff");
}

void Collector::saveFbo(const string &filename){
    ofPixels pix;
    fbo.readToPixels(pix);
    pix.setImageType(OF_IMAGE_GRAYSCALE);
    ofSaveImage(pix, "eyes/"+filename);
}