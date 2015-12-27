
#include "VizGt.h"
#include "XmlLoader4Viper.h"
#include "XmlLoader4Subito.h"
#include "XmlLoader4Iscaps.h"
#include "TracksLoader4LeedsBagDetector.h"
using namespace xml_loader;

string img_path;
string img_name_base;
string img_file_ext;
string xmlfile_viper;
int name_num_digit;
string full_file_name;

IplImage* input_image;
char wnd_name_video[64];
int curr_position;
int trackbar_init_value = 0;
int trackbar_stop_value = 0;
bool is_paused = false;
CvFont font_frame;
CvFont font_gt;
CvFont font_ar;

vector<TrackedObject*> tracked_objects_gt;
vector<TrackedObject*> tracked_objects_ar;

bool display_gt_info;
bool display_ar_info;

void DrawGtObjectsOntoImage4Trackbar(){
	cout << "	DrawGtObjectsOntoImage4Trackbar "<< endl;
	int objs_num = tracked_objects_gt.size();
	cout << "Total number of GT objects:" << objs_num << endl;
	for (int i = 0; i < objs_num; ++i) {
		TrackedObject* cur_object = tracked_objects_gt.at(i);
//		cout << "Check Object:" << i << endl;
		if( cur_object->m_visible_index.at(curr_position) )
		{
// 			if( cur_object->m_id == 7 || cur_object->m_id == 6 )
// 				continue;
//			cout << "	Draw at Frame:" << curr_position << endl;
			int elem_idx = cur_object->m_elements_index.at(curr_position);
			
			if( elem_idx == -1 )
			{
				cout << BASH_ESC_RED;
				cout << " The element of Object " << cur_object->m_id << " at Frame " << curr_position << " DOES NOT exist!!!" << endl;
				cout << BASH_ESC_WHITE;
				continue;
			}
			
			Element elm = cur_object->m_elements.at( elem_idx );
			CvPoint pt1, pt2;
			pt1.x = elm.x;		pt1.y = elm.y;
			pt2.x = elm.x + elm.width;	pt2.y = elm.y + elm.height;
			char text[16];
			sprintf(text, "id:%d", cur_object->m_id);
			
			if( elm.m_type.find("BAGGAGE") != string::npos ){				
				cvRectangle(input_image, pt1, pt2, BAG_COLOUR, 2);
				cvPutText(input_image, text, pt2, &font_gt, BAG_COLOUR);
			}
			else{
				cvRectangle(input_image, pt1, pt2, GT_COLOUR, 2);
				cvPutText(input_image, text, pt2, &font_gt, GT_COLOUR);
			}
		}
	}
	
	cvShowImage( wnd_name_video, input_image );
}
void DrawArObjectsOntoImage4Trackbar(){
	cout << "	DrawArObjectsOntoImage4Trackbar "<< endl;
	int objs_num = tracked_objects_ar.size();
	cout << "Total number of AR objects:" << objs_num << endl;
	for (int i = 0; i < objs_num; ++i) {
		TrackedObject* cur_object = tracked_objects_ar.at(i);
//		cout << "Check Object:" << i << endl;
		if( cur_object->m_visible_index.at(curr_position) )
		{
// 			if( cur_object->m_id == 7 || cur_object->m_id == 6 )
// 				continue;
//			cout << "	Draw at Frame:" << curr_position << endl;
			int elem_idx = cur_object->m_elements_index.at(curr_position);
			Element elm = cur_object->m_elements.at( elem_idx );
			CvPoint pt1, pt2;
			pt1.x = elm.x;		pt1.y = elm.y;
			pt2.x = elm.x + elm.width;	pt2.y = elm.y + elm.height;
			char text[16];
			sprintf(text, "id:%d", cur_object->m_id);
			
// 			if( elm.m_type.find("BAGGAGE") != string::npos && cur_object->m_id == 0)
			if( elm.m_type.find("BAGGAGE") != string::npos )
			{
				cvRectangle(input_image, pt1, pt2, BAG_COLOUR, 2);
				cvPutText(input_image, text, pt1, &font_ar, BAG_COLOUR);
			}
			else{				
				cvRectangle(input_image, pt1, pt2, CV_RGB(255,255,0), 3);
				cvRectangle(input_image, pt1, pt2, CV_RGB(0,255,0), 1);
				
// 				cvRectangle(input_image, pt1, pt2, AR_COLOUR, 2);
				cvPutText(input_image, text, pt1, &font_ar, AR_COLOUR);
			}
		}
	}
	
// 	char save_path[256];
// 	sprintf(save_path, "/media/Data/subito/DetectionResults/pets06s1/cam3/%04d.jpg", curr_position);
// 	cvSaveImage(save_path, input_image);
// 	cout << "Save results to " << save_path << endl;

	cvShowImage( wnd_name_video, input_image );
}
void GetFullFileName4Trackbar( int no ){
	char idx[64];
	switch (name_num_digit) {
		case 4:
			sprintf(idx, "%04d.", no); 
			break;
		case 5:
			sprintf(idx, "%05d.", no); 
			break;
		case 6:
			sprintf(idx, "%06d.", no); 
			break;
			
		default:
			break;
	}
	
	if( img_name_base.size() > 0 )
		full_file_name = img_path + img_name_base + idx + img_file_ext;
	else
		full_file_name = img_path + idx+ img_file_ext;
	
	cout << BASH_ESC_PURPLE << full_file_name << BASH_ESC_WHITE << endl;
}
void onVideoTrackbar( int position ){
	cout << "onVideoTrackbar" << endl;
	is_paused = true;
	curr_position = position;
	cout << "Current Position:" << curr_position << endl;
	
	GetFullFileName4Trackbar( curr_position );
	char text[64];
	sprintf( text, "Frame %d", curr_position );
	
	input_image = cvLoadImage( full_file_name.c_str() );		
	if(!input_image){
		printf("Could not load image file: %s\n", full_file_name.c_str());
		return;
	}
	CvPoint pt;
	pt.x = input_image->width / 3;
	pt.y = 50;
	
	CvFont font_bg, font_fg;
	cvInitFont(&font_bg, CV_FONT_HERSHEY_DUPLEX, 1, 1, 0, 3);
	cvPutText(input_image, text, pt, &font_bg, CV_RGB(255,255,255));
	cvInitFont(&font_fg, CV_FONT_HERSHEY_DUPLEX, 1, 1, 0, 1);
	cvPutText(input_image, text, pt, &font_fg, CV_RGB(0,0,255));
	
	if( display_gt_info )
		DrawGtObjectsOntoImage4Trackbar();
	if( display_ar_info )
		DrawArObjectsOntoImage4Trackbar();
	
// 		if( curr_position == 820 )
// 			cvSaveImage("ilids-AB_Hard.jpg", input_image );
		
	cvReleaseImage( &input_image);
	input_image = NULL;
	cout << endl;
}


viz_gt::VizGt::VizGt(){
	m_frame_counter = 0;
	m_display_gt_info = false;
	m_display_ar_info = false;
#ifdef USE_MYSQL_INPUT
	m_use_mysql_source = false;
#endif
	m_gt_format = UNKNOWN;
	m_ar_format = UNKNOWN;
	
//	m_trackbar_init_value = 0;
//	m_trackbar_stop_value = 0;
//	m_is_paused = false;	
//	m_input_image = NULL;
	cvInitFont(&font_frame, CV_FONT_HERSHEY_DUPLEX, 1, 1, 0, 1);
	cvInitFont(&font_gt, CV_FONT_HERSHEY_PLAIN, 1, 1, 0, 1);	
	cvInitFont(&font_ar, CV_FONT_HERSHEY_PLAIN, 1, 1, 0, 1);	
	sprintf(wnd_name_video, "%s", "Video");	
	cvNamedWindow(wnd_name_video);	
}
viz_gt::VizGt::~VizGt(){
	cvDestroyWindow(wnd_name_video);
	
	if(input_image)
		cvReleaseImage(&input_image);
}
void viz_gt::VizGt::ParseConfigFile( char* config_file)
{
//	cout << "ParseConfigFile" << endl;
	ifstream confFile;
	confFile.open( config_file );
	
	if(!confFile.is_open() )
	{
		cout << BASH_ESC_RED << "Could not open config file: " << BASH_ESC_YELLOW << "config/vizConf" << BASH_ESC_WHITE << endl;
		throw("yikes!");
	}
	
	string command;
	string tmp;
	while( !confFile.eof() )
	{
		int count = 0;
		confFile >> command;
		cout << command << endl;
		// skip comments
		if( command.find('#') == 0 )
		{
			char c = ' ';
			// 			while( (c != '\n') && (c != EOF ) )
			while( (c != '\n') && (!confFile.eof()) )
			{
				confFile.get(c);
				// 				cout << count++ << ":" << c << endl;
				// 				if( count > 120 ) exit(-1);
			}
		}
		
		//
		if(command.find("IMAGE_PATH") != std::string::npos )
		{
			confFile >> m_img_path;
		}
		
		if(command.find("IMAGE_NAME_BASE") != std::string::npos )
		{
			confFile >> m_img_name_base;
			if( m_img_name_base.find("NULL") != string::npos )
				m_img_name_base = "";
		}
		
		if(command.find("NUM_DIGIT_IN_NAME") != std::string::npos )
		{
			confFile >> m_name_num_digit;
		}
		
		
		if(command.find("IMAGE_FILE_EXT") != std::string::npos )
		{
			confFile >> m_img_file_ext;
			
		}
		
		if(command.find("GT_XMLFILE") != std::string::npos )
		{
			confFile >> m_gt_xmlfile;
		}
		
		if(command.find("AR_XMLFILE") != std::string::npos )
		{
			confFile >> m_ar_xmlfile;
		}
		
		if(command.find("TRACKS_FILE_PATH") != std::string::npos )
		{
			confFile >> m_tracks_file_path;
		}
		
		if(command.find("OUTPUT_GT_IN_SUBITO_FORMAT") != std::string::npos )
		{
			confFile >> m_output_gt_in_subito_format;
		}
		
		if(command.find("OUTPUT_AR_IN_SUBITO_FORMAT") != std::string::npos )
		{
			confFile >> m_output_ar_in_subito_format;
		}
		
		if(command.find("OUTPUT_GT_IN_VIPER_FORMAT") != std::string::npos )
		{
			confFile >> m_output_gt_in_viper_format;
		}
		
		if(command.find("OUTPUT_AR_IN_VIPER_FORMAT") != std::string::npos )
		{
			confFile >> m_output_ar_in_viper_format;
		}
		
		if(command.find("GT_XML_FORMAT") != std::string::npos )
		{
			string format;
			confFile >> format;
			if(format.find("VIPER") != std::string::npos)
				m_gt_format = VIPER;
			else if(format.find("SUBITO") != std::string::npos)
				m_gt_format = SUBITO;
			else if(format.find("ISCAPS") != std::string::npos)
				m_gt_format = ISCAPS;
			else if(format.find("AVITRACK") != std::string::npos)
				m_gt_format = AVITRACK;
			else if(format.find("LEEDS") != std::string::npos)
				m_gt_format = LEEDS;
			else{
				cout << "Can not determine GT file format. Quitting now... "<< endl;
				exit(0);
			}
			
		}
		
		if(command.find("AR_XML_FORMAT") != std::string::npos )
		{
			string format;
			confFile >> format;
			if(format.find("VIPER") != std::string::npos)
				m_ar_format = VIPER;
			else if(format.find("SUBITO") != std::string::npos)
				m_ar_format = SUBITO;
			else if(format.find("ISCAPS") != std::string::npos)
				m_ar_format = ISCAPS;
			else if(format.find("AVITRACK") != std::string::npos)
				m_ar_format = AVITRACK;
			else{
				cout << "Can not determine AR file format. Quitting now... "<< endl;
				exit(0);
			}
		}
		
		if(command.find("DISPLAY_GT_INFO") != std::string::npos )
		{
			m_display_gt_info = true;
			display_gt_info = true;
		}
		
		if(command.find("DISPLAY_AR_INFO") != std::string::npos )
		{
			m_display_ar_info = true;
			display_ar_info = true;
		}
		
		
#ifdef USE_MYSQL_INPUT
		if(command.find("READ_SOURCE_FROM_MYSQL") != std::string::npos )
		{
			m_use_mysql_source = true;
		}
		
		if(command.find("DB_HOST") != std::string::npos )
		{
			confFile >> tmp;
			m_db_conn.host_name = new char[ tmp.size() ];
			strcpy( m_db_conn.host_name, tmp.c_str() );
		}
								
		if(command.find("DB_PORT") != std::string::npos )
		{
			confFile >> m_db_conn.port;
		}
				
		if(command.find("DB_USER") != std::string::npos )
		{
			confFile >> tmp;
			m_db_conn.user_name = new char[ tmp.size() ];
			strcpy( m_db_conn.user_name, tmp.c_str() );
		}        
				
	    if(command.find("DB_PWD") != std::string::npos )
	    {
			confFile >> tmp;
			m_db_conn.pwd = new char[ tmp.size() ];
			strcpy( m_db_conn.pwd, tmp.c_str() );
	    }
					
		if(command.find("DB_NAME") != std::string::npos )
		{
			confFile >> tmp;
			m_db_conn.db_name = new char[ tmp.size() ];
			strcpy( m_db_conn.db_name, tmp.c_str() );
		}
					
		if(command.find("DB_TABLE") != std::string::npos )
		{
			confFile >> tmp;
			m_db_conn.table_name = new char[ tmp.size() ];
			strcpy( m_db_conn.table_name, tmp.c_str() );
		}
#endif

	}
#ifdef USE_MYSQL_INPUT
	if( m_use_mysql_source )
	{
		cout << BASH_ESC_YELLOW << "DB_HOST:\t\t" << BASH_ESC_RED << m_db_conn.host_name << endl;
		cout << BASH_ESC_YELLOW << "DB_PORT:	" << BASH_ESC_RED << m_db_conn.port << endl;
		cout << BASH_ESC_YELLOW << "DB_USER:	" << BASH_ESC_RED << m_db_conn.user_name << endl;
		cout << BASH_ESC_YELLOW << "DB_PWD:\t\t" << BASH_ESC_RED << m_db_conn.pwd << endl;
		cout << BASH_ESC_YELLOW << "DB_NAME:\t\t" << BASH_ESC_RED << m_db_conn.db_name << endl;
		cout << BASH_ESC_YELLOW << "DB_TABLE:\t\t" << BASH_ESC_RED << m_db_conn.table_name << endl;
	}
#endif
	cout << BASH_ESC_RED << "m_img_path:\t\t" << BASH_ESC_YELLOW << m_img_path << endl;
	cout << BASH_ESC_RED << "m_img_name_base:	" << BASH_ESC_YELLOW << m_img_name_base << endl;
	cout << BASH_ESC_RED << "m_name_num_digit:	" << BASH_ESC_YELLOW << m_name_num_digit << endl;
	cout << BASH_ESC_RED << "m_img_file_ext:\t\t" << BASH_ESC_YELLOW << m_img_file_ext << endl;
	cout << BASH_ESC_RED << "m_gt_xmlfile:\t\t" << BASH_ESC_YELLOW << m_gt_xmlfile << endl;
	switch (m_gt_format) {
		case VIPER:
			cout << BASH_ESC_PURPLE << "gt xml format:\t\t" << BASH_ESC_YELLOW << "VIPER" << endl;
			m_xml_parser_gt = new XmlLoader4Viper();
			break;
		case SUBITO:
			cout << BASH_ESC_PURPLE << "gt xml format:\t\t" << BASH_ESC_YELLOW << "SUBITO" << endl;
			m_xml_parser_gt = new XmlLoader4Subito();
			break;
		case ISCAPS:
			cout << BASH_ESC_PURPLE << "gt xml format:\t\t" << BASH_ESC_YELLOW << "ISCAPS" << endl;
			m_xml_parser_gt = new XmlLoader4Iscaps();
			break;
		case AVITRACK:
			cout << BASH_ESC_PURPLE << "gt xml format:\t\t" << BASH_ESC_YELLOW << "AVITRACK, but..." << endl;
			cout << "		Not implemented yet. So quit!" << endl;
			exit(0);
//			m_xml_parser_gt = new XmlLoader4Avitrack();
			break;
		case LEEDS:
			cout << BASH_ESC_PURPLE << "gt format:\t\t" << BASH_ESC_YELLOW << "Leeds Tracks" << endl;
			m_xml_parser_gt = new TracksLoader4LeedsBagDetector( m_tracks_file_path );
			break;
		case UNKNOWN:
			cout << "Can not determine GT file format. Quitting now... "<< endl;
			exit(0);
			
		default:
			break;
	}
	cout << BASH_ESC_RED << "m_ar_xmlfile:\t\t" << BASH_ESC_YELLOW << m_ar_xmlfile << endl;
	switch (m_ar_format) {
		case VIPER:
			cout << BASH_ESC_PURPLE << "ar xml format:\t\t" << BASH_ESC_YELLOW << "VIPER" << endl;
			m_xml_parser_ar = new XmlLoader4Viper();
			break;
		case SUBITO:
			cout << BASH_ESC_PURPLE << "ar xml format:\t\t" << BASH_ESC_YELLOW << "SUBITO" << endl;
			m_xml_parser_ar = new XmlLoader4Subito();
			break;
		case ISCAPS:
			cout << BASH_ESC_PURPLE << "ar xml format:\t\t" << BASH_ESC_YELLOW << "ISCAPS" << endl;
			m_xml_parser_ar = new XmlLoader4Iscaps();
			break;
		case AVITRACK:
			cout << BASH_ESC_PURPLE << "ar xml format:\t\t" << BASH_ESC_YELLOW << "AVITRACK, but..." << endl;
			cout << "		Not implemented yet. So quit!" << endl;
			exit(0);
//			m_xml_parser_ar = new XmlLoader4Avitrack();
			break;
		case UNKNOWN:
			cout << "Can not determine AR file format. Quitting now... "<< endl;
			exit(0);
			
		default:
			break;
	}
	
	cout << BASH_ESC_RED << "m_output_gt_in_subito_format:\t" << BASH_ESC_YELLOW << m_output_gt_in_subito_format << endl;
	cout << "full file length: " << m_output_gt_in_subito_format.size() << endl;
	
	cout << BASH_ESC_RED << "m_output_gt_in_viper_format:\t" << BASH_ESC_YELLOW << m_output_gt_in_viper_format << endl;
	cout << "full file length: " << m_output_gt_in_viper_format.size() << endl;
	cout << BASH_ESC_RED << "m_output_ar_in_viper_format:\t" << BASH_ESC_YELLOW << m_output_ar_in_viper_format << endl;
	cout << "full file length: " << m_output_ar_in_viper_format.size() << endl;
	
	
	img_path = m_img_path;
	img_name_base = m_img_name_base;
	img_file_ext = m_img_file_ext;
	name_num_digit = m_name_num_digit;
	
//	cout << boolalpha << endl;
//	cout << BASH_ESC_RED << "m_use_size_filter: " << BASH_ESC_YELLOW << m_use_size_filter << endl;
//	cout << noboolalpha << endl;

	GetFullFileName();
	cout << BASH_ESC_RED << "m_full_file_name:	" << BASH_ESC_YELLOW << m_full_file_name << endl;
	
	cout << BASH_ESC_WHITE << endl;
	//sleep(3);
}
void viz_gt::VizGt::LoadDataFromXml(){	
//	cout << "LoadDataFromXml" << endl;
	m_xml_parser_gt->LoadFromFile( m_gt_xmlfile.c_str()  );
	m_xml_parser_gt->ReadToTrackedObject( m_tracked_objects_gt );

	m_xml_parser_ar->LoadFromFile( m_ar_xmlfile.c_str()  );
	m_xml_parser_ar->ReadToTrackedObject( m_tracked_objects_ar );
	
	tracked_objects_gt = m_tracked_objects_gt;
	tracked_objects_ar = m_tracked_objects_ar;
//	cout << "done deal!!!!" << endl;
}

#ifdef USE_MYSQL_INPUT
void viz_gt::VizGt::LoadDataFromMysql(){
	InitialiseDatabase();
}

void viz_gt::VizGt::InitialiseDatabase()
{
  cout << "[VizGt]  Connect to database... " << endl;  
  printf("        Mysql parameters:       %s:%d:%s:%s:%s:%s\n", m_db_conn.host_name, m_db_conn.port, m_db_conn.user_name, m_db_conn.pwd, m_db_conn.db_name, m_db_conn.table_name);
  
  m_fac = new cvgDBFactory();  
  if( m_fac )
  {
    m_db = m_fac->createDB( mysql );
	
	/// for some reason, BAE's database does not like password
	if( strcmp(m_db_conn.pwd, "NULL") == 0 )
		m_db->connectDB(m_db_conn.host_name, m_db_conn.user_name, "", m_db_conn.port, m_db_conn.db_name);
	else
		m_db->connectDB(m_db_conn.host_name, m_db_conn.user_name, m_db_conn.pwd, m_db_conn.port, m_db_conn.db_name);
                
    char tmp[64];
    sprintf(tmp, "delete from %s", m_db_conn.table_name);
    m_db->execute(tmp);
                
    //we don't need factory anymore
    delete m_fac;
  }
}
#endif

void viz_gt::VizGt::DisplayImage(){
	cout << "DisplayImage" << endl;
	trackbar_stop_value = m_xml_parser_ar->GetTotalFrameNum();
	cvCreateTrackbar( "Frame", wnd_name_video, &trackbar_init_value, trackbar_stop_value, onVideoTrackbar);
	cout << trackbar_init_value << " : " << trackbar_stop_value << endl;
	
	
	while(1) {
		m_frame_counter++;
		char text[64];
		sprintf( text, "Frame %d", m_frame_counter );
		cout << "is_paused: " << is_paused << " at while(1)" << endl;
		if( is_paused)
		{
			while (is_paused) {
				cout << "is_paused: " << is_paused << " at while(is_paused)" << endl;
				char c = (char)cvWaitKey();
				switch (c) {
					case 'c':
						is_paused = false;
						m_frame_counter = curr_position;
						cout << "m_frame_counter:" << m_frame_counter << endl;
						break;
					case 'f':
						curr_position++;
						onVideoTrackbar(curr_position);
						break;
					case 'b':
						curr_position--;
						onVideoTrackbar(curr_position);
						break;
					default:
						break;
				}
			}
		}
		
		GetFullFileName();
		input_image = cvLoadImage( m_full_file_name.c_str() );		
		if(!input_image){
			printf("Could not load image file: %s\n", m_full_file_name.c_str());
			break;
		}
		CvPoint pt;
		pt.x = input_image->width / 3;
		pt.y = 50;
		
		cvInitFont(&font_frame, CV_FONT_HERSHEY_SIMPLEX, 1, 1.5, 0, 5);
		cvPutText(input_image, text, pt, &font_frame, CV_RGB(0,255,0));
		
		if( m_display_gt_info )
			DrawGtObjectsOntoImage();
		if( m_display_ar_info )
			DrawArObjectsOntoImage();
		
		cvShowImage( wnd_name_video, input_image );
		
// 		if( m_frame_counter == 820 )
// 			cvSaveImage("ilids-AB_Hard.jpg", input_image );
//		if( m_frame_counter == 13059 )
//			cvSaveImage("false-cam1-13059.jpg", input_image );
		
// 		char filename[64];
// 		sprintf(filename, "results/tlc41/%05d.jpg", m_frame_counter);
// 		cvSaveImage( filename, input_image);
// 		cout << "Save to: " << filename << endl;
		
		cvSetTrackbarPos("Frame", wnd_name_video, m_frame_counter);
		
		cvReleaseImage( &input_image);
		input_image = NULL;
		
		if( (char)cvWaitKey(2) == 'q' )
			break;
	}
}


/************ Utitlity Funcitons *************/

void viz_gt::VizGt::GetFullFileName(){
	char idx[64];
	switch (m_name_num_digit) {
		case 4:
			sprintf(idx, "%04d.", m_frame_counter); 
			break;
		case 5:
			sprintf(idx, "%05d.", m_frame_counter); 
			break;
		case 6:
			sprintf(idx, "%06d.", m_frame_counter); 
			break;
			
		default:
			break;
	}
	
	if( m_img_name_base.size() > 0 )
		m_full_file_name = m_img_path + m_img_name_base + idx + m_img_file_ext;
	else
		m_full_file_name = m_img_path + idx+ m_img_file_ext;
	
//	cout << BASH_ESC_YELLOW << m_full_file_name << BASH_ESC_WHITE << endl;
}

void viz_gt::VizGt::DrawGtObjectsOntoImage(){
	cout << "	DrawArObjectsOntoImage" << endl;
	int objs_num = m_tracked_objects_gt.size();
	for (int i = 0; i < objs_num; ++i) {
		TrackedObject* cur_object = m_tracked_objects_gt.at(i);
//		cout << "Check Object:" << i << endl;
		if( cur_object->m_visible_index.at(m_frame_counter) )
		{
//			cout << "Draw at Frame:" << m_frame_counter << endl;
			int elem_idx = cur_object->m_elements_index.at(m_frame_counter);
			if( elem_idx == -1 )
			{
				cout << BASH_ESC_RED;
				cout << " The element of Object " << cur_object->m_id << " at Frame " << m_frame_counter << " DOES NOT exist!!!" << endl;
				cout << BASH_ESC_WHITE;
				continue;
			}
			
			Element elm = cur_object->m_elements.at( elem_idx );
			CvPoint pt1, pt2;
			pt1.x = elm.x;		pt1.y = elm.y;
			pt2.x = elm.x + elm.width;	pt2.y = elm.y + elm.height;
			char text[16];
			sprintf(text, "id:%d", cur_object->m_id);
			cvInitFont(&font_gt, CV_FONT_HERSHEY_PLAIN, 1, 1, 0, 1);
			
			if( elm.m_type.find("BAGGAGE") != string::npos ){
				cvRectangle(input_image, pt1, pt2, BAG_COLOUR, 2);
				cvPutText(input_image, text, pt2, &font_gt, BAG_COLOUR);
			}
			else{
				cvRectangle(input_image, pt1, pt2, GT_COLOUR, 2);
				cvPutText(input_image, text, pt2, &font_gt, GT_COLOUR);
			}
		}
	}
}

void viz_gt::VizGt::DrawArObjectsOntoImage(){
	cout << "	DrawArObjectsOntoImage" << endl;
	
	int objs_num = m_tracked_objects_ar.size();
	for (int i = 0; i < objs_num; ++i) {
		TrackedObject* cur_object = m_tracked_objects_ar.at(i);
//		cout << "Check Object:" << i << endl;
		if( cur_object->m_visible_index.at(m_frame_counter) )
		{
//			cout << "Draw at Frame:" << m_frame_counter << endl;
			int elem_idx = cur_object->m_elements_index.at(m_frame_counter);
			Element elm = cur_object->m_elements.at( elem_idx );
			CvPoint pt1, pt2;
			pt1.x = elm.x;		pt1.y = elm.y;
			pt2.x = elm.x + elm.width;	pt2.y = elm.y + elm.height;
			char text[16];
			sprintf(text, "id:%d", cur_object->m_id);
			
			if( elm.m_type.find("BAGGAGE") != string::npos ){
				cvRectangle(input_image, pt1, pt2, BAG_COLOUR, 2);
				cvPutText(input_image, text, pt1, &font_ar, BAG_COLOUR);
			}
			else{
				cvRectangle(input_image, pt1, pt2, AR_COLOUR, 2);
				cvPutText(input_image, text, pt1, &font_ar, AR_COLOUR);
			}
		}
	}
}

void viz_gt::VizGt::OutputGtTracks(){
	cout << BASH_ESC_RED << "	Now Output GT Tracks to CSV File - Object based" << BASH_ESC_WHITE << endl;
	ofstream outfile, bbfile;
	
	outfile.open("tracks/tracks.csv");
	int obj_num = m_tracked_objects_gt.size();
	for (int i = 0; i < obj_num; ++i) {
		TrackedObject* cur_object = m_tracked_objects_gt.at(i);
		int id = cur_object->m_id;
		int numSpan = cur_object->m_framespans.size();
		string name = cur_object->m_name;
		cout << BASH_ESC_CYAN << "	=== Object " << id << " (" << name << ", Span Number:" << numSpan << ") ===" << BASH_ESC_WHITE << endl;
		int sum = 0;
		for(int sc = 0; sc < numSpan; ++sc)
		{
			int s_fid = cur_object->m_framespans[sc].start_fid;
			int e_fid = cur_object->m_framespans[sc].end_fid;
			printf("Span:%d (%d-%d)\n", sc, s_fid, e_fid);
			outfile << cur_object->m_id << "," << s_fid << "," << e_fid << endl;
			
			int diff = e_fid - s_fid + 1;
			sum += diff;
		}
		
		char path[64];
		sprintf(path, "tracks/%d_BB.dat", cur_object->m_id);
		bbfile.open(path);	//output bounding box
		int n = cur_object->m_elements.size();		
		printf("Sum of Spans:%d Vs. Total Element Number:%d\n", sum, n);
		assert( sum == n );
		for (int j = 0; j < n; ++j) {
			Element elm = cur_object->m_elements.at(j);
// 			bbfile << elm.frame_id << "," << elm.m_x_3d << "," << elm.m_y_3d << "," << elm.m_z_3d << endl;
           bbfile << elm.frame_id << "," << elm.x << "," << elm.y << "," << elm.width << "," << elm.height << endl;
		}
		bbfile.close();
	}
	
	outfile.close();
    getchar();
}

void viz_gt::VizGt::OutputGtTracksFrameBased(){
	cout << BASH_ESC_YELLOW << "	Now Output GT Tracks to CSV File - Frame-based" << BASH_ESC_WHITE << endl;
	ofstream outfile;
	
	int frnogt =  m_xml_parser_gt->GetTotalFrameNum();
	int frnoar =  m_xml_parser_ar->GetTotalFrameNum();
	assert( frnogt == frnoar );
	
	outfile.open("tracks/tracks-framebased.csv");
	for (int fid = 0; fid < frnogt; ++fid)
	{
		int obj_num = m_tracked_objects_gt.size();
		for (int n = 0; n < obj_num; ++n) {
			TrackedObject* cur_object = m_tracked_objects_gt.at(n);
			int id = cur_object->m_id;
			int idx = cur_object->m_elements_index[fid];
			if ( idx != -1) {
				Element elm = cur_object->m_elements.at(idx);
				outfile << fid << "," << id << "," << elm.x << "," << elm.y << "," << elm.width << "," << elm.height << ",-1" << endl;
//				outfile << cur_object->m_total_frame_num << "," << fid << "," << elm.frame_id << "," << id << "," << elm.x << "," << elm.y << "," << elm.width << "," << elm.height << ", -1" << endl;

			}
		}
	}
	
	outfile.close();
	getchar();
}

void viz_gt::VizGt::OutputGtToSubitoXml(){
	xmlTextWriterPtr writer;
	
	if( m_output_gt_in_subito_format.size() == 0 )
	{
		cout << BASH_ESC_RED << "	| SUBITO xml output file is not specified - No Action for Output |" << BASH_ESC_WHITE << endl;
		return;
	}else
		cout << BASH_ESC_PURPLE << "	NOTE: Now outputing GT to xml file in SUBITO format: " << m_output_gt_in_subito_format << BASH_ESC_WHITE << endl;
	
	writer = xmlNewTextWriterFilename( m_output_gt_in_subito_format.c_str(), 0);
	
	//write subito headers
	int ret = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
	
	ret = xmlTextWriterStartElement(writer, BAD_CAST "WP500_Header");
	ret = xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns", BAD_CAST "http://www.subito-project.eu");
	ret = xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns:xsi", BAD_CAST "http://www.w3.org/2001/XMLSchema-instance");
	ret = xmlTextWriterWriteAttribute(writer, BAD_CAST "xsi:schemaLocation", BAD_CAST "http://www.subito-project.eu XML_IAA_Schema_v1.0.xsd");
	ret = xmlTextWriterWriteAttribute(writer, BAD_CAST "provider", BAD_CAST "UoR");
	ret = xmlTextWriterWriteAttribute(writer, BAD_CAST "mode", BAD_CAST "offline");
	
	ret = xmlTextWriterStartElement(writer, BAD_CAST "Sequence");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "pets06s1");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "camIdx", "%d", 4);
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "recordDateTime", "%s", BAD_CAST "2006:03:28:00:00:00");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "width", "%d", 720);
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "height", "%d", 576);
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "fps", "%d", 25);
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "processDate", "%d:%d:%d", 2010, 4, 8);
	
	int n = m_xml_parser_gt->GetTotalFrameNum();
	int m = m_tracked_objects_gt.size();
	for (int i = 0; i < n; ++i) {
		ret = xmlTextWriterStartElement(writer, BAD_CAST "entityFrame");
		ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "frameID", "%d", i);
		ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "processTime", "%d:%d:%d:%d", 0, 0, 0, 0);

		for (int j = 0; j < m; ++j) {
			TrackedObject* obj = m_tracked_objects_gt.at(j);
			if( obj->m_visible_index.at(i) )
			{
				ret = xmlTextWriterStartElement(writer, BAD_CAST "entity");
				ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "entityID", "%d", obj->m_id);
				ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "confidence", "%.2f", 1.0);
				
				int xmin = obj->m_elements[ obj->m_elements_index[i] ].x;
				int ymin = obj->m_elements[ obj->m_elements_index[i] ].y;
				int xmax = xmin + obj->m_elements[ obj->m_elements_index[i] ].width - 1;
				int ymax = ymin + obj->m_elements[ obj->m_elements_index[i] ].height - 1;

				ret = xmlTextWriterStartElement(writer, BAD_CAST "detection");
				ret = xmlTextWriterStartElement(writer, BAD_CAST "info2D");
				ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "xMin", "%d", xmin);
				ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "yMin", "%d", ymin);
				ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "xMax", "%d", xmax);
				ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "yMax", "%d", ymax);
				
				ret = xmlTextWriterEndElement(writer);	//closing info2D
				ret = xmlTextWriterEndElement(writer);	//closing detection			
				ret = xmlTextWriterEndElement(writer);	//closing entity
			}
		}
		
		ret = xmlTextWriterEndElement(writer);	//closing entityFrame		
	}
	
	
	ret = xmlTextWriterEndDocument(writer);
	xmlFreeTextWriter(writer);
	
}

void viz_gt::VizGt::OutputGtToViperXml(){
	xmlTextWriterPtr writer;
	
	if( m_output_gt_in_viper_format.size() == 0 )
	{
		cout << BASH_ESC_RED << "	| VIPER xml output file is not specified - No Action for Output |" << BASH_ESC_WHITE << endl;
		return;
	}else
		cout << BASH_ESC_PURPLE << "	NOTE: Now outputing GT to xml file in Viper format: " << m_output_gt_in_viper_format << BASH_ESC_WHITE << endl;
	
	writer = xmlNewTextWriterFilename( m_output_gt_in_viper_format.c_str(), 0);
	int ret = xmlTextWriterSetIndent(writer, 1);
	//write Viper headers
	ret = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
	
// 	<viper xmlns="http://lamp.cfar.umd.edu/viper#" xmlns:data="http://lamp.cfar.umd.edu/viperdata#">

// 	<descriptor name="Person" type="OBJECT">
// <attribute dynamic="false" name="Name" type="http://lamp.cfar.umd.edu/viperdata#svalue"/>
// <attribute dynamic="false" name="Index" type="http://lamp.cfar.umd.edu/viperdata#dvalue"/>
// <attribute dynamic="true" name="Location" type="http://lamp.cfar.umd.edu/viperdata#bbox"/>
// </descriptor>

	ret = xmlTextWriterStartElement(writer, BAD_CAST "viper");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "xmlns", "http://lamp.cfar.umd.edu/viper#");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "xmlns:data", "http://lamp.cfar.umd.edu/viperdata#");
	
	ret = xmlTextWriterStartElement(writer, BAD_CAST "config");
	ret = xmlTextWriterStartElement(writer, BAD_CAST "descriptor");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "Person");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "type", "OBJECT");
	
	ret = xmlTextWriterStartElement(writer, BAD_CAST "attribute");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "dynamic", "false");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "Name");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "type", "http://lamp.cfar.umd.edu/viperdata#svalue");
	ret = xmlTextWriterEndElement(writer);	//closing attribute
	
	ret = xmlTextWriterStartElement(writer, BAD_CAST "attribute");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "dynamic", "false");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "Index");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "type", "http://lamp.cfar.umd.edu/viperdata#dvalue");
	ret = xmlTextWriterEndElement(writer);	//closing attribute
	
	ret = xmlTextWriterStartElement(writer, BAD_CAST "attribute");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "dynamic", "true");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "Location");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "type", "http://lamp.cfar.umd.edu/viperdata#bbox");
	ret = xmlTextWriterEndElement(writer);	//closing attribute
	
	ret = xmlTextWriterEndElement(writer);	//closing descriptor
	ret = xmlTextWriterEndElement(writer);	//closing config

	
// 	<sourcefile filename="C:\Users\Anna\Documents\PETS\2009\PETS2009.xml">
// <file>
// </file>
// <object framespan="0:0 7:7 14:14 21:21 28:28 35:35 42:42 49:49 56:56 63:63 70:70 77:77 84:84 91:91 98:98" id="0" name="I-Frames"/>
// <object framespan=" 7:7 14:14 21:21 28:28 35:35 42:42 49:49 56:56 63:63 70:70 77:77 84:84 91:91 98:98 " id="0" name="FRAME">
// <attribute name="EVALUATE">

	// now the real meat!
	ret = xmlTextWriterStartElement(writer, BAD_CAST "data");
	ret = xmlTextWriterStartElement(writer, BAD_CAST "sourcefile");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "filename", "something");
	
	ret = xmlTextWriterStartElement(writer, BAD_CAST "file");
	ret = xmlTextWriterEndElement(writer);	//closing file
	
	string full_fs;
	GetCompleteFrameSpanGT(full_fs);
	
	ret = xmlTextWriterStartElement(writer, BAD_CAST "object");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "framespan", "%s", full_fs.c_str());
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "id", "%d", 0);
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "I-Frames");
	ret = xmlTextWriterEndElement(writer);	//closing object(I-Frames)
	
	
// <object framespan=" 7:7 14:14 21:21 28:28 35:35 42:42 49:49 56:56 63:63 70:70 77:77 777:777 784:784 791:791" id="0" name="FRAME">
// −
// <attribute name="EVALUATE">
// <data:bvalue framespan="0:791" value="true"/>
// </attribute>
// −
// <attribute name="CROWD">
// <data:bvalue framespan="0:791" value="false"/>
// </attribute>
// −
// <attribute name="MULTIPLE VEHICLE">
// <data:bvalue framespan="0:791" value="false"/>
// </attribute>
// −
// <attribute name="MULTIPLE TEXT AREAS">
// <data:bvalue framespan="0:791" value="false"/>
// </attribute>
// </object>

	ret = xmlTextWriterStartElement(writer, BAD_CAST "object");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "framespan", "%s", full_fs.c_str());
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "id", "%d", 0);
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "FRAME");
	
	string first_last_effective_span;
	char temp[32];
	sprintf(temp, "%d:%d", m_first_effective_fid, m_last_effective_fid);
	first_last_effective_span = temp;
	
	ret = xmlTextWriterStartElement(writer, BAD_CAST "attribute");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "EVALUATE");
	ret = xmlTextWriterStartElement(writer, BAD_CAST "data:bvalue");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "framespan", "%s", first_last_effective_span.c_str());
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "value", "true");
	ret = xmlTextWriterEndElement(writer);	//closing data:bvalue
	ret = xmlTextWriterEndElement(writer);	//closing attribute
	
	ret = xmlTextWriterStartElement(writer, BAD_CAST "attribute");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "CROWD");
	ret = xmlTextWriterStartElement(writer, BAD_CAST "data:bvalue");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "framespan", "%s", first_last_effective_span.c_str());
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "value", "false");
	ret = xmlTextWriterEndElement(writer);	//closing data:bvalue
	ret = xmlTextWriterEndElement(writer);	//closing attribute
	
	ret = xmlTextWriterStartElement(writer, BAD_CAST "attribute");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "MULTIPLE VEHICLE");
	ret = xmlTextWriterStartElement(writer, BAD_CAST "data:bvalue");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "framespan", "%s", first_last_effective_span.c_str());
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "value", "false");
	ret = xmlTextWriterEndElement(writer);	//closing data:bvalue
	ret = xmlTextWriterEndElement(writer);	//closing attribute
	
	ret = xmlTextWriterStartElement(writer, BAD_CAST "attribute");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "MULTIPLE TEXT AREAS");
	ret = xmlTextWriterStartElement(writer, BAD_CAST "data:bvalue");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "framespan", "%s", first_last_effective_span.c_str());
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "value", "false");
	ret = xmlTextWriterEndElement(writer);	//closing data:bvalue
	ret = xmlTextWriterEndElement(writer);	//closing attribute
	
	ret = xmlTextWriterEndElement(writer);	//closing object(FRAME)
	
	int gt_num = m_tracked_objects_gt.size();
	for( int i = 0; i < gt_num; ++i )
	{		
		TrackedObject* obj = m_tracked_objects_gt.at(i);
		string framespan;
		char tmp[32];
	
		cout << "	================================================" << endl;
		cout << "		Write for Object with ID " << obj->m_id << endl;
		
		int fs_num = obj->m_framespans.size();
		for( int fs = 0; fs < fs_num; ++fs )
		{
			int fid_s = obj->m_framespans.at(fs).start_fid;
			int fid_e = obj->m_framespans.at(fs).end_fid;
			
			if(fid_s == fid_e)
			{
				sprintf(tmp, "%d:%d ", fid_s, fid_e);
				framespan += tmp;
			} else if (fid_s < fid_e)
			{
				for( int t = fid_s; t <= fid_e; ++t )
				{
					sprintf(tmp, "%d:%d ", t, t);
					framespan += tmp;
				}
			} else
			{
				cout << "Sth is wrong! quit..." << endl;
				exit(1);
			}
		}
		framespan = framespan.substr(0, framespan.size()-1);	//get rid of the last empty space 
		
		ret = xmlTextWriterStartElement(writer, BAD_CAST "object");
		ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "framespan", "%s", framespan.c_str());
		ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "id", "%d", obj->m_id);
		ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "Person");
		
		ret = xmlTextWriterStartElement(writer, BAD_CAST "attribute");
		ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "Location");
		
		int elem_num = obj->m_elements.size();
		
		// ============ output debug info ==============
		if( obj->m_id == 2 )
		{
			cout << "		*** Object 2 *** " << endl;
			cout << "TimeSpan Number: " << fs_num << endl;
			cout << "Elements Number: " << elem_num << endl;
		}
		// ============ done ===========================
		
		for( int e = 0; e < elem_num; ++e )
		{
			int fid = obj->m_elements.at(e).frame_id;
			if( obj->m_visible_index.at(fid) )	//if this object is visible at this frame
			{
				// just double check m_elements_index is sorted correctly
				if( obj->m_elements_index.at(fid) != e )
				{
					cout << "Sorted element index " << obj->m_elements_index.at(fid) << " for frame " << fid;
					cout << "  is not consistent with its real index: " << e << endl;
					cout << "QUIT!" << endl;
					exit(1);
				}
				
				int x = obj->m_elements.at(e).x;
				int y = obj->m_elements.at(e).y;
				int width = obj->m_elements.at(e).width;
				int height = obj->m_elements.at(e).height;
				
				ret = xmlTextWriterStartElement(writer, BAD_CAST "data:bbox");
				ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "framespan", "%d:%d", fid, fid);
				ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "height", "%d", height);
				ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "width", "%d", width);
				ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "x", "%d", x);
				ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "y", "%d", y);
				ret = xmlTextWriterEndElement(writer);	//closing data:bbox
			}
		}
		
		ret = xmlTextWriterEndElement(writer);	//closing attribute
		ret = xmlTextWriterEndElement(writer);	//closing object
		
		cout << "	================================================" << endl;
	}
		
	ret = xmlTextWriterEndDocument(writer);
	xmlFreeTextWriter(writer);
}

void viz_gt::VizGt::GetCompleteFrameSpanGT( string& result )
{
	set<int> framespans;
	int obj_num = m_tracked_objects_gt.size();
	for( int i = 0; i < obj_num; ++i )
	{
		TrackedObject* obj = m_tracked_objects_gt.at(i);
		
		int elem_num = obj->m_elements.size();
		for( int e = 0; e < elem_num; ++e )
		{
			int fid = obj->m_elements.at(e).frame_id;
			framespans.insert(fid);
		}
	}
	
	int fs_num = framespans.size();
	cout << "Total Effective Frame Number:" << fs_num << endl;
	set<int>::iterator iter;
	char tmp[32];
	int value = 0;
	for( iter = framespans.begin(); iter != framespans.end(); ++iter )
	{
		cout << *iter << "	";
		sprintf(tmp, "%d:%d ", *iter, *iter);
		result += tmp;
		
		value = *iter;
		if( iter == framespans.begin() )
			m_first_effective_fid = *iter;
// 		if( iter == framespans.end() -1 )
// 			m_last_effective_fid = *iter;
	}
	result = result.substr(0, result.size()-1);
	m_last_effective_fid = value;
	
	cout << endl;
}



void viz_gt::VizGt::OutputArToViperXml(){
	xmlTextWriterPtr writer;
	
	if( m_output_ar_in_viper_format.size() == 0 )
	{
		cout << BASH_ESC_RED << "	| VIPER xml output file is not specified - No Action for Output |" << BASH_ESC_WHITE << endl;
		return;
	}else
		cout << BASH_ESC_PURPLE << "	NOTE: Now outputing AR to xml file in Viper format: " << m_output_ar_in_viper_format << BASH_ESC_WHITE << endl;
	
	writer = xmlNewTextWriterFilename( m_output_ar_in_viper_format.c_str(), 0);
	int ret = xmlTextWriterSetIndent(writer, 1);
	//write Viper headers
	ret = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
	
// 	<viper xmlns="http://lamp.cfar.umd.edu/viper#" xmlns:data="http://lamp.cfar.umd.edu/viperdata#">

// 	<descriptor name="Person" type="OBJECT">
// <attribute dynamic="false" name="Name" type="http://lamp.cfar.umd.edu/viperdata#svalue"/>
// <attribute dynamic="false" name="Index" type="http://lamp.cfar.umd.edu/viperdata#dvalue"/>
// <attribute dynamic="true" name="Location" type="http://lamp.cfar.umd.edu/viperdata#bbox"/>
// </descriptor>

	ret = xmlTextWriterStartElement(writer, BAD_CAST "viper");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "xmlns", "http://lamp.cfar.umd.edu/viper#");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "xmlns:data", "http://lamp.cfar.umd.edu/viperdata#");
	
	ret = xmlTextWriterStartElement(writer, BAD_CAST "config");
	ret = xmlTextWriterStartElement(writer, BAD_CAST "descriptor");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "Person");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "type", "OBJECT");
	
	ret = xmlTextWriterStartElement(writer, BAD_CAST "attribute");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "dynamic", "false");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "Name");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "type", "http://lamp.cfar.umd.edu/viperdata#svalue");
	ret = xmlTextWriterEndElement(writer);	//closing attribute
	
	ret = xmlTextWriterStartElement(writer, BAD_CAST "attribute");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "dynamic", "false");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "Index");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "type", "http://lamp.cfar.umd.edu/viperdata#dvalue");
	ret = xmlTextWriterEndElement(writer);	//closing attribute
	
	ret = xmlTextWriterStartElement(writer, BAD_CAST "attribute");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "dynamic", "true");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "Location");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "type", "http://lamp.cfar.umd.edu/viperdata#bbox");
	ret = xmlTextWriterEndElement(writer);	//closing attribute
	
	ret = xmlTextWriterEndElement(writer);	//closing descriptor
	ret = xmlTextWriterEndElement(writer);	//closing config

	
// 	<sourcefile filename="C:\Users\Anna\Documents\PETS\2009\PETS2009.xml">
// <file>
// </file>
// <object framespan="0:0 7:7 14:14 21:21 28:28 35:35 42:42 49:49 56:56 63:63 70:70 77:77 84:84 91:91 98:98" id="0" name="I-Frames"/>
// <object framespan=" 7:7 14:14 21:21 28:28 35:35 42:42 49:49 56:56 63:63 70:70 77:77 84:84 91:91 98:98 " id="0" name="FRAME">
// <attribute name="EVALUATE">

	// now the real meat!
	ret = xmlTextWriterStartElement(writer, BAD_CAST "data");
	ret = xmlTextWriterStartElement(writer, BAD_CAST "sourcefile");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "filename", "something");
	
	ret = xmlTextWriterStartElement(writer, BAD_CAST "file");
	ret = xmlTextWriterEndElement(writer);	//closing file
	
	string full_fs;
	GetCompleteFrameSpanAR(full_fs);
	
	ret = xmlTextWriterStartElement(writer, BAD_CAST "object");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "framespan", "%s", full_fs.c_str());
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "id", "%d", 0);
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "I-Frames");
	ret = xmlTextWriterEndElement(writer);	//closing object(I-Frames)
	
	
// <object framespan=" 7:7 14:14 21:21 28:28 35:35 42:42 49:49 56:56 63:63 70:70 77:77 777:777 784:784 791:791" id="0" name="FRAME">
// −
// <attribute name="EVALUATE">
// <data:bvalue framespan="0:791" value="true"/>
// </attribute>
// −
// <attribute name="CROWD">
// <data:bvalue framespan="0:791" value="false"/>
// </attribute>
// −
// <attribute name="MULTIPLE VEHICLE">
// <data:bvalue framespan="0:791" value="false"/>
// </attribute>
// −
// <attribute name="MULTIPLE TEXT AREAS">
// <data:bvalue framespan="0:791" value="false"/>
// </attribute>
// </object>

	ret = xmlTextWriterStartElement(writer, BAD_CAST "object");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "framespan", "%s", full_fs.c_str());
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "id", "%d", 0);
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "FRAME");
	
	string first_last_effective_span;
	char temp[32];
	sprintf(temp, "%d:%d", m_first_effective_fid, m_last_effective_fid);
	first_last_effective_span = temp;
	
	ret = xmlTextWriterStartElement(writer, BAD_CAST "attribute");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "EVALUATE");
	ret = xmlTextWriterStartElement(writer, BAD_CAST "data:bvalue");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "framespan", "%s", first_last_effective_span.c_str());
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "value", "true");
	ret = xmlTextWriterEndElement(writer);	//closing data:bvalue
	ret = xmlTextWriterEndElement(writer);	//closing attribute
	
	ret = xmlTextWriterStartElement(writer, BAD_CAST "attribute");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "CROWD");
	ret = xmlTextWriterStartElement(writer, BAD_CAST "data:bvalue");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "framespan", "%s", first_last_effective_span.c_str());
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "value", "false");
	ret = xmlTextWriterEndElement(writer);	//closing data:bvalue
	ret = xmlTextWriterEndElement(writer);	//closing attribute
	
	ret = xmlTextWriterStartElement(writer, BAD_CAST "attribute");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "MULTIPLE VEHICLE");
	ret = xmlTextWriterStartElement(writer, BAD_CAST "data:bvalue");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "framespan", "%s", first_last_effective_span.c_str());
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "value", "false");
	ret = xmlTextWriterEndElement(writer);	//closing data:bvalue
	ret = xmlTextWriterEndElement(writer);	//closing attribute
	
	ret = xmlTextWriterStartElement(writer, BAD_CAST "attribute");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "MULTIPLE TEXT AREAS");
	ret = xmlTextWriterStartElement(writer, BAD_CAST "data:bvalue");
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "framespan", "%s", first_last_effective_span.c_str());
	ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "value", "false");
	ret = xmlTextWriterEndElement(writer);	//closing data:bvalue
	ret = xmlTextWriterEndElement(writer);	//closing attribute
	
	ret = xmlTextWriterEndElement(writer);	//closing object(FRAME)
	
	int ar_num = m_tracked_objects_ar.size();
	for( int i = 0; i < ar_num; ++i )
	{		
		TrackedObject* obj = m_tracked_objects_ar.at(i);
		string framespan;
		char tmp[32];
	
		cout << "	================================================" << endl;
		cout << "		Write for Object with ID " << obj->m_id << endl;
		
		int fs_num = obj->m_framespans.size();
		for( int fs = 0; fs < fs_num; ++fs )
		{
			int fid_s = obj->m_framespans.at(fs).start_fid;
			int fid_e = obj->m_framespans.at(fs).end_fid;
			
			if(fid_s == fid_e)
			{
				sprintf(tmp, "%d:%d ", fid_s, fid_e);
				framespan += tmp;
			} else if (fid_s < fid_e)
			{
				for( int t = fid_s; t <= fid_e; ++t )
				{
					sprintf(tmp, "%d:%d ", t, t);
					framespan += tmp;
				}
			} else
			{
				cout << "Sth is wrong! quit..." << endl;
				exit(1);
			}
		}
		framespan = framespan.substr(0, framespan.size()-1);	//get rid of the last empty space 
		
		ret = xmlTextWriterStartElement(writer, BAD_CAST "object");
		ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "framespan", "%s", framespan.c_str());
		ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "id", "%d", obj->m_id);
		ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "Person");
		
		ret = xmlTextWriterStartElement(writer, BAD_CAST "attribute");
		ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "name", "Location");
		
		int elem_num = obj->m_elements.size();
		
		// ============ output debug info ==============
		if( obj->m_id == 2 )
		{
			cout << "		*** Object 2 *** " << endl;
			cout << "TimeSpan Number: " << fs_num << endl;
			cout << "Elements Number: " << elem_num << endl;
		}
		// ============ done ===========================
		
		for( int e = 0; e < elem_num; ++e )
		{
			int fid = obj->m_elements.at(e).frame_id;
			if( obj->m_visible_index.at(fid) )	//if this object is visible at this frame
			{
				// just double check m_elements_index is sorted correctly
				if( obj->m_elements_index.at(fid) != e )
				{
					cout << "Sorted element index " << obj->m_elements_index.at(fid) << " for frame " << fid;
					cout << "  is not consistent with its real index: " << e << endl;
					cout << "QUIT!" << endl;
					exit(1);
				}
				
				int x = obj->m_elements.at(e).x;
				int y = obj->m_elements.at(e).y;
				int width = obj->m_elements.at(e).width;
				int height = obj->m_elements.at(e).height;
				
				ret = xmlTextWriterStartElement(writer, BAD_CAST "data:bbox");
				ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "framespan", "%d:%d", fid, fid);
				ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "height", "%d", height);
				ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "width", "%d", width);
				ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "x", "%d", x);
				ret = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "y", "%d", y);
				ret = xmlTextWriterEndElement(writer);	//closing data:bbox
			}
		}
		
		ret = xmlTextWriterEndElement(writer);	//closing attribute
		ret = xmlTextWriterEndElement(writer);	//closing object
		
		cout << "	================================================" << endl;
	}
		
	ret = xmlTextWriterEndDocument(writer);
	xmlFreeTextWriter(writer);
}

void viz_gt::VizGt::GetCompleteFrameSpanAR( string& result )
{
	set<int> framespans;
	int obj_num = m_tracked_objects_ar.size();
	for( int i = 0; i < obj_num; ++i )
	{
		TrackedObject* obj = m_tracked_objects_ar.at(i);
		
		int elem_num = obj->m_elements.size();
		for( int e = 0; e < elem_num; ++e )
		{
			int fid = obj->m_elements.at(e).frame_id;
			framespans.insert(fid);
		}
	}
	
	int fs_num = framespans.size();
	cout << "Total Effective Frame Number:" << fs_num << endl;
	set<int>::iterator iter;
	char tmp[32];
	int value = 0;
	for( iter = framespans.begin(); iter != framespans.end(); ++iter )
	{
		cout << *iter << "	";
		sprintf(tmp, "%d:%d ", *iter, *iter);
		result += tmp;
		
		value = *iter;
		if( iter == framespans.begin() )
			m_first_effective_fid = *iter;
// 		if( iter == framespans.end() -1 )
// 			m_last_effective_fid = *iter;
	}
	result = result.substr(0, result.size()-1);
	m_last_effective_fid = value;
	
	cout << endl;
}