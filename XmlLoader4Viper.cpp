/**
 * @brief ...
 * Facts about ViPER format:
 * 1. What kind of frames are considered to be 'visible'?
 * A: any frame that listed in the value of object's attribute 'framespan'
 * 
 * 2. The number of frames which contains real object (e.g. bounding boxes for that object) can be larger than visible frames.
 *    i.e. not all annotated frame is visible
 * 
 * 3. On the other hand, a visible frame can contain an empty object. i.e. the corresponding m_visible_index[i] == -1
 *    However this mostly likely due to an annotation error and really SHOULD NOT HAPPEN.
 *
 * 4. In another word, IDEALLY, COUNT( m_visible_index[i] != -1 ) should equal to COUNT ( m_elements )
 * 
 * 5. Because tuple {m_framespans, m_visible_index} is worked out separately from tuple {m_elements, m_elements_index}
 *    And there is no cross validation done between these two tuples.
 *
 * 6. Viper has the convention to have fid starting from 1. Therefore in this implementaion, the Total Frame Number, e.g. m_num_frames and TrackedObject::m_total_frame_num equals to the total number plus 1. This way, the first element in relevant vectors is not used. i.e. TrackedObject::m_elements_index[0], TrackedObject::m_visible_index[0] etc.
 **/



#include "XmlLoader4Viper.h"


xml_loader::XmlLoader4Viper::XmlLoader4Viper(){
	m_start_data_tag = false;
	m_start_file_tag = false;
	m_start_object_tag = false;
	m_start_attri_tag = false;
	m_is_name_attribute = false;
	m_is_index_attribute = false;
	m_is_location_attribute = false;
	m_is_numframes_attribute = false;
	
	/// llz 2012-10-04
	/// somehow the (x,y) seems to be the center of the person, rather than pointing to the top left corner!
	m_is_viper_format_from_anna = false;
	
	m_num_object = 0;
	m_num_frames = 0;
}

xml_loader::XmlLoader4Viper::~XmlLoader4Viper(){}

void xml_loader::XmlLoader4Viper::LoadFromFile( const char* file_name  ){
	m_reader = xmlReaderForFile( file_name, NULL, 0);	
	if(m_reader==NULL)
	{
		printf("fail to open xml file %s. So quit now\n", file_name);
		exit(0);
	}
	else
		cout << BASH_ESC_RED << "XML Open Success:" <<  file_name;
	cout << BASH_ESC_WHITE << endl;
}

void xml_loader::XmlLoader4Viper::ReadToTrackedObject( vector<TrackedObject*>& objects ){
	TrackedObject* obj = NULL;
	Element elem;
	while(xmlTextReaderRead(m_reader)==1){
		m_name = xmlTextReaderConstName( m_reader );
		if (m_name == NULL)
			m_name = BAD_CAST "--";
		m_value = xmlTextReaderConstValue(m_reader );
//		if( current_frame < 70 ){
//			printf("name : %s\n", m_name);
//			//printf("value: %s\n", m_value);
//		}
		if( !strcmp((char*)m_name, "data") )
		{
			m_start_data_tag = !m_start_data_tag;
		}
		if( !strcmp((char*)m_name, "file") )
		{
			m_start_file_tag = !m_start_file_tag;
		}
		if( !strcmp((char*)m_name, "object") )
		{
			// only Person/PERSON object is to be added
			m_attri = xmlTextReaderGetAttribute( m_reader, BAD_CAST "name");
			if( !strcmp((char*)m_attri, "Person") || !strcmp((char*)m_attri, "PERSON") || !strcmp((char*)m_attri, "Target"))
			{	
				m_start_object_tag = !m_start_object_tag;
				if( m_start_object_tag )
				{
					cout << BASH_ESC_PURPLE;
					cout <<"		=== Adding No. " << m_num_object << " Object. ===" << BASH_ESC_WHITE << endl;
					obj = new TrackedObject();
					obj->m_elements_index.resize( m_num_frames, -1 );
					obj->m_visible_index.resize( m_num_frames, false );
					
					
					obj->m_type = (char*)m_attri;
					
					m_attri = xmlTextReaderGetAttribute( m_reader, BAD_CAST "framespan");
	//				cout << m_attri << endl;
					GetFramespansForTrackedObject( (char*)m_attri, obj );				
					GetVisibleElementNum( obj );
					cout << "Visible frames: " << obj->m_visible_num << endl;
					
					// following two lines should be replaced by value of Index!!!
					// By default Viper has a 'id' attribute starting from 0, and the value is unchangable!
					m_attri = xmlTextReaderGetAttribute( m_reader, BAD_CAST "id");
					obj->m_id = atoi( (char*)m_attri ) + 1;
				}else {
					obj->m_total_frame_num = m_num_frames;
					objects.push_back( obj );
					m_num_object++;
					
					cout << BASH_ESC_CYAN << "  Last Summary" << BASH_ESC_WHITE << endl;
					cout << "Frame elements include: " << obj->m_elements.size() << endl;
					cout << obj->m_framespans.size() << " Spans,";
					cout << "Visible frames: " << obj->m_visible_num << endl;
// 					for (int i = 0; i < obj->m_framespans.size(); ++i) {
// 						cout << "	" << obj->m_framespans.at(i).start_fid;
// 						cout << "	" << obj->m_framespans.at(i).end_fid << endl;
// 					}
// 					for (int i = 0; i < obj->m_elements_index.size(); ++i) {
// 						if( obj->m_elements_index[i] != -1 )
// 						{
// 							cout << "Frame: " << i;
// 							cout << "	Index: " << obj->m_elements_index[i];
// 							cout << "	Visible: " << boolalpha << obj->m_visible_index[i] << endl;
// 						}
// 					}
					cout << "Object Name:" << obj->m_name << endl;
					cout << "Object Type:" << obj->m_type << endl << endl;
				}
			}
		}
		
		if( !strcmp((char*)m_name, "attribute") && m_start_data_tag )
		{
			//cout << "	tag name : " << m_name << endl;
			m_attri = xmlTextReaderGetAttribute( m_reader, BAD_CAST "name");
			//cout << "	attribute: " << m_attri << endl;
			/// llz 2012-10-04 
			/// OK, now there are two formats to find out the total number of frames before parsing for the first real object
			/// 1. if the viper file is generated by Viper-gt tool, then there should be a NUMFRAMES field holding the value
			/// 2. if the gound truth file is from Anna, looking for EVALUATE attribute which indicates the framespan to be evaluated, 
			/// then take the last number in the span as the total frame number (from "data:bvalue" below)
			///     --- obviously this may be smaller than the real frame number since one can choose to only evaluate part of the sequence
			if( !strcmp((char*)m_attri, "NUMFRAMES") || !strcmp((char*)m_attri, "EVALUATE") )
			{
				m_is_numframes_attribute = true;
			}else	if( !strcmp((char*)m_attri, "Name") )
			{
				m_is_name_attribute = true;
			}else	if( !strcmp((char*)m_attri, "Index") )
			{
				m_is_index_attribute = true;
			}else	if( !strcmp((char*)m_attri, "Location") || !strcmp((char*)m_attri, "LOCATION") || !strcmp((char*)m_attri, "BBox") )
			{
				m_is_location_attribute = true;
			}else {
				m_is_numframes_attribute = false;
				m_is_name_attribute = false;
				m_is_index_attribute = false;
				m_is_location_attribute = false;	
			}
		}
		
		if( !strcmp((char*)m_name, "data:bvalue") )
		{
			cout << "Found bvalue" << endl;
			if( m_is_numframes_attribute )
			{
				m_attri = xmlTextReaderGetAttribute( m_reader, BAD_CAST "framespan");
				string span = (char*)m_attri;
				int first_id;
				GetFrameSpan(span, first_id, m_num_frames);
				m_num_frames += 1;
				cout << "Total Frame Number: " << first_id << "," << m_num_frames << " - Getting from EVALUATE field" << endl;
			}
		}
		if( !strcmp((char*)m_name, "data:dvalue") )
		{
			if( m_start_object_tag && m_is_index_attribute )
			{
				m_attri = xmlTextReaderGetAttribute( m_reader, BAD_CAST "value");
				obj->m_id = atoi( (char*)m_attri );
			}
			if( m_start_file_tag && m_is_numframes_attribute )
			{
				m_attri = xmlTextReaderGetAttribute( m_reader, BAD_CAST "value");
				m_num_frames = atoi( (char*)m_attri )+1;
				cout << "Total Frame Number: " << m_num_frames << " - Getting from NUMFRAMES field" << endl;
			}
		}
		if( !strcmp((char*)m_name, "data:svalue") )
		{
			if( m_start_object_tag && m_is_name_attribute )
			{
				m_attri = xmlTextReaderGetAttribute( m_reader, BAD_CAST "value");
				obj->m_name = (char*)m_attri;
			}
		}
		if( !strcmp((char*)m_name, "data:bbox") )
		{
			if( m_start_object_tag && m_is_location_attribute )
			{
				m_attri = xmlTextReaderGetAttribute( m_reader, BAD_CAST "framespan");
				string fs = (char*)m_attri;
				int s = 0;
				int e = 0;
				GetFrameSpan( fs, s, e );
				int diff = e - s + 1;
				
				m_attri = xmlTextReaderGetAttribute( m_reader, BAD_CAST "height");
				elem.height = atoi( (char*)m_attri );
				m_attri = xmlTextReaderGetAttribute( m_reader, BAD_CAST "width");
				elem.width = atoi( (char*)m_attri );
				m_attri = xmlTextReaderGetAttribute( m_reader, BAD_CAST "x");
				elem.x = atoi( (char*)m_attri );
				m_attri = xmlTextReaderGetAttribute( m_reader, BAD_CAST "y");
				elem.y = atoi( (char*)m_attri );
								
				if(m_is_viper_format_from_anna)
				{
					elem.x = elem.x - (int)(elem.width/2);
					elem.y = elem.y - (int)(elem.height/2);
				}
				
				/// check coords sanity
				if(elem.x < 0)
				{
					elem.width = elem.width + elem.x -1;
					elem.x = 0;
				}
				if(elem.y < 0)
				{
					elem.height = elem.height + elem.y - 1;
					elem.y = 0;
				}
				int rightSide = elem.x + elem.width;
				int bottomSide = elem.y + elem.height;
				/// 704x576 is the image size for elsag seqeunces!
//				if( rightSide > 703 )
//				{
//					elem.width = 703 - elem.x;
//				}
//				if( bottomSide > 575 )
//					elem.height = 575 - elem.y;				
				
				if( diff == 1 ){
// 					cout << BASH_ESC_CYAN << fs << BASH_ESC_WHITE << endl;
					elem.frame_id = s;
					obj->m_elements_index[s] = obj->m_elements.size();
					obj->m_elements.push_back( elem );					
				}
				// same object being static for diff frames
				if( diff > 1 ){
// 					cout << BASH_ESC_PURPLE << "  " << fs << BASH_ESC_WHITE << endl;
					for (int j = 0; j < diff; ++j) {
						elem.frame_id = s + j;
						obj->m_elements_index[s+j] = obj->m_elements.size();
						obj->m_elements.push_back( elem );
					}
				}
			}
		}		
	}
	xmlFreeTextReader(m_reader);
	m_reader = NULL;
	
	cout << "There are " << m_num_object << " objects." << endl;
}


void xml_loader::XmlLoader4Viper::GetFramespansForTrackedObject( char* str, TrackedObject* object )
{
	cout << "	[GetFramespansForTrackedObject]" << endl;
	string spans = str;
	string segment;
	int num_spans = 0;
	int s_fid = 0;
	int e_fid = 0;
	size_t idx;
	idx = spans.find_first_of(" ");
	while ( idx != string::npos ) {
		num_spans++;
		segment = spans.substr(0, idx );
		GetFrameSpan( segment, s_fid, e_fid );

		TimeSpan ts;
		ts.start_fid = s_fid;
		ts.end_fid = e_fid;
		object->m_framespans.push_back( ts );	
		spans = spans.substr( idx + 1 );
		cout << segment << "	" << spans << endl;
		idx = spans.find_first_of(" ");
	}
	
	if( spans.size() > 0 ){
		num_spans++;
		GetFrameSpan( spans, s_fid, e_fid );
		TimeSpan ts;
		ts.start_fid = s_fid;
		ts.end_fid = e_fid;
		object->m_framespans.push_back( ts );
	}
	
	cout << "There are " << num_spans << " spans." << endl;
	for(int i = 0; i < num_spans; ++i)
	{
		cout << "  Span " << i << ":" << object->m_framespans.at(i).start_fid << " - " << object->m_framespans.at(i).end_fid << endl;
	}
}

void xml_loader::XmlLoader4Viper::GetFrameSpan( string& span, int& start, int& end )
{
// 	cout << "Get span from "<< span << endl;
	size_t idx;
	idx = span.find(":");
	string fid;
	if( idx != string::npos )
	{
		fid = span.substr(0, idx);
		start = atoi( fid.c_str() );
		fid = span.substr(idx+1);
		end = atoi( fid.c_str() );		
	}else{
		cout << "frame span does not include semicolon(:), double check!!!" << endl;
		exit(0);
	}
}

void xml_loader::XmlLoader4Viper::GetVisibleElementNum( TrackedObject* object )
{
	cout << "	[GetVisibleElementNum]" << endl;
	int index = 0;
	int num_spans = object->m_framespans.size();
	cout << "num_spans:" << num_spans << endl;
	for (int i = 0; i < num_spans; ++i) 
	{
		int start = object->m_framespans.at(i).start_fid;
		int end = object->m_framespans.at(i).end_fid;
		int diff = end - start + 1;
		index += diff;
// 		cout << "No. " << i << " spans over " << diff << " frames" << endl;
		if( diff == 1 )
		{
			object->m_visible_index.at( start ) = true;
		}
		if( diff > 1 )
		{
			for (int j = 0; j < diff; ++j) {
				object->m_visible_index[ start++ ] = true;
			}
		}	
	}
	
	object->m_visible_num = index;	
}