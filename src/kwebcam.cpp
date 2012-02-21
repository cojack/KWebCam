/***************************************************************************
 *   Copyright (C) 2012 by Przemysław Czekaj <xcojack@gmail.com>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "kwebcam.h"
#include "kwebcamview.h"
#include "settings.h"

#include <QtGui/QDropEvent>
#include <QtGui/QPainter>
#include <QtGui/QPrinter>

#include <KConfigDialog>
#include <KStatusBar>

#include <KAction>
#include <KActionCollection>
#include <KStandardAction>

#include <Solid/Device>
#include <Solid/DeviceNotifier>
#include "solid/video.h"

#include <KLocale>

#include <QFile>

KWebCam::KWebCam()
    : KXmlGuiWindow(),
      m_view(new KWebCamView(this)),
      m_printer(0)
{
    // accept dnd
    setAcceptDrops(true);

    // tell the KXmlGuiWindow that this is indeed the main widget
    setCentralWidget(m_view);

    // then, setup our actions
    setupActions();

    // add a status bar
    statusBar()->show();

    // a call to KXmlGuiWindow::setupGUI() populates the GUI
    // with actions, using KXMLGUI.
    // It also applies the saved mainwindow settings, if any, and ask the
    // mainwindow to automatically save settings if changed: window size,
    // toolbar position, icon size, etc.
    setupGUI();
   
    
    Solid::DeviceNotifier *notifier = Solid::DeviceNotifier::instance();
    
    foreach (Solid::Device device, Solid::Device::listFromType(Solid::DeviceInterface::Video, QString()))
    {
        m_videoDevices << device.udi();
        getDetails( device );
    }    
      
    
}

KWebCam::~KWebCam()
{
}

void KWebCam::getDetails( const Solid::Device &dev )
{
    kDebug() << "New video device at " << dev.udi();
    const Solid::Device * vendorDevice = &dev;
    while ( vendorDevice->isValid() && vendorDevice->vendor().isEmpty() )
    {
        vendorDevice = new Solid::Device( vendorDevice->parentUdi() );
    }
    if ( vendorDevice->isValid() )
    {
        kDebug() << "vendor: " << vendorDevice->vendor() << ", product: " << vendorDevice->product();
    }
    QStringList protocols = dev.as<Solid::Video>()->supportedProtocols();
    if ( protocols.contains( "video4linux" ) )
    {
        QStringList drivers = dev.as<Solid::Video>()->supportedDrivers( "video4linux" );
        if ( drivers.contains( "video4linux" ) )
        {
            kDebug() << "V4L device path is" << dev.as<Solid::Video>()->driverHandle( "video4linux" ).toString();
        }
    }
    
    m_view->runVideo(dev.as<Solid::Video>()->driverHandle( "video4linux" ).toString());
}

void KWebCam::deviceAdded( const QString &udi )
{
    Solid::Device dev( udi );
    if( dev.is<Solid::Video>() ) {
        m_videoDevices << udi;
        getDetails( dev );
    }
}

void KWebCam::deviceRemoved(const QString &udi )
{
     Solid::Device dev = Solid::Device( udi );
     int i;
     if ( ( i = m_videoDevices.indexOf( udi ) ) != - 1 ) {
        //kDebug() << udi;
        m_videoDevices.removeAt( i );
     }
}

void KWebCam::frameChanged( int frame )
{

}

void KWebCam::setupActions()
{  
    KStandardAction::openNew(this, SLOT(fileNew()), actionCollection());
    KStandardAction::quit(qApp, SLOT(closeAllWindows()), actionCollection());

    KStandardAction::preferences(this, SLOT(optionsPreferences()), actionCollection());

    // custom menu and menu item - the slot is in the class KWebCamView
    KAction *custom = new KAction(KIcon("colorize"), i18n("Swi&tch Colors"), this);
    actionCollection()->addAction( QLatin1String("switch_action"), custom );
    connect(custom, SIGNAL(triggered(bool)), m_view, SLOT(switchColors()));
}

void KWebCam::fileNew()
{
    //movie.play();
    // this slot is called whenever the File->New menu is selected,
    // the New shortcut is pressed (usually CTRL+N) or the New toolbar
    // button is clicked

    // create a new window
    (new KWebCam)->show();
}

void KWebCam::optionsPreferences()
{
    // The preference dialog is derived from prefs_base.ui
    //
    // compare the names of the widgets in the .ui file
    // to the names of the variables in the .kcfg file
    //avoid to have 2 dialogs shown
    if ( KConfigDialog::showDialog( "settings" ) )  {
        return;
    }
    KConfigDialog *dialog = new KConfigDialog(this, "settings", Settings::self());
    QWidget *generalSettingsDlg = new QWidget;
    ui_prefs_base.setupUi(generalSettingsDlg);
    dialog->addPage(generalSettingsDlg, i18n("General"), "package_setting");
    connect(dialog, SIGNAL(settingsChanged(QString)), m_view, SLOT(settingsChanged()));
    dialog->setAttribute( Qt::WA_DeleteOnClose );
    dialog->show();
}

#include "kwebcam.moc"
