from distutils.core import setup, Extension

# the c++ extension module
extension_mod = Extension("process", ["process.cpp"])

setup(name = "process", ext_modules=[extension_mod])