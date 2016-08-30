//
//  eye_file.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-30.
//
//

#include "eye_file.hpp"

using namespace fcol::out;

FCOL_SINGLETON_INLINE_IMPLEMENTATION_CODE(EyeFile)

void EyeFile::setupParams(){
    parameters.setName("EyeFile");
    parameters.add(enabled.set("enabled", false));
    parameters.add(saveVideoFrames.set("saveVideoFrames", false));
}

void EyeFile::setup(Collector* collector){
    this->collector = collector;

    if(!collector){
        this->collector = Collector::instance();
    }

    registerCallbacks();
}

void EyeFile::destroy(){
    registerCallbacks(false);
    this->collector = NULL;
}

void EyeFile::registerCallbacks(bool _register){
    if(_register){
        ofAddListener(this->collector->videoFrameTrackerEvent, this, &EyeFile::onVideoFrameTrack);
    } else {
        ofRemoveListener(this->collector->videoFrameTrackerEvent, this, &EyeFile::onVideoFrameTrack);
    }
}

void EyeFile::onVideoFrameTrack(VideoFrameTracker& videoFrameTracker){
    if(!enabled)
        return;

    if(saveVideoFrames){
        saveVideoFrame(*videoFrameTracker.player);
    }
}

void EyeFile::saveVideoFrame(ofVideoPlayer& player){
    string filename = ofFilePath::getBaseName(player.getMoviePath());
    ofSaveImage(player.getPixels(), "eyes/" + filename + "/f" + ofToString(player.getCurrentFrame()) + "_fullframe.tiff");
}