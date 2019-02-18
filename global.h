#ifndef GLOBAL_H
#define GLOBAL_H
#include "version.h"
#include "errorhandler.h"


class errorHandler;
extern errorHandler *main_err;
#define log(a) main_err->error(L7,0,QString(a),QString());
#define err(a,b,c) main_err->error(a,b,QString(c),QString());


#endif // GLOBAL_H

