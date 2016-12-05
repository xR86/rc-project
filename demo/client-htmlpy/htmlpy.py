import htmlPy
from back_end import BackEnd

app = htmlPy.AppGUI(
    title=u"Sample application",
    maximized = True)
#app.maximized = True

app.template_path = "."
app.template = ("index.html", {"username": "htmlPy_user"})
app.bind(BackEnd(app))

#app.template = ("index.html", {})

if __name__ == "__main__":
    app.start()
