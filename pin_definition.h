#ifndef PIN_DEFINITION_H
#define PIN_DEFINITION_H

#define AM335_GPIO(bank,line)  (32 * bank + line)

#define HCSR04_INPUT    AM335_GPIO(1, 15)       // PIN 47
#define HCSR04_OUTPUT   AM335_GPIO(0, 27)       // PIN 27

#define GPIO_OUT_0      AM335_GPIO(0, 0)       // PIN ?
#define GPIO_OUT_1      AM335_GPIO(0, 0)       // PIN ?
#define GPIO_OUT_2      AM335_GPIO(0, 0)       // PIN ?
#define GPIO_OUT_3      AM335_GPIO(0, 0)       // PIN ?

#include <linux/regmap.h>
#include <linux/iio/iio.h>

#define HMC5883L_CONFIG_REG_A			0x00
#define HMC5883L_CONFIG_REG_B			0x01
#define HMC5883L_MODE_REG			0x02
#define HMC5883L_DATA_OUT_MSB_REGS		0x03
#define HMC5883L_STATUS_REG			0x09
#define HMC5883L_ID_REG				0x0a
#define HMC5883L_ID_END				0x0c

enum HMC5883L_ids {
	HMC5843_ID,
	HMC5883_ID,
	HMC5883L_ID,
	HMC5983_ID,
};

/**
 * struct HMC5883L_data	- device specific data
 * @dev:		actual device
 * @lock:		update and read regmap data
 * @regmap:		hardware access register maps
 * @variant:		describe chip variants
 * @scan:		buffer to pack data for passing to
 *			iio_push_to_buffers_with_timestamp()
 */
struct HMC5883L_data {
	struct device *dev;
	struct mutex lock;
	struct regmap *regmap;
	const struct HMC5883L_chip_info *variant;
	struct iio_mount_matrix orientation;
	struct {
		__be16 chans[3];
		s64 timestamp __aligned(8);
	} scan;
};

int HMC5883L_common_probe(struct device *dev, struct regmap *regmap,
			 enum HMC5883L_ids id, const char *name);
int HMC5883L_common_remove(struct device *dev);

int HMC5883L_common_suspend(struct device *dev);
int HMC5883L_common_resume(struct device *dev);

#ifdef CONFIG_PM_SLEEP
static __maybe_unused SIMPLE_DEV_PM_OPS(HMC5883L_pm_ops,
					HMC5883L_common_suspend,
					HMC5883L_common_resume);
#define HMC5883L_PM_OPS (&HMC5883L_pm_ops)
#else
#define HMC5883L_PM_OPS NULL
#endif

#endif /* PIN_DEFINITION_H */
