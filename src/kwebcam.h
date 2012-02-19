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

#ifndef KWEBCAM_H
#define KWEBCAM_H


#include <KXmlGuiWindow>
#include <QtGui/QMovie>

#include "ui_prefs_base.h"

class KWebCamView;
class QPrinter;
class KToggleAction;
class KUrl;
class QMovie;

namespace Solid
{
    class Device;
}

/**
 * This class serves as the main window for KWebCam.  It handles the
 * menus, toolbars and status bars.
 *
 * @short Main window class
 * @author %{AUTHOR} <%{EMAIL}>
 * @version %{VERSION}
 */
class KWebCam : public KXmlGuiWindow
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    KWebCam();

    /**
     * Default Destructor
     */
    virtual ~KWebCam();

protected:
    void getDetails( const Solid::Device & );
    
private slots:
    void fileNew();
    void optionsPreferences();
    void deviceAdded( const QString & );
    void deviceRemoved( const QString & );
    void frameChanged( int );
    
private:
    void setupActions();
    bool presentImage( const QImage & );
    
private:
    Ui::prefs_base ui_prefs_base ;
    KWebCamView *m_view;

    QPrinter   *m_printer;
    KToggleAction *m_toolbarAction;
    KToggleAction *m_statusbarAction;
    QStringList m_videoDevices;
    QMovie *m_movie;
};

#endif // _KWEBCAM_H_
