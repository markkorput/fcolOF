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
    parameters.add(enabled.set("enabled", true));
}

void Collector::setup(){
    tracker.setup();
    frameCount = 0;
}

void Collector::addFrame(ofVideoPlayer & player){
    tracker.reset();

    if(tracker.update(ofxCv::toCv(player))) {
        frameCount++;

        VideoFrameTracker vft;
        vft.player = &player;
        vft.tracker = &tracker;
        ofNotifyEvent(videoFrameTrackerEvent, vft, this);
    }
}

void Collector::addFrame(ofVideoGrabber &cam){
    tracker.reset();
    
    if(tracker.update(ofxCv::toCv(cam))) {
        frameCount++;

        BaseVideoDrawsTracker bvdTracker;
        bvdTracker.baseVideoDraws = &cam;
        bvdTracker.tracker = &tracker;
        ofNotifyEvent(baseVideoDrawsTrackerEvent, bvdTracker, this);

//        fbo.begin();
//        ofClear(0.0f, 0.0f);
//        ofPushMatrix();
//        applyTrackerEyesMatrix();
//        cam.draw(0,0);
//        ofPopMatrix();
//        fbo.end();
//
//        if(saveImages){
//            saveFbo("webcam/" + startTimestamp + "-" + ofToString(frameCount) + ".tiff");
//            frameCount++;
//        }
    }
}

//void Collector::applyTrackerEyesMatrix(){
//    // ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
//    ofVec2f leftEye = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE).getCentroid2D();
//    ofVec2f rightEye = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE).getCentroid2D();
//    float eyeDistance = leftEyePos->distance(rightEyePos);
//    float curDistance = leftEye.distance(rightEye);
//    float scale = eyeDistance / curDistance;
//    float rotation = (rightEye - leftEye).angle(rightEyePos.get() - leftEyePos.get()); // (ofVec2f(1, 0));
//    ofTranslate(leftEyePos.get());
//    //ofTranslate(-eyeDistance / 2, 0);
//    ofScale(scale, scale);
//    ofRotate(rotation);
//    ofTranslate(-leftEye);
//    //ofTranslate(-leftEye.x, -leftEye.y);
//}
//
//void Collector::saveFbo(){
//    saveFbo("f"+ofToString(ofGetFrameNum())+"_"+ofToString(tracker.getOrientation()) + ".tiff");
//}
//
//void Collector::saveFbo(const string &filename){
//    ofPixels pix;
//    fbo.readToPixels(pix);
//    //pix.setImageType(OF_IMAGE_GRAYSCALE);
//    ofSaveImage(pix, "eyes/"+filename);
//}