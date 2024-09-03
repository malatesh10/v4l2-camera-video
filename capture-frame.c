#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>

#define DEVICE_NAME "/dev/video0"
//#define DEVICE_NAME "/dev/video1"
#define WIDTH 640
#define HEIGHT 480
#define PIXEL_FORMAT V4L2_PIX_FMT_YUYV
#define BUFFER_LENGTH (WIDTH * HEIGHT * 2)

int main() {
    int fd = open(DEVICE_NAME, O_RDWR);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    struct v4l2_format format;
    memset(&format, 0, sizeof(format));
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.width = WIDTH;
    format.fmt.pix.height = HEIGHT;
    format.fmt.pix.pixelformat = PIXEL_FORMAT;
    if (ioctl(fd, VIDIOC_S_FMT, &format) == -1) {
        perror("ioctl");
     	return 1;
    }

    char* buffer = (char*)malloc(BUFFER_LENGTH);
    if (read(fd, buffer, BUFFER_LENGTH) == -1) {
        perror("read systemcall");
        return 1;
    }

    FILE* file = fopen("image.yuv", "w");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }
    fwrite(buffer, 1, BUFFER_LENGTH, file);
    fclose(file);

    free(buffer);
    close(fd);

    return 0;
}
