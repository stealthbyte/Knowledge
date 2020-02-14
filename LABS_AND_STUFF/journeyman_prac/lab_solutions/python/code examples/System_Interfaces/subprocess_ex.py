import subprocess
import time

subprocess.Popen('C:\\windows\\system32\\cmd.exe "/C dir C:\\ >> %TEMP%\\out.txt"')
cmd_list = ['C:\\windows\\system32\\cmd.exe', '/C', 'netstat', '-ano', '>>%TEMP%\\out.txt']
time.sleep(1)
subprocess.Popen(cmd_list)
time.sleep(1)
subprocess.Popen('netstat -ano >>%TEMP%\\out.txt', shell=True)

subprocess.Popen('notepad')