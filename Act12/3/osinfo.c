#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KRERK PIROMSOPA, PH.D. <Krerk.P@chula.ac.th>");
MODULE_DESCRIPTION("\"osinfo\" Character Device");

#define DEVICENAME "osinfo"
#define MAX_MINOR 2 // support 2 minor numbers: 0 and 1

static int dev_major;
static int dev_open[MAX_MINOR] = {0, 0};
static char *f_ptr[MAX_MINOR];

// Data for each minor number
static const char f_data0[] =
    "0:CP ENG CU OS 2022S2 - Instructors\n"
    "1:\tVeera Muangsin,Ph.D.\n"
    "2:\tKrerk Piromsopa,Ph.D.\n"
    "3:\tThongchai Rojkangsadan\n";

static const char f_data1[] =
    "0:CP ENG CU OS 2022S2 - Students Group Name: Kang OS\n"
    "1:\t6532035021 Chayanin Kongsareekul\n"
    "2:\t6532100021 Theepob Laopornpitchayanuwat\n"
    "3:\t6532141821 Poopha Suwananek\n";

// Function declarations
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);

static struct file_operations dev_fops = {
    .read = device_read,
    .open = device_open,
    .release = device_release,
};

int init_module(void)
{
    printk(KERN_INFO "CPCHAR: dev osinfo init\n");
    dev_major = register_chrdev(0, DEVICENAME, &dev_fops);
    if (dev_major < 0)
    {
        printk(KERN_ALERT "Fail register_chrdev osinfo with %d\n", dev_major);
        return dev_major;
    }
    printk(KERN_INFO "Device MajorNumber %d.\n", dev_major);
    printk(KERN_INFO "To create device files:\n");
    printk(KERN_INFO "\tmknod /dev/%s0 c %d 0\n", DEVICENAME, dev_major);
    printk(KERN_INFO "\tmknod /dev/%s1 c %d 1\n", DEVICENAME, dev_major);
    return 0;
}

void cleanup_module(void)
{
    printk(KERN_INFO "CPCHAR: dev osinfo cleanup\n");
    unregister_chrdev(dev_major, DEVICENAME);
}

static int device_open(struct inode *inode, struct file *file)
{
    int minor = MINOR(inode->i_rdev);
    if (minor >= MAX_MINOR)
        return -ENODEV;

    if (dev_open[minor])
        return -EBUSY;

    dev_open[minor]++;
    printk(KERN_INFO "osinfo: device_open (minor %d)\n", minor);

    // Assign appropriate data
    switch (minor)
    {
    case 0:
        f_ptr[minor] = (char *)f_data0;
        break;
    case 1:
        f_ptr[minor] = (char *)f_data1;
        break;
    default:
        return -ENODEV;
    }

    try_module_get(THIS_MODULE);
    file->private_data = (void *)(long)minor;
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    int minor = (int)(long)file->private_data;
    dev_open[minor]--;
    module_put(THIS_MODULE);
    return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
    int bytes_read = 0;
    int minor = (int)(long)filp->private_data;

    if (!f_ptr[minor] || *f_ptr[minor] == 0)
        return 0;

    while (length && *f_ptr[minor])
    {
        put_user(*(f_ptr[minor]++), buffer++);
        length--;
        bytes_read++;
    }
    return bytes_read;
}

// make
// sudo insmod osinfo.ko
// dmesg | tail  # Get major number

// # Assuming major number is 245
// sudo mknod /dev/osinfo0 c 245 0
// sudo mknod /dev/osinfo1 c 245 1

// cat /dev/osinfo0  # For instructors
// cat /dev/osinfo1  # For students
