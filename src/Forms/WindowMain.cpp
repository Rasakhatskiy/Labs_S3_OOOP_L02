#include <memory>

#include "WindowMain.hpp"
#include "ui_WindowMain.h"
#include "windowmetadata.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_button_refreshDrives_clicked();
    ui->progressBar->setHidden(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_button_refreshDrives_clicked()
{
    ui->comboBox_selectDrive->clear();
    _listStorageInfo.clear();
    for (auto &storage : QStorageInfo::mountedVolumes())
    {
        if (storage.isValid() &&
            storage.isReady() &&
            !storage.isReadOnly())
        {
            _listStorageInfo.append(storage);
            ui->comboBox_selectDrive->addItem(
                storage.rootPath() +
                " " +
                QString::number(storage.bytesTotal() / 1024 / 1024 / 1024) +
                " GB");
        }
    }
}

void MainWindow::on_button_openExplorer_clicked()
{
    auto index = ui->comboBox_selectDrive->currentIndex();
    if (index >= 0 &&
        index < _listStorageInfo.size())
    {
        QProcess myProcess(this);
        QString exe = "explorer";

        QStringList arguments;
        auto path = _listStorageInfo[index].rootPath();
        path[path.size() - 1] = '\\';
        arguments << path;

        myProcess.start(exe, arguments);
        myProcess.waitForFinished(-1);
    }
}

void MainWindow::on_button_browseSaveAs_clicked()
{
    QFileDialog dialog_saveAs(this);
    dialog_saveAs.setFileMode(QFileDialog::AnyFile);
    dialog_saveAs.setDirectory("D:\\");
    dialog_saveAs.setWindowTitle("Save as");
    dialog_saveAs.setNameFilter(tr("Binary file (*.bin *.data *.img)"));
    dialog_saveAs.setViewMode(QFileDialog::Detail);

    QString resultPath =
            dialog_saveAs.exec() ?
            dialog_saveAs.selectedFiles()[0] :
            NULL;

    auto sourcePath = isSourceValid();

    if (sourcePath == NULL ||
        resultPath == NULL)
        return;


    if (QFile::exists(resultPath))
    {
        if (QMessageBox::warning(
                    this,
                    "Invalid input",
                    "File " + resultPath + " already exists. Overwrite it?",
                    QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
            QFile::remove(resultPath);
        else
            return;
    }

    auto infoSaver = InfoSaver();
    infoSaver.scan(sourcePath);
    infoSaver.save(resultPath);
    auto window = new WindowTree(infoSaver.toQStringList(), this);
    window->show();
}

void MainWindow::on_button_browseModifyFile_clicked()
{

}

void MainWindow::on_button_startCreateFilesystem_clicked()
{

}

QString MainWindow::isSourceValid()
{
    // Check if something if index valid. It must be valid
    auto index = ui->comboBox_selectDrive->currentIndex();
    if (index < 0 ||
        index >= _listStorageInfo.size())
        throw std::invalid_argument("Index " + std::to_string(index) + " does not exists");

    // Check if this drive exists.
    auto sourcePath = _listStorageInfo[index].rootPath();
    sourcePath[sourcePath.size() - 1] = '\\';
    if (!QFileInfo(sourcePath).exists())
    {
        QMessageBox::warning(
            this,
            "Invalid input",
            "Directory " + sourcePath + " does not exists");
        return NULL;
    }

    return sourcePath;
}

void MainWindow::on_button_startEditMetadata_clicked()
{
    QFileDialog dialog_saveAs(this);
    dialog_saveAs.setFileMode(QFileDialog::ExistingFile);
    dialog_saveAs.setDirectory("D:\\");
    dialog_saveAs.setWindowTitle("Open  file");
    dialog_saveAs.setNameFilter(tr("Any file (*)"));
    dialog_saveAs.setViewMode(QFileDialog::Detail);
    if (!dialog_saveAs.exec())
        return;
    auto path = dialog_saveAs.selectedFiles()[0];
    auto modifyWindow = /*std::unique_ptr<WindowMetadata>(*/new WindowMetadata(path)/*)*/;
    modifyWindow->show();
}
