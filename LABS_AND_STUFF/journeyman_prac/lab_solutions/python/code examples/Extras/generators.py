import os
import re
import sys

def grep(look_for, in_dir, noisy=False):
    look_for_re = re.compile(look_for)
    for path, dirs, files in os.walk(in_dir):
        for fname in files:
            fpath = os.path.join(path, fname)
            try:
                for i, line in enumerate(file(fpath)):
                    if look_for_re.match(line):
                        yield fpath, i, line
            except:
                if noisy:
                    print(sys.exc_info()[1]) # the exception


def main():
    for fname, line_num, line in grep('simple', 'C:/Users/user'):
        print('%-20s,%d: %r' % (fname, line_num, line))