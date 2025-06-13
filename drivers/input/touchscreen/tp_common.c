#include <linux/device.h>
#include <linux/input/tp_common.h>

bool capacitive_keys_enabled;
static struct class *touch_class;
static struct device *touch_dev;

#define TS_ENABLE_FOPS(type)                                                   \
	int tp_common_set_##type##_ops(struct tp_common_ops *ops)              \
	{                                                                      \
		static struct kobj_attribute kattr =                           \
			__ATTR(type, (S_IWUSR | S_IRUGO), NULL, NULL);         \
		kattr.show = ops->show;                                        \
		kattr.store = ops->store;                                      \
		return sysfs_create_file(&touch_dev->kobj, &kattr.attr);       \
	}

TS_ENABLE_FOPS(capacitive_keys)
TS_ENABLE_FOPS(double_tap)
TS_ENABLE_FOPS(reversed_keys)

static int __init tp_common_init(void)
{
	touch_class = class_create(THIS_MODULE, "touch");
	if (IS_ERR(touch_class))
		return PTR_ERR(touch_class);

	touch_dev = device_create(touch_class, NULL, 'T', NULL, "touch_dev");
	if (IS_ERR(touch_dev)) {
		class_destroy(touch_class);
		return PTR_ERR(touch_dev);
	}
	return 0;
}

core_initcall(tp_common_init);
