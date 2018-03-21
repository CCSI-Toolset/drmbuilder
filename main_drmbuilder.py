# main_drmbuilder.py
# DRMBuilder's Main Window
import sys
import os
import time
import threading
import numpy as np
from PySide.QtGui import *
from PySide.QtCore import *
from matplotlib import pyplot as plt
from framework.drm_manager import DRMManager
from gui.input_variable_dlg import InputVariableDlg
from gui.output_variable_dlg import OutputVariableDlg
from gui.step_change_dlg import StepChangeDlg
from gui.dabnet_param_dlg import DABNetParamDlg
from gui.narma_param_dlg import NARMAParamDlg
from gui.plot_result_dlg import PlotResultDlg
from gui.noise_dlg import NoiseDlg
from gui.main_drmbuilder_ui import Ui_MainWindow


class MainDRMBuilder(QMainWindow, Ui_MainWindow):
    """
        The main window for D-RM Builder
    """
    def __init__(self, drm_manager_init, parent=None):
        super(MainDRMBuilder,self).__init__(parent)
        self.setupUi(self)
        self.drm_manager = drm_manager_init
        if self.drm_manager == None:
            self.bclosed = True
            self.textMessage.setText("")
        else:
            self.bclosed = False
            self.textMessage.setText(self.drm_manager.msg)
        self.bthread = False
        self.bplot_show = False
        self.update_view()
        #signals and slots
        self.actionAbout_D_RM_Builder.triggered.connect(self.on_action_about_drmbuilder)
        self.actionNew.triggered.connect(self.on_action_new)
        self.actionOpen.triggered.connect(self.on_action_open)
        self.actionSave.triggered.connect(self.on_action_save)
        self.actionSave_As.triggered.connect(self.on_action_save_as)
        self.actionClose.triggered.connect(self.on_action_close)
        self.actionD_RM_As_Matlab_Script_File.triggered.connect(self.on_action_export_drm_matlab)
        self.actionTraining_Data.triggered.connect(self.on_action_export_training_data)
        self.actionValidation_Data.triggered.connect(self.on_action_export_validation_data)
        self.actionCovariance_Matrices.triggered.connect(self.on_action_export_covariance_matrices)
        self.actionMessages_To_Log_File.triggered.connect(self.on_action_export_log)
        self.actionToolbar.triggered.connect(self.on_action_toolbar)
        self.actionStatus_Bar.triggered.connect(self.on_action_status_bar)
        self.actionChoose_High_Fidelity_Model.triggered.connect(self.on_action_choose_high_fidelity_model)
        self.actionConfigure_Input_Variables.triggered.connect(self.on_action_input_variables)
        self.actionConfigure_Output_Variables.triggered.connect(self.on_action_output_variables)
        self.actionPrepare_Training_Sequence.triggered.connect(self.on_action_training_sequence)
        self.actionPrepare_Validation_Sequence.triggered.connect(self.on_action_validation_sequence)
        # for custom training and validation
        self.actionRead_Custom_Training_Data.triggered.connect(self.on_action_read_custom_training_data)
        self.actionRead_Custom_Validation_Data.triggered.connect(self.on_action_read_custom_validation_data)
        # end of custon training and validation
        self.actionPerform_Training_Simulation.triggered.connect(self.on_action_training_simulation)
        self.actionPerform_Validation_Simulation.triggered.connect(self.on_action_validation_simulation)
        self.actionDABNet.triggered.connect(self.on_action_choose_dabnet_model)
        self.actionNARMA.triggered.connect(self.on_action_choose_narma_model)
        self.actionGenerate_Reduced_Model.triggered.connect(self.on_action_generate_drm)
        self.actionUse_Balanced_Model_For_Prediction.triggered.connect(self.on_action_use_balanced_model_for_dabnet)
        self.actionUse_High_Fidelity_Model_History_For_Prediction.triggered.connect(self.on_action_use_hfm_history_for_narma)
        self.actionPredict_Traning_Response.triggered.connect(self.on_action_predict_training)
        self.actionPredict_Validation_Response.triggered.connect(self.on_action_predict_validation)
        self.actionPlot_Training_Response.triggered.connect(self.on_action_plot_training)
        self.actionPlot_Validation_Response.triggered.connect(self.on_action_plot_validation)
        self.actionSpecify_Noise.triggered.connect(self.on_action_specify_noise)
        self.actionAnalyze.triggered.connect(self.on_action_analyze_uq)

    def on_action_about_drmbuilder(self):
        QMessageBox.information(self, "About D-RM Builder", "D-RM Builder Version 2.0\nCopyright CCSI, 2018", QMessageBox.Close)

    def on_action_save(self):
        if self.drm_manager.bsaved:
            file_name = self.drm_manager.doc_name + '.drmb'
            self.drm_manager.write_to_json_file(file_name)
            self.drm_manager.msg += file_name + " is saved\n"
        else:
            self.on_action_save_as()
        self.update_view()

    def on_action_save_as(self):
        file_name, file_filter = QFileDialog.getSaveFileName(self, "Save D-RM Builder File", self.drm_manager.doc_name, "D-RM Builder Files (*.drmb)")
        if file_name:
            path_name = os.path.dirname(file_name)
            os.chdir(path_name)
            print ('Changed working directory to ' + path_name)
            self.drm_manager.write_to_json_file(file_name)
            self.drm_manager.msg += file_name + " is saved\n"
            ipoint = file_name.rindex('/')
            doc_name = file_name[ipoint+1:]
            ipoint = doc_name.rindex('.')
            self.drm_manager.doc_name = doc_name[0:ipoint]
            self.update_view()

    def on_action_new(self):
        if not self.bclosed:
            if self.drm_manager.bmodified:
                if (QMessageBox.question(self, "Save Current Case", "Do you want to save the current case?", QMessageBox.Yes, QMessageBox.No)==QMessageBox.Yes):
                    self.on_action_save()
            del self.drm_manager
        self.drm_manager = DRMManager()
        self.bclosed = False
        self.update_view()

    def on_action_open(self):
        if not self.bclosed:
            if self.drm_manager.bmodified:
                if (QMessageBox.question(self, "Save Current Case", "Do you want to save the current case?", QMessageBox.Yes, QMessageBox.No)==QMessageBox.Yes):
                    self.on_action_save()
        file_name, file_filter = QFileDialog.getOpenFileName(self, "Open D-RM Builder File", None, "D-RM Builder Files (*.drmb)")
        if file_name:
            if not self.bclosed:
                del self.drm_manager
            path_name = os.path.dirname(file_name)
            os.chdir(path_name)
            print ('Changed working directory to ' + path_name)
            self.drm_manager = DRMManager()
            self.drm_manager.read_from_json_file(file_name)
            ipoint = file_name.rindex('/')
            doc_name = file_name[ipoint+1:]
            ipoint = doc_name.rindex('.')
            self.drm_manager.doc_name = doc_name[0:ipoint]
            self.bclosed = False
            self.update_view()

    def on_action_close(self):
        del self.drm_manager
        self.drm_manager = None
        self.bclosed = True
        self.textMessage.setText("")
        self.update_view()

    def on_action_export_drm_matlab(self):
        default_file_name = self.drm_manager.doc_name + ".m"
        file_name, file_filter = QFileDialog.getSaveFileName(self, "Save D-RM As Matlab Script", default_file_name, "Matlab Script File (*.m)")
        if file_name:
            self.drm_manager.write_to_matlab_file(file_name)
            self.drm_manager.msg += file_name + " is exported\n"
            self.update_view()

    def on_action_export_training_data(self):
        default_file_name = self.drm_manager.doc_name + "_training_data.csv"
        file_name, file_filter = QFileDialog.getSaveFileName(self, "Save Training Data", default_file_name, "CSV File (*.csv)")
        if file_name:
            self.drm_manager.write_to_training_data_file(file_name)
            self.drm_manager.msg += file_name + " is exported\n"
            self.update_view()

    def on_action_export_validation_data(self):
        default_file_name = self.drm_manager.doc_name + "_validation_data.csv"
        file_name, file_filter = QFileDialog.getSaveFileName(self, "Save Validation Data", default_file_name, "CSV File (*.csv)")
        if file_name:
            self.drm_manager.write_to_validation_data_file(file_name)
            self.drm_manager.msg += file_name + " is exported\n"
            self.update_view()

    def on_action_export_covariance_matrices(self):
        default_file_name = self.drm_manager.doc_name + "_covariance_matrices.csv"
        file_name, file_filter = QFileDialog.getSaveFileName(self, "Save Covariance Matrices", default_file_name, "CSV File (*.csv)")
        if file_name:
            self.drm_manager.write_to_covariance_matrices_file(file_name)
            self.drm_manager.msg += file_name + " is exported\n"
            self.update_view()

    def on_action_export_log(self):
        default_file_name = self.drm_manager.doc_name + ".log"
        file_name, file_filter = QFileDialog.getSaveFileName(self, "Save Message Log", default_file_name, "Text Log File (*.log)")
        if file_name:
            self.drm_manager.write_to_log_file(file_name)
            self.drm_manager.msg += file_name + " is exported\n"
            self.update_view()

    def on_action_toolbar(self):
        if self.actionToolbar.isChecked():
            self.toolBar.show()
        else:
            self.toolBar.hide()

    def on_action_status_bar(self):
        if self.actionStatus_Bar.isChecked():
            self.statusbar.show()
        else:
            self.statusbar.hide()

    def on_action_choose_high_fidelity_model(self):
        default_file_name = self.drm_manager.simsinter_config
        file_name, file_filter = QFileDialog.getOpenFileName(self, "SimSinter Configuration File", default_file_name, "SimSinter Configuration (*.json)")
        if file_name:
            path_name = os.path.dirname(file_name)
            os.chdir(path_name)
            print ('Changed working directory to ' + path_name)
            # self.drm_manager.simsinter_config = file_name
            self.drm_manager.simsinter_config = os.path.basename(file_name)
            if self.drm_manager.parse_simsinter_config():
                self.drm_manager.bhfm_file = True
                self.drm_manager.binputready = False
                self.drm_manager.boutputready = False
                self.drm_manager.bsequence_train = False
                self.drm_manager.bsequence_valid = False
                self.drm_manager.brun_train = False
                self.drm_manager.brun_valid = False
                self.drm_manager.bbuilt_dabnet = False
                self.drm_manager.bbuilt_narma = False
                self.drm_manager.bpredict_train = False
                self.drm_manager.bpredict_valid = False
                self.drm_manager.bnoise_set = False
                self.drm_manager.brun_uq = False
                self.drm_manager.bmodified = True
                self.drm_manager.msg += "SimSinter configuration has been read\n"
                self.update_view()

    def on_action_input_variables(self):
        dat = dict()
        dat['unit_time'] = self.drm_manager.acm_input.unit_time
        dat['dt_sampling'] = self.drm_manager.acm_input.dt_sampling
        dat['dt_min_solver'] = self.drm_manager.acm_input.dt_min_solver
        dat['dt_ramp'] = self.drm_manager.acm_input.dt_ramp
        dat['input_list'] = self.drm_manager.input_list_all
        dlg = InputVariableDlg(dat)
        if dlg.exec_()==1:
            self.drm_manager.acm_input.dt_sampling = dlg.dat['dt_sampling']
            self.drm_manager.input_list_all = dlg.dat['input_list']
            self.drm_manager.update_selected_input_list()
            self.drm_manager.dabnet_input.set_numbers_of_input_and_output(len(self.drm_manager.input_list_sel),len(self.drm_manager.output_list_sel))
            self.drm_manager.binputready = True
            self.drm_manager.bsequence_train = False
            self.drm_manager.bsequence_valid = False
            self.drm_manager.brun_train = False
            self.drm_manager.brun_valid = False
            self.drm_manager.bbuilt_dabnet = False
            self.drm_manager.bbuilt_narma = False
            self.drm_manager.bpredict_train = False
            self.drm_manager.bpredict_valid = False
            self.drm_manager.bnoise_set = False
            self.drm_manager.brun_uq = False
            self.drm_manager.bmodified = True
            self.drm_manager.msg += "Input variables have been configured\n"
            self.update_view()

    def on_action_output_variables(self):
        dat = dict()
        dat['output_list'] = self.drm_manager.output_list_all
        dlg = OutputVariableDlg(dat)
        if dlg.exec_()==1:
            self.drm_manager.output_list_all = dlg.dat['output_list']
            self.drm_manager.update_selected_output_list()
            self.drm_manager.dabnet_input.set_numbers_of_input_and_output(len(self.drm_manager.input_list_sel),len(self.drm_manager.output_list_sel))
            self.drm_manager.boutputready = True
            self.drm_manager.bbuilt_dabnet = False
            self.drm_manager.bbuilt_narma = False
            self.drm_manager.bpredict_train = False
            self.drm_manager.bpredict_valid = False
            self.drm_manager.bnoise_set = False
            self.drm_manager.brun_uq = False
            self.drm_manager.bmodified = True
            self.drm_manager.msg += "Output variables have been configured\n"
            self.update_view()

    def on_action_training_sequence(self):
        dat = dict()
        dat['dt_sampling'] = self.drm_manager.acm_input.dt_sampling
        dat['dt_min_solver'] = self.drm_manager.acm_input.dt_min_solver
        dat['unit_time'] = self.drm_manager.acm_input.unit_time
        dat['step_change'] = self.drm_manager.step_change_train
        dlg = StepChangeDlg(dat)
        if dlg.exec_()==1:
            self.drm_manager.step_change_train = dlg.dat['step_change']
            self.drm_manager.sample_for_training()
            self.drm_manager.bsequence_train = True
            self.drm_manager.bsequence_valid = False
            self.drm_manager.brun_train = False
            self.drm_manager.brun_valid = False
            self.drm_manager.bbuilt_dabnet = False
            self.drm_manager.bbuilt_narma = False
            self.drm_manager.bpredict_train = False
            self.drm_manager.bpredict_valid = False
            self.drm_manager.brun_uq = False
            self.drm_manager.bmodified = True
            self.drm_manager.msg += "Step change sequence for training has been generated\n"
            self.update_view()

    def on_action_validation_sequence(self):
        dat = dict()
        dat['dt_sampling'] = self.drm_manager.acm_input.dt_sampling
        dat['dt_min_solver'] = self.drm_manager.acm_input.dt_min_solver
        dat['unit_time'] = self.drm_manager.acm_input.unit_time
        dat['step_change'] = self.drm_manager.step_change_valid
        dlg = StepChangeDlg(dat)
        if dlg.exec_()==1:
            self.drm_manager.step_change_valid = dlg.dat['step_change']
            self.drm_manager.sample_for_validation()
            self.drm_manager.bsequence_valid = True
            self.drm_manager.brun_valid = False
            self.drm_manager.bpredict_valid = False
            self.drm_manager.brun_uq = False
            self.drm_manager.bmodified = True
            self.drm_manager.msg += "Step change sequence for validation has been generated\n"
            self.update_view()

    def on_action_read_custom_training_data(self):
        default_file_name = self.drm_manager.doc_name + '.csv'
        file_name, file_filter = QFileDialog.getOpenFileName(self, "Custom training data file", default_file_name, "Custom data (*.csv)")
        if file_name:
            if self.drm_manager.parse_custom_train_data(file_name):
                path_name = os.path.dirname(file_name)
                os.chdir(path_name)
                print ('Changed working directory to ' + path_name)
                self.drm_manager.dabnet_input.set_numbers_of_input_and_output(len(self.drm_manager.input_list_sel),len(self.drm_manager.output_list_sel))
                self.drm_manager.bread_custom_train = True
                self.drm_manager.binputready = True
                self.drm_manager.boutputready = True
                self.drm_manager.bread_custom_valid = False
                self.drm_manager.brun_valid = False
                self.drm_manager.brun_train = True
                self.drm_manager.bbuilt_dabnet = False
                self.drm_manager.bbuilt_narma = False
                self.drm_manager.bpredict_train = False
                self.drm_manager.bpredict_valid = False
                self.drm_manager.bnoise_set = False
                self.drm_manager.brun_uq = False
                self.drm_manager.bmodified = True
                self.drm_manager.msg += "Custom training data have been read\n"
                self.update_view()


    def on_action_read_custom_validation_data(self):
        default_file_name = self.drm_manager.doc_name + '.csv'
        file_name, file_filter = QFileDialog.getOpenFileName(self, "Custom validation data file", default_file_name, "Custom data (*.csv)")
        if file_name:
            if self.drm_manager.parse_custom_valid_data(file_name):
                self.drm_manager.bread_custom_valid = True
                self.drm_manager.brun_valid = True
                self.drm_manager.bpredict_valid = False
                self.drm_manager.brun_uq = False
                self.drm_manager.bmodified = True
                self.drm_manager.msg += "Custom validation data have been read\n"
                self.update_view()

    def on_action_training_simulation(self):
        QApplication.setOverrideCursor(QCursor(Qt.WaitCursor))
        self.bthread = True
        self.update_view()
        thd = threading.Thread(target=self.drm_manager.run_acm_training)
        thd.start()
        # wait until the thread finishes
        thd.join()
        self.bthread = False
        QApplication.restoreOverrideCursor()
        self.drm_manager.brun_train = True
        self.drm_manager.bbuilt_dabnet = False
        self.drm_manager.bbuilt_narma = False
        self.drm_manager.bpredict_train = False
        self.drm_manager.bpredict_valid = False
        self.drm_manager.brun_uq = False
        self.drm_manager.bmodified = True
        self.drm_manager.msg += "Simulation of the training sequence has been completed\n"
        self.update_view()

    def on_action_validation_simulation(self):
        QApplication.setOverrideCursor(QCursor(Qt.WaitCursor))
        self.bthread = True
        self.update_view()
        thd = threading.Thread(target=self.drm_manager.run_acm_validation)
        thd.start()
        # wait until the thread finishes
        thd.join()
        self.bthread = False
        QApplication.restoreOverrideCursor()
        self.drm_manager.brun_valid = True
        self.drm_manager.bpredict_valid = False
        self.drm_manager.brun_uq = False
        self.drm_manager.bmodified = True
        self.drm_manager.msg += "Simulation of the validation sequence has been completed\n"
        self.update_view()

    def on_action_choose_dabnet_model(self):
        self.drm_manager.imodel_type = 0
        self.drm_manager.bmodified = True
        self.drm_manager.msg += "DABNet model is enabled\n"
        self.update_view()

    def on_action_choose_narma_model(self):
        self.drm_manager.imodel_type = 1
        self.drm_manager.bmodified = True
        self.drm_manager.msg += "NARMA model is enabled\n"
        self.update_view()

    def on_action_generate_drm(self):
        dat = dict()
        dat['input_list'] = self.drm_manager.input_list_sel
        dat['output_list'] = self.drm_manager.output_list_sel
        if self.drm_manager.imodel_type == 0:
            dat['dabnet_input'] = self.drm_manager.dabnet_input
            dlg = DABNetParamDlg(dat)
            result = dlg.exec_()
            if result==1:
                self.drm_manager.dabnet_input = dlg.dat['dabnet_input']
                QApplication.setOverrideCursor(QCursor(Qt.WaitCursor))
                self.bthread = True
                self.update_view()
                thd = threading.Thread(target=self.drm_manager.generate_drm)
                thd.start()
                # wait until the thread finishes
                thd.join()
                self.bthread = False
                QApplication.restoreOverrideCursor()
                self.drm_manager.bbuilt_dabnet = True
                self.drm_manager.bpredict_train = False
                self.drm_manager.bpredict_valid = False
                self.drm_manager.brun_uq = False
                self.drm_manager.bmodified = True
                self.drm_manager.msg += "DABNet DRM has been generated\n"
                self.update_view()
        else:
            dat['narma_input'] = self.drm_manager.narma_input
            dlg = NARMAParamDlg(dat)
            result = dlg.exec_()
            if result==1:
                self.drm_manager.narma_input = dlg.dat['narma_input']
                QApplication.setOverrideCursor(QCursor(Qt.WaitCursor))
                self.bthread = True
                self.update_view()
                thd = threading.Thread(target=self.drm_manager.generate_drm)
                thd.start()
                # wait until the thread finishes
                thd.join()
                self.bthread = False
                QApplication.restoreOverrideCursor()
                self.drm_manager.bbuilt_narma = True
                self.drm_manager.bpredict_train = False
                self.drm_manager.bpredict_valid = False
                self.drm_manager.brun_uq = False
                self.drm_manager.bmodified = True
                self.drm_manager.msg += "NARMAR DRM has been generated\n"
                self.update_view()

    def on_action_use_balanced_model_for_dabnet(self):
        if self.actionUse_Balanced_Model_For_Prediction.isChecked():
            self.drm_manager.ipredict_opt_dabnet = 1
            self.drm_manager.msg += "Use balanced DABNet model for prediction.\n"
        else:
            self.drm_manager.ipredict_opt_dabnet = 0
            self.drm_manager.msg += "Use Laguerre DABNet model for prediction.\n"
        self.drm_manager.bmodified = True
        self.update_view()

    def on_action_use_hfm_history_for_narma(self):
        if self.actionUse_High_Fidelity_Model_History_For_Prediction.isChecked():
            self.drm_manager.ipredict_opt_narma = 0
            self.drm_manager.msg += "Use high-fidelity history data for NARMAR model prediction.\n"
        else:
            self.drm_manager.ipredict_opt_narma = 1
            self.drm_manager.msg += "Use NARMAR model history data for prediction.\n"
        self.drm_manager.bmodified = True
        self.update_view()

    def on_action_predict_training(self):
        # no threading
        self.drm_manager.predict_train()
        self.drm_manager.bpredict_train = True
        self.drm_manager.bmodified = True
        self.drm_manager.msg += "Response to the training sequence is predicted.\n"
        self.update_view()

    def on_action_predict_validation(self):
        # no threading
        self.drm_manager.predict_valid()
        self.drm_manager.bpredict_valid = True
        self.drm_manager.bmodified = True
        self.drm_manager.msg += "Response to the validation sequence is predicted.\n"
        self.update_view()

    def on_action_plot_training(self):
        dat = dict()
        dat['input_list'] = self.drm_manager.input_list_all
        dat['output_list'] = self.drm_manager.output_list_all
        dat['plotting_input'] = self.drm_manager.plotting_input
        dlg = PlotResultDlg(dat)
        result = dlg.exec_()
        if result==1:
            self.drm_manager.plotting_input = dlg.dat['plotting_input']
            plt_inp = self.drm_manager.plotting_input
            input_list_all = self.drm_manager.input_list_all
            output_list_all = self.drm_manager.output_list_all
            acm_result = self.drm_manager.acm_result_train
            drm_result = self.drm_manager.drm_prediction_train
            nu = len(plt_inp.input_index_list)
            ny = len(plt_inp.output_index_list)
            ninput_all = len(input_list_all)
            npair = len(acm_result[0])
            ncol = max(nu, ny)
            nrow = 1 + plt_inp.bplot_error + plt_inp.bplot_step_change + plt_inp.bplot_correlation
            dt = self.drm_manager.acm_input.dt_sampling
            tmax = dt*(npair-1)
            t = np.linspace(0, tmax, npair)
            plt.figure(1)
            # plot response of ACM and D-RM result
            for i in xrange(ny):
                i1 = i + 1
                plt.subplot(nrow, ncol, i1)
                ioutput_all = plt_inp.output_index_list[i]
                ioutput_sel = self.drm_manager.get_selected_output_index_from_index_of_all(ioutput_all)
                if ioutput_sel >= 0:
                    lines = plt.plot(t, acm_result[ninput_all + ioutput_all], t, drm_result[ioutput_sel])
                    plt.setp(lines[1], color='r')
                else:
                    plt.plot(t, acm_result[ninput_all + ioutput_all])
                lbl_x = 'Time [' + self.drm_manager.acm_input.unit_time + ']'
                plt.xlabel(lbl_x)
                lbl_y = output_list_all[ioutput_all].key_sinter + '\n[' + output_list_all[ioutput_all].unit + ']'
                plt.ylabel(lbl_y)
                plt.xlim(0, tmax)
                plt.grid(True)
            nsubplot = ncol
            if plt_inp.bplot_error: #plot relative error
                for i in xrange(ny):
                    i1 = i + 1
                    ioutput_all = plt_inp.output_index_list[i]
                    ioutput_sel = self.drm_manager.get_selected_output_index_from_index_of_all(ioutput_all)
                    if ioutput_sel >= 0:
                        plt.subplot(nrow, ncol, nsubplot + i1)
                        err_relative = [None]*npair
                        y_drm = drm_result[ioutput_sel]
                        y_acm = acm_result[ninput_all + ioutput_all]
                        for j in xrange(npair):
                            if y_acm[j] > 1.0e-20 or y_acm[j] < -1.0e-20:
                                err_relative[j] = (y_drm[j] - y_acm[j])/y_acm[j]
                            else:
                                err_relative[j] = 0.0
                        plt.plot(t, err_relative)
                        lbl_x = 'Time [' + self.drm_manager.acm_input.unit_time + ']'
                        plt.xlabel(lbl_x)
                        lbl_y = 'Normalized Error of\n' + output_list_all[ioutput_all].key_sinter
                        plt.ylabel(lbl_y)
                        plt.xlim(0, tmax)
                        plt.grid(True)
                nsubplot += ncol
            if plt_inp.bplot_step_change:   # plot input step changes
                for i in xrange(nu):
                    i1 = i + 1
                    iinput_all = plt_inp.input_index_list[i]
                    plt.subplot(nrow, ncol, nsubplot + i1)
                    plt.plot(t, acm_result[iinput_all], drawstyle='steps')
                    lbl_x = 'Time [' + self.drm_manager.acm_input.unit_time + ']'
                    plt.xlabel(lbl_x)
                    lbl_y = input_list_all[iinput_all].key_sinter + '\n[' + input_list_all[iinput_all].unit + ']'
                    plt.ylabel(lbl_y)
                    plt.xlim(0, tmax)
                    ymin = min(acm_result[iinput_all])
                    ymax = max(acm_result[iinput_all])
                    yrange = ymax - ymin
                    if ymin == ymax:
                        yrange = 0.5*abs(ymax)
                    plt.ylim(ymin-0.05*yrange, ymax+0.05*yrange)
                    plt.grid(True)
                nsubplot += ncol
            if plt_inp.bplot_correlation:   # plot correlation
                for i in xrange(ny):
                    i1 = i + 1
                    ioutput_all = plt_inp.output_index_list[i]
                    ioutput_sel = self.drm_manager.get_selected_output_index_from_index_of_all(ioutput_all)
                    if ioutput_sel >= 0:
                        plt.subplot(nrow, ncol, nsubplot + i1)
                        lbl = r'$R^2=$ ' + '{:.5f}'.format(self.drm_manager.rsquared_train[i])
                        plt.scatter(acm_result[ninput_all + ioutput_all], drm_result[ioutput_sel], marker='+', label=lbl)
                        lbl_x = output_list_all[ioutput_all].key_sinter + ' (Plant)\n[' + output_list_all[ioutput_all].unit + ']'
                        plt.xlabel(lbl_x)
                        lbl_y = output_list_all[ioutput_all].key_sinter + ' (D-RM)\n[' + output_list_all[ioutput_all].unit + ']'
                        plt.ylabel(lbl_y)
                        plt.legend(loc='upper left', shadow=True)
                        plt.grid(True)
            plt.subplots_adjust(left=0.1, right=0.95, hspace=0.4)
            self.bplot_show = True
            self.update_view()
            plt.show()
            self.bplot_show = False
            self.update_view()

    def on_action_plot_validation(self):
        dat = dict()
        dat['input_list'] = self.drm_manager.input_list_all
        dat['output_list'] = self.drm_manager.output_list_all
        dat['plotting_input'] = self.drm_manager.plotting_input
        dlg = PlotResultDlg(dat)
        result = dlg.exec_()
        if result==1:
            self.drm_manager.plotting_input = dlg.dat['plotting_input']
            plt_inp = self.drm_manager.plotting_input
            input_list_all = self.drm_manager.input_list_all
            output_list_all = self.drm_manager.output_list_all
            acm_result = self.drm_manager.acm_result_valid
            drm_result = self.drm_manager.drm_prediction_valid
            nu = len(plt_inp.input_index_list)
            ny = len(plt_inp.output_index_list)
            ninput_all = len(input_list_all)
            npair = len(acm_result[0])
            ncol = max(nu, ny)
            nrow = 1 + plt_inp.bplot_error + plt_inp.bplot_step_change + plt_inp.bplot_correlation
            dt = self.drm_manager.acm_input.dt_sampling
            tmax = dt*(npair-1)
            t = np.linspace(0, tmax, npair)
            plt.figure(1)
            # plot response of ACM and D-RM result
            for i in xrange(ny):
                i1 = i + 1
                plt.subplot(nrow, ncol, i1)
                ioutput_all = plt_inp.output_index_list[i]
                ioutput_sel = self.drm_manager.get_selected_output_index_from_index_of_all(ioutput_all)
                if ioutput_sel >= 0:
                    lines = plt.plot(t, acm_result[ninput_all + ioutput_all], t, drm_result[ioutput_sel])
                    plt.setp(lines[1], color='r')
                else:
                    plt.plot(t, acm_result[ninput_all + ioutput_all])
                lbl_x = 'Time [' + self.drm_manager.acm_input.unit_time + ']'
                plt.xlabel(lbl_x)
                lbl_y = output_list_all[ioutput_all].key_sinter + '\n[' + output_list_all[ioutput_all].unit + ']'
                plt.ylabel(lbl_y)
                plt.xlim(0, tmax)
                plt.grid(True)
            nsubplot = ncol
            if plt_inp.bplot_error: #plot relative error
                for i in xrange(ny):
                    i1 = i + 1
                    ioutput_all = plt_inp.output_index_list[i]
                    ioutput_sel = self.drm_manager.get_selected_output_index_from_index_of_all(ioutput_all)
                    if ioutput_sel >= 0:
                        plt.subplot(nrow, ncol, nsubplot + i1)
                        err_relative = [None]*npair
                        y_drm = drm_result[ioutput_sel]
                        y_acm = acm_result[ninput_all + ioutput_all]
                        for j in xrange(npair):
                            if y_acm[j] > 1.0e-20 or y_acm[j] < -1.0e-20:
                                err_relative[j] = (y_drm[j] - y_acm[j])/y_acm[j]
                            else:
                                err_relative[j] = 0.0
                        plt.plot(t, err_relative)
                        lbl_x = 'Time [' + self.drm_manager.acm_input.unit_time + ']'
                        plt.xlabel(lbl_x)
                        lbl_y = 'Normalized Error of\n' + output_list_all[ioutput_all].key_sinter
                        plt.ylabel(lbl_y)
                        plt.xlim(0, tmax)
                        plt.grid(True)
                nsubplot += ncol
            if plt_inp.bplot_step_change:   # plot input step changes
                for i in xrange(nu):
                    i1 = i + 1
                    iinput_all = plt_inp.input_index_list[i]
                    plt.subplot(nrow, ncol, nsubplot + i1)
                    plt.plot(t, acm_result[iinput_all], drawstyle='steps')
                    lbl_x = 'Time [' + self.drm_manager.acm_input.unit_time + ']'
                    plt.xlabel(lbl_x)
                    lbl_y = input_list_all[iinput_all].key_sinter + '\n[' + input_list_all[iinput_all].unit + ']'
                    plt.ylabel(lbl_y)
                    plt.xlim(0, tmax)
                    ymin = min(acm_result[iinput_all])
                    ymax = max(acm_result[iinput_all])
                    yrange = ymax - ymin
                    if ymin == ymax:
                        yrange = 0.5*abs(ymax)
                    plt.ylim(ymin-0.05*yrange, ymax+0.05*yrange)
                    plt.grid(True)
                nsubplot += ncol
            if plt_inp.bplot_correlation:   # plot correlation
                for i in xrange(ny):
                    i1 = i + 1
                    ioutput_all = plt_inp.output_index_list[i]
                    ioutput_sel = self.drm_manager.get_selected_output_index_from_index_of_all(ioutput_all)
                    if ioutput_sel >= 0:
                        plt.subplot(nrow, ncol, nsubplot + i1)
                        lbl = r'$R^2=$ ' + '{:.5f}'.format(self.drm_manager.rsquared_valid[i])
                        plt.scatter(acm_result[ninput_all + ioutput_all], drm_result[ioutput_sel], marker='+', label=lbl)
                        lbl_x = output_list_all[ioutput_all].key_sinter + ' (Plant)\n[' + output_list_all[ioutput_all].unit + ']'
                        plt.xlabel(lbl_x)
                        lbl_y = output_list_all[ioutput_all].key_sinter + ' (D-RM)\n[' + output_list_all[ioutput_all].unit + ']'
                        plt.ylabel(lbl_y)
                        plt.legend(loc='upper left', shadow=True)
                        plt.grid(True)
            plt.subplots_adjust(left=0.1, right=0.95, hspace=0.4)
            self.bplot_show = True
            self.update_view()
            plt.show()
            self.bplot_show = False
            self.update_view()

    def on_action_specify_noise(self):
        # check if the number of output variables is updated
        noutput = len(self.drm_manager.output_list_sel)
        if len(self.drm_manager.uq_input.fr_output) != noutput:
            self.drm_manager.uq_input.set_default_output_noise(noutput, 0.01)
        dat = dict()
        dat['fq_state'] = self.drm_manager.uq_input.fq_state
        dat['fr_output'] = self.drm_manager.uq_input.fr_output
        dat['output_list'] = self.drm_manager.output_list_sel
        dlg = NoiseDlg(dat)
        result = dlg.exec_()
        if result==1:
            self.drm_manager.uq_input.fq_state = dlg.dat['fq_state']
            self.drm_manager.uq_input.fr_output = dlg.dat['fr_output']
            self.drm_manager.bnoise_set = True
            self.drm_manager.brun_uq = False
            self.drm_manager.bmodified = True
            self.drm_manager.msg += "Process and measurement noises are specified.\n"
            self.update_view()

    def on_action_analyze_uq(self):
        dat = dict()
        dat['input_list'] = self.drm_manager.input_list_all
        dat['output_list'] = self.drm_manager.output_list_all
        dat['plotting_input'] = self.drm_manager.plotting_input
        dlg = PlotResultDlg(dat)
        result = dlg.exec_()
        if result==1:
            self.drm_manager.plotting_input = dlg.dat['plotting_input']
            # perform uq calculation
            self.drm_manager.perform_uq_analysis()
            self.drm_manager.brun_uq = True
            self.drm_manager.bmodified = True
            self.drm_manager.msg += "UQ analysis has been completed.\n"
            self.update_view()
            # plot uq results
            plt_inp = self.drm_manager.plotting_input
            input_list_all = self.drm_manager.input_list_all
            output_list_all = self.drm_manager.output_list_all
            acm_result = self.drm_manager.acm_result_valid
            drm_result = self.drm_manager.drm_prediction_valid
            ukf_result = self.drm_manager.ukf_prediction_valid
            obs_result = self.drm_manager.ukf_observation_valid
            std_result = self.drm_manager.ukf_std_valid
            ninput_all = len(input_list_all)
            npair = len(acm_result[0])
            nu = len(plt_inp.input_index_list)
            ny = len(plt_inp.output_index_list)
            # calculate UKF lower and upper std curve
            ukf_std_lower = [None]*ny
            ukf_std_upper = [None]*ny
            for i in xrange(ny):
                ukf_std_lower[i] = [None]*npair
                ukf_std_upper[i] = [None]*npair
                ioutput_all = plt_inp.output_index_list[i]
                ioutput_sel = self.drm_manager.get_selected_output_index_from_index_of_all(ioutput_all)
                for j in xrange(npair):
                    ukf_std_lower[i][j] = ukf_result[ioutput_sel][j] - std_result[ioutput_sel][j]
                    ukf_std_upper[i][j] = ukf_result[ioutput_sel][j] + std_result[ioutput_sel][j]
            # number of figure columns and rows
            ncol = max(nu, ny)
            nrow = 1 + plt_inp.bplot_error + plt_inp.bplot_step_change + plt_inp.bplot_correlation
            dt = self.drm_manager.acm_input.dt_sampling
            tmax = dt*(npair-1)
            t = np.linspace(0, tmax, npair)
            plt.figure(1)
            # plot response of observed and UKF response
            for i in xrange(ny):
                i1 = i + 1
                plt.subplot(nrow, ncol, i1)
                ioutput_all = plt_inp.output_index_list[i]
                ioutput_sel = self.drm_manager.get_selected_output_index_from_index_of_all(ioutput_all)
                if ioutput_sel >= 0:
                    lines = plt.plot(t, obs_result[ioutput_sel], t, ukf_result[ioutput_sel], t, ukf_std_lower[i], t, ukf_std_upper[i])
                    plt.setp(lines[0], color='k', label='Measured')
                    plt.setp(lines[1], color='r', label='UKF')
                    plt.setp(lines[2], color='b', label='UKF-STD', linestyle='--')
                    plt.setp(lines[3], color='g', label='UKF+STD', linestyle='--')
                else:
                    plt.plot(t, acm_result[ninput_all + ioutput_all], label='ACM')
                lbl_x = 'Time [' + self.drm_manager.acm_input.unit_time + ']'
                plt.xlabel(lbl_x)
                lbl_y = output_list_all[ioutput_all].key_sinter + '\n[' + output_list_all[ioutput_all].unit + ']'
                plt.ylabel(lbl_y)
                plt.xlim(0, tmax)
                plt.legend(loc='upper right')
                plt.grid(True)
            nsubplot = ncol
            if plt_inp.bplot_error: #plot relative error
                for i in xrange(ny):
                    i1 = i + 1
                    ioutput_all = plt_inp.output_index_list[i]
                    ioutput_sel = self.drm_manager.get_selected_output_index_from_index_of_all(ioutput_all)
                    if ioutput_sel >= 0:
                        plt.subplot(nrow, ncol, nsubplot + i1)
                        err_relative_drm = [None]*npair
                        err_relative_ukf = [None]*npair
                        y_drm = drm_result[ioutput_sel]
                        y_ukf = ukf_result[ioutput_sel]
                        y_acm = acm_result[ninput_all + ioutput_all]
                        for j in xrange(npair):
                            if y_acm[j] > 1.0e-20 or y_acm[j] < -1.0e-20:
                                err_relative_drm[j] = (y_drm[j] - y_acm[j])/y_acm[j]
                                err_relative_ukf[j] = (y_ukf[j] - y_acm[j])/y_acm[j]
                            else:
                                err_relative_drm[j] = 0.0
                                err_relative_ukf[j] = 0.0
                        lines = plt.plot(t, err_relative_drm, t, err_relative_ukf)
                        plt.setp(lines[0], color='k', label='DRM')
                        plt.setp(lines[1], color='r', label='UKF')
                        lbl_x = 'Time [' + self.drm_manager.acm_input.unit_time + ']'
                        plt.xlabel(lbl_x)
                        lbl_y = 'Normalized Error of\n' + output_list_all[ioutput_all].key_sinter
                        plt.ylabel(lbl_y)
                        plt.xlim(0, tmax)
                        plt.legend(loc='upper right')
                        plt.grid(True)
                nsubplot += ncol
            if plt_inp.bplot_step_change:   # plot input step changes
                for i in xrange(nu):
                    i1 = i + 1
                    iinput_all = plt_inp.input_index_list[i]
                    plt.subplot(nrow, ncol, nsubplot + i1)
                    plt.plot(t, acm_result[iinput_all], drawstyle='steps')
                    lbl_x = 'Time [' + self.drm_manager.acm_input.unit_time + ']'
                    plt.xlabel(lbl_x)
                    lbl_y = input_list_all[iinput_all].key_sinter + '\n[' + input_list_all[iinput_all].unit + ']'
                    plt.ylabel(lbl_y)
                    plt.xlim(0, tmax)
                    ymin = min(acm_result[iinput_all])
                    ymax = max(acm_result[iinput_all])
                    yrange = ymax - ymin
                    if ymin == ymax:
                        yrange = 0.5*abs(ymax)
                    plt.ylim(ymin-0.05*yrange, ymax+0.05*yrange)
                    plt.grid(True)
                nsubplot += ncol
            if plt_inp.bplot_correlation:   # plot correlation
                for i in xrange(ny):
                    i1 = i + 1
                    ioutput_all = plt_inp.output_index_list[i]
                    ioutput_sel = self.drm_manager.get_selected_output_index_from_index_of_all(ioutput_all)
                    if ioutput_sel >= 0:
                        plt.subplot(nrow, ncol, nsubplot + i1)
                        lbl_drm = r'$R^2=$ ' + '{:.5f} (DRM)'.format(self.drm_manager.rsquared_valid[i])
                        lbl_ukf = r'$R^2=$ ' + '{:.5f} (UKF)'.format(self.drm_manager.rsquared_valid_ukf[i])
                        plt.scatter(acm_result[ninput_all + ioutput_all], drm_result[ioutput_sel], color='k', marker='+', label=lbl_drm)
                        plt.scatter(acm_result[ninput_all + ioutput_all], ukf_result[ioutput_sel], color='r', marker='+', label=lbl_ukf)
                        lbl_x = output_list_all[ioutput_all].key_sinter + ' (Plant)\n[' + output_list_all[ioutput_all].unit + ']'
                        plt.xlabel(lbl_x)
                        lbl_y = output_list_all[ioutput_all].key_sinter + ' (D-RM)\n[' + output_list_all[ioutput_all].unit + ']'
                        plt.ylabel(lbl_y)
                        plt.legend(loc='upper left', shadow=True)
                        plt.grid(True)
            plt.subplots_adjust(left=0.1, right=0.95, hspace=0.4)
            self.bplot_show = True
            self.update_view()
            plt.show()
            self.bplot_show = False
            self.update_view()

    def update_view(self):
        if self.drm_manager != None:
            self.textMessage.setText(self.drm_manager.msg)
        self.textMessage.verticalScrollBar().setValue(999)  # set to a very large value
        self.actionAbout_D_RM_Builder.setEnabled(True)
        # fix a problem with plt.show() of matplotlib
        if self.bplot_show:
            self.disable_all()
            self.update()
            return
        if self.bthread:
            self.disable_all()
            self.update()
            return
        if self.bclosed:
            self.disable_all_except_open_and_new()
            self.update()
            return
        self.actionNew.setEnabled(True)
        self.actionOpen.setEnabled(True)
        self.actionSave.setEnabled(True)
        self.actionSave_As.setEnabled(True)
        if self.drm_manager.imodel_type==0:
            self.actionD_RM_As_Matlab_Script_File.setEnabled(self.drm_manager.bbuilt_dabnet)
        else:
            self.actionD_RM_As_Matlab_Script_File.setEnabled(self.drm_manager.bbuilt_narma)
        self.actionTraining_Data.setEnabled(self.drm_manager.brun_train)
        self.actionValidation_Data.setEnabled(self.drm_manager.brun_valid)
        self.actionCovariance_Matrices.setEnabled(self.drm_manager.brun_uq)
        self.actionMessages_To_Log_File.setEnabled(True)
        self.actionClose.setEnabled(True)
        self.actionChoose_High_Fidelity_Model.setEnabled(True)
        self.actionConfigure_Input_Variables.setEnabled(self.drm_manager.bhfm_file)
        self.actionConfigure_Output_Variables.setEnabled(self.drm_manager.bhfm_file)
        self.actionPrepare_Training_Sequence.setEnabled(self.drm_manager.binputready and not self.drm_manager.bread_custom_train)
        self.actionPrepare_Validation_Sequence.setEnabled(self.drm_manager.binputready and not self.drm_manager.bread_custom_train)
        self.actionRead_Custom_Training_Data.setEnabled(True)
        self.actionRead_Custom_Validation_Data.setEnabled(self.drm_manager.bread_custom_train)
        self.actionPerform_Training_Simulation.setEnabled(self.drm_manager.bsequence_train)
        self.actionPerform_Validation_Simulation.setEnabled(self.drm_manager.bsequence_valid)
        self.actionDABNet.setEnabled(True)
        self.actionDABNet.setChecked(self.drm_manager.imodel_type==0)
        self.actionNARMA.setEnabled(True)
        self.actionNARMA.setChecked(self.drm_manager.imodel_type==1)
        self.actionGenerate_Reduced_Model.setEnabled(self.drm_manager.brun_train and self.drm_manager.boutputready)
        self.actionUse_Balanced_Model_For_Prediction.setEnabled(self.drm_manager.imodel_type==0)
        self.actionUse_Balanced_Model_For_Prediction.setChecked(self.drm_manager.ipredict_opt_dabnet==1)
        self.actionUse_High_Fidelity_Model_History_For_Prediction.setEnabled(self.drm_manager.imodel_type==1)
        self.actionUse_High_Fidelity_Model_History_For_Prediction.setChecked(self.drm_manager.ipredict_opt_narma==0)
        if self.drm_manager.imodel_type==0:
            self.actionPredict_Traning_Response.setEnabled(self.drm_manager.brun_train and self.drm_manager.bbuilt_dabnet)
            self.actionPredict_Validation_Response.setEnabled(self.drm_manager.brun_valid and self.drm_manager.bbuilt_dabnet)
        else:
            self.actionPredict_Traning_Response.setEnabled(self.drm_manager.brun_train and self.drm_manager.bbuilt_narma)
            self.actionPredict_Validation_Response.setEnabled(self.drm_manager.brun_valid and self.drm_manager.bbuilt_narma)
        self.actionPlot_Training_Response.setEnabled(self.drm_manager.brun_train and self.drm_manager.bpredict_train)
        self.actionPlot_Validation_Response.setEnabled(self.drm_manager.brun_valid and self.drm_manager.bpredict_valid)
        self.actionSpecify_Noise.setEnabled(self.drm_manager.boutputready and self.drm_manager.imodel_type==0)
        self.actionAnalyze.setEnabled(self.drm_manager.brun_valid and self.drm_manager.bpredict_valid and self.drm_manager.bbuilt_dabnet and self.drm_manager.imodel_type==0 and self.drm_manager.bnoise_set)
        self.update()   # paint the window again

    def disable_all_except_open_and_new(self):
        self.disable_all()
        self.actionNew.setEnabled(True)
        self.actionOpen.setEnabled(True)

    def disable_all(self):
        self.actionNew.setEnabled(False)
        self.actionOpen.setEnabled(False)
        self.actionSave.setEnabled(False)
        self.actionSave_As.setEnabled(False)
        self.actionD_RM_As_Matlab_Script_File.setEnabled(False)
        self.actionTraining_Data.setEnabled(False)
        self.actionValidation_Data.setEnabled(False)
        self.actionCovariance_Matrices.setEnabled(False)
        self.actionClose.setEnabled(False)
        self.actionChoose_High_Fidelity_Model.setEnabled(False)
        self.actionConfigure_Input_Variables.setEnabled(False)
        self.actionConfigure_Output_Variables.setEnabled(False)
        self.actionPrepare_Training_Sequence.setEnabled(False)
        self.actionPrepare_Validation_Sequence.setEnabled(False)
        self.actionPerform_Training_Simulation.setEnabled(False)
        self.actionPerform_Validation_Simulation.setEnabled(False)
        self.actionRead_Custom_Training_Data.setEnabled(False)
        self.actionRead_Custom_Validation_Data.setEnabled(False)
        self.actionDABNet.setEnabled(False)
        self.actionNARMA.setEnabled(False)
        self.actionGenerate_Reduced_Model.setEnabled(False)
        self.actionUse_Balanced_Model_For_Prediction.setEnabled(False)
        self.actionUse_High_Fidelity_Model_History_For_Prediction.setEnabled(False)
        self.actionPredict_Traning_Response.setEnabled(False)
        self.actionPredict_Validation_Response.setEnabled(False)
        self.actionPlot_Training_Response.setEnabled(False)
        self.actionPlot_Validation_Response.setEnabled(False)
        self.actionSpecify_Noise.setEnabled(False)
        self.actionAnalyze.setEnabled(False)

    def paintEvent(self, event):
        qp = QPainter()
        qp.begin(self)
        if self.drm_manager == None:
            qp.end()
            return
        if self.drm_manager.binputready or self.drm_manager.boutputready:
            xstart_box = 350
            ystart_box = 100
            xwidth_box = 150
            yheight_box = 150
            xlength_arrow = 100
            qp.drawRect(xstart_box, ystart_box, xwidth_box, yheight_box)
            qp.setBrush(QColor(0, 0, 0))
            font_metrics = QFontMetrics(self.font())
            height_text_half = (font_metrics.ascent() - font_metrics.descent())/2
            if self.drm_manager.binputready:
                ninput = len(self.drm_manager.input_list_sel)
                dy_arrow = yheight_box/(ninput+1)
                i = 1
                for item in self.drm_manager.input_list_sel:
                    xstart_arrow = xstart_box - xlength_arrow
                    xend_arrow = xstart_box
                    ystart_arrow = ystart_box + i*dy_arrow
                    yend_arrow = ystart_arrow
                    self.drawArrowedLine(qp, xstart_arrow, ystart_arrow, xend_arrow, yend_arrow)
                    width_text = font_metrics.width(item.name)
                    qp.drawText(xstart_arrow-width_text-5, ystart_arrow+height_text_half, item.name)
                    i += 1
            if self.drm_manager.boutputready:
                noutput = len(self.drm_manager.output_list_sel)
                dy_arrow = yheight_box/(noutput+1)
                i = 1
                for item in self.drm_manager.output_list_sel:
                    xstart_arrow = xstart_box + xwidth_box
                    xend_arrow = xstart_arrow + xlength_arrow
                    ystart_arrow = ystart_box + i*dy_arrow
                    yend_arrow = ystart_arrow
                    self.drawArrowedLine(qp, xstart_arrow, ystart_arrow, xend_arrow, yend_arrow)
                    qp.drawText(xend_arrow+5, ystart_arrow+height_text_half, item.name)
                    i += 1
        qp.end()

    def drawArrowedLine(self, qp, xstart, ystart, xend, yend):
        qp.drawLine(xstart, ystart, xend, yend)
        polygon = QPolygonF()
        polygon.append(QPointF(xend, yend))
        polygon.append(QPointF(xend-15.0, yend+5.0))
        polygon.append(QPointF(xend-10, yend))
        polygon.append(QPointF(xend-15.0, yend-5.0))
        polygon.append(QPointF(xend, yend))
        qp.drawPolygon(polygon)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    dm = DRMManager()
    form = MainDRMBuilder(dm)
    form.show()
    app.exec_()

