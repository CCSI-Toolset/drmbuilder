from distutils.core import setup
import py2exe
import matplotlib
setup(
	options = {
	'py2exe':{
		'includes': ['matplotlib.backends.backend_tkagg', 'FileDialog'],
		'dll_excludes': ['MSVCP90.dll']
		}
	},
	data_files = matplotlib.get_py2exe_datafiles(),
	console = ['main_drmbuilder.py']
)