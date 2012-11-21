////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  <cvgDBFactory.cc>                 		                              //
//                                                                            //
//  Description..............                                                 //
//                                                                            //
//  Author    : <longzhen li> (llz)         	                              //
//  Created   : Wed June 28 BST 2006	                                      //
//  Revision  : 1.0 				                              //
//  Copyright : Computational Vision Group(CVG), The University of Reading    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cvgMySQL.h"
#include "cvgDBFactory.h"
#include <ostream>

using namespace std;
using namespace cvg_database;


cvgDBFactory::cvgDBFactory()
{
	pcvgDB = 0;
}

cvgDBFactory::~cvgDBFactory()
{
}

cvg_database::cvgDB*	cvgDBFactory::createDB( db_type tp )
{
	switch(tp){
	case mysql:
		pcvgDB = new cvgMySQL();
		break;
	case oracle:
		// to do
		//pcvgDB = new cvgOracle();
		break;
	default:
		break;
	}
	return pcvgDB;
	
}


