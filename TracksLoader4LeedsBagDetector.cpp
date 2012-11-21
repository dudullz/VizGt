#include "TracksLoader4LeedsBagDetector.h"

xml_loader::TracksLoader4LeedsBagDetector::TracksLoader4LeedsBagDetector(string& path){
	m_tracks_file_path = path;
	m_num_frames = 3020;
}

xml_loader::TracksLoader4LeedsBagDetector::~TracksLoader4LeedsBagDetector(){
	m_in_file.close();
	
}


void xml_loader::TracksLoader4LeedsBagDetector::LoadFromFile( const char* file_name ){
	m_in_file.open( file_name );
	if( !m_in_file )
	{
		cout << "can not open tracks file:" << file_name << endl;
		exit(0);
	}
}

void xml_loader::TracksLoader4LeedsBagDetector::ReadToTrackedObject( vector<TrackedObject*>& objects  ){
	string inLine;
	cout << "start..." << endl;
	while ( !m_in_file.eof() ) {
		getline( m_in_file, inLine);
		if( inLine.length() > 0 )
		{
			cout << inLine << "\t" << inLine.size() << endl;
			m_num_object++;
			TrackedObject* obj = new TrackedObject();
			
			ParseForTrackedObject( inLine, obj );
			
			cout << endl;
			
			objects.push_back( obj );
		}
	}
	cout << "Total Object Number:" << m_num_object << "(" << objects.size() << ")" << endl;
	
	GetVisibleElementAndIndex( objects );
}

void xml_loader::TracksLoader4LeedsBagDetector::GetVisibleElementAndIndex( vector<TrackedObject*>& objects ){
	cout << "Total Frame Number: " << m_num_frames << endl;
	for (int i = 0; i < objects.size(); ++i) {
		TrackedObject*  obj = objects.at(i);
		obj->m_total_frame_num = m_num_frames;
		
		cout << BASH_ESC_PURPLE << "	Sort out Visible Frames for Object:";
		cout << obj->m_id << BASH_ESC_WHITE << endl;
		
		obj->m_elements_index.resize( m_num_frames + 1, -1 );
		obj->m_visible_index.resize( m_num_frames + 1, false );
		
		int n = obj->m_elements.size();
		obj->m_visible_num = n;	//for iscaps format, every element is visible
		cout << "Element Number:" << n << endl << endl;
		for (int j = 0; j < n; ++j) {
			///NOTE. could also sort out the framespans for this object here
			///I simply choose to do it separately, 
			int fid = obj->m_elements.at(j).frame_id;
			obj->m_elements_index[fid] = j;
			obj->m_visible_index[fid] = true;
		}		
	}
}

void xml_loader::TracksLoader4LeedsBagDetector::GetFrameSpansForTrackedObject( vector<TrackedObject*>& objects ){
}

void xml_loader::TracksLoader4LeedsBagDetector::ParseForTrackedObject( string& line, TrackedObject* obj){
	string str;
	TimeSpan ts;
	size_t idx;
	idx = line.find_first_of(",");
	if( idx != string::npos )
	{
		str = line.substr(0, idx);
		int setNum = atoi( str.c_str() );
//		cout << "	Set Number:" << setNum << endl;
		line = line.substr(idx+1);
	}
	
	idx = line.find_first_of(",");
	if( idx != string::npos )
	{
		str = line.substr(0, idx);
		obj->m_id = atoi( str.c_str() );
//		cout << "	Object id:" << obj->m_id << endl;
		line = line.substr(idx+1);
	}
	
	idx = line.find_first_of(",");
	if( idx != string::npos )
	{
		str = line.substr(0, idx);
		ts.start_fid = atoi( str.c_str() );
//		cout << "	Start fid:" << ts.start_fid << endl;
		line = line.substr(idx+1);
	}
	
	ts.end_fid = atoi( line.c_str() );
//	cout << "	End fid:" << ts.end_fid << endl;
	
	obj->m_framespans.push_back( ts );	
	
	LoadTrackData( obj );	
}

void xml_loader::TracksLoader4LeedsBagDetector::LoadTrackData( TrackedObject* obj ){
	string full_path;
	char name[64];
	sprintf(name, "%d_BB.dat", obj->m_id);
	full_path = m_tracks_file_path + "/" + name;
	ifstream file(full_path.c_str());
	
	if( !file )
	{
		cout << "Can not open track data file:" << full_path << endl;
		exit(0);
	}else{
		cout << "	Load Track Data for " << full_path << endl;
		string line, str;
		int start_fid = 0;
		int end_fid = 0;
		int count = 0;
		int xmin = -1, ymin = -1;
		int xmax = -1, ymax = -1;
		Element elm;
		while ( !file.eof() ) {
			getline( file, line );
			if( line.size() > 0 )
			{
				size_t idx;
				idx = line.find_first_of(",");
				if( idx != string::npos )
				{
					str = line.substr(0, idx);
					elm.frame_id = atoi( str.c_str() );
					line = line.substr(idx+1);
				}
				
				if( count == 0 )
					start_fid = elm.frame_id;
								
				idx = line.find_first_of(",");
				if( idx != string::npos )
				{
					str = line.substr(0, idx);
					xmin = atoi( str.c_str() );
					line = line.substr(idx + 1);					
				}
				
				idx = line.find_first_of(",");
				if( idx != string::npos )
				{
					str = line.substr(0, idx);
					ymin = atoi( str.c_str() );
					line = line.substr(idx + 1);					
				}

				idx = line.find_first_of(",");
				if( idx != string::npos )
				{
					str = line.substr(0, idx);
					xmax = atoi( str.c_str() );
					line = line.substr(idx + 1);					
				}
				
				ymax = atoi( line.c_str() );
				
				elm.x = xmin;	elm.y = ymin;
				elm.width = xmax - xmin + 1;
				elm.height = ymax - ymin + 1;
				obj->m_elements.push_back(elm);				
				
				count++;				
			}
		}
		
		cout << "Tracked frame number:" << count << endl;
		int diff = obj->m_framespans.at(0).end_fid - obj->m_framespans.at(0).start_fid + 1;
		if( diff != count )
		{
			cout << "ooops! miss counted! quit..." << endl;
			cout << obj->m_framespans[0].start_fid << ":" << obj->m_framespans.at(0).end_fid << endl;
			exit(0);
		}
	}
	
	file.close();
	
}