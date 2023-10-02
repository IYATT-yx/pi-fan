/**
 * @author IYATT-yx (iyatt@iyatt.com)
 * @brief 树莓派调速风扇驱动组件模块函数定义
 */
#include "module.h"
#include "parameter.h"
#include "wiringPi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// 统计信息文件
const char stat_file_path[] = "/proc/stat";
// 错误日志文件
const char error_log_file_path[] = "/var/log/pi-fan-log.log";
// 温度文件
const char temperature_file[] = "/sys/class/thermal/thermal_zone0/temp";

void write_log(const char* info)
{
    FILE* file = fopen(error_log_file_path, "a");
    if (NULL == file)
    {
        file = fopen(error_log_file_path, "w");
        if (NULL == file)
        {
            perror("写错误日志失败");
            exit(9);
        }
    }

    time_t current_time = time(NULL);
    struct tm* time_info = localtime(&current_time);
    char time_stamp[20];
    memset(time_stamp, 0, sizeof(time_stamp));
    strftime(time_stamp, sizeof(time_stamp), "%Y-%m-%d %H:%M:%S", time_info);
    fprintf(file, "【%s】 %s\n", time_stamp, info);
    fclose(file);
}

int get_cpu_usage()
{
    FILE* file = fopen(stat_file_path, "r");
    if (NULL == file)
    {
        write_log("打开 CPU 统计信息文件失败");
        return -1;
    }

    char buffer[64];
    if (NULL == fgets(buffer, sizeof(buffer), file))
    {
        fclose(file);
        write_log("读取 CPU 统计文件失败");
        return -1;
    }

    unsigned long user, nice, sys, idle, iowait, irq, softirq;
    static unsigned long pre_total = 0;
    static unsigned long pre_idle = 0;

    sscanf(buffer, "%*s %lu %lu %lu %lu %lu %lu %lu", &user, &nice, &sys, &idle, &iowait, &irq, &softirq);
    unsigned long total = user + nice + sys + idle + iowait + irq + softirq;

    int usage = (int)(((double)(total - pre_total - (idle - pre_idle)) / (double)(total - pre_total)) * 100);

    pre_total = total;
    pre_idle = idle;

    return usage;
}

int get_cpu_temperature()
{
    FILE* file = fopen(temperature_file, "r");
    if (NULL == file)
    {
        write_log("打开温度文件失败");
        return -1;
    }

    char buffer[6];
    if (NULL == fgets(buffer, sizeof(buffer), file))
    {
        fclose(file);
        write_log("读取温度文件失败");
        return -1;
    }

    fclose(file);

    long temperature = strtol(buffer, NULL, 10);

    return (int)(temperature / 1000);
}

void init_pwm()
{
    wiringPiSetupGpio();
    pinMode(PWM_PIN, PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS);
    pwmSetClock(CLOCK_FREQ / PWM_FREQ / PWM_RANGE);
    pwmSetRange(PWM_RANGE);
}

void set_pwm_by_volt(double volt)
{
    if (volt < VOLT_MIN || volt > VOLT_MAX)
    {
        write_log("设置电压范围错误");
        return;
    }

    int duty = (int)((volt - VOLT_MIN) / (VOLT_MAX - VOLT_MIN) * PWM_RANGE);
    pwmWrite(PWM_PIN, duty);
}