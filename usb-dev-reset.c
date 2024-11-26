/* usbreset -- send a USB port reset to a USB device */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>

#include <linux/usbdevice_fs.h>

#define MSGBUFSZ 64
#define HELPTEXT "Usage: %s device-devpath\n\n\
EXAMPLE:\n\
$ lsusb\n\
...\n\
Bus 002 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub\n\
\n\
$ %s /dev/bus/usb/002/001\n\n"


int main(int argc, char **argv)
{
	const char *devpath;
	int fd;
	int rc;
	char msgbuf[MSGBUFSZ];

	if (argc != 2)
	{
		fprintf(stderr, HELPTEXT, argv[0], argv[0]);
		return 1;
	}

	devpath = argv[1];

	fd = open(devpath, O_WRONLY);
	if (fd < 0)
	{
		snprintf(msgbuf, MSGBUFSZ, "Error opening %s for writing", devpath);
		perror(msgbuf);
		return 1;
	}

	printf("Resetting USB device %s\n", devpath);
	rc = ioctl(fd, USBDEVFS_RESET, 0);
	if (rc < 0)
	{
		perror("Error performing ioctl (USBDEVFS_RESET)");
		close(fd);
		return 1;
	}

	printf("Reset successful\n");
	close(fd);
	return 0;
}
