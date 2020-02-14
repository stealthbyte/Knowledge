import threading
import time

global_counter = 0
global_lock = threading.Lock()

def time_f(function, *args, **kwargs):
    start_t = time.time()
    function(*args, **kwargs)
    end_t = time.time()

    print('%s(*(%r), **(%r))' % (function.__name__, args, kwargs))
    print('took %-5.2f seconds' % (end_t - start_t))


def inc_counter(times=100, lock=False):
    global global_counter
    for i in range(times):
        if lock: # the 'right' way
            global_lock.acquire()
            global_counter += 1
            global_lock.release()
        else: # the wrong way
            global_counter += 1


def run_threads(num_threads=10, *args, **kwargs):
    global global_counter
    global_counter = 0
    threads = [
                threading.Thread(target=inc_counter, args=args, kwargs=kwargs) for i in range(num_threads)
              ]
    for thread in threads: # start all threads
        thread.start()
    for thread in threads: # wait for all threads to finish
        thread.join()


def main(num_threads=10, runs_per_thread=1000, lock=False):
    global global_counter
    time_f(run_threads, num_threads, times=runs_per_thread, lock=lock)

    print(global_counter, 'should be', num_threads*runs_per_thread)