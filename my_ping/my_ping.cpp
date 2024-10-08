/* 2253731 计科 张皓钦 */
#include <iostream>
//可根据需要再加入其它需要的头文件
#include <cstdlib>
#include "../include/class_aat.h"
using namespace std;

enum OPT_ARGS {
	ARGS_HELP = 0,
	ARGS_T,
	ARGS_N,
	ARGS_L
};

/* 允许自行添加其它函数，也可以将自定义函数放在其它cpp中 */

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：想一想，为什么要加 static（虽然只有一个cpp，但这是工程习惯）
***************************************************************************/
static void usage(const char* const fullpath_procname)
{
	/* 这三行看得懂吗？*/
	const char* procname = strrchr(fullpath_procname, '\\');
	if (procname == NULL)
		procname = fullpath_procname;

	/* 注：这部分比14-b4的要求低，因为没有做到按需适应 */
	cout << endl;
	cout << "Usage: " << procname << " [--help] | [--t] [--n count] [--l size] IPaddress" << endl;
	cout << endl;
	cout << "选项:" << endl;
	cout << "    --help\t\t给出帮助信息" << endl;
	cout << "    --t\t\t\t持续Ping指定主机，直到按Ctrl+C停止" << endl;
	cout << "    --n count\t\t指定要发送的包数量[1..1024]，默认4" << endl;
	cout << "    --l size\t\t指定发送缓冲区的大小[32..64000]，默认64" << endl;
	cout << endl;
	cout << "示例:" << endl;
	cout << "    " << procname << " --help" << endl;
	cout << "    " << procname << " 192.168.80.230" << endl;
	cout << "    " << procname << " --t 192.168.80.230" << endl;
	cout << "    " << procname << " --l 2000 192.168.80.230" << endl;
	cout << "    " << procname << " --n 200 --l 1000 192.168.80.230" << endl;
	cout << "    " << procname << " --l 1000 --n 200 192.168.80.230" << endl;
	cout << "    " << procname << " --l 1000 --t 192.168.80.230" << endl;
	cout << "    " << procname << " --t --l 1000 --n 20 192.168.80.230" << endl;
	cout << endl;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：想一想，为什么要加 static（虽然只有一个cpp，但这是工程习惯）
***************************************************************************/
static bool is_ipaddr_valid(const char* const ipstr)
{
	//3个点，4个0-255的整数，两点间必须有数字，两头必须有数字，数字可以有前导0，但值要是0-255
	int point[5] = { -1,0, 0, 0,(int)strlen(ipstr) }, pointCnt = 0;//这里虚设两个点，方便后面的循环
	for (int i = 0; ipstr[i]; i++)
		if (ipstr[i] == '.') {
			if (pointCnt == 3)//如果已经有3个点了，就不合法
				return false;
			else
				point[++pointCnt] = i;
		}
	if (pointCnt != 3)
		return false;
	//检查每个点之间的数字
	for (int i = 0; i <= 3; i++) {
		if (point[i + 1] - point[i] <= 1)//如果两点之间没有数字
			return false;
		for (const char* p = ipstr + point[i] + 1; p != ipstr + point[i + 1]; p++)
			if (*p < '0' || *p>'9')
				return false;
		if (atoi(ipstr + point[i] + 1) > 255)//如果数字不在0-255之间，其中负号已经过不了前面那关，所以只需要考虑255
			return false;
	}
	return true;
}

/***************************************************************************
  函数名称：
  功    能：基本测试
  输入参数：
  返 回 值：
  说    明：命令行格式要求：先--开始的可变参数，顺序任意，最后是指定的固定参数
***************************************************************************/
int main(int argc, char* argv[])
{
	/* 因为引入了 lib_common_tools.lib，此处先检查大小是否符合要求 */
	if (sizeof(args_analyse_tools) != 200) {
		cout << "class args_analyse_tools 不是200字节，如果使用 lib_common_tools.lib 中的函数则可能出错" << endl;
		return -1;
	}

	args_analyse_tools args[] = {
		args_analyse_tools("--help", ST_EXTARGS_TYPE::boolean,          0, false), //默认false
		args_analyse_tools("--t",    ST_EXTARGS_TYPE::boolean,          0, false), //默认false
		args_analyse_tools("--n",    ST_EXTARGS_TYPE::int_with_default, 1, 4, 1, 1024), //[1..1024]，默认4
		args_analyse_tools("--l",    ST_EXTARGS_TYPE::int_with_default, 1, 64, 32, 64000), //[32..64000]，默认64
		args_analyse_tools()  //最后一个，用于结束
	};

	/* 不带任何参数做特殊处理，打印提示信息后结束 */
	if (argc == 1) {
		usage(argv[0]);
		return -1;
	}

	int need_fixed_args = 1;	//需要固定参数
	int cur_argc;

	/* 最后一个参数：1 - 表示除选项参数外，还需要其它参数
					  例1：Windows/Linux下的ping命令，ping IP地址是最基本的命令格式，另外还有很多-/--开头的可选参数
						  ping 192.168.80.230
						  ping -t -n 10 192.168.80.230
					 例2：Linux下的c++命令，c++ 源程序文件名是最基本的命令格式，另外还有很多-/--开头的可选参数
						  c++ test.cpp
						  c++ -Wall -o test test.cpp
				  0 - 表示除选项参数外，不需要其它参数
		函数返回值：-1 - 处理过程有错
		            >0 - 可变参数处理完成后，当前停在argv[]的第几个下标位置，如果后面还有argv[]，则表示固定参数
	*/
	if ((cur_argc = args_analyse_process(argc, argv, args, need_fixed_args)) < 0)
	{
		//错误信息在函数中已打印
		return -1;
	}

	/* 带--help参数做特殊处理，打印提示信息后结束 */
	if (args[ARGS_HELP].existed()) {
		usage(argv[0]);
		return -1;
	}

	/* 可变参数分析完成后，看是否还有固定参数，以及固定参数的数量是否符合要求 */
	if (need_fixed_args) {
		if (cur_argc == argc) {  //说明后面没有跟需要的固定参数
			cout << "需要指定IP地址，但未给出" << endl;
			return -1;
		}
		if (argc - cur_argc > 1) {
			cout << "除可变参数外，还给出了" << argc - cur_argc << "个固定参数，但本程序只需要一个固定参数" << endl;
			return -1;
		}
	}

	/* 检查IP地址是否是点分十进制的函数，需要实现 */
	if (is_ipaddr_valid(argv[cur_argc]) == false) {
		cout << "指定的IP地址 [" << argv[cur_argc] << "] 不正确." << endl;
		return -1;
	}

	/* 打印参数分析的结果 */
	cout << "可变参数列表:" << endl;
	args_analyse_print(args);

	/* 打印参数的值 */
	cout << "参数检查通过" << endl;
	cout << " --t参数：" << args[ARGS_L].existed() << endl;
	cout << " --n参数：" << args[ARGS_N].get_int() << endl;
	cout << " --l参数：" << args[ARGS_L].get_int() << endl;
	cout << "IP地址是：" << argv[cur_argc] << endl;

	return 0;
}
