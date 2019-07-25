import time
import multiprocessing.shared_memory as shared_memory

data = 'abdqwbeujqehujqkahwsdjkwqgherjqwabndjkgqukwjehnkqwbadxukjqwghjkkbjdklbqwuikbjklhgeujkdqwbnadsxjkbquwkgedujkqwbsdjkxhqwuikhbedjkgbqwujkasgxjkqwbeukdsgq'

shm = shared_memory.SharedMemory(name='qwer', create=True, size=1000)
shm.close()
iter = 100000

start_time = time.time()

for i in range(0, iter):
    shm1 = shared_memory.SharedMemory(name='qwer')
    shm1.buf[:len(data)] = str.encode(data)
    val = bytes(shm1.buf[:len(data)])
    shm1.close()

print(time.time() - start_time)

shm.unlink()