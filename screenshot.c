#include <stdio.h>
#include <stdlib.h>
#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>
#include <libimobiledevice/screenshotr.h>

int main() {
    idevice_t device = NULL;
    lockdownd_service_descriptor_t service = NULL;
    screenshotr_client_t screenshot_client = NULL;

    // 连接到设备
    idevice_error_t ret = idevice_new(&device, NULL);
    if (ret != IDEVICE_E_SUCCESS) {
        fprintf(stderr, "No device found!\n");
        return -1;
    }

    // 启动 screenshotr 服务
    ret = screenshotr_client_start_service(device, &screenshot_client, NULL);
    if (ret != SCREENSHOTR_E_SUCCESS) {
        fprintf(stderr, "Failed to start screenshot service!\n");
        idevice_free(device);
        return -1;
    }

    // 获取截图
    char *imgdata = NULL;
    uint64_t imgsize = 0;
    ret = screenshotr_take_screenshot(screenshot_client, &imgdata, &imgsize);
    if (ret != SCREENSHOTR_E_SUCCESS) {
        fprintf(stderr, "Failed to take screenshot!\n");
        screenshotr_client_free(screenshot_client);
        idevice_free(device);
        return -1;
    }

    // 保存截图到文件
    const char *filename = "screenshot.tiff";
    FILE *file = fopen(filename, "wb");
    if (file) {
        fwrite(imgdata, 1, imgsize, file);
        fclose(file);
        printf("Screenshot saved to %s\n", filename);
    } else {
        fprintf(stderr, "Failed to save screenshot to file!\n");
    }

    // 释放资源
    free(imgdata);
    screenshotr_client_free(screenshot_client);
    idevice_free(device);
    return 0;
}
