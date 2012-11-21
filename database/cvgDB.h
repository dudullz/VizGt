////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  <cvgDB.h>                 		                                      //
//                                                                            //
//  Description..............                                                 //
//                                                                            //
//  Author    : <longzhen li> (llz)         	                              //
//  Created   : Wed June 28 BST 2006	                                      //
//  Revision  : 1.0 				                              //
//  Copyright : Computational Vision Group(CVG), The University of Reading    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef __CVG_DATABASE_H__
#define __CVG_DATABASE_H__

//#include "dbUtil.h"
#include <string>
using namespace std;
namespace cvg_database
{
	class cvgDB{
		public:
		cvgDB();
		virtual ~cvgDB();

	/**
	*   connect to database
	*
	*   \param hostname	
	*   \param user_name	access username
	*   \param pwd  	access password
	*   \param port		database port, currently default to 3306 for MySQL
	*   \param db		name of the database connected to
	*   \return 		TODO:error code
	*/
	virtual bool connectDB(const char* hostname, const char* user_name, const char* pwd, int port = 3306, const char* db = "communication") = 0;
	virtual bool execute( const char* sql_statement) = 0;
	virtual void disconnectDB() = 0;

/*======================================================================*/
/*!
 *	acquire specified table's info
 */
/*======================================================================*/
	virtual unsigned int	getFieldNum(const char* table_name) = 0;
	virtual unsigned int	getRecordNum(const char* table_name) = 0;
	virtual void	printFieldInfo(const char* table_name) =0;	//incl. field name, SQL type

	/**
	*               add single entry to specified table (comfort to ISCAPS table structure)
	* @param table_name 
	* @param id 
	* @param sender 		possible value, "camera#"
	* @param sender_type 
	* @param xml_data 		xml result data (in XML2 or XML3 format), save to string object
	* @param insert_time 		in the format of YYYY#MM#DD HH:MM::SS, # could be replaced by any delimiter
	*		if empty(recommended), will be set to "firewire" by default by database (TIMESTAMP type in MySQL)
	* @return 	true: success	false:fail
	*/
	virtual bool	insertEntry(const char* table_name, string id, string sender, string sender_type, string xml_data, string insert_time = "" ) = 0;	//insert new row data
	virtual bool	deleteEntries(const char* table_name, string header, string value) = 0;
	virtual bool	deleteEntries(const char* table_name, string header, string value, string header1, string value1)=0;
	virtual bool	deleteLatestEntry(const char* table_name, string header, string value) = 0;
	virtual bool	deleteEarliestEntry(const char* table_name, string header, string value) = 0;


	/**
	* 
	*   update SINGLE field value. Currently ONLY apply for string value (incl. BLOB type)
	*
	*   \param table_name	
	*   \param identify_header		header name used to identify the enties
	*   \param identify_value  		field value
	*   \param update_header/dst_header	field header want to change(could be the same as identify_header)
	*   \param update_value/dst_value	new value 
	
	*   \return 		true: success	false:fail
	*/
	virtual bool	updateLatestField(const char* table_name, string identify_header, string identify_value, string update_header, string update_value) = 0;
	virtual bool	updateEarliestField(const char* table_name, string identify_header, string identify_value, string update_header, string update_value) = 0;

	/**
	* 
	*   fetch SINGLE field value. Currently ONLY apply for string value (incl. BLOB type)
	*
	*   \param table_name		
	*   \param identify_header	header name used to identify the enties
	*   \param identify_value  	field value
	*   \param dst_header		field header want to get value from (could be the same as identify_header)
	*   \param dst_value (out)	save the return value in string reference, passed by function caller
	
	*   \return 		true: success	false:fail
	*/
	virtual bool	getLatestString(const char* table_name, string identify_header, string identify_value, string dst_header, string& dst_value) = 0;
/// return specified field value by string from oldest entry
	virtual bool	getEarliestString(const char* table_name, string identify_header, string identify_value, string dst_header, string& dst_value) = 0;
	};
}

#endif	//	__CVG_DATABASE_H__
