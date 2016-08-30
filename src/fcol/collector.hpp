//
//  collector.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-23.
//
//

#ifndef collector_hpp
#define collector_hpp

#include "shared.h"
#include "ofxFaceTracker.h"

namespace fcol {

    typedef struct {
        ofVideoPlayer* player;
        ofxFaceTracker* tracker;
    } VideoFrameTracker;

    class Collector {
        
        FCOL_SINGLETON_INLINE_HEADER_CODE(Collector)

    public: // common methods

        // Collector();
        ~Collector(){ destroy(); }
        
        void setupParams();
        void setup();
        void destroy();
        void update();
        void draw();

    public: // getter methods

        inline ofFbo& getFbo(){ return fbo; }
        inline ofxFaceTracker& getTracker(){ return tracker; }

    public: // operations methods

        void addFrame(ofVideoPlayer &player);
        void addFrame(ofVideoGrabber &cam);
    
    private: // methods

        void applyTrackerEyesMatrix();
        void saveFbo();
        void saveFbo(const string &filename);

    public: // events
        
        ofEvent<VideoFrameTracker> videoFrameTrackerEvent;

    public: // params

        ofParameterGroup parameters;
        ofParameter<bool> parUpdate;
        ofParameter<bool> parDraw;

        ofParameter<bool> saveImages;
        ofParameter<ofVec2f> leftEyePos;
        ofParameter<ofVec2f> rightEyePos;
        ofParameter<bool> drawEyeCircles;
        
    private: // attributes

        string startTimestamp;
        int frameCount;
        ofxFaceTracker tracker;
        ofFbo fbo;
    };
}

#endif /* collector_hpp */
