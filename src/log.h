#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

// Clase para redirigir std::clog a un archivo.
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

enum typelog {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

struct structlog {
    bool    headers = false;
    typelog level   = WARN;
};

extern structlog LOGCFG;

class LOG {
    public:
        LOG() : LOG(DEBUG) {}
        LOG(typelog type)
        {
            msglevel = type;
            if( LOGCFG.headers ) {
                auto t  = std::time(nullptr);
                auto tm = *std::localtime(&t);

                std::ostringstream oss;
                oss << std::put_time(&tm, "[%d-%m-%Y %H:%M:%S]");
                auto time_str = oss.str();

            operator << ( time_str + " [" + this->getLabel(type) + "] ");
            }
        }

        ~LOG()
        {
            if( opened ) {
                std::clog << std::endl << std::flush;
            }

            opened = false;
        }

        template<class T>
        LOG &operator<<(const T &msg)
        {
            if( msglevel >= LOGCFG.level ) {
                std::clog << msg;
                opened = true;
            }
            return *this;
        }

    private:
        bool    opened      = false;
        typelog msglevel    = DEBUG;

        inline std::string getLabel(typelog type) {
            std::string label;
            switch( type ) {
                case DEBUG: label = "DEBUG"; break;
                case INFO:  label = "INFO "; break;
                case WARN:  label = "WARN "; break;
                case ERROR: label = "ERROR"; break;
            }
            return label;
        }
};

#endif // LOG_H_INCLUDED
