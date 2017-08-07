#include "log.h"

Log_Handler::Log_Handler(const std::string &file_name)
{
    redirect_clog(file_name);
}

void Log_Handler::redirect_clog(const std::string &file_name)
{
    // Redirigiremos clog a un fichero para escribir cosas interesantes en él.
    log_file.open(file_name, std::fstream::out);

    backup_clog = std::clog.rdbuf();	// Backup del streambuf de clog

    log_buf = log_file.rdbuf(); // Obtenemos el streambuf del fichero
    std::clog.rdbuf(log_buf);	// Redirigimos clog
}

void Log_Handler::restore_clog()
{
    std::clog.rdbuf(backup_clog);    // Restauramos el streambuf de clog
    log_file.close();			// Cerramos el fichero de log.
}

void Log_Handler::message(const std::string &str)
{
    auto t  = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "[%d-%m-%Y %H:%M:%S] ");
    auto time_str = oss.str();
    time_str += str;

    std::clog << time_str << std::endl;
}

Log_Handler log_handler;
