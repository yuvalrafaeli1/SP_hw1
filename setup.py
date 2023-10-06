from setuptools import Extension, setup

module = Extension("symnmfM", sources=['symnmfmodule.c','symnmf.c'])
setup(name='symnmfM',
     version='1.0',
     description='Python wrapper for custom C extension',
     ext_modules=[module])
