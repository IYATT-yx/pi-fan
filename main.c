/**
 * @author IYATT-yx (iyatt@iyatt.com)
 * @brief 这是一个树莓派调速风扇驱动
 */
#include "module.h"
#include "wiringPi.h"
#include "parameter.h"
#include <stdio.h>

/**
 * @brief 风扇调速规则（综合温度与 CPU 占用率分档映射到起转电压 ~ 满电电压）
 * @param temperature 温度
 * @param usgae 使用率
 */
double get_volt(int temperature, int usgae)
{
    // 读取失败的情况
    if (-1 == temperature || -1 == usgae)
    {
        return -1;
    }

    int tempLevel = 0;
    if (temperature >= tempFull)
    {
        tempLevel = levelCount;
    }
    else if (temperature >= tempStart)
    {
        tempLevel = ((temperature - tempStart) * levelCount) / (tempFull - tempStart) + 1;
    }

    int usageLevel = 0;
    if (usgae >= usageFull)
    {
        usageLevel = levelCount;
    }
    else if (usgae >= usageStart)
    {
        usageLevel = ((usgae - usageStart) * levelCount) / (usageFull - usageStart) + 1;
    }

    int targetLevel = tempLevel > usageLevel ? tempLevel : usageLevel;
    if (targetLevel <= 0)
    {
        return 0.0;
    }
    if (targetLevel > levelCount)
    {
        targetLevel = levelCount;
    }

    if (levelCount <= 1)
    {
        return VOLT_MAX;
    }

    float stepVolt = (VOLT_MAX - startVolt) / (float)(levelCount - 1);
    float volt = startVolt + stepVolt * (float)(targetLevel - 1);

    if (volt > VOLT_MAX)
    {
        volt = VOLT_MAX;
    }
    else if (volt < VOLT_MIN)
    {
        volt = VOLT_MIN;
    }

    return volt;
}

int main()
{
    write_log("启动");
    init_pwm();

    static int temperature;
    static int usage;
    int isRunning = 0;
    double targetVolt = 0.0;

    while (1)
    {
        temperature = get_cpu_temperature();
        usage = get_cpu_usage();
        targetVolt = get_volt(temperature, usage);

        if (targetVolt < 0)
        {
            // 读取失败时全速散热保护
            set_pwm_by_volt(VOLT_MAX);
            isRunning = 1;
        }
        else if (targetVolt <= 0.0)
        {
            // 低于起转阈值时输出 0，彻底停转避免空耗电
            set_pwm_by_volt(0.0);
            isRunning = 0;
        }
        else
        {
            // 从停转转为运转时，先给一次满速脉冲帮助克服静摩擦力
            if (!isRunning)
            {
                set_pwm_by_volt(VOLT_MAX);
                delay(kickstartMs);
                isRunning = 1;
            }

            set_pwm_by_volt(targetVolt);
        }

        delay(DELAY - kickstartMs);
    }
}