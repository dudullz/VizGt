////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  <cvgMySQL.cc>                 		                                      //
//                                                                            //
//  Description..............                                                 //
//                                                                            //
//  Author    : <longzhen li> (llz)         	                              //
//  Created   : Wed June 28 BST 2006	                                      //
//  Revision  : 1.0 				                              //
//  Copyright : Computational Vision Group(CVG), The University of Reading    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



#include <iostream>
#include <iomanip>

#include "cvgMySQL.h"
//#include "cvgMySQLUtil.h"

using namespace std;
using namespace cvg_database;
using namespace mysqlpp;

cvgMySQL::cvgMySQL()
	:cvgDB()
{
//	conn = Connection(true);
}

cvgMySQL::~cvgMySQL()
{
}

bool cvgMySQL::connectDB(const char* hostname, const char* user_name, const char* pwd, 
				int port, const char* db)
{
	if( conn.connect(db, hostname, user_name, pwd, port) ){		
		cerr << "---Database connection successed! " << endl;
		return true;
	}
	else {
		cerr << "---Database connection failed: " << conn.error() << endl;
		return false;
	}
}

bool cvgMySQL::execute(const char* sql_statement)
{
	if (conn){
		Query query = conn.query();
		query << sql_statement;
		StoreQueryResult res = query.store();
		return true;
	}
}

void cvgMySQL::disconnectDB()
{
	if( conn.connected() )
		conn.disconnect();
}

unsigned int	cvgMySQL::getFieldNum(const char* table_name)
{
	if(conn){
		Query query = conn.query();
		query << "select * from " << table_name;
		StoreQueryResult res = query.store();
		return res.field_names()->size();
	}
}

unsigned int	cvgMySQL::getRecordNum(const char* table_name)
{
	if(conn){
		Query query = conn.query();
		query << "select * from " << table_name;
		StoreQueryResult res = query.store();
		return res.size();
	}
}

void	cvgMySQL::printFieldInfo(const char* table_name)
{
	if(conn){
		Query query = conn.query();
		query << "select * from " << table_name;
		StoreQueryResult res = query.store();
		cout << "Field Type Info: " << endl;
		cout << "-- " << "\033[31m" << res.field_names()->size() << "\033[0m" << " --fields in total" << endl;
        	cout.setf(ios::left);
		cout << "\033[35m" << setw(5) << "No." << setw(15) << "Field Name" << setw(25) << "Field Type" << setw(15) << "C++ Type (equal to typeid)"<< "\033[32m" << endl;
		for(unsigned int i = 0; i < res.field_names()->size(); ++i){
			cout << setw(5) << i << setw(15) << res.field_name(i).c_str() << setw(25) << res.field_type(i).sql_name() << setw(15) << res.field_type(i).name() << endl;
		}
		cout << "\033[0m";
	}
}

bool	cvgMySQL::getRow(mysqlpp::StoreQueryResult& results, int row_index, Row& row)
{
	if(results)
		if( row_index >= results.size() ){
			//cerr << "The requested row number is bigger than the total row number!!!" << endl;
			return false;
		}
		else{
			row = results.at( row_index );
			return true;
		}
}

bool	cvgMySQL::getFieldByNo(mysqlpp::Row& row, int field_index, String& data)
{
	if(row)
		if( field_index > row.size() ){
			cerr << "The requested field number is bigger than the total field number!!!" << endl;
			return false;
		}
		else{
			data = row.at( field_index );
			return true;
		}
}

bool	cvgMySQL::getFieldByName(mysqlpp::Row& row, const char* field_name, String& data)
{
	if(row){
		data = row[field_name];
		return true;
	}
}


bool	cvgMySQL::insertEntry(const char* table_name, string id, string sender, string sender_type, string xml_data, string insert_time )
{
	if(conn)
	{
		try{
		//	 Specialized SQL Structure (SSQLS) version of insertion
// 			UoRTrackingResults rowEntry(id, sender, sender_type, "now()", xml_data);
// 			Query query = conn.query();
// 			query.insert(rowEntry);

			Query query = conn.query();
			if( !insert_time.empty() )
				query << "insert into " << table_name 
					<< "(identifier, sender, sender_type, insert_date, xml_data) VALUES (\"" 
					<< id << "\", \"" << sender << "\", \"" << sender_type << "\", \"" << insert_time << "\", \"" 
					<< mysqlpp::escape << xml_data <<"\")";
			else
				query << "insert into " << table_name 
					<< "(identifier, sender, sender_type, insert_date, xml_data) VALUES (\"" 
					<< id << "\", \"" << sender << "\", \"" << sender_type << "\", default, \"" 
					<< mysqlpp::escape << xml_data <<"\")";

// 			if( !sender_type.empty() && !insert_time.empty() && !id.empty() )
// 				query << "insert into " << table_name 
// 					<< "(identifier, sender, sender_type, insert_date, xml_data) VALUES (\"" 
// 					<< id << "\", \"" << sender << "\", \"" << sender_type << "\", \"" << insert_time << "\", \"" 
// 					<< mysqlpp::escape << xml_data <<"\")";
// 			else if( !sender_type.empty() && !insert_time.empty() ){
// 				query << "insert into " << table_name 
// 					<< "(sender, sender_type, insert_date, xml_data) VALUES (\""
// 					<< sender << "\", \"" << sender_type << "\", \"" << insert_time << "\", \"" 
// 					<< mysqlpp::escape << xml_data <<"\")";
// 			}
// 			else if( !sender_type.empty() )
// 				query << "insert into " << table_name 
// 					<< "(sender, sender_type, xml_data) VALUES (\"" 
// 					<< sender << "\", \"" << sender_type <<  "\", \"" 
// 					<< mysqlpp::escape << xml_data <<"\")";
// 			else
// 				query << "insert into " << table_name 
// 					<< "(sender, insert_date, xml_data) VALUES (\"" 
// 					<<sender << "\", default, \"" 
// 					<< mysqlpp::escape << xml_data <<"\")";

			// Show the query about to be executed.
// 			cout << "Query: " << query << endl;
			query.execute();
			return true;
		}
		catch (const BadQuery& er) {
		// Handle any query errors
		cerr << "Query error: " << er.what() << endl;
		return -1;
		}
		catch (const Exception& er) {
		// Catch-all for any other MySQL++ exceptions
		cerr << "Error: " << er.what() << endl;
		return -1;
		}


	}
}

bool	cvgMySQL::deleteEntries(const char* table_name, string header, string value)
{
	if(conn){
		Query query = conn.query();
		query << "delete from " << table_name << " where " << header << "=" << "\"" << value << "\"";
		//cout << "Query: " << query.preview() << endl;
		query.execute();
		
		return true;
	}
}

bool	cvgMySQL::deleteEntries(const char* table_name, string header, string value, string header1, string value1)
{
	if(conn){
		Query query = conn.query();
		query << "delete from " << table_name << " where " << header << "=" << "\"" << value << "\""
			<< " and " << header1 << "=\"" << value1 << "\"";
		//cout << "Query: " << query.preview() << endl;
		query.execute();
		return true;
	}
}

bool	cvgMySQL::deleteLatestEntry(const char* table_name, string header, string value)
{
	if(conn){
		Query query = conn.query();
		query << "delete from " << table_name << " where " << header << "=" << "\"" << value 
			<< "\" order by identifier desc limit 1";
		//cout << "Query: " << query.preview() << endl;
		query.execute();
		return true;
	}
}

bool	cvgMySQL::deleteEarliestEntry(const char* table_name, string header, string value)
{
	if(conn){
		Query query = conn.query();
		query << "delete from " << table_name << " where " << header << "=" << "\"" << value 
			<< "\" order by identifier asc limit 1";
		//cout << "Query: " << query.preview() << endl;
		query.execute();
		return true;
	}
}

bool	cvgMySQL::updateLatestField(const char* table_name, string identify_header, string identify_value, string update_header, string update_value)
{
	if(conn){
		if( !update_value.empty() ){
			Query query = conn.query();
			query << "update " << table_name << " set " << update_header << "=" << "\"" << update_value << "\""
				<< " where " << identify_header << "=" << "\"" << identify_value << "\" order by identifier desc limit 1";
			//cout << "Query: " << query.preview() << endl;
			query.execute();
			return true;
		}
	}
}

bool	cvgMySQL::updateEarliestField(const char* table_name, string identify_header, string identify_value, string update_header, string update_value)
{
	if(conn){
		if( !update_value.empty() ){
			Query query = conn.query();
			query << "update " << table_name << " set " << update_header << "=" << "\"" << update_value << "\""
				<< " where " << identify_header << "=" << "\"" << identify_value << "\" order by identifier asc limit 1";
			//cout << "Query: " << query.preview() << endl;
			query.execute();
			return true;
		}
	}
}

bool	cvgMySQL::getLatestString(const char* table_name, string identify_header, string identify_value, string dst_header, string& dst_value)
{
	if(conn)
	{
		Query query = conn.query();

		query << "select " << dst_header << " from " << table_name 
			<< " where " << identify_header << "=" << "\"" << identify_value << "\" order by identifier desc limit 1";
		cout << "select " << dst_header << " from " << table_name 
				<< " where " << identify_header << "=" << "\"" << identify_value << "\" order by identifier desc limit 1" << endl;
		StoreQueryResult res = query.store();
		//the result should contain only one row with one field
		if( res.size() == 0 )
			return false;
		String s = res[0][0];
		dst_value = s.data();
// 		Row row;
// 		getRow(res, 0, row);
// 
// 		dst_value = row.data();
		return true;
	}
}

bool	cvgMySQL::getEarliestString(const char* table_name, string identify_header, string identify_value, string dst_header, string& dst_value){
	if(conn)
	{
		Query query = conn.query();
		query << "select " << dst_header << " from " << table_name 
			<< " where " << identify_header << "=" << "\"" << identify_value << "\" order by identifier asc limit 1";
		cout << "select " << dst_header << " from " << table_name 
				<< " where " << identify_header << "=" << "\"" << identify_value << "\" order by identifier asc limit 1"<< endl;
		StoreQueryResult res = query.store();
		if( res.size() == 0 )
			return false;
		String s = res[0][0];
		dst_value = s.data();

// 		Row row;
// 		if( getRow(res, 0, row) == false) return false;
// 		dst_value = row.raw_data(0);
		return true;
	}
}