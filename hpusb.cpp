//
//
//
//
// Note hpusb opens an interface but can have multiple usb ports
// open. Hence handle information is stored else where
//
// Norm:
// return 0 = success
// return <> 0 = error

#include <QTextCodec>

#include "global.h"
#include "hpusb.h"
#include "hpdata.h"
#include <libusb.h>
#include <signal.h>

//Constructor
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

// check device handler for idVendor and idProduct
// replace with libusb_open_device_with_vid_pid
int hpusb::is_device(libusb_device * device) {

     libusb_device_descriptor desc;
     int rc=0;
     rc = libusb_get_device_descriptor(device, &desc);
     if (rc!=0) {
        err(L3,rc,QString(__FUNCTION__) + QString(" Could not get device descriptor "));
     }
     else {
//            log(QString().sprintf("Vendor:Device = %04x:%04x", desc.idVendor, desc.idProduct));
            if ((desc.idVendor==USB_VID_HP)&&(desc.idProduct==USB_PID_PRIME3)) {
                dumpDevice(device);
                return 1;
            }
     }
    return 0;
}

//print the device descriptor
void hpusb::dumpDevice(libusb_device * device) {

    QString dump("Device Descriptor\n");
    libusb_device_descriptor desc;
    int rc;
    if (device) {
        rc = libusb_get_device_descriptor(device, &desc);
        if (!rc) {
        dump+=QString().sprintf("bLength: %d\n",(int)desc.bLength);
        dump+=QString().sprintf("bDescriptor Type: %d\n",(int)desc.bDescriptorType);
        dump+=QString().sprintf("bcdUSB: %d\n",(int)desc.bcdUSB);
        dump+=QString().sprintf("bDeviceClass: %d\n",(int)desc.bDeviceClass);
        dump+=QString().sprintf("bDeviceSubClass: %d\n",(int)desc.bDeviceSubClass);
        dump+=QString().sprintf("bDeviceProtocal: %d\n",(int)desc.bDeviceProtocol);
        dump+=QString().sprintf("bMaxPacketSize0: %d\n",(int)desc.bMaxPacketSize0);
        dump+=QString().sprintf("idVendor: %X\n",(int)desc.idVendor);
        dump+=QString().sprintf("idProduct: %d\n",(int)desc.idProduct);
        dump+=QString().sprintf("bcdDevicel: %d\n",(int)desc.bcdDevice);
        dump+=QString().sprintf("iManufacture: %i\n",desc.iManufacturer);
        dump+=QString().sprintf("iProduct: %i\n",desc.iProduct);
        dump+=QString().sprintf("iSerialNumber: %i\n",desc.iSerialNumber);
        dump+=QString().sprintf("bNumConfigurations: %d\n",desc.bNumConfigurations);
        }
    }
    log(dump);
    qDebug()<<dump;
}


//Send a packet
//Based on hplib - prime_send_data
//credit to Lionel Debroux
int hpusb::submit_sync_s_transfer(hp_Handle * handle, hp_pkt_out * pktout) {

    int ret=-1;
    int trans=0;

    if (handle != NULL && pktout != NULL) {

        prime_raw_hid_pkt raw;
        uint32_t i, q, r;
        uint32_t offset = 0;
        uint8_t pkt_id = 0;
        libusb_device_handle * devh = handle->usbhandle;
        if (devh!=nullptr) {
            memset((void *)&raw, 0, sizeof(raw));
            q = (pktout->size) / (PRIME_RAW_DATA_SIZE);
            r = (pktout->size) % (PRIME_RAW_DATA_SIZE);

            qDebug()<<q;
            qDebug()<<r;

            for (i = 1; i <= q; i++) {
                raw.size = PRIME_RAW_DATA_SIZE;
                if(i>1) {
                    raw.data[0]=  0x01;
                    raw.data[1] = pkt_id;
                    memcpy(raw.data + 2, pktout->data + offset, PRIME_RAW_DATA_SIZE-1);
                    offset += PRIME_RAW_DATA_SIZE - 1;
                  }
                else {
                    memcpy(raw.data, pktout->data + offset, PRIME_RAW_DATA_SIZE-1);
                    offset += PRIME_RAW_DATA_SIZE - 1;
                }

                log("In sync send transfer");
                qDebug()<<QString().sprintf("%s %p",__FUNCTION__,handle->usbhandle);

                //write
                log("Send..");
                ret = libusb_interrupt_transfer(devh, ENDPOINT_OUT, raw.data, raw.size,
                                             &trans,10000);
                if (ret) {
                    log(QString().sprintf("Write Error: %s\n", libusb_error_name(ret)));
                    r = 0;
                    break;
                }
                else {
                    log(QString().sprintf("Write bytes: %d\n", trans));
                }
                // Increment packet ID, which seems to be necessary for computer -> calc packets
                pkt_id++;
                if (pkt_id == 0xFF) {
                    pkt_id = 0; // Skip 0xFF, which is used for other purposes.
                }
              }

            if (r || !pktout->size) {
                if (i>1) {
                    raw.size = r + 2;
                    raw.data[0] = 0x01;
                    raw.data[1] = pkt_id;
                    memcpy(raw.data + 2, pktout->data + offset, PRIME_RAW_DATA_SIZE-1);
                }
                else
                {
                    raw.size = r;
                    memcpy(raw.data, pktout->data + offset, PRIME_RAW_DATA_SIZE-1);
                }
                log("Send small packet..");

                ret = libusb_interrupt_transfer(devh, ENDPOINT_OUT, raw.data, raw.size,
                                             &trans,10000);

                if (ret) {
                    log(QString().sprintf("Write Error: %s\n", libusb_error_name(ret)));
               }
                else {
                    log(QString().sprintf("Write bytes: %d\n", trans));
                }
            }

        }
    }
    return ret;
}

//extracts the header from the pkt and returns it in the header structure
// very exprimental
int hpusb::extract_header(uint8_t * raw, usb_header * uh) {

    int b_0;
    int b_1;
    int b_2;
    int b_3;
    int b_4;
    int b_5;
    int b_6;
    int b_7;
    int b_8;
    int b_9;
    int b_10;

    b_0=raw[0];
    b_1=raw[1];
    b_2=raw[2];
    b_3=raw[3];
    b_4=raw[4];
    b_5=raw[5];
    b_6=raw[6];
    b_7=raw[7];
    b_8=raw[8];
    b_9=raw[9];
    b_10=raw[10];

    qDebug()<<QString("Extract Header");

    if(b_0>0x00) {
        qDebug()<<"Header Chunck";
        uh->type=HP_HDR_CHUNK;
        uh->typecode=0;
        uh->chunk=b_0;
        QString msg = QString("Chunk %1").arg(uh->chunk);
         msg += QString("00 %0x1 ").arg(raw[0]);
         msg += QString("01 %0x1 ").arg(raw[1]);
         msg += QString("02 %0x1 ").arg(raw[2]);
        qDebug()<<msg;
    }
    else
    {
        if((b_0==0x00)&&(b_1==0xfa)) {
            qDebug()<<"Header Std";
            uh->type=HP_HDR_STD;
            uh->cmd=b_1;
            uh->items=0xFFFF;  //assume third byte is header length
            uh->chunk=0;
            uh->size=1024;
        }
        else
        {   //recieve image

            if((b_0==0x00)&&(b_1==0xfc)) {
                //This works - leave
                qDebug()<<"Header PNG";
                uh->type=HP_HDR_PNG;
                uh->cmd=b_1;
                uh->items=b_9;
                uh->chunk=b_0;
                uh->size=1024;
            }
            else  {
                uh->typecode=b_1;
                if(uh->typecode==0XF7)
                {
                    qDebug()<<"Header First";
                    uh->type=HP_HDR_FIRST;
                    uh->items=0xFFFF;
                    QString msg = QString("Size: %1").arg(b_5);
                    qDebug()<<msg;
                    uh->size=(uint32_t)(b_5);
                    uh->chunk=0;
                    msg = QString("First Header item: %1 size: %2").arg(uh->items).arg(uh->size);
                    qDebug()<<msg;
            }
          }
        }
    }
    return 1;

}

//read a synchronuous
int hpusb::submit_sync_r_transfer(hp_Handle * handle, hp_pkt_in * pktin) {

   usb_header uh1;
   usb_header uh2;
   int ret;
   int trans=0;
   int trans_c=0;
   int chunks;
   int count=0;
   uint8_t raw[1024];
   QByteArray in_buffer(1024,0);

   log("Recieve...");

   if (!handle) {
       log("Null handle");
       return -1;
   }

   libusb_device_handle * devh = handle->usbhandle;

   qDebug()<<QString().sprintf("%s %p",__FUNCTION__,handle->usbhandle);

   if (!devh)
       return -1;

    ret = libusb_interrupt_transfer(devh,ENDPOINT_IN,raw,1024,&trans,5000);

    extract_header(raw,&uh1);

    in_buffer.insert(0,(const char *)raw,trans);
    in_buffer.resize(trans);
   uh1.items=1000;
    qDebug()<<QString("Looking for %1 items: %2 bytes read").arg(uh1.items).arg(trans);

    if ((uh1.type==HP_HDR_FIRST)&&(uh1.items>0x00)) {
        qDebug()<<QString("1 First chunk detected %1").arg(uh1.items);

        for (chunks=1; chunks<uh1.items; chunks++) {
            trans_c=0;
            count++;
            //read additional chuncks

             ret = libusb_interrupt_transfer(devh,ENDPOINT_IN,raw,1024,&trans_c,5000);
             qDebug()<<QString("x Reading Chuncks size:%1 count:%2").arg(trans_c).arg(count);

//             extract_header(raw, &uh2);
             //check for empty packet
             if (trans_c>0) {           
                qDebug()<<QString("Copying %1 bytes").arg(trans_c);
                in_buffer.append((const char *)raw,trans_c);
                qDebug()<<QString("another chunk read %1/%2").arg(chunks).arg(raw[0]);
                qDebug()<<QString("uh.item=%1 Chunks=%2").arg(uh1.items).arg(chunks);
                trans+=trans_c;
             }
             else
             {
                         qDebug()<<QString("End detected %1").arg(chunks);
                         chunks=uh1.items;
                         ret=0;
             }
        }
    }

    if ((uh1.type==HP_HDR_STD)&&(uh1.items>0x00)) {
        qDebug()<<QString("First std chunk detected %1").arg(uh1.items);


        for (chunks=1; chunks<uh1.items; chunks++) {
            qDebug()<<QString("Reading std chunk detected %1").arg(chunks);
            trans_c=0;
            count++;

            //read additional chuncks
             ret = libusb_interrupt_transfer(devh,ENDPOINT_IN,raw,1024,&trans_c,5000);
 //            extract_header(raw, &uh2);

             //check for empty packet
             if (trans_c>0) {
                qDebug()<<QString("Copying %1 bytes").arg(trans_c);
                in_buffer.append((const char *)raw,trans_c);
                qDebug()<<QString("another chunk read %1/%2").arg(chunks).arg(raw[0]);
                trans+=trans_c;
             }
             else
             {
                         qDebug()<<QString("End detected %1").arg(chunks);
                         chunks=uh1.items;
                         ret=0;
             }
        }
    }

    if ((uh1.type==HP_HDR_PNG)&&(uh1.items>0x00)) {
        qDebug()<<QString("First png chunk detected %1").arg(uh1.items);

        //first byte used as chunk counter
        int headerlen=1;
        //first message has already been read
        for (chunks=1; chunks<uh1.items; chunks++) {

            //read additional chuncks
             ret = libusb_interrupt_transfer(devh,ENDPOINT_IN,raw,1024,&trans_c,5000);
             extract_header(raw, &uh2);
             //check for empty packet
             if (trans_c>0) {
//                qDebug()<<QString("PNG: Copying %1 bytes").arg(trans_c);
                //Assume first byte a chunck count
                in_buffer.append((const char *)&raw[headerlen],trans_c-headerlen);
                trans+=trans_c-headerlen;
             }
             else
             {
                         qDebug()<<QString("End detected %1").arg(chunks);
                         chunks=uh1.items;
                         ret=0;
             }
        }
    }

    pktin->array=in_buffer;

    if (ret){
        log(QString().sprintf("ERROR in interrupt read: %s\n",  libusb_error_name(ret)));
    }
    else{
        qDebug()<<QString("Exiting %1 ret %2").arg(__FUNCTION__).arg(ret);
        log(QString().sprintf("%d bytes received",trans));
 //       main_err-> dump((uint8_t *)pktin->array.constData(),trans);
        qDebug()<<pktin->array;
    }
    return ret;
}

//request to recieve a file transfer
int sync_s_recv_file(hp_Handle *) {

    return 0;
}

//recieve the file transfer
int sync_r_recv_file(hp_Handle *) {

    return 0;
}

//Get a screen shot. HP replies with image starting on byte 14
//ToDo - make robust
int hpusb::get_screen_shot(hp_Handle * handle, QByteArray * imageData) {

    int res;
    if (handle != NULL) {

        uint8_t transferbuffer[LEN_IN_BUFFER+8];
        hp_pkt_in pktin;
        hp_pkt_out pktout;

        transferbuffer[0]=0x00;
        transferbuffer[1]=CMD_PRIME_RECV_SCREEN;
        transferbuffer[2]=(uint8_t) CALC_SCREENSHOT_FORMAT_PRIME_PNG_320x240x4;

        pktout.cmd = CMD_PRIME_RECV_SCREEN;
        pktout.data = transferbuffer;
        pktout.size=1024;

        if (!(res=submit_sync_s_transfer(handle,&pktout))){

            pktin.cmd= CMD_PRIME_RECV_SCREEN;

            log(QString("%1: Waiting for a reply").arg(__FUNCTION__));

            if (!submit_sync_r_transfer(handle,&pktin)){
                log(QString("%1: Recieved a reply").arg(__FUNCTION__));
                //Trying to understand reply
                int endpos;
                QByteArray rd = pktin.array;
                endpos = rd.indexOf("IEND");
                qDebug()<<"End pos:"<<endpos;
                *imageData = QByteArray(rd.mid(14,endpos+4-14));
//                qDebug()<<*imageData;
                log(QString().sprintf("%d bytes received",pktin.array.size()));
            }
        }

    }

    qDebug()<<QString("In %1").arg(__FUNCTION__);
    return 0;
}

int hpusb::hp_func() {
    return 0;
}

int hpusb::is_ready(hp_Handle * handle) {

    int res;
    if (handle != NULL) {

        uint8_t transferbuffer[LEN_IN_BUFFER*8 ];

        hp_pkt_in pktin;
        hp_pkt_out pktout;

//    transferbuffer[0]=((cmd>>8)&0xFF);
//    transferbuffer[1]=((cmd>>0)&0xFF);

        transferbuffer[0]=0x00;
        transferbuffer[1]=CMD_PRIME_CHECK_READY;

        pktout.cmd = CMD_PRIME_CHECK_READY;
        pktout.data = transferbuffer;
        pktout.size=2;

        if (!(res=submit_sync_s_transfer(handle,&pktout))){

            pktin.cmd= CMD_PRIME_CHECK_READY;
//            pktin.data=in_buffer;
//            pktin.size=1024;
            log(QString("%1: Waiting for a reply").arg(__FUNCTION__));

            if (!submit_sync_r_transfer(handle,&pktin)){
                log(QString("%1: Recieved a reply").arg(__FUNCTION__));
                //Trying to understand reply
                qDebug()<<pktin.array;
                main_err-> dump((uint8_t *)pktin.array.constData(),pktin.array.size());
                QByteArray rd= QByteArray(pktin.array, pktin.array.size());
  //              lookfordouble(rd,64);
            }
        }
        else {
            log(QString("%1: Could not send ready request ").arg(__FUNCTION__));
            return res;
        }
    }
    return res;
 }

int hpusb::load_info(hp_Handle * handle, hp_Information * hpinfo) {

    uint8_t transferbuffer[LEN_IN_BUFFER+8];
//    uint8_t in_buffer[LEN_IN_BUFFER+8];

    hp_pkt_in pktin;
    hp_pkt_out pktout;

    memset((void *)&transferbuffer, 0, sizeof(transferbuffer));

    qDebug()<<QString("%1").arg(__FUNCTION__);

    transferbuffer[0]=0x0;
    transferbuffer[1]=CMD_PRIME_GET_INFOS;

    /*
    transferbuffer[0]=0xFF;
    transferbuffer[1]=0xEC;
    transferbuffer[2]=0x00;
    transferbuffer[3]=0x00;
    transferbuffer[4]=0x00;
    transferbuffer[5]=0x00;
    transferbuffer[6]=0x00;
    transferbuffer[7]=0x00;
    transferbuffer[8]=0x00;
    transferbuffer[9]=0x00;
*/

    pktout.cmd=CMD_PRIME_GET_INFOS;
    pktout.data=transferbuffer;
    pktout.size=2;

    if (submit_sync_s_transfer(handle,&pktout)){
        qDebug()<<"error sending";
    }
    else
    {
        //recieve response
        if(!submit_sync_r_transfer(handle,&pktin)) {

            //unpack data
            log("unpacking data");

            int ind=0;
            QTextCodec * codec = QTextCodec::codecForName("UTF-8");
            QByteArray rd= pktin.array;

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

            return 0;
        }
        else {
            log("failed to read info from device");
            return 1;
        }
    }
    return 0;
}

int hpusb::lookfordouble (QByteArray rd, int start) {
    long double num=0;
    QString app;
    QByteArray str1;
    QTextCodec * codec = QTextCodec::codecForName("UTF-8");
    app = codec->toUnicode(str1);
    log(app);

    int i,ind;
    ind=start;

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

int hpusb::get_info(/*calc_infos * infos*/) {
    return 0;
}

int hpusb::get_settings(hp_Handle * handle, hp_Settings * set) {
    hp_Settings inset;

    uint8_t transferbuffer[LEN_IN_BUFFER+8];

    hp_pkt_in pktin;
    hp_pkt_out pktout;

    transferbuffer[0]=0x0;
    transferbuffer[1]=CMD_PRIME_GET_SETTINGS;


    pktout.data=transferbuffer;
    pktout.size=2;

    if(!handle) {
        err(L3,0,"Passed 0 handle");
        return -1;
    }

    if (!submit_sync_s_transfer(handle,&pktout)){
    }

    if (!submit_sync_r_transfer(handle,&pktin)){
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

    uint8_t transferbuffer[LEN_IN_BUFFER*8 ];

    hp_pkt_in pktin;
    hp_pkt_out pktout;

    qDebug()<<QString("%1").arg(__FUNCTION__);

//    transferbuffer[0]=((cmd>>8)&0xFF);
//    transferbuffer[1]=((cmd>>0)&0xFF);

    transferbuffer[0]=0x00;
    transferbuffer[1]=0xF9;
    transferbuffer[2]=0x00;
    transferbuffer[3]=0x00;
    transferbuffer[4]=0x00;
    transferbuffer[5]=0x00;
    transferbuffer[6]=0x00;
    transferbuffer[7]=0x00;
    transferbuffer[8]=0x00;
    transferbuffer[9]=0x00;
    transferbuffer[10]=0x00;

    //CRC
    quint16 crcBytes = qChecksum((char *)&transferbuffer[2],9);

    qDebug()<<crcBytes;

    transferbuffer[11]=(crcBytes>>8)&0xFF;
    transferbuffer[12]=(crcBytes>>0)&0xFF;

    transferbuffer[11]=0x1c;
    transferbuffer[12]=0xf5;

    transferbuffer[13]=0x97;
    transferbuffer[14]=0x01;
    transferbuffer[15]=0x33;
    transferbuffer[16]=0x45;
    transferbuffer[17]=0xe6;
    transferbuffer[18]=0x76;
    transferbuffer[19]=0x00;
    transferbuffer[20]=0x00;
    transferbuffer[21]=0x00;
    transferbuffer[22]=0x00;
    transferbuffer[23]=0x48;


    transferbuffer[1024]=0x00;
    transferbuffer[1025]=0x00;
    transferbuffer[1026]=0x07;
    transferbuffer[1027]=0x00;
    transferbuffer[1028]=0x00;
    transferbuffer[1029]=0x00;
    transferbuffer[1030]=0xfd;
    transferbuffer[1031]=0x01;

/*
    transferbuffer[0]=0x01;
    transferbuffer[1]=0x01;

    transferbuffer[2]=0x00;
    transferbuffer[3]=0x00;
    transferbuffer[4]=0x07;
    transferbuffer[5]=0x00;
    transferbuffer[6]=0x00;
    transferbuffer[7]=0x00;
    transferbuffer[8]=0xfd;
    transferbuffer[9]=0x01;
*/



    pktout.data=transferbuffer;
    pktout.size=2;

     submit_sync_s_transfer(handle,&pktout);

     submit_sync_r_transfer(handle,&pktin);
    {

        transferbuffer[0]=0x01;
        transferbuffer[1]=0x01;
        transferbuffer[2]=0x00;
        transferbuffer[3]=0x00;
        transferbuffer[4]=0x00;
        transferbuffer[5]=0x07;
        transferbuffer[6]=0x00;
        transferbuffer[7]=0x00;
        transferbuffer[8]=0x00;
        transferbuffer[9]=0x00;

        pktout.data=transferbuffer;
        pktout.size=1024*2;

 //       submit_sync_s_transfer(handle,&pktout);
 //       submit_sync_r_transfer(handle,&pktin);

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
            log(QString().sprintf("%d %x", i, p_t->buffer[i]));
        }
    }
    return;
}


//send a submission
int hpusb::submit_async_transfer(hp_Handle * handle, hp_pkt_in * pktin, hp_pkt_out *pktout) {

//    static uint8_t in_buffer[LEN_IN_BUFFER];

    int buffer_len;
    buffer_len=pktin->size;

    struct sigaction sigact;

    int r = 1;  // result

    do_exit = 0;

    libusb_device_handle * devh = handle->usbhandle;

    //Allocation

    // allocate transfer of data IN (IN to host PC from USB-device)
    transfer_in  = libusb_alloc_transfer(0);

    //Filling
    //libusb_fill_interrupt_setup(in_buffer,LIBUSB_RECIPIENT_DEVICE ,LIBUSB_REQUEST_TYPE_STANDARD,0,0,16);
//    libusb_fill_control_transfer( transfer_in, devh,
//        pktin->data,   // Note: in_buffer is where input data written.
//        cb_in, nullptr, 1000); // no user data

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
        break;
        case out_release:
            libusb_release_interface(devh, 0);
        break;
        }
    return 0;
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

hpusb::~hpusb() {
    libusb_exit(ctx);
}
