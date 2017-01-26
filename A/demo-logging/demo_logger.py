# original source: https://stackoverflow.com/questions/14058453/making-python-loggers-output-all-messages-to-stdout-in-addition-to-log
import logging
from logging.handlers import RotatingFileHandler
import os
import sys
import time as tm

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

logging.info('hello')
logging.debug('hello')
