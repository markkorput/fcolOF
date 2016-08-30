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

namespace fcol {
    class Module {

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

    private: // callbacks
        void onNewVideoFrame(ofVideoPlayer & player);
        void onNewWebcamFrame(ofVideoGrabber & cam);
        void onVideoSpeedResetButtonPressed();

    private: // params
        
        ofParameterGroup parameters;
        ofParameter<bool> autoSave;

    private: // gui
        
        ofxPanel gui;
        ofxButton videoSpeedResetButton;
    };
}

#endif /* module_hpp */
