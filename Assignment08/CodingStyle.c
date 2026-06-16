// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Louis Solofrizzo <louis@ne02ptzero.me>");
MODULE_DESCRIPTION("A clean, working reverse character device");

/* Variables statiques encapsulées (pas de pollution globale) */
static char device_buffer[PAGE_SIZE];
static size_t buffer_len;
static DEFINE_MUTEX(buffer_lock);

static ssize_t myfd_read(struct file *fp, char __user *user,
			 size_t size, loff_t *offs)
{
	char *rev_buf;
	ssize_t res;
	long i, j;

	mutex_lock(&buffer_lock);

	if (buffer_len == 0) {
		mutex_unlock(&buffer_lock);
		return 0;
	}

	rev_buf = kmalloc(buffer_len + 1, GFP_KERNEL);
	if (!rev_buf) {
		mutex_unlock(&buffer_lock);
		return -ENOMEM;
	}

	/* Inversion de la chaîne avec des index signés (long) */
	for (i = (long)buffer_len - 1, j = 0; i >= 0; i--, j++)
		rev_buf[j] = device_buffer[i];

	rev_buf[buffer_len] = '\0';

	res = simple_read_from_buffer(user, size, offs, rev_buf, buffer_len);

	kfree(rev_buf);
	mutex_unlock(&buffer_lock);

	return res;
}

static ssize_t myfd_write(struct file *fp, const char __user *user,
			  size_t size, loff_t *offs)
{
	ssize_t res;

	if (size >= PAGE_SIZE)
		return -EINVAL;

	mutex_lock(&buffer_lock);

	/* Utilisation sécurisée du tampon d'écriture */
	res = simple_write_to_buffer(device_buffer, PAGE_SIZE - 1, offs,
				     user, size);
	if (res > 0) {
		buffer_len = res;
		device_buffer[buffer_len] = '\0';
	}

	mutex_unlock(&buffer_lock);
	return res;
}

static const struct file_operations myfd_fops = {
	.owner = THIS_MODULE,
	.read  = myfd_read,
	.write = myfd_write,
};

static struct miscdevice myfd_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = "reverse",
	.fops  = &myfd_fops,
};

static int __init myfd_init(void)
{
	int retval;

	retval = misc_register(&myfd_device);
	if (retval) {
		pr_err("Failed to register reverse device\n");
		return retval;
	}

	pr_info("Reverse device registered successfully\n");
	return 0; /* 0 = Succès ! */
}

static void __exit myfd_cleanup(void)
{
	misc_deregister(&myfd_device);
	pr_info("Reverse device unregistered\n");
}

module_init(myfd_init);
module_exit(myfd_cleanup);
