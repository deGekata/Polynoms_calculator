#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QVector>
#include <polynom.h>
#include <exception>
#include <QPushButton>
#include <QDebug>
#include <QtGui>
#include <QInputDialog>
#include <QFileDialog>
#include <sstream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  int selected = -1;
  int operation = -1;
  bool flag = true;

 private slots:
  void on_AddPole1_released();

  void on_AddPole2_released();

  void new_selection(int, int);

  void unrelease();

  void on_mult_released();

  void on_division_released();

  void on_addition_released();

  void on_subtr_released();

  void on_deriv_released();

  void on_value_in_released();

  void on_roots_released();

  void on_Int_roots_released();

  void on_DelSelect_released();

  void on_DB_clear_released();

  void on_ClearPoles_released();

  void on_Del_range_released();

  void on_FileImport_released();

  void on_InfoButton_released();

 private:
  Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H
