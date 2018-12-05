#include <iostream>
#include <string>
#include <sstream>
#include "sqlite3.h"

using namespace std;

sqlite3 *db;

// This is the callback function to display the selected data in the table
static int callback(void *NotUsed, int argc, char **argv, char **szColName)
{
	for (int i = 0; i < argc; i++)
	{
		cout << szColName[i] << ": " << argv[i] << endl;
	}

	cout << "\n";

	return 0;
}

// return is the table has values and how many
int rowsIntable()
{
    int count=0;
    sqlite3_stmt * stmt;
    sqlite3_prepare( db, "SELECT * from EventsTable;", -1, &stmt, NULL );//preparing the statement
       sqlite3_step( stmt );//executing the statement

       while( sqlite3_column_text( stmt, 0 ) )
       {
            count++;
           sqlite3_step( stmt );
       }
    return count;

}

bool checkEmptyTable()
{
    int c=rowsIntable();
    if(c==0)
        return true;

    return false;
}

//search by name
void searchByName(string word)
{
    char *szErrMsg = 0;
    string s="select * from EventsTable where name='"+word+"'";
    /* Execute SQL statement */
   int rc = sqlite3_exec(db, s.c_str(), callback, NULL, &szErrMsg);

   if( rc != SQLITE_OK ) {
      cout<< "SQL error:"<<szErrMsg;
      sqlite3_free(szErrMsg);
   }
   else
   {
      //cout<< "Operation done successfully\n";
   }

}

//search by loc
void searchByLoc(string word)
{
    char *szErrMsg = 0;
    string s="select * from EventsTable where location='"+word+"'";
    /* Execute SQL statement */
   int rc = sqlite3_exec(db, s.c_str(), callback, NULL, &szErrMsg);

   if( rc != SQLITE_OK ) {
      cout<< "SQL error:"<<szErrMsg;
      sqlite3_free(szErrMsg);
   }
   else
   {
      //cout<< "Operation done successfully\n";
   }

}

//search by desc
void searchByDesc(string word)
{
    char *szErrMsg = 0;
    string s="select * from EventsTable where description='"+word+"'";
    /* Execute SQL statement */
   int rc = sqlite3_exec(db, s.c_str(), callback, NULL, &szErrMsg);

   if( rc != SQLITE_OK ) {
      cout<< "SQL error:"<<szErrMsg;
      sqlite3_free(szErrMsg);
   }
   else
   {
      //cout<< "Operation done successfully\n";
   }

}

void insertData(int id,string name,string time,string loc,string desc)
{
    std::ostringstream ss;
     ss << id;
    string query="INSERT INTO EventsTable(id, name, date_and_time, location, description) VALUES (";
    query.append(ss.str());
    query.append(",'"+name+"','");
    query.append(time+"','");
    query.append(loc+"','");
    query.append(desc+"')");


    cout<<query<<endl;
    char *szErrMsg = 0;
        int rc = sqlite3_exec(db,query.c_str() , NULL, NULL, &szErrMsg);
		if (rc != SQLITE_OK)
		{
			cout << "SQL Error chk: " << szErrMsg << std::endl;
			sqlite3_free(szErrMsg);

		}

}

int main()
{

	char *szErrMsg = 0;
	int retval;

	// open database
	int rc = sqlite3_open("Events1.db", &db);

	if (rc)
		cout << "Can't open or create database\n";
	else
		cout << "Open database successfully\n";

	// Create table Events
	const char *szSQL = "CREATE TABLE EventsTable (" \
	"id	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE," \
	"name	TEXT NOT NULL," \
	"date_and_time	TEXT NOT NULL," \
	"location	TEXT NOT NULL," \
	"description	TEXT );";

	// Insert data

	const char *pSQL[5];
	pSQL[0] = "INSERT or REPLACE INTO EventsTable(id, name, date_and_time, location, description) VALUES (1, 'Event1', 'Apr 18, 2pm - 3pm', 'Elizabeth Hall 205', 'The first events description')";
	pSQL[1] = "INSERT or REPLACE INTO EventsTable(id, name, date_and_time, location, description) VALUES (2, 'Event2', 'Apr 19, 11am', 'Sage Hall 301', 'The second events description')";
	pSQL[2] = "INSERT or REPLACE INTO EventsTable(id, name, date_and_time, location, description) VALUES (3, 'Event3', 'Apr 20, 10am - 11am', 'Library', 'The third events description')";
	pSQL[3] = "INSERT or REPLACE INTO EventsTable(id, name, date_and_time, location, description) VALUES (4, 'Event4', 'Apr 22, 9am', 'Library', 'The fourth events description')";
	pSQL[4] = "INSERT or REPLACE INTO EventsTable(name, date_and_time, location, description) VALUES ('Event5', 'Apr 28, 10am', 'Library', 'The fifth events description')";


	cout<<"Total rows in table:"<<rowsIntable()<<endl;

	insertData(61,"name6","Apr 22, 9am","library","desc");

    cout<<"Searching with name6"<<endl;
    searchByName("name6");

    cout<<"Searching with location Library"<<endl;
    searchByLoc("Library");

     cout<<"Searching with description(desc)"<<endl;
    searchByDesc("desc");


	// Close database
	if (db)
	{
		sqlite3_close(db);
	}

 	return 0;
}
