
import sys
from PySide.QtCore import *
from PySide.QtGui import *
from main_drmbuilder import MainDRMBuilder
from drm_manager import DRMManager

class Form(QDialog):
    def __init__(self, dat, parent=None):
        super(Form,self).__init__(parent)
        self.browser = MainDRMBuilder(dat)
        self.lineedit = QLineEdit("Type an expression and press Enter")
        self.lineedit.selectAll()
        layout = QVBoxLayout()
        layout.addWidget(self.lineedit)
        layout.addWidget(self.browser)
        self.setLayout(layout)

        self.lineedit.setFocus()
        self.connect(self.lineedit,SIGNAL("returnPressed()"),self.updateUI)
        self.setWindowTitle("Calculate")

    def updateUI(self):
        try:
            text = self.lineedit.text()
            QMessageBox.warning(self,"Calculate","You entered " + text)
            self.browser.textMessage.setText(text)
        except:
            QMessageBox.warning(self,"Calculate","Warning")


app = QApplication(sys.argv)
drm_m = DRMManager()
form = Form(drm_m)
form.show()
app.exec_()