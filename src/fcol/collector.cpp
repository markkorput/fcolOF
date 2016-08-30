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
    if(!enabled) return;

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
    if(!enabled) return;

    tracker.reset();
    
    if(tracker.update(ofxCv::toCv(cam))) {
        frameCount++;

        BaseVideoDrawsTracker bvdTracker;
        bvdTracker.baseVideoDraws = &cam;
        bvdTracker.tracker = &tracker;
        ofNotifyEvent(baseVideoDrawsTrackerEvent, bvdTracker, this);
    }
}