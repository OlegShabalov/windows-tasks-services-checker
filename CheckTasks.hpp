#ifndef CHECKTASKS_HPP
#define CHECKTASKS_HPP



enum TaskState {
    ERROR_TS,
    DISABLED,
    NOT_DISABLED,
};



TaskState checkTask(const wchar_t * path, const wchar_t * name);



#endif // CHECKTASKS_HPP
