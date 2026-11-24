/**
 * @author IYATT-yx (iyatt@iyatt.com)
 * @brief 树莓派调速风扇驱动的参数设置
 */
#ifndef PARAMETER_H
#define PARAMETER_H

#define PWM_PIN                 18                  // PWM 引脚
#define PWM_FREQ                25000                  // PWM 频率
#define PWM_RANGE               96                // PWM 范围 0-96
#define VOLT_MIN                0.0f                // PWM 最低电压
#define VOLT_MAX                3.3f                // PWM 最高电压
#define CLOCK_FREQ              19200000            // PWM 控制器时钟频率
#define ERROR_LOG_FILE_PATH     "/tmp/pi-fan.log"   // 日志文件保存路径
#define DELAY                   5000                // 风扇调速周期，单位毫秒

#define startVolt     1.7f   // 起转等效控制电压（基于 0~3.3V 映射，约 51% 占空比）
#define levelCount    5      // 起转电压到满电电压（VOLT_MAX）之间的分档数（1..levelCount）
#define tempStart     40     // 温度开始进入 1 档（℃）
#define tempFull      65     // 温度达到最高档（℃）
#define usageStart    50     // CPU 占用率开始进入 1 档（%）
#define usageFull     85     // CPU 占用率达到最高档（%）
#define kickstartMs   200    // 从停转首次启动时的全速冲刺时间（ms）

#endif