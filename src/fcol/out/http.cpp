//
//  http.cpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-31.
//
//

#include "http.hpp"

using namespace fcol::out;


FCOL_SINGLETON_INLINE_IMPLEMENTATION_CODE(Http)

void Http::setupParams(){
    parameters.setName("Http");
    parameters.add(enabled.set("enabled", false));
    parameters.add(sendEyeCropFiles.set("sendEyeCropFiles", false));
    parameters.add(eyeCropFileUrl.set("eyeCropFileUrl", "localhost:8080/fcol/eye_crop_file"));
    parameters.add(eyeCropFilePort.set("eyeCropFilePort", 8080, 0, 99999));
}

void Http::setup(EyeFile* eyeFile){
    this->eyeFile = eyeFile ? eyeFile : EyeFile::instance();
    registerCallbacks();
}

void Http::destroy(){
    registerCallbacks(false);
    this->eyeFile = NULL;
}

void Http::registerCallbacks(bool _register){
    if(eyeFile){
        if(_register){
            ofAddListener(eyeFile->eyeCropFileEvent, this, &Http::onEyeCropFile);
        } else {
            ofRemoveListener(eyeFile->eyeCropFileEvent, this, &Http::onEyeCropFile);
        }
    }
}

void Http::sendEyeCropFile(const string& localFilePath){
    ofLog() << "TODO: send file: " << localFilePath;
}


// video file frame tracked
void Http::onEyeCropFile(string& localPath){
    if(enabled && sendEyeCropFiles){
        sendEyeCropFile(localPath);
    }
}
