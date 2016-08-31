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
    parameters.add(deleteSentEyeCropFiles.set("deleteSentEyeCropFiles", false));
    parameters.add(eyeCropFileUrl.set("eyeCropFileUrl", "http://localhost:8080/fcol/eye_crop_file"));
}

void Http::setup(EyeFile* eyeFile){
    this->eyeFile = eyeFile ? eyeFile : EyeFile::instance();
    
    ofAddListener(formManager.formResponseEvent, this, &Http::onFormResponse);
    //formManager.setVerbose(true);

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
    HttpForm form = HttpForm(eyeCropFileUrl);
    // form.method = OFX_HTTP_POST;
    // form.addFormField("number", ofToString(counter));
    form.addFile("image", localFilePath);
    formManager.submitForm(form,
                           false, // don't ignore response
                           localFilePath); // identifier, required to process response

    ofLog() << "uploading: " << localFilePath << " to: " << eyeCropFileUrl;
}

// video file frame tracked
void Http::onEyeCropFile(string& localPath){
    if(enabled && sendEyeCropFiles){
        uploadEyeCropFile(localPath);
    }
}

void Http::onFormResponse(HttpFormResponse &response){
    string responseStr = ofToString(response.status) + ": " + (string)response.responseBody;

    // error?
    if(!response.ok){
        // log
        ofLogWarning() << "Http::onNewResponse -- " << responseStr;
        // abort
        return;
    }

    // log (verbose)
    ofLogVerbose() << "Http::onNewResponse -- " << responseStr;

    // delete uploaded file if necessary
    if(deleteSentEyeCropFiles){
        ofLog() << "removing uploaded file: " << response.identifier;
        ofFile::removeFile(response.identifier);
    }
}