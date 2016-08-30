//
//  eye_crop.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-30.
//
//

#include "eye_crop.hpp"


using namespace fcol::out;

FCOL_SINGLETON_INLINE_IMPLEMENTATION_CODE(EyeCrop)

void EyeCrop::setupParams(){
    parameters.setName("EyeCrop");
    parameters.add(enabled.set("enabled", false));
    parameters.add(imageWidth.set("imageWidth", 600, 1, 2000));
    parameters.add(imageHeight.set("imageHeight", 200, 1, 2000));
    parameters.add(leftEyePos.set("leftEyePos", ofVec2f(150.0f, 100.0f), ofVec2f(-500.0f, -500.0f), ofVec2f(2500.0f, 2500.0f)));
    parameters.add(rightEyePos.set("rightEyePos", ofVec2f(450.0f, 100.0f), ofVec2f(-500.0f, -500.0f), ofVec2f(2500.0f, 2500.0f)));
}

void EyeCrop::setup(Collector* collector){
    this->collector = collector;
    
    if(!collector){
        this->collector = Collector::instance();
    }

    fbo.allocate(imageWidth, imageHeight);

    registerCallbacks();
}

void EyeCrop::destroy(){
    registerCallbacks(false);
    this->collector = NULL;
}

void EyeCrop::draw(){
    fbo.draw(0,0);
}

void EyeCrop::registerCallbacks(bool _register){
    if(collector){
        if(_register){
            ofAddListener(collector->videoFrameTrackerEvent, this, &EyeCrop::onVideoFrameTrack);
        } else {
            ofRemoveListener(collector->videoFrameTrackerEvent, this, &EyeCrop::onVideoFrameTrack);
        }
    }
}

void EyeCrop::onVideoFrameTrack(VideoFrameTracker& videoFrameTracker){
    if(!enabled)
        return;

    // crop
    drawCrop(*videoFrameTracker.player, *videoFrameTracker.tracker);
    
    // notify
    VideoFrameEyeCrop vfeCrop;
    vfeCrop.player = videoFrameTracker.player;
    vfeCrop.fbo = &fbo;
    ofNotifyEvent(newEyeCropEvent, vfeCrop, this);
}

void EyeCrop::drawCrop(ofVideoPlayer &player, ofxFaceTracker &tracker){
    fbo.begin();
    ofClear(0.0f, 0.0f);
    ofPushMatrix();
        applyTrackerEyesMatrix(tracker);
        player.draw(0,0);
    ofPopMatrix();
    fbo.end();
}

void EyeCrop::applyTrackerEyesMatrix(ofxFaceTracker &tracker){
    // ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    ofVec2f leftEye = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE).getCentroid2D();
    ofVec2f rightEye = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE).getCentroid2D();
    float eyeDistance = leftEyePos->distance(rightEyePos);
    float curDistance = leftEye.distance(rightEye);
    float scale = eyeDistance / curDistance;
    float rotation = (rightEye - leftEye).angle(rightEyePos.get() - leftEyePos.get()); // (ofVec2f(1, 0));
    ofTranslate(leftEyePos.get());

    ofScale(scale, scale);
    ofRotate(rotation);
    ofTranslate(-leftEye);
}
