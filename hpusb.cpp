//
//
//
//
// Note hpusb opens an interface but can have multiple usb ports
// open. Hence handle information is stored else where



#include "hpusb.h"
#include "hpdata.h"
#include <libusb.h>
#include <signal.h>

#include "global.h"
#include <QTextCodec>

//return 0 = success
//return > err


hpusb::hpusb()
{
    lb_init=0;
}

// Initialise libusb
// returns 0 if succesfull
int hpusb::hp_init()
{
    int ret=0;

    if(!lb_init) {
        log("Initialising usb interface");

        if(!(ret=libusb_init(&ctx))) {
            log("libusb init ok");
            libusb_set_debug(ctx,LIBUSB_LOG_LEVEL_DEBUG);
            lb_init=1;
            return ret;
    }
    else
        err(L3,ret,QString(QString().sprintf("Could not open libusb: %d",ret)));
    }
    return ret;
}

//open usb and claim it
int hpusb::hp_open(hp_Handle * handle) {

    int ret=0;
    // discover devices
    libusb_device **list;
    libusb_device *found = nullptr;
    libusb_device_handle * devh;

    if(!lb_init)
        hp_init();

    if ((!handle->dev_open)&&(lb_init)) {

        ssize_t cnt = libusb_get_device_list(nullptr, &list);
        ssize_t i = 0;

        if (cnt < 0)
            err(L3,0,"Could not get a device list");
        for (i = 0; i < cnt; i++) {
            libusb_device *device = list[i];
            if (is_device(device)) {
                found = device;
                log("Device found");
                break;
            }
        }

        if (found) {
        ret= libusb_open(found, &devh);

        if (ret) {
            err(L3,ret,"Could not open usb device");
            goto endfunc;
            }
            qDebug()<<"set handle";
            if (handle!=nullptr) {
                handle->usbhandle = devh;
                handle->usbdevice = found;
            }
            else
            {
                err(L3,0,"handle null");
            }

            log("Device opened");

            //claim interface
            ret =  libusb_kernel_driver_active	( handle->usbhandle, 0x0) ;
            if (ret==1) {
                log(QString().sprintf("Keneral active"));
                    ret =  libusb_detach_kernel_driver( handle->usbhandle, 0x0) ;
                log(QString().sprintf("Keneral detach: %s\n", libusb_error_name(ret)));
                if (ret!=0) {
                    log(QString().sprintf("Keneral detach error: %s\n", libusb_error_name(ret)));
                    goto endfunc;
                }
            }
            else
            if (ret!=0) {
                log(QString().sprintf("Kernal error %s\n", libusb_error_name(ret)));
                goto endfunc;
            }

            //Note: Configuration 0 causes calculator to reboot
            //                    1 ok
            //
//            ret=libusb_set_configuration(handle->usbhandle,0x1);
//            if (ret!=0) {
//                log(QString().sprintf("Set Configuration: %s\n", libusb_error_name(ret)));
//                goto endfunc;
//            }
            handle->dev_open=1;

            ret = libusb_claim_interface(handle->usbhandle, 0x0);
            if (ret!=0) {
                log(QString().sprintf("Claim interface Error: %s\n", libusb_error_name(ret)));
                return -1;
            }
        }
        else {
            ret=-1;
        }

        endfunc:
        libusb_free_device_list(list, 1);
    }
    return ret;
}

//replace with libusb_open_device_with_vid_pid
int hpusb::is_device(libusb_device * device) {

     libusb_device_descriptor desc = {0};
     int rc=0;
     rc = libusb_get_device_descriptor(device, &desc);
     if (rc!=0) {
        err(L3,rc,QString(__FUNCTION__) + QString(" Could not get device descriptor "));
     }
     else {
//            log(QString().sprintf("Vendor:Device = %04x:%04x", desc.idVendor, desc.idProduct));
            if ((desc.idVendor==USB_VID_HP)&&(desc.idProduct==USB_PID_PRIME3)) {
                return 1;
            }
     }
    return 0;
}

int hpusb::submit_sync_transfer(hp_Handle * handle, hp_pkt_in * pktin, hp_pkt_out * pktout) {

    log("In sync transfer");

    if (!handle)
        return -1;

    libusb_device_handle * devh = handle->usbhandle;
    qDebug()<<QString().sprintf("%s %p",__FUNCTION__,handle->usbhandle);

    int ret;
    int trans=0;

    if (!devh)
        return -1;
    //write
    log("Send..");
    ret = libusb_interrupt_transfer(devh, 0x02, pktin->buffer, pktin->size,
                                             &trans,10000);

    log(QString().sprintf("Write Error: %s\n", libusb_error_name(ret)));
    log(QString().sprintf("Write bytes: %d\n", trans));

    //read
    log("Recieve...");
    ret = libusb_interrupt_transfer(devh,0x81,pktout->buffer,pktout->size-8,&trans,10000);

    log(QString().sprintf("read: %d\n", trans));

    if (ret){
        log(QString().sprintf("ERROR in interrupt read: %s\n",  libusb_error_name(ret)));
    }
    else{
        //printf("%d receive %d bytes from device: %s\n");

        log(QString().sprintf("%d bytes received",trans));
        main_err-> dump(pktout->buffer,trans);
    }
    return ret;
}

//send a submission
int hpusb::submit_async_transfer(hp_Handle * handle, hp_pkt_in * pktin, hp_pkt_out *pktout) {

//    static uint8_t in_buffer[LEN_IN_BUFFER];

    int buffer_len;
    buffer_len=pktin->size;

    struct sigaction sigact;

    int r = 1;  // result
    int i;
    do_exit = 0;

    libusb_device_handle * devh = handle->usbhandle;

    //Allocation

    // allocate transfer of data IN (IN to host PC from USB-device)
    transfer_in  = libusb_alloc_transfer(0);

    //Filling
    //libusb_fill_interrupt_setup(in_buffer,LIBUSB_RECIPIENT_DEVICE ,LIBUSB_REQUEST_TYPE_STANDARD,0,0,16);
    libusb_fill_control_transfer( transfer_in, devh,
        pktin->buffer,   // Note: in_buffer is where input data written.
        cb_in, nullptr, 1000); // no user data

    //take the initial time measurement
    clock_gettime(CLOCK_REALTIME, &t1);

    //Submission

    //submit the transfer, all following transfers are initiated from the CB
    r = libusb_submit_transfer(transfer_in);

    // Define signal handler to catch system generated signals
    // (If user hits CTRL+C, this will deal with it.)
    sigact.sa_handler = sighandler;  // sighandler is defined below. It just sets do_exit.
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigaction(SIGINT, &sigact, NULL);
    sigaction(SIGTERM, &sigact, NULL);
    sigaction(SIGQUIT, &sigact, NULL);

    printf("Entering loop to process callbacks...\n");

    /* The implementation of the following while loop makes a huge difference.
     * Since libUSB asynchronous mode doesn't create a background thread,
     * libUSB can't create a callback out of nowhere. This loop calls the event handler.
     * In real applications you might want to create a background thread or call the event
     * handler from your main event hanlder.
     * For a proper description see:
     * http://libusbx.sourceforge.net/api-1.0/group__asyncio.html#asyncevent
     * http://libusbx.sourceforge.net/api-1.0/group__poll.html
     * http://libusbx.sourceforge.net/api-1.0/mtasync.html
     */
    int c=0;
    if(0){
        qDebug()<<"At loop";
        // This implementation uses a blocking call
        while (!do_exit) {
            c++;
            r =  libusb_handle_events_completed(ctx, NULL);
            if ((r < 0)||(c>100000)){   // negative values are errors
                qDebug()<<"At break";
                exitflag = out_deinit;
                break;
            }
        }
    }
    else{
        // This implementation uses a blocking call and aquires a lock to the event handler
        struct timeval timeout;
        timeout.tv_sec  = 0;       // seconds
        timeout.tv_usec = 100000;  // ( .1 sec)
        libusb_lock_events(ctx);
        c=0;
        while (!do_exit) {
            c++;
            r = libusb_handle_events_locked(ctx, &timeout);
            if ((r < 0)||(c>10000)){   // negative values are errors
                exitflag = out_deinit;
                        qDebug()<<"At break 2";
                break;
            }
        }
        libusb_unlock_events(ctx);
    }

    // If these transfers did not complete then we cancel them.
    // Unsure if this is correct...
    if (transfer_out) {
        r = libusb_cancel_transfer(transfer_out);
        if (0 == r){
            printf("transfer_out successfully cancelled\n");
        }
        if (r < 0){
            exitflag = out_deinit;
        }

    }
    if (transfer_in) {
        r = libusb_cancel_transfer(transfer_in);
        if (0 == r){
            printf("transfer_in successfully cancelled\n");
        }
        if (r < 0){
            exitflag = out_deinit;
        }
    }

    //Completion Handling


    //Deallocation

    switch(exitflag){
        case out_deinit:
            printf("at out_deinit\n");
            libusb_free_transfer(transfer_out);
            libusb_free_transfer(transfer_in);
        case out_release:
            libusb_release_interface(devh, 0);
        }
}

// This will catch user initiated CTRL+C type events and allow the program to exit
void sighandler(int signum)
{
    printf("sighandler\n");
}


//submission callback
// Out Callback
//   - This is called after the Out transfer has been received by libusb
void cb_out(struct libusb_transfer *transfer) {

    QString().sprintf("status =%d, actual_length=%d\n",
                                transfer->status, transfer->actual_length);

}

// In Callback
//   - This is called after the command for version is processed.
//     That is, the data for in_buffer IS AVAILABLE.
void cb_in(struct libusb_transfer *transfer)
{

    qDebug()<<"in cb_in";
    uint32_t benchPackets=1;
    uint32_t benchBytes=0;
    struct timespec t1, t2;
    uint32_t diff=0;

    //measure the time
    clock_gettime(CLOCK_REALTIME, &t2);
    //submit the next transfer
    //libusb_submit_transfer(transfer_in);

    benchBytes += transfer->actual_length;
    //this averages the bandwidth over many transfers
    if(++benchPackets%100==0){
        //Warning: uint32_t has a max value of 4294967296 so this will overflow over 4secs
        diff = (t2.tv_sec-t1.tv_sec)*1000000000L+(t2.tv_nsec-t1.tv_nsec);
        t1.tv_sec = t2.tv_sec;
        t1.tv_nsec = t2.tv_nsec;
        printf("\rreceived %5d transfers and %8d bytes in %8d us, %8.1f B/s", benchPackets, benchBytes, diff/1000, benchBytes*1000000.0/(diff/1000));
        fflush(stdout);
        benchPackets=0;
        benchBytes=0;
    }
}

int hpusb::hp_func() {
    return 0;
}

int hpusb::is_ready() {

    return 0;
}

int hpusb::load_info(hp_Handle * handle, hp_Information * hpinfo) {

    uint8_t transferbuffer[PRIME_RAW_HID_DATA_SIZE ];
    uint8_t out_buffer[LEN_IN_BUFFER+8];

    hp_pkt_in pktin;
    hp_pkt_out pktout;

    transferbuffer[0]=0x0;
    transferbuffer[1]=CMD_PRIME_GET_INFOS;

    pktin.buffer=transferbuffer;
    pktin.size=2;

    pktout.buffer=out_buffer;
    pktout.size=sizeof(out_buffer);
//    pktout.size=PRIME_RAW_HID_DATA_SIZE+16;

    if (!submit_sync_transfer(handle,&pktin,&pktout)){
    //unpack data
        log("unpacking data");

        int ind=0;
        QTextCodec * codec = QTextCodec::codecForName("UTF-8");
        QByteArray rd= QByteArray(reinterpret_cast<const char*>(pktout.buffer), pktout.size);

        //find name
        ind=rd.indexOf(QChar(0x6c),0)+1;
        QByteArray str1 =rd.mid(ind,64);

        QString name;
        name = codec->toUnicode(str1);
        hpinfo->name=name;

        //find OS Version
        unsigned char searchstr[] = {0x80,0x20,0x80,0x01,0x62,0x01};
        ind+=rd.indexOf((char *) searchstr,ind+64)+4;
        qDebug()<<ind;
        //ind+=;
        str1 =rd.mid(ind,16);
        QString osv;
        osv = codec->toUnicode(str1);
        hpinfo->osver=osv;
        qDebug()<<osv;
        log(osv);

        //find Serial Number
        ind+=16;
        str1 =rd.mid(ind,16);
        QString serial;
        serial = codec->toUnicode(str1);
        hpinfo->serialnum=serial;
        log(serial);

        //find Application Version
        ind+=16;
        str1 =rd.mid(ind,16);
        QString app;
        app = codec->toUnicode(str1);
        //hpinfo->appver=app;
        log(app);



//       QByteArray db= QByteArray(reinterpret_cast<const double*>(&pktout.buffer[ind]), pktout.size-ind);



        long double num=0;
        int i;
        ind+=16;
        for (i=0;  i<300; i++) {
            ind+=1;
            str1 =rd.mid(ind,sizeof(num));
            num= *(long double*)(str1.constData());
            app = codec->toUnicode(str1);
           // qDebug()<<num;
            qDebug()<<QString().sprintf("%e",num);
            qDebug()<<app;
        }
        return 0;
    }
    else {
        log("failed to read info from device");
        return 1;
    }

    return 0;
}

int hpusb::get_info(/*calc_infos * infos*/) {
    return 0;
}

int hpusb::get_settings(hp_Handle * handle, hp_Settings * set) {
    hp_Settings inset;

    uint8_t transferbuffer[PRIME_RAW_HID_DATA_SIZE ];
    uint8_t out_buffer[LEN_IN_BUFFER+8];

    hp_pkt_in pktin;
    hp_pkt_out pktout;

    transferbuffer[0]=0x0;
    transferbuffer[1]=CMD_PRIME_GET_SETTINGS;

    pktin.buffer=transferbuffer;
    pktin.size=2;

    pktout.buffer=out_buffer;
    pktout.size=sizeof(out_buffer);
//    pktout.size=PRIME_RAW_HID_DATA_SIZE+16;
    if(!handle) {
        err(L3,0,"Passed 0 handle");
        return -1;
    }

    if (!submit_sync_transfer(handle,&pktin,&pktout)){
    }

    inset.entry=1;

    if(set)
        *set=inset;

    return 0;
}

int hpusb::set_settings(hp_Handle * handle, hp_Settings set) {


    return 0;
}


int hpusb::vpkt_send_experiments(hp_Handle * handle, int cmd) {

    uint8_t transferbuffer[PRIME_RAW_HID_DATA_SIZE ];
    uint8_t out_buffer[LEN_IN_BUFFER+8];

    hp_pkt_in pktin;
    hp_pkt_out pktout;

    transferbuffer[0]=0x0;
    transferbuffer[1]=cmd;

    pktin.buffer=transferbuffer;
    pktin.size=2;

    pktout.buffer=out_buffer;
    pktout.size=sizeof(out_buffer);
//    pktout.size=PRIME_RAW_HID_DATA_SIZE+16;

    if (!submit_sync_transfer(handle,&pktin,&pktout)) {
        return 0;
    }

    return 1;
}

int hpusb::hp_close(hp_Handle * handle)
{
    if (handle) {
        if (handle->dev_open) {
            libusb_release_interface(handle->usbhandle, 0);
            libusb_close(handle->usbhandle);
            handle->dev_open =0;
         }
    }
    return 0;
}


// debugging function to display libusb_transfer
void hpusb::print_libusb_transfer(struct libusb_transfer *p_t)
{	int i;
    if ( NULL == p_t){
        log("No libusb_transfer...\n");
    }
    else {
        log("libusb_transfer structure:\n");
        log(QString().sprintf("flags   =%x \n", p_t->flags));
        log(QString().sprintf("endpoint=%x \n", p_t->endpoint));
        log(QString().sprintf("type    =%x \n", p_t->type));
        log(QString().sprintf("timeout =%d \n", p_t->timeout));
        // length, and buffer are commands sent to the device
        log(QString().sprintf("length        =%d \n", p_t->length));
        log(QString().sprintf("actual_length =%d \n", p_t->actual_length));
        log(QString().sprintf("buffer    =%p \n", p_t->buffer));

        for (i=0; i < p_t->length; i++){
            log(QString().sprintf(" %x", i, p_t->buffer[i]));
        }
    }
    return;
}

hpusb::~hpusb() {
    libusb_exit(ctx);
}
