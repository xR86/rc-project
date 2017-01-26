# PyQt GridAdmin client

# General dependencies (multiple)
import os
import sys

# Browser class
from PyQt4.QtWebKit import QWebView
# from PyQt4.QtGui import QApplication
# from PyQt4.QtCore import QUrl
from PyQt4 import QtCore

# Window class
from PyQt4 import QtGui
from PyQt4.QtGui import QAction
from PyQt4.QtGui import QApplication
from PyQt4.QtGui import QFileDialog
from PyQt4.QtGui import QIcon
from PyQt4.QtGui import QMainWindow
from PyQt4.QtGui import QMessageBox
from PyQt4.QtGui import QVBoxLayout
from PyQt4.QtGui import QPushButton
from PyQt4.QtGui import QStyle

# Connection
import socket
import json


# Logging
import logging
from logging.handlers import RotatingFileHandler
import time as tm



# Function to configure logging format
def configure_logging():
	log = logging.getLogger('')
	log.setLevel(logging.DEBUG)
	format = logging.Formatter("%(asctime)s |%(name)s| %(levelname)s | >> %(message)s")

	ch = logging.StreamHandler(sys.stdout)
	ch.setFormatter(format)
	log.addHandler(ch)

	if not os.path.exists('logs/'):
		os.makedirs('logs/')
	time = tm.strftime('%Y-%m-%d-%H_%M_%S', tm.localtime( tm.time() ))
	fh = RotatingFileHandler('logs/log-%s.log' % time, maxBytes=(1048576*5), backupCount=7)
	fh.setFormatter(format)
	log.addHandler(fh)


#
# Utility classes
# 
class BindObject(QtCore.QObject):
	"""Simple class with one slot and one read-only property."""

	hosts_json = {}

	@QtCore.pyqtSlot(str)
	def showMessage(self, msg):
		"""Open a message box and display the specified message."""
		QtGui.QMessageBox.information(None, "Info", msg)

	@QtCore.pyqtSlot(str)
	def connect(self, ip):
		#QtGui.QMessageBox.information(None, "Info", ip)
		s = socket.socket()         # Create a socket object
		# host = socket.gethostname() # Get local machine name
		# port = 12345                # Reserve a port for your service.

		if not self.hosts_json:
			QtGui.QMessageBox.information(None, "Info", "No hosts.json loaded")
			logging.debug('User tried to connect to server without hosts.json file.')
			return

		ip_flag = True
		host = []
		port = []
		for char in str(ip):
			if char == ':':
				ip_flag = False
				continue
			if ip_flag:
				host.append(char)
			else:
				port.append(char)
			#print char
		
		host = ''.join(host)
		port = int(''.join(port))
		logging.info('connecting to: ' + host + ':' + str(port)) 

		try:
			s.connect((host, port))
		except Exception as e:
			logging.debug(e)
		else:
			logging.info('connected to server')

		logging.info(self.hosts_json)
		logging.info('Sending a %i characters hosts.json file' % len(str(self.hosts_json)))
		s.send(str(self.hosts_json))

		logging.info("received message from server: %s" % s.recv(4))
		#s.close                     # Close the socket when done


	@QtCore.pyqtSlot()
	def get_files(self):
		dlg = QFileDialog(caption='Open a config file', directory='/home/xr86/rc-project/A')
		dlg.setFileMode(QFileDialog.AnyFile)
		dlg.setFilter("JSON files (*.json)")
		filenames = QtCore.QStringList()

		if dlg.exec_():
			filenames = dlg.selectedFiles()
			f = open(filenames[0], 'r')

			with f:
				data = f.read()
				#self.contents.setText(data)
				QMessageBox.information(None, "Info", data)
				self.hosts_json = json.loads(data)

	
	def _pyVersion(self):
		"""Return the Python version."""
		return sys.version

	"""Python interpreter version property."""
	pyVersion = QtCore.pyqtProperty(str, fget=_pyVersion)


#
# MAIN CLASS
#
class Browser(QWebView):

	def __init__(self):
		QWebView.__init__(self)
		self.loadFinished.connect(self._result_available)

	def _result_available(self, ok):
		frame = self.page().mainFrame()
		#print unicode(frame.toHtml()).encode('utf-8')


if __name__ == '__main__':
	configure_logging()

	app = QApplication(sys.argv)
	logging.info('initiated QApplication')
	
	data_object = BindObject()

	view = Browser()
	view.resize(1200,600)
	view.move(300, 300)
	view.setWindowTitle('GridAdmin')
	view.page().mainFrame().addToJavaScriptWindowObject("pyObj", data_object)

	stdicon = view.style().standardIcon
	style = QStyle
	
	#SP_TitleBarMenuButton
	view.setWindowIcon(QIcon(stdicon(style.SP_ComputerIcon)))

	file_path = os.path.abspath(os.path.join(os.path.dirname(__file__), 'index.html'))
	local_url = QtCore.QUrl.fromLocalFile(file_path)
	view.load(local_url)
	view.show()

	app.exec_()
	# sys.exit(app.exec_())