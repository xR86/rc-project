import sys
import os

# Browser class
from PyQt4.QtWebKit import QWebView
# from PyQt4.QtGui import QApplication
from PyQt4.QtCore import QUrl

# Window class
from PyQt4 import QtGui
from PyQt4.QtGui import QAction
from PyQt4.QtGui import QApplication
from PyQt4.QtGui import QIcon
from PyQt4.QtGui import QMainWindow
from PyQt4.QtGui import QVBoxLayout
from PyQt4.QtGui import QPushButton
from PyQt4.QtGui import QStyle



class Browser(QWebView):

	def __init__(self):
		QWebView.__init__(self)
		self.loadFinished.connect(self._result_available)

	def _result_available(self, ok):
		frame = self.page().mainFrame()
		#print unicode(frame.toHtml()).encode('utf-8')


class Window(QMainWindow):

	def __init__(self):
		super(Window, self).__init__()
		self.setGeometry(50, 50, 500, 300)
		self.setWindowTitle("GridAdmin")
		stdicon = self.style().standardIcon
		style = QStyle
		#reload_foo = QAction(, '&Reload', self)
		#SP_TitleBarMenuButton
		self.setWindowIcon(QIcon(stdicon(style.SP_ComputerIcon)))

		vbox = QVBoxLayout()

		extractAction = QAction('Quit', self)
		extractAction.setShortcut('Ctrl+Q')
		extractAction.setStatusTip('Leave app')
		extractAction.triggered.connect(self.close_application)

		self.statusBar()

		mainMenu = self.menuBar()
		fileMenu = mainMenu.addMenu('File')
		fileMenu.addAction(extractAction)

		# btn = QPushButton('Quit', self)
		# btn.clicked.connect(self.close_application)
		# btn.resize(btn.minimumSizeHint())
		# btn.move(0,100)

		# vbox.addWidget(btn)
		
		
		# TODO: not quite working here
		# view = Browser()

		# file_path = os.path.abspath(os.path.join(os.path.dirname(__file__), 'index.html'))
		# local_url = QUrl.fromLocalFile(file_path)
		# view.load(local_url)
		# view.show()
		
		view = QWebView()
		file_path = os.path.abspath(os.path.join(os.path.dirname(__file__), 'index.html'))
		local_url = QUrl.fromLocalFile(file_path)
		view.load(local_url)
		view.show()

		vbox.addWidget(view)
		
		self.show()
		

		self.home()

	def home(self):
		pass

	def close_application(self):
		print 'closing application'
		sys.exit()



if __name__ == '__main__':
	# app = QApplication(sys.argv)
	# GUI = Window()
	# sys.exit(app.exec_())

	
	app = QApplication(sys.argv)
	# button = QPushButton("Hello World", None)
	# button.show()

	view = Browser()
	view.resize(1200,600)
	# view.move(300, 300)
	view.setWindowTitle('GridAdmin')

	file_path = os.path.abspath(os.path.join(os.path.dirname(__file__), 'index.html'))
	local_url = QUrl.fromLocalFile(file_path)
	view.load(local_url)
	view.show()

	app.exec_()
	