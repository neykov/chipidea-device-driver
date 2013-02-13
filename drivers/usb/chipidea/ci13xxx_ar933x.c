/* Copyright (c) 2010, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/usb/ulpi.h>
#include <linux/usb/gadget.h>
#include <linux/usb/chipidea.h>
#include <asm/mach-ath79/ath79.h>
#include <asm/mach-ath79/ar71xx_regs.h>

#include "ci.h"

static struct ci13xxx_platform_data ci13xxx_ar933x_platdata = {
	.name			= "ci13xxx_ar933x",
	.flags			= 0,
	.capoffset		= DEF_CAPOFFSET
};

static int __devinit ci13xxx_ar933x_probe(struct platform_device *pdev)
{
	u32 bootstrap;
	struct platform_device *plat_ci;

	dev_dbg(&pdev->dev, "ci13xxx_ar933x_probe\n");

	bootstrap = ath79_reset_rr(AR933X_RESET_REG_BOOTSTRAP);
	if (bootstrap & AR933X_BOOTSTRAP_USB_MODE_HOST)
		ci13xxx_ar933x_platdata.flags = CI13XXX_FORCE_HOST_MODE;
	else
		ci13xxx_ar933x_platdata.flags = CI13XXX_FORCE_DEVICE_MODE;

	plat_ci = ci13xxx_add_device(&pdev->dev,
				pdev->resource, pdev->num_resources,
				&ci13xxx_ar933x_platdata);
	if (IS_ERR(plat_ci)) {
		dev_err(&pdev->dev, "ci13xxx_add_device failed!\n");
		return PTR_ERR(plat_ci);
	}

	platform_set_drvdata(pdev, plat_ci);

	pm_runtime_no_callbacks(&pdev->dev);
	pm_runtime_enable(&pdev->dev);

	return 0;
}

static int __devexit ci13xxx_ar933x_remove(struct platform_device *pdev)
{
	struct platform_device *plat_ci = platform_get_drvdata(pdev);

	pm_runtime_disable(&pdev->dev);
	ci13xxx_remove_device(plat_ci);

	return 0;
}

static struct platform_driver ci13xxx_ar933x_driver = {
	.probe = ci13xxx_ar933x_probe,
	.remove = __devexit_p(ci13xxx_ar933x_remove),
	.driver = { .name = "ehci-platform", },
};

module_platform_driver(ci13xxx_ar933x_driver);

MODULE_ALIAS("platform:ar933x_hsusb");
MODULE_LICENSE("GPL v2");
