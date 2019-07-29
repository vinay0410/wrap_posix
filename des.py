import mymodule
import sys

foo = mymodule.Foo()

'''
print('ref')
print(sys.getrefcount(foo))

print('here')
'''
print(foo.get())

'''
print('ref')
print(sys.getrefcount(foo))

#print(foo)

#del foo
'''

print("ref")
print(sys.getrefcount(foo))