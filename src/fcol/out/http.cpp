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
    parameters.add(eyeCropFileUrl.set("eyeCropFileUrl", "http://localhost:8080/fcol/eye_crop_file"));
}

void Http::setup(EyeFile* eyeFile){
    this->eyeFile = eyeFile ? eyeFile : EyeFile::instance();
    
    ofAddListener(httpUtils.newResponseEvent,this, &Http::onNewResponse);
    httpUtils.start();
    httpUtils.setMaxRetries(2);

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

void Http::uploadEyeCropFile(const string& localFilePath){
    ofxHttpForm form;
    form.action = eyeCropFileUrl;
    form.method = OFX_HTTP_POST;
    // form.addFormField("number", ofToString(counter));
    form.addFile("image", localFilePath);
    httpUtils.addForm(form);
    ofLog() << "uploading: " << localFilePath << " to: " << eyeCropFileUrl;
}

// video file frame tracked
void Http::onEyeCropFile(string& localPath){
    if(enabled && sendEyeCropFiles){
        uploadEyeCropFile(localPath);
    }
}

void Http::onNewResponse(ofxHttpResponse & response){
    string responseStr = ofToString(response.status) + ": " + (string)response.responseBody;

    if(response.status == 200){
        ofLogVerbose() << "Http::onNewResponse -- " << responseStr;
        return;
    }

    ofLogWarning() << "Http::onNewResponse -- " << responseStr;
}