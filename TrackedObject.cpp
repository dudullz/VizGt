#include "TrackedObject.h"

TrackedObject::TrackedObject()
{
	//these initialisation is important!!!
	m_id = -1;
	m_total_frame_num = 0;
	m_visible_num = 0;
    
	if( m_name.size() > 0 )
		m_name.clear();
	if( m_type.size() > 0 )
		m_type.clear();
}

TrackedObject::~TrackedObject(){
	ClearObject();
}

void TrackedObject::ClearObject()
{
	m_name.clear();
	m_type.clear();
	m_elements_index.clear();
	m_visible_index.clear();
	m_framespans.clear();
	m_elements.clear();
}