////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  <cvgMySQL.h>                 		                                      //
//                                                                            //
//  Description..............                                                 //
//                                                                            //
//  Author    : <longzhen li> (llz)         	                              //
//  Created   : Wed June 28 BST 2006	                                      //
//  Revision  : 1.0 				                              //
//  Copyright : Computational Vision Group(CVG), The University of Reading    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef __CVG_MYSQL_H__
#define __CVG_MYSQL_H__

#include "cvgDB.h"
#include <mysql++/mysql++.h>
using namespace mysqlpp;

#include <string>
using namespace std;
namespace cvg_database
{
	class cvgMySQL: public cvg_database::cvgDB {
	public:
		cvgMySQL();
		~cvgMySQL();

		bool connectDB(const char* hostname, const char* user_name, const char* pwd, 
				int port, const char* db);
		bool execute( const char* sql_statement);
		void disconnectDB();
		
		unsigned int	getFieldNum(const char* table_name);
		unsigned int	getRecordNum(const char* table_name);
		void	printFieldInfo(const char* table_name);		//incl. field name, SQL type
		bool	insertEntry(const char* table_name, string id, string sender, string sender_type, string xml_data, string insert_time );	//insert new row data
		bool	deleteEntries(const char* table_name, string header, string value);
		bool	deleteEntries(const char* table_name, string header, string value, string header1, string value1);
		bool	deleteLatestEntry(const char* table_name, string header, string value);
		bool	deleteEarliestEntry(const char* table_name, string header, string value);
		bool	updateLatestField(const char* table_name, string identify_header, string identify_value, string update_header, string update_value);
		bool	updateEarliestField(const char* table_name, string identify_header, string identify_value, string update_header, string update_value);

/// fetch single specified field value(string) from latest entry
		bool	getLatestString(const char* table_name, string identify_header, string identify_value, string dst_header, string& dst_value);
/// fetch single specified field value(string) from oldest entry
		bool	getEarliestString(const char* table_name, string identify_header, string identify_value, string dst_header, string& dst_value);


	private:
		bool	getRow(StoreQueryResult& results, int no, Row& row);
		bool	getFieldByNo(Row& row, int no, String& data);
		bool	getFieldByName(Row& row, const char* field_name, String& data);

		Connection conn;
	};
}


#endif	//__CVG_MYSQL_H__

