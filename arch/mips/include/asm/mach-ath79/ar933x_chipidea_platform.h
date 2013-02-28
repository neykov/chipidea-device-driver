/*
 *  Platform data definition for Atheros AR933X Chipidea USB controller
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#ifndef _AR933X_CHIPIDEA_PLATFORM_H
#define _AR933X_CHIPIDEA_PLATFORM_H

#include <linux/usb/otg.h>

struct ar933x_chipidea_platform_data {
	enum usb_dr_mode dr_mode;
};

#endif /* _AR933X_UART_PLATFORM_H */
