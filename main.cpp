#include <stdio.h>
#include <iostream>
#define	USE_OPENCV
#include "VizGt.h"

using namespace std;
using namespace viz_gt;

int main( int argc, char* argv[] ){
	if(argc < 2){
		printf("Function: Visualise tracking results (either in SUBITO or ViPER format)\n");
		printf("Usage: %s config_file\n", argv[0]);
		return 1;
	}
	
	VizGt* viz = new VizGt();
	viz->ParseConfigFile( argv[1] );
	
	//load xml file 
	//parse xml file
	//convert to internal data structure, i.e. TrackedObject
#ifdef USE_MYSQL_INPUT
	if( viz->ReadXMLSourceFromDB() )
		viz->LoadDataFromMysql();
	else
#endif
		viz->LoadDataFromXml();
	

	viz->OutputGtTracks();
		
	/// if corresponding files not specified in the config file, the following functions do NOTHING!
	viz->OutputGtToSubitoXml();
	viz->OutputGtToViperXml();
	viz->OutputArToViperXml();
	
	viz->DisplayImage();
	delete viz;
}