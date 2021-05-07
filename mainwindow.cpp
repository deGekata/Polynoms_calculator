#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helpwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  QPixmap* info_button = new QPixmap(":/Images/Info2.png");
  QIcon info_button_icon(*info_button);
  ui->InfoButton->setIcon(info_button_icon);
  ui->InfoButton->setIconSize(QSize(ui->InfoButton->width() - 2, ui->InfoButton->height() - 2));
  this->setMinimumWidth(this->width());
  this->setMaximumWidth(this->width());
  this->setMaximumHeight(this->height());
  this->setMinimumHeight(this->height());
  connect(ui->DataBase, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(new_selection(int, int)));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_AddPole2_released() {
  QString temp = ui->Form2->text();
  std::string text = temp.toLocal8Bit().constData();
   Polynom poly;
  try {
    poly = parse(text);
    ui->DataBase->insertRow(ui->DataBase->rowCount());
    std::string str = poly.print();
    ui->DataBase->setItem(ui->DataBase->rowCount() - 1, 0,
                          new QTableWidgetItem(QString::fromStdString(str)));
    ui->Messagebox->setPlainText(QString(""));
    ui->Form2->setText(QString(""));
  } catch (const std::exception& ex) {
    ui->Messagebox->setPlainText(QString(ex.what()));
  }

}

void MainWindow::on_AddPole1_released() {
  QString temp = ui->Form1->text();
  std::string text = temp.toLocal8Bit().constData();
  Polynom poly;
  try {
    poly = parse(text);
    ui->DataBase->insertRow(ui->DataBase->rowCount());
    std::string str = poly.print();
    ui->DataBase->setItem(ui->DataBase->rowCount() - 1, 0,
                          new QTableWidgetItem(QString::fromStdString(str)));
    ui->Messagebox->setPlainText(QString(""));
    ui->Form1->setText(QString(""));
  } catch (const std::exception& ex) {
    ui->Messagebox->setPlainText(QString(ex.what()));
  }
}

void MainWindow::unrelease() {
  ui->mult->setDown(false);
  ui->division->setDown(false);
  ui->addition->setDown(false);
  ui->subtr->setDown(false);
  return;
}


void MainWindow::new_selection(int row, int) {
  if (operation == -1) {
    if (selected != -1) {
      ui->DataBase->item(selected, 0)->setFont(QFont("Seqoe UI", 9));
    }
    selected = row;
    ui->DataBase->item(selected, 0)->setFont(QFont("Seqoe UI", 14));
  } else {
    if (selected == -1) {
      ui->Messagebox->setPlainText(QString("Please, select to operands"));
      return;
    }
    /* 0 - *
     * 1 - /
     * 2 - +
     * 3 - -
     */
    unrelease();
    std::string text =
        ui->DataBase->item(selected, 0)->text().toLocal8Bit().constData();
    Polynom first = parse(text);
    text = ui->DataBase->item(row, 0)->text().toLocal8Bit().constData();
    Polynom second = parse(text);
    if ((first.base != second.base) && (first.base != 0 || second.base != 0)) {
      ui->Messagebox->setPlainText(QString("Polynoms have different base"));
    } else
    switch (operation) {
      //*
      case 0: {
        std::string text =
            ui->DataBase->item(selected, 0)->text().toLocal8Bit().constData();
        Polynom first = parse(text);
        text = ui->DataBase->item(row, 0)->text().toLocal8Bit().constData();
        Polynom second = parse(text);
        Polynom result;
        try {
          result = first * second;
          std::string str = result.print();
          ui->Messagebox->setPlainText(QString(""));
          ui->Form1->setText(QString::fromStdString(str));
        } catch (std::exception& ex) {
          ui->Messagebox->setPlainText(QString(ex.what()));
        }
        break;
      }
      // /
      case 1: {
        std::string text =
            ui->DataBase->item(selected, 0)->text().toLocal8Bit().constData();
        Polynom first = parse(text);
        text = ui->DataBase->item(row, 0)->text().toLocal8Bit().constData();
        Polynom second = parse(text);
        std::pair<Polynom*, Polynom*> result;
        try {
          result = first / second;
          std::string str1 = result.first->print();
          std::string str2 = result.second->print();
          ui->Messagebox->setPlainText(QString(""));
          ui->Form1->setText(QString::fromStdString(str1));
          ui->Form2->setText(QString::fromStdString(str2));
        } catch (std::exception& ex) {
          ui->Messagebox->setPlainText(QString(ex.what()));
        }
        break;
      }
      // +
      case 2: {
        std::string text =
            ui->DataBase->item(selected, 0)->text().toLocal8Bit().constData();
        Polynom first = parse(text);
        text = ui->DataBase->item(row, 0)->text().toLocal8Bit().constData();
        Polynom second = parse(text);
        Polynom result;
        try {
          result = first + second;
          std::string str = result.print();
          ui->Messagebox->setPlainText(QString(""));
          ui->Form1->setText(QString::fromStdString(str));
        } catch (std::exception& ex) {
          ui->Messagebox->setPlainText(QString(ex.what()));
        }
        break;
      }
      // -
      case 3: {
        std::string text =
            ui->DataBase->item(selected, 0)->text().toLocal8Bit().constData();
        Polynom first = parse(text);
        text = ui->DataBase->item(row, 0)->text().toLocal8Bit().constData();
        Polynom second = parse(text);
        Polynom result;
        try {
          result = first - second;
          std::string str = result.print();
          ui->Messagebox->setPlainText(QString(""));
          ui->Form1->setText(QString::fromStdString(str));
        } catch (std::exception& ex) {
          ui->Messagebox->setPlainText(QString(ex.what()));
        }
        break;
      }

      default:
        operation = -1;
        break;
    }
    ui->DataBase->item(selected, 0)->setFont(QFont("Seqoe UI", 9));
    selected = -1;
    operation = -1;
  }
}



void MainWindow::on_mult_released() {
  if (selected == -1) {
    ui->Messagebox->setPlainText(QString(
        "Select first operand, press the button and then second operand"));
    ui->mult->setDown(false);
    return;
  }
  if (ui->mult->isDown()) {
    operation = -1;
    unrelease();
    ui->mult->setDown(false);
  } else {
    unrelease();
    ui->mult->setDown(true);
    operation = 0;
  }
  return;
}

void MainWindow::on_division_released() {
  if (selected == -1) {
    ui->Messagebox->setPlainText(QString(
        "Select first operand, press the button and then second operand"));
    ui->division->setDown(false);
    return;
  }
  if (ui->division->isDown()) {
    operation = -1;
    unrelease();
    ui->division->setDown(false);
  } else {
    operation = 1;
    unrelease();
    ui->division->setDown(true);
  }
  return;
}

void MainWindow::on_addition_released() {
  if (selected == -1) {
    ui->Messagebox->setPlainText(QString(
        "Select first operand, press the button and then second operand"));
    ui->addition->setDown(false);
    return;
  }
  if (ui->addition->isDown()) {
    unrelease();
    operation = -1;
    ui->addition->setDown(false);
  } else {
    unrelease();
    ui->addition->setDown(true);
    operation = 2;
  }
  return;
}

void MainWindow::on_subtr_released() {
  if (selected == -1) {
    ui->Messagebox->setPlainText(QString(
        "Select first operand, press the button and then second operand"));
    ui->subtr->setDown(false);
    return;
  }
  if (ui->subtr->isDown()) {
    unrelease();
    operation = -1;
    ui->subtr->setDown(false);
  } else {
    unrelease();
    ui->subtr->setDown(true);
    operation = 3;
  }
  return;
}

void MainWindow::on_deriv_released() {
  if (selected == -1) {
    ui->Messagebox->setPlainText(QString("No selected polynom"));
    ui->mult->setDown(false);
    return;
  }
  unrelease();
  std::string text =
      ui->DataBase->item(selected, 0)->text().toLocal8Bit().constData();
  Polynom pol = parse(text);
  bool ok;
  int cnt = QInputDialog::getInt(
      this, QString::fromUtf8("Input order of the derivative"),
      QString::fromUtf8("Order:"), 0, 0, 10000, 1, &ok);
  if (ok) {
    std::string str;
    if (cnt > pol.back()->power) {
      str = "0";
    } else {
      for (int i = 0; i < cnt; ++i) {
        pol = pol.deriv();
      }
      str = pol.print();
    }
    ui->Messagebox->setPlainText(QString(""));
    ui->Form1->setText(QString::fromStdString(str));
    ui->DataBase->item(selected, 0)->setFont(QFont("Seqoe UI", 9));
    selected = -1;
    return;

  } else {
    ui->Messagebox->setPlainText(QString("Operation cancelled"));
    ui->DataBase->item(selected, 0)->setFont(QFont("Seqoe UI", 9));
    selected = -1;
    return;
  }
}

void MainWindow::on_value_in_released() {
  if (selected == -1) {
    ui->Messagebox->setPlainText(QString("No selected polynom"));
    ui->mult->setDown(false);
    return;
  }
  unrelease();
  std::string text =
      ui->DataBase->item(selected, 0)->text().toLocal8Bit().constData();
  Polynom pol = parse(text);
  bool ok;
  double dot = QInputDialog::getDouble(
      this, QString::fromUtf8("Input value of variable"),
      QString::fromUtf8("Value:"), 0.00, -10000, 10000, 3, &ok);
  if (ok) {
    dot = pol.value_in(dot);
    std::ostringstream strs;
    strs << dot;
    std::string str = strs.str();
    ui->Messagebox->setPlainText(QString(""));
    ui->Form1->setText(QString::fromStdString(str));
    ui->DataBase->item(selected, 0)->setFont(QFont("Seqoe UI", 9));
    selected = -1;
    return;
  } else {
    ui->Messagebox->setPlainText(QString("Operation cancelled"));
    ui->DataBase->item(selected, 0)->setFont(QFont("Seqoe UI", 9));
    selected = -1;
    return;
  }
}

void MainWindow::on_roots_released() {
  if (selected == -1) {
    ui->Messagebox->setPlainText(QString("No selected polynom"));
    ui->mult->setDown(false);
    return;
  }
  unrelease();
  std::string text =
      ui->DataBase->item(selected, 0)->text().toLocal8Bit().constData();
  Polynom pol = parse(text);
  auto roots = pol.roots();
  std::string ans;
  if (roots.first == -1) {
    ans = "No Integer roots";
  } else if (roots.first == 0) {
    ans = "Infinite quantity of roots";
  } else if (roots.second.size() != 0) {
    ans = "Roots: ";
    for (auto elem : roots.second) {
      std::ostringstream strs;
      strs << elem;
      ans += strs.str() + " ";
    }
  } else {
    ans = "No roots";
  }

  ui->Messagebox->setPlainText(QString(""));
  ui->Form1->setText(QString::fromStdString(ans));
  ui->DataBase->item(selected, 0)->setFont(QFont("Seqoe UI", 9));
  selected = -1;
  return;
}

void MainWindow::on_Int_roots_released() {
  if (selected == -1) {
    ui->Messagebox->setPlainText(QString("No selected polynom"));
    ui->mult->setDown(false);
    return;
  }
  unrelease();
  std::string text =
      ui->DataBase->item(selected, 0)->text().toLocal8Bit().constData();
  Polynom pol = parse(text);
  auto roots = pol.Int_roots();
  std::string ans;
  if (roots.first == -1) {
    ans = "No Integer roots";
  } else if (roots.first == 0) {
    ans = "Infinite quantity of roots";
  } else if (roots.second.size() != 0) {
    ans = "Roots: ";
    for (auto elem : roots.second) {
      std::ostringstream strs;
      strs << elem;
      ans += strs.str() + " ";
    }
  } else {
    ans = "No Integer roots";
  }

  ui->Messagebox->setPlainText(QString(""));
  ui->Form1->setText(QString::fromStdString(ans));
  ui->DataBase->item(selected, 0)->setFont(QFont("Seqoe UI", 9));
  selected = -1;
  return;
}

void MainWindow::on_DelSelect_released() {
  if (selected != -1) {
    ui->DataBase->removeRow(selected);
    unrelease();
    selected = -1;
    operation = -1;
  } else {
    ui->Messagebox->setPlainText(QString("No selected polynom"));
  }
  return;
}

void MainWindow::on_DB_clear_released() {
  ui->DataBase->setRowCount(0);
  selected = -1;
  ui->Form1->clear();
  ui->Form2->clear();
  ui->Messagebox->clear();
  operation = -1;
  return;
}

void MainWindow::on_ClearPoles_released() {
  ui->Form1->clear();
  ui->Form2->clear();
  return;
}

void MainWindow::on_Del_range_released() {
  QList<QTableWidgetSelectionRange> ranges = ui->DataBase->selectedRanges();
  ui->Messagebox->setPlainText(QString(""));
  if (ranges.size() == 0) {
    ui->Messagebox->setPlainText(QString("No polynoms selected"));
  }
  while (ranges.size()) {
    QTableWidgetSelectionRange range = ranges.back();
    int bottom = range.bottomRow();
    for (int i = 0; i < range.rowCount(); ++i) {
      ui->DataBase->removeRow(bottom - i);
    }
    ranges.pop_back();
  }
  unrelease();
  selected = -1;
  operation = -1;
  return;
}



void MainWindow::on_FileImport_released() {
  QUrl filepath = QFileDialog::getOpenFileUrl(
      this, tr("Select txt file"),
      QStandardPaths::writableLocation(QStandardPaths::DataLocation));
  QString name = filepath.url();
  if (int(name.toStdString().rfind(".txt")) == int(name.size() - 4)) {
    Polynom mypol;
    std::ifstream file(name.toStdString().substr(8));
    int line = 0;
    qDebug() << file.is_open() << "   "
             << QString::fromStdString(name.toStdString().substr(8));
    while (!file.eof()) {
      std::string inp;
      std::getline(file, inp);
      ++line;
      if (inp.length() == 0) {
        continue;
      }
      try {
        mypol = parse(inp);
        ui->DataBase->insertRow(ui->DataBase->rowCount());
        std::string str = mypol.print();
        ui->DataBase->setItem(
            ui->DataBase->rowCount() - 1, 0,
            new QTableWidgetItem(QString::fromStdString(str)));
      } catch (const std::exception& ex) {
        QString text = ui->Messagebox->toPlainText() +
                       QString("Error on line ") +
                       QString::fromStdString(std::to_string(line)) +
                       QString(": ") + QString(ex.what()) + QString("\n");

        ui->Messagebox->setPlainText(text);
      }
    }
    qDebug() << "yep";
  } else {
    ui->Messagebox->setPlainText(QString("Incorrect type of file"));
  }
  return;
}

void MainWindow::on_InfoButton_released() {
  HelpWindow secondwindow;
  secondwindow.setModal(true);
  secondwindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
  secondwindow.exec();
}
