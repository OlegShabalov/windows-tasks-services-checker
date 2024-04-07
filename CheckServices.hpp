#ifndef CHECKSERVICES_HPP
#define CHECKSERVICES_HPP

#include <vector>
#include <string>



struct ServicesState {
    enum CurrentState {
        ERROR_CS,
        STOPPED,
        RUNNING,
        PAUSED,
        START_PENDING,
        STOP_PENDING,
        CONTINUE_PENDING,
        PAUSE_PENDING,
    };

    enum StartType {
        ERROR_ST,
        DISABLED,
        DEMAND_START,
        AUTO_START,
        SYSTEM_START,
        BOOT_START,
    };

    CurrentState currentState;
    StartType startType;
};



std::vector<ServicesState> checkServices(const std::vector<std::string> & servNames);



#endif // CHECKSERVICES_HPP
