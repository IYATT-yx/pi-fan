/**
 * @author IYATT-yx (iyatt@iyatt.com)
 * @brief 树莓派调速风扇驱动组件模块函数声明
 */
#ifndef MODULE_H
#define MODULE_H

/**
 * @brief 保存日志
 * @param info 待保存的内容
 */
void write_log(const char* info);

/**
 * @brief 根据统计信息计算 CPU 使用率
 * @return CPU 使用率整数结果
 */
int get_cpu_usage();

/**
 * @brief 获取 CPU 温度
 * @return CPU 温度值整数结果
 */
int get_cpu_temperature();

/**
 * @brief 初始化 PWM
 */
void init_pwm();

/**
 * @brief PWM 的平均电压值
 * @param volt 电压值
 */
void set_pwm_by_volt(double volt);

#endif