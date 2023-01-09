#include "salarios.h"
#include "ui_salarios.h"


Salarios::Salarios(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Salarios)
{
    ui->setupUi(this);
    m_contolador = new Controlador();
}

Salarios::~Salarios()
{
    delete ui;
}


void Salarios::on_btnCalcular_clicked()
{
    calcular();
}


void Salarios::on_actionNuevo_triggered()
{
    // Limpiar widgets
    limpiar();
    // Limpiar el texto de los calculos
    ui->outCalculos->clear();
    // Mostrar mensaje en la barra de estado
    ui->statusbar->showMessage("Nuevos cálculos de salario.", 3000);
}

void Salarios::limpiar()
{
    // Limpiar widgets
    ui->inNombre->setText("");
    ui->inHoras->setValue(0);
    ui->inMatutina->setChecked(true);
    ui->inNombre->setFocus();
}

void Salarios::calcular()
{
    // Obtener datos de la GUI
    QString nombre = ui->inNombre->text();
    int horas = ui->inHoras->value();
    TipoJornada jornada;
    if(ui->inMatutina->isChecked()){
        jornada = TipoJornada::Matituna;
    }else if (ui->inVespertina->isChecked()){
        jornada = TipoJornada::Vespertina;
    }else{
        jornada = TipoJornada::Nocturna;
    }
    // Validacion de datos
    if(nombre == "" || horas == 0){
        QMessageBox::warning(this,"Advertencia","El nombre y/o el número de horas es incorrecto");
        return;
    }

    // Establecer datos al controlador
    m_contolador->setDatos(nombre, horas, jornada);
    // Calcular salarios
    if (m_contolador->calcular()) {
        // Muestra los resultados
        ui->outCalculos->appendPlainText(m_contolador->getDatos());
    } else {
        QMessageBox::critical(
                    this,
                    "Error",
                    "No se puede calcular el salario.");
    }
    // Limpiar widgets
    limpiar();
    // Mostrar mensaje en la barra de estado
    ui->statusbar->showMessage("Salario de " + nombre + " calculado.",5000);

}


void Salarios::on_actionCalcular_triggered()
{
    calcular();
}


void Salarios::on_actionSalir_triggered()
{
    this->close();
}


void Salarios::on_actionGuardar_triggered()
{
    // Abrir un cuadro de diálogo para seleccionar el path y archivo a guardar
    QString nombreArchivo = QFileDialog::getSaveFileName(this,
                                                   "Guardar calculos de salarios",
                                                   QDir::home().absolutePath() + "/salarios.sal",
                                                   "Aplicacion Salarios(*.sal)");
    // Crear un objeto File
    QFile archivo(nombreArchivo);
    // Tartar de abrir para escritura
    if(archivo.open(QFile::WriteOnly | QFile::Truncate)){
        // cRear un objeto 'stream' de texto
        QTextStream salida(&archivo);
        // Enviar los datos del resultado a la salida
        salida << ui->outCalculos->toPlainText();
        // Mostrar mensaje en la barra de estados
        ui->statusbar->showMessage("Datos guardados en: " + nombreArchivo, 5000);
        // Cerrar el archivo
        archivo.close();
    }else {
        // Mensaje de error
        QMessageBox::warning(this,
                             "Guardar archivo",
                             "No se puede acceder al archivo para guardar los datos.");
    }


}

void Salarios::on_actionAbrir_triggered()
{
    QString salarios = QFileDialog::getOpenFileName(this,
                                                    "Leer calculos de salarios",
                                                    QDir::home().absolutePath(),
                                                    "Aplicacion Salarios (*.sal)");
    QFile archivo(salarios);
    int tam = archivo.size();
    if (tam > 0){
        QMessageBox messageBox(this);
        messageBox.addButton(tr("Conservar"), QMessageBox::AcceptRole);
        messageBox.addButton(tr("Descartar"), QMessageBox::RejectRole);
        messageBox.setText("Desea conservar los datos calculados?");
        int ret = messageBox.exec();
        if(ret == QMessageBox::Accepted)
            ui->outCalculos->clear();
        QTextStream in(&archivo);
        if(archivo.open(QFile::ReadOnly)){
            ui->outCalculos->appendPlainText(qPrintable(in.readAll()));
        }else {
            // Mensaje de error
            QMessageBox::warning(this,
                                 "Leer archivo",
                                 "No se puede acceder al archivo para leer los datos.");
        }
        ui->statusbar->showMessage("Se ha cargado el archivo correctamente.",3000);
        archivo.close();
    } else {
        QMessageBox::warning(this,
                             "Abrir Archivo",
                             "Error el archivo no contiene datos");
    }

}

void Salarios::on_actionAcerca_de_triggered()
{
    // Crear un objeto del cuadro de diálogo
    Acerca *dialog = new Acerca(this);
    // Enviar datos a la otra ventana
    dialog->setVersion(VERSION);
    // Mostrar la venta en modo MODAL
    dialog->exec();
    // Luego de cerrar la ventana, se acceden a los datos de la misma
    qDebug() << dialog->valor();

}

