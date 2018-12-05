#include "student.h"
#include "ccevent.h"
#include "sqlite3.h"
#include <vector>
#include <iostream>
#include <cstring>
#include <sstream>
using namespace std;

//takes string filePath which is the path to the MyCultural.db in MyCultural.Events, including trailing slash. I.E: "MyCultural.Events/"
database::database(const char* filePath)
{
	int retval = 0;
	retval = sqlite3_open(filePath, &db);
	if (retval != SQLITE_OK)
	{
		cout << "Cannot open MyCultural.db: " << sqlite3_errcode(db) << endl;
	}

	retval = createTables();



}

database::~database()
{
	sqlite3_close(db);
}

int database::createTables()
{
	int retval = sqlite3_exec(db,"CREATE TABLE IF NOT EXISTS StudentsTable (" \
			"id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE," \
			"name TEXT," \
			"email TEXT," \
			"currentCC INTEGER," \
			"gradSemester TEXT," \
			"gradYear INTEGER," \
			"preferences TEXT);", NULL, NULL, &errmsg);
	if(retval != SQLITE_OK)
	{
		cout << "Error in creating students: " << errmsg << endl;
		sqlite3_free(errmsg);
	}

	retval = sqlite3_exec(db,"CREATE TABLE IF NOT EXISTS EventTable (" \
			"id	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE," \
			"name	TEXT NOT NULL," \
			"date_and_time	TEXT NOT NULL," \
			"location	TEXT NOT NULL," \
			"description	TEXT );", NULL, NULL, &errmsg);
	if(retval != SQLITE_OK)
	{
		cout << "Error in creating events: " << errmsg << endl;
		sqlite3_free(errmsg);
	}
	return retval;
}

int database::callback(void *NotUsed, int argc, char **argv, char **szColName)
{
	for (int i = 0; i < argc; i++)
	{
		cout << szColName[i] << ": " << argv[i] << endl;
	}

	cout << "\n";

	return 0;
}


void database::insertStudentData(string name, string email, int currentCC , string gradSemester, int gradYear, string preferences)
{

	std::ostringstream ss;
	string query="INSERT INTO StudentsTable(name, email, currentCC, gradSemester, gradYear, preferences) VALUES (";
	query.append("'"+name+"','");
	query.append(email+"',");
	ss << currentCC;
	query.append(ss.str()+ ",'");
	query.append(gradSemester+"',");
	ss.str("");
	ss << gradYear;
	query.append(ss.str() + ",");
	query.append("'" + preferences + "')");

	char *szErrMsg = 0;
	int rc = sqlite3_exec(db,query.c_str() , NULL, NULL, &szErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL Error chk: " << szErrMsg << endl;
		sqlite3_free(szErrMsg);

	}
}

void database::insertEventData(string name,string time,string loc,string desc)
{

	sqlite3_stmt * stmt;
	vector<CCEvent> v = searchEventByName(name);
	for (int i = 0; i < v.size(); i++)
	{
		if(name == v[i].getTitle() && time == v[i].getDateTime() && loc == v[i].getLocation()){
			return;
		}
	} 
	string query="INSERT INTO EventTable(name, date_and_time, location, description) VALUES (?,?,?,?)";
	const char *szErrMsg = 0;
	int rc = sqlite3_prepare(db,query.c_str(), -1, &stmt, &szErrMsg);

	if( rc == SQLITE_OK ) {
 
		sqlite3_bind_text(stmt, 1, name.c_str(), -1, NULL);
		sqlite3_bind_text(stmt, 2, time.c_str(), -1, NULL);
		sqlite3_bind_text(stmt, 3, loc.c_str(), -1, NULL);
		sqlite3_bind_text(stmt, 4, desc.c_str(), -1, NULL);
	}



//	int rc = sqlite3_prepare(db,query.c_str(), -1, &stmt, &szErrMsg);

	while ( (rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		(sqlite3_column_text(stmt, 1));
		(sqlite3_column_text(stmt, 2));
		(sqlite3_column_text(stmt, 3));
		(sqlite3_column_text(stmt, 4));
	}

	sqlite3_finalize(stmt);
	
}


// return is the table has values and how many
int database::rowsInStudentTable ( )
{
	int count=0;
	sqlite3_stmt * stmt;
	sqlite3_prepare( db, "SELECT * from StudentsTable;", -1, &stmt, NULL );//preparing the statement
	sqlite3_step( stmt );//executing the statement

	while( sqlite3_column_text( stmt, 0 ) )
	{
		count++;
		sqlite3_step( stmt );
	}
	return count;

}

// return is the table has values and how many
int database::rowsInEventTable()
{
	int count=0;
	sqlite3_stmt * stmt;
	sqlite3_prepare( db, "SELECT * from EventTable;", -1, &stmt, NULL );//preparing the statement
	sqlite3_step( stmt );//executing the statement

	while( sqlite3_column_text( stmt, 0 ) )
	{
		count++;
		sqlite3_step( stmt );
	}
	return count;

}

//Check if the table is empty
bool database::checkEmptyEventTable()
{
	int c=rowsInEventTable();
	if(c==0)
		return true;

	return false;
}

// checks if the tables is empty
bool database::checkEmptyStudentTable ( )
{
	int c=rowsInStudentTable();
	if(c==0)
		return true;

	return false;
}



vector<Student> database::searchStudentById ( int id )
{
	vector<Student> v;
	sqlite3_stmt *stmt;
	const char *pzTest;

	string s = "select * from StudentsTable where id=?";
	int rc = sqlite3_prepare(db, s.c_str(), -1, &stmt, &pzTest);
	int x = (sqlite3_bind_int(stmt, 1, id));
	if (x != SQLITE_OK) {
		cout << x << "      did not work 2.5" << endl;
	}

	string tempName;
	string tempEmail;
	string tempSemester;
	string tempPrefs;
	int tempId;
	int tempCC;
	int tempYear;

	while ( (rc = sqlite3_step(stmt)) == SQLITE_ROW) {

		tempName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		tempEmail = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))));
		tempSemester =(const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))));
		tempPrefs = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6))));

		tempId = sqlite3_column_int(stmt, 0);
		tempCC = sqlite3_column_int(stmt, 3);
		tempYear = sqlite3_column_int(stmt, 5);
	}

	Student stud(tempName,tempEmail, tempId , tempSemester, tempYear, tempCC, tempPrefs);
	v.push_back(stud);
	sqlite3_finalize(stmt);
	return v;




}
// search for studnets with the same name
vector<Student> database::searchStudentByName(string word )
{
	vector<Student> v;
	sqlite3_stmt *stmt;
	const char *pzTest;

	string s = "select * from StudentsTable where name=?";

	int rc = sqlite3_prepare(db, s.c_str(), -1, &stmt, &pzTest);
	if (sqlite3_bind_text(stmt, 1, word.c_str(), -1, NULL) != SQLITE_OK) {
		cout << "did not work" << endl;
	}
	string tempName;
	string tempEmail;
	string tempSemester;
	string tempPrefs;
	int tempId;
	int tempCC;
	int tempYear;
	while ( (rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		tempName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		tempEmail = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))));
		tempSemester =(const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))));
		tempPrefs = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6))));
		tempId = sqlite3_column_int(stmt, 0);
		tempCC = sqlite3_column_int(stmt, 3);
		tempYear = sqlite3_column_int(stmt, 5);
	}
	Student stud(tempName,tempEmail, tempId , tempSemester, tempYear, tempCC, tempPrefs);
	v.push_back(stud);
	sqlite3_finalize(stmt);
	return v;

}


// search students with the same email, " i mean its impossible :P "
vector<Student> database::searchStudentByEmail(string word )
{
	vector<Student> v;
	sqlite3_stmt *stmt;
	const char *pzTest;

	string s = "select * from StudentsTable where email=?";

	int rc = sqlite3_prepare(db, s.c_str(), -1, &stmt, &pzTest);
	if (sqlite3_bind_text(stmt, 1, word.c_str(), -1, NULL) != SQLITE_OK) {
		cout << "did not work" << endl;
	}
	//Read each row
	string tempName;
	string tempEmail;
	string tempSemester;
	string tempPrefs;
	int tempId;
	int tempCC;
	int tempYear;
	while ( (rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		tempName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		tempEmail = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))));
		tempSemester =(const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))));
		tempPrefs = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6))));
		tempId = sqlite3_column_int(stmt, 0);
		tempCC = sqlite3_column_int(stmt, 3);
		tempYear = sqlite3_column_int(stmt, 5);
	}
	Student stud(tempName,tempEmail, tempId , tempSemester, tempYear, tempCC, tempPrefs);
	v.push_back(stud);
	sqlite3_finalize(stmt);
	return v;


}
// search students with the same preferences " music, sports ... etc "
vector<Student> database::searchStudentByPreferences(string word )
{
	vector<Student> v;
	sqlite3_stmt *stmt;
	const char *pzTest;

	string s = "select * from StudentsTable where preferences=?";

	int rc = sqlite3_prepare(db, s.c_str(), -1, &stmt, &pzTest);
	if (sqlite3_bind_text(stmt, 1, word.c_str(), -1, NULL) != SQLITE_OK) {
		cout << "did not work" << endl;
	}
	//Read each row
	string tempName;
	string tempEmail;
	string tempSemester;
	string tempPrefs;
	int tempId;
	int tempCC;
	int tempYear;
	while ( (rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		tempName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		tempEmail = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))));
		tempSemester =(const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))));
		tempPrefs = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6))));
		tempId = sqlite3_column_int(stmt, 0);
		tempCC = sqlite3_column_int(stmt, 3);
		tempYear = sqlite3_column_int(stmt, 5);
	}
	Student stud(tempName,tempEmail, tempId , tempSemester, tempYear, tempCC, tempPrefs);
	v.push_back(stud);
	sqlite3_finalize(stmt);
	return v;

}

vector<Student> database::searchStudentByCurrentcc(int cc)
{
	vector<Student> v;
	sqlite3_stmt *stmt;
	const char *pzTest;

	string s = "select * from StudentsTable where currentCC=?";

	int rc = sqlite3_prepare(db, s.c_str(), -1, &stmt, &pzTest);
	if (sqlite3_bind_int(stmt, 1, cc) != SQLITE_OK) {
		cout << "did not work" << endl;
	}

	//Read each row
	string tempName;
	string tempEmail;
	string tempSemester;
	string tempPrefs;
	int tempId;
	int tempCC;
	int tempYear;
	while ( (rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		tempName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		tempEmail = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))));
		tempSemester =(const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))));
		tempPrefs = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6))));
		tempId = sqlite3_column_int(stmt, 0);
		tempCC = sqlite3_column_int(stmt, 3);
		tempYear = sqlite3_column_int(stmt, 5);
	}

	Student stud(tempName,tempEmail, tempId , tempSemester, tempYear, tempCC, tempPrefs);
	v.push_back(stud);
	sqlite3_finalize(stmt);
	return v;

}

// search students with the same semester graduation
vector<Student> database::searchStudentByGradsemester(string word )
{
	vector<Student> v;
	sqlite3_stmt *stmt;
	const char *pzTest;

	string s = "select * from StudentsTable where gradSemester=?";

	int rc = sqlite3_prepare(db, s.c_str(), -1, &stmt, &pzTest);
	if (sqlite3_bind_text(stmt, 1, word.c_str(), -1, NULL) != SQLITE_OK) {
		cout << "did not work" << endl;
	}
	//Read each row
	string tempName;
	string tempEmail;
	string tempSemester;
	string tempPrefs;
	int tempId;
	int tempCC;
	int tempYear;
	while ( (rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		tempName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		tempEmail = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))));
		tempSemester =(const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))));
		tempPrefs = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6))));
		tempId = sqlite3_column_int(stmt, 0);
		tempCC = sqlite3_column_int(stmt, 3);
		tempYear = sqlite3_column_int(stmt, 5);
	}
	Student stud(tempName,tempEmail, tempId , tempSemester, tempYear, tempCC, tempPrefs);
	v.push_back(stud);
	sqlite3_finalize(stmt);
	return v;

}

// search students with the same graduation year
vector<Student> database::searchStudentByGradyear(int year)
{
	vector<Student> v;
	sqlite3_stmt *stmt;
	const char *pzTest;

	string s = "select * from StudentsTable where gradYear=?";

	int rc = sqlite3_prepare(db, s.c_str(), -1, &stmt, &pzTest);
	if (sqlite3_bind_int(stmt, 1, year) != SQLITE_OK) {
		cout << "did not work" << endl;
	}
	//Read each row
	string tempName;
	string tempEmail;
	string tempSemester;
	string tempPrefs;
	int tempId;
	int tempCC;
	int tempYear;
	while ( (rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		tempName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		tempEmail = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))));
		tempSemester =(const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))));
		tempPrefs = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6))));
		tempId = sqlite3_column_int(stmt, 0);
		tempCC = sqlite3_column_int(stmt, 3);
		tempYear = sqlite3_column_int(stmt, 5);
	}

	Student stud(tempName,tempEmail, tempId , tempSemester, tempYear, tempCC, tempPrefs);
	v.push_back(stud);
	sqlite3_finalize(stmt);
	return v;

}


vector<CCEvent> database::searchEventByPreferences(string word)
{
	vector<CCEvent> v;
	sqlite3_stmt *stmt;
	string s = "select * from EventTable;";
        int rc = sqlite3_prepare(db, s.c_str(), -1, &stmt, NULL );//preparing the statement
	string tempName;
        string tempDate;
        string tempLocation;
        string tempDesc;
        int tempId;
        while ( (rc = sqlite3_step(stmt)) == SQLITE_ROW) {
                tempName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                tempDate = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))));
                tempLocation =(const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))));
                tempDesc = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))));
                tempId = sqlite3_column_int(stmt, 0);
		CCEvent event(tempId, tempName,tempDate, tempLocation, tempDesc);
		v.push_back(event);
        }
	sqlite3_finalize(stmt);

	//v = searchEventById(rowsInEventTable());
	
	for (std::vector<CCEvent>::iterator it = v.begin(); it != v.end(); ++it)
	{
		string tempDesc = it->getDescription();
		if (tempDesc.find(word) == -1)
		{
			v.erase(it);
		}
	}
	
        return v;

}

//search by event name
vector<CCEvent> database::searchEventByName(string word)
{
	vector<CCEvent> v;
	sqlite3_stmt *stmt;
	const char *pzTest;

	string s = "select * from EventTable where name=?";

	int rc = sqlite3_prepare(db, s.c_str(), -1, &stmt, &pzTest);
	if (sqlite3_bind_text(stmt, 1, word.c_str(), -1, NULL) != SQLITE_OK) {
		cout << "did not work" << endl;
	}
	//Read each row
	string tempName;
	string tempDate;
	string tempLocation;
	string tempDesc;
	int tempId;
	while ( (rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		tempName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		tempDate = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))));
		tempLocation =(const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))));
		tempDesc = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))));
		tempId = sqlite3_column_int(stmt, 0);
	}
	CCEvent event(tempId, tempName,tempDate, tempLocation, tempDesc);
	v.push_back(event);
	sqlite3_finalize(stmt);
	return v;

}

//search by event location
vector<CCEvent> database::searchEventByLoc(string word)
{
	vector<CCEvent> v;
	sqlite3_stmt *stmt;
	const char *pzTest;

	string s = "select * from EventTable where location=?";
	int rc = sqlite3_prepare(db, s.c_str(), -1, &stmt, &pzTest);
	if (sqlite3_bind_text(stmt, 1, word.c_str(), -1, NULL) != SQLITE_OK) {
		cout << "did not work" << endl;
	}
	//Read each row
	string tempName;
	string tempDate;
	string tempLocation;
	string tempDesc;
	int tempId;
	while ( (rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		tempName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		tempDate = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))));
		tempLocation =(const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))));
		tempDesc = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))));
		tempId = sqlite3_column_int(stmt, 0);
	}
	CCEvent event(tempId, tempName,tempDate, tempLocation, tempDesc);
	v.push_back(event);
	sqlite3_finalize(stmt);
	return v;

}

//search by description
vector<CCEvent> database::searchEventByDesc(string word)
{
	vector<CCEvent> v;
	sqlite3_stmt *stmt;
	const char *pzTest;

	string s = "select * from EventTable where description=?";
	int rc = sqlite3_prepare(db, s.c_str(), -1, &stmt, &pzTest);
	if (sqlite3_bind_text(stmt, 1, word.c_str(), -1, NULL) != SQLITE_OK) {
		cout << "did not work" << endl;
	}
	//Read each row
	string tempName;
	string tempDate;
	string tempLocation;
	string tempDesc;
	int tempId;
	while ( (rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		tempName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		tempDate = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))));
		tempLocation =(const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))));
		tempDesc = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))));
		tempId = sqlite3_column_int(stmt, 0);
	}
	CCEvent event(tempId, tempName,tempDate, tempLocation, tempDesc);
	v.push_back(event);
	sqlite3_finalize(stmt);
	return v;



}



// search by id
vector<CCEvent> database::searchEventById(int i)
{

	vector<CCEvent> v;
	sqlite3_stmt *stmt;
	const char *pzTest;

	string s = "select * from EventTable where id=?";

	int rc = sqlite3_prepare(db, s.c_str(), -1, &stmt, &pzTest);
	if (sqlite3_bind_int(stmt, 1, i) != SQLITE_OK) {
		cout << "did not work" << endl;
	}
	//Read each row
	string tempName;
	string tempDate;
	string tempLocation;
	string tempDesc;
	int tempId;
	while ( (rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		tempName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		tempDate = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))));
		tempLocation =(const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))));
		tempDesc = (const_cast<char*>(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))));
		tempId = sqlite3_column_int(stmt, 0);
	}
	CCEvent event(tempId, tempName,tempDate, tempLocation, tempDesc);
	v.push_back(event);
	sqlite3_finalize(stmt);
	return v;


}



