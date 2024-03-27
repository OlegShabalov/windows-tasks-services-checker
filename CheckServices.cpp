#include "CheckServices.hpp"

#include <windows.h>



std::vector<ServicesState> checkServices(const std::vector<const char *> & servNames) {
    if (servNames.empty()) {
        return std::vector<ServicesState>();
    }

    std::vector<ServicesState> result(servNames.size());
    for (int i=0; i<servNames.size(); ++i) {
        result[i].currentState = ServicesState::CurrentState::ERROR_CS;
        result[i].startType    = ServicesState::StartType::ERROR_ST;
    }

    SC_HANDLE schSCManager;
    SC_HANDLE schService;
    SERVICE_STATUS sStatus;
    DWORD dwBytesNeeded, cbBufSize;
    LPQUERY_SERVICE_CONFIG lpsc = nullptr;

    //schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);

    if (schSCManager == 0)  {
        return result;
    }

    for (int i=0; i<servNames.size(); ++i) {
        //schService = OpenServiceA(schSCManager, servNames[i], SERVICE_ALL_ACCESS);
        schService = OpenServiceA(schSCManager, servNames[i], SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS);

        if (schService == 0) {
            continue;
        }

        if (QueryServiceStatus(schService, &sStatus)) {
            if (sStatus.dwCurrentState == SERVICE_STOPPED) {
                result[i].currentState = ServicesState::CurrentState::STOPPED;
            }
            if (sStatus.dwCurrentState == SERVICE_START_PENDING) {
                result[i].currentState = ServicesState::CurrentState::START_PENDING;
            }
            if (sStatus.dwCurrentState == SERVICE_STOP_PENDING) {
                result[i].currentState = ServicesState::CurrentState::STOP_PENDING;
            }
            if (sStatus.dwCurrentState == SERVICE_RUNNING) {
                result[i].currentState = ServicesState::CurrentState::RUNNING;
            }
            if (sStatus.dwCurrentState == SERVICE_CONTINUE_PENDING) {
                result[i].currentState = ServicesState::CurrentState::CONTINUE_PENDING;
            }
            if (sStatus.dwCurrentState == SERVICE_PAUSE_PENDING) {
                result[i].currentState = ServicesState::CurrentState::PAUSE_PENDING;
            }
            if (sStatus.dwCurrentState == SERVICE_PAUSED) {
                result[i].currentState = ServicesState::CurrentState::PAUSED;
            }
        }

        if (!QueryServiceConfig(schService, NULL, 0, &dwBytesNeeded) &&
            GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
            cbBufSize = dwBytesNeeded;
            lpsc = (LPQUERY_SERVICE_CONFIG) LocalAlloc(LMEM_FIXED, cbBufSize);
        }
        else {
            CloseServiceHandle(schService);
            continue;
        }

        if (QueryServiceConfig(schService, lpsc, cbBufSize, &dwBytesNeeded)) {
            QUERY_SERVICE_CONFIG & config = *lpsc;

            if (config.dwStartType == SERVICE_DISABLED) {
                result[i].startType = ServicesState::StartType::DISABLED;
            }
            if (config.dwStartType == SERVICE_DEMAND_START) {
                result[i].startType = ServicesState::StartType::DEMAND_START;
            }
            if (config.dwStartType == SERVICE_AUTO_START) {
                result[i].startType = ServicesState::StartType::AUTO_START;
            }
            if (config.dwStartType == SERVICE_SYSTEM_START) {
                result[i].startType = ServicesState::StartType::SYSTEM_START;
            }
            if (config.dwStartType == SERVICE_BOOT_START) {
                result[i].startType = ServicesState::StartType::BOOT_START;
            }
        }

        LocalFree(lpsc);
        CloseServiceHandle(schService);
    }

    CloseServiceHandle(schSCManager);

    return result;
}
