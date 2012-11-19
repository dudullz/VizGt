#ifndef	XML_LOADER_VIPER_H
#define	XML_LOADER_VIPER_H

#include "XmlLoader.h"
#include "TrackedObject.h"

namespace xml_loader {
	class XmlLoader4Viper : public xml_loader::XmlLoader {
		xmlTextReaderPtr m_reader;
		xmlTextWriterPtr m_writer;
		const xmlChar* m_name, *m_value, *m_attri;
		
		bool m_start_data_tag;
		bool m_start_file_tag;
		bool m_start_object_tag;
		bool m_start_attri_tag;
		bool m_is_name_attribute;
		bool m_is_index_attribute;
		bool m_is_location_attribute;
		bool m_is_numframes_attribute;
		
		bool m_is_viper_format_from_anna;
		int m_num_object;
		int m_num_frames;
		
	public:
		XmlLoader4Viper();
		~XmlLoader4Viper();
		
		void LoadFromFile( const char* file_name );
		void ReadToTrackedObject( vector<TrackedObject*>& objects );
		void GetFramespansForTrackedObject( char* str, TrackedObject* object );
		void GetFrameSpan( string& span, int& start, int&end );
		void GetVisibleElementNum( TrackedObject* object );
		
		int GetTotalFrameNum(){ return m_num_frames; };
	};
}


#endif