/*
 * libhpcables: hand-helds support libraries.
 * Copyright (C) 2013 Lionel Debroux
 * Code patterns and snippets borrowed from libticables & libticalcs:
 * Copyright (C) 1999-2009 Romain Li�vin
 * Copyright (C) 2009-2013 Lionel Debroux
 * Copyright (C) 1999-2013 libti* contributors.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * \file link_prime_hid.c Cables: Prime HID cable.
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <inttypes.h>

#include <hidapi.h>

#include <hplibs.h>
#include <hpcalcs.h>
#include "logging.h"
#include "error.h"

extern const cable_fncts cable_prime_hid_fncts;

static int cable_prime_hid_probe(cable_handle * handle) {
    int res;
    // In fact, we're not using handle here, but let's nevertheless flag misuse of the API.
    if (handle != NULL) {
        // Enumerating the device seems to do the job.
        struct hid_device_info * info = hid_enumerate(USB_VID_HP, USB_PID_PRIME1);
        if (info != NULL) {
            hid_free_enumeration(info);
            res = ERR_SUCCESS;
            hpcables_info("%s: cable probe succeeded, PID=%04X", __FUNCTION__, USB_PID_PRIME1);
        }
        else {
            info = hid_enumerate(USB_VID_HP, USB_PID_PRIME2);
            if (info != NULL) {
                hid_free_enumeration(info);
                res = ERR_SUCCESS;
                hpcables_info("%s: cable probe succeeded, PID=%04X", __FUNCTION__, USB_PID_PRIME2);
            }
            else {
                res = ERR_CABLE_PROBE_FAILED;
                hpcables_error("%s: cable probe failed", __FUNCTION__);
            }
        }
    }
    else {
        res = ERR_INVALID_HANDLE;
        hpcables_error("%s: handle is NULL", __FUNCTION__);
    }
    return res;
}

static int cable_prime_hid_open(cable_handle * handle) {
    int res;
    if (handle != NULL) {
        hid_device * device_handle = hid_open(USB_VID_HP, USB_PID_PRIME1, NULL);
        unsigned int pid = USB_PID_PRIME1;
        if (device_handle) {
device_handle_ok:
            handle->model = CABLE_PRIME_HID;
            handle->handle = (void *)device_handle;
            handle->fncts = &cable_prime_hid_fncts;
            // Especially screenshots can take a while before beginning to send data.
            handle->read_timeout = 8000;
            handle->open = 1;
            handle->busy = 0;
            res = ERR_SUCCESS;
            hpcables_info("%s: cable open succeeded, PID=%04X", __FUNCTION__, pid);
        }
        else {
            device_handle = hid_open(USB_VID_HP, USB_PID_PRIME2, NULL);
            pid = USB_PID_PRIME2;
            if (device_handle) {
                goto device_handle_ok;
            }
            else {
                res = ERR_CABLE_NOT_OPEN;
                hpcables_error("%s: cable open failed", __FUNCTION__);
            }
        }
    }
    else {
        res = ERR_INVALID_HANDLE;
        hpcables_error("%s: handle is NULL", __FUNCTION__);
    }
    return res;
}

static int cable_prime_hid_close(cable_handle * handle) {
    int res;
    if (handle != NULL) {
        hid_device * device_handle = (hid_device *)handle->handle;
        if (device_handle != NULL) {
            if (handle->open) {
                hid_close(device_handle);
                handle->model = CABLE_NUL;
                handle->handle = NULL;
                handle->fncts = NULL;
                handle->open = 0;
                res = ERR_SUCCESS;
                hpcables_info("%s: cable close succeeded", __FUNCTION__);
            }
            else {
                res = ERR_CABLE_NOT_OPEN;
                hpcables_error("%s: cable was not open", __FUNCTION__);
            }
        }
        else {
            res = ERR_INVALID_HANDLE;
            hpcables_error("%s: device_handle is NULL", __FUNCTION__);
        }
    }
    else {
        res = ERR_INVALID_HANDLE;
        hpcables_error("%s: handle is NULL", __FUNCTION__);
    }
    return res;
}

static int cable_prime_hid_set_read_timeout(cable_handle * handle, int read_timeout) {
    int res;
    if (handle != NULL) {
        res = ERR_SUCCESS;
        handle->read_timeout = read_timeout;
    }
    else {
        res = ERR_INVALID_HANDLE;
        hpcables_error("%s: handle is NULL", __FUNCTION__);
    }
    return res;
}


static int cable_prime_hid_send(cable_handle * handle, uint8_t * data, uint32_t len) {
    int res;
    if (handle != NULL && data != NULL) {
        hid_device * device_handle = (hid_device *)handle->handle;
        if (device_handle != NULL) {
            uint32_t bytes_written = 0;
            while (bytes_written < len) {
                if (handle->open) {
                    res = hid_write(device_handle, data + bytes_written, len - bytes_written);
                    if (res >= 0) {
                        bytes_written += res;
                    }
                    else {
                        res = ERR_CABLE_WRITE_ERROR;
                        hpcables_error("%s: write failed %ls", __FUNCTION__, hid_error(device_handle));
                        return res;
                    }
                }
                else {
                    res = ERR_CABLE_NOT_OPEN;
                    hpcables_error("%s: cable was not open", __FUNCTION__);
                    return res;
                }
            }
            res = ERR_SUCCESS;
            hpcables_info("%s: wrote %d bytes", __FUNCTION__, bytes_written);
        }
        else {
            res = ERR_INVALID_HANDLE;
            hpcables_error("%s: device_handle is NULL", __FUNCTION__);
        }
    }
    else {
        res = ERR_INVALID_PARAMETER;
        hpcables_error("%s: an argument is NULL", __FUNCTION__);
    }
    return res;
}

static int cable_prime_hid_recv(cable_handle * handle, uint8_t ** data, uint32_t * len) {
    int res;
    // Use pre-allocated area pointed to by data.
    if (handle != NULL && data != NULL && *data != NULL && len != NULL) {
        hid_device * device_handle = (hid_device *)handle->handle;
        if (device_handle != NULL) {
            if (handle->open) {
                res = hid_read_timeout(device_handle, *data, PRIME_RAW_HID_DATA_SIZE, handle->read_timeout);
                if (res >= 0) {
                    *len = res;
                    res = ERR_SUCCESS;
                    hpcables_info("%s: read %" PRIu32 "bytes", __FUNCTION__, *len);
                }
                else {
                    res = ERR_CABLE_READ_ERROR;
                    hpcables_error("%s: read failed", __FUNCTION__);
                }
            }
            else {
                res = ERR_CABLE_NOT_OPEN;
                hpcables_error("%s: cable was not open", __FUNCTION__);
            }
        }
        else {
            res = ERR_INVALID_HANDLE;
            hpcables_error("%s: device_handle is NULL", __FUNCTION__);
        }
    }
    else {
        res = ERR_INVALID_PARAMETER;
        hpcables_error("%s: an argument is NULL", __FUNCTION__);
    }
    return res;
}

const cable_fncts cable_prime_hid_fncts =
{
    CABLE_PRIME_HID,
    "Prime HID cable",
    "Prime HID cable",
    &cable_prime_hid_probe,
    &cable_prime_hid_open,
    &cable_prime_hid_close,
    &cable_prime_hid_set_read_timeout,
    &cable_prime_hid_send,
    &cable_prime_hid_recv
};
