////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  <headerfile.h>    		                                              //
//                                                                            //
//  Description..............                                                 //
//                                                                            //
//  Author    : <fullname> (<initials>)                                       //
//  Created   : BST 2006                                  		      //
//  Revision  :                            				      //
//  Copyright : Computational Vision Group(CVG), The University of Reading    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

//the time to expand macro is extensive... therefore not recommended!
/// not be used for now (13/07/06)

#ifndef __CVG_MYSQL_UTIL_H__
#define __CVG_MYSQL_UTIL_H__

#include <mysql++/mysql++.h>
#include <mysql++/custom.h>
#include <string>
using namespace std;
using namespace mysqlpp;

sql_create_5(UoRTrackingResults, 1, 5, int, id, string, sender,string, sender_type,DateTime, insert_date,string, xml_data)


#endif		// __CVG_MYSQL_UTIL_H__