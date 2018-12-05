#ifndef ccevent_h
#define ccevent_h

#include <string>
#include <set>

#include "database.h"

class CCEvent {
private:
	int id;
	std::string title;
	std::string location;
	std::string dateTime;
	std::string description;

public:
	CCEvent();
	CCEvent(int _id, std::string _title, std::string _dateTime, std::string _location,  std::string _description);
	void setId(int i);
	void setTitle(std::string t);
	void setLocation(std::string loc);
	void setDateTime(std::string dt);
	void setDescription(std::string d);

	int getId();
	std::string getTitle();
	std::string getLocation();
	std::string getDateTime();
	std::string getDescription();
};

#endif	
