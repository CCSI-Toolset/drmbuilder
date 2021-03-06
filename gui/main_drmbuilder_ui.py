# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'main_drmbuilder.ui'
#
# Created: Fri May 22 11:09:31 2015
#      by: pyside-uic 0.2.15 running on PySide 1.2.2
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(886, 528)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(":/icons/ccsi_logo.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        MainWindow.setWindowIcon(icon)
        self.centralwidget = QtGui.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.verticalLayout = QtGui.QVBoxLayout(self.centralwidget)
        self.verticalLayout.setObjectName("verticalLayout")
        self.widgetDisplay = QtGui.QWidget(self.centralwidget)
        self.widgetDisplay.setMinimumSize(QtCore.QSize(0, 200))
        self.widgetDisplay.setObjectName("widgetDisplay")
        self.verticalLayout.addWidget(self.widgetDisplay)
        self.textMessage = QtGui.QTextBrowser(self.centralwidget)
        self.textMessage.setObjectName("textMessage")
        self.verticalLayout.addWidget(self.textMessage)
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 886, 26))
        self.menubar.setObjectName("menubar")
        self.menuFile = QtGui.QMenu(self.menubar)
        self.menuFile.setObjectName("menuFile")
        self.menuExport = QtGui.QMenu(self.menuFile)
        self.menuExport.setObjectName("menuExport")
        self.menuView = QtGui.QMenu(self.menubar)
        self.menuView.setObjectName("menuView")
        self.menuSetup = QtGui.QMenu(self.menubar)
        self.menuSetup.setObjectName("menuSetup")
        self.menuBuild = QtGui.QMenu(self.menubar)
        self.menuBuild.setObjectName("menuBuild")
        self.menuD_RM_Model_Type = QtGui.QMenu(self.menuBuild)
        self.menuD_RM_Model_Type.setObjectName("menuD_RM_Model_Type")
        self.menuPost_Process = QtGui.QMenu(self.menubar)
        self.menuPost_Process.setObjectName("menuPost_Process")
        self.menuUQ = QtGui.QMenu(self.menubar)
        self.menuUQ.setObjectName("menuUQ")
        self.menuHelp = QtGui.QMenu(self.menubar)
        self.menuHelp.setObjectName("menuHelp")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtGui.QStatusBar(MainWindow)
        self.statusbar.setInputMethodHints(QtCore.Qt.ImhUppercaseOnly)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)
        self.toolBar = QtGui.QToolBar(MainWindow)
        self.toolBar.setObjectName("toolBar")
        MainWindow.addToolBar(QtCore.Qt.TopToolBarArea, self.toolBar)
        self.actionAbout_D_RM_Builder = QtGui.QAction(MainWindow)
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(":/icons/help.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionAbout_D_RM_Builder.setIcon(icon1)
        self.actionAbout_D_RM_Builder.setObjectName("actionAbout_D_RM_Builder")
        self.actionNew = QtGui.QAction(MainWindow)
        icon2 = QtGui.QIcon()
        icon2.addPixmap(QtGui.QPixmap(":/icons/new.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionNew.setIcon(icon2)
        self.actionNew.setObjectName("actionNew")
        self.actionOpen = QtGui.QAction(MainWindow)
        icon3 = QtGui.QIcon()
        icon3.addPixmap(QtGui.QPixmap(":/icons/open.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionOpen.setIcon(icon3)
        self.actionOpen.setObjectName("actionOpen")
        self.actionClose = QtGui.QAction(MainWindow)
        icon4 = QtGui.QIcon()
        icon4.addPixmap(QtGui.QPixmap(":/icons/close.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionClose.setIcon(icon4)
        self.actionClose.setObjectName("actionClose")
        self.actionSave = QtGui.QAction(MainWindow)
        icon5 = QtGui.QIcon()
        icon5.addPixmap(QtGui.QPixmap(":/icons/save.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionSave.setIcon(icon5)
        self.actionSave.setObjectName("actionSave")
        self.actionSave_As = QtGui.QAction(MainWindow)
        self.actionSave_As.setObjectName("actionSave_As")
        self.actionD_RM_As_Matlab_Script_File = QtGui.QAction(MainWindow)
        icon6 = QtGui.QIcon()
        icon6.addPixmap(QtGui.QPixmap(":/icons/export_drm.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionD_RM_As_Matlab_Script_File.setIcon(icon6)
        self.actionD_RM_As_Matlab_Script_File.setObjectName("actionD_RM_As_Matlab_Script_File")
        self.actionToolbar = QtGui.QAction(MainWindow)
        self.actionToolbar.setCheckable(True)
        self.actionToolbar.setChecked(True)
        self.actionToolbar.setObjectName("actionToolbar")
        self.actionStatus_Bar = QtGui.QAction(MainWindow)
        self.actionStatus_Bar.setCheckable(True)
        self.actionStatus_Bar.setChecked(True)
        self.actionStatus_Bar.setObjectName("actionStatus_Bar")
        self.actionChoose_High_Fidelity_Model = QtGui.QAction(MainWindow)
        icon7 = QtGui.QIcon()
        icon7.addPixmap(QtGui.QPixmap(":/icons/aspen_tech.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionChoose_High_Fidelity_Model.setIcon(icon7)
        self.actionChoose_High_Fidelity_Model.setObjectName("actionChoose_High_Fidelity_Model")
        self.actionConfigure_Input_Variables = QtGui.QAction(MainWindow)
        icon8 = QtGui.QIcon()
        icon8.addPixmap(QtGui.QPixmap(":/icons/config_input.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionConfigure_Input_Variables.setIcon(icon8)
        self.actionConfigure_Input_Variables.setObjectName("actionConfigure_Input_Variables")
        self.actionConfigure_Output_Variables = QtGui.QAction(MainWindow)
        icon9 = QtGui.QIcon()
        icon9.addPixmap(QtGui.QPixmap(":/icons/config_output.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionConfigure_Output_Variables.setIcon(icon9)
        self.actionConfigure_Output_Variables.setObjectName("actionConfigure_Output_Variables")
        self.actionPrepare_Training_Sequence = QtGui.QAction(MainWindow)
        icon10 = QtGui.QIcon()
        icon10.addPixmap(QtGui.QPixmap(":/icons/steps_train.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionPrepare_Training_Sequence.setIcon(icon10)
        self.actionPrepare_Training_Sequence.setObjectName("actionPrepare_Training_Sequence")
        self.actionPrepare_Validation_Sequence = QtGui.QAction(MainWindow)
        icon11 = QtGui.QIcon()
        icon11.addPixmap(QtGui.QPixmap(":/icons/steps_valid.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionPrepare_Validation_Sequence.setIcon(icon11)
        self.actionPrepare_Validation_Sequence.setObjectName("actionPrepare_Validation_Sequence")
        self.actionPerform_Training_Simulation = QtGui.QAction(MainWindow)
        icon12 = QtGui.QIcon()
        icon12.addPixmap(QtGui.QPixmap(":/icons/run_train.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionPerform_Training_Simulation.setIcon(icon12)
        self.actionPerform_Training_Simulation.setObjectName("actionPerform_Training_Simulation")
        self.actionPerform_Validation_Simulation = QtGui.QAction(MainWindow)
        icon13 = QtGui.QIcon()
        icon13.addPixmap(QtGui.QPixmap(":/icons/run_valid.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionPerform_Validation_Simulation.setIcon(icon13)
        self.actionPerform_Validation_Simulation.setObjectName("actionPerform_Validation_Simulation")
        self.actionDABNet = QtGui.QAction(MainWindow)
        self.actionDABNet.setCheckable(True)
        self.actionDABNet.setObjectName("actionDABNet")
        self.actionNARMA = QtGui.QAction(MainWindow)
        self.actionNARMA.setCheckable(True)
        self.actionNARMA.setObjectName("actionNARMA")
        self.actionGenerate_Reduced_Model = QtGui.QAction(MainWindow)
        icon14 = QtGui.QIcon()
        icon14.addPixmap(QtGui.QPixmap(":/icons/build_drm.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionGenerate_Reduced_Model.setIcon(icon14)
        self.actionGenerate_Reduced_Model.setObjectName("actionGenerate_Reduced_Model")
        self.actionUse_Balanced_Model_For_Prediction = QtGui.QAction(MainWindow)
        self.actionUse_Balanced_Model_For_Prediction.setCheckable(True)
        self.actionUse_Balanced_Model_For_Prediction.setObjectName("actionUse_Balanced_Model_For_Prediction")
        self.actionUse_High_Fidelity_Model_History_For_Prediction = QtGui.QAction(MainWindow)
        self.actionUse_High_Fidelity_Model_History_For_Prediction.setCheckable(True)
        self.actionUse_High_Fidelity_Model_History_For_Prediction.setObjectName("actionUse_High_Fidelity_Model_History_For_Prediction")
        self.actionPredict_Traning_Response = QtGui.QAction(MainWindow)
        icon15 = QtGui.QIcon()
        icon15.addPixmap(QtGui.QPixmap(":/icons/predict_train.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionPredict_Traning_Response.setIcon(icon15)
        self.actionPredict_Traning_Response.setObjectName("actionPredict_Traning_Response")
        self.actionPredict_Validation_Response = QtGui.QAction(MainWindow)
        icon16 = QtGui.QIcon()
        icon16.addPixmap(QtGui.QPixmap(":/icons/predict_valid.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionPredict_Validation_Response.setIcon(icon16)
        self.actionPredict_Validation_Response.setObjectName("actionPredict_Validation_Response")
        self.actionPlot_Training_Response = QtGui.QAction(MainWindow)
        icon17 = QtGui.QIcon()
        icon17.addPixmap(QtGui.QPixmap(":/icons/plot_train.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionPlot_Training_Response.setIcon(icon17)
        self.actionPlot_Training_Response.setObjectName("actionPlot_Training_Response")
        self.actionPlot_Validation_Response = QtGui.QAction(MainWindow)
        icon18 = QtGui.QIcon()
        icon18.addPixmap(QtGui.QPixmap(":/icons/plot_valid.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionPlot_Validation_Response.setIcon(icon18)
        self.actionPlot_Validation_Response.setObjectName("actionPlot_Validation_Response")
        self.actionSpecify_Noise = QtGui.QAction(MainWindow)
        icon19 = QtGui.QIcon()
        icon19.addPixmap(QtGui.QPixmap(":/icons/noise.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionSpecify_Noise.setIcon(icon19)
        self.actionSpecify_Noise.setObjectName("actionSpecify_Noise")
        self.actionAnalyze = QtGui.QAction(MainWindow)
        icon20 = QtGui.QIcon()
        icon20.addPixmap(QtGui.QPixmap(":/icons/uq.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionAnalyze.setIcon(icon20)
        self.actionAnalyze.setObjectName("actionAnalyze")
        self.actionMessages_To_Log_File = QtGui.QAction(MainWindow)
        icon21 = QtGui.QIcon()
        icon21.addPixmap(QtGui.QPixmap(":/icons/export_log.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionMessages_To_Log_File.setIcon(icon21)
        self.actionMessages_To_Log_File.setObjectName("actionMessages_To_Log_File")
        self.actionTraining_Data = QtGui.QAction(MainWindow)
        icon22 = QtGui.QIcon()
        icon22.addPixmap(QtGui.QPixmap(":/icons/training_data.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionTraining_Data.setIcon(icon22)
        self.actionTraining_Data.setObjectName("actionTraining_Data")
        self.actionValidation_Data = QtGui.QAction(MainWindow)
        icon23 = QtGui.QIcon()
        icon23.addPixmap(QtGui.QPixmap(":/icons/validation_data.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionValidation_Data.setIcon(icon23)
        self.actionValidation_Data.setObjectName("actionValidation_Data")
        self.actionCovariance_Matrices = QtGui.QAction(MainWindow)
        icon24 = QtGui.QIcon()
        icon24.addPixmap(QtGui.QPixmap(":/icons/covariance.png"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionCovariance_Matrices.setIcon(icon24)
        self.actionCovariance_Matrices.setObjectName("actionCovariance_Matrices")
        self.actionRead_Custom_Training_Data = QtGui.QAction(MainWindow)
        self.actionRead_Custom_Training_Data.setObjectName("actionRead_Custom_Training_Data")
        self.actionRead_Custom_Validation_Data = QtGui.QAction(MainWindow)
        self.actionRead_Custom_Validation_Data.setObjectName("actionRead_Custom_Validation_Data")
        self.menuExport.addAction(self.actionD_RM_As_Matlab_Script_File)
        self.menuExport.addAction(self.actionTraining_Data)
        self.menuExport.addAction(self.actionValidation_Data)
        self.menuExport.addAction(self.actionCovariance_Matrices)
        self.menuExport.addAction(self.actionMessages_To_Log_File)
        self.menuFile.addAction(self.actionNew)
        self.menuFile.addAction(self.actionOpen)
        self.menuFile.addAction(self.actionClose)
        self.menuFile.addAction(self.actionSave)
        self.menuFile.addAction(self.actionSave_As)
        self.menuFile.addSeparator()
        self.menuFile.addAction(self.menuExport.menuAction())
        self.menuView.addAction(self.actionToolbar)
        self.menuView.addAction(self.actionStatus_Bar)
        self.menuSetup.addAction(self.actionChoose_High_Fidelity_Model)
        self.menuSetup.addAction(self.actionConfigure_Input_Variables)
        self.menuSetup.addAction(self.actionConfigure_Output_Variables)
        self.menuSetup.addAction(self.actionPrepare_Training_Sequence)
        self.menuSetup.addAction(self.actionPrepare_Validation_Sequence)
        self.menuSetup.addAction(self.actionRead_Custom_Training_Data)
        self.menuSetup.addAction(self.actionRead_Custom_Validation_Data)
        self.menuD_RM_Model_Type.addAction(self.actionDABNet)
        self.menuD_RM_Model_Type.addAction(self.actionNARMA)
        self.menuBuild.addAction(self.actionPerform_Training_Simulation)
        self.menuBuild.addAction(self.actionPerform_Validation_Simulation)
        self.menuBuild.addSeparator()
        self.menuBuild.addAction(self.menuD_RM_Model_Type.menuAction())
        self.menuBuild.addAction(self.actionGenerate_Reduced_Model)
        self.menuPost_Process.addAction(self.actionUse_Balanced_Model_For_Prediction)
        self.menuPost_Process.addAction(self.actionUse_High_Fidelity_Model_History_For_Prediction)
        self.menuPost_Process.addSeparator()
        self.menuPost_Process.addAction(self.actionPredict_Traning_Response)
        self.menuPost_Process.addAction(self.actionPredict_Validation_Response)
        self.menuPost_Process.addSeparator()
        self.menuPost_Process.addAction(self.actionPlot_Training_Response)
        self.menuPost_Process.addAction(self.actionPlot_Validation_Response)
        self.menuUQ.addAction(self.actionSpecify_Noise)
        self.menuUQ.addAction(self.actionAnalyze)
        self.menuHelp.addAction(self.actionAbout_D_RM_Builder)
        self.menubar.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuView.menuAction())
        self.menubar.addAction(self.menuSetup.menuAction())
        self.menubar.addAction(self.menuBuild.menuAction())
        self.menubar.addAction(self.menuPost_Process.menuAction())
        self.menubar.addAction(self.menuUQ.menuAction())
        self.menubar.addAction(self.menuHelp.menuAction())
        self.toolBar.addAction(self.actionNew)
        self.toolBar.addAction(self.actionOpen)
        self.toolBar.addAction(self.actionSave)
        self.toolBar.addAction(self.actionClose)
        self.toolBar.addSeparator()
        self.toolBar.addAction(self.actionChoose_High_Fidelity_Model)
        self.toolBar.addAction(self.actionConfigure_Input_Variables)
        self.toolBar.addAction(self.actionConfigure_Output_Variables)
        self.toolBar.addAction(self.actionPrepare_Training_Sequence)
        self.toolBar.addAction(self.actionPrepare_Validation_Sequence)
        self.toolBar.addSeparator()
        self.toolBar.addAction(self.actionPerform_Training_Simulation)
        self.toolBar.addAction(self.actionPerform_Validation_Simulation)
        self.toolBar.addAction(self.actionGenerate_Reduced_Model)
        self.toolBar.addSeparator()
        self.toolBar.addAction(self.actionPredict_Traning_Response)
        self.toolBar.addAction(self.actionPredict_Validation_Response)
        self.toolBar.addAction(self.actionPlot_Training_Response)
        self.toolBar.addAction(self.actionPlot_Validation_Response)
        self.toolBar.addSeparator()
        self.toolBar.addAction(self.actionSpecify_Noise)
        self.toolBar.addAction(self.actionAnalyze)
        self.toolBar.addSeparator()
        self.toolBar.addAction(self.actionAbout_D_RM_Builder)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QtGui.QApplication.translate("MainWindow", "D-RM Builder", None, QtGui.QApplication.UnicodeUTF8))
        self.menuFile.setTitle(QtGui.QApplication.translate("MainWindow", "File", None, QtGui.QApplication.UnicodeUTF8))
        self.menuExport.setTitle(QtGui.QApplication.translate("MainWindow", "Export", None, QtGui.QApplication.UnicodeUTF8))
        self.menuView.setTitle(QtGui.QApplication.translate("MainWindow", "View", None, QtGui.QApplication.UnicodeUTF8))
        self.menuSetup.setTitle(QtGui.QApplication.translate("MainWindow", "Setup", None, QtGui.QApplication.UnicodeUTF8))
        self.menuBuild.setTitle(QtGui.QApplication.translate("MainWindow", "Build", None, QtGui.QApplication.UnicodeUTF8))
        self.menuD_RM_Model_Type.setTitle(QtGui.QApplication.translate("MainWindow", "D-RM Model Type", None, QtGui.QApplication.UnicodeUTF8))
        self.menuPost_Process.setTitle(QtGui.QApplication.translate("MainWindow", "Post-Process", None, QtGui.QApplication.UnicodeUTF8))
        self.menuUQ.setTitle(QtGui.QApplication.translate("MainWindow", "UQ", None, QtGui.QApplication.UnicodeUTF8))
        self.menuHelp.setTitle(QtGui.QApplication.translate("MainWindow", "Help", None, QtGui.QApplication.UnicodeUTF8))
        self.toolBar.setWindowTitle(QtGui.QApplication.translate("MainWindow", "toolBar", None, QtGui.QApplication.UnicodeUTF8))
        self.actionAbout_D_RM_Builder.setText(QtGui.QApplication.translate("MainWindow", "About D-RM Builder...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionAbout_D_RM_Builder.setStatusTip(QtGui.QApplication.translate("MainWindow", "About D-RM Builder", None, QtGui.QApplication.UnicodeUTF8))
        self.actionNew.setText(QtGui.QApplication.translate("MainWindow", "New", None, QtGui.QApplication.UnicodeUTF8))
        self.actionNew.setStatusTip(QtGui.QApplication.translate("MainWindow", "Create a new D-RM Builder case", None, QtGui.QApplication.UnicodeUTF8))
        self.actionNew.setShortcut(QtGui.QApplication.translate("MainWindow", "Ctrl+N", None, QtGui.QApplication.UnicodeUTF8))
        self.actionOpen.setText(QtGui.QApplication.translate("MainWindow", "Open...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionOpen.setStatusTip(QtGui.QApplication.translate("MainWindow", "Open an existing D-RM Builder case", None, QtGui.QApplication.UnicodeUTF8))
        self.actionClose.setText(QtGui.QApplication.translate("MainWindow", "Close", None, QtGui.QApplication.UnicodeUTF8))
        self.actionClose.setStatusTip(QtGui.QApplication.translate("MainWindow", "Close current D-RM Builder case", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave.setText(QtGui.QApplication.translate("MainWindow", "Save", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave.setStatusTip(QtGui.QApplication.translate("MainWindow", "Save current D-RM Builder case", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_As.setText(QtGui.QApplication.translate("MainWindow", "Save As...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_As.setStatusTip(QtGui.QApplication.translate("MainWindow", "Save current D-RM Builder case to a different file", None, QtGui.QApplication.UnicodeUTF8))
        self.actionD_RM_As_Matlab_Script_File.setText(QtGui.QApplication.translate("MainWindow", "D-RM As Matlab Script File...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionD_RM_As_Matlab_Script_File.setStatusTip(QtGui.QApplication.translate("MainWindow", "Export D-RM model to MATLAB script file", None, QtGui.QApplication.UnicodeUTF8))
        self.actionToolbar.setText(QtGui.QApplication.translate("MainWindow", "Toolbar", None, QtGui.QApplication.UnicodeUTF8))
        self.actionStatus_Bar.setText(QtGui.QApplication.translate("MainWindow", "Status Bar", None, QtGui.QApplication.UnicodeUTF8))
        self.actionChoose_High_Fidelity_Model.setText(QtGui.QApplication.translate("MainWindow", "Choose High-Fidelity Model...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionChoose_High_Fidelity_Model.setStatusTip(QtGui.QApplication.translate("MainWindow", "Choose a configured high-fidelity ACM dynamic model", None, QtGui.QApplication.UnicodeUTF8))
        self.actionConfigure_Input_Variables.setText(QtGui.QApplication.translate("MainWindow", "Configure Input Variables...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionConfigure_Input_Variables.setStatusTip(QtGui.QApplication.translate("MainWindow", "Specify input parameters", None, QtGui.QApplication.UnicodeUTF8))
        self.actionConfigure_Output_Variables.setText(QtGui.QApplication.translate("MainWindow", "Configure Output Variables...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionConfigure_Output_Variables.setStatusTip(QtGui.QApplication.translate("MainWindow", "Specify output parameters", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPrepare_Training_Sequence.setText(QtGui.QApplication.translate("MainWindow", "Prepare Training Sequence...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPrepare_Training_Sequence.setStatusTip(QtGui.QApplication.translate("MainWindow", "Prepare a sequence of step changes for training", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPrepare_Validation_Sequence.setText(QtGui.QApplication.translate("MainWindow", "Prepare Validation Sequence...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPrepare_Validation_Sequence.setStatusTip(QtGui.QApplication.translate("MainWindow", "Prepare a sequence of step changes for validation", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPerform_Training_Simulation.setText(QtGui.QApplication.translate("MainWindow", "Perform Training Simulation", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPerform_Training_Simulation.setStatusTip(QtGui.QApplication.translate("MainWindow", "Launch ACM simulation for training sequence", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPerform_Validation_Simulation.setText(QtGui.QApplication.translate("MainWindow", "Perform Validation Simulation", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPerform_Validation_Simulation.setStatusTip(QtGui.QApplication.translate("MainWindow", "Launch ACM simulation for validation sequence", None, QtGui.QApplication.UnicodeUTF8))
        self.actionDABNet.setText(QtGui.QApplication.translate("MainWindow", "DABNet", None, QtGui.QApplication.UnicodeUTF8))
        self.actionDABNet.setStatusTip(QtGui.QApplication.translate("MainWindow", "Set D-RM type as DABNet", None, QtGui.QApplication.UnicodeUTF8))
        self.actionNARMA.setText(QtGui.QApplication.translate("MainWindow", "NARMA", None, QtGui.QApplication.UnicodeUTF8))
        self.actionNARMA.setStatusTip(QtGui.QApplication.translate("MainWindow", "Set D-RM type as NARMA", None, QtGui.QApplication.UnicodeUTF8))
        self.actionGenerate_Reduced_Model.setText(QtGui.QApplication.translate("MainWindow", "Generate Reduced Model...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionGenerate_Reduced_Model.setStatusTip(QtGui.QApplication.translate("MainWindow", "Build D-RM using the training sequence", None, QtGui.QApplication.UnicodeUTF8))
        self.actionUse_Balanced_Model_For_Prediction.setText(QtGui.QApplication.translate("MainWindow", "Use Balanced Model For Prediction", None, QtGui.QApplication.UnicodeUTF8))
        self.actionUse_Balanced_Model_For_Prediction.setStatusTip(QtGui.QApplication.translate("MainWindow", "Toggle to use the balanced model for prediction", None, QtGui.QApplication.UnicodeUTF8))
        self.actionUse_High_Fidelity_Model_History_For_Prediction.setText(QtGui.QApplication.translate("MainWindow", "Use High-Fidelity Model History For Prediction", None, QtGui.QApplication.UnicodeUTF8))
        self.actionUse_High_Fidelity_Model_History_For_Prediction.setStatusTip(QtGui.QApplication.translate("MainWindow", "Use the high-fidelity model history for prediction", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPredict_Traning_Response.setText(QtGui.QApplication.translate("MainWindow", "Predict Traning Response", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPredict_Traning_Response.setStatusTip(QtGui.QApplication.translate("MainWindow", "Using the generated D-RM to predict response of training sequence", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPredict_Validation_Response.setText(QtGui.QApplication.translate("MainWindow", "Predict Validation Response", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPredict_Validation_Response.setStatusTip(QtGui.QApplication.translate("MainWindow", "Use the generated D-RM to predict response of validation sequence", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPlot_Training_Response.setText(QtGui.QApplication.translate("MainWindow", "Plot Training Response...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPlot_Training_Response.setStatusTip(QtGui.QApplication.translate("MainWindow", "Plot the response of training sequence", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPlot_Validation_Response.setText(QtGui.QApplication.translate("MainWindow", "Plot Validation Response...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPlot_Validation_Response.setStatusTip(QtGui.QApplication.translate("MainWindow", "Plot the response of validation sequence", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSpecify_Noise.setText(QtGui.QApplication.translate("MainWindow", "Specify Noise...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSpecify_Noise.setStatusTip(QtGui.QApplication.translate("MainWindow", "Specify the process and measurement noise", None, QtGui.QApplication.UnicodeUTF8))
        self.actionAnalyze.setText(QtGui.QApplication.translate("MainWindow", "Analyze...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionAnalyze.setStatusTip(QtGui.QApplication.translate("MainWindow", "Perform UQ analysis", None, QtGui.QApplication.UnicodeUTF8))
        self.actionMessages_To_Log_File.setText(QtGui.QApplication.translate("MainWindow", "Messages To Log File...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionMessages_To_Log_File.setStatusTip(QtGui.QApplication.translate("MainWindow", "Export message window text to a log file", None, QtGui.QApplication.UnicodeUTF8))
        self.actionTraining_Data.setText(QtGui.QApplication.translate("MainWindow", "Training Data...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionValidation_Data.setText(QtGui.QApplication.translate("MainWindow", "Validation Data...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionCovariance_Matrices.setText(QtGui.QApplication.translate("MainWindow", "Covariance Matrices...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionRead_Custom_Training_Data.setText(QtGui.QApplication.translate("MainWindow", "Read Custom Training Data...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionRead_Custom_Validation_Data.setText(QtGui.QApplication.translate("MainWindow", "Read Custom Validation Data...", None, QtGui.QApplication.UnicodeUTF8))

import icons_rc
