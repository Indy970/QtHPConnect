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
#include <QObject>

#include "global.h"
#include "hpdata.h"
#include <libusb.h>
#include <signal.h>
#include "hpusb.h"

uint16_t crc16_block(const uint8_t * buffer, uint32_t len) {
    static const uint16_t ccitt_crc16_table[256] = {
        0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
        0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
        0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
        0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
        0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
        0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
        0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
        0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
        0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
        0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
        0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
        0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
        0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
        0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
        0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
        0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
        0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
        0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
        0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
        0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
        0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
        0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
        0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
        0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
        0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
        0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
        0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
        0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
        0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
        0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
        0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
        0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
    };
    uint16_t crc = 0;

    while (len--) {
       crc = ccitt_crc16_table[(crc >> 8) ^ *buffer++] ^ (crc << 8);
    }
    return crc;
}

//hot plug call back
int hotplug_callback(struct libusb_context *ctx, struct libusb_device *dev,
                     libusb_hotplug_event event, void *hp_usbptr) {

    hpusb * usbptr;
    usbptr=(hpusb*)hp_usbptr;
    qDebug()<<"hotplug_callback: Hot plugin";
    if (hp_usbptr) {
        usbptr->hotplugcallback(ctx,dev,event);
    }
    return 0;
}

//Constructor
hpusb::hpusb():QObject()
{
    lb_init=0;
}

// Initialise libusb
// returns 0 if succesfull
int hpusb::hp_init()
{
    int ret=0;
    libusb_hotplug_callback_handle callback_handle;
    int rc;

    if(!lb_init) {
        log("Initialising usb interface");

        if(!(ret=libusb_init(&ctx))) {
            log("libusb init ok");
            libusb_set_debug(ctx,LIBUSB_LOG_LEVEL_WARNING);

            rc = libusb_hotplug_register_callback(ctx,	(libusb_hotplug_event) (LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED |
                                                  LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT), LIBUSB_HOTPLUG_NO_FLAGS , USB_VID_HP, USB_PID_PRIME3,
                                                 LIBUSB_HOTPLUG_MATCH_ANY, hotplug_callback, this,
                                                  &callback_handle);

            if (LIBUSB_SUCCESS != rc) {
              log("Error creating a hotplug callback\n");
              libusb_exit(ctx);
              return EXIT_FAILURE;
            }
            log("hpusb::hp_init - hotplug registered\n");
            hp_callback_handle=callback_handle;
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

    qDebug()<<QString("Extracting Header");
    int Int32=0;

    //Chunck Header
    if(raw[0]>0x00) {
        qDebug()<<"hpusb:Chunck Header";
        uh->type=HP_HDR_CHUNK;
        uh->typecode=0;
        uh->chunk=raw[0];
        uh->headerlen=1;
        QString msg = QString("hpusb:Chunk Number: %1").arg(uh->chunk);
        qDebug()<<msg;
        return 0;
    }

    //Info Header
    if((raw[0]==0x00)&&(raw[1]==0xfa)) {
            uh->type=HP_HDR_INFO;
            uh->typecode=raw[1];
            uh->items =raw[2];
            uh->pkt_size=PRIME_RAW_DATA_SIZE;  //assume third byte is header length
            uh->num_chunks=(uh->pkt_size-1)/PRIME_RAW_DATA_SIZE;
            uh->name_length=0;
            uh->headerlen=6;
            uh->embedded_crc = 0;
             memcpy(uh->header,raw,6);
            QString msg = QString("Info Header chunks: %1 size: %2 items: %3")
                    .arg(uh->num_chunks).arg(uh->pkt_size).arg(uh->items);
            qDebug()<<msg;
            return 0;
     }

    //PNG Header
     if((raw[0]==0x00)&&(raw[1]==0xfc)) {
                //This works - leave
                qDebug()<<"hpusb:PNG Header";
                uh->type=HP_HDR_PNG;              
                uh->typecode=raw[1];
                uh->items=raw[2];
                Int32=0;
                Int32=(Int32<<8)+raw[3];
                Int32=(Int32<<8)+raw[4];
                Int32=(Int32<<8)+raw[5];
                Int32=(Int32<<8)+raw[6];
                uh->pkt_size=Int32;  //assume third byte is header length
                uh->num_chunks=uh->pkt_size/PRIME_RAW_DATA_SIZE;
                uh->name_length=0;
                uh->embedded_crc = (((uint16_t)(raw[8] & 0xFF)) << 8) | ((uint16_t)(raw[7] & 0xFF));
                uh->headerlen=14;
                memcpy(uh->header,raw,HEADER_LEN);
                QString msg = QString("PNG Header chunks: %1 size: %2 Items: %3")
                        .arg(uh->num_chunks).arg(uh->pkt_size).arg(uh->items);
                qDebug()<<msg;

                return 0;
      }

    //Filed Header
    if((raw[0]==0x00)&&(raw[1]==0xf7)) {

                qDebug()<<"hpusb:File Header";
                uh->type=HP_HDR_FILE;
                uh->typecode=raw[1];
                uh->items =raw[2];
                Int32=0;
                Int32=(Int32<<8)+raw[3];
                Int32=(Int32<<8)+raw[4];
                Int32=(Int32<<8)+raw[5];
                Int32=(Int32<<8)+raw[6];
                uh->pkt_size=Int32;  //assume third byte is data length
                uh->pkt_type=raw[7]; //File type being read
                uh->num_chunks=uh->pkt_size/PRIME_RAW_DATA_SIZE;
                uh->name_length=0;
                uh->headerlen=8;
                uh->embedded_crc = 0;
                memcpy(uh->header,raw,HEADER_LEN);
                QString msg = QString("File Header chunks: %1 size: %2 items: %3")
                        .arg(uh->num_chunks).arg(uh->pkt_size).arg(uh->items);
                qDebug()<<msg;
                return 0;
     }

    //Unknown Header
     if((raw[0]==0x00)) {

                qDebug()<<"Unknown Header";
                uh->type=HP_HDR_UNKNOWN;
                uh->typecode=raw[1];
                uh->items =raw[2];
                Int32=0;
                Int32=(Int32<<8)+raw[3];
                Int32=(Int32<<8)+raw[4];
                Int32=(Int32<<8)+raw[5];
                Int32=(Int32<<8)+raw[6];
                uh->pkt_size=Int32;  //assume third byte is data length
                uh->num_chunks=uh->pkt_size/PRIME_RAW_DATA_SIZE;
                uh->name_length=raw[7];
                uh->headerlen=11;
                uh->embedded_crc = (((uint16_t)(raw[7])) << 8) | ((uint16_t)(raw[8]));
                memcpy(uh->header,raw,HEADER_LEN);
                QString msg = QString("File Header chunks: %1 size: %2 items: %3")
                        .arg(uh->num_chunks).arg(uh->pkt_size).arg(uh->items);
                qDebug()<<msg;
        }
       qDebug()<<"hpusb::extract_header: Unknown Header Found";
     return 1;
}

//read a synchronuous
int hpusb::submit_sync_r_transfer(hp_Handle * handle, hp_pkt_in * pktin) {

   usb_header uh1;
   usb_header uh2;
   hp_pkt_in pkt;
   int ret;
   int trans=0;
   int trans_c=1024;
   int chunks;
   int q,r;
   int extra=0;
   int bytes_to_read;
   int exitflag=1;
   quint8 crc;

   uint8_t raw[PRIME_RAW_DATA_SIZE];
   QByteArray in_buffer(PRIME_RAW_DATA_SIZE,0);
   libusb_device_handle * devh = handle->usbhandle;

   log("hpusb::submit_sync_r_transfer: Receive...");
   qDebug()<<QString().sprintf("%s %p",__FUNCTION__,handle->usbhandle);

   if (!handle) {
       log("Null handle");
       return -1;
   }

   if (!devh)
       return -1;

   pkt.cmd=pktin->cmd;
   pkt.calc=handle->calc;

   while(exitflag) {
    trans_c=0;

    //read
    memset(raw,0,PRIME_RAW_DATA_SIZE);
    if ((ret = libusb_interrupt_transfer(devh,ENDPOINT_IN,raw,PRIME_RAW_DATA_SIZE,&trans_c,TIME_OUT))!=0) {
        qDebug()<<QString("hpusb::submit_sync_r_transfer: Read Error %1").arg(libusb_error_name(ret));
        log(QString("Read Error: %1\n").arg( libusb_error_name(ret)));
        exitflag=0;
        return ret;
    }

    if ((trans_c==0)) {
        qDebug()<<"hpusb::submit_sync_r_transfer: End Found";
        exitflag=0;
    }
    else
    {
        extract_header(raw,&uh1);

        if ((uh1.type!=HP_HDR_FILE)) {
            qDebug()<<"hpusb::submit_sync_r_transfer: Not a file header exit loop";
            exitflag=0;
        }

        in_buffer.insert(0,(const char *)&raw[uh1.headerlen],trans_c-uh1.headerlen);
        in_buffer.resize(trans_c-uh1.headerlen);
        qDebug()<<QString("hpusb::submit_sync_r_transfer: Read %1: Transfered %2").arg(trans_c).arg(trans_c-uh1.headerlen);
        trans=trans_c-uh1.headerlen;

        q = (uh1.pkt_size-1)/ (PRIME_RAW_DATA_SIZE);
        r = uh1.pkt_size % (PRIME_RAW_DATA_SIZE);

         qDebug()<<QString("hpusb::submit_sync_r_transfer: Looking for %1 chunks and %2 bytes read").arg(q).arg(r);

         //read additional chunks if they exist
         for (chunks=1; chunks<=q; chunks++) {
            extra++;
            trans_c=0;

            if(chunks<q) {
                bytes_to_read = PRIME_RAW_DATA_SIZE;
            }
            else {
                    //bytes_to_read=uh1.pkt_size-trans+uh1.headerlen;
                    bytes_to_read = PRIME_RAW_DATA_SIZE;
            }
//            qDebug()<<"Bytes to read "<<bytes_to_read;

            memset(raw,0,PRIME_RAW_DATA_SIZE);
            ret = libusb_interrupt_transfer(devh,ENDPOINT_IN,raw,PRIME_RAW_DATA_SIZE,&trans_c,TIME_OUT);
            extract_header(raw, &uh2);
            qDebug()<<QString("hpusb::submit_sync_r_transfer: Reading Chuncks read:%1 should have read:%2").arg(trans_c).arg(bytes_to_read);
            if (trans_c>0) {
                in_buffer.append((const char *)&raw[uh2.headerlen],trans_c-uh2.headerlen);
                trans+=trans_c-uh2.headerlen;
                qDebug()<<QString("hpusb::submit_sync_r_transfer: Read %1: Transfered %2: Buffer now %3 ").arg(trans_c).arg(trans_c-uh2.headerlen).arg(trans);
            }
            else
            {
                     qDebug()<<QString("hpusb::submit_sync_r_transfer End detected %1").arg(chunks);
                     chunks=q;
                     exitflag=0;
                     ret=0;
            }
        }
     }

     qDebug()<<QString("hpusb::submit_sync_r_transfer: Checking for exit trans:%1 pkt_size:%2").arg(trans).arg(uh1.pkt_size);
     if (trans+uh1.headerlen>=uh1.pkt_size) {
     }

//     pkt.array=in_buffer.mid(0,uh1.pkt_size-uh1.headerlen+3);

     //PROBLEM : Needed to -1 to get text file to work?
     pkt.array=in_buffer.mid(0,uh1.pkt_size);

     qDebug()<<QString("hpusb::submit_sync_r_transfer: Read %1: Copied %2 ").arg(trans).arg(pkt.array.size());

     QByteArray temp((const char *)&(uh1.header[0]),uh1.headerlen);
     temp[7]=0x00;
     temp[8]=0x00;

     temp.append(pkt.array);
//     crc=crc16_block((uint8_t *)temp.constData(),temp.size());
     crc= qChecksum(pkt.array,pkt.array.size());
//     crc=crc16_block((uint8_t *)pkt.array.constData(),pkt.array.size());
     qDebug()<<QString("hpusb::submit_sync_r_transfer: CRC calc: x%1").arg(crc,0,16)<<
               QString("hpusb::submit_sync_r_transfer: CRC Expected x%1").arg(uh1.embedded_crc,0,16);

     pkt.type=uh1.type;
     pkt.pkt_type=uh1.pkt_type;
     data_dispatch(&pkt);
   }
    return ret;
}

//request to recieve a file transfer
int hpusb::data_dispatch(hp_pkt_in * pktin) {

    switch(pktin->type) {
        case HP_HDR_PNG:
            send_screen_shot(pktin);
        break;
        case HP_HDR_INFO:
            send_info(pktin);
        break;
        case HP_HDR_FILE:
            send_file(pktin);
        break;
    }
    main_err-> dump((uint8_t *)pktin->array.constData(),pktin->array.size());

    return 0;
}

//request to recieve a file transfer
int hpusb::sync_s_recv_file(hp_Handle *) {

    return 0;
}

//recieve the file transfer
int hpusb::sync_r_recv_file(hp_Handle *) {

    return 0;
}

int hpusb::send_info(hp_pkt_in * pkt) {

    hp_Information hpinfo;

    qDebug()<<"hpusb:In send info";
    if( pkt->calc!=nullptr) {

       log("Unpacking Data");
       int ind=0;
       QTextCodec * codec = QTextCodec::codecForName("UTF-16LE");
       QTextCodec * codec8 = QTextCodec::codecForName("UTF-8");
       QByteArray rd= pkt->array;

            //find name
            ind=rd.indexOf(QChar(0x6c),0)+1;
            QByteArray str1 =rd.mid(ind,64);

            QString name;
            name = codec->toUnicode(str1);
            hpinfo.name=name;

        //unsigned char searchstr[] = {0x80,0x20,0x80,0x01,0x62};
        //ind+=rd.indexOf((char *) searchstr,ind+64);
        ind +=64;

        //find Application Version
        //FIX
        str1 =rd.mid(ind,12);

        //test
        QTime t;
        QDataStream ds1(str1);
        ds1.setByteOrder(QDataStream::LittleEndian);
        ds1>>t;
        qDebug()<<t;

        QDataStream ds(str1);
        ds.setByteOrder(QDataStream::LittleEndian);

        uint16_t num;
        int j;
        qint16 listnum[6];
        for (j=0;j<6;j++) {
            ds >> listnum[j];
            qDebug()<<listnum[j];
        }
        //end test
        qDebug()<<str1;
        QString app;
        app = codec8->toUnicode(str1);
        hpinfo.appver=QString("v%1").arg(listnum[4]);
        log(app);

        //find OS Version
        ind+=12;
        str1 =rd.mid(ind,16);
//        qDebug()<<str1;
        QString osv;
        osv = codec8->toUnicode(str1);
        hpinfo.osver=osv;
//        qDebug()<<osv;
        log(osv);

        //find Serial Number
        ind+=16;
        str1 =rd.mid(ind,16);
        QString serial;
        serial = codec8->toUnicode(str1);
        hpinfo.serialnum=serial;
        log(serial);
        pkt->calc->recvInfo(hpinfo);
        return 0;

        }
    else {
            log("Passed a null pointer");
            return 1;
        }
    //Error
    return 1;
}

int hpusb::send_screen_shot(hp_pkt_in * pkt) {

    int endpos;
    QByteArray imageData;
    QPixmap * screenShot;
    hp_ScreenShot shot;

    if( pkt->calc!=nullptr) {

        endpos = pkt->array.indexOf("IEND");
        qDebug()<<"hpusb::send_screen_shot: End pos:"<<endpos;
        imageData = QByteArray(pkt->array.mid(0,endpos+4));
        screenShot = new QPixmap();
        screenShot->loadFromData(imageData);
        shot.image=screenShot;
//        shot.format=0;
        pkt->calc->recvScreen(shot);
        return 0;
    }
    //Error
    return 1;
}

//File Processor
int hpusb::send_file(hp_pkt_in * pkt) {

    qDebug()<<"hpusb::send_file: In File Processor";

    QString filename;
    QTextCodec * codec = QTextCodec::codecForName("UTF-16LE");
    QByteArray rd= pkt->array;
    int len;
    qint8 crc;

    len = rd[0];

    //TODO

    //What is byte rd[1],rd[2]?
    //Check if CRC

    QByteArray temp((const char *)pkt->array,pkt->array.size());
    temp[1]=0x00;
    temp[2]=0x00;
    crc=crc16_block((uint8_t *)temp.constData(),temp.size());
//     crc= qChecksum(pkt.array,pkt.array.size());
//     crc=crc16_block((uint8_t *)pkt.array.constData(),pkt.array.size());
    qDebug()<<QString(" hpusb::send_file: CRC calc: %1").arg(crc,0,16)<<
              QString("hpusb::send_file: CRC Expected %1 %2").arg((qint8)rd[0],1,16).arg((qint8)rd[1],1,16);

    //find file name
    QByteArray str1 =rd.mid(3,len);
    filename = codec->toUnicode(str1);
    log(QString("File: %1 Type: %2").arg(filename).arg(pkt->pkt_type));

    qDebug()<<"hpusb:Checking file type";
    qDebug()<<QString("File: %1 Type: %2").arg(filename).arg(pkt->pkt_type);
    qDebug()<<QString("%1 %2 %3 %4")
              .arg((uint8_t)rd[0],1,16).arg((uint8_t)rd[1],1,16).arg((uint8_t)rd[2],1,16).arg((uint8_t)rd[3],1,16);

    //handle each file type
    switch (pkt->pkt_type) {

        case HP_TP_SETTINGS: {
               qDebug()<<"hpusb:File type settings";
               qDebug()<<filename;
               hp_Data sData;
               sData.type=HP_MAIN;
               sData.name=filename;
               sData.data=rd.mid(len+3,-1);
               pkt->calc->recvSettings(sData);
         }
        break;
        case HP_TP_FUNCTIONS: {
               qDebug()<<"hpusb:File functions";
               hp_Data sData;
               sData.type=HP_APP;
               sData.name=filename;
               sData.data=rd.mid(len+3,-1);
               pkt->calc->recvData(sData);
        }
        break;
        case HP_TP_LIST: {
            qDebug()<<"hpusb:File type list";
            hp_Data sData;
            sData.type=HP_LIST;
            sData.name=filename;
            sData.data=rd.mid(len+3,-1);
            pkt->calc->recvData(sData);
        }
        break;
        case HP_TP_MATRIX: {
            qDebug()<<"hpusb:File type matrix";
            hp_Data sData;
            sData.type=HP_MATRIX;
            sData.name=filename;
            sData.data=rd.mid(len+3,-1);
            pkt->calc->recvData(sData);
        }
        break;
        case HP_TP_NOTE: {
            //get a note
            int size;
             qDebug()<<"hpusb:File type Note";
            hp_Note note;
            note.filename=filename;

            qDebug()<<QString("%1 %2 %3").arg((uint8_t)rd[len],1,16).arg((uint8_t)rd[len+1],1,16).arg((uint8_t)rd[len+2],1,16);
            size=rd[len+1]; //Is the byte a size?
            QByteArray str1 =rd.mid(len+3,-1);
            note.text = codec->toUnicode(str1);
            note.data=str1;
            pkt->calc->recvNote(note);
        }
        break;
        case HP_TP_PROG: {
            //get a grogram
             int size;
             qDebug()<<"hpusb:File type program";
             hp_Prog prog;
             prog.filename=filename;

             qDebug()<<QString("%1 %2 %3").arg((uint8_t)rd[len],1,16).arg((uint8_t)rd[len+1],1,16).arg((uint8_t)rd[len+2],1,16);
             size=rd[len+1];
             QByteArray str1 =rd.mid(len+3,-1);
             prog.data=str1;
             prog.prog = codec->toUnicode(str1);
             pkt->calc->recvProg(prog);
        }
        break;
        case HP_TP_CUSTOM:
             qDebug()<<"hpusb:File type custom";
        break;
        default:
             qDebug()<<"hpusb:Unknown file type";
    }

    return 0;
}

//Get a screen shot. HP replies with image starting on byte 14
//ToDo - make robust
int hpusb::get_screen_shot(hp_Handle * handle) {

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
/*                int endpos;
                QByteArray rd = pktin.array;
                endpos = rd.indexOf("IEND");
                qDebug()<<"End pos:"<<endpos;
                *imageData = QByteArray(rd.mid(14,endpos+4-14));
//                qDebug()<<*imageData;
                log(QString().sprintf("%d bytes received",pktin.array.size()));*/
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

int hpusb::load_info(hp_Handle * handle) {

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
        if(submit_sync_r_transfer(handle,&pktin)) {
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
    QTextCodec * codec = QTextCodec::codecForName("UTF-16");
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

int hpusb::get_settings(hp_Handle * handle) {
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
    sigaction(SIGINT, &sigact, nullptr);
    sigaction(SIGTERM, &sigact, nullptr);
    sigaction(SIGQUIT, &sigact, nullptr);

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
            r =  libusb_handle_events_completed(ctx, nullptr);
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
        case out:
            qDebug()<<"out";
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

int hpusb::hotplugcallback(struct libusb_context *ctx, struct libusb_device *dev,
                     libusb_hotplug_event event) {
  int count=0;
  qDebug()<<"hpusb::hotplug_callback - Hotplug";;

  static libusb_device_handle *handle = nullptr;
  struct libusb_device_descriptor desc;

  (void)libusb_get_device_descriptor(dev, &desc);

  if (LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED == event) {

      qDebug()<<"Hot plugin event";
      emit hotplug(HP_OPEN_DEVICE);
  } else if (LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT == event) {
    if (handle) {
      emit hotplug(HP_CLOSE_DEVICE);
    }
  } else {
    qDebug()<<QString("hpusb::hotplug_callback Unhandled event %1\n").arg(event);
    log(QString("hpusb::hotplug_callback Unhandled event %1\n").arg(event));
  }
  count++;
  return 0;
}

//eventhandler called periodically to handle async events
int hpusb::eventHandler() {

   int completed;
//   qDebug()<<"In Eventhandler";
   libusb_handle_events_completed(nullptr, &completed);
    return 0;
}

int hpusb::hp_close() {

    libusb_device_handle * usbhandle;
    usbhandle=hp_handle.usbhandle;
    libusb_close(usbhandle);
    hp_handle.usbhandle=nullptr;

    return 0;
}

hpusb::~hpusb() {
    libusb_exit(ctx);
    qDebug()<<"hpusb::close";
}
