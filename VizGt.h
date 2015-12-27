#ifndef	GT_VISUALISATION_H
#define	GT_VISUALISATION_H

#include "TrackedObject.h"
#include "XmlLoader.h"

#define USE_MYSQL_INPUT
#ifdef USE_MYSQL_INPUT
#include "database/cvgDB.h"
#include "database/cvgDBFactory.h"
typedef struct db_connection_t{
	char* host_name;
	unsigned int port;
	char* user_name;
	char* pwd;
	char* db_name;
	char* table_name;	
}db_connection;

using namespace cvg_database;

#endif


namespace viz_gt {
	
	enum XML_FORMAT {
		VIPER,
		SUBITO,
		ISCAPS,
		AVITRACK,
		LEEDS,	//tracks file 
		UNKNOWN
	};
	
	class VizGt{
		string m_img_path;
		string m_img_name_base;
		string m_img_file_ext;
		string m_gt_xmlfile;
		string m_ar_xmlfile;
		int m_name_num_digit;
		string m_full_file_name;
		string m_tracks_file_path;	//track file for leeds bag detector
		
		string m_output_gt_in_subito_format;
		string m_output_ar_in_subito_format;
		string m_output_gt_in_viper_format;
		string m_output_ar_in_viper_format;
		
		bool m_display_gt_info;
		bool m_display_ar_info;
		
#ifdef USE_MYSQL_INPUT		
		bool m_use_mysql_source;	//read xml results from MySQL database instead of xml files	
		db_connection m_db_conn;
		cvgDBFactory* m_fac;
		cvgDB* m_db;
#endif
		
		int m_frame_counter;
		
//		IplImage* m_input_image;
//		char m_wnd_name_video[64];
//		int m_position;
//		int m_trackbar_init_value;
//		int m_trackbar_stop_value;
//		bool m_is_paused;
//		CvFont m_font;
		
		vector<TrackedObject*> m_tracked_objects_gt;
		vector<TrackedObject*> m_tracked_objects_ar;
		xml_loader::XmlLoader* m_xml_parser_gt;
		xml_loader::XmlLoader* m_xml_parser_ar;
		
		XML_FORMAT m_gt_format;
		XML_FORMAT m_ar_format;
		
		int m_first_effective_fid;
		int m_last_effective_fid;
		
	public:
		VizGt();
		~VizGt();
		
		void ParseConfigFile( char* config_file );
		void LoadDataFromXml();
		void LoadDataFromMysql();
		void DisplayImage();
		void DrawGtObjectsOntoImage();
		void DrawArObjectsOntoImage();	//AR: Algorithm Results
		
		
		///////// Utility Functions /////////////
		void GetFullFileName();
		void OutputGtTracks();	//with Leeds bag detector format
		void OutputGtToSubitoXml();
		
		void OutputGtToViperXml();
		void OutputArToViperXml();
		void GetCompleteFrameSpanGT( string& result );
		void GetCompleteFrameSpanAR( string& result );
		
#ifdef USE_MYSQL_INPUT
		bool ReadXMLSourceFromDB(){ return m_use_mysql_source; };
		void InitialiseDatabase();
#endif
		
		
	};
}

#endif