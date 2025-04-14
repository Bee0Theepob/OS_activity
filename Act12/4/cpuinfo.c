#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <asm/processor.h> // for cpuid
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("CPUInfo Character Device");

#define DEV_NAME "cpuinfo"
#define BUF_LEN 512

static int dev_major;
static char device_buffer[BUF_LEN];
static int dev_open = 0;
static char *buf_ptr;

static void fill_cpuinfo_buffer(void)
{
    unsigned int eax, ebx, ecx, edx;
    char vendor[13];

    // Vendor ID
    eax = 0;
    native_cpuid(&eax, &ebx, &ecx, &edx);
    *(unsigned int *)&vendor[0] = ebx;
    *(unsigned int *)&vendor[4] = edx;
    *(unsigned int *)&vendor[8] = ecx;
    vendor[12] = '\0';

    // CPU features
    eax = 1;
    native_cpuid(&eax, &ebx, &ecx, &edx);
    int stepping = eax & 0xF;
    int model = (eax >> 4) & 0xF;
    int family = (eax >> 8) & 0xF;
    int proc_type = (eax >> 12) & 0x3;
    int ext_model = (eax >> 16) & 0xF;
    int ext_family = (eax >> 20) & 0xFF;

    // Serial number
    eax = 3;
    native_cpuid(&eax, &ebx, &ecx, &edx);

    snprintf(device_buffer, BUF_LEN,
             "Vendor ID: %s\n"
             "Stepping: %d\n"
             "Model: %d\n"
             "Family: %d\n"
             "Processor Type: %d\n"
             "Extended Model: %d\n"
             "Extended Family: %d\n"
             "Serial Number: 0x%08x%08x\n",
             vendor,
             stepping, model, family, proc_type, ext_model, ext_family,
             edx, ecx);
    buf_ptr = device_buffer;
}

// device operations
static int device_open(struct inode *inode, struct file *file)
{
    if (dev_open)
        return -EBUSY;

    dev_open++;
    fill_cpuinfo_buffer();
    try_module_get(THIS_MODULE);
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    dev_open--;
    module_put(THIS_MODULE);
    return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
    int bytes_read = 0;

    while (length && *buf_ptr)
    {
        put_user(*(buf_ptr++), buffer++);
        length--;
        bytes_read++;
    }

    return bytes_read;
}

static struct file_operations fops = {
    .read = device_read,
    .open = device_open,
    .release = device_release,
};

static int __init cpuinfo_init(void)
{
    dev_major = register_chrdev(0, DEV_NAME, &fops);
    if (dev_major < 0)
    {
        printk(KERN_ALERT "Registering char device failed with %d\n", dev_major);
        return dev_major;
    }

    printk(KERN_INFO "CPUInfo module loaded. Major number: %d\n", dev_major);
    printk(KERN_INFO "Create device with: mknod /dev/%s c %d 0\n", DEV_NAME, dev_major);
    return 0;
}

static void __exit cpuinfo_exit(void)
{
    unregister_chrdev(dev_major, DEV_NAME);
    printk(KERN_INFO "CPUInfo module unloaded.\n");
}

module_init(cpuinfo_init);
module_exit(cpuinfo_exit);

// make
// sudo insmod cpuinfo.ko
// dmesg | tail  # Get the major number
// sudo mknod /dev/cpuinfo c <major> 0
// cat /dev/cpuinfo