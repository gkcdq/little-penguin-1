// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("milin");
MODULE_DESCRIPTION("Assignment 07: Debugfs Driver");
MODULE_VERSION("0.1");

#define LOGIN_STR "milin\n"
#define LOGIN_LEN 6

/* Pointeurs pour l'arborescence debugfs */
static struct dentry *dir_fortytwo;

/* Variables globales et verrous pour le fichier 'foo' */
static char *foo_buffer;
static size_t foo_size = 0;
static DEFINE_MUTEX(foo_lock);

/* ------------------------------------------------------------------------- */
/* 1. OPERATIONS POUR LE FICHIER 'id'                                        */
/* ------------------------------------------------------------------------- */
static ssize_t id_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	if (*ppos >= LOGIN_LEN)
		return 0;
	if (count > LOGIN_LEN - *ppos)
		count = LOGIN_LEN - *ppos;
	if (copy_to_user(buf, LOGIN_STR + *ppos, count))
		return -EFAULT;
	*ppos += count;
	return count;
}

static ssize_t id_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	char kbuf[32];

	if (count >= sizeof(kbuf))
		return -EINVAL;
	if (copy_from_user(kbuf, buf, count))
		return -EFAULT;
	kbuf[count] = '\0';

	if (count == LOGIN_LEN && memcmp(kbuf, LOGIN_STR, LOGIN_LEN) == 0)
		return count;
	return -EINVAL;
}

static const struct file_operations id_fops = {
	.owner = THIS_MODULE,
	.read  = id_read,
	.write = id_write,
};

/* ------------------------------------------------------------------------- */
/* 2. OPERATIONS POUR LE FICHIER 'jiffies'                                   */
/* ------------------------------------------------------------------------- */
static ssize_t jiffies_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	char tmp[32];
	int len;

	/* On convertit le jiffies actuel en chaîne de caractères */
	len = snprintf(tmp, sizeof(tmp), "%lu\n", jiffies);
	
	if (*ppos >= len)
		return 0;
	if (count > len - *ppos)
		count = len - *ppos;
	if (copy_to_user(buf, tmp + *ppos, count))
		return -EFAULT;
		
	*ppos += count;
	return count;
}

static const struct file_operations jiffies_fops = {
	.owner = THIS_MODULE,
	.read  = jiffies_read,
};

/* ------------------------------------------------------------------------- */
/* 3. OPERATIONS POUR LE FICHIER 'foo' (Thread-Safe avec Mutex)              */
/* ------------------------------------------------------------------------- */
static ssize_t foo_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	ssize_t ret = 0;

	mutex_lock(&foo_lock);
	if (*ppos >= foo_size) {
		ret = 0;
		goto out;
	}
	if (count > foo_size - *ppos)
		count = foo_size - *ppos;
	if (copy_to_user(buf, foo_buffer + *ppos, count)) {
		ret = -EFAULT;
		goto out;
	}
	*ppos += count;
	ret = count;

out:
	mutex_unlock(&foo_lock);
	return ret;
}

static ssize_t foo_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	ssize_t ret = 0;

	/* Pas le droit de dépasser une page mémoire (PAGE_SIZE = 4096) */
	if (count > PAGE_SIZE)
		return -EINVAL;

	mutex_lock(&foo_lock);
	
	/* On remet à zéro à chaque nouvelle écriture standard (comme demandé) */
	if (copy_from_user(foo_buffer, buf, count)) {
		ret = -EFAULT;
		goto out;
	}
	
	foo_size = count;
	ret = count;

out:
	mutex_unlock(&foo_lock);
	return ret;
}

static const struct file_operations foo_fops = {
	.owner = THIS_MODULE,
	.read  = foo_read,
	.write = foo_write,
};

/* ------------------------------------------------------------------------- */
/* INIT / EXIT                                                               */
/* ------------------------------------------------------------------------- */
static int __init debugfs_init(void)
{
	/* Allocation d'une page mémoire pour le tampon foo */
	foo_buffer = kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (!foo_buffer)
		return -ENOMEM;

	/* Création du dossier /sys/kernel/debug/fortytwo */
	dir_fortytwo = debugfs_create_dir("fortytwo", NULL);
	if (IS_ERR(dir_fortytwo)) {
		kfree(foo_buffer);
		return PTR_ERR(dir_fortytwo);
	}

	/* Création des fichiers avec les bons masques de droits d'origine */
	/* id: 0666 (RW all) | jiffies: 0444 (R all) | foo: 0644 (RW root, R all) */
	debugfs_create_file("id", 0666, dir_fortytwo, NULL, &id_fops);
	debugfs_create_file("jiffies", 0444, dir_fortytwo, NULL, &jiffies_fops);
	debugfs_create_file("foo", 0644, dir_fortytwo, NULL, &foo_fops);

	pr_info("Hello world! Debugfs files registered.\n");
	return 0;
}

static void __exit debugfs_exit(void)
{
	/* Nettoie récursivement tout le dossier et les fichiers enfants */
	debugfs_remove_recursive(dir_fortytwo);
	kfree(foo_buffer);
	pr_info("Cleaning hello world! module.\n");
}

module_init(debugfs_init);
module_exit(debugfs_exit);
