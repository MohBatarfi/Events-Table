#include <iostream>       
#include "ccevent.h"

using namespace std;


CCEvent::CCEvent() {
	id = 0;
	title = "notitle";
	location = "nolocation";
	dateTime = "nodatetime";
	description = "nodescription";
}

CCEvent::CCEvent(int _id, string _title, string _dateTime, string _location, string _description){
	id = _id;
	title = _title;
	location = _location;
	dateTime = _dateTime;
	description = _description;
}

void CCEvent::setId(int i){
        id = i;
}

void CCEvent::setTitle(string t){
	title = t;
}

void CCEvent::setLocation(string loc){
	location = loc;
}

void CCEvent::setDateTime(string dt){
	dateTime = dt;
}

void CCEvent::setDescription(string d){
	description = d;
}

int CCEvent::getId(){
	return id;
}

string CCEvent::getTitle(){
	return title;
}

string CCEvent::getLocation(){
	return location;
}

string CCEvent::getDateTime(){
	return dateTime;
}

string CCEvent::getDescription(){
	return description;
}
