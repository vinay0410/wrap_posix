#!/usr/bin/env python3
# encoding: utf-8

from distutils.core import setup, Extension

example_module = Extension(
    'mymodule',
    sources=['mymodule.cpp'],
    language='C++', )

setup(
    name='mymodule',
    version='0.1.0',
    description='example module written in C++',
    ext_modules=[example_module], )
