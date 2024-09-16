#!/usr/bin/env bash

root_path=$(pwd)

# 安装构建工具
function install_build_tool()
{
	sudo apt update
	sudo apt install -y git build-essential
}

# 安装 WiringPi
function install_wiringpi()
{
	cd /tmp || exit 1
	rm -rf WiringPi
	git clone https://github.com/WiringPi/WiringPi.git --depth=1 --branch=3.10
	cd WiringPi || exit 1

	cat <<'PATCH' | patch --ignore-whitespace -p1 wiringPi/wiringPi.c
	@@ -747,6 +747,7 @@ int piGpioLayout (void)
	if ((cpuFd = fopen ("/proc/cpuinfo", "r")) == NULL)
		piGpioLayoutOops ("Unable to open /proc/cpuinfo") ;

	+#ifdef DONT_CARE_ANYMORE
	// Start by looking for the Architecture to make sure we're really running
	// on a Pi. I'm getting fed-up with people whinging at me because
	// they can't get it to work on weirdFruitPi boards...
	@@ -769,7 +770,6 @@ int piGpioLayout (void)
	// I do not support so don't email me your bleating whinges about anything
	// other than a genuine Raspberry Pi.

	-#ifdef DONT_CARE_ANYMORE
	if (! (strstr (line, "BCM2708") || strstr (line, "BCM2709") || strstr (line, "BCM2835")))
	{
		fprintf (stderr, "Unable to determine hardware version. I see: %s,\n", line) ;
PATCH

	bash INSTALL
}

# 构建风扇驱动
function build()
{
	cd "$root_path" || exit 1
	mkdir build 2> /dev/null
	make clean
	make build
}

# 安装
function install()
{
	sudo cp build/pi-fan /usr/local/bin/pi-fan
	sudo cp pi-fan.service /etc/systemd/system/pi-fan.service
	sudo systemctl enable pi-fan.service
	sudo systemctl start pi-fan.service
	systemctl status pi-fan.service
}

# 检查命令工具是否安装
function check_command()
{
	command -v "$1" >/dev/null 2>&1
}

# 检查进程是否在运行
function check_process()
{
	pgrep "$1" >/dev/null 2>&1
}

function main
{
	# 检查是否安装构建工具
	commands=("git" "make" "gcc")
	for cmd in "${commands[@]}"
	do
		if ! check_command "$cmd"
		then
			install_build_tool
		fi
	done

	# 检查是否安装 WiringPi
	if ! check_command "gpio"
	then
		install_wiringpi
	fi

	# 构建
	build

	# 有正在运行的实例就关闭
	if check_process "pi-fan"
	then
		sudo systemctl stop pi-fan.service
	fi

	# 安装
	install
}

if [ "$0" == "${BASH_SOURCE[*]}" ]
then
	main
fi
