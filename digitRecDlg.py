from PySide2.QtUiTools import QUiLoader
from PySide2.QtWidgets import QApplication, QMessageBox
from functools import partial


import slots


DEBUG = True


class digitsRec():
    def __init__(self) -> None:
        if DEBUG:
            import os
            path = os.path.realpath(os.curdir)
            self.ui = QUiLoader().load(path+'/digit-rec/digits-rec.ui')
        else:
            self.ui = QUiLoader().load('./digits-rec.ui')

        # signal -> slots
        self.ui.file_browse_button.clicked.connect(
            partial(slots.file_browse_clicked, self.ui))
        self.ui.show_image_button.clicked.connect(
            partial(slots.show_image_button_clicked, self.ui))
        self.ui.run_button.clicked.connect(
            partial(slots.run_button_clicked, self.ui))
        # end


app = QApplication([])
digits_rec = digitsRec()
digits_rec.ui.show()
app.exec_()
