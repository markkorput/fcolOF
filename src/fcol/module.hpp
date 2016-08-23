//
//  module.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-22.
//
//

#ifndef module_hpp
#define module_hpp

#include "shared.h"
#include "ofxGui.h"
#include "ofxFaceTracker.h"

namespace fcol {
    class Module /*: public ofThread*/ {

        FCOL_SINGLETON_INLINE_HEADER_CODE(Module)

    public:
        
        // Module();
        ~Module(){ destroy(); }
        
        void setupParams();
        void setup();
        void destroy();
        void update();
        void draw();

        //void threadedFunction();
        void dragEvent(ofDragInfo dragInfo);

    private: // methods
        
        void processTracker();
        void drawEyer();

    private: // callbacks
        void onNewVideoFrame(ofVideoPlayer & player);

    private: // params
        
        ofParameterGroup parameters;
        ofParameter<bool> autoSave;
        ofParameter<bool> updateSource;
        ofParameter<bool> drawSource;
        ofParameter<bool> drawFbo;
        ofParameter<bool> saveImages;
        ofParameter<bool> drawEyeCircles;
        ofParameter<int> startFrame;

        ofParameter<ofVec2f> leftEyePos;
        ofParameter<ofVec2f> rightEyePos;
        
        
    private: // gui
        
        ofxPanel gui;

    private: // attributes

        ofVideoGrabber cam;
        ofxFaceTracker tracker;
        ofFbo fbo;
    };
}

#endif /* module_hpp */
