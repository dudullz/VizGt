#include "XmlLoader4Iscaps.h"

xml_loader::XmlLoader4Iscaps::XmlLoader4Iscaps(){
	m_start_entity_tag = false;
	m_is_entity_existed = false;
	
	m_num_object	= 0;
	m_current_fid	= 0;
	m_previous_fid	= -1;
	m_num_frames	= 0;
	m_entity_id		= 0;
	m_num_entity_in_current_frame = 0;
	m_min_x = m_min_y = m_max_x = m_max_y = m_width = m_width = -1;
}

xml_loader::XmlLoader4Iscaps::~XmlLoader4Iscaps(){}

void xml_loader::XmlLoader4Iscaps::LoadFromFile( const char* file_name ){
	cout << file_name << endl;
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

void xml_loader::XmlLoader4Iscaps::ReadToTrackedObject( vector<TrackedObject*>& objects ){
	cout << BASH_ESC_RED << "XmlLoader4Iscaps::ReadToTrackedObject" << BASH_ESC_WHITE << endl;
	
	TrackedObject* obj = NULL;
	Element elem;
	
	while ( xmlTextReaderRead(m_reader) ==  1 ) {
		m_name = xmlTextReaderConstName( m_reader );
		if( m_name == NULL )
			m_name = BAD_CAST "--";
		
		if( !strcmp((char*)m_name, "entityFrame") )
		{			
			m_attri = xmlTextReaderGetAttribute( m_reader, BAD_CAST "frameID" );
//			cout << "Tag Name:" << m_name << endl;
//			cout << "	Frame ID:" << m_attri << endl;
			
			m_current_fid = atoi( (char*)m_attri );
// 			cout<< "	m_previous_fid:" << m_previous_fid << " ? m_current_fid:" << m_current_fid << "" << endl;
			if( m_current_fid == m_previous_fid )
			{
				//finish current entityFrame
//				cout << BASH_ESC_RED << "Entity Number:" << m_num_entity_in_current_frame;
//				cout << BASH_ESC_WHITE << endl << endl;
				//set entity number to zero
				m_num_entity_in_current_frame = 0;
			}else {
				//finish last entityFrame, and start current entityFrame
				
				//entity number in last frame IS zero
				m_num_entity_in_current_frame = 0;
			}
			
//			if( m_current_fid > 38)
//				break;
			
			m_previous_fid = m_current_fid;
		}
		
		if( !strcmp((char*)m_name, "entity") )
		{
			m_start_entity_tag = !m_start_entity_tag;
			if( m_start_entity_tag )
			{
				m_attri = xmlTextReaderGetAttribute( m_reader, BAD_CAST "entityID" );
				m_entity_id = atoi( (char*)m_attri );
				
				m_is_entity_existed  = false;
				for (m_entity_index = 0; m_entity_index < m_existing_ids.size(); ++m_entity_index)
				{
					if( m_entity_id == m_existing_ids.at(m_entity_index) )
					{
						m_is_entity_existed = true;
						break; 
					}
				}
				
				if( m_is_entity_existed )
				{
					obj = objects.at( m_entity_index );
					
				}else{
					//new object
					m_existing_ids.push_back( m_entity_id );
					
					obj = new TrackedObject();
					obj->m_id = m_entity_id;
					objects.push_back( obj );
				}
			}else{
				m_num_entity_in_current_frame++;
			}
		}
		
		if( !strcmp((char*)m_name, "info2D") )
		{
			if( m_start_entity_tag )
			{
				m_attri = xmlTextReaderGetAttribute( m_reader, BAD_CAST "xMin" );
				m_min_x = atoi( (char*)m_attri );
				m_attri = xmlTextReaderGetAttribute( m_reader, BAD_CAST "xMax" );
				m_max_x = atoi( (char*)m_attri );
				m_attri = xmlTextReaderGetAttribute( m_reader, BAD_CAST "yMin" );
				m_min_y = atoi( (char*)m_attri );
				m_attri = xmlTextReaderGetAttribute( m_reader, BAD_CAST "yMax" );
				m_max_y = atoi( (char*)m_attri );
				
				m_width = m_max_x - m_min_x + 1;
				m_height = m_max_y - m_min_y + 1;
				
				Element elem;
				elem.x = m_min_x;	elem.y = m_min_y;
				elem.width = m_width;
				elem.height = m_height;
				elem.frame_id = m_current_fid;
				
				obj->m_elements.push_back( elem );
//				if( m_is_entity_existed )
//				{
//					obj->m_elements.push_back( elem );
//				}else{
//					obj->m_elements.push_back( elem );
//				}
				
//				cout << BASH_ESC_YELLOW << "ID:" << m_entity_id << "	";
//				cout << m_min_x << ":" << m_max_x << "	";
//				cout << m_min_y << ":" << m_max_y << BASH_ESC_WHITE << endl;
			}
		}
	}	

	xmlFreeTextReader( m_reader );
	m_reader = NULL;
	
	cout << "Total object number:" << m_existing_ids.size() << ", double check:" << objects.size() << endl;
	
	GetVisibleElementAndIndex( objects );
	
	GetFrameSpansForTrackedObject( objects );

}

void xml_loader::XmlLoader4Iscaps::GetVisibleElementAndIndex( vector<TrackedObject*>& objects ){
	cout << "Total Frame Number: " << m_current_fid << endl;
	m_num_frames = m_current_fid;
	for (int i = 0; i < objects.size(); ++i) {
		TrackedObject*  obj = objects.at(i);
		obj->m_total_frame_num = m_current_fid;
		
		cout << BASH_ESC_PURPLE << "	Sort out Visible Frames for Object with ID:";
		cout << obj->m_id << BASH_ESC_WHITE << endl;
				
		obj->m_elements_index.resize( m_current_fid + 1, -1 );
		obj->m_visible_index.resize( m_current_fid + 1, false );
		
		int n = obj->m_elements.size();
		obj->m_visible_num = n;	//for iscaps format, every element is visible
		cout << "Element Number:" << n << endl << endl;
		for (int j = 0; j < n; ++j) {
			///NOTE. could also sort out the framespans for this object here
			///I simply choose to do it separately, 
			int fid = obj->m_elements.at(j).frame_id;
			obj->m_elements_index[fid] = j;
			obj->m_visible_index[fid] = true;
						
// 			if(obj->m_id == 14)
// 				cout << "	at:" << fid << endl;
		}		
	}
}

/// llz 2011-07-06 fixed a small bug, which caused by prev_fid=-1 and so on.
void xml_loader::XmlLoader4Iscaps::GetFrameSpansForTrackedObject( vector<TrackedObject*>& objects ){	
	for (int i = 0;  i < objects.size() ; ++i) {
		TrackedObject* obj = objects.at(i);
		int n = obj->m_elements.size();
		
		cout << BASH_ESC_PURPLE << "	Sort out Time Spans for Object with ID:";
		cout << obj->m_id << BASH_ESC_WHITE << endl;
		
		//add single TimeSpan
		if( n == 1 )
		{
			TimeSpan ts;
			ts.start_fid = obj->m_elements.at(0).frame_id;;
			ts.end_fid = obj->m_elements.at(0).frame_id;;
			obj->m_framespans.push_back( ts );
		}
		
		int prev_fid = -1;
		int start_fid;
		for (int j = 0; j < n; ++j) {
			int curr_fid = obj->m_elements.at(j).frame_id;
			
// 			if(obj->m_id == 14)
// 				cout << "curr_fid:" << curr_fid << ", prev_fid:" << prev_fid << "" <<endl;
			if( j == 0 )
			{
				start_fid = curr_fid;
				prev_fid = curr_fid;
				continue;
			}
			
			int diff = curr_fid - prev_fid;
			if( diff > 1 )
			{
// 				if(obj->m_id == 14)
// 					cout << "	Save TimeSpan (start_fid:" << start_fid << ", prev_fid:" << prev_fid << ")" <<endl;

				TimeSpan ts;
				ts.start_fid = start_fid;
				ts.end_fid = prev_fid;
				obj->m_framespans.push_back( ts );
				
				start_fid = curr_fid;
			}
			prev_fid = curr_fid;
			
			//always add the last segment
			if( j == (n-1) )
			{
				TimeSpan ts;
				ts.start_fid = start_fid;
				ts.end_fid = prev_fid;
				obj->m_framespans.push_back( ts );
			}
		}
		
		cout << "The Number of Timespans:" << obj->m_framespans.size() << ", First TimeSpan:" << obj->m_framespans.at(0).start_fid << " - " << obj->m_framespans.at(0).end_fid << endl;
		int total_num = 0;
		for(int s = 0; s < obj->m_framespans.size(); ++s)
		{
			int num = obj->m_framespans.at(s).end_fid - obj->m_framespans.at(s).start_fid + 1;
			total_num += num;			
			
// 			if(obj->m_id == 14)
// 				cout << "	TimeSpan Idx:" << s << ", " << obj->m_framespans.at(s).start_fid << "-" << obj->m_framespans.at(s).end_fid << endl;
		}
		cout << "The Total Number of Elements:" << total_num << endl << endl;
	}
}
