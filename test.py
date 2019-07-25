import time
import process

data='abdqwbeujqehujqkahwsdjkwqgherjqwabndjkgqukwjehnkqwbadxukjqwghjkkbjdklbqwuikbjklhgeujkdqwbnadsxjkbquwkgedujkqwbsdjkxhqwuikhbedjkgbqwujkasgxjkqwbeukdsgq'

iter = 100000

start_time = time.time()

for i in range(0, iter):
    process.set("abcd", data)
    value = process.get("abcd")

print(time.time() - start_time)

process.clean("abcd")