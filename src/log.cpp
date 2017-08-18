#include "log.h"

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
    log_file.close();			     // Cerramos el fichero de log.
}

// Creamos un objeto global, declarado como extern en el fichero cabecera.
// Bastará con hacer include "log.h" para tener acceso a las funciones de log
// desde cualquier punto del programa.
Log_Handler log_handler;

structlog LOGCFG = {};
LOGCFG.headers = true;
LOGCFG.level = DEBUG;
