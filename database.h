#ifndef DATABASE_H
#define DATABASE_H
#include "sqlite3.h"
#include <vector>
#include <string>

class Student;
class CCEvent;
class database 
{
	public:
		//database();
		~database();
		database(const char*);
		int createTables();	
		int getID(std::string);
		std::string getName(int);
		
		void toString();
		std::vector<CCEvent> searchEventById(int);
		std::vector<CCEvent> searchEventByName(std::string);
		std::vector<CCEvent> searchEventByLoc(std::string);	
		std::vector<CCEvent> searchEventByDesc(std::string);
		std::vector<CCEvent> searchEventByPreferences(std::string);
//		void searchByName(std::string);
//		void searchByDesc(std::string);
		
		std::vector<Student> searchStudentById(int);
		std::vector<Student> searchStudentByName(std::string);
		std::vector<Student> searchStudentByEmail(std::string);
		std::vector<Student> searchStudentByPreferences(std::string);
		std::vector<Student> searchStudentByCurrentcc(int);
		std::vector<Student> searchStudentByGradsemester(std::string);
		std::vector<Student> searchStudentByGradyear(int);
		
//		void searchByLoc(std::string);
//		void searchById(int);
		
		static int callback(void*, int, char**, char**);
		int getCredits(int);
		std::string getEmail(int);
		std::string getPreferences(int);
		void insertStudentData(std::string, std::string, int, std::string, int, std::string);
		void insertEventData(std::string, std::string, std::string, std::string);
		
		int rowsInEventTable();
		
		int rowsInStudentTable();
		bool checkEmptyStudentTable();

		bool checkEmptyEventTable();

	private:
		sqlite3* db;
		char* errmsg;


};

#endif
