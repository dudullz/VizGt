////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  <cvgDBFactory.h>                 	                                      //
//                                                                            //
//  Description..............                                                 //
//                                                                            //
//  Author    : <longzhen li> (llz)         	                              //
//  Created   : Wed June 28 BST 2006	                                      //
//  Revision  : 1.0 				                              //
//  Copyright : Computational Vision Group(CVG), The University of Reading    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef __CVG_DATABASE_FACTORY_H__
#define __CVG_DATABASE_FACTORY_H__

#include <string>
using namespace std;
#include "cvgDB.h"

namespace cvg_database
{	
	enum db_type { mysql, oracle };
		
class cvgDBFactory{
	
	public:
	cvgDBFactory();
	virtual ~cvgDBFactory();

/*======================================================================*/
/*!
 *   create database object
 *
 *   \param db_type	enum type of database, {mysql, oracle}

 *   \return 		TODO:error code
 */
/*======================================================================*/	
	cvgDB*	createDB( db_type tp );

	int	closeDB();

	private:
		cvgDB*	pcvgDB;
};

}

#endif	// __CVG_DATABASE_H__
