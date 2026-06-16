// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("milin");
MODULE_DESCRIPTION("Assignment 05: Misc Character Device Driver");
MODULE_VERSION("0.1");

#define LOGIN_STR "milin\n"
#define LOGIN_LEN 6 /* "milin\n" contient 6 caractères */

/*
 * Opération de lecture (read)
 * Renvoie le login étudiant à l'espace utilisateur.
 */
static ssize_t fortytwo_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	/* Si l'utilisateur a déjà tout lu, on renvoie 0 (EOF) */
	if (*ppos >= LOGIN_LEN)
		return 0;

	/* On ajuste la taille si l'utilisateur demande moins que ce qu'il reste */
	if (count > LOGIN_LEN - *ppos)
		count = LOGIN_LEN - *ppos;

	/* Sécurité : transfert de l'espace noyau vers l'espace utilisateur */
	if (copy_to_user(buf, LOGIN_STR + *ppos, count))
		return -EFAULT;

	*ppos += count;
	return count;
}

/*
 * Opération d'écriture (write)
 * Compare l'entrée utilisateur avec le login étudiant.
 */
static ssize_t fortytwo_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	char kbuf[32];

	/* Protection contre les dépassements de tampon */
	if (count >= sizeof(kbuf))
		return -EINVAL;

	/* Sécurité : transfert de l'espace utilisateur vers l'espace noyau */
	if (copy_from_user(kbuf, buf, count))
		return -EFAULT;

	kbuf[count] = '\0'; /* On s'assure que la chaîne se termine proprement */

	/* Comparaison de l'entrée avec notre login attendu */
	if (count == LOGIN_LEN && memcmp(kbuf, LOGIN_STR, LOGIN_LEN) == 0)
		return count; /* Succès ! */

	/* Si ça ne correspond pas, on renvoie une erreur d'argument invalide */
	return -EINVAL;
}

/* Liaison des opérations de fichiers prises en charge */
static const struct file_operations fortytwo_fops = {
	.owner   = THIS_MODULE,
	.read    = fortytwo_read,
	.write   = fortytwo_write,
};

/* Configuration du périphérique "misc" */
static struct miscdevice fortytwo_miscdev = {
	.minor = MISC_DYNAMIC_MINOR, /* Numéro minor alloué dynamiquement */
	.name  = "fortytwo",         /* Créera automatiquement /dev/fortytwo */
	.fops  = &fortytwo_fops,
};

static int __init hello_init(void)
{
	int ret;

	/* Enregistrement du périphérique misc */
	ret = misc_register(&fortytwo_miscdev);
	if (ret) {
		pr_err("Failed to register misc device /dev/fortytwo\n");
		return ret;
	}

	pr_info("Hello world! /dev/fortytwo registered successfully.\n");
	return 0;
}

static void __exit hello_exit(void)
{
	/* Désenregistrement du périphérique misc */
	misc_deregister(&fortytwo_miscdev);
	pr_info("Cleaning hello world! module.\n");
}

module_init(hello_init);
module_exit(hello_exit);
