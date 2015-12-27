
#ifndef	TRACKED_OBJECT_H
#define	TRACKED_OBJECT_H

#include "MyHeaderFile.h"

typedef struct Element {
	int frame_id;
	int x;	int y;	//top left corner
	int width;
	int height;
    double m_x_3d;
    double m_y_3d;
    double m_z_3d;
	string m_type;	//if object is not tracked, then will have type for each frame
} Element;

typedef struct TimeSpan {
	int start_fid;
	int end_fid;
} TimeSpan;

class TrackedObject{
public:
	int m_id;
	int m_total_frame_num;	//total frame number of the sequence. should be the same for all objs!
							// for Viper format, it equals the real frame number plus 1.
	int m_visible_num;	//number of frames where this object is visible(in a user defined sense). should be m_visible_num <= m_elements.size()
    
	string m_name;
	string m_type;
	/// llz 2012-03-08
	/// the following vector's size = m_total_frame_num.
	/// for certain frame (eg. fid) where this object exists, the value of m_elements_index[fid] gives this object's location index in m_elements
	/// if the object not exist in the frame, m_elements_index[fid] = -1
	vector<int>	m_elements_index;	
	
	/// llz 2012-03-08
	/// the following vector's size = m_total_frame_num.
	/// for certain frame (eg. fid) the value of m_visible_index[fid] indicates whether this object is visible (true) in a user defined sense or not (false)#
	/// such that an existing object can be defined as 'invisible'
	vector<bool> m_visible_index;	
	
	vector<TimeSpan> m_framespans;
	vector<Element>	m_elements;		//the object instances for frames within which this object is detected
	
	TrackedObject();
	~TrackedObject();
	void ClearObject();

};

#endif