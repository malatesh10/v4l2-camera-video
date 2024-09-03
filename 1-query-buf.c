#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int fd = open("/dev/video0", O_RDWR);  // Open video device
    if (fd < 0) {
        perror("open");
        return 1;
    }

    struct v4l2_buffer buf;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    
    // Query buffer 0
    buf.index = 0;
    if (ioctl(fd, VIDIOC_QUERYBUF, &buf) < 0) {
        perror("VIDIOC_QUERYBUF");
        close(fd);
        return 1;
    }

    printf("Buffer length: %u\n", buf.length);
    printf("Buffer offset: %u\n", buf.m.offset);

    close(fd);
    return 0;
}
