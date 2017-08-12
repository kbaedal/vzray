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

        void enable(bool b = true)
        {
            to_file = b;
        }

		template<class T>
        Log_Handler &operator<<(const T &dato)
        {
            if( to_file ) {
                auto t  = std::time(nullptr);
                auto tm = *std::localtime(&t);

                std::ostringstream oss;
                oss << std::put_time(&tm, "[%d-%m-%Y %H:%M:%S] ");
                auto time_str = oss.str();

                std::clog << time_str << dato << std::endl;
            }

            return *this;
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
