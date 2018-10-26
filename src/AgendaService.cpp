#include "AgendaService.hpp"
#include <iostream>
using namespace std;
AgendaService::AgendaService() {
  startAgenda();
}

AgendaService::~AgendaService() {
  quitAgenda();
}


bool AgendaService::userLogIn(const std::string &userName, const std::string &password) {
  auto filter1 = [userName](const User& user) {
    return userName == user.getName();
  };
  if(m_storage -> queryUser(filter1).size() == 0){
    throw 1;
  }
  auto filter2 = [userName, password](const User& user) {
  	return userName == user.getName() && password != user.getPassword();
  };
  if(m_storage -> queryUser(filter2).size() > 0) {
  	throw 2.0;
  }
  auto filter = [userName, password](const User& user) {
    return userName == user.getName() && password == user.getPassword();
  };
  return m_storage -> queryUser(filter).size() > 0;
}

bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                    const std::string &email, const std::string &phone) {
  auto filter = [userName](const User& user) {
    return userName == user.getName();
  };
  if(m_storage -> queryUser(filter).size() == 0) {
    m_storage -> createUser(User(userName, password, email, phone));
    return true;
  }
  return false;
}

bool AgendaService::deleteUser(const std::string &userName, const std::string &password) {
  auto filter = [userName, password](const User& user) {
    return userName == user.getName() && password == user.getPassword();
  };  
  auto filter2 = [userName](const Meeting& meeting) {
    return meeting.isParticipator(userName);
  };
  auto switcher = [userName](Meeting& meeting) {
    meeting.removeParticipator(userName);
  };
  if(m_storage -> deleteUser(filter) > 0) {
    deleteAllMeetings(userName);
    if(m_storage -> updateMeeting(filter2, switcher) > 0) {
      auto filter3 = [](const Meeting& meeting) {
        return meeting.getParticipator().size() == 0;
      };
      m_storage -> deleteMeeting(filter3);
    }
    return true;
  }
  return false;
}

std::list<User> AgendaService::listAllUsers(void) const {
  auto filter = [](const User& user) {return true;};
  return m_storage -> queryUser(filter);
}

bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                     const std::string &startDate, const std::string &endDate,
                     const std::vector<std::string> &participator) {
  if(participator.size() == 0) return false;
  for(int i = 0; i < participator.size(); i++) {
  	if(participator[i] == userName)
  		return false;
    for(int j = 0; j < participator.size(); j++) {
      if(participator[i] == participator[j] && i != j) 
        return false;
    }
  }
  if(!Date::isValid(Date::stringToDate(startDate)) 
  	     || !Date::isValid(Date::stringToDate(endDate))
  	       || Date::stringToDate(startDate) >= Date::stringToDate(endDate))
  	return false;
  auto filter1 = [userName](const User& user) {
    return userName == user.getName();
  };
  auto filter3 = [title](const Meeting& meeting) {
    return title == meeting.getTitle();
  };
  if(m_storage -> queryMeeting(filter3).size() > 0) {
    throw 3;
  }
  auto filter5 = [userName, startDate, endDate](const Meeting& meeting) {
    return (meeting.isParticipator(userName) || meeting.getSponsor() == userName) &&
            ((meeting.getStartDate() <= Date::stringToDate(startDate) && meeting.getEndDate() > Date::stringToDate(startDate)) ||
             (meeting.getStartDate() < Date::stringToDate(endDate) && meeting.getEndDate() >= Date::stringToDate(endDate)) ||
              (meeting.getStartDate() >= Date::stringToDate(startDate) && meeting.getEndDate() <= Date::stringToDate(endDate)));
  };

  if(m_storage -> queryMeeting(filter5).size() > 0) {
    throw 4;
  }
  int flag = 0;
  for(int i = 0; i < participator.size(); i++) {
    auto participator_ = participator[i];
    auto filter2 = [participator_](const User& user) {
      return participator_ == user.getName();
    };
    auto filter4 = [participator_, startDate, endDate](const Meeting& meeting) {
      return (meeting.isParticipator(participator_) || meeting.getSponsor() == participator_) && 
            ((meeting.getStartDate() <= Date::stringToDate(startDate) && meeting.getEndDate() > Date::stringToDate(startDate)) || 
             (meeting.getStartDate() < Date::stringToDate(endDate) && meeting.getEndDate() >= Date::stringToDate(endDate)) || 
              (meeting.getStartDate() >= Date::stringToDate(startDate) && meeting.getEndDate() <= Date::stringToDate(endDate)));
    };
    if(m_storage -> queryUser(filter2).size() <= 0) {
      cout << "[create meeting] There is no participator " << i + 1 << endl;
      flag = 1;
    }
    if(m_storage -> queryMeeting(filter4).size() > 0) {
      cout << "[create meeting] participtor " << i + 1 << " has no time!" << endl;
      flag = 2;
    }
  }
  if(flag == 1) throw 1;
  if(flag == 2) throw 2;
  if(m_storage -> queryUser(filter1).size() <= 0 ||
  	   m_storage -> queryMeeting(filter3).size() > 0 ||
  	     m_storage -> queryMeeting(filter5).size() > 0) {
    return false;
  }
  m_storage -> createMeeting(Meeting(userName, participator, 
  	     Date::stringToDate(startDate), Date::stringToDate(endDate), title));
  return true;
}

bool AgendaService::addMeetingParticipator(const std::string &userName,
                              const std::string &title,
                              const std::string &participator) {
  if(userName == participator) throw 1;
  auto filter = [participator](const User& user) {
    return participator == user.getName();
  };
  auto f1 = [title](const Meeting& meeting) {
    return title == meeting.getTitle();
  };
  auto f2 = [userName, title] (const Meeting& meeting) {
    return title == meeting.getTitle() && userName != meeting.getSponsor();
  };
  auto f3 = [userName, title, participator] (const Meeting& meeting) {
    return title == meeting.getTitle() && userName == meeting.getSponsor() && !(meeting.isParticipator(participator));
  };
  if(m_storage -> queryMeeting(f1).size() == 0)
    throw 2.0;
  if(m_storage -> queryUser(filter).size() == 0)
    throw 3.0f;
  if(m_storage -> queryMeeting(f2).size() > 0)
    throw 'a';
  if(m_storage -> queryMeeting(f3).size() > 0)
    throw 2;
  if(m_storage -> queryUser(filter).size() > 0 && meetingQuery(userName, title).size() > 0) {
    auto m = meetingQuery(userName, title).begin();
    auto startDate = m -> getStartDate();
    auto endDate = m -> getEndDate();
    auto filter2 = [participator, startDate, endDate](const Meeting& meeting) {
      return (meeting.isParticipator(participator) || meeting.getSponsor() == participator) && 
            ((meeting.getStartDate() <= startDate && meeting.getEndDate() > startDate) || 
             (meeting.getStartDate() < endDate && meeting.getEndDate() >= endDate) || 
              (meeting.getStartDate() >= startDate && meeting.getEndDate() <= endDate));
    };
    if(m_storage -> queryMeeting(filter2).size() > 0) {
      throw 3;
    }
    if(m_storage -> queryMeeting(filter2).size() == 0) {
      auto switcher = [participator](Meeting& meeting) {
        meeting.addParticipator(participator);
      };
      auto filter3 = [userName, title](const Meeting& meeting) {
        return title == meeting.getTitle() && userName == meeting.getSponsor();
      };
      m_storage -> updateMeeting(filter3, switcher);
      return true;
    }
  }
  return false;
}


bool AgendaService::removeMeetingParticipator(const std::string &userName,
                                 const std::string &title,
                                 const std::string &participator) {
  auto filter1 = [userName, title](const Meeting& meeting) {
    return userName != meeting.getSponsor() && title == meeting.getTitle();
  };
  auto filter2 = [participator](const User& user) {
    return participator == user.getName();
  };
  auto filter3 = [userName, title, participator](const Meeting& meeting) {
    return title == meeting.getTitle() && userName == meeting.getSponsor() && !(meeting.isParticipator(participator));
  };
  auto filter4 = [title](const Meeting& meeting) {
    return title == meeting.getTitle();
  };
  if(participator == userName) {
    throw 'a';
  }
  if(m_storage -> queryMeeting(filter4).size() == 0) {
    throw 2;
  }
  if(m_storage -> queryUser(filter2).size() == 0) {
    throw 2.0;
  }
  if(m_storage -> queryMeeting(filter1).size() > 0) {
    throw 1;
  }
  if(m_storage -> queryMeeting(filter3).size() > 0) {
    throw 3.0f;
  }
  auto filter = [userName, title, participator](const Meeting& meeting) {
    return title == meeting.getTitle() && userName == meeting.getSponsor() && meeting.isParticipator(participator);
  };
  auto switcher = [participator](Meeting& meeting) {
    meeting.removeParticipator(participator);
  };
  if(m_storage -> updateMeeting(filter, switcher) > 0) {
    auto filter2 = [](const Meeting& meeting) {
      return meeting.getParticipator().size() == 0;
    };
    m_storage -> deleteMeeting(filter2);
    return true;
  }
  return false;  
}

bool AgendaService::quitMeeting(const std::string &userName, const std::string &title) {
  auto filter1 = [userName, title](const Meeting& meeting) {
    return title == meeting.getTitle() && !(meeting.isParticipator(userName));
  };
  auto filter2 = [title](const Meeting& meeting) {
    return title == meeting.getTitle();
  };
  auto filter3 = [userName, title](const Meeting& meeting) {
    return title == meeting.getTitle() && meeting.getSponsor() == userName;
  };
  if(m_storage -> queryMeeting(filter3).size() > 0) {
    throw 3.0f;
  }
  if(m_storage -> queryMeeting(filter1).size() > 0){
    throw 1;
  }
  if(m_storage -> queryMeeting(filter2).size() == 0) {
    throw 2.0;
  }
  auto filter = [userName, title](const Meeting& meeting) {
    return title == meeting.getTitle() && meeting.isParticipator(userName);
  };
  auto switcher = [userName](Meeting& meeting) {
      meeting.removeParticipator(userName);
  };
  if(m_storage -> updateMeeting(filter, switcher) > 0) {
    auto filter2 = [](const Meeting& meeting) {
      return meeting.getParticipator().size() == 0;
    };
    m_storage -> deleteMeeting(filter2);
    return true;
  }
  return false;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &title) const {
  auto filter = [userName, title](const Meeting& meeting) {
    return title == meeting.getTitle() && 
                 (userName == meeting.getSponsor() || meeting.isParticipator(userName));
  };
  return m_storage -> queryMeeting(filter);
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &startDate,
                                  const std::string &endDate) const {
  if(!Date::isValid(Date::stringToDate(startDate)) 
  	     || !Date::isValid(Date::stringToDate(endDate))
  	       || Date::stringToDate(startDate) > Date::stringToDate(endDate)) {
  	return list<Meeting>();
  }
  auto filter = [userName, startDate, endDate](const Meeting& meeting) {
    return (userName == meeting.getSponsor() || meeting.isParticipator(userName)) && 
            ((meeting.getStartDate() >= Date::stringToDate(startDate) && meeting.getStartDate() <= Date::stringToDate(endDate)) || 
             (meeting.getEndDate() >= Date::stringToDate(startDate) && meeting.getEndDate() <= Date::stringToDate(endDate)) || 
              (meeting.getStartDate() <= Date::stringToDate(startDate) && meeting.getEndDate() >= Date::stringToDate(endDate)));
  };
  return m_storage -> queryMeeting(filter);
}

std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const {
  auto filter = [userName](const Meeting& meeting) {
    return userName == meeting.getSponsor() || meeting.isParticipator(userName);
  };
  return m_storage -> queryMeeting(filter);
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const {
  auto filter = [userName](const Meeting& meeting) {
    return userName == meeting.getSponsor();
  };
  return m_storage -> queryMeeting(filter);
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(const std::string &userName) const {
  auto filter = [userName](const Meeting& meeting) {
    return meeting.isParticipator(userName);
  };
  return m_storage -> queryMeeting(filter);
}

bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title) {
  auto filter = [userName, title](const Meeting& meeting) {
    return userName == meeting.getSponsor() && title == meeting.getTitle();
  };
  return m_storage -> deleteMeeting(filter) > 0;
}

bool AgendaService::deleteAllMeetings(const std::string &userName) {
  auto filter = [userName](const Meeting& meeting) {
    return userName == meeting.getSponsor();
  };
  return m_storage -> deleteMeeting(filter) > 0;
}


void AgendaService::startAgenda(void) {
  m_storage = Storage::getInstance();
}

void AgendaService::quitAgenda(void) {
  m_storage -> sync();
}
