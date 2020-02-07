#ifndef HPINTERFACE_H
#define HPINTERFACE_H

extern "C" {

//work around C call in C++
#define char16_t _C_CHAR16_

#include <hidapi.h>
#include <hpcalcs.h>
#include <hpfiles.h>
#include <hpcables.h>
#include <hpopers.h>
#include <prime_cmd.h>
#include "internal.h"
#include "logging.h"
#include "error.h"
#include "gettext.h"



#undef char16_t

}

#define STRBUF_SIZE 1024
#define DATA_SIZE 512
#define SEPARATORS ", \t\n"

struct hp_Handle {
    cable_handle * cable = nullptr;
    calc_handle * calc = nullptr;
};

struct hp_Information;

class hp {

private:
    cable_model model1 = CABLE_NUL;
    calc_model model2 = CALC_NONE;
    cable_handle * cable;
    calc_handle * calc;

public:

    hp();
    int hp_init();
    int hp_close();
    int hp_func();
    int is_ready();
    hp_Information load_info();
    int get_info(calc_infos * infos);
    hp_Handle getHandle();
    int vpkt_send_experiments(int id);

    ~hp();
};

#endif // HPINTERFACE_H
