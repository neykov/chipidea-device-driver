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
#include <asm/mach-ath79/ar933x_chipidea_platform.h>

#include "ci.h"

static int ci13xxx_ar933x_probe(struct platform_device *pdev)
{
	u32 bootstrap;
	struct ar933x_chipidea_platform_data *ar933x_chipidea_data;
	struct ci13xxx_platform_data *pdata;
	struct platform_device *plat_ci;

	dev_dbg(&pdev->dev, "ci13xxx_ar933x_probe\n");

	ar933x_chipidea_data = pdev->dev.platform_data;

	pdata = devm_kzalloc(&pdev->dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata) {
		dev_err(&pdev->dev, "Failed to allocate ci13xxx-ar933x pdata!\n");
		return -ENOMEM;
	}

	pdata->name = "ci13xxx_ar933x";
	pdata->capoffset = DEF_CAPOFFSET;
	pdata->dr_mode = ar933x_chipidea_data->dr_mode;
	plat_ci = ci13xxx_add_device(&pdev->dev,
				pdev->resource, pdev->num_resources,
				pdata);
	if (IS_ERR(plat_ci)) {
		dev_err(&pdev->dev, "ci13xxx_add_device failed!\n");
		return PTR_ERR(plat_ci);
	}

	platform_set_drvdata(pdev, plat_ci);

	pm_runtime_no_callbacks(&pdev->dev);
	pm_runtime_enable(&pdev->dev);

	return 0;
}

static int ci13xxx_ar933x_remove(struct platform_device *pdev)
{
	struct platform_device *plat_ci = platform_get_drvdata(pdev);

	pm_runtime_disable(&pdev->dev);
	ci13xxx_remove_device(plat_ci);

	return 0;
}

static struct platform_driver ci13xxx_ar933x_driver = {
	.probe = ci13xxx_ar933x_probe,
	.remove = ci13xxx_ar933x_remove,
	.driver = { .name = "ar933x-chipidea", },
};

module_platform_driver(ci13xxx_ar933x_driver);

MODULE_ALIAS("platform:ci13xxx_ar933x_driver");
MODULE_LICENSE("GPL v2");
