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
    parameters.add(saveFrames.set("saveFrames", false));
    parameters.add(saveEyeCrops.set("saveEyeCrops", false));
}

void EyeFile::setup(Collector* collector, EyeCrop* eyeCrop){
    this->collector = collector ? collector : Collector::instance();
    this->eyeCrop = eyeCrop ? eyeCrop : EyeCrop::instance();

    startTimestamp =  ofToString(ofGetYear()) + ofToString(ofGetMonth()) + ofToString(ofGetDay()) + ofToString(ofGetHours()) + ofToString(ofGetMinutes()) + ofToString(ofGetSeconds());

    registerCallbacks();
}

void EyeFile::destroy(){
    registerCallbacks(false);
    this->collector = NULL;
    this->eyeCrop = NULL;
}

void EyeFile::registerCallbacks(bool _register){
    if(collector){
        if(_register){
            ofAddListener(collector->videoFrameTrackerEvent, this, &EyeFile::onVideoFrameTrack);
            ofAddListener(collector->baseVideoDrawsTrackerEvent, this, &EyeFile::onBaseVideoDrawsTracker);
        } else {
            ofRemoveListener(collector->videoFrameTrackerEvent, this, &EyeFile::onVideoFrameTrack);
            ofRemoveListener(collector->baseVideoDrawsTrackerEvent, this, &EyeFile::onBaseVideoDrawsTracker);
        }
    }

    if(eyeCrop){
        if(_register){
            ofAddListener(eyeCrop->videoEyeCropEvent, this, &EyeFile::onVideoEyeCrop);
            ofAddListener(eyeCrop->baseVideoDrawsEyeCropEvent, this, &EyeFile::onBaseVideoDrawsEyeCrop);
        } else {
            ofRemoveListener(eyeCrop->videoEyeCropEvent, this, &EyeFile::onVideoEyeCrop);
            ofRemoveListener(eyeCrop->baseVideoDrawsEyeCropEvent, this, &EyeFile::onBaseVideoDrawsEyeCrop);
        }
    }
}



void EyeFile::saveVideoFrame(ofVideoPlayer& player){
    string filename = ofFilePath::getBaseName(player.getMoviePath());
    ofSaveImage(player.getPixels(), "output/" + filename + "/f" + ofToString(player.getCurrentFrame()) + "_frame.png");
}

string EyeFile::saveEyeCrop(EyeCrop::VideoFrameEyeCrop& videoFrameEyeCrop){
    // read pixel dat from fbo
    ofPixels pix;
    videoFrameEyeCrop.fbo->readToPixels(pix);
    // get path of file
    string filename = ofFilePath::getBaseName(videoFrameEyeCrop.player->getMoviePath());
    filename = "output/" + filename + "/f" + ofToString(videoFrameEyeCrop.player->getCurrentFrame()) + "_eyecrop.png";
    // save
    ofSaveImage(pix, filename);
    return filename;
}


// video file frame tracked
void EyeFile::onVideoFrameTrack(VideoFrameTracker& videoFrameTracker){
    if(enabled && saveFrames){
        saveVideoFrame(*videoFrameTracker.player);
    }
}

// video file frame eyes cropped
void EyeFile::onVideoEyeCrop(EyeCrop::VideoFrameEyeCrop& videoFrameEyeCrop){
    if(enabled && saveEyeCrops){
        // save
        string filepath = saveEyeCrop(videoFrameEyeCrop);
        // notify
        ofNotifyEvent(eyeCropFileEvent, filepath, this);
    }
}

// webcam frame tracked
void EyeFile::onBaseVideoDrawsTracker(BaseVideoDrawsTracker& baseVideoDrawsTracker){
    if(enabled && saveFrames){
        ofSaveImage(baseVideoDrawsTracker.baseVideoDraws->getPixels(), "output/webcam/" + startTimestamp + "-" + ofToString(collector->getFrameCount()) + "_frame.png");
    }
}

// webcam frame eyes cropped
void EyeFile::onBaseVideoDrawsEyeCrop(EyeCrop::BaseVideoDrawsEyeCrop& crop){
    if(enabled && saveEyeCrops){
        // get pixel data from fbo
        ofPixels pix;
        crop.fbo->readToPixels(pix);
        // get path where to save to
        string path = "output/webcam/" + startTimestamp + "-" + ofToString(collector->getFrameCount()) + "_eyecrop.png";
        // save
        ofSaveImage(pix, path);
        // notify
        ofNotifyEvent(eyeCropFileEvent, path, this);
    }
}
