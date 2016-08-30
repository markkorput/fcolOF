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
        ofBaseVideoDraws* baseVideoDraws;
        ofxFaceTracker* tracker;
    } BaseVideoDrawsTracker;

    typedef struct {
        ofVideoPlayer* player;
        ofxFaceTracker* tracker;
    } VideoFrameTracker;

    class Collector {
        
        FCOL_SINGLETON_INLINE_HEADER_CODE(Collector)

    public: // common methods

        // Collector();
        // ~Collector(){ destroy(); }

        void setupParams();
        void setup();
        // void destroy();
        // void update();
        // void draw();

    public: // getter methods

        inline ofxFaceTracker& getTracker(){ return tracker; }
        inline int getFrameCount() const { return frameCount; }

    public: // operations methods

        void addFrame(ofVideoPlayer &player);
        void addFrame(ofVideoGrabber &cam);

    public: // events
        
        ofEvent<BaseVideoDrawsTracker> baseVideoDrawsTrackerEvent;
        ofEvent<VideoFrameTracker> videoFrameTrackerEvent;

    public: // params

        ofParameterGroup parameters;
        ofParameter<bool> enabled;
        
    private: // attributes

        int frameCount;
        ofxFaceTracker tracker;
        ofFbo fbo;
    };
}

#endif /* collector_hpp */
