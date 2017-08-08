#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

class Log_Handler {
    public:
        Log_Handler() : Log_Handler("log.txt") {};
        Log_Handler(const std::string &file_name)
        {
            redirect_clog(file_name);
            to_file = true;
        };

        ~Log_Handler() {
            restore_clog();
        };

        void message(const std::string &msg);
        void enable(bool b = true)
        {
            to_file = b;
        }

    private:
        void redirect_clog(const std::string &file_name);
        void restore_clog();

        std::fstream    log_file;
        std::streambuf  *backup_clog, *log_buf;

        bool to_file;
};

extern Log_Handler log_handler;

#endif // LOG_H_INCLUDED
