from distutils.core import setup, Extension

module1 = Extension('embed',
                    sources = ['embed-high.c'])

setup (name = 'embed_c',
       version = '1.0',
       description = 'This is a demo package',
       ext_modules = [module1])
