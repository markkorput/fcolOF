//
//  video.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-23.
//
//

#include "video.hpp"

using namespace fcol;

FCOL_SINGLETON_INLINE_IMPLEMENTATION_CODE(Video)

void Video::setupParams(){
    parameters.setName("Video");
    parameters.add(parUpdate.set("update", true));
    parameters.add(parDraw.set("draw", true));
    parameters.add(parSpeed.set("speed", 1.0, -10.0, 30.0));

    parSpeed.addListener(this, &Video::onSpeedChange);
}

//void Video::setup(){
//    
//}

bool Video::load(const string &path){
    if(player.load(path)){
        player.setSpeed(parSpeed);
        // player.setFrame(startFrame);
        // player.setLoopState(OF_LOOP_NORMAL);
        player.setVolume(0.0f);
        player.play();
        // cam.close();
        return true;
    }

    // player.closeMovie();
    // player.close();
    ofLogWarning() << "Failed to load video";
    // cam.initGrabber(800, 600);
    return false;
}

void Video::destroy(){
    if(player.isLoaded()){
        player.closeMovie();
    }

    player.close();
}

void Video::update(){
    if(!parUpdate) return;
    player.update();

    if(player.isFrameNew()){
        ofNotifyEvent(newFrameEvent, player);
    }
}

void Video::draw(){
    if(!parDraw) return;
    ofSetColor(255.0);
    player.draw(0,0);
}

void Video::onSpeedChange(float & speed){
    player.setSpeed(speed);
    ofLog() << "Player speed: " << player.getSpeed();
}