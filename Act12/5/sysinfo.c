#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("SysInfo Character Device");

#define DEV_NAME "cp-sysinfo"
#define BUF_LEN 1024

static int dev_major;
static char device_buffer[BUF_LEN];
static int dev_open = 0;
static char *buf_ptr;

// Helper function to get active processes
static void get_processes_info(void)
{
    struct task_struct *task;
    char buff[BUF_LEN];
    int len = 0;

    for_each_process(task)
    {
        len += snprintf(buff + len, BUF_LEN - len, "%d,%s\n", task->pid, task->comm);
        if (len >= BUF_LEN)
            break;
    }

    // Copy the data to the device buffer
    snprintf(device_buffer, BUF_LEN, "%s", buff);
    buf_ptr = device_buffer;
}

// Helper function to get memory information
static void get_memory_info(void)
{
    struct sysinfo info;
    si_meminfo(&info);

    snprintf(device_buffer, BUF_LEN,
             "MemTotal: %lu kB\n"
             "MemFree: %lu kB\n"
             "MemAvailable: %lu kB\n",
             info.totalram * 4, info.freeram * 4, info.bufferram * 4);
    buf_ptr = device_buffer;
}

// Device operations
static int device_open(struct inode *inode, struct file *file)
{
    if (dev_open)
        return -EBUSY;

    dev_open++;
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
    struct inode *inode = filp->f_path.dentry->d_inode; // Get the inode from the file pointer

    // Depending on the minor number, get either process or memory info
    if (MINOR(inode->i_rdev) == 0)
    {
        // Minor 0: Get process info
        get_processes_info();
    }
    else if (MINOR(inode->i_rdev) == 1)
    {
        // Minor 1: Get memory info
        get_memory_info();
    }

    // Check if we reached the end of the buffer
    if (*buf_ptr == 0)
        return 0;

    // Copy data from kernel space to user space
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

// Init function for the kernel module
static int __init sysinfo_init(void)
{
    dev_major = register_chrdev(0, DEV_NAME, &fops);
    if (dev_major < 0)
    {
        printk(KERN_ALERT "Registering char device failed with %d\n", dev_major);
        return dev_major;
    }

    printk(KERN_INFO "SysInfo module loaded. Major number: %d\n", dev_major);
    printk(KERN_INFO "Create device with: mknod /dev/cp-psinfo c %d 0 for processes\n", dev_major);
    printk(KERN_INFO "Create device with: mknod /dev/cp-meminfo c %d 1 for memory info\n", dev_major);
    return 0;
}

// Exit function for the kernel module
static void __exit sysinfo_exit(void)
{
    unregister_chrdev(dev_major, DEV_NAME);
    printk(KERN_INFO "SysInfo module unloaded.\n");
}

module_init(sysinfo_init);
module_exit(sysinfo_exit);
