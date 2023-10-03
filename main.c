/**
 * @author IYATT-yx (iyatt@iyatt.com)
 * @brief 这是一个树莓派调速风扇驱动
 */
#include "module.h"
#include "wiringPi.h"
#include "parameter.h"
#include <stdio.h>

/**
 * @brief 风扇调速规则
 * @param temperature 温度
 * @param usage 使用率
 */
double get_volt(int temperature, int usgae)
{
    static float volt1, volt2;

    // 读取失败的情况
    if (-1 == temperature || -1 == usgae)
    {
        return -1;
    }

    // 40 度起转，达到 67 度时全速
    volt1 = (float)(temperature - 40) * 0.1f + 0.6f;
    // 占用率 50 起转，达到 77 全速
    volt2 = (float)(usgae - 50) * 0.1f + 0.6f;

    if (volt1 > VOLT_MAX)
    {
        volt1 = VOLT_MAX;
    }
    else if (volt1 < VOLT_MIN)
    {
        volt1 = VOLT_MIN;
    }

    if (volt2 > VOLT_MAX)
    {
        volt2 = VOLT_MAX;
    }
    else if (volt2 < VOLT_MIN)
    {
        volt2 = VOLT_MIN;
    }

    return volt1 > volt2 ? volt1 : volt2;
}

int main()
{
    write_log("启动");
    init_pwm();
    
    static int temperature;
    static int usage;

    while (1)
    {
        temperature = get_cpu_temperature();
        usage = get_cpu_usage();

        set_pwm_by_volt(get_volt(temperature, usage));

        delay(3000);
    }
}