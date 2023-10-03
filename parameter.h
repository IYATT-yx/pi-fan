/**
 * @author IYATT-yx (iyatt@iyatt.com)
 * @brief 树莓派调速风扇驱动的参数设置
 */
#ifndef PARAMETER_H
#define PARAMETER_H

#define PWM_PIN                 18                  // PWM 引脚
#define PWM_FREQ                20                  // PWM 频率
#define PWM_RANGE               1023                // PWM 范围 0-1023
#define VOLT_MIN                0.0f                // PWM 最低电压
#define VOLT_MAX                3.3f                // PWM 最高电压
#define CLOCK_FREQ              19200000            // PWM 控制器时钟频率
#define ERROR_LOG_FILE_PATH     "/tmp/pi-fan.log"   // 日志文件保存路径
#define DELAY                   5000                // 风扇调速周期，单位毫秒

#endif