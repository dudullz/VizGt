//although derived from XmlLoader, this class has nothing to do with xml files

#ifndef	TRACKS_LOADER_LEEDS_H
#define	TRACKS_LOADER_LEEDS_H

#include "XmlLoader.h"
#include "TrackedObject.h"

namespace xml_loader {
	class TracksLoader4LeedsBagDetector : public XmlLoader {
		ifstream m_in_file;
		string m_tracks_file_path;
		
		int m_num_object;
		int m_num_frames;
//		int m_min_x, m_min_y;
//		int m_max_x, m_max_y;
//		int m_width, m_height;
		
	public:
		TracksLoader4LeedsBagDetector( string& path );
		~TracksLoader4LeedsBagDetector();
		
		void LoadFromFile( const char* file_name );
		void ReadToTrackedObject( vector<TrackedObject*>& objects );		
		void GetVisibleElementAndIndex( vector<TrackedObject*>& objects );
		void GetFrameSpansForTrackedObject( vector<TrackedObject*>& objects );
		
		int GetTotalFrameNum(){ return m_num_frames; };
		void ParseForTrackedObject( string& line, TrackedObject* obj);
		void LoadTrackData( TrackedObject* obj );
	};
}


#endif