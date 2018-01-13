
#include "CClipboardManagerTray.hpp"

#include <QApplication>
#include <QMessageBox>


int main(int argc, char* argv[])
{
   QApplication a(argc, argv);

   QApplication::setQuitOnLastWindowClosed(false);

   if (!CClipboardManagerTray::isSystemTrayAvailable())
   {
      QMessageBox::critical(
               0,
               QObject::tr("Systray"),
               QObject::tr("I couldn't detect any system tray on this system.")
               );

      return 1;
   }

   CClipboardManagerTray clipboardManager;
   clipboardManager.show();

   return a.exec();
}
