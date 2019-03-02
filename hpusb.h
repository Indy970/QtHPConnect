#ifndef HPUSB_H
#define HPUSB_H

#include <libusb.h>
#include <QByteArray>

struct hp_Settings;
class hpCalcData;


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
#define PRIME_RAW_DATA_SIZE (1024)


#define LEN_IN_BUFFER 1024*8
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

#define ENDPOINT_OUT (0x02)
#define ENDPOINT_IN  (0x81)
#define HEADER_LEN (0xFD)
#define TIME_OUT (5000)

struct hp_Handle {
      libusb_device_handle *usbhandle = nullptr;
      libusb_device *usbdevice = nullptr;
      hpCalcData * calc;
      int dev_open=0;
};

struct usb_firstchunk {
    uint8_t start1;
    uint8_t start2;
    uint8_t type;
    int items;
    int size;
};

struct usb_chunk {
    uint8_t start1;
    uint8_t chunk;  //the chunk number
};

enum usb_header_type {
    HP_HDR_FILE,
    HP_HDR_CHUNK,
    HP_HDR_INFO,
    HP_HDR_PNG,
    HP_HDR_UNKNOWN

};

enum hp_pkt_type {
    HP_TP_SETTINGS=0,
    HP_TP_FUNCTIONS=02,
    HP_TP_LIST=03,
    HP_TP_MATRIX=04,
    HP_TP_NOTE=05,
    HP_TP_PROG=06,
    HP_TP_CUSTOM=11
 };

struct usb_header {
    usb_header_type type;
    uint8_t cmd;
    uint8_t typecode;
    int items;
    int pkt_size; //bytes in packet
    int num_chunks;
    int name_length;
    uint16_t embedded_crc;
    int pkt_type;
    int chunk;
    uint8_t header[HEADER_LEN];
    int headerlen;
};

struct hp_cmd {

};

struct hp_pkt_in {
    QByteArray array;
    uint32_t size;
    uint8_t cmd;
    hpCalcData * calc;
    usb_header_type type;
    int pkt_type;
};

struct hp_pkt_out {
    uint8_t * data;
    QByteArray array= QByteArray(1024,0);
    uint32_t size;
    uint8_t cmd;
};

typedef  enum {
    // 5 is triggered periodically by the official connectivity kit. It returns something with a PNG header, but much smaller.
    CALC_SCREENSHOT_FORMAT_FIRST = 8,
    CALC_SCREENSHOT_FORMAT_PRIME_PNG_320x240x16 = 8,
    CALC_SCREENSHOT_FORMAT_PRIME_PNG_320x240x4 = 9,
    CALC_SCREENSHOT_FORMAT_PRIME_PNG_160x120x16 = 10,
    CALC_SCREENSHOT_FORMAT_PRIME_PNG_160x120x4 = 11,
    CALC_SCREENSHOT_FORMAT_LAST ///< Keep this one last
} hp_screenshot_format;

//! Structure defining a raw packet for the Prime, used at the lowest layer of the protocol implementation.
typedef struct
{
    uint32_t size;
    uint8_t data[PRIME_RAW_DATA_SIZE + 1];
} prime_raw_hid_pkt;

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
        void dumpDevice(libusb_device * );
        int hp_open(hp_Handle *);
        int submit_async_transfer(hp_Handle *, hp_pkt_in *, hp_pkt_out *);
        int submit_sync_s_transfer(hp_Handle *, hp_pkt_out *);
        int submit_sync_r_transfer(hp_Handle *, hp_pkt_in *);
        int sync_s_recv_file(hp_Handle *);
        int sync_r_recv_file(hp_Handle *);
        int extract_header(uint8_t *, usb_header *);
        int submit_callback();
        int hp_close(hp_Handle * );
        int hp_func();
        int is_ready(hp_Handle *);
        int load_info(hp_Handle *);
        int lookfordouble (QByteArray , int );
        int get_info( /*calc_infos * infos*/);
        int get_settings(hp_Handle *);
        int set_settings(hp_Handle * , hp_Settings set);
        int get_screen_shot(hp_Handle *);
        int send_screen_shot(hp_pkt_in *);
        int send_info(hp_pkt_in *);
        int send_file(hp_pkt_in *);
        int data_dispatch(hp_pkt_in *);

        int vpkt_send_experiments(hp_Handle * handle, int cmd);
        // Function Prototypes:

        void print_libusb_transfer(struct libusb_transfer *p_t);

        friend  void sighandler(int signum);
        friend    void cb_out(struct libusb_transfer *transfer);
        friend    void cb_in(struct libusb_transfer *transfer);

        ~hpusb();
};

#endif // HPUSB_H
