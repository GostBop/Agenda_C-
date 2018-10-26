#include "AgendaUI.hpp"
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;
AgendaUI::AgendaUI() {
  m_userName = "";
  m_userPassword = "";
  m_agendaService.startAgenda();
}

void AgendaUI::OperationLoop(void) {
  if(m_userName == "") {
    cout << "-------------------------------------Agenda-------------------------------------\n"
         << "Action :\n"
         << "l    - log in Agenda by user name and password\n"
         << "r    - register an Agenda account\n"
         << "q    - quit Agenda\n"
         << "--------------------------------------------------------------------------------\n"
         <<"\n";
  } else {
    cout << "-------------------------------------Agenda-------------------------------------\n"
         << "Action :\n"
         << "o    - log out Agenda\n"
         << "dc   - delete Agenda account\n"
         << "lu   - list all Agenda user\n"
         << "cm   - create a meeting\n"
         << "amp  - add meeting participator\n"
         << "rmp  - remove meeting participator\n"
         << "rqm  - request to quit meeting\n"
         << "la   - list all meetings\n"
         << "las  - list all sponsor meetings\n"
         << "lap  - list all participator meetings\n"
         << "qm   - query meeting by title\n"
         << "qt   - query meeting by time interval\n"
         << "dm   - delete meeting by title\n"
         << "da   - delete all meetings\n"
         << "--------------------------------------------------------------------------------\n"
         <<"\n";
  }
  startAgenda();
}

void AgendaUI::startAgenda(void) {
  if(m_userName == "") {
    cout << "Agenda :~$ ";
    if(!executeOperation(getOperation())) {
      OperationLoop();
    }
  } else {
    cout <<"Agenda@" << m_userName << " :~# ";
    if(!executeOperation(getOperation())) {
      OperationLoop();
    }
  }
}

std::string AgendaUI::getOperation() {
    string operation;
    cin >> operation;
    return operation; 
}


bool AgendaUI::executeOperation(const std::string &t_operation) {
  if(t_operation == "l" && m_userName == "") {
    userLogIn();
    return true;
  } else if(t_operation == "r" && m_userName == "") {
    userRegister();
    return true;
  } else if(t_operation == "q" && m_userName == "") {
    quitAgenda();
    return true;
  } else if(t_operation == "o") {
    userLogOut();
    return true;
  } else if(t_operation == "dc") {
  	deleteUser();
    return true;
  } else if(t_operation == "lu") {
    listAllUsers();
    return true;
  } else if(t_operation == "cm") {
    createMeeting();
    return true;
  } else if(t_operation == "amp") {
  	addMeetingParticipator();
  	return true;
  } else if(t_operation == "rmp") {
  	removeMeetingParticipator();
  	return true;
  } else if(t_operation == "rqm") {
    quitMeeting();
    return true;
  } else if(t_operation == "la") {
  	listAllMeetings();
  	return true;
  } else if(t_operation == "las") {
  	listAllSponsorMeetings();
  	return true;
  } else if(t_operation == "lap") {
  	listAllParticipateMeetings();
  	return true;
  } else if(t_operation == "qm") {
  	queryMeetingByTitle();
  	return true;
  } else if(t_operation == "qt") {
  	queryMeetingByTimeInterval();
  	return true;
  } else if(t_operation == "dm") {
  	deleteMeetingByTitle();
  	return true;
  } else if(t_operation == "da") {
  	deleteAllMeetings();
  	return true;
  }
  return false;
}

void AgendaUI::userLogIn(void) {
  cout << "[log in] [username] [password]\n"
       << "[log in] ";
  string username, password;
  cin >> username >> password;
  try{
    m_agendaService.userLogIn(username, password);
  }
  catch(int) {
    cout << "[log in] user doesn't exist\n";
    OperationLoop();
    return;
  }
  catch(double) {
    cout << "[log in] Password Error!\n";
    OperationLoop();
    return;
  } 
  m_userName = username;
  m_userPassword = password;
  cout << "[log in] succeed!\n"
       << "\n";
  OperationLoop();
}

bool isvalid(string email) {
  if(email.size() < 7) return false;
  if(email.substr(email.size() - 4) != ".com") return false;
  int ai;
  for(int i = 0; i < email.size(); i++) {
    if(email[i] == '@') {
      ai = i;
      break;
    }
  }
  if(ai == 0 || ai == email.size() - 5) return false;
  string s1 = email.substr(0, ai);
  for(int i = 0; i < s1.size(); i++) {
    if(! ( (s1[i] >= '0' && s1[i] <= '9') || (s1[i] >= 'A' && s1[i] <= 'Z') || (s1[i] >= 'a' && s1[i] <= 'z') ) ){
      return false;
    }
  }
  email.erase(0, ai + 1);
  email.erase(email.size() - 4);
  for(int i = 0; i < email.size(); i++) {
     if(! ( (email[i] >= '0' && email[i] <= '9') || (email[i] >= 'A' && email[i] <= 'Z') || (email[i] >= 'a' && email[i] <= 'z') ) ){
      return false;
    }
  }
  return true; 
}

bool Isvalid(string phone) {
  if(phone.size() != 11) return false;
  for(int i = 0; i < phone.size(); i++) {
    if(! (phone[i] >= '0' && phone[i] <= '9') ) {
      return false;
    }
  }
  return true;
}

void AgendaUI::userRegister(void) {
  cout << "[register] [username] [password] [email(x..x@x..x.com)] [phone(11 bits)]\n"
       << "[register] ";
  string username, password, email, phone;
  cin >> username >> password >> email >> phone;
  while(username.size() > 15 || password.size() > 15 || !isvalid(email) || !Isvalid(phone)) {
    if(username.size() > 15) {
      cout << "[register] Your userName is too long, please input a new one(<=15)\n"
           << "[register] userName : ";
      cin >> username;
    }
    if(password.size() > 15) {
      cout << "[register] Your password is too long, please input a new one(<=15)\n"
           << "[register] password : ";
      cin >> password;
    }
    if(!isvalid(email)) {
      cout << "[register] Your email is not valid, please input a new one(x..x@x..x.com)\n"
           << "[register] email : ";
      cin >> email;
    }
    if(!Isvalid(phone)) {
      cout << "[register] Your phone is not valid, please input a new one\n"
           << "[register] phone : ";
      cin >> phone;
    }
  }
  if(m_agendaService.userRegister(username, password, email, phone)) {
    cout << "[register] succeed!\n"
         << "\n";
  }
  else {
    cout << "[register] This username has been registered!\n";
  }
  OperationLoop();
}

void AgendaUI::userLogOut(void) {
  m_userName = "";
  m_userPassword = "";
  OperationLoop();
}

void AgendaUI::quitAgenda(void) {
  m_agendaService.quitAgenda();
}

void AgendaUI::deleteUser(void) {
  m_agendaService.deleteUser(m_userName, m_userPassword);
  m_userName = "";
  m_userPassword = "";
  cout << "[delete Agenda account] succeed!\n";
  OperationLoop();
}

void AgendaUI::listAllUsers(void) {
  list<User> user = m_agendaService.listAllUsers();
  cout << "[list all users]\n"
       << "\n"
       << "name\temali\tphone\n";
  for(auto it = user.begin(); it != user.end(); it++) {
    cout << it -> getName() << "\t" << it -> getEmail() << "\t" << it -> getPhone() << "\n";
  }
  OperationLoop();
}

void AgendaUI::createMeeting(void) {
  int num;
  cout << "[create meeting] [the number of participators]\n"
       << "[create meeting] ";
  cin >> num;
  vector<string> participators;
  for(int i = 1; i <= num; i++) {
    string participator;
    cout << "[create meeting] [please enter the participator " << i << " ]\n"
         << "[create meeting] ";
    cin >> participator;
    while(participator == m_userName) {
      cout << "[create meeting] Do you want to add yourself?\n";
      cout << "[create meeting] [please enter the participator " << i << " ]\n"
           << "[create meeting] ";
      cin >> participator;
    }
    if(i != 1) {
      auto it = find(participators.begin(), participators.end(), participator);
      while(it != participators.end()) {
        cout << "[create meeting] this participator has been going to be added!\n";
        cout << "[create meeting] [please enter the participator " << i << " ]\n"
             << "[create meeting] ";
        cin >> participator;
        it = find(participators.begin(), participators.end(), participator);
      }
    }
    participators.push_back(participator);
  }
  string title, startDate, endDate;
  cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]\n"
       << "[create meeting] ";
  cin >> title >> startDate >> endDate;
  while(!Date::isValid(Date::stringToDate(startDate)) ||
         !Date::isValid(Date::stringToDate(endDate)) ||
                   Date::stringToDate(startDate) >= Date::stringToDate(endDate)) {
    if(!Date::isValid(Date::stringToDate(startDate)))
      cout << "[create meeting] The start time is not valid!\n";
    if(!Date::isValid(Date::stringToDate(endDate)))
      cout << "[create meeting] The end time is not valid!\n";
    if(Date::isValid(Date::stringToDate(startDate)) &&
            Date::isValid(Date::stringToDate(endDate)) &&
                 Date::stringToDate(startDate) >= Date::stringToDate(endDate)) {
          cout << "[create meeting] The start time is later than the end time!\n";
    }
    cout << "[create meeting] input again!\n";
    cout << "[create meeting] [start time(yyyy-mm-dd/hh:mm] [end time(yyyy-mm-dd/hh:mm)]\n";
    cout << "[create meeting] ";
    cin >> startDate >> endDate;
  }
  try {
    m_agendaService.createMeeting(m_userName, title, startDate, endDate, participators);
  }
  catch(int a) {
    if(a == 1 || a == 2);
    if(a == 3) {
      cout << "[create meeting] The title has been exist\n";
    }
    if(a == 4) {
      cout << "[create meeting] Your have no time\n";
    }
    OperationLoop();
    return;
  }
  cout << "[create meeting] succeed!\n";
  OperationLoop();
}

void AgendaUI::addMeetingParticipator(void) {
  string title, participator;
  cout << "[add participator] [meeting title] [participator username]\n"
       << "[add participator] ";
  cin >> title >> participator;
  try {
    m_agendaService.addMeetingParticipator(m_userName, title, participator);
  }
  catch(int a) {
    if(a == 1) 
      cout << "[add participator] would you want to add yourself?\n";
    if(a == 2) 
      cout << "[add participator] This man has been in your meeting!\n";
    if(a == 3) 
      cout << "[add participator] He has no time!\n";
    OperationLoop();
    return;
  }
  catch(double) {
    cout << "[add participator] Can't find this title!\n";
    OperationLoop();
    return;
  }
  catch(float) {
    cout << "[add participator] Can't find this man!\n";
    OperationLoop();
    return;
  }
  catch(char) {
    cout << "[add participator] You aren't the sponsor!\n";
    OperationLoop();
    return;
  }
  cout << "[add participator] succeed!\n";
  OperationLoop();
}

void AgendaUI::removeMeetingParticipator(void) {
  string title, participator;
  cout << "[remove participator] [meeting title] [participator username]\n"
       << "[remove participator] ";
  cin >> title >> participator;
  try {
    m_agendaService.removeMeetingParticipator(m_userName, title, participator);
  }
  catch(int a) {
    if(a == 1)
      cout << "[remove participator] You aren't the sponsor!\n";
    if(a == 2)
      cout << "[remove participator] Can't find this title!\n";
    OperationLoop();
    return;
  }
  catch(double) {
    cout << "[remove participator] Can't find this participator!\n";
    OperationLoop();
    return;
  }
  catch(float) {
    cout << "[remove participator] This participator isn't in the meeting!\n";
    OperationLoop();
    return;
  }
  cout << "[remove participator] succeed!\n";
  OperationLoop();
}

void AgendaUI::quitMeeting(void) {
  string title;
  cout << "[quit meeting] [meeting title]\n"
       << "[quit meeting] ";
  cin >> title;
  try {
    m_agendaService.quitMeeting(m_userName, title);
  }
  catch(int) {
    cout << "[quit meeting] You aren't in this meeting!\n";
    OperationLoop();
    return;
  }
  catch(double) {
    cout << "[quit meeting] Can't find this title!\n";
    OperationLoop();
    return;
  }
  catch(float) {
    cout << "[quit meeting] You are the Sponsor!\n";
    OperationLoop();
    return;
  }
  cout << "[quit meeting] succeed!\n";
  OperationLoop();
}

void AgendaUI::listAllMeetings(void) {
  cout << "[list all meetings]\n"
       << "\n";
  printMeetings(m_agendaService.listAllMeetings(m_userName));
  OperationLoop();
}

void AgendaUI::listAllSponsorMeetings(void) {
  cout << "[list all sponsor meetings]\n"
       << "\n";
  printMeetings(m_agendaService.listAllSponsorMeetings(m_userName));
  OperationLoop();
}

void AgendaUI::listAllParticipateMeetings(void) {
  cout << "[list all participator meetings]\n"
       << "\n";
  printMeetings(m_agendaService.listAllParticipateMeetings(m_userName));
  OperationLoop();
}

void AgendaUI::queryMeetingByTitle(void) {
  cout << "[query meeting] [title]:\n"
       << "[query meeting] ";
  string title;
  cin >> title;
  if(m_agendaService.meetingQuery(m_userName, title).size() == 0) {
    cout << "[query meeting] Can't find this title\n";
    OperationLoop();
    return; 
  }
  printMeetings(m_agendaService.meetingQuery(m_userName, title));
  OperationLoop();
}

void AgendaUI::queryMeetingByTimeInterval(void) {
  cout << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]\n"
       << "[query meetings] ";
  string startDate, endDate;
  cin >> startDate >> endDate;
  while(!Date::isValid(Date::stringToDate(startDate)) || 
         !Date::isValid(Date::stringToDate(endDate)) ||
		   Date::stringToDate(startDate) > Date::stringToDate(endDate)) {
    if(!Date::isValid(Date::stringToDate(startDate))) 
      cout << "[query meetings] The start time is not valid!\n";
    if(!Date::isValid(Date::stringToDate(endDate)))
      cout << "[query meetings] The end time is not valid!\n";
    if(Date::isValid(Date::stringToDate(startDate)) && 
	    Date::isValid(Date::stringToDate(endDate)) && 
		 Date::stringToDate(startDate) > Date::stringToDate(endDate)) {
	  cout << "[query meetings] The start time is later than the end time!\n";	 	
    }
    cout << "[query meetings] input again!\n";
    cout << "[query meetings] [start time(yyyy-mm-dd/hh:mm] [end time(yyyy-mm-dd/hh:mm)]\n";
    cout << "[query meetings] ";
    cin >> startDate >> endDate; 
  }
  cout << "[query meetings]\n";
  printMeetings(m_agendaService.meetingQuery(m_userName, startDate, endDate));
  OperationLoop();
}

void AgendaUI::deleteMeetingByTitle(void) {
  cout << "[delete meeting] [title]\n"
       << "[delete meeting] ";
  string title;
  cin >> title;
  if(m_agendaService.deleteMeeting(m_userName, title)) {
  	cout << "[delete meeting] succeed!\n";
  }
  else {
  	cout << "[error] delete meeting fail!\n";
  }
  OperationLoop();
}


void AgendaUI::deleteAllMeetings(void) {
  m_agendaService.deleteAllMeetings(m_userName);
  cout << "[delete all meeting] succeed!\n";
  OperationLoop();
}

void AgendaUI::printMeetings(const std::list<Meeting> &t_meetings) {
  cout << setiosflags(ios::left);
  cout << setw(20) << "title" << setw(20) << "sponsor" << setw(20) << "start time" << setw(20) << "end time" << setw(20) << "participators" << endl;
  cout << setiosflags(ios::left);
  for(auto it = t_meetings.begin(); it != t_meetings.end(); it++) {
  	cout << setiosflags(ios::left) << setw(20) << it -> getTitle()  << setw(20) << it -> getSponsor() << setw(20) 
  		  << Date::dateToString(it -> getStartDate()) << setw(20) << Date::dateToString(it -> getEndDate()) ;
  	for(int i = 0; i < it -> getParticipator().size(); i++) {
  	  if(i == 0) {
  	    cout << it -> getParticipator()[i];
  	  }
  	  else
  	    cout << "," << it -> getParticipator()[i];
  	}
  	cout << endl;
  }
}
