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

namespace fcol {
namespace out {

    class EyeFile {

        FCOL_SINGLETON_INLINE_HEADER_CODE(EyeFile)
    
    public: // common methods

        // EyeFile(){}
        ~EyeFile(){ destroy(); }

        void setupParams();
        void setup(Collector* collector);
        void destroy();
    
    private: // methods
        
        void registerCallbacks(bool _register=true);
        void saveVideoFrame(ofVideoPlayer& player);

    private: // callbacks

        void onVideoFrameTrack(VideoFrameTracker& videoFrameTracker);

    public: // params
        
        ofParameterGroup parameters;
        ofParameter<bool> enabled;
        ofParameter<bool> saveVideoFrames;

    private: // attributes
        
        Collector* collector;

    }; // class EyeFile

} // namespace out
} // namespace fcol

#endif /* eye_file_hpp */
