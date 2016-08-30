//
//  webcam.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-23.
//
//

#include "webcam.hpp"


using namespace fcol;

FCOL_SINGLETON_INLINE_IMPLEMENTATION_CODE(Webcam)

void Webcam::setupParams(){
    parameters.setName("Webcam");
    parameters.add(parUpdate.set("update", true));
    parameters.add(parDraw.set("draw", true));
}

void Webcam::setup(){
    cam.initGrabber(800, 600);
}

void Webcam::destroy(){
    if(cam.isInitialized()){
        cam.close();
    }
}

void Webcam::update(){
    if(!parUpdate) return;
    cam.update();

    if(cam.isFrameNew()){
        ofNotifyEvent(newFrameEvent, cam);
    }
}

void Webcam::draw(){
    if(!parDraw) return;
    ofSetColor(255.0);
    cam.draw(0,0);
}