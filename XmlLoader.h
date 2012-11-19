/*
 Vitual class to load xml files
*/

#ifndef	XML_LOADER_H
#define	XML_LOADER_H

#include <vector>
#include "TrackedObject.h"

using namespace std;


namespace xml_loader {
	class XmlLoader{		
	public:
		XmlLoader();
		~XmlLoader();
		
		virtual void LoadFromFile( const char* file_name ) = 0;
		virtual void ReadToTrackedObject( vector<TrackedObject*>& objects ) = 0;
		virtual int GetTotalFrameNum() = 0;
		
	};
}


#endif