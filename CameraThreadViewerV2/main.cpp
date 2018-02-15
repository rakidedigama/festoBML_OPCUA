
#include <QApplication>
#include "camerathreadviewergui.h"
#include "QConsoleDebugStream.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_ConsoleDebugStream logStream(std::cout,"log_CameraThreadViewer");
    CameraThreadViewerGUI w;
    w.showMaximized();
    
    return a.exec();
}
