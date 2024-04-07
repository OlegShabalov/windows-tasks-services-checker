#ifndef CHECKTASKS_HPP
#define CHECKTASKS_HPP

#include <string>



enum TaskState {
    ERROR_TS,
    DISABLED,
    NOT_DISABLED,
};



TaskState checkTask(const std::wstring & path, const std::wstring & name);



#endif // CHECKTASKS_HPP
