//
//  http.hpp
//  emptyExample
//
//  Created by Mark van de Korput on 16-08-31.
//
//

#ifndef http_hpp
#define http_hpp

#include "../shared.h"
#include "../collector.hpp"
#include "eye_file.hpp"

namespace fcol {
    namespace out {
        
        class Http {
            
            FCOL_SINGLETON_INLINE_HEADER_CODE(Http)
            
        public: // common methods
            
            // Http(){}
            ~Http(){ destroy(); }

            void setupParams();
            void setup(EyeFile* eyeFile);
            void destroy();
            
        private: // methods
            
            void registerCallbacks(bool _register=true);
            void sendEyeCropFile(const string& localFilePath);
            
        private: // callbacks

            void onEyeCropFile(string& localPath);

        public: // params
            
            ofParameterGroup parameters;
            ofParameter<bool> enabled;
            ofParameter<bool> sendEyeCropFiles;
            ofParameter<string> eyeCropFileUrl;
            ofParameter<int> eyeCropFilePort;
            
        private: // attributes

            EyeFile* eyeFile;
            
        }; // class Http
        
    } // namespace out
} // namespace fcol


#endif /* http_hpp */
