#ifndef	XML_LOADER_SUBITO_H
#define	XML_LOADER_SUBITO_H

#include "XmlLoader.h"
#include "TrackedObject.h"

namespace xml_loader {
	class XmlLoader4Subito : public xml_loader::XmlLoader {
		xmlTextReaderPtr m_reader;
		xmlTextWriterPtr m_writer;
		const xmlChar* m_name, *m_value, *m_attri;
		
		bool m_start_entity_tag;
//		bool m_start_object_tag;
		
		int m_num_object;
		int m_num_frames;
		int m_current_fid;
		int m_previous_fid;
		int m_num_entity_in_current_frame;
		int m_min_x, m_min_y;
		int m_max_x, m_max_y;
		int m_width, m_height;
		int m_entity_id;
		int m_entity_index;
		bool m_is_entity_existed;
		
		vector<int> m_existing_ids;
		
	public:
		XmlLoader4Subito();
		~XmlLoader4Subito();
		
		void LoadFromFile( const char* file_name );
		void ReadToTrackedObject( vector<TrackedObject*>& objects );		
		void GetVisibleElementAndIndex( vector<TrackedObject*>& objects );
		void GetFrameSpansForTrackedObject( vector<TrackedObject*>& objects );
		
		int GetTotalFrameNum(){ return m_num_frames; };
	};
}


#endif