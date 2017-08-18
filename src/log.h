#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

class Log_Handler {
    public:
        Log_Handler() : Log_Handler("log.txt") {}
        Log_Handler(const std::string &file_name)
        {
            redirect_clog(file_name);

            to_file     = true;
            print_level = true;
            print_dt    = true;

            level = "Info";
        }

        ~Log_Handler() {
            restore_clog();
        };

        void enable(bool b = true)
        {
            to_file = b;
        }

        void set_level(int l)
        {
            switch( l ) {
                case 0 : level = "Info";
                    break;
                case 1 : level = "Warning";
                    break;
                case 2 : level = "Fatal";
                    break;
                default: level = "Info";
                    break;
            }
        }

		template<class T>
        Log_Handler &operator<<(const T &dato)
        {
            if( to_file ) {
                if( print_dt ) {
                    auto t  = std::time(nullptr);
                    auto tm = *std::localtime(&t);

                    std::ostringstream oss;
                    oss << std::put_time(&tm, "[%d-%m-%Y %H:%M:%S]");
                    auto time_str = oss.str();

                    std::clog << time_str << " ";
                }

                if( print_level )
                    std::clog << "[" << level << "] ";

                std::clog << dato << std::endl << std::flush;
            }

            return *this;
        }

    private:
        void redirect_clog(const std::string &file_name);
        void restore_clog();

        std::fstream    log_file;
        std::streambuf  *backup_clog, *log_buf;

        bool    to_file,
                print_level,
                print_dt;

        std::string level;
};

extern Log_Handler log_handler;

/*
 * File:   Log.h
 * Author: Alberto Lepe <dev@alepe.com>
 *
 * Created on December 1, 2015, 6:00 PM
 */

class CLog_Redir {
    public:
        CLog_Redir() : CLog_Redir("log.txt") {}
        CLog_Redir(const std::string &file_name)
        {
            // Redirigiremos clog a un fichero para escribir cosas interesantes en él.
            log_file.open(file_name, std::fstream::out);

            backup_clog = std::clog.rdbuf();	// Backup del streambuf de clog

            log_buf = log_file.rdbuf(); // Obtenemos el streambuf del fichero
            std::clog.rdbuf(log_buf);	// Redirigimos clog
        }

        ~CLog_Redir()
        {
            std::clog.rdbuf(backup_clog);    // Restauramos el streambuf de clog
            log_file.close();			     // Cerramos el fichero de log.
        }
    private:
        std::fstream    log_file;
        std::streambuf  *backup_clog, *log_buf;
};

#ifndef LOG_H
#define LOG_H

#include <iostream>

enum typelog {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

struct structlog {
    bool headers = false;
    typelog level = WARN;
};

extern structlog LOGCFG;

class LOG {
public:
    LOG() : LOG(DEBUG) {}
    LOG(typelog type) {
        msglevel = type;
        if(LOGCFG.headers) {
            //operator << ("["+getLabel(type)+"]");
            auto t  = std::time(nullptr);
            auto tm = *std::localtime(&t);

            std::ostringstream oss;
            oss << std::put_time(&tm, "[%d-%m-%Y %H:%M:%S]");
            auto time_str = oss.str();

            operator << ( time_str + " " );
        }
    }

    ~LOG() {
        if(opened) {
            std::clog << std::endl << std::flush;
        }
        opened = false;
    }

    template<class T>
    LOG &operator<<(const T &msg) {
        if(msglevel >= LOGCFG.level) {
            std::clog << msg;
            opened = true;
        }
        return *this;
    }
private:
    bool opened = false;
    typelog msglevel = DEBUG;
    inline std::string getLabel(typelog type) {
        std::string label;
        switch(type) {
            case DEBUG: label = "DEBUG"; break;
            case INFO:  label = "INFO "; break;
            case WARN:  label = "WARN "; break;
            case ERROR: label = "ERROR"; break;
        }
        return label;
    }
};

#endif  /* LOG_H */

#endif // LOG_H_INCLUDED
