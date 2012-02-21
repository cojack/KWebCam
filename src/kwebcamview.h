/***************************************************************************
 *   Copyright (C) %{CURRENT_YEAR} by %{AUTHOR} <%{EMAIL}>                            *
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

#ifndef KWEBCAMVIEW_H
#define KWEBCAMVIEW_H

#include <QtGui/QWidget>
#include <QtGui/QMovie>

#include "ui_kwebcamview_base.h"

class QPainter;
class KUrl;
class QAbstractVideoSurface;

/**
 * This is the main view class for KWebCam.  Most of the non-menu,
 * non-toolbar, and non-statusbar (e.g., non frame) GUI code should go
 * here.
 *
 * @short Main view
 * @author %{AUTHOR} <%{EMAIL}>
 * @version %{VERSION}
 */

class KWebCamView : public QWidget, public Ui::kwebcamview_base
{
    Q_OBJECT
public:
    /**
     * Default constructor
     */
    KWebCamView(QWidget *parent);

    /**
     * Destructor
     */
    virtual ~KWebCamView();

    void runVideo(QString);
    
private:
    bool presentImage(const QImage &image);
  
    Ui::kwebcamview_base ui_kwebcamview_base;
    QMovie movie;
    QAbstractVideoSurface *surface;
    QAbstractButton *playButton;
    QSlider *positionSlider;
    
signals:
    /**
     * Use this signal to change the content of the statusbar
     */
    void signalChangeStatusbar(const QString& text);

    /**
     * Use this signal to change the content of the caption
     */
    void signalChangeCaption(const QString& text);
    
private slots:
    void switchColors();
    void settingsChanged();
    void setBrightness(int);
    void setContrast(int);
    void setHue(int);
    void setSaturation(int);
    void takeSnapShot();
};

#endif // KWebCamVIEW_H
