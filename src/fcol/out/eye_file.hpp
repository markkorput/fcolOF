//
//  eye_file.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-30.
//
//

#ifndef eye_file_hpp
#define eye_file_hpp

#include "../shared.h"
#include "../collector.hpp"
#include "eye_crop.hpp"

namespace fcol {
namespace out {

    class EyeFile {

        FCOL_SINGLETON_INLINE_HEADER_CODE(EyeFile)
    
    public: // common methods

        // EyeFile(){}
        ~EyeFile(){ destroy(); }

        void setupParams();
        void setup(Collector* collector, EyeCrop* eyeCrop);
        void destroy();
    
    private: // methods
        
        void registerCallbacks(bool _register=true);
        static void saveVideoFrame(ofVideoPlayer& player);
        static void saveEyeCrop(EyeCrop::VideoFrameEyeCrop& videoFrameEyeCrop);

    private: // callbacks

        void onVideoFrameTrack(VideoFrameTracker& videoFrameTracker);
        void onNewEyeCrop(EyeCrop::VideoFrameEyeCrop& videoFrameEyeCrop);

    public: // params
        
        ofParameterGroup parameters;
        ofParameter<bool> enabled;
        ofParameter<bool> saveVideoFrames;
        ofParameter<bool> saveEyeCrops;
        

    private: // attributes
        
        Collector* collector;
        EyeCrop* eyeCrop;

    }; // class EyeFile

} // namespace out
} // namespace fcol

#endif /* eye_file_hpp */
