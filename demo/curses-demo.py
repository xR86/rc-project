import curses

'''
stdscr = curses.initscr()
curses.noecho()
curses.cbreak()
stdscr.keypad(1)

sleep(10)
curses.nocbreak(); stdscr.keypad(0); curses.echo()
curses.endwin()
'''

curses.wrapper()

begin_x = 20; begin_y = 7
height = 5; width = 40
win = curses.newwin(height, width, begin_y, begin_x)

'''
import curses

myscreen = curses.initscr()
myscreen.border(0)
myscreen.addstr(12, 25, "Python curses in action!")
myscreen.refresh()
myscreen.getch()
'''

curses.endwin()
