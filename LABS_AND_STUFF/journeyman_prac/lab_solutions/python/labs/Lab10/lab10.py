import os
import time
import subprocess
pslist = "C:\\wintools\\pslist.exe"
pskill = "C:\\wintools\\pskill.exe"


def parse_pslist(stdout):
    process_dict = {}
    lines = stdout.split(b'\n')
    name_found = False
    for line in lines:
        # skip empty lines
        if not line.strip():
            continue
        line_split = line.split()
        if line_split[0] == b'Name' and not name_found:
            name_found = True
            continue

        # skipping the header section
        if not name_found:
            continue

        name = line_split[0]
        pid = line_split[1]

        if name.decode() in process_dict:
            process_dict[name.decode()].append(pid.decode())
        else:
            process_dict[name.decode()] = [pid.decode()]

        print('{0:30}{1}'.format(name.decode("utf-8"), pid.decode("utf-8")))

    return process_dict


# Use subprocess to start and kill notepad
proc = subprocess.Popen('notepad')
time.sleep(2)
proc.terminate()


# Use subprocess to start an instance of pslist.exe, pipe the output to a file,
# and display the pids and process names of all the running processes
out_file = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'out1.txt')
with open(out_file, 'wb') as fh:
    proc = subprocess.Popen(
        pslist,
        stdout=fh,
        stderr=fh
    )
# alternate method for piping output of a Popen call to a file
# proc = subprocess.Popen([pslist, '>%s' % out_file],
#                        shell = True,
#                        stdout = subprocess.PIPE,
#                        stderr = subprocess.PIPE)

# time.sleep(1)
proc.wait()
stdout = open(out_file, 'rb').read()
parse_pslist(stdout)


# Do the above command without the intermediate file; overwrite the stdout
# file handle when you execute it
proc = subprocess.Popen(
    pslist,
    stdout=subprocess.PIPE
)
stdout, _ = proc.communicate()
parse_pslist(stdout)


# Write a function that takes as input a PID, and use the subprocess module
# and pskill.exe to kill the process with the given ID
def kill_pid(pid):
    proc = subprocess.Popen(
        [pskill, str(pid)],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )


proc = subprocess.Popen('notepad')
time.sleep(2)
kill_pid(proc.pid)


# Write a function that takes as input a process name and use the subprocess
# module and pskill.exe to kill the process(es) with the given name
def kill_process(process_name):
    proc = subprocess.Popen(
        pslist,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )
    stdout, _ = proc.communicate()
    process_dict = parse_pslist(stdout)
    if process_name in process_dict:
        for pid in process_dict[process_name]:
            kill_pid(pid)
    else:
        raise Exception('%s is not an active process' % process_name)


proc1 = subprocess.Popen('notepad')
proc2 = subprocess.Popen('notepad')
time.sleep(2)
kill_process('notepad')
