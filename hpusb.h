#ifndef HPUSB_H
#define HPUSB_H

#include <libusb.h>

struct hp_Settings;

//! USB Vendor ID of Hewlett-Packard.
#define USB_VID_HP (0x03F0)
//! USB Product ID of the Prime calculator in firmware revisions < 8151.
#define USB_PID_PRIME1 (0x0441)
//! USB Product ID of the Prime calculator in firmware revisions >= 8151.
#define USB_PID_PRIME2 (0x1541)
//! USB Product ID of the Prime calculator in firmware revisions >= 8151.
#define USB_PID_PRIME3 (0x2441)
//! Size of a raw HID packet for the Prime.
#define PRIME_RAW_HID_DATA_SIZE (64)

#define LEN_IN_BUFFER 1024
//*8
#define USB_ENDPOINT_IN	    (LIBUSB_ENDPOINT_IN  | 1)   /* endpoint address */
#define USB_ENDPOINT_OUT	(LIBUSB_ENDPOINT_OUT | 2)   /* endpoint address */

#define CMD_PRIME_CHECK_READY (0xFF)
#define CMD_PRIME_GET_SETTINGS (0xF9)
#define CMD_PRIME_GET_INFOS (0xFA)
#define CMD_PRIME_RECV_SCREEN (0xFC)
#define CMD_PRIME_RECV_BACKUP (0xF9)
#define CMD_PRIME_REQ_FILE (0xF8)
#define CMD_PRIME_RECV_FILE (0xF7)
#define CMD_PRIME_SEND_CHAT (0xF2)
#define CMD_PRIME_RECV_CHAT (0xF2)
#define CMD_PRIME_SEND_KEY (0xEC)
#define CMD_PRIME_SET_DATE_TIME (0xE7)

struct hp_Handle {
      libusb_device_handle *usbhandle = nullptr;
      libusb_device *usbdevice = nullptr;
      int dev_open=0;
};

struct hp_cmd {

};

struct hp_pkt_in {
    uint8_t * buffer;
    int size;
};

struct hp_pkt_out {
    uint8_t * buffer;
    int size;
};

struct hp_Information;

     void cb_out(struct libusb_transfer *transfer);
     void cb_in(struct libusb_transfer *transfer);
     void sighandler(int signum);

class hpusb
{

    private:


        struct timespec t1, t2;

         enum {
            out_deinit,
            out_release,
            out
         } exitflag;

         libusb_context * ctx;

  //      cable_model model1 = CABLE_NUL;
  //      calc_model model2 = CALC_NONE;
  //      cable_handle * cable;
  //      static libusb_device_handle *usbhandle;
  //      static libusb_device *usbdevice;

        static hp_Handle hp_handle;
        int lb_init=0;

        // OUT-going transfers (OUT from host PC to USB-device)
        struct libusb_transfer *transfer_out = NULL;

        // IN-coming transfers (IN to host PC from USB-device)
        struct libusb_transfer *transfer_in = NULL;
        int do_exit = 0;

    public:

        hpusb();
        int hp_init();
        int is_device(libusb_device *);
        int hp_open(hp_Handle *);
        int submit_async_transfer(hp_Handle *, hp_pkt_in *, hp_pkt_out *);
        int submit_sync_transfer(hp_Handle *, hp_pkt_in *, hp_pkt_out *);
        int submit_callback();
        int hp_close(hp_Handle * );
        int hp_func();
        int is_ready();
        int load_info(hp_Handle *, hp_Information *);
        int get_info( /*calc_infos * infos*/);
        int get_settings(hp_Handle * , hp_Settings * );
        int set_settings(hp_Handle * , hp_Settings set);

        int vpkt_send_experiments(hp_Handle * handle, int cmd);
        // Function Prototypes:

        void print_libusb_transfer(struct libusb_transfer *p_t);

        friend  void sighandler(int signum);
        friend    void cb_out(struct libusb_transfer *transfer);
        friend    void cb_in(struct libusb_transfer *transfer);

        ~hpusb();
};

#endif // HPUSB_H
